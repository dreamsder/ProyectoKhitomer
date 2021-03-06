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

#include <QApplication>
#include "qmlapplicationviewer.h"
#include <funciones.h>
#include "QDeclarativeContext"
#include <QtSql>
#include "QMessageBox"
//#include <QTranslator>
#include <moduloclientes.h>
#include <modulotipoclientes.h>
#include <modulotipoclasificacion.h>
#include <modulousuarios.h>
#include <modulolistaproveedor.h>
#include <moduloarticulos.h>
#include <moduloarticulosbarra.h>
#include <moduloliquidaciones.h>
#include <modulolistasprecios.h>
#include <modulolistaprecioarticulos.h>
#include <moduloivas.h>
#include <modulomonedas.h>
#include <Utilidades/utilidadesdemenu.h>
#include <moduloperfiles.h>
#include <modulolistatipodocumentos.h>
#include <modulolistaimpresoras.h>
#include <modulodocumentos.h>
#include <modulomediosdepago.h>
#include <moduloreportesmenu.h>
#include <moduloreportes.h>
#include <QDir>
#include <Utilidades/moduloconfiguracion.h>
#include <Utilidades/utilidadesxml.h>
#include <modulorubros.h>
#include <modulosubrubros.h>
#include <modulobancos.h>
#include <modulotarjetascredito.h>
#include <modulocuentasbancarias.h>
#include <modulotipocheques.h>
#include <modulolineasdepago.h>
#include <modulototalchequesdiferidos.h>
#include <modulobusquedainteligente.h>
#include <modulopaises.h>
#include <modulodepartamentos.h>
#include <modulolocalidades.h>
#include <Mantenimientos/controlesmantenimientos.h>
#include <Mantenimientos/mantenimientobatch.h>
#include <Mantenimientos/dialogoswidget.h>
#include <modulomodelosdeimpresion.h>
#include <modulogenericocombobox.h>
#include <moduloformasdepago.h>
#include <modulotipodocumentocliente.h>
#include <QDebug>
#include <iostream>
#include <modulotipoprocedenciacliente.h>

#include <modulotipopromocion.h>
#include <modulopromociones.h>


#include <CFE/modulo_cfe_parametrosgenerales.h>


#include <QSqlQuery>
#include <Utilidades/database.h>



