
/* BEGIN_COMMON_COPYRIGHT_HEADER
 *
 * TOra - An Oracle Toolkit for DBA's and developers
 *
 * Shared/mixed copyright is held throughout files in this product
 *
 * Portions Copyright (C) 2000-2001 Underscore AB
 * Portions Copyright (C) 2003-2005 Quest Software, Inc.
 * Portions Copyright (C) 2004-2009 Numerous Other Contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;  only version 2 of
 * the License is valid for this program.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 *      As a special exception, you have permission to link this program
 *      with the Oracle Client libraries and distribute executables, as long
 *      as you follow the requirements of the GNU GPL in regard to all of the
 *      software in the executable aside from Oracle client libraries.
 *
 *      Specifically you are not permitted to link this program with the
 *      Qt/UNIX, Qt/Windows or Qt Non Commercial products of TrollTech.
 *      And you are not permitted to distribute binaries compiled against
 *      these libraries.
 *
 *      You may link this product with any GPL'd Qt library.
 *
 * All trademarks belong to their respective owners.
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "core/toconfiguration.h"
#include "core/tosplash.h"
#include "core/toconnectionprovider.h"
#include "core/tooracleconst.h"
#include "core/tosql.h"
#include "core/toraversion.h"
#include "core/toabout.h"
#include "core/toconf.h"
#include "core/tomain.h"
#include "core/utils.h"

#ifndef Q_OS_WIN32
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <memory>

#include <QtCore/QTextCodec>
#include <QtCore/QString>
#include <QtCore/QTranslator>
#include <QtCore/QLibrary>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtGui/QProgressBar>
#include <QtGui/QStyleFactory>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>

int main(int argc, char **argv)
{
    /* This is probably needed for toWorksheet parser threads
     * otherwise application crashes with:
     * "Fatal IO error 11 (Resource temporarily unavailable) on X server :0"
     */
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads); //  or just XInitThreads();
    toConfiguration::setQSettingsEnv();

    /*! \warning: Keep the code before QApplication init as small
        as possible. There could be serious display issues when
        you construct some Qt classes before QApplication.
        It's the same for global static stuff - some instances can
        break it (e.g. qscintilla lexers etc.).
     */
    QApplication app(argc, argv);

