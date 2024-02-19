<?php

module_load_include('php', 'viajesiron', 'domain\repositories\capacidad_carga_repository');
//module_load_include('php', 'viajesiron', 'domain\rest_service\call_webservice_endpoints');
module_load_include('php', 'viajesiron', 'domain\rest_service\call_viajes_endpoints');
module_load_include('php', 'viajesiron', 'domain\entities\capacidad_carga_entity');
module_load_include('php', 'viajesiron', 'infrastructure\models\capacidad_carga_model');


class CapacidadCargaWebserviceDatasource implements CapacidadCargaRepository {

    public function getCapacidadCarga($nombre_carga) {
        $response = false;
        $json = CallViajesEndpoints::call_consultar_carga($nombre_carga);
        if ( isset($json['cargaBO']['id']) ) {
            $response = array();
            $entity = new CapacidadCargaWebserviceEntity();
            $entity = $entity->fromJson($json);
            $local_model = new CapacidadCargaModel();
            $response = $local_model->fromEntityWebservice($entity);
        }
        return $response;
    }

} 