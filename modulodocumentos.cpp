/*********************************************************************
Khitomer - Sistema de facturación
Copyright (C) <2012-2019>  <Cristian Montano>

Este archivo es parte de Khitomer.

Khitomer es software libre: usted puede redistribuirlo y/o modificarlo
bajo los términos de la Licencia Pública General GNU publicada
por la Fundación para el Software Libre, ya sea la versión 3
de la Licencia, o (a su elección) cualquier versión posterior.

Este programa se distribuye con la esperanza de que sea útil, pero
SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita
MERCANTIL o de APTITUD PARA UN PROPÓSITO DETERMINADO.
Consulte los detalles de la Licencia Pública General GNU para obtener
una información más detallada.

Debería haber recibido una copia de la Licencia Pública General GNU
junto a este programa.
En caso contrario, consulte <http://www.gnu.org/licenses/>.
*********************************************************************/

#include "modulodocumentos.h"
#include <QtSql>
#include <QSqlQuery>
#include <Utilidades/database.h>
#include <funciones.h>
#include <QPrinter>
#include <QPainter>
#include <QDebug>
#include <QFont>
#include <moduloarticulos.h>
#include "modulomediosdepago.h"
#include <Utilidades/moduloconfiguracion.h>
#include <modulolistatipodocumentos.h>
#include <CFE/modulo_cfe_parametrosgenerales.h>
#include <curl/curl.h>

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <iostream>

#include <algorithm>

#include <json/json.h>




ModuloConfiguracion func_configuracion;

Modulo_CFE_ParametrosGenerales func_CFE_ParametrosGenerales;

ModuloListaTipoDocumentos func_tipoDocumentos;


Funciones funcion;

ModuloArticulos func_articulos;

ModuloMediosDePago func_medioDePago;

ModuloDocumentos miDocumento;

QRectF cuadro(double x, double y, double ancho, double alto, bool justifica);

QRectF cuadroA4(double x, double y, double ancho, double alto, bool justifica);

double centimetro;

QString crearJsonIMIX(QString, QString);

QString crearJsonDynamia(QString, QString, QString , QString , QString );





bool procesarImix(QString, QString);

bool procesarDynamia(QString, QString, QString, QString, QString );


QString numeroDocumentoV="";
QString codigoTipoDocumentoV="";


QString tipoDeCFEAEnviarDynamiaV="";

QString caeTipoDocumentoCFEDescripcionV="";


#define CURL_ICONV_CODESET_FOR_UTF8 "UTF-8"

QString resultadoFinal="";



bool ModuloDocumentos::actualizarNumeroCFEDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _numeroCae) const{

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("update Documentos set cae_numeroCae='"+_numeroCae+"' where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){

            return true;

        }else{
            return false;
        }
    }else{
        return false;
    }
}




static int writer(char *data, size_t size, size_t nmemb, std::string *buffer_in)
{

    if (buffer_in != NULL)
    {
        QString resultado = ((QString)data).trimmed();

        if(resultado.contains("<title>Request Error</title>",Qt::CaseInsensitive)){

            funcion.loguear("Respuesta Imix:\n"+resultado);
            funcion.mensajeAdvertenciaOk("Hubo un error al procesar el CFE: \n\n"+resultado);

            //qDebug()<< resultado;
            return -1;

        }

        QXmlStreamReader xs(resultado);

        while (!xs.atEnd()) {
            if (xs.readNextStartElement()){
                if(xs.name().toString().toLower()!="int"){
                    funcion.mensajeAdvertenciaOk("ERROR: "+xs.readElementText());
                    funcion.loguear("Respuesta Imix:\n"+resultado);

                    return -1;
                }else{
                    QString numeroCae= xs.readElementText();

                    if(miDocumento.actualizarNumeroCFEDocumento(numeroDocumentoV,codigoTipoDocumentoV,numeroCae)){
                        funcion.loguear("Respuesta Imix:\nNúmero CFE OK: "+numeroCae);

                        return size * nmemb;
                    }else{
                        funcion.mensajeAdvertenciaOk("ERROR: \nEl CFE se emitio, pero no se pudo actualizar el documento en Khitomer.");
                        return -1;
                    }

                }
            }
        }

        if(resultado.contains("status",Qt::CaseSensitive)){
            resultadoFinal=resultado;
        }
        return size * nmemb;
    }

}


static int writerDynamia(char *data, size_t size, size_t nmemb, std::string *buffer_in)
{

    if (buffer_in != NULL)
    {
        QString resultado = ((QString)data).trimmed();

        //        qDebug()<< resultado;

        if(resultado.contains("status",Qt::CaseSensitive)){
            funcion.loguear("Respuesta Dynamia:\n"+resultado);
            resultadoFinal=resultado;
            return size * nmemb;
        }else{
            funcion.loguear("Respuesta Dynamia:\n"+resultado);
            funcion.mensajeAdvertenciaOk("ERROR GRAVE: \n\n"+resultado+"\n\n Anote la información del documento y ponganse en contacto\ncon su proveedor del sistema");
            return 0;
        }
    }else{
        return 0;
    }

}





ModuloDocumentos::ModuloDocumentos(QObject *parent)
    : QAbstractListModel(parent)
{

    QHash<int, QByteArray> roles;
    roles[CodigoDocumentoRole] = "codigoDocumento";
    roles[CodigoTipoDocumentoRole] = "codigoTipoDocumento";
    roles[SerieDocumentoRole] = "serieDocumento";
    roles[CodigoEstadoDocumentoRole] = "codigoEstadoDocumento";

    roles[CodigoClienteRole] = "codigoCliente";
    roles[TipoClienteRole] = "tipoCliente";
    roles[CodigoMonedaDocumentoRole] = "codigoMonedaDocumento";
    roles[FechaEmisionDocumentoRole] = "fechaEmisionDocumento";

    roles[PrecioTotalVentaRole] = "precioTotalVenta";
    roles[PrecioSubTotalVentaRole] = "precioSubTotalVenta";
    roles[PrecioIvaVentaRole] = "precioIvaVenta";
    roles[CodigoLiquidacionRole] = "codigoLiquidacion";

    roles[CodigoVendedorLiquidacionRole] = "codigoVendedorLiquidacion";
    roles[CodigoVendedorComisionaRole] = "codigoVendedorComisiona";

    roles[NombreClienteRole] = "nombreCliente";
    roles[RazonSocialRole] = "razonSocial";
    roles[DescripcionTipoDocumentoRole] = "descripcionTipoDocumento";
    roles[DescripcionEstadoDocumentoRole] = "descripcionEstadoDocumento";

    roles[TotalIva1Role] = "totalIva1";
    roles[TotalIva2Role] = "totalIva2";
    roles[TotalIva3Role] = "totalIva3";
    roles[ObservacionRole] = "observaciones";

    roles[numeroCuentaBancariaRole] = "numeroCuentaBancaria";
    roles[codigoBancoRole] = "codigoBanco";
    roles[esDocumentoWebRole] = "esDocumentoWeb";
    roles[montoDescuentoTotalRole] = "montoDescuentoTotal";

    roles[saldoClienteCuentaCorrienteRole] = "saldoClienteCuentaCorriente";
    roles[formaDePagoRole] = "formaDePago";

    roles[porcentajeDescuentoAlTotalRole] = "porcentajeDescuentoAlTotal";
    roles[esDocumentoCFERole] = "esDocumentoCFE";

    roles[cae_numeroCaeRole] = "cae_numeroCae";
    roles[cae_serieRole] = "cae_serie";






    setRoleNames(roles);



}

Documentos::Documentos(const qulonglong &codigoDocumento, const int &codigoTipoDocumento, const QString &serieDocumento, const QString &codigoEstadoDocumento
                       ,const QString &codigoCliente ,const int &tipoCliente ,const int &codigoMonedaDocumento ,const QString &fechaEmisionDocumento
                       ,const QString &precioTotalVenta ,const QString &precioSubTotalVenta ,const QString &precioIvaVenta ,const QString &codigoLiquidacion
                       ,const QString &codigoVendedorLiquidacion,const QString &codigoVendedorComisiona

                       ,const QString &nombreCliente,const QString &razonSocial,const QString &descripcionTipoDocumento,const QString &descripcionEstadoDocumento
                       ,const QString &totalIva1,const QString &totalIva2,const QString &totalIva3,const QString &observaciones
                       ,const QString &numeroCuentaBancaria,const QString &codigoBanco,const QString &esDocumentoWeb,const QString &montoDescuentoTotal
                       ,const QString &saldoClienteCuentaCorriente,const QString &formaDePago,const QString &porcentajeDescuentoAlTotal
                       ,const QString &esDocumentoCFE

                       ,const QString &cae_numeroCae
                       ,const QString &cae_serie

                       )

    : m_codigoDocumento(codigoDocumento), m_codigoTipoDocumento(codigoTipoDocumento) , m_serieDocumento(serieDocumento), m_codigoEstadoDocumento(codigoEstadoDocumento) , m_codigoCliente(codigoCliente), m_tipoCliente(tipoCliente)
    , m_codigoMonedaDocumento(codigoMonedaDocumento), m_fechaEmisionDocumento(fechaEmisionDocumento), m_precioTotalVenta(precioTotalVenta), m_precioSubTotalVenta(precioSubTotalVenta)
    , m_precioIvaVenta(precioIvaVenta), m_codigoLiquidacion(codigoLiquidacion), m_codigoVendedorLiquidacion(codigoVendedorLiquidacion), m_codigoVendedorComisiona(codigoVendedorComisiona)

    , m_nombreCliente(nombreCliente), m_razonSocial(razonSocial), m_descripcionTipoDocumento(descripcionTipoDocumento)
    , m_descripcionEstadoDocumento(descripcionEstadoDocumento), m_totalIva1(totalIva1), m_totalIva2(totalIva2), m_totalIva3(totalIva3), m_observaciones(observaciones)
    , m_numeroCuentaBancaria(numeroCuentaBancaria), m_codigoBanco(codigoBanco), m_esDocumentoWeb(esDocumentoWeb), m_montoDescuentoTotal(montoDescuentoTotal)
    , m_saldoClienteCuentaCorriente(saldoClienteCuentaCorriente), m_formaDePago(formaDePago),m_porcentajeDescuentoAlTotal(porcentajeDescuentoAlTotal)
    , m_esDocumentoCFE(esDocumentoCFE), m_cae_numeroCae(cae_numeroCae), m_cae_serie(cae_serie)

{
}
qulonglong Documentos::codigoDocumento() const
{
    return m_codigoDocumento;
}
int Documentos::codigoTipoDocumento() const
{
    return m_codigoTipoDocumento;
}
QString Documentos::serieDocumento() const
{
    return m_serieDocumento;
}
QString Documentos::codigoEstadoDocumento() const
{
    return m_codigoEstadoDocumento;
}
QString Documentos::codigoCliente() const
{
    return m_codigoCliente;
}
int Documentos::tipoCliente() const
{
    return m_tipoCliente;
}
int Documentos::codigoMonedaDocumento() const
{
    return m_codigoMonedaDocumento;
}
QString Documentos::fechaEmisionDocumento() const
{
    return m_fechaEmisionDocumento;
}


QString Documentos::precioTotalVenta() const
{
    return m_precioTotalVenta;
}
QString Documentos::precioSubTotalVenta() const
{
    return m_precioSubTotalVenta;
}

QString Documentos::precioIvaVenta() const
{
    return m_precioIvaVenta;
}

QString Documentos::codigoLiquidacion() const
{
    return m_codigoLiquidacion;
}
QString Documentos::codigoVendedorLiquidacion() const
{
    return m_codigoVendedorLiquidacion;
}
QString Documentos::codigoVendedorComisiona() const
{
    return m_codigoVendedorComisiona;
}


QString Documentos::nombreCliente() const
{
    return m_nombreCliente;
}
QString Documentos::razonSocial() const
{
    return m_razonSocial;
}
QString Documentos::descripcionTipoDocumento() const
{
    return m_descripcionTipoDocumento;
}
QString Documentos::descripcionEstadoDocumento() const
{
    return m_descripcionEstadoDocumento;
}



QString Documentos::totalIva1() const
{
    return m_totalIva1;
}
QString Documentos::totalIva2() const
{
    return m_totalIva2;
}
QString Documentos::totalIva3() const
{
    return m_totalIva3;
}
QString Documentos::observaciones() const
{
    return m_observaciones;
}


QString Documentos::numeroCuentaBancaria() const
{
    return m_numeroCuentaBancaria;
}
QString Documentos::codigoBanco() const
{
    return m_codigoBanco;
}
QString Documentos::esDocumentoWeb() const
{
    return m_esDocumentoWeb;
}
QString Documentos::montoDescuentoTotal() const
{
    return m_montoDescuentoTotal;
}

QString Documentos::saldoClienteCuentaCorriente() const
{
    return m_saldoClienteCuentaCorriente;
}
QString Documentos::formaDePago() const
{
    return m_formaDePago;
}
QString Documentos::porcentajeDescuentoAlTotal() const
{
    return m_porcentajeDescuentoAlTotal;
}
QString Documentos::esDocumentoCFE() const
{
    return m_esDocumentoCFE;
}

QString Documentos::cae_numeroCae() const
{
    return m_cae_numeroCae;
}
QString Documentos::cae_serie() const
{
    return m_cae_serie;
}


qulonglong ModuloDocumentos::retornaCodigoDocumentoPorIndice(int indice) const{
    return m_Documentos[indice].codigoDocumento();
}
int ModuloDocumentos::retornaCodigoTipoDocumentoPorIndice(int indice) const{
    return m_Documentos[indice].codigoTipoDocumento();
}
QString ModuloDocumentos::retornaTotalDocumentoPorIndice(int indice) const{
    return m_Documentos[indice].precioTotalVenta();
}
QString ModuloDocumentos::retornaSaldoCuentaCorrientePorIndice(int indice) const{
    return m_Documentos[indice].saldoClienteCuentaCorriente();
}
int ModuloDocumentos::retornaCodigoMonedaPorIndice(int indice) const{
    return m_Documentos[indice].codigoMonedaDocumento();
}
QString ModuloDocumentos::retornaFechaDocumentoPorIndice(int indice) const{
    return m_Documentos[indice].fechaEmisionDocumento();
}
QString ModuloDocumentos::retornaObservacionesDocumentoPorIndice(int indice) const{
    return m_Documentos[indice].observaciones();
}


void ModuloDocumentos::agregarDocumento(const Documentos &documentos)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_Documentos << documentos;
    endInsertRows();
}

void ModuloDocumentos::limpiarListaDocumentos(){
    m_Documentos.clear();
}

