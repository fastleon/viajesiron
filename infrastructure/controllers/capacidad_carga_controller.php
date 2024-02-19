<?php

module_load_include('php', 'viajesiron', 'infrastructure\datasources\capacidad_carga_webservice_datasource');


class CapacidadCargaController implements CapacidadCargaRepository {

    protected $capacidad_carga_source;

    public function __construct() {
        $this->capacidad_carga_source = new CapacidadCargaWebserviceDatasource(); 
    }

    public function getCapacidadCarga($nombre_carga) {
        return $this->capacidad_carga_source->getCapacidadCarga($nombre_carga);
    }
    
}