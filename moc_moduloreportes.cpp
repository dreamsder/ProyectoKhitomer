/****************************************************************************
** Meta object code from reading C++ file 'moduloreportes.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "moduloreportes.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'moduloreportes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ModuloReportes[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: signature, parameters, type, tag, flags
      25,   16,   15,   15, 0x02,
      51,   15,   15,   15, 0x02,
      85,   78,   74,   15, 0x02,
     107,   15,   74,   15, 0x22,
     138,  127,  118,   15, 0x02,
     166,  160,  118,   15, 0x22,
     187,  184,   15,   15, 0x02,
     234,  232,  227,   15, 0x02,
     290,  285,  277,   15, 0x02,
     343,  232,  277,   15, 0x02,
     378,   15,  277,   15, 0x02,
     415,   15,  277,   15, 0x02,
     453,  232,  277,   15, 0x02,
     520,  232,  277,   15, 0x02,
     588,  232,  277,   15, 0x02,
     655,  232,  227,   15, 0x02,
     711,  232,  227,   15, 0x02,
     758,  232,   15,   15, 0x02,
     798,  232,   15,   15, 0x02,
     838,   15,  227,   15, 0x02,
     874,   15,  277,   15, 0x02,
     904,   15,  277,   15, 0x02,
     936,   15,  277,   15, 0x02,
     969,   15,  277,   15, 0x02,
    1002,   15,  277,   15, 0x02,
    1038,   15,  277,   15, 0x02,
    1081,   15,  227,   15, 0x02,
    1111,   15,  277,   15, 0x02,
    1138,   15,  277,   15, 0x02,
    1173,   15,  277,   15, 0x02,
    1207,   15,   15,   15, 0x02,

       0        // eod
};

static const char qt_meta_stringdata_ModuloReportes[] = {
    "ModuloReportes\0\0Reportes\0"
    "agregarReportes(Reportes)\0"
    "limpiarListaReportes()\0int\0parent\0"
    "rowCount(QModelIndex)\0rowCount()\0"
    "QVariant\0index,role\0data(QModelIndex,int)\0"
    "index\0data(QModelIndex)\0,,\0"
    "buscarReportes(QString,QString,QString)\0"
    "bool\0,\0retornaPermisosDelReporte(QString,QString)\0"
    "QString\0,,,,\0"
    "generarReporte(QString,QString,QString,bool,QString)\0"
    "generarReporteXLS(QString,QString)\0"
    "retornaDirectorioReporteXLS(QString)\0"
    "retornaDescripcionDelReporte(QString)\0"
    "retornaConfiguracionAlineacionDeColumnaDelReporte(QString,QString)\0"
    "retornaConfiguracionTotalizadorDeColumnaDelReporte(QString,QString)\0"
    "retornaConfiguracionTipoDeDatoDeColumnaDelReporte(QString,QString)\0"
    "retornaSiReportaEstaHabilitadoEnPerfil(QString,QString)\0"
    "retornaReporteActivoPorPerfil(QString,QString)\0"
    "eliminarReportesPerfil(QString,QString)\0"
    "insertarReportesPerfil(QString,QString)\0"
    "imprimirReporteEnImpresora(QString)\0"
    "retornaDirectorioReporteWeb()\0"
    "retornaDirectorioEstiloCssPDF()\0"
    "retornaDirectorioEstiloCssHTML()\0"
    "retornaDirectorioJquery_min_js()\0"
    "retornaDirectorioJs_highcharts_js()\0"
    "retornaDirectorioJs_modules_exporting_js()\0"
    "imprimirReporteEnPDF(QString)\0"
    "retornaSqlReporte(QString)\0"
    "retornaSqlReporteGraficas(QString)\0"
    "retornaSqlReporteCabezal(QString)\0"
    "abrirNavegadorArchivos()\0"
};

void ModuloReportes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ModuloReportes *_t = static_cast<ModuloReportes *>(_o);
        switch (_id) {
        case 0: _t->agregarReportes((*reinterpret_cast< const Reportes(*)>(_a[1]))); break;
        case 1: _t->limpiarListaReportes(); break;
        case 2: { int _r = _t->rowCount((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->rowCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 5: { QVariant _r = _t->data((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = _r; }  break;
        case 6: _t->buscarReportes((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: { bool _r = _t->retornaPermisosDelReporte((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { QString _r = _t->generarReporte((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 9: { QString _r = _t->generarReporteXLS((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 10: { QString _r = _t->retornaDirectorioReporteXLS((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: { QString _r = _t->retornaDescripcionDelReporte((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 12: { QString _r = _t->retornaConfiguracionAlineacionDeColumnaDelReporte((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 13: { QString _r = _t->retornaConfiguracionTotalizadorDeColumnaDelReporte((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 14: { QString _r = _t->retornaConfiguracionTipoDeDatoDeColumnaDelReporte((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->retornaSiReportaEstaHabilitadoEnPerfil((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: { bool _r = _t->retornaReporteActivoPorPerfil((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: _t->eliminarReportesPerfil((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 18: _t->insertarReportesPerfil((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 19: { bool _r = _t->imprimirReporteEnImpresora((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: { QString _r = _t->retornaDirectorioReporteWeb();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 21: { QString _r = _t->retornaDirectorioEstiloCssPDF();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 22: { QString _r = _t->retornaDirectorioEstiloCssHTML();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 23: { QString _r = _t->retornaDirectorioJquery_min_js();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 24: { QString _r = _t->retornaDirectorioJs_highcharts_js();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 25: { QString _r = _t->retornaDirectorioJs_modules_exporting_js();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 26: { bool _r = _t->imprimirReporteEnPDF((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 27: { QString _r = _t->retornaSqlReporte((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 28: { QString _r = _t->retornaSqlReporteGraficas((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 29: { QString _r = _t->retornaSqlReporteCabezal((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 30: _t->abrirNavegadorArchivos(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ModuloReportes::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ModuloReportes::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_ModuloReportes,
      qt_meta_data_ModuloReportes, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ModuloReportes::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ModuloReportes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ModuloReportes::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ModuloReportes))
        return static_cast<void*>(const_cast< ModuloReportes*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int ModuloReportes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
