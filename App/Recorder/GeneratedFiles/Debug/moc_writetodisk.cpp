/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'writetodisk.h'
**
** Created: Thu Sep 26 13:46:49 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/writetodisk.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'writetodisk.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WriteToDisk[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
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
     180,   12,   12,   12, 0x08,
     210,   12,   12,   12, 0x08,
     226,   12,   12,   12, 0x08,
     245,   12,   12,   12, 0x08,
     270,   12,   12,   12, 0x08,
     299,   12,   12,   12, 0x08,
     322,   12,   12,   12, 0x08,
     347,   12,   12,   12, 0x08,
     365,   12,   12,   12, 0x08,
     383,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WriteToDisk[] = {
    "WriteToDisk\0\0cmbLocationName_indexChanged(int)\0"
    "cmbRoomName_indexChanged(int)\0"
    "cmbSearchCategory_indexChanged(int)\0"
    "txtSearch_textChanged(QString)\0,\0"
    "eventDetails_cellClicked(int,int)\0"
    "tblEventDetails_cellChanged()\0"
    "btnAddClicked()\0btnRemoveClicked()\0"
    "listSessionItemClicked()\0"
    "copySessionListItemClicked()\0"
    "cbxOpticalChanged(int)\0cbxRemovableChanged(int)\0"
    "btnBrowsClicked()\0btnWriteClicked()\0"
    "showAvailability(int)\0"
};

const QMetaObject WriteToDisk::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_WriteToDisk,
      qt_meta_data_WriteToDisk, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WriteToDisk::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WriteToDisk::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WriteToDisk::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WriteToDisk))
        return static_cast<void*>(const_cast< WriteToDisk*>(this));
    return QDialog::qt_metacast(_clname);
}

int WriteToDisk::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 5: tblEventDetails_cellChanged(); break;
        case 6: btnAddClicked(); break;
        case 7: btnRemoveClicked(); break;
        case 8: listSessionItemClicked(); break;
        case 9: copySessionListItemClicked(); break;
        case 10: cbxOpticalChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: cbxRemovableChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: btnBrowsClicked(); break;
        case 13: btnWriteClicked(); break;
        case 14: showAvailability((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
