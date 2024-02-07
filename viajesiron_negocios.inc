<?php

/**
 * Logica busqueda whs consultado el servicio existente solo para un whs
 *  */
function buscar_whs_por_id($whs, $data)
{
    //llamar servicio y procesar informacion
    $response = call_get_whs_service($whs);
    if ($response) {
        if (isset($response['despachosDTO'])) {
            if ($response['despachosDTO'] != array()) {
                //Consulta OK
                $data['busqueda'] = array(
                    'id' => '',
                    'info' => array(),
                    //'costo' => '',
                    //'fecha' => '',
                );
                $data['busqueda'] = array(
                    'id' => $whs,
                    'info' => $response['despachosDTO'][0]['tmsMaestroDTO'],
                    //'costo' => $response['despachosDTO'][0]['entregaDTO']['costoFlete'],
                    //'fecha' => $response['despachosDTO'][0]['entregaDTO']['fechaCalculadaEntregaPlanificadaFormato'],
                );
            } else {
                //Consulta sin resultado
                $data['busqueda'] = array();
                drupal_set_message(t('No se encontraron despachos con ese número de WHS.'), 'info');
            }
        } else {
            $data['busqueda'] = array();
            drupal_set_message('ERROR AL REALIZAR LA BUSQUEDA, INTENTE NUEVAMENTE');
        }
    }
    return ($data);
}

/**
 * Logica busqueda varios whss consultado el servicio existente para muchos whs
 *  */
function buscar_varios_whs_por_id($whss, $data)
{
    //llamar servicio y procesar informacion
    $response = call_get_varios_whs_service($whss);
    if ($response) {
        if (isset($response['despachosDTO'])) {
            if ($response['despachosDTO'] != array()) {
                //Consulta OK
                $data['busqueda'] = array();
                $peso_busqueda_kg = 0;
                foreach ($response['despachosDTO'] as $despacho) {
                    if (isset($despacho['nombreViaje'])) {
                        $viaje = $despacho['nombreViaje'];
                        $id_viaje = $despacho['idViaje'];
                    } else {
                        $viaje = null;
                        $id_viaje = null;
                    }

                    if (isset($despacho['sinErrores'])) {
                        $sin_errores = $despacho['sinErrores'];
                    } else {
                        $sin_errores = 0;
                    }

                    $peso_despacho =  $despacho['tmsMaestroDTO']['pesoBruto'];
                    if ($despacho['tmsMaestroDTO']['unidadPesoBruto'] == 'LB') {
                        $peso_despacho /= 2;
                    }
                    $peso_busqueda_kg += $peso_despacho;

                    $data['busqueda'][] = array(
                        'id' => $despacho['tmsMaestroDTO']['idExpedicion'],
                        'info' => $despacho['tmsMaestroDTO'],
                        'viaje' => $viaje,
                        'id_viaje' => $id_viaje,
                        'sin_errores' => $sin_errores,
                    );
                }
                $data['peso_busqueda_kg'] = $peso_busqueda_kg;

            } else {
                //Consulta sin resultado
                $data['busqueda'] = array();
                drupal_set_message(t('No se encontraron despachos con esa información.'), 'info');
            }
        } else {
            $data['busqueda'] = array();
            drupal_set_message('ERROR AL REALIZAR LA BUSQUEDA, INTENTE NUEVAMENTE');
        }
    }
    return ($data);
}


/**
 * Logica para guardar el viaje en el servidor y luego en Drupal
 */
function guardar_viaje($data)
{
    //convertir la informacion local en informacion con la estructura del server
    $modo_editar = ($data['id'] != 'new'); //TRUE si se va a editar

    $proccessed_data = convertir_localdata_to_serverdata($data, $modo_editar);
	add_error('datos a guardar:');
    add_error($proccessed_data);

    if ($modo_editar) {
        add_error('editar');
        $response = call_post_editar_viaje_service($proccessed_data);
    } else {
        add_error('crear');
        $response = call_post_crear_viaje_service($proccessed_data);
    }

    if ($response) {
        if (isset($response['viajeBO']['id'])) {
            //recibio una respuesta correcta del servidor, procesar la información
            $data_reporte = convertir_serverdata_to_localdata($response);

            //Guardar datos en el contenido
            $response_drupal = actualizar_nodo($data_reporte);

            if ($response_drupal) {
                return $response['viajeBO']['id'];
            } else {
                drupal_set_message(t('se logró guardar la información en el servidor, pero no localmente. contacte al administrador, consecutivo: @consecutivo', array('@consecutivo' => $response['viajeBO']['id'])), 'error');
                return FALSE;
            }
        } else {
            drupal_set_message(t('Error en la respuesta del servidor'), 'error');
            return FALSE;
        }
    } else {
        drupal_set_message(t('Error al guardar la información en el servidor'), 'error');
        return FALSE;
    }
}


/**
 * Logica para actualizar la información del nodo con información del servidor
 */
