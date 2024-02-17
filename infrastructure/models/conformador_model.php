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
        $transportadora = new TransportadoraModel(),
        $adicionales = array(),
        $remisiones = array(),
        $peso_remisiones_kg = 0,
        $peso_busqueda_kg = 0,
        $busqueda = array()
    )
    {
        $this->id = $id;
        $this->nombre = $nombre;
        $this->tipo_carga = $tipo_carga;
        $this->tipo_carro = $tipo_carro;
        $this->transportadora = $transportadora;
        $this->adicionales = $adicionales;
        $this->remisiones = $remisiones;
        $this->peso_remisiones_kg = $peso_remisiones_kg;
        $this->peso_busqueda_kg = $peso_busqueda_kg;
        $this->busqueda = $busqueda;
    }

    public function datos_en_blanco() {
        this->__construct();
        (new DataControlConformador())->llamarGuardarSesion();
        (new DataControlTransportadoras())->llamarGuardarSesion();
        (new DataControlConceptosAdicionales())->llamarGuardarSesion();
        (new DataControlCapacidadCarga())->llamarGuardarSesion();
    }
}