<<?php

module_load_include('php', 'viajesiron', 'infrastructure/models/transportadora_model');
module_load_include('php', 'viajesiron', 'infrastructure/models/capacidad_carga_model');
module_load_include('php', 'viajesiron', 'domain/constantes');

class Campos_formulario {
    protected $textfields;
    protected $datepickers;
    protected $selects;
    protected $default_values;

    public function __construct() {
        $this->textfields = array(
            'remision' => array(
                'nombre_campo' =>'opcion_remision',
                'fieldset_text' => 'REMISION',
                'label' => 'Remision :',
            ),
            'ciudad_origen' => array(
                'nombre_campo' =>'opcion_ciudad_origen',
                'fieldset_text' => 'CIUDAD DE ORIGEN',
                'label' => 'Ciudad de Origen :',
            ),
            'ciudad_destino' => array(
                'nombre_campo' =>'opcion_ciudad_destino',
                'fieldset_text' => 'CIUDAD DE DESTINO',
                'label' => 'Ciudad de Destino :',
            ),
            'tipo_carga' => array(
                'nombre_campo' =>'opcion_tipo_carga',
                'fieldset_text' => 'TIPO DE CARGA',
                'label' => 'Tipo de Carga :',
            ),
            'transportadora' => array(
                'nombre_campo' =>'opcion_transportadora',
                'fieldset_text' => 'TRANSPORTADORA',
                'label' => 'Transportadora :',
            ),
        );
    
        $this->datepickers = array(
            'fecha_creacion' => array(
                'nombre_campo' => 'opcion_fecha_creacion',
                'fieldset_text' => 'FECHA DE CREACION',
            ),
            'fecha_planeacion' => array(
                'nombre_campo' => 'opcion_fecha_planeacion',
                'fieldset_text' => 'FECHA DE PLANEACIÓN',
            ),
            'fecha_estimada' => array(
                'nombre_campo' => 'opcion_fecha_estimada',
                'fieldset_text' => 'FECHA ESTIMADA',
            ),
            'fecha_entrega' => array(
                'nombre_campo' => 'opcion_fecha_entrega',
                'fieldset_text' => 'FECHA DE ENTREGA',
            ),
        );
        
        $this->selects = array(
            'tipo_carga' => array(
                'nombre_campo' => 'opcion_tipo_carga',
                'fieldset_text' => 'TIPO DE CARGA',
                'options' => self::buscarTiposCarga(),
                'title' => '<h3>' . t('Tipo de carga:') . '</h3>',
            ),
            'transportadora' => array(
                'nombre_campo' =>'opcion_transportadora',
                'fieldset_text' => 'TRANSPORTADORA',
                'options' => self::buscarTiposTransportadoras(),
                'title' => '<h3>' . t('Transportadora :') . '</h3>',
            ),
        );

        $this->default_values = array(
            // 'tipo_carga' => 'TODOS LOS TIPOS', //ligado a funcion buscarTiposCarga
            // 'transportadora' => 'TODAS LAS TRANSPORTADORAS', //ligado a funcion buscarTiposTransportadoras
        );
    }

    public function getTextfields() {
        return $this->textfields;
    }
    public function getDatepickers() {
        return $this->datepickers;
    }
    public function setDefaultValues($default_values) {
        $this->default_values = $default_values;
    } 

    private static function buscarTiposCarga() {
        $options_tipo_carga = Constantes::$tipo_de_cargas;
        $options_tipo_carga = array_merge(array('TODOS LOS TIPOS'), $options_tipo_carga);//ligado al construcotr
        $options_tipo_carga = drupal_map_assoc($options_tipo_carga);
        return $options_tipo_carga;
    }
    private static function buscarTiposTransportadoras() {
        $options_transportadora = (new DataControlTransportadoras())->llamarCargarDato();
        $valor = array(0=>'TODAS LAS TRANSPORTADORAS');
        $options_transportadora = $valor + $options_transportadora;
        return $options_transportadora;
    }

    public function crear_date_pickers($campo) {
        $nombre_campo = $this->datepickers[$campo]['nombre_campo'];
        $default_inicial = empty($this->default_values[$campo . '_inicial']) ? '' : $this->default_values[$campo . '_inicial'];
        $default_final = empty($this->default_values[$campo . '_final']) ? '' : $this->default_values[$campo . '_final'];
        $fieldset = $nombre_campo . '_fieldset';
        $fieldset_text = $this->datepickers[$campo]['fieldset_text'];
        $contenedor = $nombre_campo . '_wrapper'; 
        $fecha_inicial = $nombre_campo . '_inicial';
        $fecha_final = $nombre_campo . '_final';
        $form[$fieldset] = array(
            '#type' => 'fieldset',
            '#title' => t($fieldset_text),
            '#collapsible' => true,
            '#collapsed' => ( empty($default_inicial) && empty($default_final) ) ? true : false,
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
                'class' => array('vi-rc-timepicker'),
            ),
            '#default_value' => Utils::dateToYMD($default_inicial),
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
                'class' => array('vi-rc-timepicker'),
            ),
            '#default_value' => Utils::dateToYMD($default_final),
        );
        //TODO: llamar dato anterior, si hubo uno busqueda
    
        return $form;
    }
    
    public function crear_textfield($campo) {
        $nombre_campo = $this->textfields[$campo]['nombre_campo'];
        $default = empty($this->default_values[$campo]) ? '' : $this->default_values[$campo];
        
        $fieldset = $nombre_campo . '_fieldset';
        $fieldset_text = $this->textfields[$campo]['fieldset_text'];
        $label = $this->textfields[$campo]['label'];
        $contenedor = $nombre_campo . '_wrapper'; 
        $form[$fieldset] = array(
            '#type' => 'fieldset',
            '#title' => t($fieldset_text),
            '#collapsible' => TRUE,
            '#collapsed' => empty($default) ? TRUE : FALSE,
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
                'class' => array('vi-rc-textfield'),
            ),
            '#default_value' => $default,
        );
        //TODO: llamar dato anterior, si hubo uno busqueda
        return $form;
    }

    public function crear_select($campo) {

        $nombre_campo = $this->selects[$campo]['nombre_campo'];
        $default = empty($this->default_values[$campo]) ? '' : $this->default_values[$campo];
        $title = $this->selects[$campo]['title'];
        $options = $this->selects[$campo]['options'];
        $fieldset = $nombre_campo . '_fieldset';
        $fieldset_text = $this->textfields[$campo]['fieldset_text'];
        $contenedor = $nombre_campo . '_wrapper'; 

        $form[$fieldset] = array(
            '#type' => 'fieldset',
            '#title' => t($fieldset_text),
            '#collapsible' => TRUE,
            '#collapsed' => empty($default) ? TRUE : FALSE,
        );
        $form[$fieldset][$contenedor] = array(
            '#type' => 'markup',
            '#prefix' => '<div id="' . $contenedor . '">',
            '#suffix' => '</div>'
        );
        $form[$fieldset][$contenedor][$nombre_campo] = array(
            '#type' => 'select',
            '#title' => $title,
            //'#description' => t('Opciones predeterminadas'),
            '#options' => $options,
            '#required' => false,
            '#attributes' => array(
                'class' => array('vi-rc-textfield'),
            ),
            '#default_value' => $default,
        );
        return $form;

    }

}