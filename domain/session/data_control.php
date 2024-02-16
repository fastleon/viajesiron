<?php

abstract class DataControl {
    protected static function guardarSesion($variable_sesion, $data)  {
        $_SESSION[$variable_sesion] = $data;
        // add_error('Guardando :' . $variable_sesion);
    }
    
    protected static function cargarSesion($variable_sesion) {
        $response = array();
        if (isset($_SESSION[$variable_sesion])) {
            $all_data = $_SESSION[$variable_sesion];
            foreach( $all_data as $single_data ) {
                $response[] = $single_data;
            }
        }
        // add_error('Cargando: ' . $variable_sesion);
        // add_error($response);
        return ($response);
    } 
    
    protected static function borrarSesion($variable_sesion) {
        // add_error('eliminando: ' . $variable_sesion);
        unset($_SESSION[$variable_sesion]);
    }
}


class DataControlReportesCumplidos extends DataControl {
    protected $variable_sesion = 'viajesiron_reporte_cumplidos';
    public function llamarGuardarSesion($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarSesion() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ReporteCumplidosModel(); }
        return ($data);
    } 
    
    public function llamarBorrarSesion() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

// class DataControlConformador {

// }
