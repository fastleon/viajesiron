<<?php

module_load_include('php', 'viajesiron', 'infrastructure\models\reporte_cumplidos_model');
module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplidos_entity');
module_load_include('php', 'viajesiron', 'domain\session\data_control');
module_load_include('php', 'viajesiron', 'tools\simulacion_cumplidos');

//TEST
module_load_include('php', 'viajesiron', 'infrastructure\models\transportadora_model');
module_load_include('php', 'viajesiron', 'infrastructure\models\capacidad_carga_model');

class Campos_formulario {
    protected $textfields;
    protected $datepickers;

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
                'nombre_campo' => 'fecha_creacion',
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
    }

    public function getTextfields () {
        return $this->textfields;
    }
    public function getDatepickers () {
        return $this->datepickers;
    }

}