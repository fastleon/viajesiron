<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class CapacidadCargaWebserviceEntity {
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
        Utils::toArray($this);
    }

    public function fromJson($json) {
        if ( isset($json['cargaBO']) ) {
            $this->setTipoCarro($json['cargaBO']['tipoDeCarro']['nombre']);
            $this->setCargaMinima($json['cargaBO']['cargaMinima']);
            $this->setCargaMaxima($json['cargaBO']['cargaMaxima']);
            return $this;
        } else {
            return false;
        }
    }
}

/*"cargaBO": {
    "cargaMaxima": 1000,
    "cargaMinima": 950,
    "id": 1,
    "status": true,
    "tipoDeCarro": {
        "codigo": "CARRY",
        "descripcion": "Carry",
        "enabledCodigo": true,
        "enabledDescripcion": true,
        "enabledFechaCreacion": true,
        "enabledFechaModificacion": true,
        "enabledId": true,
        "enabledNombre": true,
        "enabledUsuarioCreacion": true,
        "enabledUsuarioModificacion": true,
        "id": 5177,
        "nombre": "Carry",
        "status": true,
        "usuarioCreacion": 1
    }
}*/