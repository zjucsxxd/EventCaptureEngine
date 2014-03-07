/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'linkdetails.h'
**
** Created: Thu Sep 26 16:47:48 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/linkdetails.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'linkdetails.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LinkDetails[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      31,   12,   12,   12, 0x08,
      50,   12,   12,   12, 0x08,
      68,   66,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LinkDetails[] = {
    "LinkDetails\0\0btnPlus_Clicked()\0"
    "btnMinus_Clicked()\0btnOK_Clicked()\0,\0"
    "tblURL_indexChange(int,int)\0"
};

const QMetaObject LinkDetails::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LinkDetails,
      qt_meta_data_LinkDetails, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LinkDetails::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LinkDetails::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LinkDetails::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LinkDetails))
        return static_cast<void*>(const_cast< LinkDetails*>(this));
    return QDialog::qt_metacast(_clname);
}

int LinkDetails::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnPlus_Clicked(); break;
        case 1: btnMinus_Clicked(); break;
        case 2: btnOK_Clicked(); break;
        case 3: tblURL_indexChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/