void ModuloDocumentos::buscarDocumentos(QString campo, QString datoABuscar){

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery q = Database::consultaSql("select * from Documentos D left join Clientes C on D.codigoCliente=C.codigoCliente and D.tipoCliente=C.tipoCliente join TipoDocumento TD on TD.codigoTipoDocumento=D.codigoTipoDocumento join TipoEstadoDocumento TED on TED.codigoEstadoDocumento=D.codigoEstadoDocumento where "+campo+"'"+datoABuscar+"'");
        QSqlRecord rec = q.record();

        ModuloDocumentos::reset();
        if(q.record().count()>0){

            while (q.next()){
                ModuloDocumentos::agregarDocumento(Documentos(
                                                       q.value(rec.indexOf("codigoDocumento")).toULongLong(),
                                                       q.value(rec.indexOf("codigoTipoDocumento")).toInt(),
                                                       q.value(rec.indexOf("serieDocumento")).toString(),
                                                       q.value(rec.indexOf("codigoEstadoDocumento")).toString(),


                                                       q.value(rec.indexOf("codigoCliente")).toString(),
                                                       q.value(rec.indexOf("tipoCliente")).toInt(),
                                                       q.value(rec.indexOf("codigoMonedaDocumento")).toInt(),
                                                       q.value(rec.indexOf("fechaEmisionDocumento")).toString(),

                                                       q.value(rec.indexOf("precioTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioSubTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioIvaVenta")).toString(),
                                                       q.value(rec.indexOf("codigoLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorComisiona")).toString(),

                                                       q.value(rec.indexOf("nombreCliente")).toString(),
                                                       q.value(rec.indexOf("razonSocial")).toString(),
                                                       q.value(rec.indexOf("descripcionTipoDocumento")).toString(),
                                                       q.value(rec.indexOf("descripcionEstadoDocumento")).toString(),

                                                       q.value(rec.indexOf("totalIva1")).toString(),
                                                       q.value(rec.indexOf("totalIva2")).toString(),
                                                       q.value(rec.indexOf("totalIva3")).toString(),
                                                       q.value(rec.indexOf("observaciones")).toString(),

                                                       q.value(rec.indexOf("numeroCuentaBancaria")).toString(),
                                                       q.value(rec.indexOf("codigoBanco")).toString(),
                                                       q.value(rec.indexOf("esDocumentoWeb")).toString(),
                                                       q.value(rec.indexOf("montoDescuentoTotal")).toString(),

                                                       q.value(rec.indexOf("saldoClienteCuentaCorriente")).toString(),
                                                       q.value(rec.indexOf("formaDePago")).toString(),
                                                       q.value(rec.indexOf("porcentajeDescuentoAlTotal")).toString(),
                                                       q.value(rec.indexOf("esDocumentoCFE")).toString(),
                                                       q.value(rec.indexOf("cae_numeroCae")).toString(),
                                                       q.value(rec.indexOf("cae_serie")).toString()

                                                       )

                                                   );
            }
        }
    }
}
void ModuloDocumentos::buscarDocumentosEnLiquidaciones(QString _codigoLiquidacion, QString _codigoVendedorLiquidacion, QString _codigoPerfil){




    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery q = Database::consultaSql("select * from Documentos D join TipoDocumentoPerfilesUsuarios TDP on TDP.codigoTipoDocumento=D.codigoTipoDocumento    left join Clientes C on D.codigoCliente=C.codigoCliente and D.tipoCliente=C.tipoCliente join TipoDocumento TD on TD.codigoTipoDocumento=D.codigoTipoDocumento join TipoEstadoDocumento TED on TED.codigoEstadoDocumento=D.codigoEstadoDocumento where D.codigoLiquidacion='"+_codigoLiquidacion+"' and D.codigoVendedorLiquidacion='"+_codigoVendedorLiquidacion+"' and TDP.codigoPerfil='"+_codigoPerfil+"'  order by D.fechaHoraGuardadoDocumentoSQL desc");
        QSqlRecord rec = q.record();

        ModuloDocumentos::reset();
        if(q.record().count()>0){

            while (q.next()){
                ModuloDocumentos::agregarDocumento(Documentos(
                                                       q.value(rec.indexOf("codigoDocumento")).toULongLong(),
                                                       q.value(rec.indexOf("codigoTipoDocumento")).toInt(),
                                                       q.value(rec.indexOf("serieDocumento")).toString(),
                                                       q.value(rec.indexOf("codigoEstadoDocumento")).toString(),

                                                       q.value(rec.indexOf("codigoCliente")).toString(),
                                                       q.value(rec.indexOf("tipoCliente")).toInt(),
                                                       q.value(rec.indexOf("codigoMonedaDocumento")).toInt(),
                                                       q.value(rec.indexOf("fechaEmisionDocumento")).toString(),

                                                       q.value(rec.indexOf("precioTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioSubTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioIvaVenta")).toString(),
                                                       q.value(rec.indexOf("codigoLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorComisiona")).toString(),

                                                       q.value(rec.indexOf("nombreCliente")).toString(),
                                                       q.value(rec.indexOf("razonSocial")).toString(),
                                                       q.value(rec.indexOf("descripcionTipoDocumento")).toString(),
                                                       q.value(rec.indexOf("descripcionEstadoDocumento")).toString(),

                                                       q.value(rec.indexOf("totalIva1")).toString(),
                                                       q.value(rec.indexOf("totalIva2")).toString(),
                                                       q.value(rec.indexOf("totalIva3")).toString(),
                                                       q.value(rec.indexOf("observaciones")).toString(),
                                                       q.value(rec.indexOf("numeroCuentaBancaria")).toString(),
                                                       q.value(rec.indexOf("codigoBanco")).toString(),
                                                       q.value(rec.indexOf("esDocumentoWeb")).toString(),
                                                       q.value(rec.indexOf("montoDescuentoTotal")).toString(),

                                                       q.value(rec.indexOf("saldoClienteCuentaCorriente")).toString(),
                                                       q.value(rec.indexOf("formaDePago")).toString(),
                                                       q.value(rec.indexOf("porcentajeDescuentoAlTotal")).toString(),
                                                       q.value(rec.indexOf("esDocumentoCFE")).toString(),
                                                       q.value(rec.indexOf("cae_numeroCae")).toString(),
                                                       q.value(rec.indexOf("cae_serie")).toString()


                                                       )

                                                   );
            }
        }
    }
}


void ModuloDocumentos::buscarDocumentosEnMantenimiento(QString campo, QString datoABuscar, QString _codigoPerfil){


    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery q = Database::consultaSql("select D.*, TD.descripcionTipoDocumento, TED.descripcionEstadoDocumento, C.nombreCliente,C.razonSocial from Documentos D join TipoDocumentoPerfilesUsuarios TDP on TDP.codigoTipoDocumento=D.codigoTipoDocumento left join Clientes C on D.codigoCliente=C.codigoCliente and D.tipoCliente=C.tipoCliente join TipoDocumento TD on TD.codigoTipoDocumento=D.codigoTipoDocumento join TipoEstadoDocumento TED on TED.codigoEstadoDocumento=D.codigoEstadoDocumento left join DocumentosLineas DL on DL.codigoDocumento=D.codigoDocumento and DL.codigoTipoDocumento=D.codigoTipoDocumento where "+campo+"'"+datoABuscar+"' and TDP.codigoPerfil='"+_codigoPerfil+"' group by D.codigoDocumento,D.codigoTipoDocumento   order by D.fechaHoraGuardadoDocumentoSQL desc");
        QSqlRecord rec = q.record();


        //  qDebug()<<q.lastQuery() ;

        ModuloDocumentos::reset();
        if(q.record().count()>0){

            while (q.next()){
                ModuloDocumentos::agregarDocumento(Documentos(
                                                       q.value(rec.indexOf("codigoDocumento")).toULongLong(),
                                                       q.value(rec.indexOf("codigoTipoDocumento")).toInt(),
                                                       q.value(rec.indexOf("serieDocumento")).toString(),
                                                       q.value(rec.indexOf("codigoEstadoDocumento")).toString(),


                                                       q.value(rec.indexOf("codigoCliente")).toString(),
                                                       q.value(rec.indexOf("tipoCliente")).toInt(),
                                                       q.value(rec.indexOf("codigoMonedaDocumento")).toInt(),
                                                       q.value(rec.indexOf("fechaEmisionDocumento")).toString(),

                                                       q.value(rec.indexOf("precioTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioSubTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioIvaVenta")).toString(),
                                                       q.value(rec.indexOf("codigoLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorComisiona")).toString(),

                                                       q.value(rec.indexOf("nombreCliente")).toString(),
                                                       q.value(rec.indexOf("razonSocial")).toString(),
                                                       q.value(rec.indexOf("descripcionTipoDocumento")).toString(),
                                                       q.value(rec.indexOf("descripcionEstadoDocumento")).toString(),

                                                       q.value(rec.indexOf("totalIva1")).toString(),
                                                       q.value(rec.indexOf("totalIva2")).toString(),
                                                       q.value(rec.indexOf("totalIva3")).toString(),
                                                       q.value(rec.indexOf("observaciones")).toString(),
                                                       q.value(rec.indexOf("numeroCuentaBancaria")).toString(),
                                                       q.value(rec.indexOf("codigoBanco")).toString(),
                                                       q.value(rec.indexOf("esDocumentoWeb")).toString(),
                                                       q.value(rec.indexOf("montoDescuentoTotal")).toString(),

                                                       q.value(rec.indexOf("saldoClienteCuentaCorriente")).toString(),
                                                       q.value(rec.indexOf("formaDePago")).toString(),
                                                       q.value(rec.indexOf("porcentajeDescuentoAlTotal")).toString(),
                                                       q.value(rec.indexOf("esDocumentoCFE")).toString(),
                                                       q.value(rec.indexOf("cae_numeroCae")).toString(),
                                                       q.value(rec.indexOf("cae_serie")).toString()



                                                       )

                                                   );
            }
        }
    }
}


void ModuloDocumentos::buscarDocumentosAPagarCuentaCorriente(QString _codigoMoneda, QString _codigoCliente, QString _codigoTipoCliente){

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery q = Database::consultaSql("select * FROM Documentos DOC join TipoDocumento TDOC on TDOC.codigoTipoDocumento=DOC.codigoTipoDocumento where DOC.tipoCliente='"+_codigoTipoCliente+"' and DOC.codigoEstadoDocumento in ('E','G') and TDOC.afectaCuentaCorriente=1 and DOC.codigoCliente='"+_codigoCliente+"' and DOC.codigoMonedaDocumento='"+_codigoMoneda+"'  and DOC.saldoClienteCuentaCorriente!=0;");
        QSqlRecord rec = q.record();

        ModuloDocumentos::reset();
        if(q.record().count()>0){

            while (q.next()){




                ModuloDocumentos::agregarDocumento(Documentos(
                                                       q.value(rec.indexOf("codigoDocumento")).toULongLong(),
                                                       q.value(rec.indexOf("codigoTipoDocumento")).toInt(),
                                                       q.value(rec.indexOf("serieDocumento")).toString(),
                                                       q.value(rec.indexOf("codigoEstadoDocumento")).toString(),


                                                       q.value(rec.indexOf("codigoCliente")).toString(),
                                                       q.value(rec.indexOf("tipoCliente")).toInt(),
                                                       q.value(rec.indexOf("codigoMonedaDocumento")).toInt(),
                                                       q.value(rec.indexOf("fechaEmisionDocumento")).toString(),

                                                       q.value(rec.indexOf("precioTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioSubTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioIvaVenta")).toString(),
                                                       q.value(rec.indexOf("codigoLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorComisiona")).toString(),

                                                       q.value(rec.indexOf("nombreCliente")).toString(),
                                                       q.value(rec.indexOf("razonSocial")).toString(),
                                                       q.value(rec.indexOf("descripcionTipoDocumento")).toString(),
                                                       q.value(rec.indexOf("descripcionEstadoDocumento")).toString(),

                                                       q.value(rec.indexOf("totalIva1")).toString(),
                                                       q.value(rec.indexOf("totalIva2")).toString(),
                                                       q.value(rec.indexOf("totalIva3")).toString(),
                                                       q.value(rec.indexOf("observaciones")).toString(),
                                                       q.value(rec.indexOf("numeroCuentaBancaria")).toString(),
                                                       q.value(rec.indexOf("codigoBanco")).toString(),
                                                       q.value(rec.indexOf("esDocumentoWeb")).toString(),
                                                       q.value(rec.indexOf("montoDescuentoTotal")).toString(),

                                                       q.value(rec.indexOf("saldoClienteCuentaCorriente")).toString(),
                                                       q.value(rec.indexOf("formaDePago")).toString(),
                                                       q.value(rec.indexOf("porcentajeDescuentoAlTotal")).toString(),
                                                       q.value(rec.indexOf("esDocumentoCFE")).toString(),
                                                       q.value(rec.indexOf("cae_numeroCae")).toString(),
                                                       q.value(rec.indexOf("cae_serie")).toString()

                                                       )

                                                   );
            }
        }
    }
}



void ModuloDocumentos::buscarDocumentosDePagoCuentaCorriente(QString _codigoMoneda, QString _codigoCliente, QString _codigoTipoCliente){

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery q = Database::consultaSql("select * FROM Documentos DOC join TipoDocumento TDOC on TDOC.codigoTipoDocumento=DOC.codigoTipoDocumento where DOC.tipoCliente='"+_codigoTipoCliente+"' and DOC.codigoEstadoDocumento in ('E','G') and TDOC.afectaCuentaCorriente=-1 and DOC.codigoCliente='"+_codigoCliente+"' and DOC.codigoMonedaDocumento='"+_codigoMoneda+"'  and DOC.saldoClienteCuentaCorriente!=0;");
        QSqlRecord rec = q.record();

        ModuloDocumentos::reset();
        if(q.record().count()>0){

            while (q.next()){




                ModuloDocumentos::agregarDocumento(Documentos(
                                                       q.value(rec.indexOf("codigoDocumento")).toULongLong(),
                                                       q.value(rec.indexOf("codigoTipoDocumento")).toInt(),
                                                       q.value(rec.indexOf("serieDocumento")).toString(),
                                                       q.value(rec.indexOf("codigoEstadoDocumento")).toString(),


                                                       q.value(rec.indexOf("codigoCliente")).toString(),
                                                       q.value(rec.indexOf("tipoCliente")).toInt(),
                                                       q.value(rec.indexOf("codigoMonedaDocumento")).toInt(),
                                                       q.value(rec.indexOf("fechaEmisionDocumento")).toString(),

                                                       q.value(rec.indexOf("precioTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioSubTotalVenta")).toString(),
                                                       q.value(rec.indexOf("precioIvaVenta")).toString(),
                                                       q.value(rec.indexOf("codigoLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorLiquidacion")).toString(),
                                                       q.value(rec.indexOf("codigoVendedorComisiona")).toString(),

                                                       q.value(rec.indexOf("nombreCliente")).toString(),
                                                       q.value(rec.indexOf("razonSocial")).toString(),
                                                       q.value(rec.indexOf("descripcionTipoDocumento")).toString(),
                                                       q.value(rec.indexOf("descripcionEstadoDocumento")).toString(),

                                                       q.value(rec.indexOf("totalIva1")).toString(),
                                                       q.value(rec.indexOf("totalIva2")).toString(),
                                                       q.value(rec.indexOf("totalIva3")).toString(),
                                                       q.value(rec.indexOf("observaciones")).toString(),
                                                       q.value(rec.indexOf("numeroCuentaBancaria")).toString(),
                                                       q.value(rec.indexOf("codigoBanco")).toString(),
                                                       q.value(rec.indexOf("esDocumentoWeb")).toString(),
                                                       q.value(rec.indexOf("montoDescuentoTotal")).toString(),

                                                       q.value(rec.indexOf("saldoClienteCuentaCorriente")).toString(),
                                                       q.value(rec.indexOf("formaDePago")).toString(),
                                                       q.value(rec.indexOf("porcentajeDescuentoAlTotal")).toString(),
                                                       q.value(rec.indexOf("esDocumentoCFE")).toString(),
                                                       q.value(rec.indexOf("cae_numeroCae")).toString(),
                                                       q.value(rec.indexOf("cae_serie")).toString()

                                                       )

                                                   );
            }
        }
    }
}



int ModuloDocumentos::rowCount(const QModelIndex & parent) const {
    return m_Documentos.count();
}

QVariant ModuloDocumentos::data(const QModelIndex & index, int role) const {

    if (index.row() < 0 || index.row() > m_Documentos.count()){
        return QVariant();

    }

    const Documentos &documentos = m_Documentos[index.row()];

    if (role == CodigoDocumentoRole){
        return documentos.codigoDocumento();
    }
    else if (role == CodigoTipoDocumentoRole){
        return documentos.codigoTipoDocumento();
    }
    else if (role == SerieDocumentoRole){
        return documentos.serieDocumento();
    }
    else if (role == CodigoEstadoDocumentoRole){
        return documentos.codigoEstadoDocumento();
    }

    else if (role == CodigoClienteRole){
        return documentos.codigoCliente();
    }
    else if (role == TipoClienteRole){
        return documentos.tipoCliente();
    }
    else if (role == CodigoMonedaDocumentoRole){
        return documentos.codigoMonedaDocumento();
    }
    else if (role == FechaEmisionDocumentoRole){
        return documentos.fechaEmisionDocumento();
    }
    else if (role == PrecioTotalVentaRole){
        return documentos.precioTotalVenta();
    }
    else if (role == PrecioSubTotalVentaRole){
        return documentos.precioSubTotalVenta();
    }

    else if (role == PrecioIvaVentaRole){
        return documentos.precioIvaVenta();
    }
    else if (role == CodigoLiquidacionRole){
        return documentos.codigoLiquidacion();
    }
    else if (role == CodigoVendedorLiquidacionRole){
        return documentos.codigoVendedorLiquidacion();
    }
    else if (role == CodigoVendedorComisionaRole){
        return documentos.codigoVendedorComisiona();
    }

    else if (role == NombreClienteRole){
        return documentos.nombreCliente();
    }
    else if (role == RazonSocialRole){
        return documentos.razonSocial();
    }
    else if (role == DescripcionTipoDocumentoRole){
        return documentos.descripcionTipoDocumento();
    }
    else if (role == DescripcionEstadoDocumentoRole){
        return documentos.descripcionEstadoDocumento();
    }

    else if (role == TotalIva1Role){
        return documentos.totalIva1();
    }
    else if (role == TotalIva2Role){
        return documentos.totalIva2();
    }
    else if (role == TotalIva3Role){
        return documentos.totalIva3();
    }
    else if (role == ObservacionRole){
        return documentos.observaciones();
    }
    else if (role == numeroCuentaBancariaRole){
        return documentos.numeroCuentaBancaria();
    }
    else if (role == codigoBancoRole){
        return documentos.codigoBanco();
    }
    else if (role == esDocumentoWebRole){
        return documentos.esDocumentoWeb();
    }
    else if (role == montoDescuentoTotalRole){
        return documentos.montoDescuentoTotal();
    }
    else if (role == saldoClienteCuentaCorrienteRole){
        return documentos.saldoClienteCuentaCorriente();
    }
    else if (role == formaDePagoRole){
        return documentos.formaDePago();
    }
    else if (role == porcentajeDescuentoAlTotalRole){
        return documentos.porcentajeDescuentoAlTotal();
    }
    else if (role == esDocumentoCFERole){
        return documentos.esDocumentoCFE();
    }
    else if (role == cae_numeroCaeRole){
        return documentos.cae_numeroCae();
    }
    else if (role == cae_serieRole){
        return documentos.cae_serie();
    }

    return QVariant();
}


