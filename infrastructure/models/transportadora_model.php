<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class TransportadoraModel {
    private $id;
    private $nombre;

    public function __construct($id, $nombre) {
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

    public function fromEntity($transportadora) {
        $this->__construct($transportadora['id'], $transportadora['nombre']);
        return $this;
    }

}