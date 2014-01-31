/****************************************************************************
** Meta object code from reading C++ file 'wizard.h'
**
** Created: Thu Sep 26 13:46:49 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/wizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Wizard[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      30,    7,    7,    7, 0x08,
      51,    7,    7,    7, 0x08,
      73,    7,    7,    7, 0x08,
      94,    7,    7,    7, 0x08,
     116,    7,    7,    7, 0x08,
     135,    7,    7,    7, 0x08,
     149,    7,    7,    7, 0x08,
     168,    7,    7,    7, 0x08,
     214,  201,    7,    7, 0x08,
     246,  201,    7,    7, 0x08,
     278,    7,    7,    7, 0x08,
     300,    7,    7,    7, 0x08,
     317,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Wizard[] = {
    "Wizard\0\0btnRegisterPC_click()\0"
    "btnVideoPlus_click()\0btnVideoMinus_click()\0"
    "btnAudioPlus_click()\0btnAudioMinus_click()\0"
    "btnConnect_click()\0btnOk_click()\0"
    "btnRestore_click()\0cmbLocationName_indexChange(int)\0"
    "row_,column_\0videotable_indexChange(int,int)\0"
    "audiotable_indexChange(int,int)\0"
    "wizardTabChanged(int)\0text_PCChanged()\0"
    "cmbRoomName_indexChange(int)\0"
};

const QMetaObject Wizard::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Wizard,
      qt_meta_data_Wizard, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Wizard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Wizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Wizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Wizard))
        return static_cast<void*>(const_cast< Wizard*>(this));
    return QDialog::qt_metacast(_clname);
}

int Wizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnRegisterPC_click(); break;
        case 1: btnVideoPlus_click(); break;
        case 2: btnVideoMinus_click(); break;
        case 3: btnAudioPlus_click(); break;
        case 4: btnAudioMinus_click(); break;
        case 5: btnConnect_click(); break;
        case 6: btnOk_click(); break;
        case 7: btnRestore_click(); break;
        case 8: cmbLocationName_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: videotable_indexChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: audiotable_indexChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: wizardTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: text_PCChanged(); break;
        case 13: cmbRoomName_indexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
