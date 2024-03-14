<?php

/** 
 * ARCHIVO DONDE SE MANEJA TODO EL RENDER
 * FORMULARIO CONFORMADOR DE VIAJES
 */

//render formulario conformador Datos Viaje
function datos_viaje_formulario_form($form, &$form_state)
{
  //Llamar CSS
  drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/viajesiron.css');

  //Configurar como colombia el formato de moneda
  setlocale(LC_MONETARY, 'es_CO');

  $data = bring_data();

  //key -> carga, values -> carro
  $tipo_carga_carro = llamar_variable_tipo_carga();

  //Titulo
  $form['titulo_datos_base'] = array(
    '#markup' => '<h2>' . t('Datos del Viaje:') . '</h2>',
  );
  // Campo de texto - ID - Relacion con Backend
  $form['id'] = array(
    '#type' => 'hidden',
    '#title' => t('ID'),
    '#value' => $data['id'],
    '#required' => FALSE,
    '#disabled' => TRUE,
  );

  //Campo de texto - Nombre del viaje -> Tipo de contenido Drupal
  $form['nombre'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#title' => '<h3>' . t('Nombre del viaje: ') . '</h3>',
    '#default_value' => 'autogenerado',
    '#maxlength' => 40,
    '#required' => FALSE,
    '#disabled' => TRUE,
    '#attributes' => array(
      'class' => array('vi-conformador-textfield'),
    ),
  );
  if (isset($data['nombre'])) {
    $form['nombre']['#default_value'] = $data['nombre'];
  }

  //Cargar las opciones para el TIPO CARGA
  $options = array(
    'Carga Seca',
    'Granel',
    'Paqueteo',
  ); //cargar_opciones_taxonomia('tipo_carga');
  $options = drupal_map_assoc($options);
  //Select de texto - Tipo Carga -> Tipo de contenido Drupal
  $form['tipo_carga'] = array(
    '#name' => 'tipo_carga',
    '#type' => 'select',
    '#title' => '<h3>' . t('Tipo de carga: ') . '</h3>',
    '#description' => t('Opciones predeterminadas'),
    '#options' => $options,
    '#required' => TRUE,
    '#attributes' => array(
      'class' => array('vi-conformador-textfield'),
    ),
    //**************************************TTTTTTTTTTTTTTTT */
    '#element_validate' => array('datos_viaje_formulario_guardar_dato'),
    '#ajax' => array(
      'callback' => 'tipo_carga_ajax_callback',
      'wrapper' => array('tipo_carro_wrapper', 'info-capacidad-wrapper'),
    ),
    '#default_value' => ($data['tipo_carga'] != '') ? $data['tipo_carga'] : reset($options),
  );

  //Cargar las opciones para el TIPO CARRO
  //Select de texto - Tipo Carro -> Tipo de contenido Drupal
  $form['tipo_carro_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="tipo_carro_wrapper">',
    '#suffix' => '</div>'
  );

  $tipo_carga = (isset($form['tipo_carga']['#default_value'])) ? $form['tipo_carga']['#default_value'] : 'Todas';
  $options2 = check_valores_tipo_carro($tipo_carga);
  $options2 = drupal_map_assoc($options2);

  $form['tipo_carro_wrapper']['tipo_carro'] = array(
    '#name' => 'tipo_carro',
    '#type' => 'select',
    '#title' => '<h3>' . t('Tipo de carro: ') . '</h3>',
    '#description' => t('depende del tipo de carga'),
    '#options' => $options2,
    '#attributes' => array('class' => array('vi-conformador-textfield'),),
    '#element_validate' => array('datos_viaje_formulario_guardar_dato'),
    '#ajax' => array(
      'callback' => 'ejecutar_capacidad_carga_callback',
      'wrapper' => 'info-capacidad-wrapper',
    ),
    '#default_value' => $data['tipo_carro'],
  );

  
  //Cargar las opciones para TRANSPORTADORA del servidor
  $options = cargar_opciones_transportadora();
  $form['transportadora'] = array(
    '#name' => 'transportadora',
    '#type' => 'select',
    '#title' => '<h3>' . t('Transportadora: ') . '</h3>',
    '#description' => t('datos provenientes del servidor'),
    '#options' => $options,
    '#required' => TRUE,
    '#element_validate' => array('datos_viaje_formulario_guardar_dato'),
    //'#ajax' => array('callback' => 'test'),
    '#attributes' => array(
      'class' => array('vi-conformador-textfield'),
    ),
  );
  //Cargar dato por defecto en tipo_carga segun si existe una sesion o no
  if (!($data['transportadora'] == '')) {
    $form['transportadora']['#default_value'] = $data['transportadora'];
  }

  //ADVERTENCIA Mensaje de información de la capacidad de carga
  $form['info_capacidad_wrapper'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="info-capacidad-wrapper">',
    '#suffix' => '</div>',
    //'#attributes' => array('style' => 'display:none;'),
  );
  $mensaje = verificar_capacidad_carga_callback()['#markup']; 
  $form['info_capacidad_wrapper']['info_capacidad'] = array(
    '#markup' => $mensaje,
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  
  //Boton para evitar que se active algun otro al presiona la tecla enter
  $form['invisible_submit'] = array(
    '#type' => 'submit',
    '#name' => 'none',
    '#value' => 'Submit Invisible',
    '#attributes' => array('style' => 'display:none;'),
  );
  
  /*//PRUEBA****************************************************
  $form['boton_pruebas'] = array(
    '#type' => 'submit',
    '#name' => 'pruebas',
    '#value' => 'Pruebas',
    '#submit' => array('fantasma'),
  );*/

  //Separación
  $form['separacion_viajes'] = array(
    '#type' => 'markup',
    '#markup' => '<hr>',
  );

  //Listado de remisiones - Tabla -> Datos Externos
  $form = datos_remisiones_formulario_form($form, $data);

  //Listado de Adicionales - Tabla -> Datos Externos
  $form = datos_adicionales_formulario_form($form, $data);

  /* BOTON GUARDAR */
  $form['submit_guardar_viaje'] = array(
    '#name' => 'submit_guardar_viaje',
    '#type' => 'submit',
    '#value' => t('Guardar Viaje'),
  );

  /* BOTON LIMPIAR */
  $form['submit_limpiar_formulario'] = array(
    '#name' => 'submit_limpiar',
    '#type' => 'submit',
    '#value' => t('Limpiar Formulario'),
  );

  return $form;
}

/**
 * Generar Render para TABLA ADICIONALES
 */
function datos_adicionales_formulario_form($form, $data)
{
  // Fieldset para organizar los elementos relacionados
  $form['fs_adicionales'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adicionales'),
  );
  //Titulo
  $form['fs_adicionales']['titulo_tabla_adicionales'] = array(
    '#markup' => '<h2>' . t('COSTOS ADICIONALES') . '</h2>',
  );
  /*****TABLA*****/
  //header
  $header = array(
    //'id' => 'ID',
    'concepto' => t('Concepto'),
    'costo' => t('Costo'),
  );
  //Crear adicional en blanco
  $options = array();
  //traer datos en caso que existan

  foreach ($data['adicionales'] as $adicional) {
    if (isset($adicional['id'])) {
      if ($adicional['id'] != '') {
        $temp = array();
        $temp['concepto'] = $adicional['concepto'];
        $temp['costo'] = money_format20($adicional['costo']);
        $options[] = $temp;
      }
    }
  }
  //Quitar el campo 'ID' antes de mostrar datos
  unset($options['id']);
  //Creamos el render de la tabla
  $form['fs_adicionales']['tabla_adicionales'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#multiple' => FALSE,
    '#empty' => t('Sin datos'),
    '#attributes' => array(
      'class' => array('vi-conformador-tabla-adicionales'),
    ),
  );

  /*****CAMPOS*****/
  //Cargar las opciones para la CONCEPTO ADICIONAL del servidor
  $options = cargar_opciones_conceptos_adicionales(); //cargar_opciones_taxonomia('tipo_adicional');
  $form['fs_adicionales']['campo_tipo_adicional_agregar'] = array(
    '#type' => 'select',
    '#title' => '<h3>' . t('Concepto: ')  . '</h3>',
    '#description' => t('Opciones actuales en el servidor'),
    '#options' => $options,
    '#required' => FALSE,
    '#attributes' => array(
      'class' => array('vi-conformador-textfield'),
    ),
  );
  //Campo de texto - COSTO ADICIONAL
  $form['fs_adicionales']['campo_costo_adicional_agregar'] = array(
    '#type' => 'textfield',
    '#title' => '<h3>' . t('Costo') . '</h3>',
    '#description' => t('solo números, sin caracteres'),
    '#maxlength' => 15,
    '#size' => 15,
    '#required' => FALSE,
    '#disabled' => FALSE,
    '#attributes' => array(
      'class' => array('vi-conformador-textfield'),
    ),
  );
  // Boton AGREGAR ADICIONAL
  $form['fs_adicionales']['submit_agregar_adicional'] = array(
    '#name' => 'submit_agregar_adicional',
    '#type' => 'submit',
    '#value' => t('Agregar Adicional'),
  );
  //Boton para ELIMINAR ADICIONAL
  $form['fs_adicionales']['submit_eliminar_adicional'] = array(
    '#name' => 'submit_eliminar_adicional',
    '#type' => 'submit',
    '#value' => t('Eliminar Adicional'),
  );
  //Separación
  $form['separacion_adicionales'] = array(
    '#type' => 'markup',
    '#markup' => '<hr>',
  );
  return $form;
}

/**
 * Generar Render para TABLA REMISIONES
 */
function datos_remisiones_formulario_form($form, $data)
{
  // Fieldset para organizar los elementos relacionados
  $form['fs_remisiones'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remisiones'),
  );

  //LINK A REMISIONES SIN VIAJE
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();

  $text = 'VER REMISIONES SIN ANEXAR (VIAJE)';
  $output = 'ajax/nojs/show/modal';
  $alt = 'Mostrar remisiones sin anexara un viaje';

  $link = ctools_modal_text_button($text, $output, $alt);
  $form['fs_remisiones']['popup_link'] = array(
    '#markup' => '<strong>' . $link . '</strong>',
  );

  //TITULO
  $form['fs_remisiones']['titulo_tabla_remisiones'] = array(
    '#markup' => '<h2>' . t('REMISIONES') . '</h2>',
  );
  //HEADER
  $header = array(
    'remision' => t('Remision'),
    'poblacion_expedidor' => t('Población de expedidor'),
    'direccion_expedidor' => t('Dirección de expedidor'),
    'poblacion_receptor' => t('Población del receptor'),
    'direccion_receptor' => t('Dirección del receptor'),
    'zonaVentas' => t('Zona de Ventas'),
    'numeroPoblacion' => t('Numero de Población'),
    'fecha_entrega' => t('Fecha entrega planificada'),
    'peso' => t('Peso Bruto'),
    'unidad_peso' => t('Unidad'),
  ); 

  $options = array();
  //traer datos en caso que existan
  foreach ($data['remisiones'] as $remision) {
    if (isset($remision['remision'])) {
      if ($remision['remision'] != '') {
        $options[] = $remision;
      }
    }
  }

  //quitamos la columna id
  unset($options['id']);
  //Creamos el render de la tabla
  $form['fs_remisiones']['tabla_remisiones'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#multiple' => TRUE,
    '#empty' => t('Sin datos'),
    '#attributes' => array(
      'id' => 'tabla-remisiones', // Añade el identificador a la tabla *******TEST**********
    ),
  );

  //Boton para eliminar remision
  $form['fs_remisiones']['submit_eliminar_remision'] = array(
    '#name' => 'submit_eliminar_remision',
    '#type' => 'submit',
    '#value' => t('Quitar Remisión'),
    '#executes_submit_callback' => TRUE,
  );

  //Separación
  $form['fs_remisiones']['separacion_remisiones'] = array(
    '#type' => 'markup',
    '#markup' => '<hr> <br>',
  );

  //Container busqueda de resultados
  $form['fs_remisiones']['container_busqueda'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('vi-conformador-busqueda-whs'),
    ),
  );
  // Campo de texto para ingresar el ID de búsqueda
  $form['fs_remisiones']['container_busqueda']['campo_whs'] = array(
    '#type' => 'textfield',
    '#title' => '<h5>' . t('Escriba el WHS a buscar :') . '</h5>',
    '#size' => 200,
    '#maxlength' => 1000,
    '#attributes' => array(
      'class' => array('vi-conformador-textfield'),
    ),
  );
  // Botón para realizar la búsqueda
  $form['fs_remisiones']['container_busqueda']['submit_buscar_whs'] = array(
    '#name' => 'submit_buscar_whs',
    '#type' => 'submit',
    '#value' => t('Buscar WHS'),
    '#executes_submit_callback' => TRUE,
  );

  //Resultado de busqueda
  $form = busqueda_remisiones_formulario_form($form, $data);

  return $form;
}


