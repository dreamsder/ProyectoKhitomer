/****************************************************************************
** Meta object code from reading C++ file 'modulolistaprecioarticulos.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "modulolistaprecioarticulos.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modulolistaprecioarticulos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModuloListaPrecioArticulos[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      49,   28,   27,   27, 0x02,
      95,   27,   27,   27, 0x02,
     134,  127,  123,   27, 0x02,
     156,   27,  123,   27, 0x22,
     187,  176,  167,   27, 0x02,
     215,  209,  167,   27, 0x22,
     235,  233,   27,   27, 0x02,
     279,   27,   27,   27, 0x02,
     331,  328,  123,   27, 0x02,
     390,   27,  385,   27, 0x02,
     428,   27,  385,   27, 0x02,
     467,  233,  385,   27, 0x02,
     523,  233,  515,   27, 0x02,
     561,  233,  515,   27, 0x02,
     603,  233,  596,   27, 0x02,
     658,   27,  596,   27, 0x02,
     707,   27,  596,   27, 0x02,
     748,   27,  515,   27, 0x02,
     801,  328,  385,   27, 0x02,
     868,   27,  858,   27, 0x02,
     915,   27,  515,   27, 0x02,
     945,   27,  515,   27, 0x02,
     972,   27,  515,   27, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_ModuloListaPrecioArticulos[] = {
    "ModuloListaPrecioArticulos\0\0"
    "ArticulosListaPrecio\0"
    "addArticulosListaPrecio(ArticulosListaPrecio)\0"
    "clearArticulosListaPrecio()\0int\0parent\0"
    "rowCount(QModelIndex)\0rowCount()\0"
    "QVariant\0index,role\0data(QModelIndex,int)\0"
    "index\0data(QModelIndex)\0,\0"
    "buscarArticulosListaPrecio(QString,QString)\0"
    "buscarArticulosListaPrecioParaModificar(QString)\0"
    ",,\0insertarArticulosListaPrecio(QString,QString,QString)\0"
    "bool\0eliminarArticulosListaPrecio(QString)\0"
    "eliminarArticuloDeListaPrecio(QString)\0"
    "eliminarArticuloPorListaPrecio(QString,QString)\0"
    "QString\0retornarArticulosEnLista(int,QString)\0"
    "retornarPrecioEnLista(int,QString)\0"
    "double\0retornarPrecioDeArticuloEnBaseDeDatos(QString,QString)\0"
    "retornarCostoMonedaReferenciaDelSistema(QString)\0"
    "retornarCostoEnMonedaExtrangera(QString)\0"
    "retornarSimboloMonedaDocumentoArticuloCosto(QString)\0"
    "actualizarArticuloDeListaPrecio(QString,QString,QString)\0"
    "qlonglong\0retornaCantidadArticulosEnListaPrecio(QString)\0"
    "retornaCodigoListaPrecio(int)\0"
    "retornaCodigoArticulo(int)\0"
    "retornaPrecioArticulo(int)\0"
};

void ModuloListaPrecioArticulos::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModuloListaPrecioArticulos *_t = static_cast<ModuloListaPrecioArticulos *>(_o);
        switch (_id) {
        case 0: _t->addArticulosListaPrecio((*reinterpret_cast< const ArticulosListaPrecio(*)>(_a[1]))); break;
        case 1: _t->clearArticulosListaPrecio(); break;
        case 2: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 5: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: _t->buscarArticulosListaPrecio((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->buscarArticulosListaPrecioParaModificar((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: { int _r = _t->insertarArticulosListaPrecio((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->eliminarArticulosListaPrecio((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->eliminarArticuloDeListaPrecio((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->eliminarArticuloPorListaPrecio((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { QString _r = _t->retornarArticulosEnLista((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 13: { QString _r = _t->retornarPrecioEnLista((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 14: { double _r = _t->retornarPrecioDeArticuloEnBaseDeDatos((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 15: { double _r = _t->retornarCostoMonedaReferenciaDelSistema((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 16: { double _r = _t->retornarCostoEnMonedaExtrangera((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< double*>(_a[0]) = _r; }  break;
        case 17: { QString _r = _t->retornarSimboloMonedaDocumentoArticuloCosto((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->actualizarArticuloDeListaPrecio((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { qlonglong _r = _t->retornaCantidadArticulosEnListaPrecio((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qlonglong*>(_a[0]) = _r; }  break;
        case 20: { QString _r = _t->retornaCodigoListaPrecio((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 21: { QString _r = _t->retornaCodigoArticulo((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 22: { QString _r = _t->retornaPrecioArticulo((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModuloListaPrecioArticulos::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModuloListaPrecioArticulos::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_ModuloListaPrecioArticulos,
      qt_meta_data_ModuloListaPrecioArticulos, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuloListaPrecioArticulos::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuloListaPrecioArticulos::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuloListaPrecioArticulos::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuloListaPrecioArticulos))
        return static_cast<void*>(const_cast< ModuloListaPrecioArticulos*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int ModuloListaPrecioArticulos::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
