<?php

class ConformadorModel {
    
    public $id;
    public $nombre;
    public $tipo_carga;
    public $tipo_carro;
    public $transportadora;
    public $adicionales;
    public $remisiones;
    public $peso_remisiones_kg;
    public $peso_busqueda_kg;
    public $busqueda;

    public function __construct(
        $id = 'new',
        $nombre = 'autogenerado',
        $tipo_carga = '',
        $tipo_carro = '',
        $transportadora = '',
        $adicionales = array(),
        $remisiones = array(),
        $peso_remisiones_kg = 0,
        $peso_busqueda_kg = 0,
        $busqueda = array(),
    )
    {
        
    }

    public function datos_en_blanco() {
    $data = array();
    $data = array(
        'id' => 'new',
        'nombre' => 'autogenerado',
        'tipo_carga' => '',
        'tipo_carro' => '',
        'transportadora' => '',
        'adicionales' => array(),
        //'id' => '',
        //'concepto' => '',
        //'costo' => 0,    ),
        'remisiones' => array(),
        'peso_remisiones_kg' => 0,
        'peso_busqueda_kg' => 0,
        //'remision' => '',
        //'costo' => 0,    ),
        'busqueda' => array(),
    );
    send_data($data);
    unset($_SESSION['viajesiron_transportadoras']);
    unset($_SESSION['viajesiron_conceptos_adicionales']);
    unset($_SESSION['viajesrion_capacidad_carga']);
}