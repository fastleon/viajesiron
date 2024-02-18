<?php

//dependencias
module_load_include('php', 'viajesiron', 'infrastructure\controllers\transportadoras_controller');

//constantes para guardar los ultimos cambios de cada variable
define('LAST_MOD', 'viajesiron_last_modification');
define('MINS_LAST_MOD', 'viajesiron_mins_last_modification');

//constantes de sesion para cada dato bajo control de esta clase
define('PARAMETROS_REST', 'viajesiron_rest_viajes');
define('TRANSPORTADORAS', 'viajesiron_transportadoras');
define('CONFORMADOR', 'viajesiron_conformador');
define('REPORTES_CUMPLIDOS', 'viajesiron_reportes_cumplidos');
// define('', 'viajesiron_');
// define('', 'viajesiron_');


/**
 * Clase para la administracion de los datos guardados en capa de sesion
 * metodos guardarSesion, cargarSesion y borrarSesion.
 * Propiedades: [LAST_MOD][$variable] -> ultima vez que fue guardada
 *              [MINS_LAST_MOD][$variable] 
 */
abstract class DataControl {
    protected static function guardarSesion($variable_sesion, $data)  {
        $_SESSION[LAST_MOD][$variable_sesion] = REQUEST_TIME;
        $_SESSION[$variable_sesion] = $data;
        // add_error('Guardando :' . $variable_sesion);
    }
    
    protected static function cargarSesion($variable_sesion) {
        $response = false;
        if (isset($_SESSION[LAST_MOD][$variable_sesion])) {
            $_SESSION[MINS_LAST_MOD][$variable_sesion] = ( (REQUEST_TIME - $_SESSION[LAST_MOD][$variable_sesion]) /60 );
        } 
        if (isset($_SESSION[$variable_sesion])) {
            $response = array();
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


/** 
 * Controlador de datos guardados en sesion
 * Datos para parametros de consulta en servicios REST - modulo viajes
 */
class DataControlParametrosREST extends DataControl {
    protected $variable_sesion = PARAMETROS_REST;
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }

    public function llamarCargarDato($last_time_checked = 0) {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if (!$data) {
            form_set_error('', 'No se logró cargar los parametros de configuración del modulo, por favor comuniquese con su administrador.');
        }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}


class DataControlReportesCumplidos extends DataControl {
    protected $variable_sesion = REPORTES_CUMPLIDOS;
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }

    public function llamarCargarDato($last_time_checked = 0) {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ( isset($_SESSION[MINS_LAST_MOD][$this->variable_sesion]) && $_SESSION[MINS_LAST_MOD][$this->variable_sesion]>$last_time_checked ) {
            //no aplica logica de carga cada tiempo determinado
            //TODO: REVISAR si aplica el tiempo de carga
        }

        unset($data['mins_last_mod']);
        if ($data == array()){ $data = new ReporteCumplidosModel(); }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}


class DataControlTransportadoras extends DataControl{
    protected $variable_sesion = TRANSPORTADORAS;

    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarDato($last_time_checked = 0) {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ( (!$data) || ($_SESSION[MINS_LAST_MOD][$this->variable_sesion] > $last_time_checked) ) {
            $data = (new TransportadorasController())->getTransportadoras();
        }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlConformador extends DataControl{
    protected $variable_sesion = 'viajesiron_conformador';
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarDato() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}
//TODO: Agregar la logica de tiempo de ultima modificacion a los controles posteriores

class DataControlConceptosAdicionales extends DataControl{
    protected $variable_sesion = 'viajesiron_conceptos_adicionales';
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarDato() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlCapacidadCarga extends DataControl{
    protected $variable_sesion = 'viajesiron_capacidad_carga';
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarDato() {
        $data = DataControl::cargarSesion($this->variable_sesion);
        if ($data == array()){ $data = new ConformadorModel(); }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