bool ModuloDocumentos::eliminarDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        // Elimina las lineas de articulo del documento
        if(eliminarLineaDocumento(_codigoDocumento,_codigoTipoDocumento)){

            // Elimina las lineas de medio de pago del documento
            if(func_medioDePago.eliminarLineaMedioDePagoDocumento(_codigoDocumento,_codigoTipoDocumento)){

                // Elimina el documento.
                if(query.exec("delete from Documentos where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){

                    return true;

                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool ModuloDocumentos::eliminarLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("delete from DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){

            return true;

        }else{
            return false;
        }
    }else{
        return false;
    }
}


bool ModuloDocumentos::actualizarDatoExtraLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _numeroLinea, QString _datoAModificar) const{

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("update DocumentosLineas set codigoArticuloBarras='"+_datoAModificar+"' where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_numeroLinea+"'")){

            return true;

        }else{
            return false;
        }
    }else{
        return false;
    }
}



int ModuloDocumentos::guardarDocumentos(QString _codigoDocumento,QString _codigoTipoDocumento,QString _serieDocumento, QString _codigoCliente,QString _tipoCliente ,

                                        QString _codigoMonedaDocumento , QString _fechaEmisionDocumento,

                                        QString _precioTotalVenta, QString _precioSubTotalVenta,QString _precioIvaVenta ,

                                        QString _codigoLiquidacion, QString _codigoVendedorComisiona,QString _usuarioAlta, QString _estadoFinalDocumento

                                        , QString _codigoVendedorLiquidacion,
                                        QString _totalIva1,
                                        QString _totalIva2,
                                        QString _totalIva3,
                                        QString _cotizacionMoneda,QString _observaciones,
                                        QString _numeroCuentaBancaria,int _codigoBancoCuentaBancaria,
                                        QString _montoDescuentoTotal, QString _precioSubTotalSinDescuento,
                                        QString _formaDePago,
                                        QString _porcentajeDescuentoAlTotal,
                                        QString _saldoClientePagoContado
                                        ) const {

    // -1  No se pudo conectar a la base de datos
    // -2  Documento con estado invalido
    //  1  documento dado de alta ok
    //  2  Documento actualizado correctamente
    // -3  no se pudo guardar el documento
    // -4 no se pudo realizar la consulta a la base de datos
    // -5 El documento no tiene los datos sufucientes para darse de alta.
    // -6 El documento existe cancelado.
    // -7 El documento existe como emitido/impreso/finalizado
    // -8 El documento existe como guardado/finalizado
    // -9 El documento existe en proceso de guardado, ATENCION.
    // -10 No se realizaron cambios en el documento existente como pendiente.


    if(_codigoDocumento.trimmed()=="" || _codigoTipoDocumento.trimmed()=="" || _serieDocumento.trimmed()==""){
        return -5;
    }



    QString esUnDocumentoDeCFE="0";

    if(func_configuracion.retornaValorConfiguracion("MODO_CFE")=="1" && func_tipoDocumentos.retornaPermisosDelDocumento(_codigoTipoDocumento,"emiteCFEImprime")){
        esUnDocumentoDeCFE="1";
    }

    if(_codigoLiquidacion==""){
        _codigoLiquidacion="0";
    }

    QString _tieneDescuentoAlTotal="0";

    if(!_montoDescuentoTotal.toDouble()==0.00){
        _tieneDescuentoAlTotal="1";
    }


    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());

        if(_estadoFinalDocumento=="EMITIR"){

            if(query.exec("select codigoDocumento,codigoEstadoDocumento from Documentos where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {

                if(query.first()){
                    if(query.value(0).toString()!=""){

                        if(query.value(1).toString()=="C"){
                            return -6;
                        }else if(query.value(1).toString()=="E"){
                            return -7;
                        }else if(query.value(1).toString()=="G"){
                            return -8;
                        }else if(query.value(1).toString()=="A"){
                            return -9;
                        }else if(query.value(1).toString()=="P"){
                            if(funcion.mensajeAdvertencia("El documento ya existe guardado en estado pendiente.\nDesea guardarlo como finalizado?\n\nPresione [ Sí ] para confirmar.")){

                                if(query.exec("update Documentos set codigoEstadoDocumento='A',  codigoMonedaDocumento='"+_codigoMonedaDocumento+"',fechaUltimaModificacionDocumento='"+funcion.fechaHoraDeHoy()+"', fechaEmisionDocumento='"+_fechaEmisionDocumento+"',usuarioUltimaModificacion='"+_usuarioAlta+"',precioTotalVenta='"+_precioTotalVenta+"',precioSubTotalVenta='"+_precioSubTotalVenta+"',precioSubTotalVentaSinDescuento='"+_precioSubTotalSinDescuento+"'     ,precioIvaVenta='"+_precioIvaVenta+"',codigoLiquidacion='"+_codigoLiquidacion+"',codigoVendedorComisiona='"+_codigoVendedorComisiona+"', codigoVendedorLiquidacion='"+_codigoVendedorLiquidacion+"', totalIva1='"+_totalIva1+"', totalIva2='"+_totalIva2+"', totalIva3='"+_totalIva3+"',cotizacionMoneda='"+_cotizacionMoneda+"',observaciones='"+_observaciones+"',numeroCuentaBancaria='"+_numeroCuentaBancaria+"',codigoBanco='"+QString::number(_codigoBancoCuentaBancaria)+"', montoDescuentoTotal='"+_montoDescuentoTotal+"',tieneDescuentoAlTotal='"+_tieneDescuentoAlTotal+"',saldoClienteCuentaCorriente='"+_precioTotalVenta+"',formaDePago='"+_formaDePago+"',porcentajeDescuentoAlTotal='"+_porcentajeDescuentoAlTotal+"',saldoClientePagoContado='"+_saldoClientePagoContado+"',esDocumentoCFE='"+esUnDocumentoDeCFE+"'          where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' ")){
                                    return 1;
                                }else{
                                    funcion.mensajeAdvertencia(query.lastError().text());
                                    funcion.mensajeAdvertencia(query.lastQuery());
                                    return -3;
                                }
                            }else{
                                return -10;
                            }
                        }else{
                            return -2;
                        }
                    }else{return -4;}
                }else{
                    if(query.exec("insert INTO Documentos (codigoDocumento, codigoTipoDocumento, serieDocumento, codigoEstadoDocumento, codigoCliente, tipoCliente, codigoMonedaDocumento, fechaEmisionDocumento, precioTotalVenta,precioSubTotalVenta, precioIvaVenta, codigoLiquidacion, codigoVendedorComisiona, usuarioAlta,codigoVendedorLiquidacion,totalIva1,totalIva2,totalIva3,cotizacionMoneda,observaciones,numeroCuentaBancaria,codigoBanco,montoDescuentoTotal,tieneDescuentoAlTotal,precioSubTotalVentaSinDescuento,saldoClienteCuentaCorriente,formaDePago,porcentajeDescuentoAlTotal,saldoClientePagoContado,esDocumentoCFE) values('"+_codigoDocumento+"','"+_codigoTipoDocumento+"','"+_serieDocumento+"','A','"+_codigoCliente+"','"+_tipoCliente+"','"+_codigoMonedaDocumento+"','"+_fechaEmisionDocumento+"','"+_precioTotalVenta+"','"+_precioSubTotalVenta+"','"+_precioIvaVenta+"','"+_codigoLiquidacion+"','"+_codigoVendedorComisiona+"','"+_usuarioAlta+"','"+_codigoVendedorLiquidacion+"','"+_totalIva1+"','"+_totalIva2+"','"+_totalIva3+"','"+_cotizacionMoneda+"','"+_observaciones+"','"+_numeroCuentaBancaria+"','"+QString::number(_codigoBancoCuentaBancaria)+"','"+_montoDescuentoTotal+"','"+_tieneDescuentoAlTotal+"','"+_precioSubTotalSinDescuento+"','"+_precioTotalVenta+"','"+_formaDePago+"','"+_porcentajeDescuentoAlTotal+"','"+_saldoClientePagoContado+"','"+esUnDocumentoDeCFE+"' )")){
                        return 1;
                    }else{
                        funcion.mensajeAdvertencia(query.lastError().text());
                        funcion.mensajeAdvertencia(query.lastQuery());
                        return -3;
                    }
                }
            }else{
                return -4;
            }
        }else if(_estadoFinalDocumento=="PENDIENTE"){
            if(query.exec("select codigoDocumento,codigoEstadoDocumento from Documentos where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {

                if(query.first()){
                    if(query.value(0).toString()!=""){

                        if(query.value(1).toString()=="C"){
                            return -6;
                        }else if(query.value(1).toString()=="E"){
                            return -7;
                        }else if(query.value(1).toString()=="G"){
                            return -8;
                        }else if(query.value(1).toString()=="A"){
                            return -9;
                        }else if(query.value(1).toString()=="P"){
                            if(funcion.mensajeAdvertencia("El documento ya existe guardado en estado pendiente.\nDesea actualizarlo?\n\nPresione [ Sí ] para confirmar.")){

                                if(query.exec("update Documentos set codigoMonedaDocumento='"+_codigoMonedaDocumento+"',fechaUltimaModificacionDocumento='"+funcion.fechaHoraDeHoy()+"', fechaEmisionDocumento='"+_fechaEmisionDocumento+"',usuarioUltimaModificacion='"+_usuarioAlta+"',precioTotalVenta='"+_precioTotalVenta+"',precioSubTotalVenta='"+_precioSubTotalVenta+"',precioSubTotalVentaSinDescuento='"+_precioSubTotalSinDescuento+"'  ,precioIvaVenta='"+_precioIvaVenta+"',codigoLiquidacion='"+_codigoLiquidacion+"',codigoVendedorComisiona='"+_codigoVendedorComisiona+"',codigoVendedorLiquidacion='"+_codigoVendedorLiquidacion+"', totalIva1='"+_totalIva1+"', totalIva2='"+_totalIva2+"', totalIva3='"+_totalIva3+"',cotizacionMoneda='"+_cotizacionMoneda+"',observaciones='"+_observaciones+"',numeroCuentaBancaria='"+_numeroCuentaBancaria+"',codigoBanco='"+QString::number(_codigoBancoCuentaBancaria)+"', montoDescuentoTotal="+_montoDescuentoTotal+",tieneDescuentoAlTotal='"+_tieneDescuentoAlTotal+"',saldoClienteCuentaCorriente='"+_precioTotalVenta+"',formaDePago='"+_formaDePago+"',porcentajeDescuentoAlTotal='"+_porcentajeDescuentoAlTotal+"',saldoClientePagoContado='"+_saldoClientePagoContado+"'    where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
                                    return 1;
                                }else{
                                    funcion.mensajeAdvertencia(query.lastError().text());
                                    funcion.mensajeAdvertencia(query.lastQuery());
                                    return -3;
                                }
                            }else{
                                return -10;
                            }
                        }else{
                            return -2;
                        }
                    }else{return -4;}
                }else{

                    if(query.exec("insert INTO Documentos (codigoDocumento, codigoTipoDocumento, serieDocumento, codigoEstadoDocumento, codigoCliente, tipoCliente, codigoMonedaDocumento, fechaEmisionDocumento, precioTotalVenta,precioSubTotalVenta, precioIvaVenta, codigoLiquidacion, codigoVendedorComisiona, usuarioAlta,codigoVendedorLiquidacion,totalIva1,totalIva2,totalIva3,cotizacionMoneda,observaciones,numeroCuentaBancaria,codigoBanco,montoDescuentoTotal,tieneDescuentoAlTotal,precioSubTotalVentaSinDescuento,saldoClienteCuentaCorriente,formaDePago,porcentajeDescuentoAlTotal,saldoClientePagoContado,esDocumentoCFE) values('"+_codigoDocumento+"','"+_codigoTipoDocumento+"','"+_serieDocumento+"','P','"+_codigoCliente+"','"+_tipoCliente+"','"+_codigoMonedaDocumento+"','"+_fechaEmisionDocumento+"','"+_precioTotalVenta+"','"+_precioSubTotalVenta+"','"+_precioIvaVenta+"','"+_codigoLiquidacion+"','"+_codigoVendedorComisiona+"','"+_usuarioAlta+"','"+_codigoVendedorLiquidacion+"','"+_totalIva1+"','"+_totalIva2+"','"+_totalIva3+"','"+_cotizacionMoneda+"','"+_observaciones+"','"+_numeroCuentaBancaria+"','"+QString::number(_codigoBancoCuentaBancaria)+"','"+_montoDescuentoTotal+"','"+_tieneDescuentoAlTotal+"','"+_precioSubTotalSinDescuento+"','"+_precioTotalVenta+"','"+_formaDePago+"','"+_porcentajeDescuentoAlTotal+"','"+_saldoClientePagoContado+"','"+esUnDocumentoDeCFE+"'    )")){
                        return 1;
                    }else{
                        funcion.mensajeAdvertencia(query.lastError().text());
                        funcion.mensajeAdvertencia(query.lastQuery());
                        return -3;
                    }
                }
            }else{
                return -4;
            }
        }
    }else{
        return -1;
    }
}

bool ModuloDocumentos::guardarLineaDocumento(QString _consultaInsertLineas) const {

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec(_consultaInsertLineas)){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}


bool ModuloDocumentos::anuloMontosDebitadosCuentaCorriente(QString _codigoDocumentoDePago, QString _codigoTipoDocumentoDePago) const {

    bool actualizacionCorrecta=true;

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());

        if(query.exec("SELECT  codigoDocumento,codigoTipoDocumento,montoDescontadoCuentaCorriente   FROM DocumentosCanceladosCuentaCorriente where codigoDocumentoQueCancela='"+_codigoDocumentoDePago+"' and codigoTipoDocumentoQueCancela='"+_codigoTipoDocumentoDePago+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=""){

                    query.previous();
                    while (query.next()){

                        if(restauroMontoDeudaCuentaCorrienteDocumento(query.value(0).toString(),query.value(1).toString(),query.value(2).toString())==false){
                            actualizacionCorrecta==false;
                            break;
                        }

                    }
                    return actualizacionCorrecta;
                }else{
                    return true;
                }
            }else {return true;}
        }else{
            return false;
        }
    }else{
        qDebug()<<"No se puede conectar a mysql server";
        return false;
    }

}

bool ModuloDocumentos::restauroMontoDeudaCuentaCorrienteDocumento(QString _codigoDocumentoAPagar, QString _codigoTipoDocumentoAPagar,QString _montoADebitar) const {

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        ///Actualizo el documento de venta(factura credito, ajuste cuenta corriente +)
        if(query.exec("update Documentos set saldoClienteCuentaCorriente=saldoClienteCuentaCorriente+"+_montoADebitar+" where codigoDocumento="+_codigoDocumentoAPagar+" and codigoTipoDocumento="+_codigoTipoDocumentoAPagar+"")){
            return true;
        }else{
            qDebug()<< query.lastError();
            return false;
        }
    }else{
        qDebug()<<"No se puede conectar a mysql server";
        return false;
    }
}

