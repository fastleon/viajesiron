<?php

module_load_include('php', 'viajesiron', 'domain\repositories\reportes_cumplidos_repository');
module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplido_entity');
module_load_include('php', 'viajesiron', 'domain\rest_service\call_viajes_endpoints');
//module_load_include('php', 'viajesiron', 'domain\rest_service\call_webservice_endpoints');


class ReportesCumplidosWebserviceDatasource implements ReportesCumplidosRepository {

    public function getReportesCumplidos($filter_data)
    {
        //BORRAR+++++
        $test_data =  '{
            "remision":"",
            "fechaCreacionInicial":"",
            "fechaCreacionFinal":"",
            "fechaPlaneacionInicial":"",
            "fechaPlaneacionFinal":"",
            "fechaEstimadaInicial":"",
            "fechaEstimadaFinal":"",
            "fechaEntregaInicial":"",
            "fechaEntregaFinal":"",
            "ciudadOrigen":"",
            "ciudadDestino":"",
            "tipoDeCarga":"Paqueteo",
            "transportadora":""
        }'; //+++++
        $json = CallViajesEndpoints::call_post_reportes_cumplidos($filter_data);
        //TEST +++++
        //$json = json_decode(mockup_cumplidos2());
        //FIN TEST
        $list_reporte_cumplido = array();
        if ($json) {
            foreach ($json['resultados'] as $resultado) {
                $entity = new ReporteCumplidoEntityWebservice();
                $entity = $entity->fromJson($resultado);
                $local_model = new ReporteCumplidoModel();
                $list_reporte_cumplido[] = $local_model->fromEntityWebservice($entity);
            }
            $reportes_cumplidos = new ReportesCumplidosModel($list_reporte_cumplido);
        }
        return $reportes_cumplidos;
    }

    // private function getMockup() {
    //     // Llamado de data simulacion
    //     $json = mockup_cumplidos2();
    //     $datos = json_decode($json);
    //     $datos_modelo = array();
    //     foreach ($datos as $dato) {
    //         $temp_cumplido_model = new ReporteCumplidoModel();
    //         $temp_cumplido_model->fromEntityWebservice($dato);
    //         $datos_modelo[] = $temp_cumplido_model;
    //     }
    // }

}

