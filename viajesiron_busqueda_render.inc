<?php
function formulario_busqueda_viajes_form($form, &$form_state) {
    //Llamar CSS
    drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/viajesiron_busqueda.css');

    //cargar estado anterior
    if (isset($_SESSION['viajesiron_busqueda']['state'])){
        $last_state = $_SESSION['viajesiron_busqueda']['state'];
    }


    //Cargar las opciones de busqueda
    $options = array(
        'opcion1' => 'Por Transportadora - Fechas',
        'opcion2' => 'Por Nombre',
    );  //Si se modifican estas Opciones se debe edit el submit y validate (OJO)

    $options = drupal_map_assoc($options);
    //Select de texto - Tipo Carga -> Tipo de contenido Drupal
    $form['opcion_busqueda'] = array(
    '#type' => 'radios',
    '#title' => '<h3>' . t('Selecciona que tipo de busqueda deseas realizar: ') . '</h3>',
    '#options' => $options,
    '#required' => TRUE,
    '#ajax' => array(
        'callback' => 'cambiar_busqueda_ajax_callback',
        'wrapper' => 'datos_busqueda_wrapper',
        ),
    );
    if (isset($_SESSION['viajesiron_busqueda']['state'])){
        if ($_SESSION['viajesiron_busqueda']['state']['opcion_busqueda'] == 'Nombre'){
            $form['opcion_busqueda']['#default_value'] = 'Por Nombre';
        } else {
            $form['opcion_busqueda']['#default_value'] = 'Por Transportadora - Fechas';
        }
    }
    
    //Wrapper para el ajax
    $form['datos_busqueda_wrapper'] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="datos_busqueda_wrapper">',
        '#suffix' => '</div>'
    );
    
    /*****************FIELDSET BUSQUEDA POR FECHAS ********************* */
    $form['datos_busqueda_wrapper']['fs_busqueda_fechas'] = array(
        '#type' => 'fieldset',
        '#title' => t('Buscar por fechas'),
        '#access' => TRUE,
    );
    
    //TITULO
    $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['titulo_busqueda_transportadora'] = array(
        '#markup' => '<h2>' . t('Seleccione la transportadora y las fechas correspondientes') . '</h2>',
    );
    
    //SELECT TRANSPORTADORA
    $options = cargar_opciones_transportadora();
    $options[0] = 'TODAS LAS TRANSPORTADORAS';

    $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['transportadora'] = array(
        '#type' => 'select',
        '#title' => '<h3>' . t('Transportadora: ') . '</h3>',
        '#description' => t('datos provenientes del servidor'),
        '#options' => $options,
        '#attributes' => array(
            '#required' => FALSE,
            'class' => array('vi-conformador-textfield'),
        ),
    );
    $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['transportadora']['#default_value'] = 1;
    if (isset($_SESSION['viajesiron_busqueda']['state']['transportadora'])){
        $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['transportadora']['#default_value'] = $_SESSION['viajesiron_busqueda']['state']['transportadora'];
    }

    //TEXTO FECHA INICIAL
    $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['fecha_inicial'] = array(
        '#type' => 'date_popup',
        '#timepicker' => 'timepicker',
        '#title' => '<h3>' . t('Fecha Inicial: ') . '</h3>',
        //'#description' => t('rango inferior de la busqueda'),
        '#date_format' => 'Y-m-d',
        '#required' => FALSE,
        '#date_timezone' => date_default_timezone(),
        '#date_label_position' => 'invisible',
        '#datepicker_options' => array(
            'maxDate' =>  0, //hoy
        ),
        '#attributes' => array(
            'class' => array('vi-busqueda-timepicker'),
        ),
    );
    if (isset($_SESSION['viajesiron_busqueda']['state']['fecha_inicial'])){
        $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['fecha_inicial']['#default_value'] = $_SESSION['viajesiron_busqueda']['state']['fecha_inicial'];
    }
    
    //TEXTO FECHA FINAL
    $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['fecha_final'] = array(
        '#type' => 'date_popup',
        '#timepicker' => 'timepicker',
        '#title' => '<h3>' . t('Fecha Final: ') . '</h3>',
        //'#description' => t('rango superior de la busqueda'),
        '#date_format' => 'Y-m-d',
        '#required' => FALSE,
        '#date_timezone' => date_default_timezone(),
        '#date_label_position' => 'invisible',
        '#datepicker_options' => array(
            //'minDate' => isset($form['fecha_inicial']) ? $form_state['values']['fecha_inicial'] : 0, //hoy
            'maxDate' => 0,
        ),
        '#attributes' => array(
            'class' => array('vi-busqueda-timepicker'),
        ),
    );
    if (isset($_SESSION['viajesiron_busqueda']['state']['fecha_final'])){
        $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['fecha_final']['#default_value'] = $_SESSION['viajesiron_busqueda']['state']['fecha_final'];
    }
    
    /*****************FIELDSET BUSQUEDA POR NOMBRE ********************* */
    $form['datos_busqueda_wrapper']['fs_busqueda_nombre'] = array(
        '#type' => 'fieldset',
        '#title' => t('Buscar por nombre'),
        '#access' => TRUE,
    );
    $form['datos_busqueda_wrapper']['fs_busqueda_nombre']['nombre'] = array(
        '#type' => 'textfield',
        '#title' => '<h3>' . t('Nombre: ') . '</h3>',
        '#size' => 20,
        '#attributes' => array(
            '#required' => FALSE,
            'class' => array('vi-conformador-textfield'),
        ),
    );
    if (isset($_SESSION['viajesiron_busqueda']['state']['nombre'])){
        $form['datos_busqueda_wrapper']['fs_busqueda_nombre']['nombre']['#default_value'] = $_SESSION['viajesiron_busqueda']['state']['nombre'];
    }

    /**********************BOTONES BUSCAR Y LIMPIAR **************** */
    $form['submit_buscar'] = array(
        '#name' => 'Buscar',
        '#type' => 'submit',
        '#value' => t('Buscar'),
    );
    
    $form['submit_limpiar'] = array(
        '#name' => 'Limpiar', 
        '#type' => 'submit',
        '#value' => t('Limpiar'),
    );


    /*************************RESULTADOS DE LA BUSQUEDA ****************/
    if (isset($_SESSION['viajesiron_busqueda']['data']) && ($_SESSION['viajesiron_busqueda']['data'])){
        $form = mostrar_resultados_busqueda($form, $form_state);
    } else{
        //Sin busqueda realizada
    }

    $form['datos_busqueda_wrapper'] = cambiar_busqueda_ajax_callback ($form, $form_state);
    return $form;

}

