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

// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "../Controles"

Rectangle{
    id: rectListaItem
    width: parent.width
    //width: 800
    height: 35
    color: "#e9e8e9"
    radius: 1
    border.width: 1
    border.color: "#aaaaaa"
    smooth: true
    opacity: 1

    property double montoDeuda: parseFloat(montoDeLaDeuda)
    property double montoTotalFactura: parseFloat(montoTotalFactura)

    property bool seleccionado: false


    signal clicSolo





    function reclamoSeleccionado(valor){
        if(valor==true){
            seleccionado=valor


        }else{
            seleccionado=valor


        }
        chbSeleccionado.setActivo(valor)
        modeloListaDocumentoConDeuda.setProperty(index,"checkboxActivo",valor)
    }


    Text {
        id: txtBanderaParaDeseleccionarRegistros
        text: BanderaParaDeseleccionarRegistros
        visible: false
        onTextChanged: {

            if(BanderaParaDeseleccionarRegistros=="1"){
                reclamoSeleccionado(false)            

            }else if(BanderaParaDeseleccionarRegistros=="2"){
                reclamoSeleccionado(true)

            }
        }
    }

    Text {
        id:txtNumeroFactura
        width: 80
        text: modeloListaTipoDocumentosComboBox.retornaDescripcionTipoDocumento(codigoTipoDocumento)+"("+numeroFactura+") - "+fechaFactura
        font.family: "Arial"
        opacity: 0.900
        smooth: true
        font.pointSize: 10
        font.bold: false
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 0
        color: "#080707"
        objectName: codigoTipoDocumento

    }

    MouseArea{
        id: mousearea1
        anchors.fill: parent
        z: 2
        hoverEnabled: true
        visible: true

        onClicked: {
            /*if ((mouse.button == Qt.LeftButton) && (mouse.modifiers & Qt.ShiftModifier)){
                reclamoSeleccionado(true)
                clicMasShift()
            }else if ((mouse.button == Qt.LeftButton) && (mouse.modifiers & Qt.ControlModifier)){
                reclamoSeleccionado(true)
                clicMasControl()
            }else{*/
                if(seleccionado){
                    reclamoSeleccionado(false)
                    clicSolo()
                }else{
                    reclamoSeleccionado(true)
                    clicSolo()
                }

             /*   if(chbSeleccionado.chekActivo){
                    chbSeleccionado.setActivo(false)
                    modeloListaDocumentoConDeuda.setProperty(index,"checkboxActivo",false)
                }else{
                    chbSeleccionado.setActivo(true)
                    modeloListaDocumentoConDeuda.setProperty(index,"checkboxActivo",true)
                }*/
            //}
        }

        /*
        onEntered: {
            rectListaItemColorDeseleccionado.stop()
            rectListaItemColorSeleccionado.start()
        }
        onExited: {
            rectListaItemColorSeleccionado.stop()
            rectListaItemColorDeseleccionado.start()
        }
        onClicked: {
            if(chbSeleccionado.chekActivo){
                chbSeleccionado.setActivo(false)
                modeloListaTarjetasCreditoACobrar.setProperty(index,"checkboxActivo",false)
            }else{
                chbSeleccionado.setActivo(true)
                modeloListaTarjetasCreditoACobrar.setProperty(index,"checkboxActivo",true)
            }
        }
        onDoubleClicked: {

            if(chbSeleccionado.chekActivo){
                chbSeleccionado.setActivo(false)
                modeloListaTarjetasCreditoACobrar.setProperty(index,"checkboxActivo",false)
            }else{
                chbSeleccionado.setActivo(true)
                modeloListaTarjetasCreditoACobrar.setProperty(index,"checkboxActivo",true)
            }


            if(tagFacturacion.enabled && funcionesmysql.mensajeAdvertencia("Se va a cargar la factura seleccionada, \nsi tiene datos en uso se perderan, desea continuar?\n\nPresione [ Sí ] para confirmar.")){

                mostrarMantenimientos(0,"derecha")

                mantenimientoFactura.cargarFactura(codigoDoc
                                                   ,codigoTipoDoc
                                                   ,modeloDocumentos.retornacodigoEstadoDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornatipoClienteDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornacodigoClienteDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaserieDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornacodigoVendedorComisionaDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornacodigoLiquidacionDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornacodigoVendedorLiquidacionDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornafechaEmisionDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornafechaEmisionDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornacodigoMonedaDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaprecioIvaVentaDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaprecioSubTotalVentaDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaprecioTotalVentaDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornatotalIva1Documento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornatotalIva2Documento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornatotalIva3Documento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaobservacionesDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaonumeroCuentaBancariaDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaocodigoBancoDocumento(codigoDoc,codigoTipoDoc)
                                                   ,modeloDocumentos.retornaMontoDescuentoTotalDocumento(codigoDoc,codigoTipoDoc)
                                                   )
            }
        }*/
    }

    PropertyAnimation{
        id:rectListaItemColorSeleccionado
        target: rectListaItem
        property: "color"
        from: "#e9e8e9"
        to:"#9294C6"
        duration: 100

    }
    PropertyAnimation{
        id:rectListaItemColorDeseleccionado
        target: rectListaItem
        property: "color"
        to: "#e9e8e9"
        from:"#9294C6"
        duration: 50

    }

    Text {
        id: txtMontoSaldoDeuda
        height: 16
        color: "#4a68b5"
        text: montoDeuda.toFixed(modeloconfiguracion.retornaValorConfiguracion("CANTIDAD_DIGITOS_DECIMALES_MONTO"))
        font.family: "Arial"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        smooth: true
        anchors.right: parent.right
        anchors.rightMargin: 30
        horizontalAlignment: Text.AlignRight
        font.pointSize: 10
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        objectName: montoDeLaDeuda
    }

    Text {
        id: txtSimboloMoneda
        x: 0
        y: 3
        height: 16
        color: "#4a68b5"
        text: simboloMoneda
        font.family: "Arial"
        smooth: true
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 2
        font.bold: true
        font.pointSize: 10
        anchors.right: txtMontoSaldoDeuda.left
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    CheckBox {
        id: chbSeleccionado
        buscarActivo: false
        chekActivo: checkboxActivo
        anchors.top: parent.top
        anchors.topMargin: -8
        textoValor: ""
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    Text {
        id: txtObservaciones
        color: "#080707"
        text: "Observación: "+observaciones
        anchors.right: txtSimboloMoneda.left
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        verticalAlignment: Text.AlignVCenter
        opacity: 0.6
        anchors.leftMargin: 50
        font.family: "Arial"
        font.pointSize: 10
        font.bold: false
        smooth: true
    }

   /* Text {
        id: txtFechaFactura
        color: "#212121"
        text: "Fecha doc: "+fechaDocumento
        smooth: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        font.bold: false
        font.family: "Arial"
        font.pointSize: 10
        anchors.leftMargin: 30
        horizontalAlignment: Text.AlignLeft
        opacity: 0.500
        verticalAlignment: Text.AlignVCenter
        anchors.left: txtClienteFactura.right
    }*/

}
