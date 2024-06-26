<?php

module_load_include('php', 'viajesiron', 'domain/repositories/transportadoras_repository');
module_load_include('php', 'viajesiron', 'domain/rest_service/call_webservice_endpoints');
module_load_include('php', 'viajesiron', 'domain/entities/transportadora_entity');

class TransportadorasWebserviceDatasource implements TransportadorasRepository {

    public function getTransportadoras() {
        $response = false;
        $json = CallWebServiceEndpoints::call_get_transportadoras();
        if ($json) {
            $list_transportadoras = array();
            foreach ($json['transportadoras'] as $transportadora) {
                $entity = new TransportadoraEntityWebservice();
                $entity = $entity->fromJson($transportadora);
                $local_model = new TransportadoraModel();
                $list_transportadoras[] = $local_model->fromEntityWebservice($entity);
            }
        }
        $transportadoras = new TransportadorasModel($list_transportadoras);
        return $transportadoras;
    }

} 