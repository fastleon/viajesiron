<?php
//PLANTILLA de datos de transferencia (temporales)
function datos_en_blanco() {
  $data = array();
  $data = array(
    'id' => 'new',
    'nombre' => 'autogenerado',
    'tipo_carga' => '',
    'tipo_carro' => '',
    'transportadora' => '',
    'adicionales' => array(),
      //'id' => '',
      //'concepto' => '',
      //'costo' => 0,    ),
    'remisiones' => array(),
    'peso_remisiones_kg' => 0,
    'peso_busqueda_kg' => 0,
      //'remision' => '',
      //'costo' => 0,    ),
    'busqueda' => array(),
  );
  send_data($data);
  unset($_SESSION['viajesiron_transportadoras']);
  unset($_SESSION['viajesiron_conceptos_adicionales']);
  unset($_SESSION['viajesrion_capacidad_carga']);
}

//PLANTILLA de datos para reporte (ligado al back)
function data_reporte_template() {
  $data_reporte = array();
  $data_reporte = array(
    'id' => null,
    'nombre' => '',
    'status' => FALSE,
    'tipo_carga' => '',
    'tipo_carro' => '',
    'transportadora' => array(
      'id' => '',
      'nombre' => '',
    ),
    'costo_total_flete' => 0,
    'peso_total_flete' => 0,
    'adicionales' => array(),
      //'id' => '',
      //'concepto' => '',
      //'costo' => 0,     ),
    'remisiones' => array(),
      //'remision' => '',
      //'costo' => 0,
      //'otros: poblacion exp rep peso...     ),
  );
  return $data_reporte;
}


/** 
 * Crear y/o llamar datos de la sesion 
 */
function bring_data() {
  if (isset($_SESSION['viajesiron_data'])){
    //ya existe la variable
  } else {;
    datos_en_blanco(); //generar datos en blanco en la sesion
  }
  return $_SESSION['viajesiron_data']; //devolver datos en la sesion
}


/** 
 * Enviar datos a la variable de sesion
 */ 
function send_data($data) {
  //guardar informacion en sesion
  $_SESSION['viajesiron_data'] = $data;
}


function convertir_serverdata_to_localdata($get_data) {
  $data_reporte = data_reporte_template();
  $server_data = $get_data['viajeBO'];
  $data_reporte['id'] = $server_data['id'];
  $data_reporte['nombre'] = $server_data['nombre'];
  $data_reporte['status'] = $server_data['status'];
  $data_reporte['tipo_carga'] = $server_data['tipoDeCarga']['nombre'];
  if (isset($server_data['tipoDeCarro']['nombre'])) {
    $data_reporte['tipo_carro'] = $server_data['tipoDeCarro']['nombre'];
  } else {
    $data_reporte['tipo_carro'] = '';
  }
  $data_reporte['transportadora']['id'] = $server_data['transportadora']['id'];
  $data_reporte['transportadora']['nombre'] = $server_data['transportadora']['nombre'];
  $data_reporte['costo_total_flete'] = $server_data['costoTotalFlete'];
  $data_reporte['peso_total_flete'] = $server_data['pesoTotalFlete'];
  if (isset($server_data['adicionalViaje'])){
    foreach ($server_data['adicionalViaje'] as $server_adicional){
      $temp_data = array();
      $temp_data['id'] = $server_adicional['conceptoBO']['id'];
      $temp_data['concepto'] = $server_adicional['conceptoBO']['nombre'];
      $temp_data['costo'] = $server_adicional['valor'];
      $data_reporte['adicionales'][] = $temp_data;
    }
  }
  $sin_costo_flete = array();
  foreach ($server_data['despachos'] as $despacho){
    $temp_data = array();
    if (isset($despacho['entregaDTO']['costoFlete'])){
      $costo_flete = $despacho['entregaDTO']['costoFlete'];
    } else {
      //drupal_set_message(t('No se encontró el costo de flete para la remisión @whs', array('@whs' => $despacho['tmsMaestroDTO']['idExpedicion'])), 'info');
      $sin_costo_flete[] = $despacho['tmsMaestroDTO']['idExpedicion']; 
      $costo_flete = 0;
    }
    $temp_data = array(
      'remision' => $despacho['tmsMaestroDTO']['idExpedicion'],
      'costo' => $costo_flete,
      'poblacion_expedidor' => empty($despacho['tmsMaestroDTO']['poblacionExpedidor']) ? '' : $despacho['tmsMaestroDTO']['poblacionExpedidor'],
      'direccion_expedidor' => empty($despacho['tmsMaestroDTO']['direccionExpedidor']) ? '' : $despacho['tmsMaestroDTO']['direccionExpedidor'],
      'poblacion_receptor' => empty($despacho['tmsMaestroDTO']['poblacionReceptor']) ? '' : $despacho['tmsMaestroDTO']['poblacionReceptor'],
      'direccion_receptor' => empty($despacho['tmsMaestroDTO']['direccionReceptor']) ? '' : $despacho['tmsMaestroDTO']['direccionReceptor'],
      'numeroPoblacion' => empty($despacho['tmsMaestroDTO']['numeroPoblacion']) ? '' : $despacho['tmsMaestroDTO']['numeroPoblacion'],
      'zonaVentas' => empty($despacho['tmsMaestroDTO']['zonaVentas']) ? '' : $despacho['tmsMaestroDTO']['zonaVentas'],
      'fecha_entrega' => empty($despacho['fechaCalculadaEntregaPlanificadaFormato']) ? '' : $despacho['fechaCalculadaEntregaPlanificadaFormato'],
      'peso' => empty($despacho['tmsMaestroDTO']['pesoBruto']) ? '' : $despacho['tmsMaestroDTO']['pesoBruto'],
      'unidad_peso' => empty($despacho['tmsMaestroDTO']['unidadPesoBruto']) ? '' : $despacho['tmsMaestroDTO']['unidadPesoBruto'],
    );
    $data_reporte['remisiones'][] = $temp_data;
  }
  if ($sin_costo_flete != array() && $data_reporte['status']) {
    add_error('entrando a error de costo de flete');
    if (count($sin_costo_flete) == 1){
      drupal_set_message(t('No se encontró el costo de flete para la remisión @whs', array('@whs' => $sin_costo_flete[0])), 'info');
    } else {
      drupal_set_message(t('No se encontró el costo de flete para las remisiones @whs', array('@whs' => implode(', ', $sin_costo_flete))), 'info');
    }
  }
  return($data_reporte);
}


