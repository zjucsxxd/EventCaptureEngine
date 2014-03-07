/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'textedit.h'
**
** Created: Thu Sep 26 13:46:59 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/textedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TextEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      28,    9,    9,    9, 0x05,
      48,    9,    9,    9, 0x05,
      71,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      90,    9,    9,    9, 0x0a,
     104,    9,    9,    9, 0x0a,
     120,    9,    9,    9, 0x0a,
     139,    9,    9,    9, 0x0a,
     163,    9,    9,    9, 0x0a,
     172,    9,    9,    9, 0x0a,
     187,    9,    9,    9, 0x08,
     229,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TextEdit[] = {
    "TextEdit\0\0currentBold(bool)\0"
    "currentItalic(bool)\0currentUnderline(bool)\0"
    "cursorStatus(bool)\0setBold(bool)\0"
    "setItalic(bool)\0setUnderline(bool)\0"
    "getSelectionTxtStatus()\0getTxt()\0"
    "setColorText()\0currentCharFormatChanged(QTextCharFormat)\0"
    "currentSelectionTextStatus(bool)\0"
};

const QMetaObject TextEdit::staticMetaObject = {
    { &SpellTextEdit::staticMetaObject, qt_meta_stringdata_TextEdit,
      qt_meta_data_TextEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextEdit))
        return static_cast<void*>(const_cast< TextEdit*>(this));
    return SpellTextEdit::qt_metacast(_clname);
}

int TextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SpellTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: currentItalic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: currentUnderline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: cursorStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: setBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: setItalic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: setUnderline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: getSelectionTxtStatus(); break;
        case 8: getTxt(); break;
        case 9: setColorText(); break;
        case 10: currentCharFormatChanged((*reinterpret_cast< const QTextCharFormat(*)>(_a[1]))); break;
        case 11: currentSelectionTextStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void TextEdit::currentBold(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextEdit::currentItalic(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TextEdit::currentUnderline(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TextEdit::cursorStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/