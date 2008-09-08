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

#ifndef TOEVENTQUERYTASK_H
#define TOEVENTQUERYTASK_H

#include "config.h"
#include "toconnection.h"
#include "tothread.h"
#include "torunnable.h"

#include <QThread>
#include <QPointer>
#include <QMetaType>
#include <QEvent>
#include <QMutex>

typedef QList<toQValue> ValuesList;
Q_DECLARE_METATYPE(ValuesList);

class toResultStats;


/**
 * Threaded class used by toEventQuery. Internal to toEventQuery only.
 *
 * All signals and calls to slots should be created with
 * Qt::QueuedConnection except to read() and stop()
 *
 */
class toEventQueryTask : public toRunnable {
    Q_OBJECT;

    // the real query object
    QPointer<toQuery> Query;

    // sql and bind parameters
    QString SQL;
    toQList Params;

    int Columns;

    // Statistics to be used if any.
    QPointer<toResultStats> Statistics;

    toConnection *Connection;

    // object was closed
    volatile bool Closed;

    // can't synchronize on query or cancel wouldn't work, but need to
    // protect access during close() which may be called from the main
    // thread
    QMutex CloseLock;


public:

    // keeps thread from exiting until toEventQuery exits. this
    // prevents many race conditions and the caller can always assume
    // task is alive.
    QMutex ThreadAlive;

    toEventQueryTask(QObject *parent,
                     toConnection &conn,
                     const QString &sql,
                     const toQList &param,
                     toResultStats *stats = NULL);
    virtual ~toEventQueryTask();

protected:


    /**
     * Overrides toRunnable::run. Create toRunnableThread to start
     *
     */
    virtual void run(void);


private slots:
    void pread(bool all);
    void timeout(void);


public slots:
    /**
     * Queues a read request. Emits readRequested()
     *
     */
    void read(bool all = false);

    /**
     * Closes this class. Good idea to call this.
     *
     */
    void close(void);


signals:
    // must be careful when defining signals. passing by reference
    // will share memory between threads.


    /**
     * A read was requested
     *
     */
    void readRequested(bool all);


    /**
     * Error message
     *
     */
    void error(const toConnection::exception &msg);


    /**
     * Result headers
     *
     * @param desc list of header values
     * @param columns number of columns in result
     */
    void headers(toQDescList &desc, int columns);


    /**
     * Data read from query
     *
     */
    void data(ValuesList &values);


    /**
     * Emitted when sql query is done
     *
     */
    void done();
};

#endif
