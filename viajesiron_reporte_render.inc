<?php
/** 
 * ARCHIVO DONDE SE MANEJA TODO EL RENDER
 * REPORTE CONFORMADOR DE VIAJES
 */

//render paginma reporte Datos Viaje
function datos_viaje_reporte_form($form, &$form_state) {
  //Llamar CSS
  drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/viajesiron.css');
  //Configurar como colombia el formato de moneda
  setlocale(LC_MONETARY, 'es_CO');

  /*/ Cargar la clase EntityFieldQuery si no está disponible
  if (!class_exists('EntityFieldQuery')) {
    module_load_include('inc', 'entity', 'includes/entity.query');
  }*/

  //traer información
  if (isset($_SESSION['viajesiron_data_reporte'])){
    $data_reporte = $_SESSION['viajesiron_data_reporte'];
    actualizar_nodo($data_reporte);
    if ($data_reporte['status'] == FALSE){
      $data_reporte = data_reporte_template();
    }
  } else {
    $data_reporte = data_reporte_template();
  }

  //Titulo
  $form['titulo'] = array(
    '#markup' => '<h2>' . t('REPORTE VIAJE : ') . '</h2>', //@nombre.', array('@nombre' => $data_reporte['nombre_viaje'])) . '</h2>',
  );
    /* / Campo de texto - ID - Relacion con Backend
  $form['id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID'),
    '#value' => $data_reporte['id'],
    '#required' => FALSE,
    '#disabled' => TRUE, 
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );*/

  // Nombre del viaje
  //Select de texto - Consecutivo -> Tipo de contenido Drupal
  $form['nombre_viaje'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Consecutivo : ') . '</h3>',
    '#default_value' => $data_reporte['nombre'],
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );

  // tipo de carga
  //Select de texto - Tipo Carga -> Tipo de contenido Drupal
  $form['tipo_carga'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Tipo de carga: ') . '</h3>',
    '#default_value' => $data_reporte['tipo_carga'],
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );
  // tipo de carro
  //Select de texto - Tipo Carga -> Tipo de contenido Drupal
  if ($data_reporte['tipo_carga'] == 'Paqueteo'){
    $data_reporte['tipo_carro'] = '';
  } //no mostrar si el tipo de carga es paqueteo
  $form['tipo_carro'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Tipo de carro: ') . '</h3>',
    '#default_value' => $data_reporte['tipo_carro'],
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );

  // transportadora
  //Select de texto - Tipo Despacho -> Tipo de contenido Drupal
  $form['transportadora'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Transportadora : ') . '</h3>',
    '#default_value' => $data_reporte['transportadora']['nombre'],
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );

  // Costo Total
  //Select de texto - Tipo Despacho -> Tipo de contenido Drupal
  $valor_moneda = money_format20($data_reporte['costo_total_flete']);
  $form['costo_total_flete'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Costo total del flete : ') . '</h3>',
    '#default_value' => $valor_moneda,
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );

  // Peso Total
  //Select de texto - Tipo Despacho -> Tipo de contenido Drupal
  $form['peso_total_flete'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Peso total del flete : ') . '</h3>',
    '#default_value' => round($data_reporte['peso_total_flete'], 2)  . ' KG',
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-texfields-reporte'),
    ),
  );

  //Separación
  $form['separacion_superior'] = array(
    '#type' => 'markup',
    '#markup' => '<hr>',
  );

  //Listado de  - Tabla -> Datos Externos
  $form = datos_form($form, $data_reporte);

  //Listado de Adicionales - Tabla -> Datos Externos
  $form = datos_adicionales_form($form, $data_reporte);

  //Boton de editar
  $form['submit_editar'] = array(
    '#name' => 'Editar',
    '#type' => 'submit',
    '#value' => t('Editar'),
  );

  //Boton de eliminar
  $form['submit_eliminar'] = array(
    '#name' => 'Eliminar',
    '#type' => 'submit',
    '#value' => t('Eliminar'),
  );

  return $form;
}


/**
 * Generar Render para TABLA 
 */
