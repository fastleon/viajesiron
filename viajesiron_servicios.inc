<?php
/** 
 * PROCESAMIENTO CONSULTAS A LOS SERVICIOS REST PROPIOS DEL MODULO Y DEL MODULO WEB DRUPAL
 */

/**
 * tomar los parametros del servidor guardados en la configuracion del modulo Configuracion -> Viajes Iron Module
*/
function load_server_parameters() {
    //llamar la data_control
    module_load_include('php', 'viajesiron', 'domain/session/data_control');
    $parametros_rest = (new DataControlParametrosREST())->llamarCargarDato();
    //tomar parametros de los datos guardados
    $server_url = $parametros_rest['server_ip_address'];
    $server_port = $parametros_rest['server_port'];
    $service_url = $parametros_rest['server_url'];
    $endpoint_post_crear_viaje = $parametros_rest['post_viaje'];
    $endpoint_post_reportes_cumplidos = $parametros_rest['post_reportes_cumplidos'];
    $endpoint_post_eliminar_viaje = $parametros_rest['post_delete_viaje'];
    $endpoint_post_editar_viaje = $parametros_rest['post_edit_viaje'];
    $endpoint_get_viaje_por_id = $parametros_rest['get_viaje_por_id'];
    $endpoint_get_remisiones_sin_viaje = $parametros_rest['get_remisiones_sin_viaje'];
    $endpoint_get_consultar_carga = $parametros_rest['get_consultar_carga'];

    // Antiguas variables - abandonado
    // $server_url = variable_get('viajesiron_server_ip_add', '');
    // $server_port = variable_get('viajesiron_server_port', '');
    // $service_url = variable_get('viajesiron_server_url', '');
    // $endpoint_post_crear_viaje = variable_get('viajesiron_post_viaje', '');
    // $endpoint_get_viaje_por_id = variable_get('viajesiron_get_viaje_por_id', '');
    // $endpoint_get_remisiones_sin_viaje = variable_get('viajesiron_get_remisiones_sin_viaje', '');
    // $endpoint_get_consultar_carga = variable_get('viajesiron_get_consultar_carga', '');
    // $endpoint_post_editar_viaje = variable_get('viajesiron_post_editar_viaje', '');
    // $endpoint_post_eliminar_viaje = variable_get('viajesiron_post_delete_viaje', '');
    
    //generar paths
    if ($server_url == ''){
        drupal_set_message(t('No han sido configurados los parametros del servidor'), 'error');
        return false;
    } else {
        $server_path = $server_url . ':' . $server_port . '/' . $service_url;
        $response = array(
            'server_url' => $server_path,
            'url_post_crear_viaje' => $server_path . '/' . $endpoint_post_crear_viaje,
            'url_post_reportes_cumplidos' => $server_path . '/' . $endpoint_post_reportes_cumplidos,
            'url_post_eliminar_viaje' => $server_path . '/' . $endpoint_post_eliminar_viaje,
            'url_post_editar_viaje' => $server_path . '/' . $endpoint_post_editar_viaje,
            'url_get_viaje_por_id' => $server_path . '/' . $endpoint_get_viaje_por_id,
            'url_get_remisiones_sin_viaje' => $server_path . '/' . $endpoint_get_remisiones_sin_viaje,
            'url_get_consultar_carga' => $server_path . '/' . $endpoint_get_consultar_carga,
        );
        return $response;
    }
}


/**
 * Llamado al servicio GET para solicitar viaje por id
 */
function call_get_viaje_por_id($viaje_id) {
    $params = array(
        'propiedad' => 'url_get_viaje_por_id',
        'parametros' => array('id' => $viaje_id),
        'method' => 'GET',
    );
    return llamar_servicio_viajesiron($params);
}


/**
 * Llamado al servicio GET para solicitar remisiones sin viaje
 */
function call_get_remisiones_sin_viaje() {
    $params = array(
        'propiedad' => 'url_get_remisiones_sin_viaje',
        'method' => 'GET',
    );
    return llamar_servicio_viajesiron($params);

    /*/TEMPORAL ************************************************OJO!!!!!!!!!!*******
        $contenido_json = cargar_datos_json();

        // Decodifica el contenido JSON en un array de PHP.
        $test_json = json_decode($contenido_json, TRUE);

        // Verifica si hubo errores durante la decodificación.
        if (json_last_error() !== JSON_ERROR_NONE) {
            // Maneja el error si es necesario.
            add_error('Error al decodificar el archivo JSON.');
        }
        return $test_json;
    //FIN TEMPORAL*/

}


/**
 * Llama al servicio GET para consultar carga y su capacidad
 * @param string $nombre_carga - nombre de la carga a consultar (Carry, Turbo, Sencillo, Doble Troque, Mula)
 * @return array response, FALSE si no hay exito. 
 */
function call_consultar_carga($nombre_carga) {
    $params = array(
        'propiedad' => 'url_get_consultar_carga',
        'parametros' => array('nombre' => $nombre_carga),
        'method' => 'GET',
    );
    return llamar_servicio_viajesiron($params);
}


/**
 * Llama al servicio POST para crear viaje
 * @param string json $proccessed_data
 * @return array response, FALSE si no hay exito
 */
function call_post_crear_viaje_service($proccessed_data){
    $params = array(
        'propiedad' => 'url_post_crear_viaje',
        'method' => 'POST',
        'timeout' => 240,
        'data' => $proccessed_data,
    );
    return llamar_servicio_viajesiron($params);
} 


