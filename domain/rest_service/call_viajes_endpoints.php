<?php

/** 
 * PROCESAMIENTO CONSULTAS A LOS SERVICIOS REST PROPIOS DEL MODULO
 */

abstract class CallViajesEndpoints {

    /**
     * tomar los parametros del servidor guardados en la configuracion del modulo Configuracion -> Viajes Iron Module
    */
    private static function load_server_parameters() {
        $parametros_rest = (new DataControlParametrosREST())->llamarCargarDato(60);
        //tomar parametros de los datos guardados
        $server_url = $parametros_rest['server_ip_add'];
        $server_port = $parametros_rest['server_port'];
        $service_url = $parametros_rest['server_url'];
        $endpoint_post_crear_viaje = $parametros_rest['post_viaje'];
        $endpoint_get_viaje_por_id = $parametros_rest['get_viaje_por_id'];
        $endpoint_get_remisiones_sin_viaje = $parametros_rest['get_remisiones_sin_viaje'];
        $endpoint_get_consultar_carga = $parametros_rest['get_consultar_carga'];
        $endpoint_post_editar_viaje = $parametros_rest['post_editar_viaje'];
        $endpoint_post_eliminar_viaje = $parametros_rest['post_delete_viaje'];
        $endpoint_post_reportes_cumplidos = $parametros_rest['post_reportes_cumplidos'];
        
        //generar paths
        if ($server_url == ''){
            drupal_set_message(t('No han sido configurados los parametros del servidor'), 'error');
            return FALSE;
        } else {
            $server_path = $server_url . ':' . $server_port . '/' . $service_url;
            $response = array(
                'server_url' => $server_path,
                'url_post_viaje' => $server_path . '/' . $endpoint_post_crear_viaje,
                'url_get_viaje_por_id' => $server_path . '/' . $endpoint_get_viaje_por_id,
                'url_get_remisiones_sin_viaje' => $server_path . '/' . $endpoint_get_remisiones_sin_viaje,
                'url_get_consultar_carga' => $server_path . '/' . $endpoint_get_consultar_carga,
                'url_post_editar_viaje' => $server_path . '/' . $endpoint_post_editar_viaje,
                'url_post_eliminar_viaje' => $server_path . '/' . $endpoint_post_eliminar_viaje,
                'url_post_reportes_cumplidos' => $server_path . '/' . $endpoint_post_reportes_cumplidos,
            );
            return $response;
        }
    }

/**
     * Llamado a servicios propios del modulo VIAJESIRON
     * @param array params {propiedad, metodo, timeout, data}
     * @return array decoded json
    */
    private static function llamar_servicio_viajesiron($params) {
        
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

    /**
     * Llamado al servicio GET para solicitar viaje por id
     */
    static function call_get_viaje_por_id($viaje_id) {
        $params = array(
            'propiedad' => 'url_get_viaje_por_id',
            'parametros' => array('id' => $viaje_id),
            'method' => 'GET',
        );
        return self::llamar_servicio_viajesiron($params);
    }


    /**
     * Llamado al servicio GET para solicitar remisiones sin viaje
     */
    static function call_get_remisiones_sin_viaje() {
        $params = array(
            'propiedad' => 'url_get_remisiones_sin_viaje',
            'method' => 'GET',
        );
        return self::llamar_servicio_viajesiron($params);
    }


    /**
     * Llama al servicio GET para consultar carga y su capacidad
     * @param string $nombre_carga - nombre de la carga a consultar (Carry, Turbo, Sencillo, Doble Troque, Mula)
     * @return array response, FALSE si no hay exito. 
     */
    static function call_consultar_carga($nombre_carga) {
        $params = array(
            'propiedad' => 'url_get_consultar_carga',
            'parametros' => array('nombre' => $nombre_carga),
            'method' => 'GET',
        );
        return self::llamar_servicio_viajesiron($params);
    }


    /**
     * Llama al servicio POST para crear viaje
     * @param string json $proccessed_data
     * @return array response, FALSE si no hay exito
     */
    static function call_post_crear_viaje_service($proccessed_data) {
        $params = array(
            'propiedad' => 'url_post_viaje',
            'method' => 'POST',
            'timeout' => 240,
            'data' => $proccessed_data,
        );
        return self::llamar_servicio_viajesiron($params);
    } 


    /**
     * Llama al servicio post para editar viaje
     * @param string json $proccessed_data
     * @return array response, FALSE si no hay exito
     */
    static function call_post_editar_viaje_service($proccessed_data) {
        $params = array(
            'propiedad' => 'url_post_editar_viaje',
            'method' => 'POST',
            'timeout' => 240,
            'data' => $proccessed_data,
        );
        return self::llamar_servicio_viajesiron($params);
    }


    /**
     * Llama al servicio post para editar viaje
     * @param long $id
     * @return array response, FALSE si no hay exito
     */
    static function call_post_eliminar_viaje_por_id($id) {
        $params = array(
            'propiedad' => 'url_post_eliminar_viaje',
            'method' => 'POST',
            'timeout' => 240,
            'data' => $id,
        );
        return self::llamar_servicio_viajesiron($params);
    }

    /**
     * Llama al servicio post para reportes cumplidos
     * @param mixed parametros de busqueda
     * @return array response, FALSE si no hay exito
     */
    static function call_post_reportes_cumplidos($proccessed_data) {
        $params = array(
            'propiedad' => 'url_post_reportes_cumplidos',
            'method' => 'POST',
            'timeout' => 240,
            'data' => $proccessed_data,
        );
        return self::llamar_servicio_viajesiron($params);
    }
    
}