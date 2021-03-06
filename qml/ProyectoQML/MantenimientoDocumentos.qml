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
import "Controles"
import "Listas"



Rectangle {
    id: rectPrincipalMantenimientoDocumentos
    width: 900
    height: 600
    color: "#ffffff"
    radius: 8
    smooth: true

    Rectangle {
        id: rectContenedorDocumentos
        x: 0
        y: 30
        color: "#494747"
        radius: 8
        z: 2
        smooth: true
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 30
        anchors.fill: parent

        Flow {
            id: flow1
            height: flow1.implicitHeight
            z: 1
            spacing: 7
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 10

            TextInputSimple {
                id: txtNumeroDocumento
             //   width: 130
                enFocoSeleccionarTodo: true
                textoDeFondo: ""
                largoMaximo: 9
                inputMask: "0000000000;"
                botonBuscarTextoVisible: true
                botonBorrarTextoVisible: true
                textoTitulo: "Num. documento:"
                textoInputBox: ""
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.codigoDocumento =",txtNumeroDocumento.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtSerieDocumento.tomarElFoco()
                onTabulacion: txtSerieDocumento.tomarElFoco()
            }


            TextInputSimple {
                id: txtSerieDocumento
              //  width: 90
                enFocoSeleccionarTodo: true
                textoInputBox: ""
                botonBuscarTextoVisible: true
                inputMask: ""
                botonBorrarTextoVisible: true
                largoMaximo: 4
                textoTitulo: "Serie:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.serieDocumento =",txtSerieDocumento.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtNumeroDocumentoCFE.tomarElFoco()
                onTabulacion: txtNumeroDocumentoCFE.tomarElFoco()

            }


            TextInputSimple {
                id: txtNumeroDocumentoCFE
             //   width: 130
                enFocoSeleccionarTodo: true
                textoDeFondo: ""
                largoMaximo: 9
                inputMask: "0000000000;"
                botonBuscarTextoVisible: true
                botonBorrarTextoVisible: true
                textoTitulo: "Num. CFE:"
                textoInputBox: ""
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.cae_numeroCae =",txtNumeroDocumentoCFE.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtNumeroLiquidacionDelDocumento.tomarElFoco()
                onTabulacion: txtNumeroLiquidacionDelDocumento.tomarElFoco()
            }



            TextInputSimple {
                id: txtNumeroLiquidacionDelDocumento
              //  width: 130
                enFocoSeleccionarTodo: true
                textoInputBox: ""
                botonBuscarTextoVisible: true
                inputMask: "0000000;"
                textoDeFondo: ""
                largoMaximo: 7
                botonBorrarTextoVisible: true
                textoTitulo: "Num. liquidación:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.codigoLiquidacion =",txtNumeroLiquidacionDelDocumento.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtVendedorLiquidacionDelDocumento.tomarElFoco()
                onTabulacion: txtVendedorLiquidacionDelDocumento.tomarElFoco()
            }
            ComboBoxListaVendedores {
                id: txtVendedorLiquidacionDelDocumento
                width: 170
                codigoValorSeleccion: "-1"
                textoComboBox: ""
                botonBuscarTextoVisible: true
                textoTitulo: "Vendedor de la liquidación:"
                z: 2
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.codigoVendedorLiquidacion =",txtVendedorLiquidacionDelDocumento.codigoValorSeleccion,modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtFechaDocumento.tomarElFoco()
                onTabulacion: txtFechaDocumento.tomarElFoco()
            }

            TextInputSimple {
                id: txtFechaDocumento
               // width: 130
                enFocoSeleccionarTodo: true
                textoInputBox: ""
                validaFormato: validacionFecha
                botonBuscarTextoVisible: true
                inputMask: "nnnn-nn-nn; "
                textoTitulo: "Fecha emision:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.fechaEmisionDocumento =",txtFechaDocumento.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtCodigoClienteDocumento.tomarElFoco()
                onTabulacion: txtCodigoClienteDocumento.tomarElFoco()

            }
            RegExpValidator{
                id:validacionFecha
                ///Fecha AAAA/MM/DD
                regExp: new RegExp("(20|  |2 | 2)(0[0-9, ]| [0-9, ]|1[0123456789 ]|2[0123456789 ]|3[0123456789 ]|4[0123456789 ])\-(0[1-9, ]| [1-9, ]|1[012 ]| [012 ])\-(0[1-9, ]| [1-9, ]|[12 ][0-9, ]|3[01 ]| [01 ])")
            }



            TextInputSimple {
                id: txtCodigoClienteDocumento
              //  width: 150
                enFocoSeleccionarTodo: true
                textoInputBox: ""
                botonBuscarTextoVisible: true
                inputMask: ""
                largoMaximo: 6
                botonBorrarTextoVisible: true
                textoTitulo: "Cliente:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.codigoCliente =",txtCodigoClienteDocumento.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtTipoClienteDocumento.tomarElFoco()
                onTabulacion: txtTipoClienteDocumento.tomarElFoco()
            }

            ComboBoxListaTipoCliente {
                id: txtTipoClienteDocumento
                width: 120
                botonBuscarTextoVisible: true
                textoTitulo: "Tipo de Cliente:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.tipoCliente =",txtTipoClienteDocumento.codigoValorSeleccion,modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtCodigoArticuloEnDocumento.tomarElFoco()
                onTabulacion: txtCodigoArticuloEnDocumento.tomarElFoco()
            }

            TextInputSimple {
                id: txtCodigoArticuloEnDocumento
               // width: 280
                enFocoSeleccionarTodo: true
                botonBuscarTextoVisible: true
                largoMaximo: 30
                botonBorrarTextoVisible: true
                textoTitulo: "Artículo en documento:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento(" ( DL.codigoArticulo='"+txtCodigoArticuloEnDocumento.textoInputBox.trim()+"' or DL.codigoArticuloBarras='"+txtCodigoArticuloEnDocumento.textoInputBox.trim()+"') and ","1=1",modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:cbListaTipoDeDocumentosEnMantenimiento.tomarElFoco()
                onTabulacion: cbListaTipoDeDocumentosEnMantenimiento.tomarElFoco()
            }


            ComboBoxListaTipoDocumentos {
                id: cbListaTipoDeDocumentosEnMantenimiento
                width: 250
                codigoValorSeleccion: "-1"
                botonBuscarTextoVisible: true
                textoTitulo: "Tipos de documentos:"
                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.codigoTipoDocumento =",cbListaTipoDeDocumentosEnMantenimiento.codigoValorSeleccion,modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
            }

            TextInputSimple {
                id: txtObservacionesEnDocumento
               // width: 370
                enFocoSeleccionarTodo: true
                textoInputBox: ""
                botonBuscarTextoVisible: true
                largoMaximo: 80
                textoTitulo: "Observaciones:"

                onClicEnBusqueda: {
                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("D.observaciones rlike ",txtObservacionesEnDocumento.textoInputBox.trim(),modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0
                }
                onEnter:txtNumeroDocumento.tomarElFoco()
                onTabulacion: txtNumeroDocumento.tomarElFoco()

            }



            CheckBox {
                id: chbDocumentoSinLiquidacion
                buscarActivo: true
                chekActivo: false
                colorTexto: "#dbd8d8"
                textoValor: "Fuera de caja"

                onClicEnBusqueda: {

                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    if(chekActivo){
                        modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento(" ( D.codigoLiquidacion=0 and (D.codigoVendedorLiquidacion is null or D.codigoVendedorLiquidacion='' or D.codigoVendedorLiquidacion='0') ) and ","1=1",modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    }else{
                        modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento(" ( D.codigoLiquidacion!=0 and (D.codigoVendedorLiquidacion is not null and D.codigoVendedorLiquidacion!='' and D.codigoVendedorLiquidacion!='0') ) and ","1=1",modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    }
                    listaDeDocumentosFiltrados.currentIndex=0

                }


            }
        }

        Rectangle{
            id: rectangle2
            color: {
                if(listaDeDocumentosFiltrados.count==0){
                    "#C4C4C6"
                }else{
                    "#dbb5f9"
                }
            }
            radius: 3
            clip: true
            smooth: true
            anchors.top: flow1.bottom
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 20
            ListView {
                id: listaDeDocumentosFiltrados
                clip: true
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottomMargin: 25
                boundsBehavior: Flickable.DragAndOvershootBounds
                highlightFollowsCurrentItem: true
                interactive: {
                    if(listaDeDocumentosFiltrados.count>5){
                        true
                    }else{
                        false
                    }
                }

                spacing: 1
                anchors.rightMargin: 1
                anchors.leftMargin: 1
                anchors.topMargin: 45
                snapMode: ListView.NoSnap
                keyNavigationWraps: true
                highlightRangeMode: ListView.NoHighlightRange
                flickableDirection: Flickable.VerticalFlick
                smooth: true
                delegate:  ListaDocumentos{}
                model: modeloDocumentosMantenimiento

                Rectangle {
                    id: scrollbarlistaDeDocumentosFiltrados
                    y: listaDeDocumentosFiltrados.visibleArea.yPosition * listaDeDocumentosFiltrados.height+5
                    width: 10
                    color: "#000000"
                    height: listaDeDocumentosFiltrados.visibleArea.heightRatio * listaDeDocumentosFiltrados.height+18
                    radius: 2
                    anchors.right: listaDeDocumentosFiltrados.right
                    anchors.rightMargin: 4
                    z: 1
                    opacity: 0.500
                    visible: {
                        if(listaDeDocumentosFiltrados.count>5){
                            true
                        }else{
                            false
                        }
                    }
                    smooth: true
                }
            }

            Text {
                id: lblCantidadDeDocumentosFiltrados
                x: 10
                width: lblCantidadDeDocumentosFiltrados.implicitWidth
                color: "#000000"
                text: qsTr("Documentos filtrados:")
                font.family: "Arial"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                font.pointSize: 10
                font.bold: false
                anchors.left: parent.left
                anchors.leftMargin: 5
                smooth: true
            }

            Text {
                id: txtCantidadDedocumentosFiltrados
                x: 127
                width: 37
                color: "#000000"
                text: listaDeDocumentosFiltrados.count
                font.family: "Arial"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.leftMargin: 5
                smooth: true
                font.bold: false
                font.pointSize: 10
                anchors.left: lblCantidadDeDocumentosFiltrados.right
            }

            Rectangle {
                id: rectTituloListaItemFacturacion
                x: 6
                y: -7
                height: 16
                color: "#2b2a2a"
                radius: 3
                anchors.top: parent.top
                anchors.topMargin: 25
                Text {
                    id: lbltemCodigoDocumento
                    width: 110
                    color: "#ffffff"
                    text: "# Doc. interno"
                    font.family: "Arial"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    style: Text.Normal
                    anchors.bottomMargin: 0
                    font.bold: true
                    font.pointSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                }

                Rectangle {
                    id: rectLineaSeparacionTitulo
                    y: 0
                    width: 2
                    color: "#C4C4C6"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 10
                    anchors.left: lbltemCodigoDocumento.right
                }

                Text {
                    id: lbltemRazonSocialDocumento
                    x: 4
                    y: 0
                    width: 220
                    color: "#ffffff"
                    text: "Razón social"
                    font.family: "Arial"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    style: Text.Normal
                    anchors.bottomMargin: 0
                    font.bold: true
                    font.pointSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: rectLineaSeparacion2Titulo.right
                }

                Rectangle {
                    id: rectLineaSeparacion2Titulo
                    x: -1
                    y: -4
                    width: 2
                    color: "#C4C4C6"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 10
                    anchors.left: lbltemSerieDocumento.right
                }

                Rectangle {
                    id: rectLineaSeparacion3Titulo
                    x: 3
                    y: -7
                    width: 2
                    color: "#C4C4C6"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 10
                    anchors.left: lbltemRazonSocialDocumento.right
                }

                Text {
                    id: lbltemSerieDocumento
                    x: 2
                    y: 0
                    width: 80
                    color: "#ffffff"
                    text: "# CFE"
                    font.family: "Arial"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    style: Text.Normal
                    anchors.bottomMargin: 0
                    font.bold: true
                    font.pointSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: rectLineaSeparacionTitulo.right
                }

                Text {
                    id: lbltemNombreClienteDocumento
                    x: 3
                    y: 7
                    width: 160
                    color: "#ffffff"
                    text: "Nombre"
                    font.family: "Arial"
                    smooth: true
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.bottom: parent.bottom
                    style: Text.Normal
                    anchors.bottomMargin: 0
                    font.bold: true
                    font.pointSize: 10
                    anchors.leftMargin: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: rectLineaSeparacion3Titulo.right
                }
                anchors.rightMargin: 1
                anchors.right: parent.right
                anchors.leftMargin: 1
                opacity: 1
                anchors.left: parent.left
            }

            Text {
                id: txtInformacionDobleClic
                text: qsTr("Doble-clic para visualizar facturas.")
                font.pointSize: 10
                font.bold: false
                font.family: "Arial"
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                smooth: true
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                visible: tagFacturacion.enabled
            }

            BotonBarraDeHerramientas {
                id: botonSubirListaFinal
                x: 845
                y: 60
                width: 14
                height: 14
                anchors.right: parent.right
                anchors.rightMargin: 3
                toolTip: ""
                anchors.top: parent.top
                anchors.topMargin: 5
                source: "qrc:/imagenes/qml/ProyectoQML/Imagenes/FlechaFinal.png"
                rotation: 90
                visible: scrollbarlistaDeDocumentosFiltrados.visible
                onClic: listaDeDocumentosFiltrados.positionViewAtIndex(0,0)
            }

            BotonBarraDeHerramientas {
                id: botonBajarListaFinal
                x: 845
                y: 343
                width: 14
                height: 14
                anchors.right: parent.right
                anchors.rightMargin: 3
                toolTip: ""
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                source: "qrc:/imagenes/qml/ProyectoQML/Imagenes/FlechaFinal.png"
                rotation: -90
                visible: scrollbarlistaDeDocumentosFiltrados.visible
                onClic: listaDeDocumentosFiltrados.positionViewAtIndex(listaDeDocumentosFiltrados.count-1,0)

            }
        }

        BotonPaletaSistema {
            id: btnFiltrarDocumentos
            y: 53
            text: "Filtrar documentos"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.bottom: rectangle2.top
            anchors.bottomMargin: 10
            onClicked: {

                var consultaSql="";

                if(txtNumeroDocumento.textoInputBox.trim()!=""){
                    consultaSql+=" D.codigoDocumento = '"+txtNumeroDocumento.textoInputBox.trim()+"' and ";
                }
                if(txtSerieDocumento.textoInputBox.trim()!=""){
                    consultaSql+="D.serieDocumento = '"+txtSerieDocumento.textoInputBox.trim()+"' and "
                }

                if(txtNumeroDocumentoCFE.textoInputBox.trim()!=""){
                    consultaSql+=" D.cae_numeroCae = '"+txtNumeroDocumentoCFE.textoInputBox.trim()+"' and ";
                }

                if(txtNumeroLiquidacionDelDocumento.textoInputBox.trim()!=""){
                    consultaSql+="D.codigoLiquidacion = '"+txtNumeroLiquidacionDelDocumento.textoInputBox.trim()+"' and "
                }                
                if(txtObservacionesEnDocumento.textoInputBox.trim()!=""){
                    consultaSql+="D.observaciones rlike '"+txtObservacionesEnDocumento.textoInputBox.trim()+"' and "
                }
                if(txtVendedorLiquidacionDelDocumento.codigoValorSeleccion.trim()!="" && txtVendedorLiquidacionDelDocumento.codigoValorSeleccion.trim()!="-1"){
                    consultaSql+="D.codigoVendedorLiquidacion = '"+txtVendedorLiquidacionDelDocumento.codigoValorSeleccion.trim()+"' and "
                }
                if(txtFechaDocumento.textoInputBox.trim()!="--"){
                    consultaSql+="D.fechaEmisionDocumento = '"+txtFechaDocumento.textoInputBox.trim()+"' and "
                }
                if(txtCodigoClienteDocumento.textoInputBox.trim()!=""){
                    consultaSql+="D.codigoCliente = '"+txtCodigoClienteDocumento.textoInputBox.trim()+"' and "
                }
                if(txtTipoClienteDocumento.codigoValorSeleccion!="" && txtTipoClienteDocumento.codigoValorSeleccion!="-1"){
                    consultaSql+="D.tipoCliente ='"+txtTipoClienteDocumento.codigoValorSeleccion+"' and "
                }
                if(txtCodigoArticuloEnDocumento.textoInputBox.trim()!=""){
                    consultaSql+=" (DL.codigoArticulo='"+txtCodigoArticuloEnDocumento.textoInputBox.trim()+"' or DL.codigoArticuloBarras='"+txtCodigoArticuloEnDocumento.textoInputBox.trim()+"') and  "
                }
                if(cbListaTipoDeDocumentosEnMantenimiento.codigoValorSeleccion.trim()!="" && cbListaTipoDeDocumentosEnMantenimiento.codigoValorSeleccion.trim()!="-1"){
                    consultaSql+="D.codigoTipoDocumento = '"+cbListaTipoDeDocumentosEnMantenimiento.codigoValorSeleccion.trim()+"' and "
                }
                if(chbDocumentoSinLiquidacion.chekActivo){
                    consultaSql+=" ( D.codigoLiquidacion=0 and (D.codigoVendedorLiquidacion is null or D.codigoVendedorLiquidacion='' or D.codigoVendedorLiquidacion='0') ) and "
                }
                if(chbDocumentoSinLiquidacion.chekActivo==false){
                    consultaSql+=" ( D.codigoLiquidacion!=0 and (D.codigoVendedorLiquidacion is not null and D.codigoVendedorLiquidacion!='' and D.codigoVendedorLiquidacion!='0') ) and "
                }

                if(consultaSql!=""){

                    modeloDocumentosMantenimiento.limpiarListaDocumentos()
                    modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento(consultaSql,"1=1",modeloListaPerfiles.retornaCodigoPerfil(txtNombreDeUsuario.textoInputBox.trim()))
                    listaDeDocumentosFiltrados.currentIndex=0

                }
            }
        }

    }


    Row {
        id: rowBarraDeHerramientasDocumentos
        smooth: true
        anchors.bottom: rectContenedorDocumentos.top
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
        spacing: distanciaEntreBotonesBarraDeTareas

        BotonBarraDeHerramientas {
            id: botonNuevoFiltro
            x: 33
            y: 10
            toolTip: "Nuevo filtro"
            z: 8
            source: "qrc:/imagenes/qml/ProyectoQML/Imagenes/Search.png"
            anchors.verticalCenter: parent.verticalCenter


            onClic: {

                txtNumeroDocumento.textoInputBox=""

                txtSerieDocumento.textoInputBox=""
                txtNumeroDocumentoCFE.textoInputBox=""

                txtNumeroLiquidacionDelDocumento.textoInputBox=""
                txtVendedorLiquidacionDelDocumento.codigoValorSeleccion="-1"
                txtVendedorLiquidacionDelDocumento.textoComboBox=""
                txtFechaDocumento.textoInputBox=funcionesmysql.fechaDeHoy()
                txtCodigoClienteDocumento.textoInputBox=""
                txtTipoClienteDocumento.codigoValorSeleccion=""
                txtTipoClienteDocumento.textoComboBox=""
                txtCodigoArticuloEnDocumento.textoInputBox=""
                cbListaTipoDeDocumentosEnMantenimiento.codigoValorSeleccion="-1"
                cbListaTipoDeDocumentosEnMantenimiento.textoComboBox=""
                chbDocumentoSinLiquidacion.setActivo(false)
                modeloDocumentosMantenimiento.limpiarListaDocumentos()
                modeloDocumentosMantenimiento.buscarDocumentosEnMantenimiento("-1","-1","null")
                listaDeDocumentosFiltrados.currentIndex=0
            }
        }

        Text {
            id: txtMensajeInformacionArticulos
            y: 7
            color: "#d93e3e"
            text: qsTr("Información:")
            font.family: "Arial"
            visible: false
            font.pixelSize: 14
            styleColor: "white"
            style: Text.Raised
            font.bold: true
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignRight
            smooth: true
        }
    }

    Timer{
        id:txtMensajeInformacionTimer
        repeat: false
        interval: 5000
        onTriggered: {

            txtMensajeInformacionArticulos.visible=false
            txtMensajeInformacionArticulos.color="#d93e3e"
        }



    }

    Rectangle {
        id: rectangle4
        x: -4
        y: -1
        width: 10
        color: "#7e0cc5"
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: rectContenedorDocumentos.top
        anchors.bottomMargin: -10
        z: 1
        anchors.leftMargin: 0
        anchors.left: parent.left
    }


}
