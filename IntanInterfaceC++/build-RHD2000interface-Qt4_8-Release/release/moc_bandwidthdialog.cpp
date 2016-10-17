/****************************************************************************
** Meta object code from reading C++ file 'bandwidthdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/bandwidthdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bandwidthdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BandwidthDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      48,   41,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BandwidthDialog[] = {
    "BandwidthDialog\0\0onLineEditTextChanged()\0"
    "enable\0onDspCheckBoxChanged(bool)\0"
};

void BandwidthDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BandwidthDialog *_t = static_cast<BandwidthDialog *>(_o);
        switch (_id) {
        case 0: _t->onLineEditTextChanged(); break;
        case 1: _t->onDspCheckBoxChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BandwidthDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BandwidthDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_BandwidthDialog,
      qt_meta_data_BandwidthDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BandwidthDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BandwidthDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BandwidthDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BandwidthDialog))
        return static_cast<void*>(const_cast< BandwidthDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int BandwidthDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
