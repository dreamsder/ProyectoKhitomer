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

import QtQuick 1.1
import "Controles"
import "Listas"
import "Listas/Delegates"
Rectangle {
    id: rectPrincipalMantenimientoPromociones
    width: 900
    height: 600
    color: "#ffffff"
    radius: 8
    visible: true
    smooth: true


    Rectangle {
        id: rectContenedorPromociones
        x: 0
        y: 30
        color: "#484646"
        radius: 8
        z: 2
        smooth: true
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 30
        anchors.fill: parent


    }
}
