<?php
// $Id$

/**
 * Admin page callback para modulo Viajes Iron
 */

/**
 * Genera el form para las configuraciones del modulo
 */
function viajesiron_admin_settings_form() {
  $form['server_ip_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Ip address para el servidor ej: http://127.0.0.1'),
    '#default_value' => variable_get('viajesiron_server_ip_add', 'http://127.0.0.1'),
    '#required' => TRUE,
  );
  $form['server_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Puerto de acceso a los servicios del servidor ej: 8080'),
    '#default_value' => variable_get('viajesiron_server_port', '8080'),
    '#required' => TRUE,
  );
  $form['server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL del servicio de consulta del modulo Viajes'),
    '#default_value' => variable_get('viajesiron_server_url', 'ToxementRest/TMS'),
    '#required' => TRUE,
  );
  $form['post_viaje'] = array(
    '#type' => 'textfield',
    '#title' => t('operación POST del conformador'),
    '#default_value' => variable_get('viajesiron_post_viaje', 'crearViajeIRONTEST'),
    '#required' => TRUE,
  );
  $form['get_viaje_por_id'] = array(
    '#type' => 'textfield',
    '#title' => t('operación GET de consulta'),
    '#default_value' => variable_get('viajesiron_get_viaje_por_id', 'consultarViajeIRONPorIdTEST'),
    '#required' => TRUE,
  );
  $form['get_remisiones_sin_viaje'] = array(
    '#type' => 'textfield',
    '#title' => t('operación GET para revisar que remisiones no tienen un viaje asignado'),
    '#default_value' => variable_get('viajesiron_get_remisiones_sin_viaje', 'consultarWHSSinViajeAsociadoTEST'),
    '#required' => TRUE,
  );
  $form['get_consultar_carga'] = array(
    '#type' => 'textfield',
    '#title' => t('operación GET para consultar capacidad de las cargas'),
    '#default_value' => variable_get('viajesiron_get_consultar_carga', 'consultarCargaTEST'),
    '#required' => TRUE,
  );
  $form['edit_viaje'] = array(
    '#type' => 'textfield',
    '#title' => t('operación POST para editar viaje'),
    '#default_value' => variable_get('viajesiron_post_editar_viaje', 'editarViajeIRONTEST'),
    '#required' => TRUE,
  );
  $form['delete_viaje'] = array(
    '#type' => 'textfield',
    '#title' => t('operación POST para eliminar viaje'),
    '#default_value' => variable_get('viajesiron_post_delete_viaje', 'eliminarViajeIRONTEST'),
    '#required' => TRUE,
  );
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
  return ($form);

}


/**
 * Implementacion del hook_submit
 */
function  viajesiron_admin_settings_form_submit($form, &$form_state) {
  //Realizar logica según el boton presionado
  $button_clicked = $form_state['triggering_element']['#name'];

  switch ($button_clicked) {

    case 'submit':
      variable_set('viajesiron_server_ip_add', $form_state['values']['server_ip_address']);
      variable_set('viajesiron_server_port',$form_state['values']['server_port']);
      variable_set('viajesiron_server_url', $form_state['values']['server_url']);
      variable_set('viajesiron_post_viaje', $form_state['values']['post_viaje']);
      variable_set('viajesiron_get_viaje_por_id', $form_state['values']['get_viaje_por_id']);
      variable_set('viajesiron_get_remisiones_sin_viaje', $form_state['values']['get_remisiones_sin_viaje']);
      variable_set('viajesiron_get_consultar_carga', $form_state['values']['get_consultar_carga']);
	    variable_set('viajesiron_post_editar_viaje', $form_state['values']['edit_viaje']);
      variable_set('viajesiron_post_delete_viaje', $form_state['values']['delete_viaje']);
    
      drupal_set_message(t('Configuración guardada correctamente.'));
      break;

    case 'revisar_contenido':
      verificar_tipo_contenido();
      drupal_set_message(t('Revisión finalizada'), 'info');
      break;
    }
    
}