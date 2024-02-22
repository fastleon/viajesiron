<?php

module_load_include('php', 'viajesiron', 'domain\repositories\reporte_cumplidos_repository');
module_load_include('php', 'viajesiron', 'domain\rest_service\call_webservice_endpoints');
//module_load_include('php', 'viajesiron', 'domain\rest_service\call_webservice_endpoints');
module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplidos_entity');



class ReportesCumplidosWebserviceDatasource implements ReportesCumplidosRepository {

    public function getReportesCumplidos()
    {
        $json = CallViajesEndpoints::()
    }
}



module_load_include('php', 'viajesiron', 'domain\repositories\transportadoras_repository');
module_load_include('php', 'viajesiron', 'domain\rest_service\call_webservice_endpoints');
module_load_include('php', 'viajesiron', 'domain\entities\transportadora_entity');

class TransportadorasWebserviceDatasource implements TransportadorasRepository {

    public function getTransportadoras() {
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