int ModuloDocumentos::actualizarCuentaCorriente(QString _codigoDocumentoAPagar, QString _codigoTipoDocumentoAPagar, QString _codigoClienteAPagar, QString _codigoTipoClienteAPagar, QString _codigoMonedaAPagar
                                                ,QString _codigoDocumentoDePago, QString _codigoTipoDocumentoDePago, QString _codigoClienteDePago, QString _codigoTipoClienteDePago, QString _codigoMonedaDePago,
                                                QString _montoADebitar,QString _montoDelSaldo
                                                ) const {
    /// -5 Atencion, error al actualizar el documento de pago, y no se pudo restaurar el documento original, esto provoca incongruencias en el sistema
    /// -4 Datos no concuerdan para realizar la actualización
    /// -3 - Error en monto a debitar
    /// -2 - Error al actualizar el documento de pago
    /// -1 - Error al actualizar el documento a pagar
    /// 0 - Error en conexion a mysql server
    /// 1 - Actualizado con exito



    if(_montoADebitar=="0.00" || _montoADebitar=="0" || _montoADebitar=="0.0" || _montoADebitar=="0."|| _montoADebitar==".00" || _montoADebitar==".0"){
        return -3;
    }else{
        if(_codigoClienteAPagar!=_codigoClienteDePago || _codigoTipoClienteAPagar!=_codigoTipoClienteDePago || _codigoMonedaAPagar!=_codigoMonedaDePago){
            return -4;
        }
    }

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        ///Actualizo el documento de venta(factura credito, ajuste cuenta corriente +)
        if(query.exec("update Documentos set saldoClienteCuentaCorriente=saldoClienteCuentaCorriente-"+_montoADebitar+" where codigoDocumento="+_codigoDocumentoAPagar+" and codigoTipoDocumento="+_codigoTipoDocumentoAPagar+" and codigoCliente='"+_codigoClienteAPagar+"' and tipoCliente="+_codigoTipoClienteAPagar+" and codigoMonedaDocumento="+_codigoMonedaAPagar+"")){
            query.clear();
            ///Actualizo el documento de pago(recibo, nota de credito, ajuste cuenta corriente -)
            if(query.exec("update Documentos set saldoClienteCuentaCorriente=saldoClienteCuentaCorriente-"+_montoADebitar+" where codigoDocumento="+_codigoDocumentoDePago+" and codigoTipoDocumento="+_codigoTipoDocumentoDePago+" and codigoCliente='"+_codigoClienteDePago+"' and tipoCliente="+_codigoTipoClienteDePago+" and codigoMonedaDocumento="+_codigoMonedaDePago+"")){

                query.clear();
                ///Inserto las referencias de pago en la base de datos
                if(query.exec("insert into DocumentosCanceladosCuentaCorriente(codigoDocumento,codigoTipoDocumento,codigoDocumentoQueCancela,codigoTipoDocumentoQueCancela,montoDescontadoCuentaCorriente)values('"+_codigoDocumentoAPagar+"','"+_codigoTipoDocumentoAPagar+"','"+_codigoDocumentoDePago+"','"+_codigoTipoDocumentoDePago+"',"+_montoADebitar+");")){
                    return 1;
                }else{
                    qDebug()<< query.lastError();
                    funcion.mensajeAdvertencia(query.lastError().text());
                    funcion.mensajeAdvertencia(query.lastQuery());
                    return -5;
                }
            }else{
                qDebug()<< query.lastError();
                query.clear();
                if(query.exec("update Documentos set saldoClienteCuentaCorriente="+_montoDelSaldo+" where codigoDocumento="+_codigoDocumentoAPagar+" and codigoTipoDocumento="+_codigoTipoDocumentoAPagar+" and codigoCliente='"+_codigoClienteAPagar+"' and tipoCliente="+_codigoTipoClienteAPagar+" and codigoMonedaDocumento="+_codigoMonedaAPagar+"")){
                    return -2;
                }else{
                    return -5;
                }
            }
        }else{
            qDebug()<< query.lastError();
            return -1;
        }
    }else{
        qDebug()<<"No se puede conectar a mysql server";
        return 0;
    }
}

bool ModuloDocumentos::actualizoEstadoDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _estadoDocumento ,QString _usuarioAlta) const {


    if(_codigoDocumento.trimmed()=="" || _codigoTipoDocumento.trimmed()=="" ){
        return false;
    }
    Database::chequeaStatusAccesoMysql();
    bool conexion=true;
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("update Documentos set codigoEstadoDocumento='"+_estadoDocumento+"' ,usuarioUltimaModificacion='"+_usuarioAlta+"' where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}


bool ModuloDocumentos::actualizoEstadoDocumentoCFE(QString _codigoDocumento,QString _codigoTipoDocumento, QString _estadoDocumento) const {


    if(_codigoDocumento.trimmed()=="" || _codigoTipoDocumento.trimmed()=="" ){
        return false;
    }
    Database::chequeaStatusAccesoMysql();
    bool conexion=true;
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("update Documentos set codigoEstadoDocumento='"+_estadoDocumento+"'  where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}


bool ModuloDocumentos::actualizoSaldoClientePagoContadoDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _saldoClientePagoContado) const {


    if(_codigoDocumento.trimmed()=="" || _codigoTipoDocumento.trimmed()=="" ){
        return false;
    }
    Database::chequeaStatusAccesoMysql();
    bool conexion=true;
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());
        if(query.exec("update Documentos set saldoClientePagoContado='"+_saldoClientePagoContado+"'  where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

int ModuloDocumentos::retornaCantidadLineasDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("SELECT count(*) FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toInt();
                }else{
                    return 0;
                }
            }else {return 0;}
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

QString ModuloDocumentos::retornoCodigoArticuloDeLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _linea) {
    bool conexion=true;

    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("SELECT codigoArticulo FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_linea+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toString();
                }else{
                    return "";
                }
            }else{return "";}
        }else{
            return "";
        }
    }else{
        return "";
    }
}

QString ModuloDocumentos::retornoCantidadDocumentosPorCliente(QString _codigoCliente,QString _codigoTipoCliente, QString _estadoDocumentos) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){
        QSqlQuery query(Database::connect());

        if(_estadoDocumentos=="ANULADOS"){
            if(query.exec("select count(*) from Documentos where codigoCliente ='"+_codigoCliente+"'   and tipoCliente='"+_codigoTipoCliente+"' and codigoEstadoDocumento in ('C')")) {
                if(query.first()){
                    if(query.value(0).toString()!=0){
                        return query.value(0).toString();
                    }else{
                        return "0";
                    }
                }else{return "0";}
            }else{
                return "0";
            }
        }else{
            if(query.exec("select count(*) from Documentos where codigoCliente ='"+_codigoCliente+"'   and tipoCliente='"+_codigoTipoCliente+"' and codigoEstadoDocumento in ('E','G')")) {
                if(query.first()){
                    if(query.value(0).toString()!=0){
                        return query.value(0).toString();
                    }else{
                        return "0";
                    }
                }else{return "0";}
            }else{
                return "0";
            }
        }
    }else{
        return "0";
    }
}

QString ModuloDocumentos::retornoCodigoArticuloBarrasDeLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _linea) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("SELECT codigoArticuloBarras FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_linea+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toString();
                }else{
                    return "";
                }
            }else{return "";}
        }else{
            return "";
        }
    }else{
        return "";
    }
}

double ModuloDocumentos::retornoPrecioArticuloDeLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _linea) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());

        if(query.exec("SELECT precioArticuloUnitario FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_linea+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toDouble();
                }else{
                    return 0.00;
                }
            }else{return 0.00;}
        }else{
            return 0.00;
        }
    }else{
        return 0.00;
    }
}

double ModuloDocumentos::retornoCostoArticuloMonedaReferenciaDeLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _linea) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());

        if(query.exec("SELECT costoArticuloMonedaReferencia FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_linea+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toDouble();
                }else{
                    return 0.00;
                }
            }else{return 0.00;}
        }else{
            return 0.00;
        }
    }else{
        return 0.00;
    }
}


double ModuloDocumentos::retornoDescuentoLineaArticuloDeLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _linea) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());

        if(query.exec("SELECT montoDescuento FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_linea+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toDouble();
                }else{
                    return 0.00;
                }
            }else{return 0.00;}
        }else{
            return 0.00;
        }
    }else{
        return 0.00;
    }
}


int ModuloDocumentos::retornoCantidadArticuloDeLineaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento, QString _linea) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("SELECT cantidad FROM DocumentosLineas where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"' and numeroLinea='"+_linea+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toInt();
                }else{
                    return 1;
                }
            }else{return 1;}
        }else{
            return 1;
        }
    }else{
        return 1;
    }
}

qlonglong ModuloDocumentos::retornoCodigoUltimoDocumentoDisponible(QString _codigoTipoDocumento) {
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("SELECT codigoDocumento FROM Documentos where codigoTipoDocumento='"+_codigoTipoDocumento+"' order by codigoDocumento desc limit 1")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    return query.value(0).toLongLong()+1;
                }else{
                    return 1;
                }
            }else{return 1;}
        }else{
            return 1;
        }
    }else{
        return 1;
    }
}

QString ModuloDocumentos::retornacodigoEstadoDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoEstadoDocumento from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=""){
                    return query.value(0).toString();
                }else{
                    return "";
                }
            }else{return "Error BD";}
        }else{
            return "Error SQL";
        }
    }else{
        return "Error BD";
    }
}

QString ModuloDocumentos::retornaDescripcionEstadoDocumento(QString _codigoEstado) const{
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec("SELECT codigoEstadoDocumento,descripcionEstadoDocumento FROM TipoEstadoDocumento where codigoEstadoDocumento='"+_codigoEstado+"';")) {
            if(query.first()){
                if(query.value(0).toString()!=""){
                    return query.value(1).toString();
                }else{
                    return "Nuevo";
                }
            }else{return "Nuevo";}
        }else{
            return "Nuevo";
        }
    }else{
        return "Nuevo";
    }
}

bool ModuloDocumentos::retornaClienteTieneRUT(QString _codigoCliente,QString _codigoTipoCliente) const{
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec("select rut,codigoTipoDocumentoCliente from Clientes where codigoCliente='"+_codigoCliente+"' and tipoCliente='"+_codigoTipoCliente+"' limit 1")) {
            if(query.first()){
                if(query.value(1).toString()=="2"){
                    if(query.value(0).toString()=="" || query.value(0).toString().toLower().trimmed() =="null" || query.value(0).toString().isNull() || query.value(0).toString().isEmpty()){
                        return false;
                    }else{
                        return true;
                    }
                }else{
                    return false;
                }
            }else{return false;}
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool ModuloDocumentos::documentoValidoParaCalculoPonderado(QString _codigoTipoDocumento)const{
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec("SELECT * FROM TipoDocumento where codigoTipoDocumento='"+_codigoTipoDocumento+"' and utilizaArticulos=1 and afectaStock=1 and utilizaSoloProveedores=1")) {
            if(query.first()){
                if(query.value(0).toString()=="" || query.value(0).toString().toLower().trimmed() =="null" || query.value(0).toString().isNull() || query.value(0).toString().isEmpty()){
                    return false;
                }else{
                    return true;
                }
            }else{return false;}
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool ModuloDocumentos::retornoSiClienteTieneDocumentos(QString _codigoCliente)const{
    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec("SELECT codigoDocumento FROM Documentos where codigoCliente ='"+_codigoCliente+"' ")) {
            if(query.first()){
                if(query.value(0).toString()=="" || query.value(0).toString().toLower().trimmed() =="null" || query.value(0).toString().isNull() || query.value(0).toString().isEmpty()){
                    return false;
                }else{
                    return true;
                }
            }else{return false;}
        }else{
            return true;
        }
    }else{
        return true;
    }
}

double ModuloDocumentos::retonaCostoPonderadoSegunStock(QString _codigoArticulo, qlonglong _cantidad, double _costoArticuloUnitario) const {

    double totalCostoDelArticulo=_costoArticuloUnitario*_cantidad;

    qlonglong stockTotalArticulo=func_articulos.retornaStockTotalArticulo(_codigoArticulo);

    if(stockTotalArticulo == 0){
        return _costoArticuloUnitario;
    }

    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());

        if(query.exec("SELECT DL.costoArticuloPonderado FROM DocumentosLineas DL join Documentos DOC on DOC.codigoDocumento=DL.codigoDocumento and DOC.codigoTipoDocumento=DL.codigoTipoDocumento join TipoDocumento TD on TD.codigoTipoDocumento=DOC.codigoTipoDocumento where DOC.codigoEstadoDocumento in ('G','E') and TD.utilizaArticulos=1 and TD.afectaStock=1 and TD.utilizaSoloProveedores=1 and DL.codigoArticulo='"+_codigoArticulo+"' order by DOC.fechaHoraGuardadoDocumentoSQL desc limit 1 ")) {
            if(query.first()){
                if(query.value(0).toString()!=0){

                    double totalCostoPonderado=query.value(0).toDouble()*stockTotalArticulo;

                    double totalCostoDeTodoElStock=totalCostoPonderado+totalCostoDelArticulo;

                    return totalCostoDeTodoElStock/(_cantidad+stockTotalArticulo);

                }else{
                    return _costoArticuloUnitario;
                }
            }else{
                return _costoArticuloUnitario;
            }
        }else{
            return _costoArticuloUnitario;
        }
    }else{
        return _costoArticuloUnitario;
    }
}



