<?php

module_load_include('php', 'viajesiron', 'presentation\reportes_cumplidos\dependencies');

/**
 * ARCHIVO QUE GENERA EL RENDER DEL REPORTE DE CUMPLIDOS
 */

function reporte_cumplidos_render_form($form, &$form_state) {
    //Llamar CSS
    drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/vi_reportes_cumplidos.css');
    //Configurar como colombia el formato de moneda
    setlocale(LC_MONETARY, 'es_CO');

    //traer información formulario

    //RENDER FORMULARIO BUSQUEDA


    //Llamado de data simulacion
    $json = mockup_cumplidos();
    $datos = json_decode($json);
    $datos_modelo = array();
    foreach ($datos as $dato) {
        $temp_cumplido_model = new ReporteCumplidosModel();
        $temp_cumplido_model->fromEntity($dato);
        $datos_modelo[] = $temp_cumplido_model;
    }

    //Guardar datos modelo en sesion
    (new DataControlReportesCumplidos())->llamarGuardarDato($datos_modelo);

    //TEST +++++++++++++++++++++++++++++++++++++
    // (new DataControlTransportadoras())->llamarBorrarDato();
    // $transportadoras = (new DataControlTransportadoras())->llamarCargarDato(20);
    // add_error($transportadoras, 'transportadoras');
    
    // (new DataControlCapacidadCargas())->llamarBorrarDato();
    // $capacidad_carga = new CapacidadCargaModel();
    // $capacidad_carga = (new DataControlCapacidadCargas())->llamarCargarDato('Sencillo');
    // add_error($capacidad_carga, 'capacidad carga');

    //FIN TEST

    //Llamar opciones de los campos
    $campos = new Campos_formulario();

    // "remision":"",
    $form = array_merge($form, opcion_remision($campos->getTextfields()['remision']));

	// "fechaCreacionInicial":"18/02/2024",// "fechaCreacionFinal":"",
	$form = array_merge($form, opcion_fecha_creacion($campos->getDatepickers()['fecha_creacion']));
    
    // "fechaPlaneacionInicial":"",// "fechaPlaneacionFinal":"",
    $form = array_merge($form, opcion_fecha_planeacion($campos->getDatepickers()['fecha_planeacion']));
    
    // "fechaEstimadaInicial":"",// "fechaEstimadaFinal":"",
    $form = array_merge($form, opcion_fecha_estimada($campos->getDatepickers()['fecha_estimada']));

	// "fechaEntregaInicial":"",// "fechaEntregaFinal":"",
    $form = array_merge($form, opcion_fecha_entrega($campos->getDatepickers()['fecha_entrega']));

	// "ciudadOrigen":"",
    $form = array_merge($form, opcion_ciudad_origen($campos->getTextfields()['ciudad_origen']));

	// "ciudadDestino":"",
    $form = array_merge($form, opcion_ciudad_destino($campos->getTextfields()['ciudad_destino']));

	// "tipoDeCarga":"Paqueteo",
    $form = array_merge($form, opcion_tipo_carga($campos->getTextfields()['tipo_carga']));

	// "transportadora":""
    $form = array_merge($form, opcion_transportadora($campos->getTextfields()['transportadora']));

    //Boton BUSCAR
    $form = array_merge($form, crear_boton_buscar());
    
    //Boton LIMPIAR
    $form = array_merge($form, crear_boton_limpiar());


    //RENDER TABLA RESULTADOS
    $form = array_merge($form, tabla_resultados());

    return $form;
}


/**
 * RENDER PARA EL CAMPO DE BUSQUEDA remision
 */
