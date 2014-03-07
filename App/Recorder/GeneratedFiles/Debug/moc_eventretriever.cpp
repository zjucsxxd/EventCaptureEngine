/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'eventretriever.h'
**
** Created: Thu Sep 26 16:48:04 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/eventretriever.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventretriever.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EventRetriever[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      31,   15,   15,   15, 0x05,
      49,   15,   15,   15, 0x05,
      69,   15,   15,   15, 0x05,
      87,   15,   15,   15, 0x05,
     105,   15,   15,   15, 0x05,
     123,   15,   15,   15, 0x05,
     145,   15,   15,   15, 0x05,
     173,   15,   15,   15, 0x05,
     192,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
     213,   15,   15,   15, 0x0a,
     225,   15,   15,   15, 0x0a,
     241,   15,   15,   15, 0x0a,
     256,   15,   15,   15, 0x0a,
     272,   15,   15,   15, 0x0a,
     288,   15,   15,   15, 0x0a,
     306,   15,   15,   15, 0x0a,
     327,   15,   15,   15, 0x0a,
     345,   15,   15,   15, 0x0a,
     364,   15,   15,   15, 0x0a,
     390,   15,   15,   15, 0x08,
     409,  407,   15,   15, 0x08,
     445,  407,   15,   15, 0x08,
     480,   15,   15,   15, 0x08,
     500,   15,   15,   15, 0x08,
     524,   15,   15,   15, 0x08,
     545,   15,   15,   15, 0x08,
     565,   15,   15,   15, 0x08,
     582,   15,   15,   15, 0x08,
     599,   15,   15,   15, 0x08,
     613,   15,   15,   15, 0x08,
     622,  407,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EventRetriever[] = {
    "EventRetriever\0\0FindBold(bool)\0"
    "FindItalics(bool)\0FindUnderLine(bool)\0"
    "chkRBStatus(bool)\0chkRIStatus(bool)\0"
    "chkRUStatus(bool)\0retrieveCellClicked()\0"
    "retrieveCellDoubleClicked()\0"
    "retreiveRowPress()\0textHighlightColor()\0"
    "exportPdf()\0exportWordDoc()\0printPreview()\0"
    "printDocument()\0checkBold(bool)\0"
    "checkItalic(bool)\0checkUnderline(bool)\0"
    "btnPlus_Clicked()\0btnMinus_Clicked()\0"
    "getSelection_Status(bool)\0updateAllNotes()\0"
    ",\0eventRetriever_cellChanged(int,int)\0"
    "eventRetriver_cellClicked(int,int)\0"
    "exportToPdf_click()\0exportToWordDoc_click()\0"
    "exportToHtml_click()\0exportToCsv_click()\0"
    "print(QPrinter*)\0enableSearchGo()\0"
    "closeSearch()\0search()\0"
    "eventRetriver_cellDoubleClicked(int,int)\0"
};

const QMetaObject EventRetriever::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EventRetriever,
      qt_meta_data_EventRetriever, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EventRetriever::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EventRetriever::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EventRetriever::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EventRetriever))
        return static_cast<void*>(const_cast< EventRetriever*>(this));
    return QWidget::qt_metacast(_clname);
}

int EventRetriever::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: FindBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: FindItalics((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: FindUnderLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: chkRBStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: chkRIStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: chkRUStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: retrieveCellClicked(); break;
        case 7: retrieveCellDoubleClicked(); break;
        case 8: retreiveRowPress(); break;
        case 9: textHighlightColor(); break;
        case 10: exportPdf(); break;
        case 11: exportWordDoc(); break;
        case 12: printPreview(); break;
        case 13: printDocument(); break;
        case 14: checkBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: checkItalic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: checkUnderline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: btnPlus_Clicked(); break;
        case 18: btnMinus_Clicked(); break;
        case 19: getSelection_Status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: updateAllNotes(); break;
        case 21: eventRetriever_cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: eventRetriver_cellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: exportToPdf_click(); break;
        case 24: exportToWordDoc_click(); break;
        case 25: exportToHtml_click(); break;
        case 26: exportToCsv_click(); break;
        case 27: print((*reinterpret_cast< QPrinter*(*)>(_a[1]))); break;
        case 28: enableSearchGo(); break;
        case 29: closeSearch(); break;
        case 30: search(); break;
        case 31: eventRetriver_cellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void EventRetriever::FindBold(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EventRetriever::FindItalics(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EventRetriever::FindUnderLine(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void EventRetriever::chkRBStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void EventRetriever::chkRIStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void EventRetriever::chkRUStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void EventRetriever::retrieveCellClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void EventRetriever::retrieveCellDoubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void EventRetriever::retreiveRowPress()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void EventRetriever::textHighlightColor()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/