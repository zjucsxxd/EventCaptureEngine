/*Owner & Copyrights: Vance King Saxbe. A.*//* Copyright (c) <2014> Author Vance King Saxbe. A, and contributors Power Dominion Enterprise, Precieux Consulting and other contributors. Modelled, Architected and designed by Vance King Saxbe. A. with the geeks from GoldSax Consulting and GoldSax Technologies email @vsaxbe@yahoo.com. Development teams from Power Dominion Enterprise, Precieux Consulting. Project sponsored by GoldSax Foundation, GoldSax Group and executed by GoldSax Manager.*//****************************************************************************
** Meta object code from reading C++ file 'videowid.h'
**
** Created: Thu Sep 26 13:46:51 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Common/DSCORE/videowid.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videowid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VideoWid[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      25,    9,    9,    9, 0x05,
      42,   40,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,    9,    9,    9, 0x08,
      73,    9,    9,    9, 0x08,
      82,   40,    9,    9, 0x0a,
     127,    9,    9,    9, 0x0a,
     155,    9,    9,    9, 0x0a,
     183,    9,    9,    9, 0x0a,
     205,    9,    9,    9, 0x0a,
     227,    9,    9,    9, 0x0a,
     240,    9,    9,    9, 0x0a,
     251,    9,    9,    9, 0x0a,
     270,  263,    9,    9, 0x0a,
     287,   40,    9,    9, 0x0a,
     318,    9,  313,    9, 0x0a,
     329,    9,  313,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_VideoWid[] = {
    "VideoWid\0\0beginRestart()\0finished(bool)\0"
    ",\0audioLevel(int,int)\0timerTic()\0"
    "signal()\0widget_Position(action_status,VideoWidgets*)\0"
    "toMainwidget(VideoWidgets*)\0"
    "toFullScreen(VideoWidgets*)\0"
    "toHide(VideoWidgets*)\0toShow(VideoWidgets*)\0"
    "finish(bool)\0gStarted()\0pause(bool)\0"
    "status\0fullScreen(bool)\0"
    "setChannelvolume(int,int)\0bool\0"
    "runGraph()\0stopGraph()\0"
};

const QMetaObject VideoWid::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VideoWid,
      qt_meta_data_VideoWid, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VideoWid::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VideoWid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VideoWid::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VideoWid))
        return static_cast<void*>(const_cast< VideoWid*>(this));
    return QWidget::qt_metacast(_clname);
}

int VideoWid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: beginRestart(); break;
        case 1: finished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: audioLevel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: timerTic(); break;
        case 4: signal(); break;
        case 5: widget_Position((*reinterpret_cast< action_status(*)>(_a[1])),(*reinterpret_cast< VideoWidgets*(*)>(_a[2]))); break;
        case 6: toMainwidget((*reinterpret_cast< VideoWidgets*(*)>(_a[1]))); break;
        case 7: toFullScreen((*reinterpret_cast< VideoWidgets*(*)>(_a[1]))); break;
        case 8: toHide((*reinterpret_cast< VideoWidgets*(*)>(_a[1]))); break;
        case 9: toShow((*reinterpret_cast< VideoWidgets*(*)>(_a[1]))); break;
        case 10: finish((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: gStarted(); break;
        case 12: pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: fullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: setChannelvolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: { bool _r = runGraph();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = stopGraph();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void VideoWid::beginRestart()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void VideoWid::finished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoWid::audioLevel(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/