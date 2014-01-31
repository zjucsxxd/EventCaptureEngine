/****************************************************************************
** Meta object code from reading C++ file 'uiloginuser.h'
**
** Created: Thu Sep 26 13:46:59 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/uiloginuser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uiloginuser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LoginUser[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      28,   10,   10,   10, 0x08,
      46,   10,   10,   10, 0x08,
      62,   10,   10,   10, 0x08,
      91,   10,   10,   10, 0x08,
     111,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LoginUser[] = {
    "LoginUser\0\0btnLogin_click()\0"
    "btnCancel_click()\0btnMore_click()\0"
    "cmbLocation_indexChange(int)\0"
    "readSavedPassword()\0clearUserPswrdEdit(QString)\0"
};

const QMetaObject LoginUser::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LoginUser,
      qt_meta_data_LoginUser, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LoginUser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LoginUser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LoginUser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoginUser))
        return static_cast<void*>(const_cast< LoginUser*>(this));
    return QDialog::qt_metacast(_clname);
}

int LoginUser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnLogin_click(); break;
        case 1: btnCancel_click(); break;
        case 2: btnMore_click(); break;
        case 3: cmbLocation_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: readSavedPassword(); break;
        case 5: clearUserPswrdEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
