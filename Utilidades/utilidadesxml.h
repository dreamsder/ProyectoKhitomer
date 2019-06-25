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

#ifndef UTILIDADESXML_H
#define UTILIDADESXML_H

#include <QString>


class UtilidadesXml
{    

public:

    static bool leerConfiguracionXml();

    static QString getUsuarioLocal(){
        return m_usuarioLocal;
    }
    static QString getClaveLocal(){
        return m_claveLocal;
    }
    static QString getBaseLocal(){
        return m_baseLocal;
    }
    static int getPuertoLocal(){
        return m_puertoLocal;
    }
    static QString getHostLocal(){
        return m_hostLocal;
    }




private:
   UtilidadesXml();

    static QString m_usuarioLocal;
    static QString m_claveLocal;
    static QString m_baseLocal;
    static int m_puertoLocal;
    static QString m_hostLocal;


    static void setUsuarioLocal(QString value){
        m_usuarioLocal=value;
    }
    static void setClaveLocal(QString value){
        m_claveLocal=value;
    }
    static void setBaseLocal(QString value){
        m_baseLocal=value;
    }
    static void setPuertoLocal(int value){
        m_puertoLocal=value;
    }
    static void setHostLocal(QString value){
        m_hostLocal=value;
    }


};
#endif // UTILIDADESXML_H
