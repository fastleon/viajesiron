<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class ReporteCumplidoModel {

    private $whs_id;
    private $fecha_creacion;
    private $planeacion_informe;
    private $fecha_estimada_entrega;
    private $ciudad_origen;
    private $prueba_entrega;
    private $tipo_carga;
    private $dias_entrega;
    private $ciudad_destino;
    private $zona_ventas;
    private $numero_poblacion;
    private $transportadora;
    private $fotos;
    
    public function __construct(
        $whs_id = '',
        $fecha_creacion = '',
        $planeacion_informe = '',
        $fecha_estimada_entrega = '',
        $ciudad_origen = '',
        $prueba_entrega = '',
        $tipo_carga = '',
        $dias_entrega = '',
        $ciudad_destino = '',
        $zona_ventas = '',
        $numero_poblacion = '',
        $transportadora = '',
        $fotos = ''
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
            $this->zona_ventas = $zona_ventas;
            $this->numero_poblacion = $numero_poblacion;
            $this->transportadora = $transportadora;
            $this->fotos = $fotos;
        }

    public function setWhsId($whs_id) {$this->whs_id = $whs_id;}
    public function setFechaCreacion($fecha_creacion) {$this->fecha_creacion = $fecha_creacion;}
    public function setPlaneacionInforme($planeacion_informe) {$this->planeacion_informe = $planeacion_informe;}
    public function setFechaEstimadaEntrega($fecha_estimada_entrega) {$this->fecha_estimada_entrega = $fecha_estimada_entrega;}
    public function setCiudadOrigen($ciudad_origen) {$this->ciudad_origen = $ciudad_origen;}
    public function setPruebaEntrega($prueba_entrega) {$this->prueba_entrega = $prueba_entrega;}
    public function setTipoCarga($tipo_carga) {$this->tipo_carga = $tipo_carga;}
    public function setDiasEntrega($dias_entrega) {$this->dias_entrega = $dias_entrega;}
    public function setCiudadDestino($ciudad_destino) {$this->ciudad_destino = $ciudad_destino;}
    public function setZonaVentas($zona_ventas) {$this->zona_ventas = $zona_ventas;}
    public function setNumeroPoblacion($numero_poblacion) {$this->numero_poblacion = $numero_poblacion;}
    public function setFotos($fotos) {$this->fotos = $fotos;}
    public function setTransportadora($transportadora) {$this->transportadora = $transportadora;}

    public function getWhsId() {return $this->whs_id;}
    public function getFechaCreacion() {return $this->fecha_creacion;}
    public function getPlaneacionInforme() {return $this->planeacion_informe;}
    public function getFechaEstimadaEntrega() {return $this->fecha_estimada_entrega;}
    public function getCiudadOrigen() {return $this->ciudad_origen;}
    public function getPruebaEntrega() {return $this->prueba_entrega;}
    public function getTipoCarga() {return $this->tipo_carga;}
    public function getDiasEntrega() {return $this->dias_entrega;}
    public function getCiudadDestino() {return $this->ciudad_destino;}
    public function getZonaVentas() {return $this->zona_ventas;}
    public function getNumeroPoblacion() {return $this->numero_poblacion;}
    public function getFotos() {return $this->fotos;}
    public function getTransportadora() {return $this->transportadora;}
    
    public function fromArray($array_data) {
        $this->__construct(
            isset($array_data['whs_id']) ? $array_data['whs_id'] : null,
            isset($array_data['fecha_creacion']) ? $array_data['fecha_creacion'] : null,
            isset($array_data['planeacion_informe']) ? $array_data['planeacion_informe'] : null,
            isset($array_data['fecha_estimada_entrega']) ? $array_data['fecha_estimada_entrega'] : null,
            isset($array_data['localidad_origen']) ? $array_data['localidad_origen'] : null,
            isset($array_data['prueba_entrega']) ? $array_data['prueba_entrega'] : null,
            isset($array_data['tipo_carga']) ? $array_data['tipo_carga'] : null,
            isset($array_data['dias_entrega']) ? $array_data['dias_entrega'] : null,
            isset($array_data['ciudad']) ? $array_data['ciudad'] : null,
            isset($array_data['zona_ventas']) ? $array_data['zona_ventas'] : null,
            isset($array_data['numero_poblacion']) ? $array_data['numero_poblacion'] : null,
            isset($array_data['transportadora']) ? $array_data['transportadora'] : null,
            isset($array_data['fotos']) ? $array_data['fotos'] : null
        );
    }
    
    public function fromEntityWebservice($reporte) {
        $this->setWhsId($reporte->getWhsId());
        $this->setFechaCreacion($reporte->getFechaCreacion());
        $this->setPlaneacionInforme($reporte->getPlaneacionInforme());
        $this->setFechaEstimadaEntrega($reporte->getFechaEstimadaEntrega());
        $this->setCiudadOrigen($reporte->getCiudadOrigen());
        $this->setPruebaEntrega($reporte->getPruebaEntrega());
        $this->setTipoCarga($reporte->getTipoCarga());
        $this->setDiasEntrega($reporte->getDiasEntrega());
        $this->setCiudadDestino($reporte->getCiudadDestino());
        $this->setZonaVentas($reporte->getZonaVentas());
        $this->setNumeroPoblacion($reporte->getNumeroPoblacion());
        $this->setFotos($reporte->getFotos());
        $this->setTransportadora($reporte->getTransportadora());
        return $this;
    }

    public function toArray() {
        return Utils::toArray($this);
    }
    
}


class ReportesCumplidosModel {

    private $reportes_cumplidos;

    public function __construct(array $reportes) {
        $datos = array();
        foreach ($reportes as $reporte) {
            $datos[$reporte->getWhsId()] = $reporte;
        }
        $this->reportes_cumplidos = $datos;
    }

    public function toArray(){
        $datos = array();
        foreach ($this->reportes_cumplidos as $key=>$value) {
            $datos[$key] = $value->toArray(); 
        }
        return $datos;
    }

}