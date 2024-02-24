<?php

module_load_include('php', 'viajesiron', 'domain\repositories\reportes_cumplidos_repository');
module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplido_entity');
module_load_include('php', 'viajesiron', 'domain\rest_service\call_viajes_endpoints');
//module_load_include('php', 'viajesiron', 'domain\rest_service\call_webservice_endpoints');


class ReportesCumplidosWebserviceDatasource implements ReportesCumplidosRepository {

    public function getReportesCumplidos($filter_data)
    {
        $filter_data = json_encode($filter_data->toArray());
        add_error($filter_data, 'DEBUG: json a enviar a la consulta');
        $json = CallViajesEndpoints::call_post_reportes_cumplidos($filter_data);
        //TEST +++++
        $ruta_test_file = drupal_get_path('module','viajesiron') . '\tools\respuesta_reportes_cumplidos.json';
        $json = file_get_contents($ruta_test_file);
        $json = json_decode($json, true);
        add_error('OJO USANDO JSON DE PRUEBAS NO UNA CONSULTA REAL');
        //END TEST +++++
        
        
        $reportes_cumplidos = false;
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

}