function mostrar_resultados_busqueda($form, &$form_state) {
    
    $response = $_SESSION['viajesiron_busqueda']['data'];

    $resultado_anidado = array();
    foreach ($response['reporteViajesDTO'] as $viaje) {
        $nombre_viaje = $viaje['nombreViaje'];
        if (!isset($resultado_anidado[$nombre_viaje])) {
            //Crear Datos unicos
            $resultado_anidado[$nombre_viaje]['nombreViaje'] = $viaje['nombreViaje'];
            $resultado_anidado[$nombre_viaje]['tipoDeCarga'] = $viaje['tipoDeCarga'];
            $resultado_anidado[$nombre_viaje]['tipoDeCarro'] = $viaje['tipoDeCarro'];
            $resultado_anidado[$nombre_viaje]['transportadora'] = $viaje['transportadora'];
            $resultado_anidado[$nombre_viaje]['pesoTotalViaje'] = round($viaje['pesoTotalViaje'], 2) . ' KG';
            $resultado_anidado[$nombre_viaje]['costoTotalViaje'] = money_format20($viaje['costoTotalViaje']);
        }

        //datos multiples;
        $temp_data = array();
        $temp_data['numeroRemision'] = empty($viaje['numeroRemision']) ? '' : $viaje['numeroRemision'];
        $temp_data['departamentoOrigen'] = empty($viaje['departamentoOrigen']) ? '' : $viaje['departamentoOrigen'];
        $temp_data['ciudadOrigen'] = empty($viaje['ciudadOrigen']) ? '' : $viaje['ciudadOrigen'];
        $temp_data['departamentoEntrega'] = empty($viaje['departamentoEntrega']) ? '' : $viaje['departamentoEntrega'];
        $temp_data['areaEntrega'] = empty($viaje['areaEntrega']) ? '' : $viaje['areaEntrega'];
        $temp_data['empresaReceptora'] = empty($viaje['empresaReceptora']) ? '' : $viaje['empresaReceptora'];
        $temp_data['direccionEmpresaReceptora'] = empty($viaje['direccionEmpresaReceptora']) ? '' : $viaje['direccionEmpresaReceptora'];
        $temp_data['fechaEntregaCalculada'] = empty($viaje['fechaEntregaCalculada']) ? '' : $viaje['fechaEntregaCalculada'];
        $temp_data['pesoBruto'] = empty(round($viaje['pesoBruto'], 2) . ' KG') ? '' : round($viaje['pesoBruto'], 2) . ' KG';
        $temp_data['costoTotalRemision'] = empty(money_format20($viaje['costoTotalRemision'])) ? '' : money_format20($viaje['costoTotalRemision']);
        $temp_data['zonaVentas'] = empty($viaje['zonaVentas']) ? '' : $viaje['zonaVentas'];
        $temp_data['numeroPoblacion'] = empty($viaje['numeroPoblacion']) ? '' : $viaje['numeroPoblacion'];
        $resultado_anidado[$nombre_viaje]['tabla'][] = $temp_data; 

    }
    /**************************TABLA POR RESULTADO************* */
    foreach($resultado_anidado as $viaje){
        $form = crear_estructura_resultados($viaje, $form);
    }
    /**************************CONSOLIDADOS******************** */
    $form['consolidado_container'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('vi-consolidado-container'),
          ),
        );
    $peso_total_con = $_SESSION['viajesiron_busqueda']['data']['pesoTotalReporte'];
    $peso_total_con = round($peso_total_con, 2) . ' KG';
    $costo_total_con = $_SESSION['viajesiron_busqueda']['data']['costoTotalReporte'];
    $costo_total_con = money_format20($costo_total_con);
    $form['consolidado_container']['peso_total'] = crear_textfield_busqueda('Peso Total : ', $peso_total_con, array('vi-textfields-consolidado'));
    $form['consolidado_container']['costo_total'] = crear_textfield_busqueda('Costo Total : ', $costo_total_con, array('vi-textfields-consolidado'));


    /**************************BOTON EXCEL********************* */
    $form['exportar']['submit'] = array(
        '#name' => 'Excel',
        '#type' => 'submit',
        '#value' => t('Exportar Excel'),
    );
        
    return $form;
}


