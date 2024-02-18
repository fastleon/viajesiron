<?php

//module_load_include('php', 'viajesiron', 'domain/repositories/transportadoras_repository');
//module_load_include('php', 'viajesiron', 'domain/entities/transportadora_entity');
module_load_include('php', 'viajesiron', 'infrastructure\datasources\transportadoras_webservice_datasource');


class TransportadorasController  {

    protected $transportadora_source;

    public function __construct() {
        $this->transportadora_source = new TransportadorasWebserviceDatasource(); 
    }

    public function getTransportadoras() {
        return $this->transportadora_source->getTransportadoras();
    }

    
}