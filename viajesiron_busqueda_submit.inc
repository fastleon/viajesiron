<?php

function formulario_busqueda_viajes_form_validate($form, &$form_state) {

    $button_clicked = $form_state['triggering_element']['#name'];
    $valores = $form_state['values'];
    
    switch ($button_clicked){
        case 'Buscar':
            add_error('opcion de busqueda');
            add_error($valores['opcion_busqueda']);
            //Revisar parametros
            if ($valores['opcion_busqueda'] == 'Por Nombre'){ //por nombre
                if ($valores['nombre'] == ''){
                    form_set_error('nombre', 'Debe escribir algun valor.');
                }
            } else { //por fecha
                $fecha_inicial = strtotime($valores['fecha_inicial']);
                $fecha_final = strtotime($valores['fecha_final']);
                $diferencia = $fecha_final - $fecha_inicial; //segundos
                $diferencia = $diferencia / ( 60*60*24 );  //dias
                if ($fecha_final < $fecha_inicial){
                    form_set_error('fecha_final', 'La fecha final debe ser posterior o igual a la inicial.');

                } elseif ($diferencia > 31){
                    form_set_error('fecha_final', 'La fecha final no puede ser mayor a 30 días.');
                }            
            }
            break;
        case 'Limpiar':
            break;
        case 'Excel':
            break;
        }

}

function formulario_busqueda_viajes_form_submit($form, &$form_state) {
    if (strstr($form_state['values']['opcion_busqueda'], 'Fecha') !== FALSE){ //por fechas
        $opcion = 'Fechas';
    }else {//por nombre        
        $opcion = 'Nombre';
    }

    //salvar actual estado
    $state = array();
    $state['opcion_busqueda'] = $opcion;
    if ($state['opcion_busqueda'] == 'Fechas'){ //Fechas
        $state['transportadora'] = $form_state['values']['transportadora'];
        $state['fecha_inicial'] = $form_state['values']['fecha_inicial'];
        $state['fecha_final'] = $form_state['values']['fecha_final'];
        unset($state['nombre']);
    } elseif ($state['opcion_busqueda'] == 'Nombre'){ //Nombre
        unset($state['transportadora']);
        unset($state['fecha_inicial']);
        unset($state['fecha_final']);
        $state['nombre'] = $form_state['values']['nombre'];
    }
    $_SESSION['viajesiron_busqueda']['state'] = $state;

    $button_clicked = $form_state['triggering_element']['#name'];

    switch ($button_clicked){
        case 'Buscar':
            if ($_SESSION['viajesiron_busqueda']['state']['opcion_busqueda'] == 'Fechas'){ //Fechas
                $params = array(
                    'fechaInicial' => $state['fecha_inicial'], 
                    'fechaFinal' => $state['fecha_final'], 
                    'idTransportadora' => $state['transportadora'],
                );
                add_error('revisando parametros');
                add_error($params);
                $response = buscar_viaje_por_fechas($params);
            } elseif($_SESSION['viajesiron_busqueda']['state']['opcion_busqueda'] == 'Nombre'){ //Nombre
                $params = array(
                    'nombre' => $state['nombre'],
                );
                $response = buscar_viaje_por_nombre($params);
            }
            if (isset($response['reporteViajesDTO']) && $response['reporteViajesDTO'] != array()){
                $_SESSION['viajesiron_busqueda']['data'] = $response;
            } else{
                $_SESSION['viajesiron_busqueda']['data'] = FALSE;
                drupal_set_message(t('No hay resultados para esta busqueda'), 'info');
            }
            break;
        case 'Limpiar':
            unset($_SESSION['viajesiron_busqueda']);
            break;
        case 'Excel':
            if (isset($_SESSION['viajesiron_busqueda']['data']) && ($_SESSION['viajesiron_busqueda']['data'])){
                if ($opcion == 'Fechas'){
                    $valor_fecha_inicial = 'fi_';
                    $valor_fecha_final = 'ff_';
                    $valor_transportadora = 'tr_' . $state['transportadora'];
                    $timestamp = strtotime($state['fecha_inicial']);
                    if ($timestamp !== false) {
                        $valor_fecha_inicial = date('Ymd', $timestamp);
                    }
                    $timestamp = strtotime($state['fecha_final']);
                    if ($timestamp !== false) {
                        $valor_fecha_final = date('Ymd', $timestamp);
                    }
                    $nombre_archivo = 'viajes_de_' . $valor_fecha_inicial . '_a_' . $valor_fecha_final . '_' . $valor_transportadora;
                } elseif($opcion == 'Nombre') {
                    $nombre_archivo = 'viaje_' . $state['nombre'];
                } else {
                    $nombre_archivo = 'filename_' . time();
                }
                exportar_excel($_SESSION['viajesiron_busqueda']['data'], $nombre_archivo);
            } else{
                drupal_set_message(t('No hay datos que exportar'), 'info');
            }
            break;
        }
}


