<?php
/*********  ADMINISTRACION DE LA TAXONOMIA DEL MODULO */

/** 
 * Realizar verificación que todas las taxonomias del modulo existen 
 * y en caso contrario crearlas
 */
function verificar_taxonomia() {
//  revisar_taxonomia('tipo_carga'); //vocabulario para tipos de carga
//  revisar_taxonomia('tipo_adicional'); //vocabulario para categorias de costos adicionales
}

function revisar_taxonomia($nombre_taxonomia) {
  $taxonomia = taxonomy_vocabulary_machine_name_load($nombre_taxonomia);
  if ($taxonomia){
    //Existe
  }else{
    drupal_set_message(t('No existe el vocabulario @vocabulario', array('@vocabulario' => $nombre_taxonomia)), 'warning');
    viajesiron_vocabularies_info($nombre_taxonomia);
    viajesiron_terms_info($nombre_taxonomia);
  }
}

  /**
 * Crea un vocabulario nuevo llamado "Tipo de carga".
 */
function viajesiron_vocabularies_info($nombre_vocabulario) {
  $conf_vocabulario = array();
  switch ($nombre_vocabulario){
    case ('tipo_carga'):
      $conf_vocabulario = array(
        'name' => t('Tipos de Carga'),
        'description' => t('Vocabulario para categorizar los tipos de carga (Viajes)'),
      );
      break;
    case ('tipo_adicional'):
      $conf_vocabulario = array(
        'name' => t('Conceptos costos adicionales'),
        'description' => t("Vocabulario para categorizar los costos adicionales (Viajes)"),
      );
      break;
  }
  //crear el vocabulario en drupal
  $vocabulario = (object)array(
    'name' => $conf_vocabulario['name'],
    'machine_name' => $nombre_vocabulario,
    'description' => $conf_vocabulario['description'],
  );
  taxonomy_vocabulary_save($vocabulario);
  drupal_set_message(t('Agregado vocabulario @vocabulario', array('@vocabulario' => $nombre_vocabulario)), 'info');
  
}

/**
 * Crea los términos "cupo" y "paqueteo" en el vocabulario "Tipo de carga".
 */
function viajesiron_terms_info($nombre_vocabulario) {
  $terminos = array();
  switch ($nombre_vocabulario){
    case ('tipo_carga'):
      $terminos[] = array(
        'name' => 'Paqueteo',
        'description' => 'Tipo de carga con una o varias Expediciones.',
      );
      $terminos[] = array(
        'name' => 'Carga Seca',
        'description' => 'Tipo de carga con una sola Expedicion.',
      );
      $terminos[] = array(
        'name' => 'Granel',
        'description' => 'Tipo de carga con una sola Expedicion.',
      );
      break;
    case ('tipo_adicional'):
      $terminos[] = array(
        'name' => 'Cuadrilla de descargue',
        'description' => 'Costos por contratación de personal adicional para descargue.',
      );
      $terminos[] = array(
        'name' => 'Pernoctaje',
        'description' => 'Costos por pernoctaje.',
      );
      break;
  }
  
  // cargar el id del vocabulario
  $vid = taxonomy_vocabulary_machine_name_load($nombre_vocabulario)->vid;
  foreach ($terminos as $categoria){
    // guardar los términos de taxonomía -Termino Cupo
    $term = new stdClass();
    $term->name = $categoria['name'];
    $term->description = $categoria['description'];
    $term->vid = $vid;
    taxonomy_term_save($term);
  }
  drupal_set_message(t('Agregados Terminos de ejemplo en @vocabulario', array('@vocabulario' => $nombre_vocabulario)), 'info');
  //cache_clear_all('taxonomy:vocabulary:' . $vid, 'cache_taxonomy', TRUE);
}

/**
 * Cargar los terminos del vocabulario ingresado (opciones de los Select Fields)
 * @params $nombre_taxonomia -> nombre del vocabulario a usar de la taxonomia
 * @Return $options -> array con los datos de la taxonomia
 */
function cargar_opciones_taxonomia($nombre_taxonomia) {
  $vid = taxonomy_vocabulary_machine_name_load($nombre_taxonomia);
  $terms = taxonomy_get_tree($vid->vid);
  $options = array();
  foreach ($terms as $term) {
      $options[$term->name] = $term->name;
  }
  return $options;
}