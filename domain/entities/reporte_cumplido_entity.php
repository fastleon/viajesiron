<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class ReporteCumplidoEntityWebservice {
    private $whs_id;
    private $fecha_creacion;
    private $planeacion_informe;
    private $fecha_estimada_entrega;
    private $ciudad_origen;
    private $prueba_entrega;
    private $tipo_carga;
    private $dias_entrega;
    private $ciudad_destino;
    private $fotos;
    private $transportadora;
    private $numero_poblacion;
    private $zona_ventas;

    function __construct(
        $whs_id = '',
        $fecha_creacion = '',
        $planeacion_informe = '',
        $fecha_estimada_entrega = '',
        $ciudad_origen = '',
        $prueba_entrega = '',
        $tipo_carga = '',
        $dias_entrega = '',
        $ciudad_destino = '',
        $fotos = '',
        $transportadora = '',
        $numero_poblacion = '',
        $zona_ventas = ''
    )
    {
        $this->whs_id = $whs_id;
        $this->fecha_creacion = $fecha_creacion;
        $this->planeacion_informe = $planeacion_informe;
        $this->fecha_estimada_entrega = $fecha_estimada_entrega;
        $this->ciudad_origen = $ciudad_origen;
        $this->prueba_entrega = $prueba_entrega;
        $this->tipo_carga = $tipo_carga;
        $this->dias_entrega = $dias_entrega;
        $this->ciudad_destino = $ciudad_destino;
        $this->fotos = $fotos;
        $this->transportadora = $transportadora;
        $this->numero_poblacion = $numero_poblacion;
        $this->zona_ventas = $zona_ventas;
    }
   
    public function setWhsId($whs_id) {$this->whs_id = $whs_id;}
    public function setFechaCreacion($fecha_creacion) {$this->fecha_creacion = $fecha_creacion;}
    public function setPlaneacionInforme($planeacion_informe) {$this->planeacion_informe = $planeacion_informe;}
    public function setFechaEstimadaEntrega($fecha_estimada_entrega) {$this->fecha_estimada_entrega = $fecha_estimada_entrega;}
    public function setCiudadOrigen($localidad_origen) {$this->ciudad_origen = $localidad_origen;}
    public function setPruebaEntrega($prueba_entrega) {$this->prueba_entrega = $prueba_entrega;}
    public function setTipoCarga($tipo_carga) {$this->tipo_carga = $tipo_carga;}
    public function setDiasEntrega($dias_entrega) {$this->dias_entrega = $dias_entrega;}
    public function setCiudadDestino($ciudad) {$this->ciudad_destino = $ciudad;}
    public function setFotos($fotos) {$this->fotos = $fotos;}
    public function setTransportadora($transportadora) {$this->transportadora = $transportadora;}
    public function setNumeroPoblacion($numero_poblacion) {$this->numero_poblacion = $numero_poblacion;}
    public function setZonaVentas($zona_ventas) {$this->zona_ventas = $zona_ventas;}

    public function getWhsId() {return $this->whs_id;}
    public function getFechaCreacion() {return $this->fecha_creacion;}
    public function getPlaneacionInforme() {return $this->planeacion_informe;}
    public function getFechaEstimadaEntrega() {return $this->fecha_estimada_entrega;}
    public function getCiudadOrigen() {return $this->ciudad_origen;}
    public function getPruebaEntrega() {return $this->prueba_entrega;}
    public function getTipoCarga() {return $this->tipo_carga;}
    public function getDiasEntrega() {return $this->dias_entrega;}
    public function getCiudadDestino() {return $this->ciudad_destino;}
    public function getFotos() {return $this->fotos;}
    public function getTransportadora() {return $this->transportadora;}
    public function getNumeroPoblacion() {return $this->numero_poblacion;}
    public function getZonaVentas() {return $this->zona_ventas;}

    public function toArray() {
        return Utils::toArray($this);
    }
    public function fromJson($json) {
        if ( !empty($json) ) {
            if ( empty($json['remision']) ) {
                return false;
            }
            empty($json['remision']) ? '' : $this->setWhsId($json['remision']);
            empty($json['fechaCreacion']) ? '' : $this->setFechaCreacion($json['fechaCreacion']);
            empty($json['fechaPlaneadaDeEntrega']) ? '' : $this->setPlaneacionInforme($json['fechaPlaneadaDeEntrega']);
            empty($json['fechaEstimadaDeEntrega']) ? '' : $this->setFechaEstimadaEntrega($json['fechaEstimadaDeEntrega']);
            empty($json['ciudadOrigen']) ? '' : $this->setCiudadOrigen($json['ciudadOrigen']);
            empty($json['fechaDeEntrega']) ? '' : $this->setPruebaEntrega($json['fechaDeEntrega']);
            empty($json['tipoDeCarga']) ? '' : $this->setTipoCarga($json['tipoDeCarga']);
            empty($json['diasDeEntrega']) ? '' : $this->setDiasEntrega($json['diasDeEntrega']);
            empty($json['ciudadDestino']) ? '' : $this->setCiudadDestino($json['ciudadDestino']);
            empty($json['soportes']) ? '' : $this->setFotos($json['soportes']);
            empty($json['transportadora']) ? '' : $this->setTransportadora($json['transportadora']);      
            empty($json['numeroPoblacion']) ? '' : $this->setNumeroPoblacion($json['numeroPoblacion']);      
            empty($json['zonaVentas']) ? '' : $this->setZonaVentas($json['zonaVentas']);
            return $this;
        } else {
            return false;
        }
    }
    /* Ejemplo respuesta
    {"resultados": [
        {
            "ciudadDestino": "MEDELLIN",
            "ciudadOrigen": "TOCANCIPA",
            "denominacionZonaVentas": "ZONA ANTIOQUIA",
            "departamentoDestino": "Antioquia",
            "departamentoOrigen": "Cundinamarca",
            "diasDeEntrega": "1",
            "fechaCreacion": "2023-11-01",
            "fechaEstimadaDeEntrega": "2023-11-03",
            "fechaPlaneadaDeEntrega": "2023-11-01",
            "numeroPoblacion": "50001",
            "remision": "50809844",
            "tipoDeCarga": "Paqueteo",
            "transportadora": "EXPRESO ANDINO DE CARGA SA",
            "zonaVentas": "CO04"
        },
        ...
    ]}
    */
}
