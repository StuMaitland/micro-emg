/****************************************************************************
** Meta object code from reading C++ file 'auxdigoutconfigdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/auxdigoutconfigdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'auxdigoutconfigdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AuxDigOutConfigDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   23,   22,   22, 0x08,
      55,   23,   22,   22, 0x08,
      80,   23,   22,   22, 0x08,
     105,   23,   22,   22, 0x08,
     138,  130,   22,   22, 0x08,
     163,  130,   22,   22, 0x08,
     188,  130,   22,   22, 0x08,
     213,  130,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AuxDigOutConfigDialog[] = {
    "AuxDigOutConfigDialog\0\0enable\0"
    "enablePortAChanged(bool)\0"
    "enablePortBChanged(bool)\0"
    "enablePortCChanged(bool)\0"
    "enablePortDChanged(bool)\0channel\0"
    "channelPortAChanged(int)\0"
    "channelPortBChanged(int)\0"
    "channelPortCChanged(int)\0"
    "channelPortDChanged(int)\0"
};

void AuxDigOutConfigDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AuxDigOutConfigDialog *_t = static_cast<AuxDigOutConfigDialog *>(_o);
        switch (_id) {
        case 0: _t->enablePortAChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->enablePortBChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->enablePortCChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->enablePortDChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->channelPortAChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->channelPortBChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->channelPortCChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->channelPortDChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AuxDigOutConfigDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AuxDigOutConfigDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AuxDigOutConfigDialog,
      qt_meta_data_AuxDigOutConfigDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AuxDigOutConfigDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AuxDigOutConfigDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AuxDigOutConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AuxDigOutConfigDialog))
        return static_cast<void*>(const_cast< AuxDigOutConfigDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int AuxDigOutConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
