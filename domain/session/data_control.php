<?php

//dependencias
module_load_include('php', 'viajesiron', 'infrastructure/controllers/transportadoras_controller');
module_load_include('php', 'viajesiron', 'infrastructure/controllers/capacidad_carga_controller');
module_load_include('php', 'viajesiron', 'infrastructure/models/transportadora_model');
module_load_include('php', 'viajesiron', 'infrastructure/models/capacidad_carga_model');

//constantes para guardar los ultimos cambios de cada variable
define('LAST_MOD', 'viajesiron_last_modification');
define('MINS_LAST_MOD', 'viajesiron_mins_last_modification');

//constantes de sesion para cada dato bajo control de esta clase
define('PARAMETROS_REST', 'viajesiron_parametros_rest');
define('TRANSPORTADORAS', 'viajesiron_transportadoras');
define('CAPACIDAD_CARGAS', 'viajesiron_lista_capacidad_carga');
//define('CONFORMADOR', 'viajesiron_conformador');
define('REPORTES_CUMPLIDOS', 'viajesiron_reportes_cumplidos');
define('ESTADO_FORM_REPORTES_CUMPLIDOS', 'viajesiron_estado_reportes_cumplidos');

/**
 * Clase para la administracion de los datos guardados en capa de sesion
 * metodos guardarSesion, cargarSesion y borrarSesion.
 * Propiedades: [LAST_MOD][$variable] -> ultima vez que fue guardada
 *              [MINS_LAST_MOD][$variable] 
 */
abstract class DataControl
{
    protected static function guardarSesion($variable_sesion, $data)  {
        $_SESSION[LAST_MOD][$variable_sesion] = REQUEST_TIME;
        $_SESSION[$variable_sesion] = $data;
        if (DEBUG_MODE) { add_error('DEBUG: Guardando :' . $variable_sesion); }
        // add_error($data, 'dato a guardar');
    }
    
    protected static function cargarSesion($variable_sesion) {
        $response = false;
        if (isset($_SESSION[LAST_MOD][$variable_sesion])) {
            $_SESSION[MINS_LAST_MOD][$variable_sesion] = ( (REQUEST_TIME - $_SESSION[LAST_MOD][$variable_sesion]) /60 );
        } 
        if (isset($_SESSION[$variable_sesion])) {
            $response = array();
            $all_data = $_SESSION[$variable_sesion];
            if ( is_array($all_data) ) {
                foreach( $all_data as $key=>$value ) {
                    $response[$key] = $value;
                }
            } else {
                $response = $all_data;
            }
        }
        if (DEBUG_MODE) { add_error('DEBUG: Cargando :' . $variable_sesion); }
        // add_error($response, 'respuesta:');
        return ($response);
    } 
    
    protected static function borrarSesion($variable_sesion) {
        if (DEBUG_MODE) { add_error('DEBUG: borrando :' . $variable_sesion); }
        unset($_SESSION[$variable_sesion]);
    }
}

/** 
 * Controlador de datos guardados en sesion
 * Datos para parametros de consulta en servicios REST - modulo viajes
 * NO USA ACTUALMENTE _SESSION.
 */
class DataControlParametrosREST extends DataControl
{
    protected $variable_drupal = PARAMETROS_REST;
    public function llamarGuardarDato($data) {
        // DataControl::guardarSesion($this->variable_sesion, $data);
        variable_set($this->variable_drupal, $data);
    }

    public function llamarCargarDato($last_time_checked = 0) {
        // $minutos_ultima_modificacion = $last_time_checked;
        // if ( !empty($_SESSION[LAST_MOD][$this->variable_drupal]) ){
        //     $ultima_modificacion = $_SESSION[LAST_MOD][$this->variable_drupal];
        //     $minutos_ultima_modificacion = (REQUEST_TIME - $ultima_modificacion) / 60;
        // }
        // if ($minutos_ultima_modificacion >= $last_time_checked) {
            //LOGICA SI SE BUSCARA DE DOS FUENTES SEGUN EL TIEMPO DE ULTIMA CONSULTA
        // }
        // $data = DataControl::cargarSesion($this->variable_drupal);
        $data = variable_get($this->variable_drupal, false);
        $_SESSION[LAST_MOD][$this->variable_drupal] = REQUEST_TIME;
        if (!$data) {
            drupal_set_message('No se logró cargar los parametros de configuración del modulo, por favor comuniquese con su administrador.', 'warning');
        }
        return ($data);
    } 

