<?php
function formulario_busqueda_viajes_form($form, &$form_state) {
    //Llamar CSS
    drupal_add_css(drupal_get_path('module', 'viajesiron') . '/css/viajesiron.css');


    //Cargar las opciones de busqueda
    $options = array(
        'opcion1' => 'Por Transportadora - Fechas',
        'opcion2' => 'Por Nombre',
    );

    $options = drupal_map_assoc($options);
    //Select de texto - Tipo Carga -> Tipo de contenido Drupal
    $form['opcion_busqueda'] = array(
    '#type' => 'radios',
    '#title' => '<h3>' . t('Selecciona que tipo de busqueda deseas realizar: ') . '</h3>',
    '#options' => $options,
    '#required' => TRUE,
    '#default_values' => isset($form_state['values']['opcion_busqueda']),
    //'#attributes' => array(
    //  'class' => array('vi-conformador-textfield'),
    );
    //'#ajax' => array(
    //  'callback' => 'tipo_carga_ajax_callback',
    //  'wrapper' => 'tipo_carro_wrapper',
    //)

    $form['fs_busqueda_transportadora'] = array(
        '#type' => 'fieldset',
        '#title' => t('Buscar por fechas'),
    );

      //TITULO
    $form['fs_busqueda_transportadora']['titulo_busqueda_transportadora'] = array(
    '#markup' => '<h2>' . t('Seleccione la transportadora y las fechas correspondientes') . '</h2>',
    );

    //SELECT TRANSPORTADORA
    $options = cargar_opciones_transportadora();
    $form['fs_busqueda_transportadora']['transportadora'] = array(
        '#type' => 'select',
        '#title' => '<h3>' . t('Transportadora: ') . '</h3>',
        '#description' => t('datos provenientes del servidor'),
        '#options' => $options,
        '#required' => TRUE,
        '#default_value' => isset($form_state['values']['transportadora']),
        '#attributes' => array(
            'class' => array('vi-conformador-textfield'),
        ),
    );

    //TEXTO FECHA INICIAL
    $form['fs_busqueda_transportadora']['fecha_inicial'] = array(
        '#type' => 'date_popup',
        '#timepicker' => 'timepicker',
        '#title' => '<h3>' . t('Fecha Inicial: ') . '</h3>',
        '#description' => t('rango inferior de la busqueda'),
        '#date_format' => 'Y-m-d',
        '#required' => TRUE,
        '#date_timezone' => date_default_timezone(),
        '#default_value' => isset($form_state['values']['fecha_inicial']),
        '#date_label_position' => 'invisible',
        '#datepicker_options' => array(
            'maxDate' => 0, //hoy
        ),
        '#attributes' => array(
            'class' => array('vi-conformador-textfield'),
        ),
    );

    //TEXTO FECHA FINAL
    $form['fs_busqueda_transportadora']['fecha_inicial'] = array(
        '#type' => 'date_popup',
        '#timepicker' => 'timepicker',
        '#title' => '<h3>' . t('Fecha Inicial: ') . '</h3>',
        '#description' => t('rango inferior de la busqueda'),
        '#date_format' => 'Y-m-d',
        '#required' => TRUE,
        '#date_timezone' => date_default_timezone(),
        '#default_value' => isset($form_state['values']['fecha_inicial']),
        '#date_label_position' => 'invisible',
        '#datepicker_options' => array(
            'maxDate' => 0, //hoy
        ),
        '#attributes' => array(
            'class' => array('vi-conformador-textfield'),
        ),
    );

    $form['actions'] = array(
        '#type' => 'actions',
      );
    $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    );

    return $form;
}