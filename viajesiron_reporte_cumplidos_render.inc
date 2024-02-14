<?php

require_once (drupal_get_path('module', 'viajesiron') . '/models/viajesiron_reporte_cumplidos_model.inc');

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


    //RENDER TABLA RESULTADOS
    $form .= tabla_resultados();

    return $form;
}

/**
 * RENDER para TABLA RESULTADOS
 */
function tabla_resultados() {

    $data_control = new DataControlReportesCumplidos();
    $reporte_cumplidos = $data_control->cargarSesion();

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
    if ( $reporte_cumplidos[0]->isEmpty() ) {
        /* $temp_data = array(
            'remision' => l($remision['remision'], $URL_REMISION_DETAIL, $link_options),
            'poblacion_expedidor' => $remision['poblacion_expedidor'],
            'direccion_expedidor' => $remision['direccion_expedidor'],
            'poblacion_receptor' => $remision['poblacion_receptor'],
            'direccion_receptor' => $remision['direccion_receptor'],
            'fecha_entrega' => $remision['fecha_entrega'],
            'costo' => money_format20($remision['costo']),
            'peso' => round($remision['peso'], 2),
            'unidad_peso' => $remision['unidad_peso'],
        );
        $options[] = $temp_data;*/
        $options = array();
    } else {
        $options = get_object_vars($reporte_cumplidos);
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
}
