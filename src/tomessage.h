/*****
*
* TOra - An Oracle Toolkit for DBA's and developers
* Copyright (C) 2003-2008 Quest Software, Inc
* Portions Copyright (C) 2008 Other Contributors
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
*      these libraries without written consent from Quest Software, Inc.
*      Observe that this does not disallow linking to the Qt Free Edition.
*
*      You may link this product with any GPL'd Qt library such as Qt/Free
*
* All trademarks belong to their respective owners.
*
*****/

#ifndef TOMESSAGE_H
#define TOMESSAGE_H

#include "ui_tomessageui.h"


/*! \brief Non-modal and non-blocking message dialog for
errors/warnings reporting.

It's used in toMain as a class member. This dialog is never
deleted. All messages are appended into end of text-log.

I've rewrote the old-modal-dialog to prevent TOra blocking
(100% CPU) on huge amount of error messages - e.g. when
I run the Server Tuning tool under user with only few grants
on system objects I had to kill the whole desktop to recover.

It takes about 4% CPU in this implementattion.

TODO: IMHO is this dialog a hot candidate for "docking" window

\author Petr Vanek <petr@scribus.info>
*/
class toMessage : public QDialog, public Ui::toMessageUI
{
    Q_OBJECT

    public:
        toMessage(QWidget * parent = 0, Qt::WindowFlags f = 0);

    public slots:
        /*! \brief Appends text to the log widget.
        The dialog is shown too when it's hidden.
        */
        void appendText(const QString & text);

    protected:
        //! Save the prefs
        void hideEvent(QHideEvent * event);

};

#endif