/**
 * Metodo para exportar a Excel y descargar en PC del usuario
 */
function exportar_excel($datos_respuesta, $nombre_archivo){
    $titulo = 'ResultadoBusqueda';
    /* EXPORTAR A EXCEL *************************************************/
    // Crear una instancia de la clase Spreadsheet
    $spreadsheet = new PhpOffice\PhpSpreadsheet\Spreadsheet();
    $sheet = $spreadsheet->getActiveSheet();
    $sheet->setTitle($titulo, true);
    
    // Agregar datos a la tabla de prueba
    $sheet->setCellValue('A1', 'Area Entrega');
    $sheet->setCellValue('B1', 'Ciudad Origen');
    $sheet->setCellValue('C1', 'Costo Flete');
    $sheet->setCellValue('D1', 'Costo Total Flete');
    $sheet->setCellValue('E1', 'Departamento Entrega');
    $sheet->setCellValue('F1', 'Departamento Origen');
    $sheet->setCellValue('G1', 'Direccion Empresa Receptora');
    $sheet->setCellValue('H1', 'Empresa Receptora');
    $sheet->setCellValue('I1', 'Fecha Entrega Calculada');
    $sheet->setCellValue('J1', 'Zona de Ventas');
    $sheet->setCellValue('K1', 'Número de Población');
    $sheet->setCellValue('L1', 'Nombre Viaje');
    $sheet->setCellValue('M1', 'NúmeroRemision');
    $sheet->setCellValue('N1', 'Peso Bruto');
    $sheet->setCellValue('O1', 'Peso Total Flete');
    $sheet->setCellValue('P1', 'Tipo De Carga');
    $sheet->setCellValue('Q1', 'Tipo De Carro');
    $sheet->setCellValue('R1', 'Transportadora');
    $sheet->setCellValue('S1', 'Unidad');
    
    $rownum = 2;
    foreach($datos_respuesta['reporteViajesDTO'] as $dato){
        $sheet->setCellValue('A'.$rownum, $dato['areaEntrega']);
        $sheet->setCellValue('B'.$rownum, $dato['ciudadOrigen']);
        if (!isset($dato['costoFlete'])){
            $dato['costoFlete'] = 0;
        }
        $sheet->setCellValue('C'.$rownum, $dato['costoTotalRemision']);
        $sheet->setCellValue('D'.$rownum, $dato['costoTotalViaje']);
        $sheet->setCellValue('E'.$rownum, $dato['departamentoEntrega']);
        $sheet->setCellValue('F'.$rownum, $dato['departamentoOrigen']);
        $sheet->setCellValue('G'.$rownum, $dato['direccionEmpresaReceptora']);
        $sheet->setCellValue('H'.$rownum, $dato['empresaReceptora']);
        $sheet->setCellValue('I'.$rownum, $dato['fechaEntregaCalculada']);
        $sheet->setCellValue('J'.$rownum, $dato['zonaVentas']);
        $sheet->setCellValue('K'.$rownum, $dato['numeroPoblacion']);
        $sheet->setCellValue('L'.$rownum, $dato['nombreViaje']);
        $sheet->setCellValue('M'.$rownum, $dato['numeroRemision']);
        $sheet->setCellValue('N'.$rownum, $dato['pesoBruto']);
        $sheet->setCellValue('O'.$rownum, $dato['pesoTotalViaje']);
        $sheet->setCellValue('P'.$rownum, $dato['tipoDeCarga']);
        $sheet->setCellValue('Q'.$rownum, $dato['tipoDeCarro']);
        $sheet->setCellValue('R'.$rownum, $dato['transportadora']);
        $sheet->setCellValue('S'.$rownum, $dato['unidad']);
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
