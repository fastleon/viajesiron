<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class ReporteCumplidosModel {

    private $whs_pedido;
    private $creacion_remision;
    private $planeacion_informe;
    private $fecha_estimada_entrega;
    private $localidad_origen;
    private $prueba_entrega;
    private $tipo_carga;
    private $dias_entrega;
    private $ciudad;
    private $transportadora;
    private $fotos;
    
    public function __construct(
        $whs_pedido = '',
        $creacion_remision = '',
        $planeacion_informe = '',
        $fecha_estimada_entrega = '',
        $localidad_origen = '',
        $prueba_entrega = '',
        $tipo_carga = '',
        $dias_entrega = '',
        $ciudad = '',
        $transportadora = '',
        $fotos = ''
        )
        {
            $this->whs_pedido = $whs_pedido;
            $this->creacion_remision = $creacion_remision;
            $this->planeacion_informe = $planeacion_informe;
            $this->fecha_estimada_entrega = $fecha_estimada_entrega;
            $this->localidad_origen = $localidad_origen;
            $this->prueba_entrega = $prueba_entrega;
            $this->tipo_carga = $tipo_carga;
            $this->dias_entrega = $dias_entrega;
            $this->ciudad = $ciudad;
            $this->transportadora = $transportadora;
            $this->fotos = $fotos;
        }
        
    
    public function setWhsPedido($whs_pedido) {$this->whs_pedido = $whs_pedido;}
    public function setCreacionRemision($creacion_remision) {$this->creacion_remision = $creacion_remision;}
    public function setPlaneacionInforme($planeacion_informe) {$this->planeacion_informe = $planeacion_informe;}
    public function setFechaEstimadaEntrega($fecha_estimada_entrega) {$this->fecha_estimada_entrega = $fecha_estimada_entrega;}
    public function setLocalidadOrigen($localidad_origen) {$this->localidad_origen = $localidad_origen;}
    public function setPruebaEntrega($prueba_entrega) {$this->prueba_entrega = $prueba_entrega;}
    public function setTipoCarga($tipo_carga) {$this->tipo_carga = $tipo_carga;}
    public function setDiasEntrega($dias_entrega) {$this->dias_entrega = $dias_entrega;}
    public function setCiudad($ciudad) {$this->ciudad = $ciudad;}
    public function setFotos($fotos) {$this->fotos = $fotos;}
    public function setTransportadora($transportadora) {$this->transportadora = $transportadora;}

    public function getWhsPedido() {return $this->whs_pedido;}
    public function getcreacion_remision() {return $this->creacion_remision;}
    public function getPlaneacionInforme() {return $this->planeacion_informe;}
    public function getFechaEstimadaEntrega() {return $this->fecha_estimada_entrega;}
    public function getLocalidadOrigen() {return $this->localidad_origen;}
    public function getPruebaEntrega() {return $this->prueba_entrega;}
    public function getTipoCarga() {return $this->tipo_carga;}
    public function getDiasEntrega() {return $this->dias_entrega;}
    public function getCiudad() {return $this->ciudad;}
    public function getFotos() {return $this->fotos;}
    public function getTransportadora() {return $this->transportadora;}
    
    public function fromArray($array_data) {
        $this->__construct(
            isset($array_data['whs_pedido']) ? $array_data['whs_pedido'] : null,
            isset($array_data['creacion_remision']) ? $array_data['creacion_remision'] : null,
            isset($array_data['planeacion_informe']) ? $array_data['planeacion_informe'] : null,
            isset($array_data['fecha_estimada_entrega']) ? $array_data['fecha_estimada_entrega'] : null,
            isset($array_data['localidad_origen']) ? $array_data['localidad_origen'] : null,
            isset($array_data['prueba_entrega']) ? $array_data['prueba_entrega'] : null,
            isset($array_data['tipo_carga']) ? $array_data['tipo_carga'] : null,
            isset($array_data['dias_entrega']) ? $array_data['dias_entrega'] : null,
            isset($array_data['ciudad']) ? $array_data['ciudad'] : null,
            isset($array_data['transportadora']) ? $array_data['transportadora'] : null,
            isset($array_data['fotos']) ? $array_data['fotos'] : null
        );
    }
    
    public function fromEntity($dato) {
        $this->__construct(     //llamado al constructor
            isset($dato->whs_id) ? $dato->whs_id : null, 
            isset($dato->fecha_creacion) ? $dato->fecha_creacion : null, 
            isset($dato->planeacion_informe) ? $dato->planeacion_informe : null, 
            isset($dato->fecha_estimada_entrega) ? $dato->fecha_estimada_entrega : null, 
            isset($dato->localidad_origen) ? $dato->localidad_origen : null, 
            isset($dato->prueba_entrega) ? $dato->prueba_entrega : null, 
            isset($dato->tipo_carga) ? $dato->tipo_carga : null, 
            isset($dato->dias_entrega) ? $dato->dias_entrega : null, 
            isset($dato->ciudad) ? $dato->ciudad : null, 
            isset($dato->fotos) ? $dato->fotos : null, 
            isset($dato->transportadora) ? $dato->transportadora : null
        );
    }

    public function isEmpty() {
        return ($this->whs_pedido == '');
    }

    public function toArray() {
        return Utils::toArray($this);
    }
    
}