function crear_estructura_resultados($viaje, $form) {
    if ($viaje == array()){
        return array();
    }
    $nombre_viaje = $viaje['nombreViaje'];

    // Fieldset para organizar los elementos relacionados
    $form[$nombre_viaje] = array(
        '#type' => 'fieldset',
        '#title' => '', //t($nombre_viaje),
        '#attributes' => array(
            'class' => array('vi-fieldset-resultado'),
        ),
    );

    //TITULO
    $form[$nombre_viaje]['titulo_nombre_viaje'] = crear_textfield_busqueda('Viaje:', $nombre_viaje, array('vi-texfields-busqueda_items',));
    $form[$nombre_viaje]['titulo_tipo_carga'] = crear_textfield_busqueda('Tipo de Carga:', $viaje['tipoDeCarga'], array('vi-texfields-busqueda_items',));
    $form[$nombre_viaje]['titulo_tipo_carro'] = crear_textfield_busqueda('Tipo de Carro:', $viaje['tipoDeCarro'], array('vi-texfields-busqueda_items',));
    $form[$nombre_viaje]['titulo_transportadora'] = crear_textfield_busqueda('Transportadora:', $viaje['transportadora'], array('vi-texfields-busqueda_items',));
    $form[$nombre_viaje]['titulo_peso_total'] = crear_textfield_busqueda('Peso Total:', $viaje['pesoTotalViaje'], array('vi-texfields-busqueda_items', 'vi-texfields-textright',));
    $form[$nombre_viaje]['titulo_costo_total'] = crear_textfield_busqueda('Costo Total:', $viaje['costoTotalViaje'], array('vi-texfields-busqueda_items', 'vi-texfields-textright',));
    $form[$nombre_viaje]['titulo_espacio'] = array('#markup' => '<br>',);

    //HEADER
    $header = array(
        'numeroRemision' => t('Remisión'),
        'departamentoOrigen'=> t('Departamento Origen'),
        'ciudadOrigen'=> t('Ciudad Origen'),
        'departamentoEntrega'=> t('Departamento Entrega'),
        'areaEntrega'=> t('Area Entrega'),
        'empresaReceptora'=> t('Nombre'),
        'direccionEmpresaReceptora'=> t('Dirección'),
        'zonaVentas' => t('Zona de Ventas'),
        'numeroPoblacion' => t('Numero de Población'),
        'fechaEntregaCalculada'=> t('Fecha calculada de Entrega'),
        'pesoBruto'=> t('Peso Bruto'),
        'costoTotalRemision'=> t('Costo Flete'),
    );

    $options = array();
    foreach ($viaje['tabla'] as $fila){
        $options[] = array(
            'numeroRemision' => empty($fila['numeroRemision']) ? '' : $fila['numeroRemision'],
            'departamentoOrigen' => empty($fila['departamentoOrigen']) ? '' : $fila['departamentoOrigen'],
            'ciudadOrigen' => empty($fila['ciudadOrigen']) ? '' : $fila['ciudadOrigen'],
            'departamentoEntrega' => empty($fila['departamentoEntrega']) ? '' : $fila['departamentoEntrega'],
            'areaEntrega' => empty($fila['areaEntrega']) ? '' : $fila['areaEntrega'],
            'empresaReceptora' => empty($fila['empresaReceptora']) ? '' : $fila['empresaReceptora'],
            'direccionEmpresaReceptora' => empty($fila['direccionEmpresaReceptora']) ? '' : $fila['direccionEmpresaReceptora'],
            'zonaVentas' => empty($fila['zonaVentas']) ? '' : $fila['zonaVentas'],
            'numeroPoblacion' => empty($fila['numeroPoblacion']) ? '' : $fila['numeroPoblacion'],
            'fechaEntregaCalculada' => empty($fila['fechaEntregaCalculada']) ? '' : $fila['fechaEntregaCalculada'],
            'pesoBruto' => empty($fila['pesoBruto']) ? '' : $fila['pesoBruto'],
            'costoTotalRemision' => empty($fila['costoTotalRemision']) ? '' : $fila['costoTotalRemision'],
        ); 
    }

    //Creamos el render de la tabla
    $form[$nombre_viaje]['tabla_resultado'] = array(
        '#name' => 'tabla_resultado_' . $nombre_viaje,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $options,
        );

    return $form;

}