bool ModuloDocumentos::actualizarInformacionCFEDocumentoDynamia(QString _codigoDocumento, QString _codigoTipoDocumento, QString  _nro,
                                                                QString _serie, QString _vencimiento,
                                                                QString _cod_seguridad, QString _cae_id,
                                                                QString _desde, QString _hasta,
                                                                QString _qr, QString _idDocGaia,
                                                                QString _caeTipoDocumentoCFEDescripcion
                                                                ){

    Database::chequeaStatusAccesoMysql();
    bool conexion=true;
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){

        QSqlQuery query(Database::connect());
        if(query.exec("update Documentos set cae_numeroCae='"+_nro+"',cae_serie='"+_serie+"',cae_fechaVencimiento='"+_vencimiento+"',cae_codigoSeguridad='"+_cod_seguridad+"',cae_Cae='"+_cae_id+"',cae_rangoDesde='"+_desde+"',cae_rangoHasta='"+_hasta+"',cae_QrCode='"+_qr+"',cae_idDocGaia='"+_idDocGaia+"',caeTipoDocumentoCFEDescripcion='"+_caeTipoDocumentoCFEDescripcion+"'   where codigoDocumento='"+_codigoDocumento+"' and codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }



}




QString ModuloDocumentos::retornatipoClienteDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.tipoCliente from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornacodigoClienteDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoCliente from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}


QString ModuloDocumentos::retornaserieDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.serieDocumento from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornacodigoVendedorComisionaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoVendedorComisiona from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornacodigoLiquidacionDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoLiquidacion from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornacodigoVendedorLiquidacionDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoVendedorLiquidacion from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornafechaEmisionDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.fechaEmisionDocumento from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornacodigoMonedaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoMonedaDocumento from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaprecioIvaVentaDocumento    (QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.precioIvaVenta from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaprecioSubTotalVentaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.precioSubTotalVenta from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaprecioTotalVentaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.precioTotalVenta from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornatotalIva1Documento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.totalIva1 from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornatotalIva2Documento  (QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.totalIva2 from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornatotalIva3Documento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.totalIva3 from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaobservacionesDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.observaciones from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaonumeroCuentaBancariaDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.numeroCuentaBancaria from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaocodigoBancoDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.codigoBanco from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaEsDocumentoWebDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.esDocumentoWeb from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaMontoDescuentoTotalDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.montoDescuentoTotal from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaFormaDePagoDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.formaDePago from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaPorcentajeDescuentoAlTotalDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.porcentajeDescuentoAlTotal from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}

QString ModuloDocumentos::retornaEsDocumentoCFEDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{    bool conexion=true;    if(!Database::connect().isOpen()){        if(!Database::connect().open()){            qDebug() << "No conecto";            conexion=false;        }    }    if(conexion){        QSqlQuery query(Database::connect());
        if(query.exec("select DOC.esDocumentoCFE from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {            if(query.first()){                if(query.value(0).toString()!=""){                    return query.value(0).toString();                }else{                    return  "";                }            }else{return "Error BD";}        }else{            return "Error SQL";        }    }else{        return "Error BD";    }}


QString ModuloDocumentos::retornaNumeroCFEOriginal(QString _codigoDocumento,QString _codigoTipoDocumento) const{


    bool conexion=true;


    if(!Database::connect().isOpen()){

        if(!Database::connect().open()){

            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){

        QSqlQuery query(Database::connect());

        if(query.exec("select DOC.cae_numeroCae from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
            if(query.first()){
                if(query.value(0).toString()!=""){
                    return query.value(0).toString();
                }else{
                    return  "";
                }
            }
            else{
                return "Error BD";
            }
        }else{
            return "Error SQL";
        }
    }else{
        return "Error BD";
    }
}



bool ModuloDocumentos::existeDocumento(QString _codigoDocumento,QString _codigoTipoDocumento) const{


    bool conexion=true;


    if(!Database::connect().isOpen()){

        if(!Database::connect().open()){

            qDebug() << "No conecto";
            conexion=false;
        }
    }

    if(conexion){
        QSqlQuery query(Database::connect());

        if(query.exec("select DOC.codigoDocumento from Documentos DOC  where DOC.codigoDocumento='"+_codigoDocumento+"'  and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")){
            if(query.first()){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}




/// ##########################################################################################################################
/// ######## Realiza los calculos para posicionar los campos del modelo de impresion e imprime el documento ##################
/// ##########################################################################################################################

bool ModuloDocumentos::emitirDocumentoEnImpresora(QString _codigoDocumento,QString _codigoTipoDocumento,QString _impresora){

    // consulto si estoy en modo CFE
    if(func_configuracion.retornaValorConfiguracion("MODO_CFE")=="1" && func_CFE_ParametrosGenerales.retornaValor("modoFuncionCFE")=="0"){

        return true;

    }else{

        //No estoy en modo CFE, por lo que continuo con la impresion normal

        int cantidadDecimalesMonto=func_configuracion.retornaValorConfiguracion("CANTIDAD_DIGITOS_DECIMALES_MONTO_IMPRESION").toInt(0);


        bool conexion=true;
        Database::chequeaStatusAccesoMysql();

        if(!Database::connect().isOpen()){
            if(!Database::connect().open()){
                qDebug() << "No conecto";
                conexion=false;
            }
        }

        QString _codigoModeloImpresion="";
        double altoPagina;
        double anchoPagina;
        double comienzoCuerpo;
        double comienzoPie;
        double cantidadItemsPorHoja=0;
        int fuenteSizePoints=9;

        double cantidadLineas=0;

        if(conexion){
            QSqlQuery query(Database::connect());
            if(query.exec("SELECT codigoModeloImpresion FROM TipoDocumento where codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {
                if(query.first()){
                    if(query.value(0).toString()!=0){
                        _codigoModeloImpresion = query.value(0).toString();
                    }else{
                        return false;
                    }
                }else{return false;}
            }else{
                return false;
            }
        }else{
            return false;
        }


        if(_codigoModeloImpresion=="")
            return false;

        QSqlQuery query(Database::connect());
        if(query.exec("SELECT altoPagina,anchoPagina,comienzoCuerpo,comienzoPie,fuenteSizePoints FROM ModeloImpresion where codigoModeloImpresion='"+_codigoModeloImpresion+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    altoPagina = query.value(0).toDouble()*10;
                }else{
                    return false;
                }

                if(query.value(1).toString()!=0){
                    anchoPagina = query.value(1).toDouble()*10;
                }else{
                    return false;
                }

                if(query.value(2).toString()!=0){
                    comienzoCuerpo = query.value(2).toDouble();
                }else{
                    return false;
                }

                if(query.value(3).toString()!=0){
                    comienzoPie = query.value(3).toDouble();
                }else{
                    return false;
                }

                if(query.value(4).toString()!=0){
                    fuenteSizePoints = query.value(4).toInt();
                }else{
                    return false;
                }
            }else{return false;}
        }else{
            return false;
        }
        int MaximoCopias=1;


        double seteos=comienzoCuerpo;


        query.clear();


        if(query.exec("SELECT cantidadCopias FROM TipoDocumento where codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    MaximoCopias = query.value(0).toInt();
                }
            }else{return false;}
        }else{
            return false;
        }

        query.clear();

        /// Obtengo la cantidad de lineas del cuerpo a imprimir
        if(query.exec("SELECT count(distinct DOC.codigoArticulo) FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {
            if(query.first()){
                if(query.value(0).toString()!=0){
                    cantidadLineas = query.value(0).toInt();
                }else{
                    cantidadLineas = 0;
                }
            }else{cantidadLineas = 0;}
        }


        ///Obtengo la cantidad de lineas que puedo usar en la hoja
        cantidadItemsPorHoja= ((comienzoPie-comienzoCuerpo)*26)/13;

        int MaximoHojasAImprimir=0;


        ///Calculo la cantidad de hojas necesarias para imprimir la factura
        if(cantidadLineas<=cantidadItemsPorHoja){

            MaximoHojasAImprimir=1;

        }else{

            if((cantidadLineas/cantidadItemsPorHoja)<=2){
                MaximoHojasAImprimir=2;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=3){
                MaximoHojasAImprimir=3;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=4){
                MaximoHojasAImprimir=4;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=5){
                MaximoHojasAImprimir=5;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=6){
                MaximoHojasAImprimir=6;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=7){
                MaximoHojasAImprimir=7;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=8){
                MaximoHojasAImprimir=8;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=9){
                MaximoHojasAImprimir=9;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=10){
                MaximoHojasAImprimir=10;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=11){
                MaximoHojasAImprimir=11;
            }else if((cantidadLineas/cantidadItemsPorHoja)<=12){
                MaximoHojasAImprimir=12;
            }

        }

        //##################################################
        // Preparo los seteos de la impresora ##############
        QPrinter printer;
        printer.setPrinterName(_impresora);
        QPainter painter;
        printer.setOutputFormat(QPrinter::NativeFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setPageSize(QPrinter::A4);
        QSizeF size;
        size.setHeight(altoPagina);
        size.setWidth(anchoPagina);
        //printer.setPaperSize(size,QPrinter::Millimeter);
        printer.setOrientation(QPrinter::Portrait);
        printer.setCreator("Khitomer");
        printer.setColorMode(QPrinter::GrayScale);
        printer.setFullPage(true);
        QFont fuente("Arial");

        fuente.setPointSize(fuenteSizePoints);



        centimetro = printer.QPaintDevice::width()/(printer.QPaintDevice::widthMM()/10);

        // FIN de preparo los seteos de la impresora ######
        //#################################################


        QSqlQuery queryCabezal(Database::connect());
        QSqlQuery queryCuerpo(Database::connect());
        QSqlQuery queryPie(Database::connect());


        int cantidadVecesIncremento=cantidadItemsPorHoja;


        //Cantidad de veces que se repite la impresion del mismo documento
        for(int i=0;i<MaximoCopias;i++){

            int contadorLineas=0;
            int iterador=0;
            comienzoCuerpo=seteos;
            cantidadItemsPorHoja=cantidadVecesIncremento;



            if (! painter.begin(&printer)) {
                return false;
            }
            painter.setFont(fuente);


            // Cantidad de hojas que se imprimen del mismo documento
            for(int j=0;j<MaximoHojasAImprimir;j++){



                if(j<MaximoHojasAImprimir && j!=0){

                    printer.newPage();

                    iterador+=cantidadVecesIncremento;
                    cantidadItemsPorHoja+=cantidadVecesIncremento;
                    comienzoCuerpo=seteos;

                }


                ///Armamos el cabezal de la impresion

                if(query.exec("select MIL.posicionX,MIL.posicionY,MIL.largoDeCampo,MIL.alineacion,case when MIL.alineacion=2 then true else false end,IC.campoEnTabla,IC.codigoEtiqueta, MIL.textoImprimibleIzquierda, MIL.textoImprimibleDerecha, MIL.fuenteSizePoints   from ModeloImpresionLineas MIL join ImpresionCampos IC on IC.codigoCampoImpresion=MIL.codigoCampoImpresion where MIL.codigoModeloImpresion='"+_codigoModeloImpresion+"' and IC.tipoCampo=1")) {

                    while(query.next()){

                        fuente.setPointSize(query.value(9).toInt());
                        painter.setFont(fuente);

                        queryCabezal.clear();
                        if(queryCabezal.exec("SELECT case when "+query.value(5).toString()+"='' then ' ' else "+query.value(5).toString()+"  end, Doc.codigoCliente,Doc.tipoCliente FROM Documentos Doc left join Clientes C on C.codigoCliente=Doc.codigoCliente and C.tipoCliente=Doc.tipoCliente left join Localidades LOC on LOC.codigoLocalidad=C.codigoLocalidad and LOC.codigoDepartamento=C.codigoDepartamento and LOC.codigoPais=C.codigoPais join TipoDocumento TD on TD.codigoTipoDocumento=Doc.codigoTipoDocumento  where Doc.codigoDocumento='"+_codigoDocumento+"' and Doc.codigoTipoDocumento='"+_codigoTipoDocumento+"'")) {
                            queryCabezal.next();
                            if(queryCabezal.value(0).toString()!=0){

                                if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                    if(query.value(5).toString()=="Doc.fechaEmisionDocumento"){
                                        painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+QDateTime::fromString(queryCabezal.value(0).toString(),Qt::ISODate).toString("dd-MM-yyyy")+" "+query.value(8).toString());

                                    }else{
                                        if(query.value(6).toString()=="txtMarcaXDeClienteFinalCampo"){

                                            //chequeo si el cliente tiene RUT, si no tiene, imprimo la X de cliente final
                                            if(!retornaClienteTieneRUT(queryCabezal.value(1).toString().trimmed(),queryCabezal.value(2).toString().trimmed())){
                                                painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+"X"+" "+query.value(8).toString());
                                            }

                                        }else if(query.value(6).toString()=="txtTextoGenericoCabezal1" || query.value(6).toString()=="txtTextoGenericoCabezal2" || query.value(6).toString()=="txtTextoGenericoCabezal3" || query.value(6).toString()=="txtTextoGenericoCabezal4"){
                                            painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+query.value(8).toString());
                                        }else{
                                            painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+queryCabezal.value(0).toString()+" "+query.value(8).toString());
                                        }
                                    }
                                }



                                if(query.value(5).toString()=="Doc.fechaEmisionDocumento"){
                                    painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+QDateTime::fromString(queryCabezal.value(0).toString(),Qt::ISODate).toString("dd-MM-yyyy")+" "+query.value(8).toString());
                                }else{
                                    if(query.value(6).toString()=="txtMarcaXDeClienteFinalCampo"){

                                        //chequeo si el cliente tiene RUT, si no tiene, imprimo la X de cliente final
                                        if(!retornaClienteTieneRUT(queryCabezal.value(1).toString().trimmed(),queryCabezal.value(2).toString().trimmed())){

                                            painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+"X"+" "+query.value(8).toString());

                                        }
                                    }else if(query.value(6).toString()=="txtTextoGenericoCabezal1" || query.value(6).toString()=="txtTextoGenericoCabezal2" || query.value(6).toString()=="txtTextoGenericoCabezal3" || query.value(6).toString()=="txtTextoGenericoCabezal4"){
                                        painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+query.value(8).toString());
                                    }else{
                                        painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(7).toString()+" "+queryCabezal.value(0).toString()+" "+query.value(8).toString());
                                    }
                                }
                            }
                        }
                    }
                }else{ return false; }


                ///Armamos el cuerpo de la impresion
                query.clear();
                if(query.exec("select MIL.posicionX,MIL.posicionY,MIL.largoDeCampo,MIL.alineacion,case when MIL.alineacion=2 then true else false end,IC.campoEnTabla, MIL.textoImprimibleIzquierda, MIL.textoImprimibleDerecha, IC.codigoEtiqueta, MIL.fuenteSizePoints from ModeloImpresionLineas MIL join ImpresionCampos IC on IC.codigoCampoImpresion=MIL.codigoCampoImpresion where MIL.codigoModeloImpresion='"+_codigoModeloImpresion+"' and IC.tipoCampo=2")) {

                    QString campoDeOrdenItemsFacturaOrderOGroupBy=" order by DOC.numeroLinea asc  ";
                    QString campoSumaONoSumaCantidadLineasSegunCampoOrden=" DOC.cantidad ";
                    QString campoSumaONoSumaTotalVentaItemsSegunCampoOrden=" precioTotalVenta ";

                    QString campoSumaONoSumaDescuentoLineaVentaItemsSegunCampoOrden = " montoDescuento ";


                    QString resultadoConsulta=func_tipoDocumentos.retornaValorCampoTipoDocumento(_codigoTipoDocumento,"tipoOrdenLineasFacturasAlEmitirse");


                    if(resultadoConsulta=="0"){
                        campoDeOrdenItemsFacturaOrderOGroupBy=" group by DOC.codigoArticulo  ";
                        campoSumaONoSumaCantidadLineasSegunCampoOrden=" sum(DOC.cantidad) ";
                        campoSumaONoSumaTotalVentaItemsSegunCampoOrden=" sum(precioTotalVenta) ";
                        campoSumaONoSumaDescuentoLineaVentaItemsSegunCampoOrden = " sum(montoDescuento) ";
                    }else if(resultadoConsulta=="1"){
                        campoDeOrdenItemsFacturaOrderOGroupBy=" order by DOC.numeroLinea asc  ";
                        campoSumaONoSumaCantidadLineasSegunCampoOrden=" DOC.cantidad ";
                        campoSumaONoSumaTotalVentaItemsSegunCampoOrden=" precioTotalVenta ";
                        campoSumaONoSumaDescuentoLineaVentaItemsSegunCampoOrden = " montoDescuento ";
                    }

                    while(query.next()){


                        fuente.setPointSize(query.value(9).toInt());
                        painter.setFont(fuente);

                        comienzoCuerpo=seteos;
                        queryCuerpo.clear();
                        if(query.value(5).toString()=="CANTIDAD"){

                            if(queryCuerpo.exec("SELECT DOC.codigoArticulo,   "+campoSumaONoSumaCantidadLineasSegunCampoOrden+"   FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'  "+campoDeOrdenItemsFacturaOrderOGroupBy+"  ")) {

                                contadorLineas=0;
                                while(queryCuerpo.next()){


                                    if(queryCuerpo.value(0).toString()!=0){

                                        if(contadorLineas<cantidadItemsPorHoja && contadorLineas >=iterador ){

                                            if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){

                                                if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                                }else{
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+queryCuerpo.value(1).toString()+" "+query.value(7).toString());
                                                }

                                            }
                                            if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                            }else{
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+queryCuerpo.value(1).toString()+" "+query.value(7).toString());
                                            }

                                            comienzoCuerpo+=0.5;
                                        }
                                        contadorLineas++;
                                    }

                                }
                            }

                        }else if(query.value(5).toString()=="precioTotalVenta"){
                            if(queryCuerpo.exec("SELECT DOC.codigoArticulo, "+campoSumaONoSumaTotalVentaItemsSegunCampoOrden+"  FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'   "+campoDeOrdenItemsFacturaOrderOGroupBy+" ")) {

                                contadorLineas=0;

                                while(queryCuerpo.next()){
                                    if(queryCuerpo.value(0).toString()!=0){

                                        if(contadorLineas<cantidadItemsPorHoja && contadorLineas >=iterador){

                                            if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                                if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                                }else{
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+QString::number(queryCuerpo.value(1).toDouble(),'f',cantidadDecimalesMonto)+" "+query.value(7).toString());
                                                }

                                            }
                                            if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                            }else{
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+QString::number(queryCuerpo.value(1).toDouble(),'f',cantidadDecimalesMonto)+" "+query.value(7).toString());
                                            }
                                            comienzoCuerpo+=0.5;

                                        }
                                        contadorLineas++;
                                    }
                                }
                            }
                        }
                        else if(query.value(5).toString()=="montoDescuento"){
                            if(queryCuerpo.exec("SELECT DOC.codigoArticulo, "+campoSumaONoSumaDescuentoLineaVentaItemsSegunCampoOrden+"  FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'   "+campoDeOrdenItemsFacturaOrderOGroupBy+" ")) {

                                contadorLineas=0;

                                while(queryCuerpo.next()){
                                    if(queryCuerpo.value(0).toString()!=0){

                                        if(contadorLineas<cantidadItemsPorHoja && contadorLineas >=iterador){

                                            if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                                if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                                }else{
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+QString::number(queryCuerpo.value(1).toDouble(),'f',cantidadDecimalesMonto)+" "+query.value(7).toString());
                                                }

                                            }
                                            if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                            }else{
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+QString::number(queryCuerpo.value(1).toDouble(),'f',cantidadDecimalesMonto)+" "+query.value(7).toString());
                                            }
                                            comienzoCuerpo+=0.5;

                                        }
                                        contadorLineas++;
                                    }
                                }
                            }
                        }

                        else if(query.value(5).toString()=="precioArticuloUnitario"){
                            if(queryCuerpo.exec("SELECT DOC.codigoArticulo, precioArticuloUnitario  FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'  "+campoDeOrdenItemsFacturaOrderOGroupBy+" ")) {

                                contadorLineas=0;

                                while(queryCuerpo.next()){
                                    if(queryCuerpo.value(0).toString()!=0){

                                        if(contadorLineas<cantidadItemsPorHoja && contadorLineas >=iterador){
                                            if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                                if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                                }else{
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+QString::number(queryCuerpo.value(1).toDouble(),'f',cantidadDecimalesMonto)+" "+query.value(7).toString());
                                                }

                                            }
                                            if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                            }else{
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+QString::number(queryCuerpo.value(1).toDouble(),'f',cantidadDecimalesMonto)+" "+query.value(7).toString());
                                            }

                                            comienzoCuerpo+=0.5;

                                        }
                                        contadorLineas++;
                                    }
                                }
                            }
                        }else if(query.value(5).toString()=="descripcionArticulo"){
                            if(queryCuerpo.exec("SELECT DOC.codigoArticulo  FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'  "+campoDeOrdenItemsFacturaOrderOGroupBy+"  ")) {

                                contadorLineas=0;

                                while(queryCuerpo.next()){
                                    if(queryCuerpo.value(0).toString()!=0){

                                        if(contadorLineas<cantidadItemsPorHoja && contadorLineas >=iterador){

                                            if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                                if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(), query.value(6).toString()+" "+query.value(7).toString());
                                                }else{
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(), query.value(6).toString()+" "+func_articulos.retornaDescripcionArticulo(queryCuerpo.value(0).toString())+" "+query.value(7).toString());
                                                }

                                            }
                                            if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(), query.value(6).toString()+" "+query.value(7).toString());
                                            }else{
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(), query.value(6).toString()+" "+func_articulos.retornaDescripcionArticulo(queryCuerpo.value(0).toString())+" "+query.value(7).toString());
                                            }

                                            comienzoCuerpo+=0.5;

                                        }
                                        contadorLineas++;
                                    }
                                }
                            }
                        }else{
                            if(queryCuerpo.exec("SELECT DOC.codigoArticulo, case when "+query.value(5).toString()+"='' then ' ' else "+query.value(5).toString()+"  end  FROM DocumentosLineas DOC where DOC.codigoDocumento='"+_codigoDocumento+"' and DOC.codigoTipoDocumento='"+_codigoTipoDocumento+"'  "+campoDeOrdenItemsFacturaOrderOGroupBy+" ")) {

                                contadorLineas=0;

                                while(queryCuerpo.next()){
                                    if(queryCuerpo.value(0).toString()!=0){

                                        if(contadorLineas<cantidadItemsPorHoja && contadorLineas >=iterador){
                                            if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                                if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                                }else{
                                                    painter.drawText(cuadroA4(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+queryCuerpo.value(1).toString()+" "+query.value(7).toString());
                                                }

                                            }
                                            if(query.value(8).toString()=="txtTextoGenericoCuerpo1" || query.value(8).toString()=="txtTextoGenericoCuerpo2" || query.value(8).toString()=="txtTextoGenericoCuerpo3" || query.value(8).toString()=="txtTextoGenericoCuerpo4"){
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                            }else{
                                                painter.drawText(cuadro(query.value(0).toDouble(),comienzoCuerpo,query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+queryCuerpo.value(1).toString()+" "+query.value(7).toString());
                                            }

                                            comienzoCuerpo+=0.5;

                                        }
                                        contadorLineas++;
                                    }
                                }
                            }
                        }


                    }
                }else{ return false; }



                ///Armamos el pie de la impresion
                query.clear();


                QImage imagenQr;
                QByteArray imagenQrString;

                QString _montoAGrabar="0"+func_configuracion.retornaCantidadDecimalesString()+"";
                if(query.exec("select MIL.posicionX,MIL.posicionY,MIL.largoDeCampo,MIL.alineacion,case when MIL.alineacion=2 then true else false end,IC.campoEnTabla, MIL.textoImprimibleIzquierda, MIL.textoImprimibleDerecha, IC.codigoEtiqueta, MIL.fuenteSizePoints from ModeloImpresionLineas MIL join ImpresionCampos IC on IC.codigoCampoImpresion=MIL.codigoCampoImpresion where MIL.codigoModeloImpresion='"+_codigoModeloImpresion+"' and IC.tipoCampo=3")) {

                    while(query.next()){

                        fuente.setPointSize(query.value(9).toInt());
                        painter.setFont(fuente);


                        _montoAGrabar="0"+func_configuracion.retornaCantidadDecimalesString()+"";
                        queryPie.clear();
                        if(queryPie.exec("SELECT case when "+query.value(5).toString()+"='' then ' ' else "+query.value(5).toString()+"  end FROM Documentos Doc left join Clientes C on C.codigoCliente=Doc.codigoCliente and C.tipoCliente=Doc.tipoCliente join Monedas on Monedas.codigoMoneda=Doc.codigoMonedaDocumento where Doc.codigoDocumento='"+_codigoDocumento+"' and Doc.codigoTipoDocumento='"+_codigoTipoDocumento+"'"))
                        {
                            queryPie.next();
                            if(queryPie.value(0).toString()!=0){

                                if(func_configuracion.retornaValorConfiguracion("MODO_IMPRESION_A4")=="1"){
                                    if(query.value(8).toString()=="txtTextoGenericoPie1" || query.value(8).toString()=="txtTextoGenericoPie2" || query.value(8).toString()=="txtTextoGenericoPie3" || query.value(8).toString()=="txtTextoGenericoPie4"){

                                        painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());
                                    }else if(query.value(8).toString()=="txtSubTotalSinDescuentoCampo" || query.value(8).toString()=="txtTotalDescuentoAlTotal" || query.value(8).toString()=="txtIvaExentoCampo"
                                             || query.value(8).toString()=="txtIvaMinimoCampo"|| query.value(8).toString()=="txtIvaBasicoCampo"|| query.value(8).toString()=="txtIvaTotalCampo"
                                             || query.value(8).toString()=="txtTotalCampo"|| query.value(8).toString()=="txtSubTotalCampo"){
                                        if(!queryPie.value(0).toString().trimmed().isEmpty()){
                                            _montoAGrabar = QString::number(queryPie.value(0).toDouble(),'f',cantidadDecimalesMonto);
                                        }
                                        painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+_montoAGrabar+" "+query.value(7).toString());
                                    }else if(query.value(8).toString()=="txtCae_QrCode"){

                                        imagenQrString = queryPie.value(0).toString().toLatin1();
                                        imagenQr.loadFromData(QByteArray::fromBase64(imagenQrString));
                                        painter.drawImage(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(9).toDouble(),query.value(9).toDouble(),query.value(4).toBool()), imagenQr);

                                    }else if(query.value(8).toString()=="txtCae_urlCode"){

                                        painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+func_CFE_ParametrosGenerales.retornaValor("urlDGI")+" "+query.value(7).toString());
                                    }
                                    else{
                                        if(!queryPie.value(0).toString().trimmed().isEmpty()){
                                            _montoAGrabar=queryPie.value(0).toString();
                                        }
                                        painter.drawText(cuadroA4(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+_montoAGrabar+" "+query.value(7).toString());
                                    }
                                }
                                if(query.value(8).toString()=="txtTextoGenericoPie1" || query.value(8).toString()=="txtTextoGenericoPie2" || query.value(8).toString()=="txtTextoGenericoPie3" || query.value(8).toString()=="txtTextoGenericoPie4"){

                                    painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+query.value(7).toString());

                                }else if(query.value(8).toString()=="txtSubTotalSinDescuentoCampo" || query.value(8).toString()=="txtTotalDescuentoAlTotal" || query.value(8).toString()=="txtIvaExentoCampo"
                                         || query.value(8).toString()=="txtIvaMinimoCampo"|| query.value(8).toString()=="txtIvaBasicoCampo"|| query.value(8).toString()=="txtIvaTotalCampo"
                                         || query.value(8).toString()=="txtTotalCampo"|| query.value(8).toString()=="txtSubTotalCampo"

                                         ){
                                    if(!queryPie.value(0).toString().trimmed().isEmpty()){
                                        _montoAGrabar = QString::number(queryPie.value(0).toDouble(),'f',cantidadDecimalesMonto);
                                    }
                                    painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+_montoAGrabar+" "+query.value(7).toString());
                                }else if(query.value(8).toString()=="txtCae_QrCode"){

                                    imagenQrString = queryPie.value(0).toString().toLatin1();
                                    imagenQr.loadFromData(QByteArray::fromBase64(imagenQrString));
                                    painter.drawImage(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(9).toDouble(),query.value(9).toDouble(),query.value(4).toBool()), imagenQr);

                                }else if(query.value(8).toString()=="txtCae_urlCode"){

                                    painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+func_CFE_ParametrosGenerales.retornaValor("urlDGI")+" "+query.value(7).toString());
                                }
                                else{
                                    if(!queryPie.value(0).toString().trimmed().isEmpty()){
                                        _montoAGrabar=queryPie.value(0).toString();
                                    }
                                    painter.drawText(cuadro(query.value(0).toDouble(),query.value(1).toDouble(),query.value(2).toDouble(),1,query.value(4).toBool()),query.value(3).toInt(),query.value(6).toString()+" "+_montoAGrabar+" "+query.value(7).toString());
                                }

                            }
                        }
                    }
                }else{return false;}

            }
            painter.end();
        }

        return true;

    }
}





