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
import Qt.labs.particles 1.0

import "../Controles"
import "../Listas"
Rectangle {
    id: rectPrincipalMenuSistemas
    width: 700
    height: 500
    color: "#00000000"
    radius: 0
    smooth: true
    Text {
        id: txtTituloMenuOpcion
        x: 560
        color: "#4d5595"
        text: qsTr("información del sistema")
        font.family: "Arial"
        style: Text.Normal
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        anchors.top: parent.top
        anchors.topMargin: -10
        anchors.right: parent.right
        anchors.rightMargin: 40
        smooth: true
        font.pixelSize: 23
    }
    Column {
        id: column1
        anchors.rightMargin: 30
        anchors.bottomMargin: 10
        spacing: 10
        anchors.leftMargin: 30
        anchors.topMargin: 40
        anchors.fill: parent
        Text {
            id: txtInformacionModeloAplicacion
            color: "#2b2a2a"
            text: qsTr("Clase:  Ambassador")
            font.family: "Arial"
            smooth: true
            font.pixelSize: 12
            font.bold: false
        }
        Text {
            id: txtVersionDelPrograma
            x: 0
            y: 0
            color: "#2b2a2a"
            text: qsTr("Versión: "+ versionKhitomer + " - " + funcionesmysql.versionDeBaseDeDatos())
            font.family: "Arial"
            smooth: true
            font.bold: false
            font.pixelSize: 12
        }

        /// 1.2.0: Se habilita el calculo de totales si el modo de configuración esta setado para
        ///        que el articulo no tenga iva incluido.
        /// 1.2.1: Se agrega el control para la conexión a la base de datos, antiguamente sin el control, no cerraba la conexion.
        /// 1.2.2: Se modifica la inserción de las lineas de documento para hacerlas por lote, en lugar de linea por linea,
        ///        esto aumenta la velocidad drasticamente y previene los errores. Ademas se cambio la visualización de la carga
        ///        rapida de precios.
        /// 1.2.3: Se finaliza el modudo de descuentos al total, y se implementa un cuadro de autorizaciones.
        /// 1.2.4: Se realiza la correción del guardado de items de factura, cuando el documento no tiene item.
        /// 1.2.5: Se agrega soporte a la base de datos para el campo direccionWeb. Se agregan 3 reportes de cuentas bancarias.
        ///        Se agrega soporte para monedas y cuentas bancarias en los reportes. Se agrega un reporte de ventas totales.
        /// 1.2.6: Se realizó la corrección de todos los reportes de ventas. Se agrego un nuevo parametro a los tipos de documentos(esdocumentoDeVenta).
        /// 1.2.7: Se arreglan los indices para que sea mas efectivas las consultas a la base de datos.
        ///        Se cambia el nombre del menú de administración: Reportes por Mantenimientos.
        /// 1.2.8: Se agregan nuevos indices para mejorar la velocidad del sistema.
        ///        Se cambia el sitio de la configuración del sistema de /opt/Khitomer a $home/.config/Khitomer
        /// 1.3.0: Se finaliza el soporte para graficas estadisticas.
        ///        Se arregla error en el manejo de los botones de bajar y subir la lista en cambio rapido de precios.
        ///        Se quita el indicador de menú donde estamos parados, para hacerlo mas realista a los permisos que tengamos.
        ///        Se agrega el mantenimiento de tipo de documentos. Se arregla la busqueda del proveedores en los reportes.
        ///        Se cambia algunos textos al tipo de letra Arial, remplazando a Arial.
        ///        Se agrega un nuevo control (ComboBoxCheckBoxGenerico). Se agrega en le mantenimiento de permisos la posibilidad
        ///        de asignar los doucmentos que va a usar cada perfil del sistema.
        /// 1.3.1: Se corrige el cierre de opciones extras en mantenimiento facturacion, cuando se pide autorizar un descuento al total.
        ///        Se corrige un error en los permisos de los tipos de documento por perfil, para el mantenimeinto de documentos y liquidaciones.
        /// 1.3.2: Se agrega el campo subtotalsindescuento en la base de datos, y en la impresión. Se corrige en la impresión que salga 0.00 cuando el monto es vacio.
        ///        Se agrega el redondeo en las facturas, configurable desde los tipos de documentos.
        /// 1.4.0: Se agrega un nuevo combobox en el mantenimiento de facturacion para fijar el precio del articulo a vender.
        ///        Se modifican algunos combobox para soportar scrollbars. Se agregan nuevos reportes para clientes y proveedores.
        ///        Se vuelve a habilitar el manejo de stock cuando se guarda una factura. Se calcula el stock y se guarda la bandera en la tabla FaltaStock por artículo.
        /// 1.4.1: Se corrige un error si varios equipos creaban al mismo tiempo una factura, cuando el sistema numera automaticamente.
        /// 1.5.0: Se agrega control en facturación para guardar el costo del articulo.
        ///        Se agrega soporte para tamaño de fuente en la impresión de las facturas por tipo de linea.
        ///        Se agrega busqueda por observaciones en el mantenimiento de los documentos.
        ///        Se agregan reportes de saldos por cuenta corriente por cliente y moneda y por proveedor y moneda.
        ///        Se agrega control de cantidad de lineas en la facturacion por tipo de documento.
        ///        Se agrega reporte de stock segun costo.
        /// 1.5.1: Se corrige el separador de item factura para mostrar el costo.
        ///        Se agrego en el item del articulo información de costo en moneda referencia, y costo en origen
        /// 1.6.0: Se implementa el soporte para el mantenimiento de cuentas corrientes dinero.
        ///        Se agregaron nuevos reportes de cuenta corriente para contemplar las nueva funcionalidad.
        ///        Se agregaron formas de pago para seleccionar antes de emitir una factura.
        ///        Los medios de pago ahora afectan el iva y los subtotales
        ///        Se agrego un nuevo parametro en la configuracion donde se indica el iva por defecto del sistema para los medios de pago.
        ///        En el mantenimiento de facturacion, el campo de precio manual unitario, se rellena con el precio que corresponda para el articulo.
        /// 1.7.0: Se agrega la posibilidad de devolver un documento, hacer un documento contrario al ya facturado.
        ///        Se agrega la posibilidad de debitar el monto de un documento con cuenta corriente positivo a las facturas con deudas del cliente.
        ///        Se corrge la validacion de fecha/hora para quitar las z que componian el validador. Se agrega new reg para arreglar le validador.
        /// 1.8.0: Se agrega soporte para calculo de costo ponderado segun stock, cuando se graba una linea de compra de una factura compra provededor.
        /// 1.8.1: Se implemento un parametro de configuracion, el cual anula el uso de autorizaciones. Este parametro es superior jerarquicamente a los permisos del perfil de usuarios.
        /// 1.8.2: Se implemento el redondeo segun la moneda del documento. Esto aún no es configurable por interfaz grafica, pero si desde la base de datos.
        /// 1.9.0: Se agrega control en la anulación de documentos que pagaron cuenta corriente. Se corrige problema al ingresar documento que pide articulos y medios de pago, si no se ingresan articulos, el medio de pago impacta sus totales en los totales del documento.
        /// 1.9.1: Se agrego la posibilidad de order la lista de articulo al imprimirse tal cual se armo la factura. Por defecto el documento agrupa los items, pero con el valor en 1, los imprime tal cual salieron.
        /// 1.9.2: Se corrige un problema con el ingreso de cantidad, cuando esta habilitado el campo de de codigo de barras a demanda.
        ///        Soporte para consultas sql del cabezal del reporte.
        /// 1.9.3: Se arregla la busqueda inteligente para que sea mas rapida, separando los tipos de listas a independientes.
        ///        Se corrigen los campos de entrada de información para que la pantalla no quede en negro cuando volvemos a tomar el control del campo.
        /// 1.10.0:Se arreglan problemas en los combobox de rubros y sub rubros.
        ///        Se finaliza el soporte para deudas en medios de pago, de pagos contados.
        ///        Se agrega reporte Facturas con deuda de Medios de Pago y una nueva Configuracion para manejar porcentajes de deuda minima en contados con medios de pago.
        /// 1.10.1:Se realizo corrección sobre la eliminación de clientes que tenian una factura ya emitida. El sistema permitia eliminar estos clientes por error.
        ///        Se agrego el reporte Stock por proveedor, que ademas muetsra costo y fechas de venta entre otras cosas.
        ///        Se modificaron los reportes de ventas por vendedor para comisiones, se le quito el nombre del vendedor y se puso el del cliente; se le agrego un cabezal con el vendedor.
        /// 1.10.2:Se corrige un error en el campo de facturación del dato comodin.
        /// 1.10.3:Cambios en varios reportes para Orion. Se agrega reporte de ventas detalladas sin impuestos.
        /// 1.11.0:Se cambia la forma de asignar listas de precios a clientes y proveedores.
        ///       :Se realizan correcciones en el control ComboBoxCheckBoxGenerico.
        ///       :Se corrige un error grabe en el guardado de documentos, que permitia que se actualizaran documentos emitidos y guardados.
        ///       :Se mejora el combobox de cheques diferidos para que muestre barra de desplazamiento y se ajuste su tamaño.
        /// 1.12.0:Se mejora el combobox de localides para que sea mas inteligente
        ///       :Se le agrega a los textinpux simples la capacidad de adaptarse en tamaño segun texto y titulo.
        ///       :Se agrega al mantenimiento de listas de precios la posibilidad de imprimir un listida en formato duplex.
        ///       :Se modifica la forma de guardar las listas de precios para que sean mas rapidas.
        /// 1.12.1:Se corrige la impresión del formato duplex de listas de precios.
        /// 1.12.2:Se realiza una nueva corrección de la impresión del formato duplex de listas de precios.
        /// 1.12.3:Se corrige la devolucion de documentos.
        /// 1.12.4:Se corrigen reportes y se da un nuevo look a algunos dialogos. Ademas se termina la implemnetación de la impreción duplex de lista de precios.
        /// 1.12.5:Se agrega un nuevo reporte llamado Saldo Total de clientes por moneda y se agrega el vendedor automatico al hacer devolucion completa o seleccionar facturar con deuda.
        ///       :Se agrega una nueva opción en los tipos de documentos para que un documento no afecte el IVA
        /// 1.12.6:Se agrega un nuevo reporte llamado Ventas entre fechas por rubro y lista de precio
        /// 1.12.7:Se agrega un nuevo reporte llamado Articulos sin venta por rubro entre fechas.
        ///       :Se arregla un error cuando una venta tiene descuento y se hace una devolución, pero el documento de devolución no tenia el campo descuento activo.
        ///       :Se agrega una nueva funcionalidad que permite modificar el precio de un articulo en las listas de precios, cuando se esta facturando un documento; por ejemplo una factura compra proveedor.
        ///       :Se agrega en las busquedas avanzadas un checkbox para los articulo que permite se listen los inactivos. Por defecto no se listan.
        /// 1.12.8:Se realiza corrección en los 3 reportes de ranking.
        /// 1.12.9:Se modifica el orden de muestra de los articulos desde hasta en el filtro de cambios rapido de precios en las listas de precios a pedido de Jaime Magariños ya que usa artículos con codigo numerico.
        ///       :Se modifica el orden de los artículo en el filtro de mas articulos en la facturación a pedido de Jaime Magariños.
        ///       :Se modifica el orden de los proveedores en la lista desplegable en el mantenimiento de Artículos a pedido de Jaime Magariños.
        /// 1.12.10:Se amplia el campo cantidad en las facturas a 5 digitos.
        ///       :Se modifican los reportes Stock por Proveedor, Lista de clientes y Stock Total Real.
        ///       :Se modifica la salida de los reportes por impresora para que se aproveche mejor la hoja total.
        /// 1.12.11:Se agrega soporte para 4 decimales en los montos.
        ///       :Se modifica la lectura de la configuración para que no tenga que establecer una conexion a la base de datos cada vez.
        ///       :Se agrega un nuevo reporte llamado "Ventas de clientes entre fechas por clasificación"
        /// 1.12.12:Corrección de error en campo totales con 4 digitos en la impresión.
        /// 1.12.13:Nuevo reporte llamado Ventas de clientes entre fechas por lista de precio
        ///        :Se agrego un nuevo cuadro de dialogo que permite seleccinar el proximo tipo de documento. Depende de configuación.
        /// 1.12.14:Se agrega un nuevo parametro para la cantidad de decimales en la impreción.
        ///        :Se agrega soporte en los permisos para habilitar o deshabilitar reportes por perfil.
        ///        :Se modifica el ingreso a la aplicación para que si necesita configuración, la misma aplicación provea una interfaz sin tener que recurrir a otra aplicación externa.
        ///        :Se agrega un nuevo reporte llamado Balance por moneda y lista de precio a pedido de Jaime Magariños.
        /// 1.12.15:Se arregla error de control de stock bajo minimo en las anulaciones de documentos.
        ///        :Se modifica el control ComboBoxCheckBoxGenerico para que pueda mostras una seguna fila de información.
        ///        :Se agrega un nuevo reporte llamado Balance por moneda y lista de precio detallado para Jaime Magariños.
        ///        :Se agrega un nuevo reporte llamado Ventas entre fechas detallado por tipo documento para Jaime Magariños.
        ///        :Se agrega un nuevo reporte llamado Ventas por documento con detalle medios de pago para Digital World.
        /// 1.12.16:Corrección de error en perfiles al listar los tipos de documentos.
        ///        :Se modifica el reporte Balance por moneda y lista de precio detallado para que muestre nombre de articulos.
        /// 1.12.17:Se agrego una primera etapa de logueo de información de facturación, para detectar problemas.
        ///        :Se realiza corrección al agregar un artículo a la factura cuando el campo cantidad esta inactivo.
        ///        :Se agrego la fecha de emision del documento en el mantenimiento de documentos.
        ///        :Se realiza la modificación del icono del sistema y se agrega el icono de Qt.
        ///        :Se agrega el reporte Stock con listas de precios a pedido de Jaime Magariños.
        /// 1.12.18:Se modifica la carga de facturas pendientes para que se actualize la fecha de emision del documento a la del día.
        /// 1.12.19:Se agrega soporte para configurar los menues de administración.
        ///        :Se modofica la carga de facturas para que cuando son emitidas o guardadas se muestre siempre la fecha de emision.
        /// 1.12.20:Modificación de los permisos para permitir ocultar el menu de cuenta corriente.
        /// 1.12.21:Se agregan 2 reportes nuevos: Ventas entre fechas deta. x tipo documento y moneda, Artículos desde > hasta
        /// 1.12.22:Se agrega soporte para mostrar la descripción extendida del articulo en las lineas vendidas de facturación del mantenimiento facturación
        /// 1.12.23:Se agrega soporte para identificar al cliente segun su procedencia de llegada al comercio(Funconalidad pedida por DigitalWorld).
        /// 1.12.24:Se agrega soporte para modificar los codigos de barras de una linea de facturación ya emitida o en cualquier momento(Funconalidad pedida por DigitalWorld).
        /// 1.13.0 :Primera versión con CFE funcional via conexión empresa IMIX
        /// 1.13.1 :Se corrige un problema al guardar el cliente cuanto el campo procedencia no es visible
        /// 1.13.2 :Se agrega soporte para Enviar información de articulo y cliente en 1 para Imix, de manera opcional.
        /// 1.13.3 :Corrección de la modificación de versión 1.12.24. para modificar linea de codigos de barra.
        /// 1.13.4 :Primera versión con CFE funcional via conexión empresa Dynamia.
        /// 1.13.5 :Se finaliza implementación de CFE para Dynamia
        /// 1.13.6 :Correcciones varias sobre CFE
        /// 1.13.7 :Correcciones de codificación UTF-8 para envio datos a IMIX
        ///        :Implementación del menu administrativo Factura electronica para actualizar información rapidamente
        /// 1.13.8 :Se implementa log por medio de la interfaz grafica y se apaga la fecha para el envio de CFE Imix
        /// 1.13.9 :Corrección en el envio de información a Imix cuando hay comillas en los textos
        /// 1.13.12 :Corrección en procesamiento desde Dynamia. Agregado de campo observaciones en envio a Imix. Se agranda el tamaño de impresión de campos. Se agrega mas información para el envio a Dynamia.
        /// 1.13.13 :Se agrega soporte utf8 para CFE Dynamia
        /// 1.13.14 :Se agrega control configurable por tipo de documento para no permitir facturar con stock previsto en cero.
        ///         :Se agrega mejora en la lista de items a facturar, cuando se agfrega un nuevo item, la lista se desplaza al final.
        /// 1.14.0  :Se quitan los saltos de linea de los textos a enviar para CFE.
        ///         :Se agrega la posibilidad de intercambiar entre tipos de documentos sin perder los artículos ingresados, siempre que ambos documentos tengan configuraciones similares.
        ///         :Se cambia la forma de interactuar con los descuentos, ahora se pueden cambiar en cualquier momento de realizada la factura.
        ///         :Se agregan nuevos campos al modelos de impresión.
        /// 1.14.1  :Cambio de fecha en reporte "Saldo por cliente y moneda".
        /// 1.14.2  :Corrección en el alta de clientes, ahora se asigna un nuevo numero de cliente al guardarlo y no al crearlo.
        ///         :Se hicieron varios cambios en los tamaños y distancias de la interfaz para mejorar el contenido.
        /// 1.14.3  :Corrección en la validación de las cedulas que terminan en 0.
        /// 1.14.4  :Se corrige el problema de los descuentos en las lineas de articulos, ahora se refleja el descuento al total en cada linea del articulo. Se modifico el reporte de venta entre fechas por rubro para adecuarlo a esta nueva modalidad.
        ///         :Se agrego la posibilidad de imprimir el descuento por linea.
        /// 1.14.5  :Se agrega en todos los reportes con documentos el numero de CFE.
        ///         :Se muestra en todos los lugares que hay documentos el numero de CFE. Se permite en el manteimiento de documentos buscar por numero de CFE
        /// 1.14.6  :Se agrega soporte para e-remitos en Imix.
        /// 1.14.7  :Corrección sobre descuentos para Dynamia. Cambiar el tipo_descuento de 2 a 1 para indicar por porcentaje.
        /// 1.14.8  :Se agrega soporte para fecha de nacimiento en el mantenimiento de clientes.

        Text {
            id: txtCopyright
            x: 0
            y: 138
            color: "#2b2a2a"
            text: qsTr("Copyright (C) <2012-2019>  Dreamsder")
            wrapMode: Text.NoWrap
            font.family: "Arial"
            font.bold: false
            smooth: true
            font.pixelSize: 12
        }


        Text {
            id: txtTelefono
            color: "#2b2a2a"
            text: qsTr("Teléfono: (+598) 097225884")
            smooth: true
            font.pixelSize: 12
            font.family: "Arial"
        }


        Text {
            id: txtWeb
            color: "#2b2a2a"
            text: qsTr("Sitio Web: www.khitomer-software.org")
            smooth: true
            font.pixelSize: 12
            font.family: "Arial"
        }

        Text {
            id: txtEmails
            x: 0
            y: 2
            color: "#2b2a2a"
            text: qsTr("E-mail: ventas@dreamsder.com  -  info@dreamsder.com")
            smooth: true
            font.pixelSize: 12
            font.family: "Arial"
        }
        Text {
            id: txtLicencia
            x: 0
            y: 232
            color: "#2b2a2a"
            text: qsTr("Licencia Pública General GNU versión 3")
            font.family: "Arial"
            font.bold: false
            smooth: true
            font.pixelSize: 12
        }
        Text {
            id: txtImpresoraPredeterminada
            x: 0
            y: 0
            color: "#2b2a2a"
            text: qsTr("Impresora predeterminada:  "+funcionesmysql.impresoraPorDefecto())
            font.family: "Arial"
            smooth: true
            font.pixelSize: 12
        }

        TextEdit {
            id: txtCreditos
            color: "#2b2a2a"
            text: "Contribuyentes: Maria Montano - mmontano@dreamsder.com\n                          Cristian Montano - cmontano@dreamsder.com\nLibreria XLS creada por: Yap Chun Wei - Martin Fuchs - Ami Castonguay - Long Wenbiao\nLibreria JSON creada por: Eeli Reilin\nLibreria base64 creada por: René Nyffenegger\nLibreria base64-NibbleAndAHalf creada por:  William Sherif\nLibreria CURL creada por:  Daniel Stenberg"
            smooth: true
            font.pixelSize: 12
            font.family: "Arial"
        }
    }

    Image {
        id: imgFacebook
        x: 243
        y: 338
        width: 100
        height: 100
        opacity: 0.600
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 40
        asynchronous: true
        anchors.right: parent.right
        anchors.rightMargin: 50
        smooth: true
        source: "qrc:/imagenes/qml/ProyectoQML/Imagenes/Facebook.png"

        MouseArea {
            id: mouse_areaFacebook
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                opacidadOff.from=imgYouTube.opacity
                opacidadOff.target=imgYouTube
                opacidadOff.start()
                opacidadIn.target=imgFacebook
                opacidadOff.target=imgFacebook
                opacidadOff.stop()
                opacidadIn.from=imgFacebook.opacity
                opacidadIn.start()
            }
            onExited: {
                opacidadIn.target=imgFacebook
                opacidadOff.target=imgFacebook
                opacidadIn.stop()
                opacidadOff.start()
            }
            onClicked: {
                funcionesmysql.abrirPaginaWeb("https://www.facebook.com/KhitomerDreamsder")
            }
        }
    }

    Image {
        id: imgYouTube
        x: 240
        y: 336
        width: 100
        height: 100
        opacity: 0.600
        asynchronous: true
        smooth: true
        MouseArea {
            id: mouse_areaYouTube
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                opacidadOff.from=imgFacebook.opacity
                opacidadOff.target=imgFacebook
                opacidadOff.start()
                opacidadIn.target=imgYouTube
                opacidadOff.target=imgYouTube
                opacidadOff.stop()
                opacidadIn.from=imgYouTube.opacity
                opacidadIn.start()
            }
            onExited: {
                opacidadIn.target=imgYouTube
                opacidadOff.target=imgYouTube
                opacidadIn.stop()
                opacidadOff.start()
            }
            onClicked: {
                funcionesmysql.abrirPaginaWeb("http://www.youtube.com/user/DREAMSDERdotCOM")
            }
        }
        source: "qrc:/imagenes/qml/ProyectoQML/Imagenes/YouTube.png"
        anchors.bottom: parent.bottom
        anchors.rightMargin: 25
        anchors.bottomMargin: 40
        anchors.right: imgFacebook.left
    }

    PropertyAnimation{
        id:opacidadIn
        property: "opacity"
        to:1
        duration: 200
    }

    Particles {
        anchors.fill: parent
        clip: true
        source:
        {
            if(modeloconfiguracion.retornaValorConfiguracion("MODO_CFE")==="1"){
                "qrc:/imagenes/qml/ProyectoQML/Imagenes/LogoKhitomerCFE-20-03-2018_512px.png"
            }else{
                "qrc:/imagenes/qml/ProyectoQML/Imagenes/LogoKhitomer-19-07-2016_512px.png"
            }
        }


        lifeSpan: 6000
        count: 2
        angle: 70
        opacity: 0.1
        angleDeviation: 36
        velocity: 30
        velocityDeviation: 10
        ParticleMotionWander {
            xvariance: 1
            pace: 100
        }
    }

    PropertyAnimation{
        id:opacidadOff
        property: "opacity"
        to:0.600
        duration: 50
    }

    Image {
        id: imgLogoKhitomer
        width: 200
        height: 200
        anchors.top: parent.top
        anchors.topMargin: 70
        asynchronous: true
        anchors.right: parent.right
        anchors.rightMargin: 50
        smooth: true
        opacity: 0.600
        source:

        {
            if(modeloconfiguracion.retornaValorConfiguracion("MODO_CFE")==="1"){
                "qrc:/imagenes/qml/ProyectoQML/Imagenes/LogoKhitomerCFE-20-03-2018_512px.png"
            }else{
                "qrc:/imagenes/qml/ProyectoQML/Imagenes/LogoKhitomer-19-07-2016_512px.png"
            }
        }



        MouseArea {
            id: mouse_area1
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                funcionesmysql.abrirPaginaWeb("http://www.khitomer-software.org")
            }
            onEntered: {
                opacidadOff.from=imgFacebook.opacity
                opacidadOff.target=imgLogoKhitomer
                opacidadOff.start()

                opacidadIn.target=imgLogoKhitomer
                opacidadIn.from=imgLogoKhitomer.opacity
                opacidadIn.start()
            }
            onExited: {
                opacidadIn.target=imgLogoKhitomer
                opacidadOff.target=imgLogoKhitomer
                opacidadIn.stop()
                opacidadOff.start()
            }
        }
    }

    Image {
        id: imgQtdigia
        y: 345
        width: 92
        height: 100
        anchors.left: parent.left
        anchors.leftMargin: 50
        opacity: 0.6
        asynchronous: true
        anchors.bottomMargin: 40
        MouseArea {
            id: mouse_areaFacebook1
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                funcionesmysql.abrirPaginaWeb("http://www.qt.io/")
            }

            onEntered: {
                opacidadOff.from=imgQtdigia.opacity
                opacidadOff.target=imgQtdigia
                opacidadOff.start()

                opacidadIn.target=imgQtdigia
                opacidadIn.from=imgQtdigia.opacity
                opacidadIn.start()
            }
            onExited: {
                opacidadIn.target=imgQtdigia
                opacidadOff.target=imgQtdigia
                opacidadIn.stop()
                opacidadOff.start()
            }
        }
        anchors.bottom: parent.bottom
        source: "qrc:/imagenes/qml/ProyectoQML/Imagenes/Built_with_Qt_logo_RGB_vertical.png"
        smooth: true
    }








}
