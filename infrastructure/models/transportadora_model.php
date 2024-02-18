<?php

module_load_include('php', 'viajesiron', 'Utils/utils');
module_load_include('php', 'viajesiron', 'domain\entities\transportadora_entity');

class TransportadoraModel {
    private $id;
    private $nombre;

    public function __construct($id=0, $nombre='sin_nombre') {
        $this->id = $id;
        $this->nombre = $nombre;
    }

    public function getId() { return $this->id; }
    public function getNombre() { return $this->nombre; }
    
    public function setId($id) { $this->id = $id; }
    public function setNombre($nombre) { $this->nombre = $nombre; }

    public function toArray() { 
        return Utils::toArray($this); 
    }

    public function fromEntityWebservice($transportadora) {
        //EntityWebservice -> $id; $nombre; $status;
        $this->setId( $transportadora->getId() );
        $this->setNombre( $transportadora->getNombre() );
        return $this;
    }

}