//################################################################################
//######## Calcula en centimetros la posicion de los campos a imprimirse #########
//################################################################################
QRectF cuadro(double x, double y, double ancho, double alto,bool justifica=false){

    QRectF punto(x*centimetro,y*centimetro,ancho*centimetro,alto*centimetro);
    if(justifica){

        if(x>ancho)
            punto=QRect((x*centimetro)-(ancho*centimetro),y*centimetro,ancho*centimetro,alto*centimetro);

    }
    return punto;
}
QRectF cuadroA4(double x, double y, double ancho, double alto,bool justifica=false){

    QRectF punto(x*centimetro,(y+15)*centimetro,ancho*centimetro,alto*centimetro);
    if(justifica){

        if(x>ancho)
            punto=QRect((x*centimetro)-(ancho*centimetro),(y+15)*centimetro,ancho*centimetro,alto*centimetro);

    }
    return punto;
}




bool procesarImix(QString _codigoDocumento,QString _codigoTipoDocumento){

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    QString str1 = crearJsonIMIX(_codigoDocumento, _codigoTipoDocumento);



    funcion.loguear("Modo CFE Imix: \n\nEnvio a Imix:\n"+str1+"\n\n");

    if(str1=="-1"){

        funcion.loguear("ERROR: \nNo existe documento para enviar como CFE");
        funcion.mensajeAdvertenciaOk("ERROR: \nNo existe documento para enviar como CFE");
        return false;
    }else if(str1=="-2"){
        funcion.loguear("ERROR: \nNo se puede ejecutar la consulta a la base de datos");
        funcion.mensajeAdvertenciaOk("ERROR: \nNo se puede ejecutar la consulta a la base de datos");
        return false;
    }else if(str1=="-3"){
        funcion.loguear("ERROR: \nNo hay conexión con la base de datos");
        funcion.mensajeAdvertenciaOk("ERROR: \nNo hay conexión con la base de datos");
        return false;
    }


    numeroDocumentoV=_codigoDocumento;
    codigoTipoDocumentoV=_codigoTipoDocumento;

    //  qDebug()<<str1;

    QByteArray baddddd = str1.toUtf8();
    const char *mensajeAEnviarPost = baddddd.data();


    // qDebug()<< mensajeAEnviarPost;

    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl = curl_easy_init();
    if(curl) {

        resultadoFinal="";


        QByteArray testing =    func_CFE_ParametrosGenerales.retornaValor("urlImixTesting").toLatin1();
        QByteArray produccion = func_CFE_ParametrosGenerales.retornaValor("urlImixProduccion").toLatin1();


        const char *c_conexion;
        if(func_CFE_ParametrosGenerales.retornaValor("modoTrabajoCFE")=="0"){
            c_conexion=testing.data();
        }else{
            c_conexion=produccion.data();
        }

        curl_easy_setopt(curl, CURLOPT_URL, c_conexion);


        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST,1);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(mensajeAEnviarPost));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, mensajeAEnviarPost);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);


        res = curl_easy_perform(curl);

        if(res != CURLE_OK){

            QString resultadoError=curl_easy_strerror(res);

            if(resultadoError.trimmed()=="Couldn't resolve host name" || resultadoError.trimmed()=="Couldn't connect to server"){
                //  funcion.loguear("ERROR: No hay conexión con Imix: \n\n"+(QString)c_conexion);
                funcion.mensajeAdvertenciaOk("ERROR: No hay conexión con Imix: \n\n"+(QString)c_conexion);
            }else{
                //  funcion.loguear("ERROR: "+resultadoError+"\n\nConexión: \n\n"+(QString)c_conexion);
                funcion.mensajeAdvertenciaOk("ERROR: "+resultadoError+"\n\nConexión: \n\n"+(QString)c_conexion);
            }

            return false;
        }
        else {

            resultadoFinal =  resultadoFinal.remove(resultadoFinal.length()-1,1);
            //   qDebug() <<resultadoFinal;
            //   qDebug() <<"Salio todo divino";
            return true;

        }
        curl_easy_cleanup(curl);
    }
    else{
        return false;
    }
    curl_global_cleanup();



}





