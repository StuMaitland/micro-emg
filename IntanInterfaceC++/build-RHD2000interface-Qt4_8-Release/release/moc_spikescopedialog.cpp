/****************************************************************************
** Meta object code from reading C++ file 'spikescopedialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/spikescopedialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spikescopedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SpikeScopeDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x08,
      42,   18,   17,   17, 0x08,
      62,   17,   17,   17, 0x08,
      85,   18,   17,   17, 0x08,
     103,   17,   17,   17, 0x08,
     116,   18,   17,   17, 0x08,
     143,  137,   17,   17, 0x08,
     168,   18,   17,   17, 0x08,
     189,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SpikeScopeDialog[] = {
    "SpikeScopeDialog\0\0index\0changeYScale(int)\0"
    "setTriggerType(int)\0resetThresholdToZero()\0"
    "setNumSpikes(int)\0clearScope()\0"
    "setDigitalInput(int)\0value\0"
    "setVoltageThreshold(int)\0setEdgePolarity(int)\0"
    "applyToAll()\0"
};

void SpikeScopeDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SpikeScopeDialog *_t = static_cast<SpikeScopeDialog *>(_o);
        switch (_id) {
        case 0: _t->changeYScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setTriggerType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->resetThresholdToZero(); break;
        case 3: _t->setNumSpikes((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->clearScope(); break;
        case 5: _t->setDigitalInput((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setVoltageThreshold((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->setEdgePolarity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->applyToAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SpikeScopeDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SpikeScopeDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SpikeScopeDialog,
      qt_meta_data_SpikeScopeDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SpikeScopeDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SpikeScopeDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SpikeScopeDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SpikeScopeDialog))
        return static_cast<void*>(const_cast< SpikeScopeDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SpikeScopeDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
