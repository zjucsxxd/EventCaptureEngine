/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'exploresession.h'
**
** Created: Thu Sep 26 16:48:02 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/exploresession.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'exploresession.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExploreSession[] = {

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
      16,   15,   15,   15, 0x08,
      50,   15,   15,   15, 0x08,
      80,   15,   15,   15, 0x08,
     116,   15,   15,   15, 0x08,
     149,  147,   15,   15, 0x08,
     183,  147,   15,   15, 0x08,
     220,   15,   15,   15, 0x08,
     244,   15,   15,   15, 0x08,
     268,   15,   15,   15, 0x08,
     293,   15,   15,   15, 0x08,
     318,   15,   15,   15, 0x08,
     341,   15,   15,   15, 0x08,
     363,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ExploreSession[] = {
    "ExploreSession\0\0cmbLocationName_indexChanged(int)\0"
    "cmbRoomName_indexChanged(int)\0"
    "cmbSearchCategory_indexChanged(int)\0"
    "txtSearch_textChanged(QString)\0,\0"
    "eventDetails_cellClicked(int,int)\0"
    "tblEventDetails_cellChanged(int,int)\0"
    "btnStartEvent_clicked()\0btnCloseEvent_clicked()\0"
    "btnReopenEvent_clicked()\0"
    "btnResumeEvent_clicked()\0"
    "btnViewNotes_clicked()\0btnAutoFill_clicked()\0"
    "validateDescription()\0"
};

const QMetaObject ExploreSession::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ExploreSession,
      qt_meta_data_ExploreSession, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExploreSession::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExploreSession::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExploreSession::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExploreSession))
        return static_cast<void*>(const_cast< ExploreSession*>(this));
    return QDialog::qt_metacast(_clname);
}

int ExploreSession::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 6: btnStartEvent_clicked(); break;
        case 7: btnCloseEvent_clicked(); break;
        case 8: btnReopenEvent_clicked(); break;
        case 9: btnResumeEvent_clicked(); break;
        case 10: btnViewNotes_clicked(); break;
        case 11: btnAutoFill_clicked(); break;
        case 12: validateDescription(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
