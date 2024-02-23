<?php

module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplido_entity');
module_load_include('php', 'viajesiron', 'domain\entities\filtro_busqueda_reportes_cumplidos_entity');
module_load_include('php', 'viajesiron', 'presentation\reportes_cumplidos\dependencies');

// $this->remision = $data[$formulario['remision']];
        // $this->fechaCreacionInicial = $data[$formulario['fechaCreacionInicial']];
        // $this->fechaCreacionFinal = $data[$formulario['fechaCreacionFinal']];
        // $this->fechaPlaneacionInicial = $data[$formulario['fechaPlaneacionInicial']];
        // $this->fechaPlaneacionFinal = $data[$formulario['fechaPlaneacionFinal']];
        // $this->fechaEstimadaInicial = $data[$formulario['fechaEstimadaInicial']];
        // $this->fechaEstimadaFinal = $data[$formulario['fechaEstimadaFinal']];
        // $this->fechaEntregaInicial = $data[$formulario['fechaEntregaInicial']];
        // $this->fechaEntregaFinal = $data[$formulario['fechaEntregaFinal']];
        // $this->ciudadOrigen = $data[$formulario['ciudadOrigen']];
        // $this->ciudadDestino = $data[$formulario['ciudadDestino']];
        // $this->tipoDeCarga = $data[$formulario['tipoDeCarga']];
        // $this->transportadora = $data[$formulario['transportadora']];

/**
 * HOOK SUBMIT
*/
function reporte_cumplidos_render_form_submit($form, &$form_state) {

    
    //Realizar logica según el boton presionado
    $button_clicked = $form_state['triggering_element']['#name'];

    switch ($button_clicked) {

        case 'Buscar':
            //TODO: traer todos los parametros del form_state
            // $datos_formulario = array(
            //     'remision' => 'remision',
            //     'fechaCreacionInicial' => 'fecha_creacion_inicial',
            //     'fechaCreacionFinal' => 'fecha_creacion_final',
            //     'fechaPlaneacionInicial' => 'fecha_planeacion_inicial',
            //     'fechaPlaneacionFinal' => 'fecha_planeacion_final',
            //     'fechaEstimadaInicial' => 'fecha_estimada_inicial',
            //     'fechaEstimadaFinal' => 'fecha_estimada_final',
            //     'fechaEntregaInicial' => 'fecha_entrega_inicial',
            //     'fechaEntregaFinal' => 'fecha_entrega_final',
            //     'ciudadOrigen' => 'ciudad_origen',
            //     'ciudadDestino' => 'ciudad_destino',
            //     'tipoDeCarga' => 'tipo_carga',
            //     'transportadora' => 'transportadora',
            // );

            $filtro = array();
            $campos = new Campos_formulario();
            foreach($campos->getTextfields() as $key => $campo){
                $nombre_campo = $campo['nombre_campo'];
                $valor = $form_state['values'][$nombre_campo]; 
                $filtro[$key] = empty($valor) ? '' : $valor; 
            }
            foreach($campos->getDatepickers() as $key => $campo){
                $nombre_campo = $campo['nombre_campo'] . '_inicial';
                $valor = $form_state['values'][$nombre_campo]; 
                $filtro[$key . '_inicial'] = empty($valor) ? '' : Utils::dateToDMY($valor); 
                
                $nombre_campo = $campo['nombre_campo'] . '_final';
                $valor = $form_state['values'][$nombre_campo]; 
                $filtro[$key. '_final'] = empty($valor) ? '' : Utils::dateToDMY($valor); 
            }
            $parametros_consulta = (new FiltroBusquedaReportesCumplidosEntity())->fromArray($filtro);
            
            $reportes_encontrados = (new ReportesCumplidosController())->getReportesCumplidos($parametros_consulta);
            
            add_error($reportes_encontrados, 'respuesta');
            (new DataControlReportesCumplidos())->llamarGuardarDato($reportes_encontrados);
            
            break;

        case 'Limpiar':
            break;

    }
}
