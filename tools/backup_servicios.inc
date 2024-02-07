<?php
/** 
 * PROCESAMIENTO CONSULTAS A LOS SERVICIOS REST PROPIOS DEL MODULO Y DEL MODULO WEB DRUPAL
 */

/**
 * tomar los parametros del servidor guardados en la configuracion del modulo Configuracion -> Viajes Iron Module
*/
function load_server_parameters() {
    //tomar parametros de los datos guardados
    $server_url = variable_get('viajesiron_server_ip_add', '');
    $server_port = variable_get('viajesiron_server_port', '');
    $service_url = variable_get('viajesiron_server_url', '');
    $propiedad_post_crear_viaje = variable_get('viajesiron_post_viaje', '');
    $propiedad_get_viaje_por_id = variable_get('viajesiron_get_viaje_por_id', '');
    $propiedad_get_remisiones_sin_viaje = variable_get('viajesiron_get_remisiones_sin_viaje', '');
    $propiedad_post_editar_viaje = variable_get('viajesiron_post_editar_viaje', '');
    $propiedad_post_eliminar_viaje = variable_get('viajesiron_post_delete_viaje', '');
    
    //generar paths
    if ($server_url == ''){
        drupal_set_message(t('No han sido configurados los parametros del servidor'), 'error');
        return FALSE;
    } else {
        $server_path = $server_url . ':' . $server_port . '/' . $service_url;
        $response = array(
            'server_url' => $server_path,
            'propiedad_post_viaje' => $server_path . '/' . $propiedad_post_crear_viaje,
            'propiedad_get_viaje_por_id' => $server_path . '/' . $propiedad_get_viaje_por_id,
            'propiedad_get_remisiones_sin_viaje' => $server_path . '/' . $propiedad_get_remisiones_sin_viaje,
            'propiedad_post_editar_viaje' => $server_path . '/' . $propiedad_post_editar_viaje,
            'propiedad_post_eliminar_viaje' => $server_path . '/' . $propiedad_post_eliminar_viaje,
        );
        return $response;
    }
}


/**
 * Llamado al servicio GET por id
 */
function call_get_viaje_por_id($viaje_id) {
    $server_params = load_server_parameters();
    
    // URL del servicio web post
    $url = $server_params['propiedad_get_viaje_por_id'];
    $parametros = array('id' => $viaje_id);
    $url_parametros = $url . '?' . http_build_query($parametros);
    $options = array(
        'method' => 'GET',
        'headers' => array('Content-Type' => 'application/json'),
    );
    
    $response = drupal_http_request($url_parametros, $options);
    
    if ($response->code == 200) {
        //OK
    }
    else {
        // Manejar el error de la solicitud
        drupal_set_message('Error en la solicitud: ' . $response->code, 'error');
        return FALSE;
      }
      return ($response);
    }
    
    
    /**
     * Llama al servicio post para crear viaje
     * @param json $proccessed_data
     * @return array response, FALSE si no hay exito
     */
    function call_post_crear_viaje_service($proccessed_data){
    
    /*$server_params = load_server_parameters();
    
    $url = $server_params['propiedad_post_viaje'];
    
    $options = array(
        'method' => 'POST',
        'timeout' => 240,
        'data' => $proccessed_data,
        'headers' => array(
            'Content-Type' => 'application/json',
        ),
    );

    // Realizar la solicitud POST
    $response = drupal_http_request($url, $options);
    
    // Verificar si la solicitud fue exitosa
    if ($response->code == 200) {
        $decoded_response = json_decode($response->data, TRUE);
    }
    else {
        drupal_set_message('Error en la solicitud: ' . $response->code, 'error');
        return FALSE;
    }
    
    return $decoded_response;*/

    $params = array(
        'propiedad' => 'propiedad_post_viaje',
        'method' => 'POST',
        'timeout' => 240,
        'data' => $proccessed_data,
    );

    $decoded_response = llamar_servicio_viajesiron($params);

    return $decoded_response;
} 


/**
 * Llama al servicio post para editar viaje
 * @param json $proccessed_data
 * @return array response, FALSE si no hay exito
 */