/**
 * Convertir Data en formato reporte a formato para editar
 */
function convertir_reporte_to_edit($data_reporte) {
  datos_en_blanco();
  $edit_data = bring_data();
  $edit_data['id'] = $data_reporte['id'];
  $edit_data['nombre'] = $data_reporte['nombre'];
  $edit_data['tipo_carga'] = $data_reporte['tipo_carga'];
  $edit_data['tipo_carro'] = $data_reporte['tipo_carro'];
  $edit_data['transportadora'] = $data_reporte['transportadora']['id'];
  //$edit_data['transportadora']['nombre'] = $data_reporte['transportadora']['nombre'];

  $edit_data['peso_remisiones_kg'] = $data_reporte['peso_total_flete'];
  
  $edit_data['costo_total_flete'] = $data_reporte['costo_total_flete'];
  $edit_data['peso_total_flete'] = $data_reporte['peso_total_flete'];
  
  $edit_data['adicionales'] = $data_reporte['adicionales'];
  $edit_data['remisiones'] = $data_reporte['remisiones'];

  $edit_data['busqueda'] = array();

  send_data($edit_data);

} 

/**
 *  Convertir datos de formulario a datos para el servidor
 */
function convertir_localdata_to_serverdata($data, $modo_editar) {
  //crear variable a enviar
  $server_data = array();

  //Diferente JSON para crear o editar JSON
  if ($modo_editar){
    $server_data['id'] = $data['id'];
  }

  //$server_data['nombre'] = 'ejemplo'; //$data['nombre_viaje'];
  $server_data['transportadora']['id'] = $data['transportadora'];
  $server_data['tipoDeCarga']['nombre'] = $data['tipo_carga'];
  if ($data['tipo_carga'] != 'Paqueteo') {
    $server_data['tipoDeCarro']['nombre'] = $data['tipo_carro'];
  }

  foreach($data['adicionales'] as $adicional){
    if (is_array($adicional) && isset($adicional['id'])) {
      $temp_data = array();
      $temp_data['valor'] = $adicional['costo'];
      $temp_data['conceptoBO']['id'] = $adicional['id'];
      $server_data['adicionalViaje'][] = $temp_data;
    }
  }
  foreach($data['remisiones'] as $remision){
    if (is_array($remision)) {
      $server_data['remisiones'][] = $remision['remision'];
    }
  }
  //Codificar la informacion en un Json
  $proccessed_data = json_encode($server_data);
  return ($proccessed_data);
}