function opcion_remision($textfield) {

    $form = crear_texfield($textfield);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA fecha_creacion
 */
function opcion_fecha_creacion($datepicker) {

    $form = crear_date_pickers($datepicker);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA fecha_planeacion
 */
function opcion_fecha_planeacion($datepicker) {

    $form = crear_date_pickers($datepicker);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA fecha_estimada
 */
function opcion_fecha_estimada($datepicker) {

    $form = crear_date_pickers($datepicker);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA fecha_entrega
 */
function opcion_fecha_entrega($datepicker) {
    
    $form = crear_date_pickers($datepicker);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA ciudad_origen
 */
function opcion_ciudad_origen($textfield) {
    
    $form = crear_texfield($textfield);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA ciudad_destino
 */
function opcion_ciudad_destino($textfield) {

    $form = crear_texfield($textfield);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA tipo_carga
 */
function opcion_tipo_carga($textfield) {
 
    $form = crear_texfield($textfield);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA EL CAMPO DE BUSQUEDA transportadora
 */
function opcion_transportadora($textfield) {
 
    $form = crear_texfield($textfield);
    //TODO: Llamar datos de busquedas anteriores
    return $form;

}

/**
 * RENDER PARA BOTON DE BUSCAR
 */
function crear_boton_buscar() {
    $form['submit_buscar'] = array(
        '#name' => 'Buscar',
        '#type' => 'submit',
        '#value' => t('Buscar'),
    );
    return $form;
}

/**
 * RENDER PARA BOTON DE LIMPIAR
 */
function crear_boton_limpiar() {
    $form['submit_limpiar'] = array(
        '#name' => 'Limpiar', 
        '#type' => 'submit',
        '#value' => t('Limpiar'),
    );
    return $form;
}


/**
 * RENDER para TABLA RESULTADOS
 */
function tabla_resultados() {

    $reporte_cumplidos = (new DataControlReportesCumplidos())->llamarCargarDato();

    // DataControlReportesCumplidos::cargarSesion();

    //Titulo
    $form['fs_']['titulo_tabla_'] = array(
        '#markup' => '<h5>' . t(' RESULTADOS DE LA BUSQUEDA :') . '</h5>',
    );

    //header
    $header = array(
        'whs_pedido' => t('WHS pedidos'),
        'creacion_remision' => t('Creación de Remisión'),
        'planeacion_informe' => t('Planeacion Informe'),
        'fecha_estimada_entrega' => t('Fecha Est de Entrega'),
        'localidad_origen' => t('Localidad Origen'),
        'prueba_entrega' => t('Prueba de Entrega'),
        'tipo_carga' => t('Tipo de Carga'),
        'dias_entrega' => t('Días Entrega'),
        'ciudad' => t('Ciudad'),
        'transportadora' => t('Linea Transportadora'),
        'fotos' => t('Fotos POD/PDF'),
    );

    //datos
    $options = array();
    if ( (isset($reporte_cumplidos)) && ($reporte_cumplidos != array()) ) {
        foreach ($reporte_cumplidos as $dato) {
            $options[] = $dato->toArray();
        }
    }

    //Creamos el render de la tabla
    $form['fs_tabla_resultados']['tabla_resultados'] = array(
        '#name' => 'tabla_',
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $options,
        '#attributes' => array(
            //'class' => array('vi-reporte-tabla-'),
        ),
    );

    return $form;
}

/**OPCIONES FUERA DEL RENDER */
function crear_date_pickers($options) {
    $nombre_campo = $options['nombre_campo'];
    $fieldset = $nombre_campo . '_fieldset';
    $fieldset_text = $options['fieldset_text'];
    $contenedor = $nombre_campo . '_wrapper'; 
    $fecha_inicial = $nombre_campo . '_inicial';
    $fecha_final = $nombre_campo . '_final';
    $form[$fieldset] = array(
        '#type' => 'fieldset',
        '#title' => t($fieldset_text),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$fieldset][$contenedor] = array(
        '#type' => 'markup',
        '#prefix' => '<div class="date-picker-container", id="' . $contenedor .'">',
        '#suffix' => '</div>'
    );
    //FECHA INICIAL
    $form[$fieldset][$contenedor][$fecha_inicial] = array(
        '#type' => 'date_popup',
        '#timepicker' => 'timepicker',
        '#title' => '<h3>' . t('Fecha Inicial Creación: ') . '</h3>',
        //'#description' => t('rango inferior de la busqueda'),
        '#date_format' => 'd-m-Y',
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
    //TODO: llamar dato anterior, si hubo uno busqueda

    //FECHA FINAL
    $form[$fieldset][$contenedor][$fecha_final] = array(
        '#type' => 'date_popup',
        '#timepicker' => 'timepicker',
        '#title' => '<h3>' . t('Fecha Final Creación: ') . '</h3>',
        //'#description' => t('rango superior de la busqueda'),
        '#date_format' => 'd-m-Y',
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
    //TODO: llamar dato anterior, si hubo uno busqueda

    return $form;
}

function crear_texfield($options) {
    $nombre_campo = $options['nombre_campo'];
    $fieldset = $nombre_campo . '_fieldset';
    $fieldset_text = $options['fieldset_text'];
    $label = $options['label'];
    $contenedor = $nombre_campo . '_wrapper'; 
    $form[$fieldset] = array(
        '#type' => 'fieldset',
        '#title' => t($fieldset_text),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form[$fieldset][$contenedor] = array(
        '#type' => 'markup',
        '#prefix' => '<div id="' . $contenedor . '">',
        '#suffix' => '</div>'
    );
    $form[$fieldset][$contenedor][$nombre_campo] = array(
        '#type' => 'textfield',
        '#title' => '<h3>' . t($label) . '</h3>',
        '#size' => 20,
        '#attributes' => array(
            '#required' => FALSE,
            //'class' => array('vi-opcion-remision-textfield'),
        ),
    );
    //TODO: llamar dato anterior, si hubo uno busqueda
    return $form;
}