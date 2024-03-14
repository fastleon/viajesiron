<?php

module_load_include('php', 'viajesiron', 'domain/entities/reporte_cumplido_entity');
module_load_include('php', 'viajesiron', 'domain/entities/filtro_busqueda_reportes_cumplidos_entity');
module_load_include('php', 'viajesiron', 'presentation/reportes_cumplidos/dependencies');

/**
 * HOOK VALIDATE
*/
function reporte_cumplidos_render_form_validate($form, &$form_state) {

    $button_clicked = $form_state['triggering_element']['#name'];
    $valores = $form_state['values'];

    switch ($button_clicked) {
        case 'Buscar':
            //TODO: falta revisar que las fechas la final sea posterior a la fecha inicial
            $campos = new Campos_formulario();
            $datepickers = $campos->getDatepickers();
            foreach($datepickers as $datepicker) {
                $campo_fecha_inicial = $datepicker['nombre_campo'] . '_inicial';
                $campo_fecha_final = $datepicker['nombre_campo'] . '_final';
                $fecha_inicial = strtotime($valores[$campo_fecha_inicial]);
                $fecha_final = strtotime($valores[$campo_fecha_final]);
                $hoy = strtotime(date('Y-m-d'));
            //fecha final posterior a la inicial
                if ($fecha_final < $fecha_inicial) {
                    form_set_error($campo_fecha_final, t('La fecha final debe ser posterior a la inicial.'));
                }
            //que ambas fechas sean anteriores a "hoy"
                if ( ($fecha_inicial > $hoy) || ($fecha_final > $hoy) ) {
                    form_set_error($campo_fecha_final, t('La fecha no puede ser posterior al día de hoy.'));
                }
            //que la diferencia de fechas no supere un mes
                $diferencia = $fecha_final - $fecha_inicial; //segundos
                $diferencia = $diferencia / ( 60*60*24 );  //dias
                if ($diferencia > 31) {
                    form_set_error($campo_fecha_final, 'La diferencia entre fechas no puede ser mayor a 30 días.');
                }  
            }
            break;
        case 'Limpiar':
            break;
        case 'Excel':
            if (count($form_state['complete form']['tbl']['tabla_resultados']['#rows']) == 0) {
                form_set_error('submit_Excel', t('No hay datos en la tabla'));
                // add_error(count($form_state['complete form']['tbl']['tabla_resultados']['#rows']));
            }
            break;
    }

}


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
                // add_error($reportes_encontrados, 'DEBUG: reportes encontrados POO');
            //guardar el resultado en sesion en modo array para mejor manejor y evitar error __PHP
            if ($reportes_encontrados) {
                (new DataControlReportesCumplidosResultados())->llamarGuardarDato($reportes_encontrados->toArray());
            } else {
                drupal_set_message(t('La busqueda no arrojo resultados'), 'info');
            }
            
            break;

        case 'Limpiar':
            //Borrar formulario
            (new DataControlReportesCumplidosFormulario())->llamarBorrarDato();
            //Borrar resultados
            (new DataControlReportesCumplidosResultados())->llamarBorrarDato();
            
            break;

        case 'Excel':
            // $valores = (new DataControlReportesCumplidosResultados())->llamarCargarDato();
            $valores = $form_state['complete form']['tbl']['tabla_resultados']['#rows'];
            add_error($valores);
            if (count($valores)) {
                $nombre_archivo = 'Reportes_cumplidos_' . date('YmdHis');
                exportar_excel_reportes_cumplidos($valores, $nombre_archivo);
            } else{
                drupal_set_message(t('No hay datos que exportar'), 'info');
            }
            break;

    }
}

/**
 * Metodo para exportar a Excel y descargar en PC del usuario
 */