/**
 * CREACION DE TIPO DE CONTENIDO EN MODULO INSTALL 
 * */
function verificar_tipo_contenido() {

  $nombre_contenido = 'cargue_viajes';

  // Verifica si el tipo de contenido ya existe.
  if (crear_tipo_contenido($nombre_contenido)){
    //Modificar campo que viene por defecto
    // Cambiar la etiqueta del campo title
    $field_instance = field_info_instance('node', 'title', $nombre_contenido);
    if ($field_instance) {
      if ($field_instance['label'] != 'Consecutivo'){
        $field_instance['label'] = 'Consecutivo';
        field_update_instance($field_instance);
        drupal_set_message('Campo principal asignado al consecutivo', 'info');
      }
    }
    //Si no hay errores cargamos los campos a vincular
    $campos = cargar_datos();
    //se crean en caso que no existan ya en el sistema
    foreach($campos as $campo){
      if (crear_campo($campo)){
        crear_instancia($campo, $nombre_contenido);
      }
    }
  }

}


/**
 *  Funcion encargada de crear el tipo de contenido en caso que no exista
 * */
function crear_tipo_contenido($nombre_contenido) {
  $revisar_tipo_contenido = node_type_load($nombre_contenido);
  if ($revisar_tipo_contenido) {
    //existe
    drupal_set_message('El tipo de contenido ya existe en el sistema.', 'info');
    return TRUE;
  } else {
    drupal_set_message('No se encontró el tipo de contenido correspondiente, creando...', 'info');
    $tipo_contenido_config = array(
      'type' => $nombre_contenido,
      'name' => 'Cargue de Viajes',
      'description' => t('Campo para guardar datos de los viajes creados con el modulo viajesiron.'),
      'base' => 'node_content',
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'disabled' => FALSE,
    );
    //genera valores de defecto
    $node_type = node_type_set_defaults($tipo_contenido_config);
    // Crea el tipo de contenido.
    node_type_save($node_type);
    drupal_set_message('El tipo de contenido se ha creado exitosamente.', 'info');
    return TRUE;
  }
  return FALSE;
}


/**
 *  Retorna el listado de campos a crear para el tipo de contenido
 */
function cargar_datos() {
  // Revisar campos relacionados
  $campos = array(
    //Se deja el titulo como el nombre de referencia autogenerado en backend
    array(
      'nombre_maq' => 'server_id',
      'nombre_usu' => 'server ID',
      'tipo' => 'text',
      'bloqueado' => TRUE,
      'visible' => FALSE, 
    ),
    array(
      'nombre_maq' => 'tipo_carga',
      'nombre_usu' => 'Tipo de Carga',
      'tipo' => 'text', //'taxonomy_term_reference',
      'bloqueado' => TRUE,
      //'vocabulario' => 'tipo_carga',
    ),
    array(
      'nombre_maq' => 'tipo_carro',
      'nombre_usu' => 'Tipo de Carro',
      'tipo' => 'text', //'taxonomy_term_reference',
      'bloqueado' => TRUE,
    ),
    array(
      'nombre_maq' => 'transportadora',
      'nombre_usu' => 'Transportadora',
      'tipo' => 'text',
      'bloqueado' => TRUE,
    ),
    array(
      'nombre_maq' => 'remisiones',
      'nombre_usu' => 'Remisiones Anexas',
      'tipo' => 'text',
      'bloqueado' => TRUE,
      'cardinality' => -1,
    ),
    array(
      'nombre_maq' => 'costo_total_flete',
      'nombre_usu' => 'Costo total del flete',
      'tipo' => 'text',
      'bloqueado' => TRUE,
    ),
    array(
      'nombre_maq' => 'peso_total_flete',
      'nombre_usu' => 'Peso total del flete',
      'tipo' => 'text',
      'bloqueado' => TRUE,
    ),
  );
  return $campos;
}


/**
 * Funcion para crear cada campo antes de su instancia
 */
