<?php

module_load_include('php', 'viajesiron', 'Utils/utils');

class FiltroBusquedaReportesCumplidosEntity {

    private $remision;
    private $fechaCreacionInicial;
    private $fechaCreacionFinal;
    private $fechaPlaneacionInicial;
    private $fechaPlaneacionFinal;
    private $fechaEstimadaInicial;
    private $fechaEstimadaFinal;
    private $fechaEntregaInicial;
    private $fechaEntregaFinal;
    private $ciudadOrigen;
    private $ciudadDestino;
    private $tipoDeCarga;
    private $transportadora;

    public function __construct(
        $remision = '',
        $fechaCreacionInicial = '',
        $fechaCreacionFinal = '',
        $fechaPlaneacionInicial = '',
        $fechaPlaneacionFinal = '',
        $fechaEstimadaInicial = '',
        $fechaEstimadaFinal = '',
        $fechaEntregaInicial = '',
        $fechaEntregaFinal = '',
        $ciudadOrigen = '',
        $ciudadDestino = '',
        $tipoDeCarga = '',
        $transportadora = ''
    )
    {
        $this->remision = $remision;
        $this->fechaCreacionInicial = $fechaCreacionInicial;
        $this->fechaCreacionFinal = $fechaCreacionFinal;
        $this->fechaPlaneacionInicial = $fechaPlaneacionInicial;
        $this->fechaPlaneacionFinal = $fechaPlaneacionFinal;
        $this->fechaEstimadaInicial = $fechaEstimadaInicial;
        $this->fechaEstimadaFinal = $fechaEstimadaFinal;
        $this->fechaEntregaInicial = $fechaEntregaInicial;
        $this->fechaEntregaFinal = $fechaEntregaFinal;
        $this->ciudadOrigen = $ciudadOrigen;
        $this->ciudadDestino = $ciudadDestino;
        $this->tipoDeCarga = $tipoDeCarga;
        $this->transportadora = $transportadora;
    }

    public function setRemision($remision) { $this->remision = $remision;}
    public function setFechaCreacionInicial($fechaCreacionInicial) { $this->fechaCreacionInicial = $fechaCreacionInicial;}
    public function setFechaCreacionFinal($fechaCreacionFinal) { $this->fechaCreacionFinal = $fechaCreacionFinal;}
    public function setFechaPlaneacionInicial($fechaPlaneacionInicial) { $this->fechaPlaneacionInicial = $fechaPlaneacionInicial;}
    public function setFechaPlaneacionFinal($fechaPlaneacionFinal) { $this->fechaPlaneacionFinal = $fechaPlaneacionFinal;}
    public function setFechaEstimadaInicial($fechaEstimadaInicial) { $this->fechaEstimadaInicial = $fechaEstimadaInicial;}
    public function setFechaEstimadaFinal($fechaEstimadaFinal) { $this->fechaEstimadaFinal = $fechaEstimadaFinal;}
    public function setFechaEntregaInicial($fechaEntregaInicial) { $this->fechaEntregaInicial = $fechaEntregaInicial;}
    public function setFechaEntregaFinal($fechaEntregaFinal) { $this->fechaEntregaFinal = $fechaEntregaFinal;}
    public function setCiudadOrigen($ciudadOrigen) { $this->ciudadOrigen = $ciudadOrigen;}
    public function setCiudadDestino($ciudadDestino) { $this->ciudadDestino = $ciudadDestino;}
    public function setTipoDeCarga($tipoDeCarga) { $this->tipoDeCarga = $tipoDeCarga;}
    public function setTransportadora($transportadora) { $this->transportadora = $transportadora;}

    public function getRemision(){ return $this->remision; }
    public function getFechaCreacionInicial(){ return $this->fechaCreacionInicial; }
    public function getFechaCreacionFinal(){ return $this->fechaCreacionFinal; }
    public function getFechaPlaneacionInicial(){ return $this->fechaPlaneacionInicial; }
    public function getFechaPlaneacionFinal(){ return $this->fechaPlaneacionFinal; }
    public function getFechaEstimadaInicial(){ return $this->fechaEstimadaInicial; }
    public function getFechaEstimadaFinal(){ return $this->fechaEstimadaFinal; }
    public function getFechaEntregaInicial(){ return $this->fechaEntregaInicial; }
    public function getFechaEntregaFinal(){ return $this->fechaEntregaFinal; }
    public function getCiudadOrigen(){ return $this->ciudadOrigen; }
    public function getCiudadDestino(){ return $this->ciudadDestino; }
    public function getTipoDeCarga(){ return $this->tipoDeCarga; }
    public function getTransportadora(){ return $this->transportadora; }

    public function toArray() {
        return Utils::toArray($this);
    }

    public function fromArray($data) {
        $datos_formulario = array(
            'remision' => 'remision',
            'fechaCreacionInicial' => 'fecha_creacion_inicial',
            'fechaCreacionFinal' => 'fecha_creacion_final',
            'fechaPlaneacionInicial' => 'fecha_planeacion_inicial',
            'fechaPlaneacionFinal' => 'fecha_planeacion_final',
            'fechaEstimadaInicial' => 'fecha_estimada_inicial',
            'fechaEstimadaFinal' => 'fecha_estimada_final',
            'fechaEntregaInicial' => 'fecha_entrega_inicial',
            'fechaEntregaFinal' => 'fecha_entrega_final',
            'ciudadOrigen' => 'ciudad_origen',
            'ciudadDestino' => 'ciudad_destino',
            'tipoDeCarga' => 'tipo_carga',
            'transportadora' => 'transportadora',
            );
        foreach($datos_formulario as $propiedad => $campo) {
            $this->$propiedad = $data[$campo];
        }
        return $this;    
    }

}
/*{
	"remision":"",
	"fechaCreacionInicial":"18/02/2024",
	"fechaCreacionFinal":"",
	"fechaPlaneacionInicial":"",
	"fechaPlaneacionFinal":"",
	"fechaEstimadaInicial":"",
	"fechaEstimadaFinal":"",
	"fechaEntregaInicial":"",
	"fechaEntregaFinal":"",
	"ciudadOrigen":"",
	"ciudadDestino":"",
	"tipoDeCarga":"Paqueteo",
	"transportadora":""
}*/