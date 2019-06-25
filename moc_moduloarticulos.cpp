/****************************************************************************
** Meta object code from reading C++ file 'moduloarticulos.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "moduloarticulos.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'moduloarticulos.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModuloArticulos[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      26,   17,   16,   16, 0x02,
      48,   16,   16,   16, 0x02,
      76,   69,   65,   16, 0x02,
      98,   16,   65,   16, 0x22,
     129,  118,  109,   16, 0x02,
     157,  151,  109,   16, 0x22,
     178,  175,   16,   16, 0x02,
     220,   16,  214,   16, 0x02,
     263,  253,   65,   16, 0x02,
     366,   16,  361,   16, 0x02,
     400,   16,  392,   16, 0x02,
     424,   16,  361,   16, 0x02,
     455,   16,  392,   16, 0x02,
     491,   16,  392,   16, 0x02,
     562,  546,  536,   16, 0x02,
     597,  546,  536,   16, 0x02,
     636,   16,  361,   16, 0x02,
     672,   16,  392,   16, 0x02,
     709,  707,  361,   16, 0x02,
     761,   16,  536,   16, 0x02,
     871,  805,  361,   16, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_ModuloArticulos[] = {
    "ModuloArticulos\0\0Articulo\0"
    "addArticulo(Articulo)\0clearArticulos()\0"
    "int\0parent\0rowCount(QModelIndex)\0"
    "rowCount()\0QVariant\0index,role\0"
    "data(QModelIndex,int)\0index\0"
    "data(QModelIndex)\0,,\0"
    "buscarArticulo(QString,QString,int)\0"
    "ulong\0ultimoRegistroDeArticuloEnBase()\0"
    ",,,,,,,,,\0"
    "insertarArticulo(QString,QString,QString,QString,QString,QString,QStri"
    "ng,QString,QString,QString)\0"
    "bool\0eliminarArticulo(QString)\0QString\0"
    "existeArticulo(QString)\0"
    "retornaArticuloActivo(QString)\0"
    "retornaDescripcionArticulo(QString)\0"
    "retornaDescripcionArticuloExtendida(QString)\0"
    "qlonglong\0_codigoArticulo\0"
    "retornaStockTotalArticulo(QString)\0"
    "retornaStockTotalArticuloReal(QString)\0"
    "existeArticuloEnDocumentos(QString)\0"
    "retornaCantidadArticulosSinStock()\0,\0"
    "reemplazaCantidadArticulosSinStock(QString,QString)\0"
    "retornaCantidadMinimaAvisoArticulo(QString)\0"
    "_cantidad,_codigoTipoDocumento,_codigoArticulo,_cantidadYaVendida\0"
    "retornaSiPuedeVenderSinStock(qlonglong,QString,QString,qlonglong)\0"
};

void ModuloArticulos::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModuloArticulos *_t = static_cast<ModuloArticulos *>(_o);
        switch (_id) {
        case 0: _t->addArticulo((*reinterpret_cast< const Articulo(*)>(_a[1]))); break;
        case 1: _t->clearArticulos(); break;
        case 2: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 5: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: _t->buscarArticulo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: { ulong _r = _t->ultimoRegistroDeArticuloEnBase();
            if (_a[0]) *reinterpret_cast< ulong*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->insertarArticulo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QString(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9])),(*reinterpret_cast< QString(*)>(_a[10])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->eliminarArticulo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { QString _r = _t->existeArticulo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->retornaArticuloActivo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { QString _r = _t->retornaDescripcionArticulo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 13: { QString _r = _t->retornaDescripcionArticuloExtendida((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 14: { qlonglong _r = _t->retornaStockTotalArticulo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qlonglong*>(_a[0]) = _r; }  break;
        case 15: { qlonglong _r = _t->retornaStockTotalArticuloReal((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qlonglong*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->existeArticuloEnDocumentos((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { QString _r = _t->retornaCantidadArticulosSinStock();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 18: { bool _r = _t->reemplazaCantidadArticulosSinStock((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { qlonglong _r = _t->retornaCantidadMinimaAvisoArticulo((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qlonglong*>(_a[0]) = _r; }  break;
        case 20: { bool _r = _t->retornaSiPuedeVenderSinStock((*reinterpret_cast< qlonglong(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< qlonglong(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModuloArticulos::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModuloArticulos::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_ModuloArticulos,
      qt_meta_data_ModuloArticulos, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuloArticulos::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuloArticulos::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuloArticulos::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuloArticulos))
        return static_cast<void*>(const_cast< ModuloArticulos*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int ModuloArticulos::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