//Busqueda WHS - Tabla -> Datos Externos
function busqueda_remisiones_formulario_form($form, $data)
{
  /**
   * Renderizar tabla de resultados por whs
   */
  if (isset($data['busqueda']) && ($data['busqueda'] != array())) {
    //Titulo
    $form['fs_remisiones']['titulo_tabla_resultados_whs'] = array(
      '#markup' => '<h5>' . t('Resultados de la busqueda :') . '</h5>',
    );
    //header
    $header = array(
      'id' => t('Remisión'),
      'Fecha_cargue' => t('Fecha de cargue'),
      'Archivo' => t('Archivo origen'),
      'Estado' => t('Estado'),
      'Salida' => t('Salida Producto'),
      'Empresa' => t('Empresa Receptora'),
      'Direccion_expedidor' => t('Dirección de expedidor'),
      'Poblacion_expedidor' => t('Población de expedidor'),
      'Direccion_receptor' => t('Dirección del receptor'),
      'Poblacion_receptor' => t('Población del receptor'),
      'zonaVentas' => t('Zona de Ventas'),
      'numeroPoblacion' => t('Numero de Población'),
      'Peso' => t('Peso Bruto'),
      'Unidad_peso' => t('Unidad'),
      'Fecha_empaquetado' => t('Fecha nota Empaquetado'),
      'Fecha_entrega' => t('Fecha entrega calculada'),
      'Sin_errores' => t('Validado'),
      'Viaje_Actual' => t('Anexo a Viaje')
    );
    //datos
    $options = array();
    //add_error($data['busqueda']);
    foreach ($data['busqueda'] as $resultado) {

      //Enlace al Viaje en caso de estar anexado en alguno
      if (is_null($resultado['viaje'])) {
        $viaje_actual = 'Ninguno';
      } else {
        $URL_VIAJE_DETALLE = 'viajes/' . $resultado['id_viaje'];
        $link_options = array(
          'attributes' => array('target' => '_blank'),
        );
        $viaje_actual = l($resultado['viaje'], $URL_VIAJE_DETALLE, $link_options);
      }

      //Enlace a la remision
      $URL_REMISION_DETAIL = 'ironconsulta/report/detail';
      $link_options = array(
        'query' => array('idExpedicion' => $resultado['id']),
        'attributes' => array('target' => '_blank'),
      );
      $remision_link = l($resultado['id'], $URL_REMISION_DETAIL, $link_options);

      $options[] = array(
        'id' => $remision_link,
        'Fecha_cargue' => empty($resultado['info']['fechaModificacionFormato']) ? '' : substr($resultado['info']['fechaModificacionFormato'], 0, 10),
        'Archivo' => empty($resultado['info']['archivoTMSDTO']['nombre']) ? '' : $resultado['info']['archivoTMSDTO']['nombre'],
        'Estado' => empty($resultado['info']['status']) ? '' : $resultado['info']['status'],
        'Salida' => empty($resultado['info']['salidaProducto']) ? '' : $resultado['info']['salidaProducto'],
        'Empresa' => empty($resultado['info']['empresaReceptora']) ? '' : $resultado['info']['empresaReceptora'],
        'Direccion_expedidor' => empty($resultado['info']['direccionExpedidor']) ? '' : $resultado['info']['direccionExpedidor'],
        'Poblacion_expedidor' => empty($resultado['info']['poblacionExpedidor']) ? '' : $resultado['info']['poblacionExpedidor'],
        'Direccion_receptor' => empty($resultado['info']['direccionReceptor']) ? '' : $resultado['info']['direccionReceptor'],
        'Poblacion_receptor' => empty($resultado['info']['poblacionReceptor']) ? '' : $resultado['info']['poblacionReceptor'],
        'zonaVentas' => empty($resultado['info']['zonaVentas']) ? '' : $resultado['info']['zonaVentas'],
        'numeroPoblacion' => empty($resultado['info']['numeroPoblacion']) ? '' : $resultado['info']['numeroPoblacion'],
        'Peso' => empty($resultado['info']['pesoBruto']) ? '' : $resultado['info']['pesoBruto'],
        'Unidad_peso' => empty($resultado['info']['unidadPesoBruto']) ? '' : $resultado['info']['unidadPesoBruto'],
        'Fecha_empaquetado' => empty($resultado['info']['fechaNotaEmpaquetadoFormato']) ? '' : substr($resultado['info']['fechaNotaEmpaquetadoFormato'], 0, 10),
        'Fecha_entrega' => empty($resultado['info']['fechaEntregaPlanificadaFormato']) ? '' : substr($resultado['info']['fechaEntregaPlanificadaFormato'], 0, 10),
        'Sin_errores' => array(
          'data' => $resultado['sin_errores'] ? 'Sin Errores' : 'Con Errores',
          'class' => $resultado['sin_errores'] ? 'clase-sin-errores' : 'clase-con-errores',
        ),
        'Viaje_Actual' => $viaje_actual,
      );
    }
    //Render de la tabla
    $form['fs_remisiones']['tabla_resultados_whs'] = array(
      '#name' => 'tabla_resultados_whs',
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $options,
    );
    //Mensaje de información del peso total en kg de la busqueda 
    $peso_busqueda_kg = (isset($data['peso_busqueda_kg'])) ? $data['peso_busqueda_kg'] : 0;
    $form['fs_remisiones']['info_peso_busqueda'] = array(
      '#markup' => t('El peso total de las remisiones en esta busqueda es:<strong>@peso</strong> Kg', array('@peso' => $peso_busqueda_kg)),
      '#prefix' => '<p id="info-peso-busqueda">',
      '#suffix' => '</p>',
    );
    //Boton para Anexar remision
    $form['fs_remisiones']['submit_anexar_remision'] = array(
      '#name' => 'submit_anexar_remision',
      '#type' => 'submit',
      '#value' => t('Anexar Información'),
      //PRUEBA AJAX **********************************************************************PPPPPP
      /*'#ajax' => array(
        'callback' => 'anexar_remision_callback',
      ),*/
    );
  }
  return $form;
}


