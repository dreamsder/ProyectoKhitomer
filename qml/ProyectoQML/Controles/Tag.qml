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

Rectangle {
    id: rectTagPrincipal
    width: txtTextoTag.implicitWidth+50
    height: 25
    color: "#00000000"
    opacity: opacidadPorDefecto

    signal clic
    property double scaleValorActual: 1
    property alias colorTag:txtTextoTag.color
    property alias texto: txtTextoTag.text
    property alias toolTip: toolTipText.text
    property alias source: imageTag.source
    property double opacidadPorDefecto: 0.3
    property color _gradietMedioIndicador: "#f7960e"

    property alias  indicadorColor: rectangle4.color

    Image {
        id: imageTag
        width: 25
        height: 25
        smooth: true
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        asynchronous: true
        source: ""
    }

    Text {
        id: txtTextoTag
        color: "#ffffff"
        text: qsTr("TAG generico")
        font.family: "Arial"
        styleColor: "#183ccc"
        style: Text.Sunken
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        smooth: true
        font.pixelSize: 17
    }

    MouseArea {
        id: mouse_areaTag
        anchors.rightMargin: 20
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            timer1.start()
            rectTagPrincipalOpacityOff.stop()
            rectTagPrincipalOpacityIn.start()
        }
        onExited: {
            timer1.stop()
            rectToolTipText.visible=false
            rectTagPrincipalOpacityIn.stop()
            rectTagPrincipalOpacityOff.start()
        }
        onPressed: {
            timer1.stop()
            rectToolTipText.visible=false
        }
        onReleased: {
            timer1.stop()
        }


        onClicked: clic()
    }
    PropertyAnimation{
        id:rectTagPrincipalOpacityIn
        target: rectTagPrincipal
        property: "opacity"
        from:opacidadPorDefecto
        to:1
        duration:100
    }

    PropertyAnimation{
        id:rectTagPrincipalOpacityOff
        target: rectTagPrincipal
        property: "opacity"
        from:1
        to:opacidadPorDefecto
        duration:40
    }
    Rectangle{
        id:rectToolTipText
        visible: false
        width: toolTipText.implicitWidth+20
        height: toolTipText.implicitHeight
        color: "#4d7dc0"
        radius: 6
        z: 1
        smooth: true
        Text {
            id: toolTipText
            color: "#fdfbfb"
            text: ""
            font.family: "Arial"
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.fill: parent
            smooth: true
            visible: true
        }
    }
    Rectangle {
        id: rectangle4
        width: 4
        radius: 2
        border.width: 0
        clip: false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
    }

    Timer {
        id:timer1
        interval: 1500;
        running: false;
        repeat: false;
        onTriggered: {
            if(toolTipText.text!=""){
                rectToolTipText.scale=0.85
                rectToolTipText.x=rectTagPrincipal.width-20
                rectToolTipText.y=rectTagPrincipal.height/4
                rectToolTipText.visible=true

            }
        }

    }

    Rectangle {
        id: recIndicadorOpcion
        height: 30
        color: "#00000000"
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.right: parent.right
        anchors.rightMargin: 10
        visible: opacidadPorDefecto==1 ? "true" : "false"
        anchors.bottom: parent.top
        anchors.bottomMargin: 2
        clip: true

        Rectangle {
            id: rectangle1
            x: 6
            width: 25
            height: 25
            color: "#ffffff"
            radius: 4
            opacity: 1
            visible: true
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -14
            anchors.horizontalCenterOffset: 0
            z: -1
            anchors.horizontalCenter: parent.horizontalCenter
            smooth: true
            rotation: 45
        }

        Rectangle {
            id: rectangle2
            y: 6
            height: 2
            color: "#ffffff"
            radius: 4
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            smooth: true
            anchors.bottom: parent.bottom
            visible: true
            anchors.bottomMargin: 0
            z: -1
            opacity: 1
        }
    }

    Rectangle {
        id: rectangle3
        height: 2
        color: "#ffffff"
        radius: 4
        anchors.top: parent.bottom
        anchors.topMargin: 2
        smooth: true
        visible: opacidadPorDefecto==1 ? "true" : "false"
        anchors.rightMargin: 10
        z: -1
        anchors.right: parent.right
        anchors.leftMargin: 1
        opacity: 1
        anchors.left: parent.left
    }

}