/**
 * Llama al servicio post para editar viaje
 * @param string json $proccessed_data
 * @return array response, FALSE si no hay exito
 */
function call_post_editar_viaje_service($proccessed_data){
    $params = array(
        'propiedad' => 'url_post_editar_viaje',
        'method' => 'POST',
        'timeout' => 240,
        'data' => $proccessed_data,
    );
    return llamar_servicio_viajesiron($params);
}


/**
 * Llama al servicio post para editar viaje
 * @param long $id
 * @return array response, FALSE si no hay exito
 */
function call_post_eliminar_viaje_por_id($id){
    $params = array(
        'propiedad' => 'url_post_eliminar_viaje',
        'method' => 'POST',
        'timeout' => 240,
        'data' => $id,
    );
    return llamar_servicio_viajesiron($params);
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
 * @return array respuesta a servicioweb consultarTransportadoras 
 */
function call_get_transportadoras() {
    $response = llamar_servicio_web('consultarTransportadoras', array());
    return $response; 
}


/**
 * Llama al servicio y llena la variable con las opciones de conceptos
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
    if (($params['method'] == 'GET') && ($params['parametros'])) {
        $url = $url . '?' . http_build_query($params['parametros']); 
    }

    $options = array();
    $options = array(
        'method' => $params['method'],
        'headers' => array(
            'Content-Type' => 'application/json',
        ),
    );

    if ($params['method'] == 'POST') {
        $options['timeout'] = $params['timeout'];
        $options['data'] = $params['data'];
    }

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


//TEMPORAL**********************
function cargar_datos_json() {
    $datos_temporales ='
    {
        "despachosDTO": [
          {
            "fechaCreacionFormato": "",
            "cargado": true,
            "entregaDTO": {
              "status": false
            },
            "enviado": false,
            "fechaCalculadaEntregaPlanificadaFormato": "",
            "id": 79134,
            "sinErrores": true,
            "status": true,
            "tmsMaestroDTO": {
              "fechaCreacionFormato": "",
              "usuarioCreacion": 1,
              "archivoTMSDTO": {},
              "fechaEntregaPlanificadaFormato": "",
              "fechaExpedicionFormato": "",
              "fechaFacturaFormato": "",
              "fechaNotaEmpaquetadoFormato": "",
              "idExpedicion": "50809847",
              "match": false,
              "status": false
            }
          },
          {
            "fechaCreacionFormato": "",
            "cargado": true,
            "entregaDTO": {
              "status": false
            },
            "enviado": false,
            "fechaCalculadaEntregaPlanificadaFormato": "",
            "id": 78932,
            "sinErrores": true,
            "status": true,
            "tmsMaestroDTO": {
              "fechaCreacionFormato": "",
              "usuarioCreacion": 1,
              "archivoTMSDTO": {},
              "fechaEntregaPlanificadaFormato": "",
              "fechaExpedicionFormato": "",
              "fechaFacturaFormato": "",
              "fechaNotaEmpaquetadoFormato": "",
              "idExpedicion": "50809848",
              "match": false,
              "status": false
            }
          },
          {
            "fechaCreacionFormato": "",
            "cargado": true,
            "entregaDTO": {
              "status": false
            },
            "enviado": false,
            "fechaCalculadaEntregaPlanificadaFormato": "",
            "id": 78933,
            "sinErrores": true,
            "status": true,
            "tmsMaestroDTO": {
              "fechaCreacionFormato": "",
              "usuarioCreacion": 1,
              "archivoTMSDTO": {},
              "fechaEntregaPlanificadaFormato": "",
              "fechaExpedicionFormato": "",
              "fechaFacturaFormato": "",
              "fechaNotaEmpaquetadoFormato": "",
              "idExpedicion": "50809850",
              "match": false,
              "status": false
            }
          },
          {
            "fechaCreacionFormato": "",
            "cargado": true,
            "entregaDTO": {
              "status": false
            },
            "enviado": false,
            "fechaCalculadaEntregaPlanificadaFormato": "",
            "id": 78934,
            "sinErrores": true,
            "status": true,
            "tmsMaestroDTO": {
              "fechaCreacionFormato": "",
              "usuarioCreacion": 1,
              "archivoTMSDTO": {},
              "fechaEntregaPlanificadaFormato": "",
              "fechaExpedicionFormato": "",
              "fechaFacturaFormato": "",
              "fechaNotaEmpaquetadoFormato": "",
              "idExpedicion": "50809855",
              "match": false,
              "status": false
            }
          },
          {
            "fechaCreacionFormato": "",
            "cargado": true,
            "entregaDTO": {
              "status": false
            },
            "enviado": false,
            "fechaCalculadaEntregaPlanificadaFormato": "",
            "id": 78936,
            "sinErrores": true,
            "status": true,
            "tmsMaestroDTO": {
              "fechaCreacionFormato": "",
              "usuarioCreacion": 1,
              "archivoTMSDTO": {},
              "fechaEntregaPlanificadaFormato": "",
              "fechaExpedicionFormato": "",
              "fechaFacturaFormato": "",
              "fechaNotaEmpaquetadoFormato": "",
              "idExpedicion": "50809856",
              "match": false,
              "status": false
            }
          }
        ]
      }';
    return $datos_temporales;
}