function tipo_carga_ajax_callback($form, $form_state)
{
  $options = check_valores_tipo_carro($form_state['values']['tipo_carga']);
  $options = drupal_map_assoc($options);
  switch ($form_state['values']['tipo_carga']) {
    case 'Paqueteo':
      $form['tipo_carro_wrapper']['tipo_carro']['#disabled'] = TRUE;
      $form['tipo_carro_wrapper']['tipo_carro']['#type'] = 'hidden';
      break;
    case 'Carga Seca':
      $form['tipo_carro_wrapper']['tipo_carro']['#disabled'] = FALSE;
      break;
    case 'Granel':
      $form['tipo_carro_wrapper']['tipo_carro']['#disabled'] = FALSE;
      break;
    default:
      add_error('ERROR AL CARGAR SELECT');
      break;
  }

  $form['tipo_carro_wrapper']['tipo_carro']['#options'] = $options;
  $form['tipo_carro_wrapper']['tipo_carro']['#default_value'] = reset($options);
  $form['tipo_carro_wrapper']['tipo_carro']['#value'] = reset($options);
  $_SESSION['viajesiron_data']['tipo_carro'] = reset($options); //data['tipo_carro]

  $form['info_capacidad_wrapper']['info_capacidad'] = verificar_capacidad_carga_callback($form, $form_state);

  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#tipo_carro_wrapper", render($form['tipo_carro_wrapper'])),
      ajax_command_replace("#info-capacidad-wrapper", render($form['info_capacidad_wrapper']))  
    )
  );

}