/**
 * Modificar que formulario debe ser visible segun la selección
 */
function cambiar_busqueda_ajax_callback ($form, $form_state) {
    $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['#access'] = FALSE; 
    $form['datos_busqueda_wrapper']['fs_busqueda_nombre']['#access'] = FALSE;
    $opcion = FALSE;
    if (isset($form_state['values']['opcion_busqueda'])){
        $opcion = $form_state['values']['opcion_busqueda'];
    } elseif (isset($_SESSION['viajesiron_busqueda']['state']['opcion_busqueda'])){
        $opcion = $_SESSION['viajesiron_busqueda']['state']['opcion_busqueda'];
    }
    if ($opcion){
        if (strstr($opcion, 'Fecha') !== FALSE){ //por fechas
            $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['#access'] = TRUE;
            $form['datos_busqueda_wrapper']['fs_busqueda_nombre']['#access'] = FALSE;

        } elseif(strstr($opcion, 'Nombre') !== FALSE){//por nombre
            $form['datos_busqueda_wrapper']['fs_busqueda_fechas']['#access'] = FALSE; 
            $form['datos_busqueda_wrapper']['fs_busqueda_nombre']['#access'] = TRUE; 
        }
    }
    return $form['datos_busqueda_wrapper'];
  }

// function viajesiron_form_formulario_busqueda_viajes_form_alter($form, &$form_state){
//     //hook_formid_form_alter NO USADO 
//     //drupal_rebuild_form($form_id, $form_state, $form);
// }

function crear_textfield_busqueda($label, $valor, $css_class){
    $texfield = array();
    $texfield = array(
        '#type' => 'textfield',
        '#title' => '<h3>' . t($label) . '</h3>',
        '#default_value' => $valor,
        '#required' => FALSE,
        '#disabled' => TRUE,
        '#attributes' => array(
          'class' => $css_class,
        ),
    );
    return $texfield;
}

