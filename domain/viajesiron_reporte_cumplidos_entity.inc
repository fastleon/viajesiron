<?php

class ReporteCumplidosEntity {
    private $whs_id;
    private $fecha_creacion;
    private $planeacion_informe;
    private $fecha_estimada_entrega;
    private $localidad_origen;
    private $prueba_entrega;
    private $tipo_carga;
    private $dias_entrega;
    private $ciudad;
    private $fotos;
    private $transportadora;

    function __construct(
        $whs_id = '',
        $fecha_creacion = '',
        $planeacion_informe = '',
        $fecha_estimada_entrega = '',
        $localidad_origen = '',
        $prueba_entrega = '',
        $tipo_carga = '',
        $dias_entrega = '',
        $ciudad = '',
        $fotos = '',
        $transportadora = ''
    )
    {
        $this->whs_id = $whs_id;
        $this->fecha_creacion = $fecha_creacion;
        $this->planeacion_informe = $planeacion_informe;
        $this->fecha_estimada_entrega = $fecha_estimada_entrega;
        $this->localidad_origen = $localidad_origen;
        $this->prueba_entrega = $prueba_entrega;
        $this->tipo_carga = $tipo_carga;
        $this->dias_entrega = $dias_entrega;
        $this->ciudad = $ciudad;
        $this->fotos = $fotos;
        $this->transportadora = $transportadora;
    }
   
    public function setWhsId($whs_id) {$this->whs_id = $whs_id;}
    public function setFechaCreacion($fecha_creacion) {$this->fecha_creacion = $fecha_creacion;}
    public function setPlaneacionInforme($planeacion_informe) {$this->planeacion_informe = $planeacion_informe;}
    public function setFechaEstimadaEntrega($fecha_estimada_entrega) {$this->fecha_estimada_entrega = $fecha_estimada_entrega;}
    public function setLocalidadOrigen($localidad_origen) {$this->localidad_origen = $localidad_origen;}
    public function setPruebaEntrega($prueba_entrega) {$this->prueba_entrega = $prueba_entrega;}
    public function setTipoCarga($tipo_carga) {$this->tipo_carga = $tipo_carga;}
    public function setDiasEntrega($dias_entrega) {$this->dias_entrega = $dias_entrega;}
    public function setCiudad($ciudad) {$this->ciudad = $ciudad;}
    public function setFotos($fotos) {$this->fotos = $fotos;}
    public function setTransportadora($transportadora) {$this->transportadora = $transportadora;}

    public function getWhsId() {return $this->whs_id;}
    public function getFechaCreacion() {return $this->fecha_creacion;}
    public function getPlaneacionInforme() {return $this->planeacion_informe;}
    public function getFechaEstimadaEntrega() {return $this->fecha_estimada_entrega;}
    public function getLocalidadOrigen() {return $this->localidad_origen;}
    public function getPruebaEntrega() {return $this->prueba_entrega;}
    public function getTipoCarga() {return $this->tipo_carga;}
    public function getDiasEntrega() {return $this->dias_entrega;}
    public function getCiudad() {return $this->ciudad;}
    public function getFotos() {return $this->fotos;}
    public function getTransportadora() {return $this->transportadora;}

    public function toArray() {
        return array(
            'whs_id' => $this->whs_id,
            'fecha_creacion' => $this->fecha_creacion,
            'planeacion_informe' => $this->planeacion_informe,
            'fecha_estimada_entrega' => $this->fecha_estimada_entrega,
            'localidad_origen' => $this->localidad_origen,
            'prueba_entrega' => $this->prueba_entrega,
            'tipo_carga' => $this->tipo_carga,
            'dias_entrega' => $this->dias_entrega,
            'ciudad' => $this->ciudad,
            'fotos' => $this->fotos,
            'transportadora' => $this->transportadora,
        );
    }
}
