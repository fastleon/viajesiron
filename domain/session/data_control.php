<?php

//variable para guardar los ultimos cambios de cada variable
define('LAST_MOD', 'viajesiron_last_modification');
define('MINS_LAST_MOD', 'viajesiron_mins_last_modification');

//variables de sesion para cada dato bajo control de esta clase
define('CONFORMADOR', 'viajesiron_conformador');
define('REPORTES_CUMPLIDOS', 'viajesiron_reportes_cumplidos');
define('TRANSPORTADORAS', 'viajesiron_transportadoras');
// define('', 'viajesiron_');
// define('', 'viajesiron_');
// define('', 'viajesiron_');

abstract class DataControl {
    protected static function guardarSesion($variable_sesion, $data)  {
        $_SESSION[LAST_MOD][$variable_sesion] = REQUEST_TIME;
        $_SESSION[$variable_sesion] = $data;
        // add_error('Guardando :' . $variable_sesion);
    }
    
    protected static function cargarSesion($variable_sesion) {
        $response = array();
        if (isset($_SESSION[LAST_MOD][$variable_sesion])) {
            $_SESSION[MINS_LAST_MOD][$variable_sesion] = ( (REQUEST_TIME - $_SESSION[LAST_MOD][$variable_sesion]) /60 );
        } 
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
    
    public function llamarCargarSesion($last_time_checked = 0) {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ( isset($_SESSION[MINS_LAST_MOD][$this->variable_sesion]) && $_SESSION[MINS_LAST_MOD][$this->variable_sesion]>$last_time_checked ) {
            //no aplica logica de carga cada tiempo determinado
            //TODO: REVISAR si aplica el tiempo de carga
        }

        unset($data['mins_last_mod']);
        if ($data == array()){ $data = new ReporteCumplidosModel(); }
        return ($data);
    } 
    
    public function llamarBorrarSesion() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlConformador extends DataControl{
    protected $variable_sesion = 'viajesiron_conformador';
    public function llamarGuardarSesion($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarSesion() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarSesion() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlTransportadoras extends DataControl{
    protected $variable_sesion = 'viajesiron_transportadoras';
    public function llamarGuardarSesion($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarSesion() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        // add_error($data['mins_last_mod'], 'transportadora ultima mod');
        // if ( isset($data['mins_last_mod']) && $data['mins_last_mod']>MINS_SIN_MODIFICAR ) {
        //     //TODO: Hacer consulta transportadoras
        //     unset($data['mins_last_mod']);
        // }
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarSesion() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

//TODO: Agregar la logica de tiempo de ultima modificacion a los controles posteriores

class DataControlConceptosAdicionales extends DataControl{
    protected $variable_sesion = 'viajesiron_conceptos_adicionales';
    public function llamarGuardarSesion($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarSesion() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarSesion() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlCapacidadCarga extends DataControl{
    protected $variable_sesion = 'viajesiron_capacidad_carga';
    public function llamarGuardarSesion($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarSesion() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarSesion() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

