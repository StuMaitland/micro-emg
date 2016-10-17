/****************************************************************************
** Meta object code from reading C++ file 'triggerrecorddialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/triggerrecorddialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'triggerrecorddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TriggerRecordDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   21,   20,   20, 0x08,
      48,   21,   20,   20, 0x08,
      78,   72,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TriggerRecordDialog[] = {
    "TriggerRecordDialog\0\0index\0"
    "setDigitalInput(int)\0setTriggerPolarity(int)\0"
    "value\0recordBufferSeconds(int)\0"
};

void TriggerRecordDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TriggerRecordDialog *_t = static_cast<TriggerRecordDialog *>(_o);
        switch (_id) {
        case 0: _t->setDigitalInput((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setTriggerPolarity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->recordBufferSeconds((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TriggerRecordDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TriggerRecordDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TriggerRecordDialog,
      qt_meta_data_TriggerRecordDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TriggerRecordDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TriggerRecordDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TriggerRecordDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TriggerRecordDialog))
        return static_cast<void*>(const_cast< TriggerRecordDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int TriggerRecordDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
