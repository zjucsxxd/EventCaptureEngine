/****************************************************************************
** Meta object code from reading C++ file 'referenceimage.h'
**
** Created: Thu Sep 26 13:47:07 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/referenceimage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'referenceimage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReferenceImage[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      32,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ReferenceImage[] = {
    "ReferenceImage\0\0btnOk_Clicked()\0"
    "btnBrowse_Clicked()\0"
};

const QMetaObject ReferenceImage::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ReferenceImage,
      qt_meta_data_ReferenceImage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ReferenceImage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ReferenceImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ReferenceImage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReferenceImage))
        return static_cast<void*>(const_cast< ReferenceImage*>(this));
    return QDialog::qt_metacast(_clname);
}

int ReferenceImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnOk_Clicked(); break;
        case 1: btnBrowse_Clicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
