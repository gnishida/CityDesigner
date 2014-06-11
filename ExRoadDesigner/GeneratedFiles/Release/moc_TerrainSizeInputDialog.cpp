/****************************************************************************
** Meta object code from reading C++ file 'TerrainSizeInputDialog.h'
**
** Created: Wed Jun 11 17:33:28 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TerrainSizeInputDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TerrainSizeInputDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TerrainSizeInputDialog[] = {

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
      24,   23,   23,   23, 0x0a,
      31,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TerrainSizeInputDialog[] = {
    "TerrainSizeInputDialog\0\0onOK()\0"
    "onCancel()\0"
};

void TerrainSizeInputDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TerrainSizeInputDialog *_t = static_cast<TerrainSizeInputDialog *>(_o);
        switch (_id) {
        case 0: _t->onOK(); break;
        case 1: _t->onCancel(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TerrainSizeInputDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TerrainSizeInputDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_TerrainSizeInputDialog,
      qt_meta_data_TerrainSizeInputDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TerrainSizeInputDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TerrainSizeInputDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TerrainSizeInputDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TerrainSizeInputDialog))
        return static_cast<void*>(const_cast< TerrainSizeInputDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int TerrainSizeInputDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