function datos_form($form, $data_reporte) {
  if (isset($data_reporte['remisiones']) && ($data_reporte['remisiones'] != array())) {
    $info = $data_reporte['remisiones'];
    add_error($info, 'info');

    //Titulo
    $form['fs_']['titulo_tabla_'] = array(
      '#markup' => '<h5>' . t(' anexas :') . '</h5>',
    );

    //header
    $header = array(
      'remisión' => t('Remision'),
      'poblacion_expedidor' => t('Población de Expedidor'),
      'direccion_expedidor' => t('Dirección de Expedidor'),
      'poblacion_receptor' => t('Población del Receptor'),
      'direccion_receptor' => t('Dirección del Receptor'),
      'zonaVentas' => t('Zona de Ventas'),
      'numeroPoblacion' => t('Numero de Población'),
      'fecha_entrega' => t('Fecha entrega calculada'),
      'costo' => t('Costo Flete'),
      'peso' => t('Peso Bruto'),
      'unidad_peso' => t('Unidad'),
    );

    //datos
    foreach($info as $remision){
      if (isset($remision['remision'])) {
        $URL_REMISION_DETAIL = 'ironconsulta/report/detail';
        $link_options = array(
          'query' => array('idExpedicion' => $remision['remision']),
          'attributes' => array('target' => '_blank'),
        );
        $temp_data = array(
          'remision' => l($remision['remision'], $URL_REMISION_DETAIL, $link_options),
          'poblacion_expedidor' => empty($remision['poblacion_expedidor']) ? '' : $remision['poblacion_expedidor'],
          'direccion_expedidor' => empty($remision['direccion_expedidor']) ? '' : $remision['direccion_expedidor'],
          'poblacion_receptor' => empty($remision['poblacion_receptor']) ? '' : $remision['poblacion_receptor'],
          'direccion_receptor' => empty($remision['direccion_receptor']) ? '' : $remision['direccion_receptor'],
          'zonaVentas' => empty($remision['zonaVentas']) ? '' : $remision['zonaVentas'],
          'numeroPoblacion' => empty($remision['numeroPoblacion']) ? '' : $remision['numeroPoblacion'],
          'fecha_entrega' => empty($remision['fecha_entrega']) ? '' : $remision['fecha_entrega'],
          'costo' => empty($remision['costo']) ? '' : money_format20($remision['costo']),
          'peso' => empty($remision['peso']) ? '' : round($remision['peso'], 2),
          'unidad_peso' => empty($remision['unidad_peso']) ? '' : $remision['unidad_peso'],
        );
        $options[] = $temp_data;
      }
    }
    //Creamos el render de la tabla
    $form['fs_']['tabla_'] = array(
      '#name' => 'tabla_',
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#attributes' => array(
        'class' => array('vi-reporte-tabla-'),
        ),
      );
    }
    return $form;
}

/**
 * Generar Render para TABLA ADICIONALES
 */
function datos_adicionales_form($form, $data_reporte) {
  if (isset($data_reporte['adicionales']) && ($data_reporte['adicionales'] != array())) {
    $info = $data_reporte['adicionales'];
    //Titulo
    $form['fs_adicionales']['titulo_tabla_adicionales'] = array(
      '#markup' => '<h5>' . t('Costos adicionales :') . '</h5>',
    );
    //header
    $header = array(
      'concepto' => t('Concepto'),
      'costo' => t('Valor'),
    );
    //datos
    $options = array();
    foreach($info as $adicional){
      if (isset($adicional['concepto'])) {
        $options[] = array(
          'concepto' => $adicional['concepto'],
          'costo' => array(
            'data' => money_format20($adicional['costo']),
            'class' => array('valor-peso-derecha'),
          )
        );
      } 
    }
    //Creamos el render de la tabla
    $form['fs_adicionales']['tabla_adicionales'] = array(
      '#name' => 'tabla_adicionales',
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
      '#attributes' => array(
        'class' => array('vi-reporte-tabla-adicionales'),
      ),
    );
  }
  return $form;
}

function datos_viaje_reporte_form_validate($form, &$form_state){

}

function datos_viaje_reporte_form_submit($form, &$form_state){
  //traer información
  if (isset($_SESSION['viajesiron_data_reporte'])){
    $data_reporte = $_SESSION['viajesiron_data_reporte'];
  } else {
    $data_reporte = data_reporte_template();
  }
  
  $button_clicked = $form_state['triggering_element']['#name'];

  switch ($button_clicked){
    case 'Editar':
      //Abrir nueva pagina con la informacion a editar
      if (isset($_SESSION['viajesiron_data_reporte']) && $_SESSION['viajesiron_data_reporte']['status']){
        $data_reporte = $_SESSION['viajesiron_data_reporte'];
        convertir_reporte_to_edit($data_reporte);
        $url_redirect = 'viajes/editar';
        drupal_goto($url_redirect);
      } else {
        drupal_set_message('No hay información para editar');
      }
      break;

    case 'Eliminar':
      if (isset($_SESSION['viajesiron_data_reporte'])){
        $data_reporte = $_SESSION['viajesiron_data_reporte'];
        $data = array();
        $data['id'] = $data_reporte['id'];
        $id_eliminar = json_encode($data);
        $response = eliminar_viaje_por_id($id_eliminar);
        $eliminado = !$response['viajeBO']['status'];
        if ($eliminado) {
          $data_reporte['status'] = FALSE;
          add_error ('eliminado?:' . actualizar_nodo($data_reporte));  //NO QUITAR!!!
          //la eliminación del viaje (nodo) se hace al actualizar el nodo con el backend (funcion: actualizar_nodo)
          $url_redirect = 'listado-viajes';
          drupal_goto($url_redirect);
        }
      } else {
        drupal_set_message('No hay información para eliminar');
      }
      break;
  }

  
}