<?php

module_load_include('php', 'viajesiron', 'Utils\utils');
module_load_include('php', 'viajesiron', 'domain\entities\capacidad_carga_entity');

class CapacidadCargaModel {
    private $tipo_carro;
    private $carga_minima;
    private $carga_maxima;

    public function __construct($tipo_carro='', $carga_minima='', $carga_maxima='')
    {
        $this->tipo_carro = $tipo_carro;
        $this->carga_minima = $carga_minima;
        $this->carga_maxima = $carga_maxima;
    }

    public function getTipoCarro() { return $this->tipo_carro; }
    public function getCargaMinima() { return $this->carga_minima; }
    public function getCargaMaxima() { return $this->carga_maxima; }
    public function setTipoCarro($tipo_carro) { $this->tipo_carro = $tipo_carro; }
    public function setCargaMinima($carga_minima) { $this->carga_minima = $carga_minima; }
    public function setCargaMaxima($carga_maxima) { $this->carga_maxima = $carga_maxima; }

    public function toArray() {
        return Utils::toArray($this);
    }

    public function fromEntityWebservice($capacidad_carga) {
        //EntityWebservice -> tipo_carro, carga_minima, carga_maxima
        $this->setTipoCarro($capacidad_carga->getTipoCarro());
        $this->setCargaMinima($capacidad_carga->getCargaMinima());
        $this->setCargaMaxima($capacidad_carga->getCargaMaxima());
        return $this;
    }

}