function check_valores_tipo_carro($valor_tipo_carga) {

  $rel_carga_carro = llamar_variable_tipo_carga();
  if (isset($rel_carga_carro[$valor_tipo_carga])) {
    $options = $rel_carga_carro[$valor_tipo_carga];
  } else {
    $options = '';
  }

  return $options;
}


function verificar_capacidad_carga_callback () {
  $mensaje = '';
  $form = array();
  $data = bring_data();
  if ($data['tipo_carro'] != '') {
    $datos_capacidad_carga = consultar_capacidad_carga($data['tipo_carro']);
    $peso_viaje_kg = (isset($data['peso_remisiones_kg'])) ? $data['peso_remisiones_kg'] : 0;
    if ($peso_viaje_kg < $datos_capacidad_carga['carga_minima']) {
      $mensaje = 'Advertencia: El peso mínimo de carga para el tipo de carro<strong>@tipo_carro</strong> es<strong>@peso_minimo</strong> Kg. ';
    } elseif ($peso_viaje_kg > $datos_capacidad_carga['carga_maxima']) {
      $mensaje = 'Advertencia: Se ha superado el peso máximo de carga (@peso_maximo Kg) para el tipo de carro <strong>@tipo_carro</strong>, por lo que el sobrepeso se registrara en el sistema como un concepto adicional dentro del viaje. ';
    }
    $mensaje .= 'La carga actualmente conformada es de<strong>@peso_viaje_kg</strong> Kg.';
    $mensaje = t($mensaje, array(
      '@tipo_carro' => $data['tipo_carro'], 
      '@peso_minimo' => $datos_capacidad_carga['carga_minima'],
      '@peso_maximo' => $datos_capacidad_carga['carga_maxima'],
      '@peso_viaje_kg' => $peso_viaje_kg,
    ));
  }
  if ($data['tipo_carga'] == 'Paqueteo') { 
    $mensaje = '';
  }
  $form['info_capacidad'] = array(
    '#markup' => $mensaje,
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );
  add_error('info-capacidad campo:');
  add_error($form['info_capacidad']['#markup']);
  return $form['info_capacidad'];

}

