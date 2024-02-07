<?php

/** 
 * ARCHIVO DONDE SE MANEJA RENDER DE VENTANA MODAL PARA REMISIONES SIN VIAJE
 */
 
function remisiones_modal_form($form, &$form_state) {

	//add_error("dentro del modal");
	//add_error($form_state);
	//Llamar CSS
	//drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/viajesiron.css');

	/*ctools_include('modal');
	ctools_include('ajax');
	ctools_modal_add_js();*/


	//traer los datos de la consulta GET -> remisiones sin viajes
	$remisiones_sin_viaje = call_get_remisiones_sin_viaje();
	
	//******************************************** */
	$form['#attached']['js'][] = array(
		'type' => 'file',
		'data' => drupal_get_path('module', 'viajesiron') . '/js/cerrar_modal.js',
	  );

	// Fieldset para organizar los elementos relacionados
	$form['container_remisiones'] = array(
		'#type' => 'fieldset',
		'#title' => t('Remisiones'),
	);

	//TITULO
	$form['container_remisiones']['titulo_tabla_remisiones'] = array(
		'#markup' => '<h2>' . t('REMISIONES') . '</h2>',
	);
	//HEADER
	$header = array(
		'remision' => t('Remision'),
	);

	$options = array();
	//traer datos en caso que existan
	foreach ($remisiones_sin_viaje['despachosDTO'] as $remision) {
		$temp = array();
		$temp['remision'] = $remision['tmsMaestroDTO']['idExpedicion'];
		$options[] = $temp;
	}

	//Creamos el render de la tabla
	$form['container_remisiones']['tabla_remisiones'] = array(
		'#type' => 'tableselect',
		'#header' => $header,
		'#options' => $options,
		'#multiple' => TRUE,
		'#empty' => t('Sin datos'),
	);

	//Boton para buscar remisiones seleccionadas
	$form['container_remisiones']['submit_buscar_remisiones'] = array(
		'#name' => 'submit_buscar_remisiones',
		'#type' => 'submit',
		'#value' => t('Buscar Remisiones'),
		'#submit' => array('buscar_remisiones_conformador'),
	);

	return $form;
}


/**
 * Busca las remisiones seleccionadas por medio de la consulta de varias WHSs
 * Los resultados son enviados a la variable $data usada en el conformador de viajes
 */
function buscar_remisiones_conformador($form, $form_state) {
	$data = bring_data();
	$seleccionados = $form_state['input']['tabla_remisiones'];
	$whss_anexar = array();
	foreach ($seleccionados as $seleccion) {
		if (is_numeric($seleccion)) {
			$remision_seleccionada = $form['container_remisiones']['tabla_remisiones']['#options'][$seleccion]['remision'];
			$whss_anexar[] = $remision_seleccionada;
		}
	}
	$whss = implode(',', $whss_anexar);
	$data = buscar_varios_whs_por_id($whss, $data);
	send_data($data);

}
