/****************************************************************************
** Meta object code from reading C++ file 'explorecase.h'
**
** Created: Thu Sep 26 16:48:03 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/explorecase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'explorecase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExploreCase[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      47,   12,   12,   12, 0x08,
      77,   12,   12,   12, 0x08,
     113,   12,   12,   12, 0x08,
     146,  144,   12,   12, 0x08,
     180,  144,   12,   12, 0x08,
     217,   12,   12,   12, 0x08,
     235,   12,   12,   12, 0x08,
     259,   12,   12,   12, 0x08,
     283,   12,   12,   12, 0x08,
     308,   12,   12,   12, 0x08,
     333,   12,   12,   12, 0x08,
     356,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ExploreCase[] = {
    "ExploreCase\0\0cmbLocationName_indexChanged(int)\0"
    "cmbRoomName_indexChanged(int)\0"
    "cmbSearchCategory_indexChanged(int)\0"
    "txtSearch_textChanged(QString)\0,\0"
    "eventDetails_cellClicked(int,int)\0"
    "tblEventDetails_cellChanged(int,int)\0"
    "btnMore_clicked()\0btnStartEvent_clicked()\0"
    "btnCloseEvent_clicked()\0"
    "btnReopenEvent_clicked()\0"
    "btnResumeEvent_clicked()\0"
    "btnViewNotes_clicked()\0validateDescription()\0"
};

const QMetaObject ExploreCase::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ExploreCase,
      qt_meta_data_ExploreCase, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExploreCase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExploreCase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExploreCase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExploreCase))
        return static_cast<void*>(const_cast< ExploreCase*>(this));
    return QDialog::qt_metacast(_clname);
}

int ExploreCase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cmbLocationName_indexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: cmbRoomName_indexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: cmbSearchCategory_indexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: txtSearch_textChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: eventDetails_cellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: tblEventDetails_cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: btnMore_clicked(); break;
        case 7: btnStartEvent_clicked(); break;
        case 8: btnCloseEvent_clicked(); break;
        case 9: btnReopenEvent_clicked(); break;
        case 10: btnResumeEvent_clicked(); break;
        case 11: btnViewNotes_clicked(); break;
        case 12: validateDescription(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE