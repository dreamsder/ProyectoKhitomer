/****************************************************************************
** Meta object code from reading C++ file 'funcionesmysql.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "funcionesmysql.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'funcionesmysql.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FuncionesMysql[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   15,   16,   15, 0x0a,
      59,   15,   51,   15, 0x0a,
      72,   15,   51,   15, 0x0a,
      89,   15,   51,   15, 0x0a,
     114,   15,   51,   15, 0x0a,
     136,   15,   16,   15, 0x0a,
     172,  164,   16,   15, 0x0a,
     202,   15,   16,   15, 0x0a,
     226,   15,   15,   15, 0x0a,
     249,   15,   16,   15, 0x0a,
     282,   15,  272,   15, 0x0a,
     305,   15,   16,   15, 0x0a,
     343,  341,   16,   15, 0x0a,
     378,   15,   15,   15, 0x0a,
     402,   15,   51,   15, 0x0a,
     432,  427,   16,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FuncionesMysql[] = {
    "FuncionesMysql\0\0bool\0verificoConexionBaseDeDatos()\0"
    "QString\0fechaDeHoy()\0fechaHoraDeHoy()\0"
    "fechaHoraDeHoyTrimeado()\0impresoraPorDefecto()\0"
    "mensajeAdvertencia(QString)\0mensaje\0"
    "mensajeAdvertenciaOk(QString)\0"
    "consutlaMysqlEstaViva()\0reseteaConexionMysql()\0"
    "consultaPingServidor()\0qlonglong\0"
    "versionDeBaseDeDatos()\0"
    "actualizacionBaseDeDatos(qlonglong)\0"
    ",\0impactoCambioEnBD(QString,QString)\0"
    "abrirPaginaWeb(QString)\0"
    "retornaFechaImportante()\0,,,,\0"
    "guardarConfiguracionXML(QString,QString,QString,QString,QString)\0"
};

void FuncionesMysql::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FuncionesMysql *_t = static_cast<FuncionesMysql *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->verificoConexionBaseDeDatos();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { QString _r = _t->fechaDeHoy();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 2: { QString _r = _t->fechaHoraDeHoy();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 3: { QString _r = _t->fechaHoraDeHoyTrimeado();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 4: { QString _r = _t->impresoraPorDefecto();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->mensajeAdvertencia((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->mensajeAdvertenciaOk((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->consutlaMysqlEstaViva();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: _t->reseteaConexionMysql(); break;
        case 9: { bool _r = _t->consultaPingServidor();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { qlonglong _r = _t->versionDeBaseDeDatos();
            if (_a[0]) *reinterpret_cast< qlonglong*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->actualizacionBaseDeDatos((*reinterpret_cast< qlonglong(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->impactoCambioEnBD((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: _t->abrirPaginaWeb((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: { QString _r = _t->retornaFechaImportante();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->guardarConfiguracionXML((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FuncionesMysql::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FuncionesMysql::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FuncionesMysql,
      qt_meta_data_FuncionesMysql, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FuncionesMysql::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FuncionesMysql::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FuncionesMysql::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FuncionesMysql))
        return static_cast<void*>(const_cast< FuncionesMysql*>(this));
    return QObject::qt_metacast(_clname);
}

int FuncionesMysql::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
