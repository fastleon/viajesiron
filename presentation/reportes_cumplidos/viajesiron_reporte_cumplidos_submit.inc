<?php

module_load_include('php', 'viajesiron', 'domain\entities\reporte_cumplido_entity');
module_load_include('php', 'viajesiron', 'domain\entities\filtro_busqueda_reportes_cumplidos_entity');
module_load_include('php', 'viajesiron', 'presentation\reportes_cumplidos\dependencies');

/**
 * HOOK SUBMIT
*/
function reporte_cumplidos_render_form_submit($form, &$form_state) {

    
    //Realizar logica según el boton presionado
    $button_clicked = $form_state['triggering_element']['#name'];

    switch ($button_clicked) {

        case 'Buscar':

            $campos = new Campos_formulario();
            //crear estructura de los fitros a consultar
            $filtro = array();
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
            //guardar estado actual
            (new DataControlReportesCumplidosFormulario())->llamarGuardarDato($filtro);
            
            //pasar datos a la estructura definida y enviar la solicitud de la consulta
            $parametros_consulta = (new FiltroBusquedaReportesCumplidosEntity())->fromArray($filtro);
            $reportes_encontrados = (new ReportesCumplidosController())->getReportesCumplidos($parametros_consulta);
            //guardar el resultado en sesion en modo array para mejor manejor y evitar error __PHP
            (new DataControlReportesCumplidosResultados())->llamarGuardarDato($reportes_encontrados->toArray());
            
            break;

        case 'Limpiar':
            //Borrar formulario

            //Borrar resultados
            (new DataControlReportesCumplidosFormulario())->llamarBorrarDato();
            (new DataControlReportesCumplidosResultados())->llamarBorrarDato();
            break;

    }
}