function crear_campo($campo_info) {
  //agregamos prefijo para evitar duplicados con otros modulos o campos ya creados
  $nombre_maquina = 'viaje_' . $campo_info['nombre_maq'];
  //verificar si existe el campo
  if (field_info_field($nombre_maquina)) {
    //Existe
    drupal_set_message('El campo: ' . $nombre_maquina . ' ya existe', 'info');
    return TRUE;
  }else {
    $campo_config = array(
      'field_name' => $nombre_maquina,
      'type' => $campo_info['tipo'],
      'cardinality' => 1,
      'translatable' => FALSE,
      'settings' => array(),
    );
    if (isset($campo_info['cardinality'])){
      $campo_config['cardinality'] = $campo_info['cardinality'];
    }
    if ($campo_info['tipo'] == 'text') {
      $campo_config['settings'] = array(
        'max_length' => 255,
      );
    } elseif ($campo_info['tipo'] == 'taxonomy_term_reference') {
      $campo_config['settings'] = array(
        'allowed_values' => array(
          array(
            'vocabulary' => $campo_info['vocabulario'],
            'parent' => 0,
          ),
        ),
      );
    }                         
    field_create_field($campo_config);
    drupal_set_message(t('El campo @campo fue creado exitosamente', array('@campo' => $nombre_maquina)), 'info');
    return TRUE;
  }
  return FALSE;
}


/**
 * Funcion para crear la instancia que vincula el campo con el tipo de contenido
 */
function crear_instancia($campo_info, $nombre_contenido) {
  //agregamos prefijo para evitar duplicados con otros modulos o campos ya creados
  $nombre_maquina = 'viaje_' . $campo_info['nombre_maq'];
  //verificar la instancia
  $instancia_campo = field_info_instance('node', $nombre_maquina, $nombre_contenido);
  if ($instancia_campo){
    drupal_set_message('la instancia para el campo ' . $nombre_maquina . ' ya existe', 'info');
    //Existe!
  }else {
    // Definir la configuración de la instancia del campo para el tipo de contenido.
    $instancia_config = array(
      'field_name' => $nombre_maquina,
      'entity_type' => 'node',
      'bundle' => $nombre_contenido,
      'label' => $campo_info['nombre_usu'],
      'type' => '',
      'settings' => array(),
      '#disable' => FALSE,
    ); 
    if ($campo_info['tipo'] == 'text') {
      $instancia_config['widget'] = array(
        'type' => 'text_textfield',
      );
      $instancia_config['settings'] = array(
        'display_summary' => FALSE,
      );
    } elseif ($campo_info['tipo'] == 'taxonomy_term_reference'){
      $instancia_config['widget'] = array(
        'type' => 'options_select',
      );
    }
    if ($campo_info['bloqueado']) {
      $instancia_config['#disabled'] = TRUE;
    }
    if (isset($campo_info['visible'])){
      if (!$campo_info['visible']) {
        $instancia_config['widget'] = array(
          'type' => 'hidden',
        );
      }
    }
    field_create_instance($instancia_config);
    drupal_set_message('la instancia para el campo ' . $nombre_maquina . ' fue creada exitosamente', 'info');
  } 
}


/**
 * Pasa la información guardada en el servidor al contenido de Drupal
 */
function guardar_datos_drupal($data_reporte, $node_nid=0){
  if($node_nid == 0) { 
    // Crear un nuevo nodo de tipo 'viajes'.
    $node = new stdClass();
    $node->type = 'cargue_viajes';
    $node->title = $data_reporte['nombre']; //asignar como titulo el consecutivo generado desde el server
  } else {
    //Se esta editando un nodo actual, Cargar el nodo por el NID
    $node = node_load($node_nid);
  }

  $node->viaje_server_id['und'][0]['value'] = $data_reporte['id'];
  $node->viaje_transportadora['und'][0]['value'] = $data_reporte['transportadora']['nombre'];
  $node->viaje_costo_total_flete['und'][0]['value'] = $data_reporte['costo_total_flete']; 
  $node->viaje_peso_total_flete['und'][0]['value'] = $data_reporte['peso_total_flete'];

  $node->viaje_tipo_carga['und'][0]['value'] = $data_reporte['tipo_carga'];
  if ($data_reporte['tipo_carga'] == 'Paqueteo'){
    $node->viaje_tipo_carro['und'][0]['value'] = '';  
  } else {
    $node->viaje_tipo_carro['und'][0]['value'] = $data_reporte['tipo_carro'];
  }
  
  /*$term = taxonomy_get_term_by_name($data_reporte['tipo_carga']); // se cambió de taxonomia a datos desde el server
  if (!empty($term)) {
    $tid = key($term);
    $node->viaje_tipo_carga['und'][0]['tid'] = $tid;
  }*/

  $node->viaje_remisiones = array();
  for ($i = 0; $i < count($data_reporte['remisiones']); $i++) {
    $node->viaje_remisiones['und'][$i]['value'] = $data_reporte['remisiones'][$i]['remision'];
  }
  
  if($node_nid == 0) {
    // Asignar el ID del usuario actual al nodo.
    global $user;
    $node->uid = $user->uid;
    
    // Configura el estado del nodo (publicado o no publicado)
    $node->status = 0; // 1 para publicado, 0 para no publicado
    
    // fecha de creación y actualización del nodo
    $node->created = strtotime('now');
    $node->changed = $node->created;
    $node->language = 'es';
  } else {
    $node->changed = strtotime('now');
  }
  
  //add_error($node);
  
  // Guardar el nodo.
  try {
    node_save($node);
    field_attach_update('node', $node);
    return TRUE;
  } catch (Exception $e) {
    return FALSE;
  }
}



