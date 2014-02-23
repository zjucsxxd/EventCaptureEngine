/* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'eventeditor.h'
**
** Created: Thu Sep 26 16:48:05 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/eventeditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'eventeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EventEditor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      28,   12,   12,   12, 0x05,
      46,   12,   12,   12, 0x05,
      66,   12,   12,   12, 0x05,
      83,   12,   12,   12, 0x05,
     100,   12,   12,   12, 0x05,
     117,   12,   12,   12, 0x05,
     128,   12,   12,   12, 0x05,
     145,   12,   12,   12, 0x05,
     168,   12,   12,   12, 0x05,
     184,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     196,   12,   12,   12, 0x0a,
     212,   12,   12,   12, 0x0a,
     230,   12,   12,   12, 0x0a,
     251,   12,   12,   12, 0x0a,
     271,   12,   12,   12, 0x0a,
     297,   12,   12,   12, 0x0a,
     327,   12,   12,   12, 0x08,
     346,  344,   12,   12, 0x08,
     379,  344,   12,   12, 0x08,
     412,   12,   12,   12, 0x08,
     429,   12,   12,   12, 0x08,
     443,   12,   12,   12, 0x08,
     452,   12,   12,   12, 0x08,
     470,   12,   12,   12, 0x08,
     489,  344,   12,   12, 0x08,
     529,  344,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EventEditor[] = {
    "EventEditor\0\0FindBold(bool)\0"
    "FindItalics(bool)\0FindUnderLine(bool)\0"
    "chkBStatus(bool)\0chkIStatus(bool)\0"
    "chkUStatus(bool)\0keyPress()\0"
    "editorRowPress()\0editorRowDoublePress()\0"
    "findselection()\0textColor()\0checkBold(bool)\0"
    "checkItalic(bool)\0checkUnderline(bool)\0"
    "gotReplace(QString)\0getSelection_Status(bool)\0"
    "action_Text_Color_selection()\0"
    "updateAllNotes()\0,\0eventEditor_cellChanged(int,int)\0"
    "eventEditor_cellClicked(int,int)\0"
    "enableSearchGo()\0closeSearch()\0search()\0"
    "btnPlus_Clicked()\0btnMinus_Clicked()\0"
    "eventEditor_cellDoubledClicked(int,int)\0"
    "eventEditor_cellPressed(int,int)\0"
};

const QMetaObject EventEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EventEditor,
      qt_meta_data_EventEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EventEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EventEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EventEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EventEditor))
        return static_cast<void*>(const_cast< EventEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int EventEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: FindBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: FindItalics((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: FindUnderLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: chkBStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: chkIStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: chkUStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: keyPress(); break;
        case 7: editorRowPress(); break;
        case 8: editorRowDoublePress(); break;
        case 9: findselection(); break;
        case 10: textColor(); break;
        case 11: checkBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: checkItalic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: checkUnderline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: gotReplace((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: getSelection_Status((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: action_Text_Color_selection(); break;
        case 17: updateAllNotes(); break;
        case 18: eventEditor_cellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: eventEditor_cellClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: enableSearchGo(); break;
        case 21: closeSearch(); break;
        case 22: search(); break;
        case 23: btnPlus_Clicked(); break;
        case 24: btnMinus_Clicked(); break;
        case 25: eventEditor_cellDoubledClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 26: eventEditor_cellPressed((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 27;
    }
    return _id;
}

// SIGNAL 0
void EventEditor::FindBold(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EventEditor::FindItalics(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EventEditor::FindUnderLine(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void EventEditor::chkBStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void EventEditor::chkIStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void EventEditor::chkUStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void EventEditor::keyPress()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void EventEditor::editorRowPress()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void EventEditor::editorRowDoublePress()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void EventEditor::findselection()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void EventEditor::textColor()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}
QT_END_MOC_NAMESPACE
