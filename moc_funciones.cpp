/****************************************************************************
** Meta object code from reading C++ file 'funciones.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "funciones.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'funciones.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Funciones[] = {

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
      16,   10,   11,   10, 0x0a,
      54,   10,   46,   10, 0x0a,
      67,   10,   46,   10, 0x0a,
      84,   10,   46,   10, 0x0a,
     109,   10,   46,   10, 0x0a,
     131,   10,   11,   10, 0x0a,
     167,  159,   11,   10, 0x0a,
     197,   10,   11,   10, 0x0a,
     221,   10,   10,   10, 0x0a,
     244,   10,   11,   10, 0x0a,
     277,   10,  267,   10, 0x0a,
     300,   10,   11,   10, 0x0a,
     338,  336,   11,   10, 0x0a,
     373,   10,   10,   10, 0x0a,
     397,   10,   46,   10, 0x0a,
     427,  422,   11,   10, 0x0a,
     492,   10,   46,   10, 0x0a,
     517,   10,   10,   10, 0x0a,
     534,   10,   46,   10, 0x0a,
     553,   10,   46,   10, 0x0a,
     576,   10,   46,   10, 0x0a,
     586,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Funciones[] = {
    "Funciones\0\0bool\0verificoConexionBaseDeDatos()\0"
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
    "verificarCedula(QString)\0loguear(QString)\0"
    "retornaNombreLog()\0retornaDirectorioLog()\0"
    "leerLog()\0depurarLog()\0"
};

void Funciones::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Funciones *_t = static_cast<Funciones *>(_o);
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
        case 16: { QString _r = _t->verificarCedula((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 17: _t->loguear((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: { QString _r = _t->retornaNombreLog();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 19: { QString _r = _t->retornaDirectorioLog();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 20: { QString _r = _t->leerLog();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 21: _t->depurarLog(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Funciones::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Funciones::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Funciones,
      qt_meta_data_Funciones, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Funciones::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Funciones::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Funciones::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Funciones))
        return static_cast<void*>(const_cast< Funciones*>(this));
    return QObject::qt_metacast(_clname);
}

int Funciones::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
