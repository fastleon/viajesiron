<?php

//module_load_include('php', 'viajesiron', 'domain/repositories/transportadoras_repository');
//module_load_include('php', 'viajesiron', 'domain/entities/transportadora_entity');
module_load_include('php', 'viajesiron', 'infrastructure/datasources/transportadoras_webservice_datasource');


class TransportadorasController  {

    protected $transportadoras_source;

    public function __construct() {
        $this->transportadoras_source = new TransportadorasWebserviceDatasource(); 
    }

    public function getTransportadoras() {
        return $this->transportadoras_source->getTransportadoras();
    }

    
}