function ejecutar_capacidad_carga_callback ($form, &$form_state) {
  $form['info_capacidad_wrapper']['info_capacidad'] = verificar_capacidad_carga_callback();
  add_error('testing');
  add_error($form['info_capacidad_wrapper']);
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace("#info-capacidad-wrapper", render($form['info_capacidad_wrapper']))  
    )
  );
}

/**
 * Funcion usada para guardar los primeros campos del formulario y no se pierdan al hacer modificaciones
 * @param mixed element elemento que llama esta funcion 
 */
function datos_viaje_formulario_guardar_dato($element) {

  $data = bring_data();
  $data[$element['#name']] = $element['#value'];
  send_data($data);

}

/**
 * Funcion fantasma usada para activar el #element_validate, NO BORRAR
 */
function fantasma() {
  add_error('fantasma');
}

/*
function anexar_remision_callback($form, $form_state) {

  $data = bring_data();  
  if (isset($data['busqueda']) && $data['busqueda'] != array()) {
  } else {
    form_set_error('tabla_remisiones', 'Primero realiza la busqueda de una remision para poder anexarla');
    drupal_exit();
  }

  $no_anexados = array();
  foreach ($data['busqueda'] as $resultado){
    if (is_null($resultado['viaje']) && $resultado['sin_errores']){
      if ($resultado != array()) {
        //Buscar por duplicados
        $repetido = FALSE;
        foreach ($data['remisiones'] as $remision){
        if ($remision['remision'] == $resultado['id']){
          $repetido = TRUE;
        }
        }
        //duplicado
        if ($repetido == TRUE){
          drupal_set_message(t('El WHS @whs que desea anexar ya se encuentra incluido en el viaje actual', array('@whs' => $resultado['id'])),'info');
        } else {
          //no duplicado
          $new_remision = array(
            'remision' => $resultado['id'],
          );
          $data_plus = array(
            'poblacion_expedidor' => $resultado['info']['poblacionExpedidor'],
            'direccion_expedidor' => $resultado['info']['direccionExpedidor'],
            'poblacion_receptor' => $resultado['info']['poblacionReceptor'],
            'direccion_receptor' => $resultado['info']['direccionReceptor'],
            'fecha_entrega' => $resultado['info']['fechaEntregaPlanificadaFormato'],
            'peso' => $resultado['info']['pesoBruto'],
          );
          //guardamos la información actual
          $data['remisiones'][] = array_merge($new_remision, $data_plus);
          drupal_set_message (t('Remisión @whs anexada', array('@whs' => $resultado['id'])), 'status');
        }
      }
    } else{
      $no_anexados[] = $resultado['id'];
    }
  }
  if ($no_anexados != array()){
    drupal_set_message (t('Las remisiones : @remisiones no fueron anexadas porque ya pertenecen a otro viaje o tienen errores.', array('@remisiones' => implode(', ', $no_anexados))), 'info');
  }
  $data['busqueda'] = array();
  
  //subir datos a la tabla
  $options = array();
  foreach ($data['remisiones'] as $remision) {
    if (isset($remision['remision'])) {
      if ($remision['remision'] != '') {
        $options[] = $remision;
      }
    }
  }
  //quitamos la columna id
  unset($options['id']);
  add_error($options);
  //Creamos el render de la tabla
  $form['fs_remisiones']['tabla_remisiones']['#options'] = $options;

  send_data();

  $form['fs_remisiones']['titulo_tabla_remisiones'] = array(
    '#markup' => '<h2>' . t('CAMBIO') . '</h2>',
  );
  add_error($form['fs_remisiones']['titulo_tabla_remisiones']);

  $commands = array();
  $commands[] = ajax_command_replace('#tabla-remisiones', drupal_render($form['fs_remisiones']['tabla_remisiones']));
  $commands[] = ajax_command_replace('#cambio-titulo', drupal_render($form['fs_remisiones']['titulo_tabla_remisiones']));
  return array('#type' => 'ajax', '#commands' => $commands);

}*/