#pragma message WARN( "TODO/FIXME: hicolor theme is broken for Docklet icons. But we need to resolve X11 themes one day" )
    if (QIcon::themeName() == "hicolor")
        QIcon::setThemeName("oxygen");

    try
    {
        QString style(toConfigurationSingle::Instance().style());
        if (!style.isEmpty())
            QApplication::setStyle(QStyleFactory::create(style));

        // Set the default codec to use for QString
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

// qt4        if (getenv("LANG"))
//             qApp->setDefaultCodec(QTextCodec::codecForName(getenv("LANG")));

        QTranslator torats(0);
        QString qmDir = toConfigurationSingle::Instance().sharePath();
        torats.load(qmDir + QString("tora_") + toConfigurationSingle::Instance().locale(), ".");
        qApp->installTranslator(&torats);

        if (toConfigurationSingle::Instance().toadBindings())
        {
            QTranslator toadbindings(0);
            // qt4 - hot candidate for a builtin resource
//             if (!toadbindings.load(tora_toad, sizeof(tora_toad)))
//                 printf("Internal error, couldn't load TOAD bindings");
            toadbindings.load(qmDir + "tora_toad.qm");
            qApp->installTranslator(&toadbindings);
        }

        {
            toSplash splash(NULL);
            splash.show();

            QList<QString> plugins;
            plugins << "parsing";
#ifdef Q_OS_WIN
			QString mysqlHome(toConfigurationSingle::Instance().mysqlHome());
			QDir mysqlHomeDir(mysqlHome);
			QFileInfo mysqllib(mysqlHomeDir, "libmysql.dll");
			if (!mysqlHome.isEmpty() && mysqlHomeDir.exists() && Utils::toLibrary::isValidLibrary(mysqllib))
			{
				QCoreApplication::addLibraryPath(mysqlHome);
				plugins << mysqllib.absoluteFilePath();
			}

			QString pgsqlHome(toConfigurationSingle::Instance().pgsqlHome());
			QDir pgsqlHomeDir(pgsqlHome);
			if (!pgsqlHome.isEmpty() && pgsqlHomeDir.exists()) {
				QCoreApplication::addLibraryPath(pgsqlHome + QDir::separator() + "bin"); // libeay32.dll
				QCoreApplication::addLibraryPath(pgsqlHome + QDir::separator() + "lib");  // libpg.dll
				plugins << pgsqlHome + QDir::separator() + "bin" + QDir::separator() + "LIBICONV-2.DLL";
				plugins << pgsqlHome + QDir::separator() + "bin" + QDir::separator() + "LIBINTL-8.DLL";
				plugins << pgsqlHome + QDir::separator() + "bin" + QDir::separator() + "libeay32.dll";
				plugins << pgsqlHome + QDir::separator() + "bin" + QDir::separator() + "ssleay32.dll";
				plugins << pgsqlHome + QDir::separator() + "lib" + QDir::separator() + "libpq.dll";
			}
#endif

#ifdef PROVIDERS_PATH
			QCoreApplication::addLibraryPath(PROVIDERS_PATH);
#endif

		    // List of all connection provider finders
		    std::vector<std::string> finders = ConnectionProviderFinderFactory::Instance().keys();
		    // Resulting list of all the providers found
		    toProvidersList &allProviders = toProvidersListSing::Instance();

            QProgressBar *progress = splash.progress();
            QLabel *label = splash.label();
            progress->setRange(1, plugins.size() + finders.size()*2);
            qApp->processEvents();
            Q_FOREACH(QString path, plugins)
			{
            	label->setText(qApp->translate("main", "Loading plugin %1").arg(path));
            	qApp->processEvents();
            	bool success = false;
            	try {
            		QLibrary library(path);
            		success = library.load();
            	}
            	TOCATCH;

				if (success)
				{
					label->setText(qApp->translate("main", "Loaded plugin %1").arg(path));
					TLOG(5, toDecoratorNC, __HERE__) << "Loaded plugin " << path << std::endl;
				} else {
					label->setText(qApp->translate("main", "Failed loading plugin %1").arg(path));
					TLOG(5, toDecoratorNC, __HERE__) << "Failed loading plugin " << path << std::endl;
				}
				progress->setValue(progress->value()+1);
				qApp->processEvents();
			}

            // Loop over all finders and call their "find" method, each of them can return several locations
            for(std::vector<std::string>::const_iterator i = finders.begin(); i != finders.end(); ++i)
            {
		    QString finderName(i->c_str());
		    label->setText(qApp->translate("main", "Looking for %1").arg(finderName));
		    qApp->processEvents();
		    TLOG(5, toDecoratorNC, __HERE__) << "Looking for client: " << *i << std::endl;
		    try {
			    std::auto_ptr<toConnectionProviderFinder> finder = ConnectionProviderFinderFactory::Instance().create(*i, 0);
			    QList<toConnectionProviderFinder::ConnectionProvirerParams> l = finder->find();
			    allProviders.append(l);
			    finderName = finder->name();
		    }
		    TOCATCH;
		    progress->setValue(progress->value()+1);
		    qApp->processEvents();
            }

            label->setText(qApp->translate("main", "Examinating Oracle clients..."));
            qApp->processEvents();

            // Loop over all providers found and try to load desired Oracle client
            // 1st try to load requested Oracle client(if set) then load thick(TNS) Oracle client
            QDir oHome = toConfigurationSingle::Instance().oracleHome();
            Q_FOREACH(toConnectionProviderFinder::ConnectionProvirerParams const& params, allProviders)
            {
            	QString providerName = params.value("PROVIDER").toString();
            	if( params.value("PROVIDER").toString() != ORACLE_PROVIDER)
            		continue;
            	QDir pHome(params.value("ORACLE_HOME").toString());
            	if (oHome != pHome)
            		continue;
            	try {
    				label->setText(qApp->translate("main", "Loading provider %1").arg(providerName));
    				qApp->processEvents();
            		TLOG(5, toDecoratorNC, __HERE__) << "Loading: " << params.value("PATH").toString() << std::endl;
            		toConnectionProviderRegistrySing::Instance().load(params);
                    progress->setValue(progress->value()+1);
    				qApp->processEvents();
    				break;
            	}
            	TOCATCH;
            }
            Q_FOREACH(toConnectionProviderFinder::ConnectionProvirerParams const& params, allProviders)
            {
            	QString providerName = params.value("PROVIDER").toString();
            	if( params.value("PROVIDER").toString() != ORACLE_PROVIDER)
            		continue;
            	if ( params.value("IS INSTANT").toBool() == true)
            		continue;
            	if ( toConnectionProviderRegistrySing::Instance().providers().contains(providerName))
            		continue;
            	try {
    				label->setText(qApp->translate("main", "Loading provider %1").arg(providerName));
    				qApp->processEvents();
            		TLOG(5, toDecoratorNC, __HERE__) << "Loading: " << params.value("PATH").toString() << std::endl;
            		QString providerName = params.value("PROVIDER").toString();
            		toConnectionProviderRegistrySing::Instance().load(params);
                    progress->setValue(progress->value()+1);
    				qApp->processEvents();
    				break;
            	} TOCATCH;
            }
            // Load all remaning providers
            Q_FOREACH(toConnectionProviderFinder::ConnectionProvirerParams provider, allProviders)
			{
				QString providerName = provider.value("PROVIDER").toString();
				if (toConnectionProviderRegistrySing::Instance().providers().contains(providerName))
					continue;
				try {
    				label->setText(qApp->translate("main", "Loading provider %1").arg(providerName));
    				qApp->processEvents();
					toConnectionProviderRegistrySing::Instance().load(provider);
	                progress->setValue(progress->value()+1);
	    			qApp->processEvents();
				}
				TOCATCH;
			}

        } // end splash

        try
        {
            toSQL::loadSQL(toConfigurationSingle::Instance().sqlFile());
        }
		catch (QString const& e) {
			TLOG(1, toDecorator, __HERE__) << "	Ignored exception:" << e << std::endl;
		}
        catch (...)
        {
            TLOG(1, toDecorator, __HERE__) << "	Ignored exception." << std::endl;
        }

        if (toConfigurationSingle::Instance().lastVersion() != TORAVERSION)
        {
            std::auto_ptr<toAbout> about ( new toAbout(toAbout::About, NULL, "About " TOAPPNAME, true));
            if (!about->exec())
                exit (2);
            toConfigurationSingle::Instance().setLastVersion(TORAVERSION);
        }


        if (toConfigurationSingle::Instance().firstInstall().isEmpty())
        	toConfigurationSingle::Instance().setFirstInstall(QDateTime::currentDateTime().toString());

        toQValue::setNumberFormat(toConfigurationSingle::Instance().numberFormat()
        		, toConfigurationSingle::Instance().numberDecimals());

        qRegisterMetaType<toQColumnDescriptionList>("toQColumnDescriptionList&");
        qRegisterMetaType<ValuesList>("ValuesList&");
        qRegisterMetaType<toConnection::exception>("toConnection::exception");

        new toMain;

        int ret = qApp->exec();
        return ret;
    }
    catch (QString const& str)
    {
        fprintf(stderr, "Unhandled exception:\n\n%s\n", (const char *) str.toLatin1());
        TOMessageBox::critical(NULL,
                               qApp->translate("main", "Unhandled exception:"),
                               str,
                               qApp->translate("main", "Exit"));
    }
    catch (...)
    {
        fprintf(stderr, "Unhandled exception");
        TOMessageBox::critical(NULL,
                               qApp->translate("main", "Unhandled exception"),
                               "unknown",
                               qApp->translate("main", "Exit"));
    }

    return 1;
}