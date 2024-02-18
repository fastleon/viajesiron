<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class TransportadoraEntityWebservice {
    private $id;
    private $nombre;
    private $status;
    //private $codigo;
    //private $identificador;

    public function __construct()
    {
        // $this->id = $id;
        // $this->nombre = $nombre;
        // $this->status = $status;
    }

    public function setId($id) {$this->id = $id;}
    public function setNombre($nombre) {$this->nombre = $nombre;}
    public function setStatus($status) {$this->status = $status;}

    public function getId() {return $this->id;}
    public function getNombre() {return $this->nombre;}
    public function getStatus() {return $this->status;}

    public function toArray() {
        return Utils::toArray($this);
    }

    public function fromJson($json) {
        if ( isset($json['id']) ) {
            $this->setId($json['id']);
            $this->setNombre($json['nombre']);
            $this->setStatus($json['status']);
            return $this;
        } else {
            return false;
        }
    }
}