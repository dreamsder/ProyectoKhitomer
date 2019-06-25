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
    height: 32
    color: "#e9e8e9"
    radius: 1
    border.color: "#aaaaaa"
    smooth: true
    opacity: 1


    Text {
        id:lblMedioDePago
        text: descripcionMedioPago
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.verticalCenter: parent.verticalCenter
        font.family: "Arial"
        opacity: 0.900
        smooth: true
        font.pointSize: 10
        font.bold: false
        verticalAlignment: Text.AlignVCenter
        color: "#212121"
    }
    MouseArea{
        id: mousearea1
        z: 1
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            rectListaItemColorDeseleccionado.stop()
            rectListaItemColorSeleccionado.start()
        }
        onExited: {
            rectListaItemColorSeleccionado.stop()
            rectListaItemColorDeseleccionado.start()

        }

        onClicked: {
            txtCodigoMedioDePago.text=codigoMedioPago
            txtMedioDePagoSeleccionado.text="Medio de pago:  "+descripcionMedioPago
            txtMontoMedioDePago.tomarElFoco()
            txtCuotasMedioDePago.visible=modeloMediosDePago.utilizaCuotas(codigoMedioPago)
            txtCuotasMedioDePago.textoInputBox="0"
            txtFechaCheque.textoInputBox=funcionesmysql.fechaDeHoy()
            cbxTipoCheque.codigoValorSeleccion=1
            cbxTipoCheque.textoComboBox=modeloTipoChequesComboBox.retornaDescripcionCheque(1)

            cbxTarjetaCreditoMedioDePago.visible=modeloMediosDePago.utilizaCuotas(codigoMedioPago)
            if(cbxTarjetaCreditoMedioDePago.visible){
                cbxTarjetaCreditoMedioDePago.codigoValorSeleccion=1
                cbxTarjetaCreditoMedioDePago.textoComboBox=modeloListaTarjetasCredito.retornaDescripcionTarjetaCredito(1)
            }else{
                cbxTarjetaCreditoMedioDePago.codigoValorSeleccion=0
                cbxTarjetaCreditoMedioDePago.textoComboBox=""
            }

            cbxCuentasBancarias.visible=modeloMediosDePago.utilizaCuentaBancaria(codigoMedioPago)
            if(!cbxCuentasBancarias.visible){
                cbxCuentasBancarias.codigoValorSeleccion=0
                cbxCuentasBancarias.codigoValorSeleccion2=0
                cbxCuentasBancarias.textoComboBox=""
            }

            cbxBancosMedioDePago.visible=modeloMediosDePago.utilizaBanco(codigoMedioPago)
            if(cbxBancosMedioDePago.visible){
                cbxBancosMedioDePago.codigoValorSeleccion=1
                cbxBancosMedioDePago.textoComboBox=modeloListaBancos.retornaDescripcionBanco(1)

            }else{
                cbxBancosMedioDePago.codigoValorSeleccion=0
                cbxBancosMedioDePago.textoComboBox=""
            }


            txtNumeroChequeMedioDePago.visible=modeloMediosDePago.utilizaNumeroCheque(codigoMedioPago)
            txtNumeroChequeMedioDePago.textoInputBox=""
            cbxTipoCheque.visible=txtNumeroChequeMedioDePago.visible
            txtFechaCheque.visible=txtNumeroChequeMedioDePago.visible
        }

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
}