int main(int argc, char *argv[])
{



    ////Codificacion del sistema para aceptar tildes y ñ
    QTextCodec *linuxCodec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(linuxCodec);
    QTextCodec::setCodecForCStrings(linuxCodec);
    QTextCodec::setCodecForLocale(linuxCodec);


    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;


    // QTranslator traducirIngles;







    // traducirIngles.load("Traducciones/main_qml_es");
    // traducirIngles.load("Traducciones/main_qml_en");
    //  app->installTranslator(&traducirIngles);



    //Seteo Icono de la aplicacion
    if(QFile::exists("qrc:/qml/ProyectoQML/Imagenes/icono.png")){
        viewer.setWindowIcon(QIcon("qrc:/qml/ProyectoQML/Imagenes/icono.png"));
    }else{
        viewer.setWindowIcon(QIcon("icono.png"));
    }

    viewer.setMinimumWidth(1200);
    viewer.setMinimumHeight(720);

    ModuloClientes moduloClientes;
    ModuloClientes moduloClientesOpcionesExtra;
    ModuloClientes moduloClientesFiltros;
    ModuloClientes moduloClientesFiltrosProveedor;
    ModuloTipoClientes moduloTipoClientes;
    ModuloTipoDocumentoCliente moduloTipoDocumentoCliente;
    ModuloTipoClasificacion moduloTipoClasificacion;
    ModuloUsuarios moduloUsuarios;
    UtilidadesDeMenu moduloListaMenues;
    ModuloUsuarios moduloListaUsuarios;
    ModuloListaProveedor moduloListaProveedor;
    ModuloArticulos moduloArticulos;
    ModuloArticulos moduloArticulosFiltros;
    ModuloArticulos moduloArticulosFiltrosBusquedaInteligente;
    ModuloArticulos moduloArticulosFiltrosListaPrecio1;
    ModuloArticulos moduloArticulosFiltrosListaPrecio2;
    ModuloArticulos moduloArticulosFiltrosListaPrecio3;



    ModuloArticulos moduloArticulosOpcionesExtra;
    ModuloArticulos moduloArticulosOpcionesExtraFacturacion;
    ModuloArticulosBarra moduloArticulosBarra;
    ModuloUsuarios moduloListaVendedores;
    ModuloLiquidaciones moduloLiquidaciones;
    ModuloLiquidaciones moduloLiquidacionesComboBox;
    ModuloListasPrecios moduloListasPrecios;
    ModuloListasPrecios moduloListasPreciosComboBox;
    ModuloListasPrecios moduloListasPreciosClientes;
    ModuloListasPrecios moduloListasPreciosBusquedaInteligente;
    ModuloListaPrecioArticulos moduloListaPrecioArticulos;
    ModuloListaPrecioArticulos moduloListaPrecioArticulosACambiarDePrecio;

    ModuloListaPrecioArticulos moduloListasPreciosCuadroArticulosASetearPrecio;

    ModuloIvas  moduloListaIvas;
    ModuloMonedas moduloListaMonedas;
    ModuloMonedas moduloMonedas;
    ModuloMonedas moduloMonedasTotales;
    ModuloPerfiles moduloListaPerfilesComboBox;

    ModuloPerfiles moduloListaPerfiles;

    ModuloListaTipoDocumentos moduloListaTipoDocumentos;

    ModuloListaTipoDocumentos moduloListaTipoDocumentosMantenimiento;
    ModuloListaTipoDocumentos moduloListaTipoDocumentosPerfiles;
    ModuloListaTipoDocumentos moduloListaTipoDocumentosParaDevoluciones;
    ModuloTipoProcedenciaCliente moduloTipoProcedenciaCliente;


    ModuloListaImpresoras moduloListaImpresoras;


    moduloListaImpresoras.buscarImpresoras();
    moduloListaImpresoras.buscarImpresoras();


    ModuloDocumentos moduloDocumentos;
    ModuloDocumentos moduloDocumentosEnLiquidaciones;
    ModuloDocumentos moduloDocumentosMantenimiento;
    ModuloDocumentos moduloDocumentosConSaldoCuentaCorriente;
    ModuloDocumentos moduloComboBoxDocumentosConSaldoCuentaCorriente;
    ModuloDocumentos moduloDocumentosDePagoCuentaCorriente;


    ModuloMediosDePago moduloMediosDePago;
    ModuloReportesMenu moduloReportesMenu;
    ModuloReportes moduloReportes;
    ModuloRubros moduloRubros;
    ModuloRubros moduloRubrosComboBox;
    ModuloSubRubros moduloSubRubrosComboBox;
    ModuloBancos moduloBancos;
    ModuloBancos moduloBancosComboBox;

    ModuloSubRubros moduloSubRubros;
    ModuloTarjetasCredito moduloTarjetasCredito;

    Funciones funcionesMysql;
    ModuloConfiguracion moduloconfiguracion;

    ModuloCuentasBancarias moduloCuentasBancariasComboBox;
    ModuloCuentasBancarias moduloCuentasBancarias;

    ModuloTipoCheques moduloTipoChequesComboBox;
    ModuloLineasDePago moduloLineasDePagoListaChequesDiferidosComboBox;
    ModuloLineasDePago moduloLineasDePagoTarjetasCredito;
    ModuloTotalChequesDiferidos moduloTotalChequesDiferidos;
    ModuloTotalChequesDiferidos moduloTotalOtrosCheques;

    ModuloBusquedaInteligente moduloBusquedaInteligenteArticulos;
    ModuloBusquedaInteligente moduloBusquedaInteligenteClientes;
    ModuloBusquedaInteligente moduloBusquedaInteligenteProveedores;

    ModuloPaises moduloPaises;
    ModuloPaises moduloPaisesComboBox;
    ModuloDepartamentos moduloDepartamentosComboBox;
    ModuloDepartamentos moduloDepartamentos;
    ModuloLocalidades moduloLocalidadesComboBox;
    ModuloLocalidades moduloLocalidades;
    ModuloGenericoCombobox moduloGenericoCombobox;
    ModuloGenericoCombobox moduloGenericoComboboxTipoDocumento;
    ModuloGenericoCombobox moduloGenericoComboboxReportesPermisos;

    ModuloFormasDePago moduloFormasDePago;

    ModuloModelosDeImpresion moduloModelosDeImpresion;

    ControlesMantenimientos moduloControlesMantenimiento;

    MantenimientoBatch moduloMantenimientoBatch;

    ModuloGenericoCombobox moduloGenericoTipoPromocion;
    ModuloPromociones moduloPromociones;


    DialogosWidget dialogoQT;


    ///Modulos CFE
    ///
    Modulo_CFE_ParametrosGenerales modulo_CFE_ParametrosGenerales;





inicio:

    viewer.rootContext()->setContextProperty("funcionesmysql", &funcionesMysql );


    funcionesMysql.depurarLog();

    if (QSqlDatabase::isDriverAvailable("QMYSQL")==false) {
        QMessageBox men;
        men.setText(qtTrId("No se encontro el driver QMYSQL (libqt4-sql-mysql)\nLa aplicación se cerrara."));
        men.exec();
        exit(0);
    }else{
        if(UtilidadesXml::leerConfiguracionXml()){
            if(funcionesMysql.verificoConexionBaseDeDatos()){



                Database::chequeaStatusAccesoMysql();
                if(!Database::connect().isOpen()){
                    if(!Database::connect().open()){
                        qDebug() << "No conecto";
                    }
                }

                Database::consultaSql("SET GLOBAL sql_mode = ''");

                Database::connect().close();

                if(funcionesMysql.actualizacionBaseDeDatos(funcionesMysql.versionDeBaseDeDatos())==false){
                    QMessageBox men;
                    men.setText(qtTrId("ATENCIÓN: Fallaron las actualizaciones pendientes de la base de datos.\n\nSOLUCIÓN: Contactese con el soporte tecnico del sistema, los datos estan al pie de este mensaje.\n\nDisculpe las molestias.\nLa aplicación se cerrara.\n\n\nSoporte facebook: https://www.facebook.com/KhitomerDreamsder\nSoporte mail: info@dreamsder.com\nSoporte ventas: ventas@dreamsder.com"));
                    men.exec();
                    exit(0);
                }



                moduloconfiguracion.cargarConfiguracion();
                modulo_CFE_ParametrosGenerales.cargar();

            }else{
                QMessageBox men;
                men.setText(qtTrId("ATENCIÓN: No se pudo acceder a la base de datos.\n\nConfiguración actual:\nIP: '")+UtilidadesXml::getHostLocal()+qtTrId("'\nBase de datos: '")+UtilidadesXml::getBaseLocal()+qtTrId("'\nUsuario: '")+UtilidadesXml::getUsuarioLocal()+qtTrId("'\n\nSOLUCIÓN: Precione el boton \"Configurar\" para abrir el configurador.\nEl configurador le pedira información de ubicación y acceso a la base de datos khitomer en el servidor de base de datos MySql.\n\nDisculpe las molestias.\n\n\nSoporte facebook: https://www.facebook.com/KhitomerDreamsder\nSoporte mail: info@dreamsder.com\nSoporte ventas: ventas@dreamsder.com"));
                men.setStandardButtons(QMessageBox::Cancel|QMessageBox::Ok);
                men.setButtonText(2,"Salir");
                men.setButtonText(1,"Configurar...");

                if(men.exec() == QMessageBox::Cancel){
                    exit(0);
                }else {
                    viewer.showExpanded();
                    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
                    viewer.setWindowTitle(qtTrId("Khitomer - Sistema de facturación"));
                    viewer.setSource(QUrl("qrc:/qml/ProyectoQML/ConfiguracionConexionMysql.qml"));
                    app->exec();
                    goto inicio;
                }


            }
        }else{
            QMessageBox men;
            men.setText(qtTrId("ATENCIÓN: No exite configuración de acceso para esta terminal.\n\nSOLUCIÓN:\nPrecione el boton \"Configurar\" para abrir el configurador.\nEl configurador le pedira información de ubicación y acceso a la base de datos khitomer en el servidor de base de datos MySql.\n\n\nSoporte facebook: https://www.facebook.com/KhitomerDreamsder\nSoporte mail: info@dreamsder.com\nSoporte ventas: ventas@dreamsder.com"));
            men.setStandardButtons(QMessageBox::Cancel|QMessageBox::Ok);

            men.setButtonText(2,"Salir");
            men.setButtonText(1,"Configurar...");

            if(men.exec() == QMessageBox::Cancel){
                exit(0);
            }else {
                viewer.showExpanded();
                viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
                viewer.setWindowTitle(qtTrId("Khitomer - Sistema de facturación"));
                viewer.setSource(QUrl("qrc:/qml/ProyectoQML/ConfiguracionConexionMysql.qml"));
                app->exec();
                goto inicio;
            }


        }
    }


    //Funciones para QML
    viewer.rootContext()->setContextProperty("modeloGenericoCombobox", &moduloGenericoCombobox );
    viewer.rootContext()->setContextProperty("modeloGenericoComboboxTipoDocumento", &moduloGenericoComboboxTipoDocumento );
    viewer.rootContext()->setContextProperty("modeloGenericoComboboxReportesPermisos", &moduloGenericoComboboxReportesPermisos );





    viewer.rootContext()->setContextProperty("modeloconfiguracion", &moduloconfiguracion );
    viewer.rootContext()->setContextProperty("modeloClientes", &moduloClientes );

    viewer.rootContext()->setContextProperty("modeloClientesFiltros", &moduloClientesFiltros );
    viewer.rootContext()->setContextProperty("modeloClientesFiltrosProveedor", &moduloClientesFiltrosProveedor );

    viewer.rootContext()->setContextProperty("modeloClientesOpcionesExtra", &moduloClientesOpcionesExtra );
    viewer.rootContext()->setContextProperty("modeloTipoClientes", &moduloTipoClientes );
    viewer.rootContext()->setContextProperty("modeloTipoDocumentoClientes", &moduloTipoDocumentoCliente );

    viewer.rootContext()->setContextProperty("modeloTipoClasificacion", &moduloTipoClasificacion );
    viewer.rootContext()->setContextProperty("modeloUsuarios", &moduloUsuarios );
    viewer.rootContext()->setContextProperty("modeloListaMenues", &moduloListaMenues );

    viewer.rootContext()->setContextProperty("modeloListaUsuarios", &moduloListaUsuarios );
    viewer.rootContext()->setContextProperty("modeloListaProveedor", &moduloListaProveedor );
    viewer.rootContext()->setContextProperty("modeloArticulos", &moduloArticulos );
    viewer.rootContext()->setContextProperty("modeloArticulosFiltros", &moduloArticulosFiltros );
    viewer.rootContext()->setContextProperty("modeloArticulosFiltrosBusquedaInteligente", &moduloArticulosFiltrosBusquedaInteligente );
    viewer.rootContext()->setContextProperty("modeloArticulosFiltrosListaPrecio1", &moduloArticulosFiltrosListaPrecio1 );
    viewer.rootContext()->setContextProperty("modeloArticulosFiltrosListaPrecio2", &moduloArticulosFiltrosListaPrecio2 );
    viewer.rootContext()->setContextProperty("modeloArticulosFiltrosListaPrecio3", &moduloArticulosFiltrosListaPrecio3 );

    viewer.rootContext()->setContextProperty("modeloArticulosBarra", &moduloArticulosBarra );
    viewer.rootContext()->setContextProperty("modeloArticulosOpcionesExtra", &moduloArticulosOpcionesExtra );
    viewer.rootContext()->setContextProperty("modeloArticulosOpcionesExtraFacturacion", &moduloArticulosOpcionesExtraFacturacion );

    viewer.rootContext()->setContextProperty("modeloListaVendedores", &moduloListaVendedores );
    viewer.rootContext()->setContextProperty("modeloLiquidaciones", &moduloLiquidaciones );
    viewer.rootContext()->setContextProperty("modeloLiquidacionesComboBox", &moduloLiquidacionesComboBox );

    viewer.rootContext()->setContextProperty("modeloListasPrecios", &moduloListasPrecios );
    viewer.rootContext()->setContextProperty("modeloListasPreciosClientes", &moduloListasPreciosClientes );
    viewer.rootContext()->setContextProperty("modeloListasPreciosBusquedaInteligente", &moduloListasPreciosBusquedaInteligente );
    viewer.rootContext()->setContextProperty("modeloListasPreciosCuadroArticulosASetearPrecio", &moduloListasPreciosCuadroArticulosASetearPrecio );

    viewer.rootContext()->setContextProperty("modeloListaPrecioArticulos", &moduloListaPrecioArticulos );
    viewer.rootContext()->setContextProperty("modeloListaPrecioArticulosACambiarDePrecio", &moduloListaPrecioArticulosACambiarDePrecio );

    viewer.rootContext()->setContextProperty("modeloListaIvas", &moduloListaIvas );
    viewer.rootContext()->setContextProperty("modeloListaMonedas", &moduloListaMonedas );
    viewer.rootContext()->setContextProperty("modeloMonedas", &moduloMonedas );
    viewer.rootContext()->setContextProperty("modeloMonedasTotales", &moduloMonedasTotales );

    viewer.rootContext()->setContextProperty("modeloListaPerfilesComboBox", &moduloListaPerfilesComboBox );
    viewer.rootContext()->setContextProperty("modeloListaPerfiles", &moduloListaPerfiles );


    viewer.rootContext()->setContextProperty("modeloListasPreciosComboBox", &moduloListasPreciosComboBox );
    viewer.rootContext()->setContextProperty("modeloListaTipoDocumentosComboBox", &moduloListaTipoDocumentos );
    viewer.rootContext()->setContextProperty("modeloListaTipoDocumentosMantenimiento", &moduloListaTipoDocumentosMantenimiento );
    viewer.rootContext()->setContextProperty("modeloListaTipoDocumentosPerfiles", &moduloListaTipoDocumentosPerfiles );
    viewer.rootContext()->setContextProperty("modeloListaTipoDocumentosParaDevoluciones", &moduloListaTipoDocumentosParaDevoluciones );



    viewer.rootContext()->setContextProperty("modeloListaImpresorasComboBox", &moduloListaImpresoras );
    viewer.rootContext()->setContextProperty("modeloDocumentos", &moduloDocumentos );
    viewer.rootContext()->setContextProperty("modeloDocumentosEnLiquidaciones", &moduloDocumentosEnLiquidaciones );
    viewer.rootContext()->setContextProperty("modeloDocumentosMantenimiento", &moduloDocumentosMantenimiento );

    viewer.rootContext()->setContextProperty("modeloMediosDePago", &moduloMediosDePago );
    viewer.rootContext()->setContextProperty("modeloReportesMenuComboBox", &moduloReportesMenu );
    viewer.rootContext()->setContextProperty("modeloReportes", &moduloReportes );


    viewer.rootContext()->setContextProperty("modeloListaRubros", &moduloRubros );
    viewer.rootContext()->setContextProperty("modeloListaRubrosComboBox", &moduloRubrosComboBox );
    viewer.rootContext()->setContextProperty("modeloListaSubRubrosComboBox", &moduloSubRubrosComboBox );
    viewer.rootContext()->setContextProperty("modeloListaSubRubros", &moduloSubRubros);
    viewer.rootContext()->setContextProperty("modeloListaBancos", &moduloBancos );
    viewer.rootContext()->setContextProperty("modeloListaBancosComboBox", &moduloBancosComboBox );

    viewer.rootContext()->setContextProperty("modeloListaTarjetasCredito", &moduloTarjetasCredito );

    viewer.rootContext()->setContextProperty("modeloCuentasBancariasComboBox", &moduloCuentasBancariasComboBox );
    viewer.rootContext()->setContextProperty("modeloCuentasBancarias", &moduloCuentasBancarias );

    viewer.rootContext()->setContextProperty("modeloTipoChequesComboBox", &moduloTipoChequesComboBox );
    viewer.rootContext()->setContextProperty("modeloLineasDePagoListaChequesDiferidosComboBox", &moduloLineasDePagoListaChequesDiferidosComboBox );
    viewer.rootContext()->setContextProperty("modeloLineasDePagoTarjetasCredito", &moduloLineasDePagoTarjetasCredito );
    viewer.rootContext()->setContextProperty("modeloTotalChequesDiferidos", &moduloTotalChequesDiferidos );
    viewer.rootContext()->setContextProperty("modeloTotalOtrosCheques", &moduloTotalOtrosCheques );


    viewer.rootContext()->setContextProperty("modeloBusquedaInteligenteArticulos", &moduloBusquedaInteligenteArticulos );
    viewer.rootContext()->setContextProperty("modeloBusquedaInteligenteClientes", &moduloBusquedaInteligenteClientes );
    viewer.rootContext()->setContextProperty("modeloBusquedaInteligenteProveedores", &moduloBusquedaInteligenteProveedores );

    viewer.rootContext()->setContextProperty("modeloPaises", &moduloPaises );
    viewer.rootContext()->setContextProperty("modeloPaisesComboBox", &moduloPaisesComboBox );
    viewer.rootContext()->setContextProperty("modeloDepartamentosComboBox", &moduloDepartamentosComboBox );
    viewer.rootContext()->setContextProperty("modeloLocalidadesComboBox", &moduloLocalidadesComboBox);
    viewer.rootContext()->setContextProperty("modeloDepartamentos", &moduloDepartamentos );
    viewer.rootContext()->setContextProperty("modeloLocalidades", &moduloLocalidades);
    viewer.rootContext()->setContextProperty("modeloModelosDeImpresion", &moduloModelosDeImpresion);


    viewer.rootContext()->setContextProperty("modeloControlesMantenimientos", &moduloControlesMantenimiento);
    viewer.rootContext()->setContextProperty("modeloMantenimientoBatch", &moduloMantenimientoBatch);

    viewer.rootContext()->setContextProperty("modeloFormasDePago", &moduloFormasDePago);

    viewer.rootContext()->setContextProperty("modeloDocumentosConSaldoCuentaCorriente", &moduloDocumentosConSaldoCuentaCorriente);
    viewer.rootContext()->setContextProperty("modeloDocumentosDePagoCuentaCorriente", &moduloDocumentosDePagoCuentaCorriente);
    viewer.rootContext()->setContextProperty("modeloComboBoxDocumentosConSaldoCuentaCorriente", &moduloComboBoxDocumentosConSaldoCuentaCorriente);

    viewer.rootContext()->setContextProperty("modeloTipoProcedenciaCliente", &moduloTipoProcedenciaCliente);

    viewer.rootContext()->setContextProperty("moduloGenericoTipoPromocion", &moduloGenericoTipoPromocion);
    viewer.rootContext()->setContextProperty("moduloPromociones", &moduloPromociones);


    viewer.rootContext()->setContextProperty("modeloDialogoQT", &dialogoQT);

    viewer.rootContext()->setContextProperty("modelo_CFE_ParametrosGenerales", &modulo_CFE_ParametrosGenerales);






    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    //  viewer.setMainQmlFile(QLatin1String("qml/ProyectoQML/main.qml"));
    viewer.setSource(QUrl("qrc:/qml/ProyectoQML/main.qml"));


    viewer.showExpanded();
    //  viewer.showMaximized();
    // viewer.showFullScreen();
    viewer.setWindowTitle(qtTrId("Khitomer - Sistema de facturación"));



    //Crea el directorio .khitomer en el home del usuario, aquí se guardara el html de los reportes.
    QDir directorioKhitomer;
    if(!directorioKhitomer.exists(directorioKhitomer.homePath()+"/.config/Khitomer")){
        if(directorioKhitomer.mkdir(directorioKhitomer.homePath()+"/.config/Khitomer")){

        }else{
            qDebug()<< qtTrId("NO SE PUDO CREAR DIRECTORIO DE OPCIONES DE USUARIO");
        }
    }

    //Carga de datos a ComboBox
    moduloTipoClientes.buscarTipoCliente("1=","1");    
    moduloTipoDocumentoCliente.buscar();

    moduloListaMenues.buscarMenus("1=","1");
    moduloTipoClasificacion.buscarTipoClasificacion("1=","1");
    moduloListaMonedas.buscarMonedas("1=","1");
    moduloListaProveedor.buscarCliente("tipoCliente=","2");
    moduloListaVendedores.buscarUsuarios("esVendedor=","1");
    moduloListaIvas.buscarIvas("1=","1");
    moduloListaPerfilesComboBox.buscarPerfiles("1=","1");
    moduloListasPreciosComboBox.buscarListasPrecio("1=","1");
    moduloLiquidacionesComboBox.buscarLiquidacion( "1=","1");
    moduloLiquidaciones.buscarLiquidacion("1=","1");
    moduloMediosDePago.buscarMediosDePago("1=","1");
    moduloListaTipoDocumentos.buscarTipoDocumentos("1=","1","1");
    moduloListaTipoDocumentosPerfiles.buscarTodosLosTipoDocumentos("1=","1");
    moduloReportesMenu.buscarReportesMenu("1=","1","");

    moduloReportes.buscarReportes("1=","1","");

    moduloListaImpresoras.limpiarListaImpresoras();
    moduloListaImpresoras.buscarImpresoras();
    moduloSubRubrosComboBox.buscarSubRubros("1=","1");
    moduloRubrosComboBox.buscarRubros("1=","1");
    moduloBancosComboBox.buscarBancos("1=","1");
    moduloTarjetasCredito.buscarTarjetasCredito("1=","1");
    moduloMonedasTotales.buscarMonedas("1=","1");
    moduloCuentasBancariasComboBox.buscarCuentasBancarias("1=","1");
    moduloTipoChequesComboBox.buscarCheques("1=","1");
    moduloLineasDePagoListaChequesDiferidosComboBox.buscarLineasDePagoChequesDiferidos("1=","1");
    moduloTotalChequesDiferidos.buscarTotalCheques("1","1");
    moduloTotalOtrosCheques.buscarTotalOtrosCheques("1","1");
    moduloPaisesComboBox.buscarPaises("1=","1","descripcionPais");
    moduloDepartamentosComboBox.buscarDepartamentos(" codigoPais=","1");
    moduloLocalidadesComboBox.buscarLocalidades(" codigoPais=1 and codigoDepartamento=","1");
    moduloModelosDeImpresion.buscarModeloImpresion("1=","1");
    moduloGenericoCombobox.buscarModuloGenerico();
    moduloGenericoComboboxTipoDocumento.buscarTodosLosTipoDocumentos();
    moduloGenericoComboboxReportesPermisos.buscarTodosLosReportes();

    moduloGenericoTipoPromocion.buscarTodosLosTiposPromocion();

    moduloTipoProcedenciaCliente.buscar();

    moduloFormasDePago.buscarFormaDePago("1=","1");
    //  moduloComboBoxDocumentosConSaldoCuentaCorriente.agregarDocumento(Documentos(0,0,"","","",0,0,"","","","","","","","","","","","","","","","","","","","","",""));






    return app->exec();

}