/**
 * Funcion para eliminar el nodo de drupal, datos no recuperables
 */
function eliminar_datos_drupal($nid){
  // Verificar permisos antes de eliminar el nodo
  if (node_access('delete', node_load($nid))) {
    // Eliminar el nodo
    node_delete($nid);
    return (TRUE);
  }
  else {
    // No tienes permisos para eliminar el nodo
    drupal_set_message(t('No tienes permisos para eliminar este nodo.'), 'error');
    return (FALSE);
  }
}


/**
 * Buscar el nid del nodo que cumpla con el dato de id=$server_id 
 */
function buscar_nodo_por_id($server_id){
  //realizar consulta
  $nombre_contenido = 'cargue_viajes';
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', $nombre_contenido)
  ->fieldCondition('viaje_server_id', 'value', $server_id, '=')
  ->range(0, 1); //(indice 0, num resultados 1)
  $result = $query->execute();

  // Verificar si se encontraron nodos
  if (!empty($result['node'])) {
    // Obtener el primer nodo encontrado
    $nids = array_keys($result['node']);
    $nid = reset($nids);

    // Cargar el nodo por ID
    //$node = node_load($nid);
    return ($nid);

    /*/ Realizar operaciones con el nodo
    if ($node) {
      drupal_set_message('Nodo encontrado con ID: ' . $nid);
      $valores = array(
        'nombre'=>$node->title,
        'server_id'=>$node->viaje_server_id['und'][0]['value'],
        'tipo_carga'=>$node->viaje_tipo_carga['und'][0]['value'],
        'tipo_carro'=>$node->viaje_tipo_carro['und'][0]['value'],
        'transportadora'=>$node->viaje_transportadora['und'][0]['value'],         
        'costo_total_flete'=>$node->viaje_costo_total_flete['und'][0]['value'],         
        'peso_total_flete'=>$node->viaje_peso_total_flete['und'][0]['value'],
      );
      $remisiones = field_get_items('node', $node, 'viaje_remisiones');
      for ($i = 0; $i < count($remisiones); $i++) {
        //$node->viaje_remisiones['und'][$i]['value'] = $data_reporte['remisiones'][$i]['remision'];
        $valores['remisiones'][] = $node->viaje_remisiones['und'][$i]['value'];
      }
      add_error($valores);
    } else {
      return (0);
    }*/
  } else {
    //drupal_set_message('Nodo no encontrado para el id: ' . $server_id, 'error');
    return (0);
  }

}


/**
 * Generador de formato tipo moneda para drupal
 */
function money_format20($valor) {
  return ('$' . number_format($valor, 0, ',', '.'));
}

/**
 * Verificar que la fecha tiene formato Y-m-d
 */
function verificar_fecha($fecha) {
  if (!preg_match('/^\d{4}-\d{2}-\d{2}$/', $fecha)) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Funcion para llenar el campo de tipo de carro segun el tipo de carga (deshabilitado)
 */
function llamar_variable_tipo_carga() {
  if (isset($_SESSION['viajesiron_rel_carga_carro'])) {
    //no hacer nada
  } else {
    $relacion_carga_carro = array(
      'Paqueteo' => '',
      'Carga Seca' => array(
          'Carry',
          'Turbo',
          'Sencillo',
          'DobleTroque',
          'Mula',
      ),
      'Granel' => array(
          'Turbo',
          'Sencillo',
          'DobleTroque',
          'Mula'
      ),
      'Todas' => array(
          '',
          'Carry',
          'Turbo',
          'Sencillo',
          'DobleTroque',
          'Mula',
      ),
    );
    $_SESSION['viajesiron_rel_carga_carro'] = $relacion_carga_carro;
  }
  return $_SESSION['viajesiron_rel_carga_carro'];
}