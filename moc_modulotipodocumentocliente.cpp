/****************************************************************************
** Meta object code from reading C++ file 'modulotipodocumentocliente.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "modulotipodocumentocliente.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modulotipodocumentocliente.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModuloTipoDocumentoCliente[] = {

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

 // methods: signature, parameters, type, tag, flags
      49,   28,   27,   27, 0x02,
      75,   27,   27,   27, 0x02,
      96,   89,   85,   27, 0x02,
     118,   27,   85,   27, 0x22,
     149,  138,  129,   27, 0x02,
     177,  171,  129,   27, 0x22,
     197,  195,   27,   27, 0x02,
     221,   27,   27,   27, 0x02,
     238,   27,  230,   27, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_ModuloTipoDocumentoCliente[] = {
    "ModuloTipoDocumentoCliente\0\0"
    "TipoDocumentoCliente\0add(TipoDocumentoCliente)\0"
    "limpiar()\0int\0parent\0rowCount(QModelIndex)\0"
    "rowCount()\0QVariant\0index,role\0"
    "data(QModelIndex,int)\0index\0"
    "data(QModelIndex)\0,\0buscar(QString,QString)\0"
    "buscar()\0QString\0"
    "retornaDescripcionTipoDocumentoCliente(QString)\0"
};

void ModuloTipoDocumentoCliente::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModuloTipoDocumentoCliente *_t = static_cast<ModuloTipoDocumentoCliente *>(_o);
        switch (_id) {
        case 0: _t->add((*reinterpret_cast< const TipoDocumentoCliente(*)>(_a[1]))); break;
        case 1: _t->limpiar(); break;
        case 2: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 5: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: _t->buscar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->buscar(); break;
        case 8: { QString _r = _t->retornaDescripcionTipoDocumentoCliente((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModuloTipoDocumentoCliente::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModuloTipoDocumentoCliente::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_ModuloTipoDocumentoCliente,
      qt_meta_data_ModuloTipoDocumentoCliente, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuloTipoDocumentoCliente::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuloTipoDocumentoCliente::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuloTipoDocumentoCliente::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuloTipoDocumentoCliente))
        return static_cast<void*>(const_cast< ModuloTipoDocumentoCliente*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int ModuloTipoDocumentoCliente::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
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
