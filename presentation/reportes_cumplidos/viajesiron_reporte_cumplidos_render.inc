<?php
module_load_include('php', 'viajesiron', 'domain/entities/reporte_cumplido_entity');
module_load_include('php', 'viajesiron', 'domain/entities/filtro_busqueda_reportes_cumplidos_entity');
module_load_include('php', 'viajesiron', 'domain/session/data_control');
module_load_include('php', 'viajesiron', 'infrastructure/controllers/reportes_cumplidos_controller');
module_load_include('php', 'viajesiron', 'infrastructure/models/reporte_cumplido_model');
module_load_include('php', 'viajesiron', 'presentation/reportes_cumplidos/dependencies');
module_load_include('php', 'viajesiron', 'tools/simulacion_cumplidos');

module_load_include('inc', 'viajesiron', 'presentation/reportes_cumplidos/viajesiron_reporte_cumplidos_submit');
/**
 * ARCHIVO QUE GENERA EL RENDER DEL REPORTE DE CUMPLIDOS
 */

function reporte_cumplidos_render_form($form, &$form_state) {
    //Llamar CSS
    drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/vi_reportes_cumplidos.css');
    //Configurar como colombia el formato de moneda
    setlocale(LC_MONETARY, 'es_CO');

    //Llamar opciones de los campos
    $campos = new Campos_formulario();
    //cargar estado del formulario
    $last_values = (new DataControlReportesCumplidosFormulario())->llamarCargarDato();
    $campos->setDefaultValues($last_values);

    $form = array_merge($form, $campos->crear_textfield('remision'));

	// "fechaCreacionInicial":"18/02/2024",// "fechaCreacionFinal":"",
    $form = array_merge($form, $campos->crear_date_pickers('fecha_creacion'));

    // "fechaPlaneacionInicial":"",// "fechaPlaneacionFinal":"",
    $form = array_merge($form, $campos->crear_date_pickers('fecha_planeacion'));
    
    // "fechaEstimadaInicial":"",// "fechaEstimadaFinal":"",
    $form = array_merge($form, $campos->crear_date_pickers('fecha_estimada'));

	// "fechaEntregaInicial":"",// "fechaEntregaFinal":"",
    $form = array_merge($form, $campos->crear_date_pickers('fecha_entrega'));

	// "ciudadOrigen":"",
    $form = array_merge($form, $campos->crear_textfield('ciudad_origen'));

	// "ciudadDestino":"",
    $form = array_merge($form, $campos->crear_textfield('ciudad_destino'));

	// "tipoDeCarga":"Paqueteo",
    $form = array_merge($form, $campos->crear_select('tipo_carga'));

	// "transportadora":""
    $form = array_merge($form, $campos->crear_select('transportadora'));

    //Boton BUSCAR
    $form = array_merge($form, crear_boton_buscar());
    
    //Boton LIMPIAR
    $form = array_merge($form, crear_boton_limpiar());


    //RENDER TABLA RESULTADOS
    $form = array_merge($form, tabla_resultados());

    //Boton EXCEL
    $form = array_merge($form, crear_boton_excel());

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
        '#limit_validation_errors' => array(),
    );
    return $form;
}

/**
 * RENDER PARA BOTON DE EXCEL
 */
function crear_boton_excel() {
    $form['submit_Excel'] = array(
        '#name' => 'Excel', 
        '#type' => 'submit',
        '#value' => t('Exportar Excel'),
    );
    return $form;
}

/**
 * RENDER para TABLA RESULTADOS
 */
function tabla_resultados() {

    $reportes_encontrados = (new DataControlReportesCumplidosResultados())->llamarCargarDato();

    //Titulo
    $form['tbl']['titulo_tabla'] = array(
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
    if ( (isset($reportes_encontrados)) && ($reportes_encontrados != array()) ) {
        foreach ($reportes_encontrados as $dato) {
            $options[] = $dato;
        }
    }

    //Creamos el render de la tabla
    $form['tbl']['tabla_resultados'] = array(
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