function actualizar_nodo($data_reporte)
{
    $node_nid = buscar_nodo_por_id($data_reporte['id']);
    if ($data_reporte['status']) {
        //actualizar nodo con datos del backend
        $actualizado = guardar_datos_drupal($data_reporte, $node_nid);
    } else {      //status = FALSE -> eliminado logicamente en el backend
        $mensaje = 'Viaje eliminado exitosamente';
        if ($node_nid) {
            //eliminar nodo
            $actualizado = eliminar_datos_drupal($node_nid);
            if ($actualizado) {
                $_SESSION['viajesiron_data_reporte'] = array();
                unset($_SESSION['viajesiron_data_reporte']);
                $mensaje .= '.';  //solo usado como verificación del nodo eliminado sin mencionarlo para al usuario.
            }
        } else {
            //ya fue eliminado o no existe.
        }
        drupal_set_message($mensaje, 'info');
        $actualizado = TRUE;
    }
    /*if ($actualizado){
    drupal_set_message('nodo drupal actualizado con información del server', 'info');
  } else {
    drupal_set_message('no pudo ser actualizado el nodo drupal', 'warning');
  }*/
    return ($actualizado);
}


function consultar_viaje_por_id($viaje_id) {

    if (isset($_SESSION['viajesiron_data_reporte']['id'])) {
        if ($_SESSION['viajesiron_data_reporte']['id'] == $viaje_id) {
            return;
        }
    }

    $data_reporte = data_reporte_template();

    //verificar informacion
    if (is_null($viaje_id)) {
        //no manipular la informacion
        drupal_set_message(t('Error, no hay información para llamar del servidor'));
    } else {
        //$response = call_get_viaje_por_id($viaje_id);
        //$decoded_data = json_decode($response->data, true);
        $decoded_data = call_get_viaje_por_id($viaje_id);

        if (isset($decoded_data['viajeBO']['id']) && $decoded_data['viajeBO']['id'] != '') {
            $data = convertir_serverdata_to_localdata($decoded_data);

            if ($data) {
                $data_reporte = $data;
            } else {
                drupal_set_message(t('No fue encontrado esa información (@id) en el servidor', array('@id' => $viaje_id)));
            }
        } else {
            $data_reporte = data_reporte_template();
            drupal_set_message('No hay respuesta del servidor para ese valor de viaje', 'error');
        }
    }

    $_SESSION['viajesiron_data_reporte'] = $data_reporte;
}

function consultar_remisiones_sin_viajes() {
	$decoded_data = call_get_remisiones_sin_viaje();
	return $decoded_data;
}

function consultar_capacidad_carga($nombre_carga) {
    if (isset($_SESSION['viajesrion_capacidad_carga'][$nombre_carga])) {
        return $_SESSION['viajesrion_capacidad_carga'][$nombre_carga];
    } else {
        $decoded_data = call_consultar_carga($nombre_carga);
        if ($decoded_data) {
            $response = array(
                'carga_maxima' => $decoded_data['cargaBO']['cargaMaxima'],
                'carga_minima' => $decoded_data['cargaBO']['cargaMinima'],
            );
            $_SESSION['viajesrion_capacidad_carga'][$nombre_carga] = $response;
        } else {
            $response = FALSE;
        }
        return $response;
    }
}


function cargar_opciones_transportadora()
{
    if (!isset($_SESSION['viajesiron_transportadoras'])) {
        $response = call_get_transportadoras();
        $opciones_transportadora = array();
        foreach ($response['transportadoras'] as $key => $value) {
            $opciones_transportadora[$value['id']] = $value['nombre'];
        }
        $_SESSION['viajesiron_transportadoras'] = $opciones_transportadora;
    }
    return $_SESSION['viajesiron_transportadoras'];
}

function cargar_opciones_conceptos_adicionales()
{
    if (!isset($_SESSION['viajesiron_conceptos_adicionales'])) {
        $response = call_get_conceptos();
        $opciones_conceptos = array();
        foreach ($response['conceptos'] as $key => $value) {
            $opciones_conceptos[$value['id']] = $value['nombre'];
        }
        $_SESSION['viajesiron_conceptos_adicionales'] = $opciones_conceptos;
    }
    return $_SESSION['viajesiron_conceptos_adicionales'];
}


/**
 * Funciones para la busqueda de Viajes
 */
function buscar_viaje_por_fechas($params)
{
    //$params => array('fechaInicial'=>'2022-11-20', 'fechaFinal'=>'2023-12-04', 'idTransportadora'=>'1',);
    $response = call_get_viajes_por_fecha($params); //already decoded
    return $response;
}


/**
 * Funciones para la busqueda de Viajes
 */
function buscar_viaje_por_nombre($params)
{
    //$params => array('nombre'=>'TOXTMS15');
    $response = call_get_viajes_por_nombre($params); //already decoded
    return $response;
}


/**
 * Funcion para hacer llamado a la operación de eliminar viaje
 */
function eliminar_viaje_por_id($id)
{
    $response = call_post_eliminar_viaje_por_id($id); //already decoded
    return $response;
}
