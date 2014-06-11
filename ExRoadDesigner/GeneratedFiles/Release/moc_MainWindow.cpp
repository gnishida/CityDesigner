/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Wed Jun 11 17:33:28 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,
      43,   11,   11,   11, 0x0a,
      59,   11,   11,   11, 0x0a,
      73,   11,   11,   11, 0x0a,
      86,   11,   11,   11, 0x0a,
     100,   11,   11,   11, 0x0a,
     115,   11,   11,   11, 0x0a,
     129,   11,   11,   11, 0x0a,
     145,   11,   11,   11, 0x0a,
     160,   11,   11,   11, 0x0a,
     175,   11,   11,   11, 0x0a,
     191,   11,   11,   11, 0x0a,
     206,   11,   11,   11, 0x0a,
     221,   11,   11,   11, 0x0a,
     241,   11,   11,   11, 0x0a,
     258,   11,   11,   11, 0x0a,
     279,   11,   11,   11, 0x0a,
     300,   11,   11,   11, 0x0a,
     321,   11,   11,   11, 0x0a,
     349,   11,   11,   11, 0x0a,
     371,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0onNewTerrain()\0onLoadTerrain()\0"
    "onSaveTerrain()\0onLoadRoads()\0"
    "onAddRoads()\0onSaveRoads()\0onClearRoads()\0"
    "onSaveImage()\0onSaveImageHD()\0"
    "onLoadCamera()\0onSaveCamera()\0"
    "onResetCamera()\0onGenerate2D()\0"
    "onGenerate3D()\0onGenerate3DRoads()\0"
    "onDisplayRoads()\0onRenderingDefault()\0"
    "onRenderingTexture()\0onRenderingGroupBy()\0"
    "onRenderingGenerationType()\0"
    "onShowControlWidget()\0onShowPropertyWidget()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->onNewTerrain(); break;
        case 1: _t->onLoadTerrain(); break;
        case 2: _t->onSaveTerrain(); break;
        case 3: _t->onLoadRoads(); break;
        case 4: _t->onAddRoads(); break;
        case 5: _t->onSaveRoads(); break;
        case 6: _t->onClearRoads(); break;
        case 7: _t->onSaveImage(); break;
        case 8: _t->onSaveImageHD(); break;
        case 9: _t->onLoadCamera(); break;
        case 10: _t->onSaveCamera(); break;
        case 11: _t->onResetCamera(); break;
        case 12: _t->onGenerate2D(); break;
        case 13: _t->onGenerate3D(); break;
        case 14: _t->onGenerate3DRoads(); break;
        case 15: _t->onDisplayRoads(); break;
        case 16: _t->onRenderingDefault(); break;
        case 17: _t->onRenderingTexture(); break;
        case 18: _t->onRenderingGroupBy(); break;
        case 19: _t->onRenderingGenerationType(); break;
        case 20: _t->onShowControlWidget(); break;
        case 21: _t->onShowPropertyWidget(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