    public function llamarBorrarDato() {
        variable_del($this->variable_drupal);
        // DataControl::borrarSesion($this->variable_drupal);
        unset($_SESSION[LAST_MOD][$this->variable_drupal]);
        unset($_SESSION[MINS_LAST_MOD][$this->variable_drupal]);
    }
}

class DataControlReportesCumplidosResultados extends DataControl
{
    protected $variable_sesion = REPORTES_CUMPLIDOS;
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }

    public function llamarCargarDato($last_time_checked = 0) {
        //No maneja logica de tiempo de consulta, comunicación directa con $_SESSION
        $data = DataControl::cargarSesion($this->variable_sesion);
        $_SESSION[LAST_MOD][$this->variable_sesion] = REQUEST_TIME;
        if (!$data) {
            add_error('DEBUG: No se encontraron datos para cargar tabla reportes cumplidos');
        }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }

}

class DataControlReportesCumplidosFormulario extends DataControl
{
    protected $variable_sesion = ESTADO_FORM_REPORTES_CUMPLIDOS;
    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }

    public function llamarCargarDato($last_time_checked = 0) {
        //No maneja logica de tiempo de consulta, comunicación directa con $_SESSION
        $data = DataControl::cargarSesion($this->variable_sesion);
        $_SESSION[LAST_MOD][$this->variable_sesion] = REQUEST_TIME;
        if (!$data) { add_error('DEBUG: No hay estados anteriores de este formulario'); }
        return ($data);
    } 

    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlTransportadoras extends DataControl 
{
    protected $variable_sesion = TRANSPORTADORAS;

    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    /**
     * Carga los datos de las transportadoras de SESSION si han pasado $last_time_checked mins desde la ultima consulta
     */
    public function llamarCargarDato($last_time_checked = 15) {
        $minutos_ultima_modificacion = $last_time_checked;
        if ( !empty($_SESSION[LAST_MOD][$this->variable_sesion]) ){
            $ultima_modificacion = $_SESSION[LAST_MOD][$this->variable_sesion];
            $minutos_ultima_modificacion = (REQUEST_TIME - $ultima_modificacion) / 60;
        }
        if ($minutos_ultima_modificacion >= $last_time_checked) { 
            $data = (new TransportadorasController())->getTransportadoras();
            if ($data) {
                $data = $data->toArray();
                DataControl::guardarSesion($this->variable_sesion, $data);
            }
        } else {
            $data = DataControl::cargarSesion($this->variable_sesion);
        }
        return ($data);
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
        unset( $_SESSION[LAST_MOD][$this->variable_sesion]);
    }
}

class DataControlCapacidadCargas extends DataControl 
{
    protected $variable_sesion = CAPACIDAD_CARGAS;

    public function llamarGuardarDato($data) {
        DataControl::guardarSesion($this->variable_sesion, $data);
    }
    
    public function llamarCargarDato($nombre_carga, $last_time_checked = 15) {
        $data = DataControl::cargarSesion($this->variable_sesion);
        $data = ($data) ? $data : array();
                
        if ( !isset($data[$nombre_carga]) || ($_SESSION[MINS_LAST_MOD][$this->variable_sesion] > $last_time_checked) ) {
            $capacidad_carga = (new CapacidadCargaController())->getCapacidadCarga($nombre_carga);
            if ($capacidad_carga) {
                $data[$nombre_carga] = $capacidad_carga->toArray();
                DataControl::guardarSesion($this->variable_sesion, $data);
            } else {
                return false;
            }
        } 
        return $data[$nombre_carga];
    } 
    
    public function llamarBorrarDato() {
        DataControl::borrarSesion($this->variable_sesion);
    }
}

class DataControlConformador extends DataControl
{
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

class DataControlConceptosAdicionales extends DataControl
{
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