function exportar_excel_reportes_cumplidos($datos_respuesta, $nombre_archivo){
    $titulo = 'ResultadoReportesCumplidos';
    /* EXPORTAR A EXCEL *************************************************/
    // Crear una instancia de la clase Spreadsheet
    $spreadsheet = new PhpOffice\PhpSpreadsheet\Spreadsheet();
    $sheet = $spreadsheet->getActiveSheet();
    $sheet->setTitle($titulo, true);

    // Datos sueltos
    //$sheet->setCellValue('A1', 'N° Entrega');
    //$sheet->setCellValue('B1', 'Cargue en plataforma');
    //$sheet->setCellValue('C1', 'Fecha SAP');
    //$sheet->setCellValue('F1', 'Fecha cargue cumplido');
    //$sheet->setCellValue('G1', 'Paqueteo/cupo');
    //$sheet->setCellValue('K1', 'Soporte entrega');
	
	$sheet->getColumnDimension('A')->setWidth(15);
	$sheet->getColumnDimension('B')->setWidth(20);
	$sheet->getColumnDimension('C')->setWidth(20);
	$sheet->getColumnDimension('D')->setWidth(25);
	$sheet->getColumnDimension('E')->setWidth(25);
	$sheet->getColumnDimension('F')->setWidth(20);
	$sheet->getColumnDimension('G')->setWidth(15);
	$sheet->getColumnDimension('H')->setWidth(15);
	$sheet->getColumnDimension('I')->setWidth(20);
	$sheet->getColumnDimension('J')->setWidth(20);
	$sheet->getColumnDimension('K')->setWidth(20);
	$sheet->getColumnDimension('L')->setWidth(30);
	$sheet->getColumnDimension('M')->setWidth(15);
    
    // Agregar datos a la tabla de prueba
    $sheet->setCellValue('A1', 'WHS Pedido');
    $sheet->setCellValue('B1', 'Creación de Remisión');
    $sheet->setCellValue('C1', 'Planeación Informe');
    $sheet->setCellValue('D1', 'Fecha Estimada de Entrega');
    $sheet->setCellValue('E1', 'Localidad de Origen');
    $sheet->setCellValue('F1', 'Prueba de Entrega');
    $sheet->setCellValue('G1', 'Tipo de Carga');
    $sheet->setCellValue('H1', 'Días de Entrega');
    $sheet->setCellValue('I1', 'Ciudad Destino');
    $sheet->setCellValue('J1', 'Zona de Ventas');
    $sheet->setCellValue('K1', 'Númerode Población');
    $sheet->setCellValue('L1', 'Línea de Transporte');
    $sheet->setCellValue('M1', 'Fotos POD/PDF');
    //$sheet->setCellValue('', '');

    $rownum = 2;
    foreach($datos_respuesta as $dato) {
        $sheet->setCellValue('A'.$rownum, $dato['whs_id']);
        $sheet->setCellValue('B'.$rownum, Utils::dateToDMY($dato['fecha_creacion']));
        $sheet->setCellValue('C'.$rownum, Utils::dateToDMY($dato['planeacion_informe']));
        $sheet->setCellValue('D'.$rownum, Utils::dateToDMY($dato['fecha_estimada_entrega']));
        $sheet->setCellValue('E'.$rownum, $dato['ciudad_origen']);
        $sheet->setCellValue('F'.$rownum, Utils::dateToDMY($dato['prueba_entrega']));
        $sheet->setCellValue('G'.$rownum, $dato['tipo_carga']);
        $sheet->setCellValue('H'.$rownum, $dato['dias_entrega']);
        $sheet->setCellValue('I'.$rownum, $dato['ciudad_destino']);
        $sheet->setCellValue('J'.$rownum, $dato['zona_ventas']);
        $sheet->setCellValue('K'.$rownum, $dato['numero_poblacion']);
        $sheet->setCellValue('L'.$rownum, $dato['transportadora']);
        $sheet->setCellValue('M'.$rownum, $dato['fotos']);
        // $sheet->setCellValue('L'.$rownum, $dato['']);
        $rownum += 1;
    }
        
    /*/RUTA PRUEBA TABLA - Guardado "Local Drupal"
    $module_path = drupal_get_path('module', 'viajesiron');
    // Construir la ruta completa de la subcarpeta "excel"
    $excel_folder_path = $module_path . '/excel';
    $excel_file_path = $excel_folder_path . '/tabla_prueba.xlsx';*/
    //$writer = new PhpOffice/PhpSpreadsheet/Writer/Xlsx($spreadsheet); //, 'Excel2007'
    //$writer->save($excel_file_path);
    
    ob_end_clean();
    header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
    header('Content-Disposition: attachment;filename="' . $nombre_archivo . '.xlsx"');
    //header('Content-Disposition: attachment;filename="filename_' . time() . '.xlsx"');
    header('Cache-Control: max-age=0');

    $xlsxWriter = PhpOffice\PhpSpreadsheet\IOFactory::createWriter($spreadsheet, 'Xlsx');
    $xlsxWriter = new PhpOffice\PhpSpreadsheet\Writer\Xlsx($spreadsheet);
    exit($xlsxWriter->save('php://output'));
}
