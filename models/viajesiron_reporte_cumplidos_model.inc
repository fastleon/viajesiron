<?php

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
        $fotos = '') 
        {
            $this->$whs_pedido = $whs_pedido;
            $this->$creacion_remision = $creacion_remision;
            $this->$planeacion_informe = $planeacion_informe;
            $this->$fecha_estimada_entrega = $fecha_estimada_entrega;
            $this->$localidad_origen = $localidad_origen;
            $this->$prueba_entrega = $prueba_entrega;
            $this->$tipo_carga = $tipo_carga;
            $this->$dias_entrega = $dias_entrega;
            $this->$ciudad = $ciudad;
            $this->$transportadora = $transportadora;
            $this->$fotos = $fotos;
        }
    
    public function fromJson($json) {
        $this->__construct(     //llamado al constructor
            $json['whs_pedido'],
            $json['creacion_remision'],
            $json['planeacion_informe'],
            $json['fecha_estimada_entrega'],
            $json['localidad_origen'],
            $json['prueba_entrega'],
            $json['tipo_carga'],
            $json['dias_entrega'],
            $json['ciudad'],
            $json['transportadora'],
            $json['fotos']
        );
    }

    public function isEmpty() {
        return ($this->whs_pedido == '');
    }
    
}

class DataControlReportesCumplidos {

    private $variable_sesion = 'viajesiron_reporte_cumplidos';
    
    public function guardarSesion($reports) {
        $_SESSION[$this->variable_sesion] = $reports;
    }
    
    public function cargarSesion() {
        $response = array();
        if (isset($_SESSION[$this->variable_sesion])) {
            $reports = $_SESSION[$this->variable_sesion];
            foreach( $reports as $report ) {
                $temp = new ReporteCumplidosModel($report);
                $response[] = $temp;
            }

        } else {
            $response = new ReporteCumplidosModel();

        }
        return ($response);
    } 
    
    public function borrarSesion() {
        unset($_SESSION[$this->variable_sesion]);
    }
}

