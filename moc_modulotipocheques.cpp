/****************************************************************************
** Meta object code from reading C++ file 'modulotipocheques.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "modulotipocheques.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modulotipocheques.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModuloTipoCheques[] = {

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

 // methods: signature, parameters, type, tag, flags
      27,   19,   18,   18, 0x02,
      50,   18,   18,   18, 0x02,
      83,   76,   72,   18, 0x02,
     105,   18,   72,   18, 0x22,
     136,  125,  116,   18, 0x02,
     164,  158,  116,   18, 0x22,
     184,  182,   18,   18, 0x02,
     223,   18,  215,   18, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_ModuloTipoCheques[] = {
    "ModuloTipoCheques\0\0Cheques\0"
    "agregarCheque(Cheques)\0limpiarListaCheques()\0"
    "int\0parent\0rowCount(QModelIndex)\0"
    "rowCount()\0QVariant\0index,role\0"
    "data(QModelIndex,int)\0index\0"
    "data(QModelIndex)\0,\0buscarCheques(QString,QString)\0"
    "QString\0retornaDescripcionCheque(QString)\0"
};

void ModuloTipoCheques::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModuloTipoCheques *_t = static_cast<ModuloTipoCheques *>(_o);
        switch (_id) {
        case 0: _t->agregarCheque((*reinterpret_cast< const Cheques(*)>(_a[1]))); break;
        case 1: _t->limpiarListaCheques(); break;
        case 2: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 5: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: _t->buscarCheques((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: { QString _r = _t->retornaDescripcionCheque((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModuloTipoCheques::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModuloTipoCheques::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_ModuloTipoCheques,
      qt_meta_data_ModuloTipoCheques, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuloTipoCheques::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuloTipoCheques::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuloTipoCheques::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuloTipoCheques))
        return static_cast<void*>(const_cast< ModuloTipoCheques*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int ModuloTipoCheques::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
