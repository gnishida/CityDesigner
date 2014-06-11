/****************************************************************************
** Meta object code from reading C++ file 'ControlWidget.h'
**
** Created: Fri May 30 10:32:13 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ControlWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ControlWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      36,   14,   14,   14, 0x0a,
      61,   14,   14,   14, 0x0a,
      87,   14,   14,   14, 0x0a,
     117,   14,   14,   14, 0x0a,
     135,   14,   14,   14, 0x0a,
     143,   14,   14,   14, 0x0a,
     156,   14,   14,   14, 0x0a,
     168,   14,   14,   14, 0x0a,
     183,   14,   14,   14, 0x0a,
     199,   14,   14,   14, 0x0a,
     227,   14,   14,   14, 0x0a,
     252,   14,   14,   14, 0x0a,
     275,  266,   14,   14, 0x0a,
     300,  266,   14,   14, 0x0a,
     320,   14,   14,   14, 0x0a,
     345,   14,   14,   14, 0x0a,
     361,   14,   14,   14, 0x0a,
     380,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ControlWidget[] = {
    "ControlWidget\0\0generateRoadsPatch()\0"
    "generateRoadsPatchWarp()\0"
    "generateRoadsPatchWarp2()\0"
    "generateRoadsVerySmoothWarp()\0"
    "generateRoadsPM()\0clear()\0mergeRoads()\0"
    "trimRoads()\0connectRoads()\0connectRoads2()\0"
    "removeIntersectionOnRiver()\0"
    "removeIntersectingEdge()\0planarGraph()\0"
    "newValue\0updateTerrainLabels(int)\0"
    "updateRender2D(int)\0changeTerrainShader(int)\0"
    "smoothTerrain()\0contentDesign(int)\0"
    "contentDesignLevel()\0"
};

void ControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ControlWidget *_t = static_cast<ControlWidget *>(_o);
        switch (_id) {
        case 0: _t->generateRoadsPatch(); break;
        case 1: _t->generateRoadsPatchWarp(); break;
        case 2: _t->generateRoadsPatchWarp2(); break;
        case 3: _t->generateRoadsVerySmoothWarp(); break;
        case 4: _t->generateRoadsPM(); break;
        case 5: _t->clear(); break;
        case 6: _t->mergeRoads(); break;
        case 7: _t->trimRoads(); break;
        case 8: _t->connectRoads(); break;
        case 9: _t->connectRoads2(); break;
        case 10: _t->removeIntersectionOnRiver(); break;
        case 11: _t->removeIntersectingEdge(); break;
        case 12: _t->planarGraph(); break;
        case 13: _t->updateTerrainLabels((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->updateRender2D((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->changeTerrainShader((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->smoothTerrain(); break;
        case 17: _t->contentDesign((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->contentDesignLevel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ControlWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ControlWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_ControlWidget,
      qt_meta_data_ControlWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ControlWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ControlWidget))
        return static_cast<void*>(const_cast< ControlWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int ControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
