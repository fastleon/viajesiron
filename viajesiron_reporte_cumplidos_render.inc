<?php

module_load_include('php', 'viajesiron', 'infrastructure\models\reporte_cumplidos_model');
module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplidos_entity');
module_load_include('php', 'viajesiron', 'domain\session\data_control');
module_load_include('php', 'viajesiron', 'tools\simulacion_cumplidos');

//TEST
module_load_include('php', 'viajesiron', 'infrastructure\models\transportadora_model');
module_load_include('php', 'viajesiron', 'infrastructure\models\capacidad_carga_model');


/**
 * ARCHIVO QUE GENERA EL RENDER DEL REPORTE DE CUMPLIDOS
 */

function reporte_cumplidos_render_form($form, &$form_state) {
    //Llamar CSS
    drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/viajesiron.css');
    //Configurar como colombia el formato de moneda
    setlocale(LC_MONETARY, 'es_CO');

    //traer información formulario


    //Titulo
    $form['titulo'] = array(
        '#markup' => '<h2>' . t('REPORTE REMISIONES CUMPLIDAS :') . '</h2>', //@nombre.', array('@nombre' => $data_reporte['nombre_viaje'])) . '</h2>',
    );

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

    $data_control = new DataControlReportesCumplidos();
    // $data_control->llamarGuardarDato($datos_modelo);
    unset($data_control);

    //TEST +++++++++++++++++++++++++++++++++++++
    // (new DataControlTransportadoras())->llamarBorrarDato();
    $transportadoras = (new DataControlTransportadoras())->llamarCargarDato(20);
    add_error($transportadoras, 'transportadoras');
    
    // (new DataControlCapacidadCargas())->llamarBorrarDato();
    $capacidad_carga = new CapacidadCargaModel();
    $capacidad_carga = (new DataControlCapacidadCargas())->llamarCargarDato('Sencillo');
    add_error($capacidad_carga, 'capacidad carga');

    //FIN TEST

    //RENDER TABLA RESULTADOS
    //$form = array_merge($form, tabla_resultados());

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
        '#markup' => '<h5>' . t(' resultados :') . '</h5>',
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
