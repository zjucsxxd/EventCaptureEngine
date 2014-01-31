/****************************************************************************
** Meta object code from reading C++ file 'ftpmanager.h'
**
** Created: Thu Sep 26 16:47:54 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/ftpmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FTPManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   11,   28,   11, 0x08,
      54,   11,   11,   11, 0x08,
      69,   11,   11,   11, 0x08,
     102,   86,   11,   11, 0x08,
     139,  131,   11,   11, 0x08,
     171,  159,   11,   11, 0x08,
     205,   11,   11,   11, 0x08,
     218,   11,   11,   11, 0x08,
     248,  227,   11,   11, 0x08,
     296,  290,   11,   11, 0x08,
     310,   11,   11,   11, 0x08,
     333,   11,   11,   11, 0x08,
     346,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FTPManager[] = {
    "FTPManager\0\0hideFTPbutton()\0int\0"
    "connectOrDisconnect()\0downloadFile()\0"
    "cancelDownload()\0commandId,error\0"
    "ftpCommandFinished(int,bool)\0urlInfo\0"
    "addToList(QUrlInfo)\0item,column\0"
    "processItem(QTreeWidgetItem*,int)\0"
    "cdToParent()\0Upload()\0readBytes,totalBytes\0"
    "updateDataTransferProgress(qint64,qint64)\0"
    "error\0ftpdone(bool)\0enableDownloadButton()\0"
    "uploadNext()\0deleteFileFromFTP()\0"
};

const QMetaObject FTPManager::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FTPManager,
      qt_meta_data_FTPManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FTPManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FTPManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FTPManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FTPManager))
        return static_cast<void*>(const_cast< FTPManager*>(this));
    return QDialog::qt_metacast(_clname);
}

int FTPManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: hideFTPbutton(); break;
        case 1: { int _r = connectOrDisconnect();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: downloadFile(); break;
        case 3: cancelDownload(); break;
        case 4: ftpCommandFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: addToList((*reinterpret_cast< const QUrlInfo(*)>(_a[1]))); break;
        case 6: processItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: cdToParent(); break;
        case 8: Upload(); break;
        case 9: updateDataTransferProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 10: ftpdone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: enableDownloadButton(); break;
        case 12: uploadNext(); break;
        case 13: deleteFileFromFTP(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void FTPManager::hideFTPbutton()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
