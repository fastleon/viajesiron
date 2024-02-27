<?php

module_load_include('php', 'viajesiron', 'infrastructure/datasources/reportes_cumplidos_webservice_datasource');


class ReportesCumplidosController  {

    protected $reportes_cumplidos_source;

    public function __construct() {
        $this->reportes_cumplidos_source = new ReportesCumplidosWebserviceDatasource(); 
    }

    public function getReportesCumplidos($filter_data) {
        return $this->reportes_cumplidos_source->getReportesCumplidos($filter_data);
    }

    
}