function call_post_editar_viaje_service($proccessed_data){
    
    /*$server_params = load_server_parameters();
    
    $url = $server_params['propiedad_post_editar_viaje'];
    
    $options = array(
        'method' => 'POST',
        'timeout' => 240,
        'data' => $proccessed_data,
        'headers' => array(
            'Content-Type' => 'application/json',
        ),
    );
    
    // Realizar la solicitud POST
    $response = drupal_http_request($url, $options);
    
    // Verificar si la solicitud fue exitosa
    if ($response->code == 200) {
        $decoded_response = json_decode($response->data, TRUE);
    }
    else {
        drupal_set_message('Error en la solicitud: ' . $response->code, 'error');
        return FALSE;
    }
    
    return $decoded_response;*/

    $params = array(
        'propiedad' => 'propiedad_post_editar_viaje',
        'method' => 'POST',
        'timeout' => 240,
        'data' => $proccessed_data,
    );

    $decoded_response = llamar_servicio_viajesiron($params);

    return $decoded_response;
}


/**
 * Llama al servicio post para editar viaje
 * @param long $id
 * @return array response, FALSE si no hay exito
 */
function call_post_eliminar_viaje_por_id($id){

    /*$url = 'propiedad_post_eliminar_viaje';

    $options = array(
        'method' => 'POST',
        'timeout' => 240,
        'data' => $id,
        'headers' => array(
            'Content-Type' => 'application/json',
        ),
    );

    // Realizar la solicitud POST
    $response = drupal_http_request($url, $options);

    // Verificar si la solicitud fue exitosa
    if ($response->code == 200) {
        $decoded_response = json_decode($response->data, TRUE);
    }
    else {
        drupal_set_message('Error en la solicitud: ' . $response->code, 'error');
        return FALSE;
    }*/

    $params = array(
        'propiedad' => 'propiedad_post_eliminar_viaje',
        'method' => 'POST',
        'timeout' => 240,
        'data' => $id,
    );

    $decoded_response = llamar_servicio_viajesiron($params);

    return $decoded_response;
}


/** 
 * Llamado al servicio GET por ID de expedicion, Modulo otro modulo (wsclient)
 */
function call_get_whs_service($whs) {
    $params = array(
        'whs' => $whs,
    );
    $response = llamar_servicio_web('consultarArchivoTMSIRONPorWHS', $params);
    return $response;
}


/** 
 * Llamado al servicio GET por ID de expediciones, Modulo otro modulo (wsclient)
 */
function call_get_varios_whs_service($whss) {
    $params = array(
        'whss' => $whss,
    );
    $response = llamar_servicio_web('consultarArchivoTMSIRONPorWHSS', $params);
    return $response;
}


/**
 * Llama al servicio y llena la variable con las opciones de transportadoras
 * no @param 
 * @return array respuesta a servicioweb consultarTransportadoras 
 */
function call_get_transportadoras() {
    $response = llamar_servicio_web('consultarTransportadoras', array());
    return $response; 
}


/**
 * Llama al servicio y llena la variable con las opciones de conceptos
 * no @param 
 * @return array respuesta a servicioweb consultarConceptos
 */
function call_get_conceptos() {
    $response = llamar_servicio_web('consultarConceptos', array());
    return $response;
}


/** 
 * Llamado al servicio GET por ID de expedicion, Modulo otro modulo (wsclient)
 * @param {array} params -> {fechaInicial, fechaFinal, idTransportadora}
 */
function call_get_viajes_por_fecha($params) {
    //$params => array('fechaInicial'=>'2022-11-20', 'fechaFinal'=>'2023-12-04', 'idTransportadora'=>'1',);
    $response = llamar_servicio_web('consultarViajesPorRangoDeFechas', $params);
    return ($response);
}


/** 
 * Llamado al servicio GET por ID de expedicion, Modulo otro modulo (wsclient)
 * @param {array} params
 */
function call_get_viajes_por_nombre($params) {
    $response = llamar_servicio_web('consultarViajesPorNombre', $params);
    return ($response);
}


/**
 * Llamado a servicios usando el modulo Service WEB de DRUPAL (ws_client)
 * @param {string, string} propiedad, params
 * @return json
*/
function llamar_servicio_web($propiedad, $params) {
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
 * Llamado a servicios propios del modulo VIAJESIRON
 * @param array params {propiedad, metodo, timeout, data}
 * @return array decoded json
*/
function llamar_servicio_viajesiron($params) {
    
    $server_params = load_server_parameters();

    $url = $server_params[$params['propiedad']];

    $options = array(
        'method' => $params['method'],
        'timeout' => $params['timeout'],
        'data' => $params['data'],
        'headers' => array(
            'Content-Type' => 'application/json',
        ),
    );

    // Realizar la solicitud POST
    $response = drupal_http_request($url, $options);

    // Verificar si la solicitud fue exitosa
    if ($response->code == 200) {
        $decoded_response = json_decode($response->data, TRUE);
    }
    else {
        drupal_set_message('Error en la solicitud: ' . $response->code, 'error');
        return FALSE;
    }

    return $decoded_response;

}