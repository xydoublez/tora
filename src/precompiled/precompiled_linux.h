
// QT headers first
#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QCompleter>
#include <QDateEdit>
#include <QDesktopWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDirModel>
#include <QDockWidget>
#include <QFileDialog>
#include <QFontDialog>
#include <QFontMetrics>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QItemDelegate>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListView>
#include <QListWidget>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QProgressDialog>
#include <QPushButton>
#include <QRadioButton>
#include <Qsci/qsciabstractapis.h>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercustom.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qsciprinter.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscistyle.h>
#include <QScrollArea>
#include <QScrollBar>
#include <QShortcut>
#include <QSlider>
#include <QSortFilterProxyModel>
#include <QSpinBox>
#include <QSplitter>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QStatusBar>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleOptionButton>
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <QTabBar>
#include <QTableView>
#include <QTabWidget>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QAbstractListModel>
#include <QtCore/QAbstractTableModel>
#include <QtCore/QAtomicInt>
#include <QtCore/QByteArray>
#include <QtCore/QChar>
#include <QtCore/QCoreApplication>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDataStream>
#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/qglobal.h>
#include <QtCore/QHash>
#include <QtCore/QLibrary>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QMap>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaType>
#include <QtCore/QMetaType>
#include <QtCore/QMimeData>
#include <QtCore/QModelIndex>
#include <QtCore/QModelIndexList>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QPoint>
#include <QtCore/QPointer>
#include <QtCore/QProcess>
#include <QtCore/QReadWriteLock>
#include <QtCore/QRect>
#include <QtCore/QRegExp>
#include <QtCore/QSemaphore>
#include <QtCore/QSet>
#include <QtCore/QSettings>
#include <QtCore/QSharedPointer>
#include <QtCore/QSignalMapper>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QStringRef>
#include <QtCore/QtAlgorithms>
#include <QtCore/QtDebug>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QtGlobal>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QTimerEvent>
#include <QtCore/QTranslator>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/QWaitCondition>
#include <QtCore/QXmlStreamReader>
#include <QTextBrowser>
#include <QTextEdit>
#include <QtGui/QClipboard>
#include <QtGui/QCloseEvent>
#include <QtGui/QColor>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QFocusEvent>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QHideEvent>
#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMovie>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtGui/QPixmapCache>
#include <QtGui/QPolygon>
#include <QtGui/QResizeEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QTextDocument>
#include <QtNetwork/QHostInfo>
#include <QToolBar>
#include <QToolButton>
#include <QToolTip>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtXml/QDomDocument>
#include <QVBoxLayout>
#include <QWidget>

// STL headers
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <queue>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

// Boost headers
#include <boost/algorithm/string/predicate.hpp>
#include "boost/function.hpp"
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/spirit/include/classic_confix.hpp>
//#include "boost/thread/mutex.hpp"
//#include "boost/thread/thread.hpp"
//#include "boost/thread/xtime.hpp"
#include <boost/throw_exception.hpp>

// Loki headers
#include "loki/Factory_alt.h"
#include <loki/Factory.h>
#include "loki/LokiExport.h"
#include <loki/Singleton.h>
#include "loki/Singleton.h"

// OS headers
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

// Other headers
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// TOra headers
//#include "toresultview.h"
//#include "tomain.h"
//#include "tosql.h"
//#include "toconf.h"
//#include "totool.h"
//#include "toconnection.h"
//#include "utils.h"
//#include "config.h"
//#include "tologger.h"
