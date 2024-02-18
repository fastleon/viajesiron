<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class TransportadoraEntity {
    private $id;
    private $nombre;
    private $status;
    //private $codigo;
    //private $identificador;

    public function setId($id) {$this->id = $id;}
    public function setNombre($nombre) {$this->nombre = $nombre;}
    public function setStatus($status) {$this->status = $status;}

    public function getId() {return $this->id;}
    public function getNombre() {return $this->nombre;}
    public function getStatus() {return $this->status;}

    public function toArray() {
        return Utils::toArray($this);
    }
}