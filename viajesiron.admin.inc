<?php

module_load_include('php', 'viajesiron', 'domain\session\data_control');

//PARAMETROS DE CONFIGURACION - MUCHO CUIDADO AL EDITAR
class ConfigEndpoints
{
	public static function getParametros() {
		return array(
			'server_ip_address' => array(
				'title' => 'Ip address para el servidor ej: http://127.0.0.1',
				'old_variable' => 'viajesiron_server_ip_add', 
				'default_value' => 'http://127.0.0.1',
			),
			'server_port' => array(
				'title' => 'Puerto de acceso a los servicios del servidor ej: 8080',
				'old_variable' => 'viajesiron_server_port',
				'default_value' => '8080',
			),
			'server_url' => array(
				'title' => 'URL del servicio de consulta del modulo Viajes',
				'old_variable' => 'viajesiron_server_url',
				'default_value' => 'ToxementRest/TMS',
			),
			'post_viaje' => array(
				'title' => 'operación POST del conformador',
				'old_variable' => 'viajesiron_post_viaje',
				'default_value' => 'crearViajeIRONTEST',
			),
			'post_reportes_cumplidos' => array(
				'title' => 'operación POST para la busqueda filtrada de resultados cumplidos',
				'old_variable' => 'viajesiron_post_resultados_cumplidos',
				'default_value' => 'consultarReporteCumplidosTEST',
			),
			'post_delete_viaje' => array(
				'title' => 'operación POST para eliminar viaje',
				'old_variable' => 'viajesiron_post_delete_viaje',
				'default_value' => 'eliminarViajeIRONTEST',
			),
			'post_edit_viaje' => array(
				'title' => 'operación POST para editar viaje',
				'old_variable' => 'viajesiron_post_editar_viaje',
				'default_value' => 'editarViajeIRONTEST',
			),
			'get_viaje_por_id' => array(
				'title' => 'operación GET de consulta',
				'old_variable' => 'viajesiron_get_viaje_por_id',
				'default_value' => 'consultarViajeIRONPorIdTEST',
			),
			'get_remisiones_sin_viaje' => array(
				'title' => 'operación GET para revisar que remisiones no tienen un viaje asignado',
				'old_variable' => 'viajesiron_get_remisiones_sin_viaje',
				'default_value' => 'consultarWHSSinViajeAsociadoTEST',
			),
			'get_consultar_carga' => array(
				'title' => 'operación GET para consultar capacidad de las cargas',
				'old_variable' => 'viajesiron_get_consultar_carga',
				'default_value' => 'consultarCargaTEST',
			),
		);
	}

	public static function crear_admin_option($field, $default) {
		$default_value = ($default == '') 
			? variable_get($field['old_variable'], $field['default_value'])
			: $default; 
		$form = array(
			'#type' => 'textfield',
			'#title' => t($field['title']),
			'#default_value' => $default_value,
			'#required' => TRUE,
		);
		return $form;
	}
}
/**
 * Admin page callback para modulo Viajes Iron
 */

/**
 * Genera el form para las configuraciones del modulo
 */
function viajesiron_admin_settings_form()
{
	$form = array();
	//Creacion de las opciones de configuracion
	$parametros = ConfigEndpoints::getParametros();
	// $valores_salvados = variable_get('viajesiron_parametros_rest', array());
	$valores_salvados = (new DataControlParametrosREST())->llamarCargarDato();

	foreach ($parametros as $key => $value) {
		$default = isset($valores_salvados[$key]) ? $valores_salvados[$key] : '';
		$form[$key] = ConfigEndpoints::crear_admin_option($value, $default);
	}
	//botones
	$form['submit'] = array(
		'#type' => 'submit',
		'#name' => 'submit',
		'#value' => t('Guardar configuración'),
	);
	$form['submit_tipo_contenido'] = array(
		'#type' => 'submit',
		'#name' => 'revisar_contenido',
		'#value' => t('Revisar tipo de contenido'),
	);
	$form['submit_limpieza'] = array(
		'#type' => 'submit',
		'#name' => 'limpieza_contenido',
		'#value' => t('Limpiar variables temporales'),
	);
	//AGREGAR CAMPO TAMBIEN EN CallViajesEndpoints
	return ($form);
}


/**
 * Implementacion del hook_submit
 */
function  viajesiron_admin_settings_form_submit($form, &$form_state)
{
	//Realizar logica según el boton presionado
	$button_clicked = $form_state['triggering_element']['#name'];

	switch ($button_clicked) {

		case 'submit':
			//Guardando parametros a la sesion
			$parametros = ConfigEndpoints::getParametros();
			$save_data = array();
			foreach ($parametros as $key => $value) {
				$save_data[$key] = $form_state['values'][$key];
			}
			// variable_set('viajesiron_parametros_rest', $save_data);
			(new DataControlParametrosREST())->llamarGuardarDato($save_data);

			//borrar parametros de ultima carga de DATOS EN SESION
			unset($_SESSION[LAST_MOD]);
			unset($_SESSION[MINS_LAST_MOD]);
			drupal_set_message(t('Configuración guardada correctamente.'));
			break;

		case 'revisar_contenido':
			verificar_tipo_contenido();
			drupal_set_message(t('Revisión finalizada'), 'info');
			break;

		case 'limpieza_contenido':
		//variables antiguas de admin
			variable_del('viajesiron_server_ip_add');
			variable_del('viajesiron_server_port');
			variable_del('viajesiron_server_url');
			variable_del('viajesiron_post_viaje');
			variable_del('viajesiron_get_viaje_por_id');
			variable_del('viajesiron_get_remisiones_sin_viaje');
			variable_del('viajesiron_get_consultar_carga');
			variable_del('viajesiron_post_editar_viaje');
			variable_del('viajesiron_post_delete_viaje');
			variable_del('viajesiron_rest_viajes');
		//variables de tiempo
			unset($_SESSION[LAST_MOD]);
			unset($_SESSION[MINS_LAST_MOD]);
		//variables de reportes cumplidos
            (new DataControlReportesCumplidosFormulario())->llamarBorrarDato();
            (new DataControlReportesCumplidosResultados())->llamarBorrarDato();

			drupal_set_message(t('Variables eliminadas'), 'info');
			break;
	}
}
