<?php
/** 
 * PROCESAMIENTO CONSULTAS A LOS SERVICIOS DEL MODULO WEB DRUPAL
 */

abstract class CallWebServiceEndpoints {
    
    /**
     * Llamado a servicios usando el modulo Service WEB de DRUPAL (ws_client)
     * @param {string, string} propiedad, params
     * @return json
    */
    private static function llamar_servicio_web($propiedad, $params) {
        //Hacer consulta al servicio web
        try{
            $service = wsclient_service_load('toxementintranetrestservices_tms'); //Servicio configurado en Drupal
        }
        catch (WSClientException $exception) {
            drupal_set_message('No se pudo cargar el servicio REST configurado', 'error');
            return (FALSE);
        }
        // Realizar la consulta GET
        try {
            $response = $service->invoke($propiedad, $params);
        }
        catch (WSClientException $exception) {
            drupal_set_message('No hay respuesta del servidor REST o error en la solicitud', 'error');
            return (FALSE);
        }
        return ($response);
    }


    /** 
     * Llamado al servicio GET por ID de expedicion, Modulo otro modulo (wsclient)
     */
    static function call_get_whs_service($whs) {
        $params = array(
            'whs' => $whs,
        );
        $response = self::llamar_servicio_web('consultarArchivoTMSIRONPorWHS', $params);
        return $response;
    }


    /** 
     * Llamado al servicio GET por ID de expediciones, Modulo otro modulo (wsclient)
     */
    static function call_get_varios_whs_service($whss) {
        $params = array(
            'whss' => $whss,
        );
        $response = self::llamar_servicio_web('consultarArchivoTMSIRONPorWHSS', $params);
        return $response;
    }


    /**
     * Llama al servicio y llena la variable con las opciones de transportadoras
     * @return array respuesta a servicioweb consultarTransportadoras 
     */
    static function call_get_transportadoras() {
        $response = self::llamar_servicio_web('consultarTransportadoras', array());
        return $response; 
    }


    /**
     * Llama al servicio y llena la variable con las opciones de conceptos
     * @return array respuesta a servicioweb consultarConceptos
     */
    static function call_get_conceptos() {
        $response = self::llamar_servicio_web('consultarConceptos', array());
        return $response;
    }


    /** 
     * Llamado al servicio GET por ID de expedicion, Modulo otro modulo (wsclient)
     * @param {array} params -> {fechaInicial, fechaFinal, idTransportadora}
     */
    static function call_get_viajes_por_fecha($params) {
        //$params => array('fechaInicial'=>'2022-11-20', 'fechaFinal'=>'2023-12-04', 'idTransportadora'=>'1',);
        $response = self::llamar_servicio_web('consultarViajesPorRangoDeFechas', $params);
        return ($response);
    }


    /** 
     * Llamado al servicio GET por ID de expedicion, Modulo otro modulo (wsclient)
     * @param {array} params
     */
    static function call_get_viajes_por_nombre($params) {
        $response = self::llamar_servicio_web('consultarViajesPorNombre', $params);
        return ($response);
    }


}