QString crearJsonIMIX(QString _codigoDocumento,QString _codigoTipoDocumento){

    QString consultaSql = " SELECT ";
    consultaSql +=" TD.descripcionTipoDocumentoCFE'Comprobante', ";
    consultaSql +=" DOC.serieDocumento'Serie', ";
    consultaSql +=" DOC.codigoDocumento'Numero', ";
    consultaSql +=" UNIX_TIMESTAMP(STR_TO_DATE(DOC.fechaEmisionDocumento, '%Y-%m-%d '))'Fecha', ";
    consultaSql +=" case when DOC.tipoCliente=1 then  DOC.codigoCliente else 'null' end 'Cliente', ";
    consultaSql +=" case when DOC.tipoCliente=2 then  DOC.codigoCliente else 'null' end 'Proveedor', ";
    consultaSql +=" left(trim(CLI.razonSocial),32) 'RazonSocial', ";
    consultaSql +=" left(trim(CLI.direccion),32) 'Direccion',     ";

    consultaSql +=" case when CLI.rut is null then 'null' else case when CLI.codigoTipoDocumentoCliente=2 then 'null' else case when CLI.codigoTipoDocumentoCliente in (3,4,5,6) and (CLI.rut is not null and trim(CLI.rut)!='') then  CLI.rut else 'null'end end end 'Documento',";
    consultaSql +=" case when CLI.codigoTipoDocumentoCliente in (4,5,6) then PA.codigoIsoPais else 'null' end 'PaisDocumento',";
    consultaSql +=" case when CLI.codigoTipoDocumentoCliente in (4,5,6) then CLI.codigoTipoDocumentoCliente else 'null' end 'TipoDocumento',";
    consultaSql +=" case when CLI.rut is null then 'null' else case when CLI.codigoTipoDocumentoCliente = 2 then CLI.rut else 'null' end end 'Rut',";


    //consultaSql +=" case when CLI.rut is null then 'null' else CLI.rut end 'Documento', ";
    //consultaSql +=" case when CLI.rut is null then 'null' else case when CLI.codigoTipoDocumentoCliente in (2,3) then 'null' else PA.codigoIsoPais end end 'PaisDocumento', ";
    //consultaSql +=" case when CLI.rut is null then 'null' else case when CLI.codigoTipoDocumentoCliente in (2,3) then 'null' else CFETDC.descripcionTipoDocumentoCliente end end 'TipoDocumento', ";
    //consultaSql +=" case when CLI.rut is null then 'null' else case when CLI.codigoTipoDocumentoCliente = 2 then CLI.rut else 'null' end end 'Rut', ";

    consultaSql +=" MO.codigoISO4217'Moneda', ";
    consultaSql +=" left(DOC.porcentajeDescuentoAlTotal/100,6)'PorcentajeDescuento',     ";
    consultaSql +=" DOC.precioTotalVenta'Total',    ";

    consultaSql +=" DL.codigoArticulo'Articulo', ";
    consultaSql +=" left(trim(AR.descripcionArticulo),32)'Descripcion', ";
    consultaSql +=" DL.cantidad'Cantidad', ";
    consultaSql +=" DL.precioArticuloUnitario'Precio', ";
    consultaSql +=" concat('N° ',DOC.codigoDocumento, ' - ',left(trim(DOC.observaciones),48))'Observaciones',  ";
    consultaSql +=" TD.TipoTrasladoRemito'TipoTrasladoRemito'  ";


    consultaSql +=" FROM DocumentosLineas DL      ";
    consultaSql +=" join Documentos DOC on DOC.codigoDocumento=DL.codigoDocumento and DOC.codigoTipoDocumento=DL.codigoTipoDocumento ";
    consultaSql +=" join Articulos AR on AR.codigoArticulo=DL.codigoArticulo ";
    consultaSql +=" join TipoDocumento TD on TD.codigoTipoDocumento=DOC.codigoTipoDocumento ";
    consultaSql +=" join Monedas MO on MO.codigoMoneda = DOC.codigoMonedaDocumento ";
    consultaSql +=" join Clientes CLI on CLI.codigoCliente=DOC.codigoCliente and CLI.tipoCliente=DOC.tipoCliente ";
    consultaSql +=" join CFE_TipoDocumentoCliente CFETDC on CFETDC.codigoTipoDocumentoCliente=CLI.codigoTipoDocumentoCliente ";
    consultaSql +=" join Pais PA on PA.codigoPais=CLI.codigoPais ";
    consultaSql +=" where DOC.codigoDocumento="+_codigoDocumento+" and DOC.codigoTipoDocumento="+_codigoTipoDocumento+" and DOC.esDocumentoCFE='1' ";


    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec(consultaSql)) {
            if(query.first()){
                //  QString j=  "{\"Cliente\":\"1\",\"Comprobante\":\"Venta Contado\",\"Direccion\":\"Direccion del cliente\",\"Emitir\":true,\"Fecha\":\"/Date(1504666800)/\",\"Lineas\":[{\"Articulo\":\"2\",\"Cantidad\":1,\"Descripcion\":\"pepe\",\"Precio\":150},{\"Articulo\":\"1\",\"Cantidad\":1,\"Descripcion\":\"pepe\",\"Precio\":150}],\"Moneda\":\"UYU\",\"Numero\":\"207\",\"Observaciones\":\"\",\"PaisDocumento\":\"UY\",\"PorcentajeDescuento\":0.0,\"Proveedor\":\"10\",\"RazonSocial\":\"Demostracion\",\"Rut\":\"222245432128\",\"Serie\":\"B\",\"TipoDeTraslado\":1,\"TipoDocumento\":0,\"Total\":300}";

                //Variables cabezal:
                QString Comprobante=query.value(0).toString().replace("\n","");
                //    QString Serie=query.value(1).toString();
                //    QString Numero=query.value(2).toString();
                QString Fecha=query.value(3).toString().replace("\n","");
                QString Cliente=query.value(4).toString().replace("\n","");
                QString Proveedor=query.value(5).toString().replace("\n","");
                QString RazonSocial=query.value(6).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");



                QString Direccion=query.value(7).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");
                QString Documento=query.value(8).toString().replace("\n","");
                QString PaisDocumento=query.value(9).toString().replace("\n","");
                QString TipoDocumento=query.value(10).toString().replace("\n","");
                QString Rut=query.value(11).toString().replace("\n","");
                QString Moneda=query.value(12).toString().replace("\n","");
                QString PorcentajeDescuento=query.value(13).toString();
                QString Total=query.value(14).toString();
                QString Observacion=query.value(19).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");
                QString TipoTrasladoRemito=query.value(20).toString();

                QString ClienteOProveedor="Cliente";
                QString DatoDocumento=",\"Documento\":\""+Documento+"\"";
                QString DatoPaisDocumento=",\"PaisDocumento\":\""+PaisDocumento+"\"";
                QString DatoTipoDocumento=",\"TipoDocumento\":"+TipoDocumento+"";
                QString DatoRut= ",\"Rut\":\""+Rut+"\"";

                QString DatoFecha= ",\"Fecha\":\"/Date("+Fecha+")/\"";

                QString DatoTipoDeTraslado=",\"TipoDeTraslado\":"+TipoTrasladoRemito+"";






                DatoFecha="";

                if(Cliente=="null"){
                    Cliente=Proveedor;
                    ClienteOProveedor="Proveedor";
                }

                if(func_CFE_ParametrosGenerales.retornaValor("envioArticuloClienteGenerico")=="1"){
                    Cliente="1";
                }


                if(Documento=="null")
                    DatoDocumento="";

                if(PaisDocumento=="null")
                    DatoPaisDocumento="";

                if(TipoDocumento=="null")
                    DatoTipoDocumento="";


                if(TipoTrasladoRemito=="0")
                    DatoTipoDeTraslado="";

                if(Rut=="null")
                    DatoRut="";

                QString cabezal = "{\""+ClienteOProveedor+"\":\""+Cliente+"\",\"Comprobante\":\""+Comprobante+"\",\"Direccion\":\""+Direccion+"\",\"Emitir\":true "+DatoFecha+" ";

                QString pie = ""+DatoDocumento+",\"Moneda\":\""+Moneda+"\",\"Observaciones\":\""+Observacion+"\""+DatoPaisDocumento+",\"PorcentajeDescuento\":"+PorcentajeDescuento+",\"RazonSocial\":\""+RazonSocial+"\""+DatoRut+DatoTipoDeTraslado+DatoTipoDocumento+",\"Total\":"+Total+"}";

                //,\"Serie\":\""+Serie+"\"

                //\"Numero\":\""+Numero+"\"

                QString Lineas= " ,\"Lineas\":[ ";

                query.previous();
                int i=0;
                QString codigoArticuloCargado="";

                while (query.next()){

                    if(i!=0){
                        Lineas += " , ";
                    }
                    codigoArticuloCargado=query.value(15).toString();

                    if(func_CFE_ParametrosGenerales.retornaValor("envioArticuloClienteGenerico")=="1"){
                        codigoArticuloCargado="1";
                    }

                    Lineas += "{\"Articulo\":\""+codigoArticuloCargado+"\",\"Cantidad\":"+query.value(17).toString()+",\"Descripcion\":\""+query.value(16).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","")+"\",\"Precio\":"+query.value(18).toString()+"}";
                    i=1;

                }
                Lineas += "] ";

                QString ok=cabezal + Lineas + pie;

                return ok;//QString::fromUtf8(ok.toUtf8());

            }else{
                return "-1"; //No existe documento para enviar como CFE
            }
        }else{
            return "-2";//No se puede ejecutar la consulta a la base de datos
        }
    }else{
        return "-3"; //No hay conexión con la base de datos
    }
}




bool procesarDynamia(QString _codigoDocumento,QString _codigoTipoDocumento,QString _numeroDocumentoCFEADevolver,QString _fechaDocumentoCFEADevolver, QString tipoDocumentoCFEADevolver){

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    QString str1 = crearJsonDynamia(_codigoDocumento, _codigoTipoDocumento,_numeroDocumentoCFEADevolver,_fechaDocumentoCFEADevolver, tipoDocumentoCFEADevolver);


    funcion.loguear("Modo CFE Dynamia: \n\nEnvio a Dynamia:\n"+str1+"\n\n");

    if(str1=="-1"){
        funcion.mensajeAdvertenciaOk("ERROR: \nNo existe documento para enviar como CFE");
        return false;
    }else if(str1=="-2"){
        funcion.mensajeAdvertenciaOk("ERROR: \nNo se puede ejecutar la consulta a la base de datos");
        return false;
    }else if(str1=="-3"){
        funcion.mensajeAdvertenciaOk("ERROR: \nNo hay conexión con la base de datos");
        return false;
    }else if(str1=="-4"){
        funcion.mensajeAdvertenciaOk("ERROR: \nEl documento PASSAPORTE es obligatorio");
        return false;
    }else if(str1=="-5"){
        funcion.mensajeAdvertenciaOk("ERROR: \nDocumento DNI es obligatorio para el pais del cliente");
        return false;
    }else if(str1=="-6"){
        funcion.mensajeAdvertenciaOk("ERROR: \nMonto mayor a 10000UI, se requiere documento");
        return false;
    }else if(str1=="-7"){
        funcion.mensajeAdvertenciaOk("ERROR: \nError en tipo de documento CFE en khitomer.TipoDocumento");
        return false;
    }else if(str1=="-8"){
        funcion.mensajeAdvertenciaOk("ERROR: \nNo se puede anular un documento que no fue emitido como CFE.");
        return false;
    }


    numeroDocumentoV=_codigoDocumento;
    codigoTipoDocumentoV=_codigoTipoDocumento;

    qDebug()<<str1;

    QByteArray baddddd = str1.toUtf8();
    const char *mensajeAEnviarPost = baddddd.data();

    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");

    curl = curl_easy_init();
    if(curl) {

        resultadoFinal="";

        QByteArray produccion = func_CFE_ParametrosGenerales.retornaValor("urlWS").toLatin1()+tipoDeCFEAEnviarDynamiaV.toLatin1();

        const char *c_produccion = produccion.data();

        curl_easy_setopt(curl, CURLOPT_URL, c_produccion);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST,1);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(mensajeAEnviarPost));
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, mensajeAEnviarPost);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerDynamia);


        res = curl_easy_perform(curl);

        if(res != CURLE_OK){

            QString resultadoError=curl_easy_strerror(res);

            if(resultadoError.trimmed()=="Couldn't resolve host name" || resultadoError.trimmed()=="Couldn't connect to server"){
                funcion.mensajeAdvertenciaOk("ERROR: No hay conexión con Dynamia: \n\n"+(QString)produccion.data());
            }else{
                funcion.mensajeAdvertenciaOk("ERROR: "+resultadoError+"\n\nConexión: \n\n"+(QString)produccion.data());
            }

            return false;
        }
        else {

            // resultadoFinal =  resultadoFinal.remove(resultadoFinal.length()-1,1);

            //        qDebug()<<resultadoFinal;

            bool ok;
            // json is a QString containing the JSON data
            QtJson::JsonObject result = QtJson::parse(resultadoFinal, ok).toMap();

            if(!ok) {
                funcion.loguear("Respuesta Dynamia:\n"+resultadoFinal);
                funcion.mensajeAdvertenciaOk("Error en parseo de información desde Dynamia:\n\nMensaje devuelto: "+resultadoFinal);
                return false;
            }

            if(result["status"].toString().toLower()=="true"){

                QtJson::JsonObject nested = result["result"].toMap();
                QtJson::JsonObject nested2 = nested["cae"].toMap();


                /*qDebug() << "QR:" << nested["qr"].toString();
                qDebug() << "idDocGaia:" << nested["idDocGaia"].toString();*/



                /*qDebug() << "CAE_ID:" << nested2["cae_id"].toString();
                qDebug() << "NRO:" << nested2["nro"].toString();
                qDebug() << "SERIE:" << nested2["serie"].toString();
                qDebug() << "DESDE:" << nested2["desde"].toString();
                qDebug() << "HASTA:" << nested2["hasta"].toString();
                qDebug() << "VENCIMIENTO:" << nested2["vencimiento"].toString();
                qDebug() << "CODIGO_SEGURIDAD:" << nested2["cod_seguridad"].toString();*/


                ModuloDocumentos modeloDocumento;

                if(modeloDocumento.actualizarInformacionCFEDocumentoDynamia(_codigoDocumento, _codigoTipoDocumento,
                                                                            nested2["nro"].toString(),
                                                                            nested2["serie"].toString(),
                                                                            nested2["vencimiento"].toString(),
                                                                            nested2["cod_seguridad"].toString(),
                                                                            nested2["cae_id"].toString(),
                                                                            nested2["desde"].toString(),
                                                                            nested2["hasta"].toString(),
                                                                            nested["qr"].toString(),
                                                                            nested["idDocGaia"].toString(),
                                                                            caeTipoDocumentoCFEDescripcionV
                                                                            )){



                    return true;
                }else{
                    return false;
                }



            }else{

                QtJson::JsonObject nested = result["result"].toMap();

                funcion.mensajeAdvertenciaOk("ERROR: \n"+nested["err_msg"].toString());

                return false;
            }
        }
        curl_easy_cleanup(curl);
    }
    else{
        return false;
    }
    curl_global_cleanup();



}









QString crearJsonDynamia(QString _codigoDocumento, QString _codigoTipoDocumento,QString _numeroDocumentoCFEADevolver,QString _fechaDocumentoCFEADevolver,QString tipoDocumentoCFEADevolver){

    QString consultaSql = " SELECT ";

    consultaSql +=" STR_TO_DATE(DOC.fechaEmisionDocumento, '%Y-%m-%d ')'fecha',  ";
    consultaSql +=" MO.codigoISO4217'iso_moneda', ";
    consultaSql +=" (SELECT case when valorConfiguracion='1' then '1' else '0' end FROM Configuracion where codigoConfiguracion='MODO_CALCULOTOTAL')'montos_brutos', ";
    consultaSql +=" '0' as 'vencimiento', ";
    consultaSql +=" DOC.codigoCliente'id_receptor', ";
    consultaSql +=" left(trim(CLI.razonSocial),45) 'razon_social', ";
    consultaSql +=" PA.codigoIsoPais'iso_pais', ";
    consultaSql +=" left(trim(CLI.nombreCliente),45) 'nombre', ";
    consultaSql +=" left(trim(DEP.descripcionDepartamento),29) 'ciudad', ";
    consultaSql +=" left(trim(CLI.direccion),45) 'direccion', ";
    consultaSql +=" left(trim(CLI.email),39)'e_mail', ";
    consultaSql +=" left(trim(CLI.telefono),19)'telefono', ";
    consultaSql +=" CLI.codigoTipoDocumentoCliente'tipoDocumento',  ";
    consultaSql +=" CLI.rut'numeroDocumento', ";

    consultaSql +=" case when DL.precioArticuloUnitario=0 then '5' else IVA.indicadorFacturacionCFE end 'indicador_facturacion', ";
    consultaSql +=" left(trim(AR.descripcionArticulo),32)'descripcion', ";
    consultaSql +=" DL.cantidad'cantidad', ";
    consultaSql +=" 'N/A' as 'unidad', ";
    consultaSql +=" CAST(DOC.porcentajeDescuentoAlTotal AS DECIMAL(10,3)) 'descuento', ";
    consultaSql +=" '1' as 'tipo_descuento', ";
    consultaSql +=" case when IVA.indicadorFacturacionCFE=4 then CAST(IVA.porcentajeIva as DECIMAL(20,2)) else 'null' end 'ivaArticulo', ";
    consultaSql +=" CAST(DL.precioArticuloUnitario AS DECIMAL(20,3))'precio_unitario', ";
    consultaSql +=" CAST((DL.cantidad*DL.precioArticuloUnitario) -  (((DL.cantidad*DL.precioArticuloUnitario)*((DOC.porcentajeDescuentoAlTotal/100)+1))-(DL.cantidad*DL.precioArticuloUnitario))  AS DECIMAL(20,3))'totalLinea', ";
    consultaSql +=" CAST(DOC.precioSubTotalVenta AS DECIMAL(20,3))'subtotal', ";


    consultaSql +=" CAST(DOC.totalIva3 AS DECIMAL(20,3))'exento', ";
    consultaSql +=" CAST(DOC.totalIva2 AS DECIMAL(20,3))'minima', ";
    consultaSql +=" CAST(DOC.totalIva1 AS DECIMAL(20,3))'basica', ";
    consultaSql +=" case when IVA.indicadorFacturacionCFE=4 then CAST(DL.precioIvaArticulo AS DECIMAL(20,3)) else 'null' end 'otro', ";
    consultaSql +=" CAST(DOC.precioTotalVenta AS DECIMAL(20,3))'total', ";
    consultaSql +=" TD.descripcionTipoDocumentoCFE'TipoDocumentoCFE', ";

    consultaSql +=" case when DOC.codigoMonedaDocumento=1 then    ";
    consultaSql +="          case when DOC.precioTotalVenta > (SELECT 10000*valorParametro FROM CFE_ParametrosGenerales where nombreParametro='montoUI') then 'mayor' else 'menor' end ";
    consultaSql +=" else  ";
    consultaSql +="          case when (DOC.precioTotalVenta*MO.cotizacionMoneda) > (SELECT 10000*valorParametro FROM CFE_ParametrosGenerales where nombreParametro='montoUI') then 'mayor' else 'menor' end ";
    consultaSql +=" end 'UI', ";

    consultaSql +=" case when IVA.indicadorFacturacionCFE=4 then CAST(IVA.porcentajeIva as DECIMAL(20,2)) else 'null' end 'ivaOtroPorcentaje', ";


    consultaSql +=" CAST(DOC.precioTotalVenta AS DECIMAL(20,3))-(CAST(DOC.precioSubTotalVenta AS DECIMAL(20,3))+CAST(DOC.precioIvaVenta AS DECIMAL(20,3))) 'Redondeo',";

    consultaSql +=" case when AR.codigoIva=IVA.codigoIva and IVA.indicadorFacturacionCFE=1 then CAST((DL.cantidad*DL.precioArticuloUnitario) -  (((DL.cantidad*DL.precioArticuloUnitario)*((DOC.porcentajeDescuentoAlTotal/100)+1))-(DL.cantidad*DL.precioArticuloUnitario))  AS DECIMAL(20,3)) else 0 end 'Exento2'";


    consultaSql +=" FROM DocumentosLineas DL  ";

    consultaSql +=" join Documentos DOC on DOC.codigoDocumento=DL.codigoDocumento and DOC.codigoTipoDocumento=DL.codigoTipoDocumento ";
    consultaSql +=" join Articulos AR on AR.codigoArticulo=DL.codigoArticulo ";
    consultaSql +=" join TipoDocumento TD on TD.codigoTipoDocumento=DOC.codigoTipoDocumento ";
    consultaSql +=" join Monedas MO on MO.codigoMoneda = DOC.codigoMonedaDocumento ";
    consultaSql +=" join Clientes CLI on CLI.codigoCliente=DOC.codigoCliente and CLI.tipoCliente=DOC.tipoCliente ";
    consultaSql +=" join CFE_TipoDocumentoCliente CFETDC on CFETDC.codigoTipoDocumentoCliente=CLI.codigoTipoDocumentoCliente ";
    consultaSql +=" join Pais PA on PA.codigoPais=CLI.codigoPais ";
    consultaSql +=" join Departamentos DEP on DEP.codigoDepartamento=CLI.codigoDepartamento and DEP.codigoPais=CLI.codigoPais ";
    consultaSql +=" join Ivas IVA on IVA.codigoIva=AR.codigoIva ";
    consultaSql +=" where DOC.codigoDocumento="+_codigoDocumento+" and DOC.codigoTipoDocumento="+_codigoTipoDocumento+" and DOC.esDocumentoCFE='1' ";


    bool conexion=true;
    Database::chequeaStatusAccesoMysql();
    if(!Database::connect().isOpen()){
        if(!Database::connect().open()){
            qDebug() << "No conecto";
            conexion=false;
        }
    }
    if(conexion){
        QSqlQuery query(Database::connect());
        if(query.exec(consultaSql)) {
            if(query.first()){


                //Cabezal
                QString fecha=query.value(0).toString();
                QString iso_moneda=query.value(1).toString().replace("\n","");
                QString montos_brutos=query.value(2).toString();
                QString vencimientos=query.value(3).toString();
                QString id_receptor=query.value(4).toString();
                QString razon_social=query.value(5).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");
                QString iso_pais=query.value(6).toString().replace("\n","");
                QString nombre=query.value(7).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");
                QString ciudad=query.value(8).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");
                QString direccion=query.value(9).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");
                QString email=query.value(10).toString().replace("\"","\\\"").replace("\n","");
                QString telefono=query.value(11).toString().replace("\"","\\\"").replace("\n","");
                QString tipoDocumento=query.value(12).toString().replace("\n","");
                QString numeroDocumento=query.value(13).toString().replace("\n","");




                //totales
                QString subtotal=QString::number(query.value(23).toFloat()+query.value(32).toFloat(),'f',3);
                QString exento=query.value(24).toString();
                QString minima=query.value(25).toString();
                QString basica=query.value(26).toString();
                QString otro=query.value(27).toString();
                QString total=query.value(28).toString();
                QString tipoDocumentoCFE=query.value(29).toString();
                QString ui=query.value(30).toString();

                QString redondeo=query.value(32).toString();

                float _redondeoMonto=query.value(32).toFloat();

                QString _OtroIva = ",\""+query.value(31).toString()+"\":"+otro+"";

                QString _Rut=",\"rut\":\""+numeroDocumento.replace("\"","\\\"")+"\"";
                QString _Documento_identidad= ",\"documento_identidad\":{\"tipo\":"+tipoDocumento+",\"numero\":\""+numeroDocumento+"\"}";

                QString _Vencimiento="\"vencimiento\":"+vencimientos+",";

                //Verifico tipo de documento
                if(tipoDocumentoCFE!="Venta credito" && tipoDocumentoCFE!="Nota credito venta"){
                    _Vencimiento="";
                }



                if(tipoDocumentoCFE=="Venta contado"){

                    caeTipoDocumentoCFEDescripcionV="e-Ticket";
                    tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-ticketContado");

                }else if(tipoDocumentoCFE=="Venta credito"){

                    caeTipoDocumentoCFEDescripcionV="e-Ticket";
                    tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-ticketCredito");

                }else if(tipoDocumentoCFE=="Devolucion venta"){

                    caeTipoDocumentoCFEDescripcionV="Nota de Crédito de e-Ticket";
                    tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-ticketNotaCredito");

                }else if(tipoDocumentoCFE=="Nota credito venta"){

                    caeTipoDocumentoCFEDescripcionV="Nota de Crédito de e-Ticket";
                    tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-ticketNotaCredito");

                }else{
                    return "-7"; //Error en tipo documento CFE en khitomer.TipoDocumento
                }


                //Verifico si es cliente con RUT
                if(tipoDocumento=="2"){
                    //Si es rut no puede ser otro documento
                    _Documento_identidad="";

                    if(tipoDocumentoCFE=="Venta contado"){

                        caeTipoDocumentoCFEDescripcionV="e-Factura";
                        tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-facturaContado");

                    }else if(tipoDocumentoCFE=="Venta credito"){

                        caeTipoDocumentoCFEDescripcionV="e-Factura";
                        tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-facturaCredito");

                    }else if(tipoDocumentoCFE=="Devolucion venta"){

                        caeTipoDocumentoCFEDescripcionV="Nota de Crédito de e-Factura";
                        tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-facturaNotaCredito");

                    }else if(tipoDocumentoCFE=="Nota credito venta"){

                        caeTipoDocumentoCFEDescripcionV="Nota de Crédito de e-Factura";
                        tipoDeCFEAEnviarDynamiaV=func_CFE_ParametrosGenerales.retornaValor("urlE-facturaNotaCredito");

                    }else{
                        return "-7"; //Error en tipo documento CFE en khitomer.TipoDocumento
                    }



                }else if(tipoDocumento=="5"){    ///PASSAPORTE
                    _Rut="";
                    //Si es passaporte tiene que ir el documento
                    if(numeroDocumento=="null"){
                        funcion.mensajeAdvertenciaOk("Error: El documento PASSAPORTE es obligatorio");
                        return "-4";//Documento passaporte es obligatorio
                    }
                }else if(tipoDocumento=="6"){   ///DNI
                    _Rut="";
                    //Si es DNI chequeo si lo muestro segun el pais
                    if(iso_pais!="AR" && iso_pais!="BR" && iso_pais!="CL" && iso_pais!="PY"){
                        if(ui=="menor"){
                            _Documento_identidad="";
                        }else{
                            if(numeroDocumento=="null"){
                                return "-6"; //Monto mayor a 10000UI, se requiere documento
                            }
                        }
                    }else{
                        if(numeroDocumento=="null"){
                            return "-5";//Documento DNI es obligatorio para el pais del cliente
                        }
                    }
                }else{
                    ///otro documento
                    _Rut="";
                    if(ui=="menor"){
                        _Documento_identidad="";
                    }else{
                        if(numeroDocumento=="null"){
                            return "-6"; //Monto mayor a 10000UI, se requiere documento
                        }
                    }
                }


                if(otro=="null"){
                    _OtroIva="";
                }


                //Seteo las lineas genericas
                QString Cabezal = "{\"data\":{\"documento\":{\"fecha\":\""+fecha+"\",\"iso_moneda\":\""+iso_moneda+"\",";
                QString MontoBrutos = "\"montos_brutos\":"+montos_brutos+", "+_Vencimiento+" ";

                QString Receptor= "\"receptor\":{\"iso_pais\":\""+iso_pais+"\",\"ciudad\":\""+ciudad+"\",\"direccion\":\""+direccion+"\",\"e_mail\":\""+email+"\",\"id_receptor\":\""+id_receptor+"\",\"razon_social\":\""+razon_social+"\",\"nombre\":\""+nombre+"\""+_Rut+""+_Documento_identidad+" ,\"telefono\":\""+telefono+"\"},";


                QString Referencias = "\"referencias\":[{\"nro_doc\":"+_numeroDocumentoCFEADevolver.trimmed()+",\"doc_sin_codificar\":false,\"fecha\":\""+_fechaDocumentoCFEADevolver.trimmed()+"\"}],";


                if(tipoDocumentoCFE!="Devolucion venta" && tipoDocumentoCFE!="Nota credito venta"){

                    Referencias="";

                }else{
                    if(_numeroDocumentoCFEADevolver.trimmed()=="" || _numeroDocumentoCFEADevolver.trimmed().toLower()=="null" || _numeroDocumentoCFEADevolver.trimmed()==NULL){
                        return "-8"; //No se puede devolver un documento que no fue emitido como cfe
                    }
                }




                QString User_info = "\"user_info\":\""+func_CFE_ParametrosGenerales.retornaValor("claveCifrada")+"\"},";

                QString Token= "\"token\":\""+func_CFE_ParametrosGenerales.retornaValor("token")+"\"} ";



                QString Lineas= "\"lineas\":[ ";

                query.previous();
                int i=0;

                QString descripcionArticulo="";

                QString IVA="";

                float _exentoTotal=0;

                while (query.next()){

                    if(i!=0){
                        Lineas += " , ";
                    }

                    if(query.value(14).toString()=="4"){
                        IVA=",\"iva\":"+query.value(20).toString()+"";
                    }else{
                        IVA="";
                    }

                    descripcionArticulo=query.value(15).toString().toUpper().replace("Á","A").replace("É","E").replace("Í","I").replace("Ó","O").replace("Ú","U").replace("\"","\\\"").replace("\n","");

                    Lineas += "{\"cantidad\":"+query.value(16).toString()+",\"descripcion\":\""+descripcionArticulo+"\",\"indicador_facturacion\":"+query.value(14).toString()+""+IVA+",\"precio_unitario\":"+query.value(21).toString()+",\"total\":"+query.value(22).toString()+",\"unidad\":\"NA\",\"descuento\":"+query.value(18).toString()+",\"tipo_descuento\":"+query.value(19).toString()+"}";
                    i=1;

                    //Calculo el exento segun el monto de los articulos
                    _exentoTotal+=query.value(33).toFloat();

                }

                exento = QString::number(_exentoTotal,'f',3);


                //chequeo si es devolucion o no, para verificar segun el documento original los montos de redondeo
                if(Referencias!=""){
                    //Chequeo si hay redondeos y agrego la linea correspondiente
                    if(func_tipoDocumentos.retornaPermisosDelDocumento(tipoDocumentoCFEADevolver,"utilizaRedondeoEnTotal") && _redondeoMonto!=0){

                        //qDebug()<<_redondeoMonto;

                        if(_redondeoMonto<0){
                            Lineas += ", {\"cantidad\":1,\"descripcion\":\"Redondeo\",\"indicador_facturacion\":7,\"precio_unitario\":"+redondeo+",\"total\":"+redondeo+",\"unidad\":\"NA\",\"descuento\":0}";
                        }else{
                            Lineas += ", {\"cantidad\":1,\"descripcion\":\"Redondeo\",\"indicador_facturacion\":6,\"precio_unitario\":"+redondeo+",\"total\":"+redondeo+",\"unidad\":\"NA\",\"descuento\":0}";
                            //  qDebug()<<"Mayor a 0";
                        }

                    }
                }else{
                    //Chequeo si hay redondeos y agrego la linea correspondiente
                    if(func_tipoDocumentos.retornaPermisosDelDocumento(_codigoTipoDocumento,"utilizaRedondeoEnTotal") && _redondeoMonto!=0){

                        //qDebug()<<_redondeoMonto;

                        if(_redondeoMonto<0){
                            Lineas += ", {\"cantidad\":1,\"descripcion\":\"Redondeo\",\"indicador_facturacion\":7,\"precio_unitario\":"+redondeo+",\"total\":"+redondeo+",\"unidad\":\"NA\",\"descuento\":0}";
                        }else{
                            Lineas += ", {\"cantidad\":1,\"descripcion\":\"Redondeo\",\"indicador_facturacion\":6,\"precio_unitario\":"+redondeo+",\"total\":"+redondeo+",\"unidad\":\"NA\",\"descuento\":0}";
                            //  qDebug()<<"Mayor a 0";
                        }

                    }
                }



                Lineas += "], ";



                QString Totales= "\"totales\":{\"ivas\":";

                Totales+= "{\"basica\":"+basica+",\"exento\":"+exento+",\"minima\":"+minima+""+_OtroIva+"},";

                Totales+= "\"subtotal\":"+subtotal+",\"total\":"+total+"}},";

                return Cabezal + Lineas + MontoBrutos + Receptor + Referencias + Totales + User_info + Token; ;

            }else{
                return "-1"; //No existe documento para enviar como CFE
            }
        }else{

            qDebug()<< query.lastError();
            return "-2";//No se puede ejecutar la consulta a la base de datos
        }
    }else{
        return "-3"; //No hay conexión con la base de datos
    }
}





bool ModuloDocumentos::emitirDocumentoCFEImix(QString _codigoDocumento,QString _codigoTipoDocumento, QString _descripcionEstadoActualDocumento){

    //Consulto si el modo es 0 = imix, 1 = dynamia
    if(func_CFE_ParametrosGenerales.retornaValor("modoFuncionCFE")=="0"){

        bool resultadoprocesarImix= procesarImix(_codigoDocumento, _codigoTipoDocumento);

        //  qDebug()<< resultadoprocesarImix;

        //Si se procesa correctamente el cfe retorno true
        if(resultadoprocesarImix){
            return true;
        }else{

            qDebug() << "Estado del documento: ";
            qDebug() << _descripcionEstadoActualDocumento;

            if(_descripcionEstadoActualDocumento=="Pendiente"){

                actualizoEstadoDocumentoCFE(_codigoDocumento, _codigoTipoDocumento,"P");



                return false;
            }else{
                //   Si falla, intento eliminar el documento en la base
                bool resultado2 = eliminarDocumento(_codigoDocumento, _codigoTipoDocumento);

                if(resultado2){
                    return false;
                }else{
                    funcion.mensajeAdvertenciaOk("Hubo un error al intentar eliminar la informacion del documento");
                    return false;
                }
            }
        }

    }else{
        return false;
    }


}

bool ModuloDocumentos::emitirDocumentoCFEDynamia(QString _codigoDocumento,QString _codigoTipoDocumento,QString _numeroDocumentoCFEADevolver,QString _fechaDocumentoCFEADevolver,QString tipoDocumentoCFEADevolver, QString _descripcionEstadoActualDocumento){


    //Consulto si el modo es 0 = imix, 1 = dynamia
    if(func_CFE_ParametrosGenerales.retornaValor("modoFuncionCFE")=="1"){

        bool resultadoprocesarDynamia= procesarDynamia(_codigoDocumento, _codigoTipoDocumento,_numeroDocumentoCFEADevolver,_fechaDocumentoCFEADevolver,tipoDocumentoCFEADevolver);


        qDebug()<< resultadoprocesarDynamia;

        //Si se procesa correctamente el cfe retorno true
        if(resultadoprocesarDynamia){
            return true;
        }else{

            if(_descripcionEstadoActualDocumento=="Pendiente"){

                actualizoEstadoDocumentoCFE(_codigoDocumento, _codigoTipoDocumento,"P");

                return false;
            }else{

                //   Si falla, intento eliminar el documento en la base
                bool resultado2 = eliminarDocumento(_codigoDocumento, _codigoTipoDocumento);

                if(resultado2){
                    return false;
                }else{
                    funcion.mensajeAdvertenciaOk("Hubo un error al intentar eliminar la informacion del documento");
                    return false;
                }
            }
        }


    }else{
        return false;
    }


}
