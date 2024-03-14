<?php
function datos_viaje_formulario_form_validate($form, &$form_state) {
  // Obtener el botón de envío que fue presionado
  $button_clicked = $form_state['triggering_element']['#name'];
  $data = bring_data();

  //verificar cambio en tipo carga
  if (isset($form_state['values']['tipo_carga'])) {
    if ($form_state['values']['tipo_carga'] == 'Paqueteo') {
    //ignorar
    } else {
      $aguja = $form_state['values']['tipo_carro'];
      $rel_carga_carro = llamar_variable_tipo_carga();
      $pajar = $rel_carga_carro[$form_state['values']['tipo_carga']];
      if(in_array($aguja, $pajar)) {
        //ok
      } else {
        form_set_error('tipo_carro', 'verifique que la opción Tipo de Carro sea la correcta');
      }
    }
  }

  // Verificar cuál botón fue presionado
  switch ($button_clicked) { 

    case 'submit_guardar_viaje':
      $is_save_ok = TRUE;

      //Confirmar que existe al menos una remision en el viaje
      if (count($data['remisiones']) < 1){
        drupal_set_message(t('Debe tener al menos una remision anexar para poder crear un viaje'), 'error');
        $is_save_ok = FALSE;
      }

      $_SESSION['viajesiron_save_ok'] = $is_save_ok;
      break;

    case 'submit_agregar_adicional'://BOTON AGREGAR ADICIONAL
      //revisar que los campos no esten vacios
      if (empty($form_state['values']['campo_tipo_adicional_agregar'])) {
        form_set_error('campo_tipo_adicional_agregar', 'El campo no puede estar vacío.');
      }
      if (empty($form_state['values']['campo_costo_adicional_agregar'])) {
        form_set_error('campo_costo_adicional_agregar', 'El campo no puede estar vacío.');
      }
      if ((!preg_match('/^[0-9]+$/', $form_state['values']['campo_costo_adicional_agregar']))) {
        form_set_error('campo_costo_adicional_agregar', 'El campo debe ser numérico sin caracteres');
      }
      break; 

    case 'submit_eliminar_adicional'://BOTON ELIMINAR ADICIONAL
      //Verificar que hay alguna opcion seleccionada
      $tabla_info = $form_state['input']['tabla_adicionales'];
      
      if (!is_numeric($tabla_info)){
        form_set_error('tabla_adicionales', 'No hay adicionales para eliminar.');
      }
      break; 

    case 'submit_anexar_remision': //BOTON ANEXAR REMISION
      //Revisar si existió una busqueda
      if (isset($data['busqueda']) && $data['busqueda'] != array()) {
      } else {
        form_set_error('tabla_remisiones', 'Primero realiza la busqueda de una remision para poder anexarla');
      }
      break;

    case 'submit_eliminar_remision'://BOTON ELIMINAR REMISION
      //Verificar que hay alguna opcion seleccionada
      $tabla_info = $form_state['input']['tabla_remisiones'];
      $no_items = TRUE;
      foreach ($tabla_info as $ids_selected) {
        if (is_numeric($ids_selected)){
          $no_items = FALSE;
        }
      }
      if ($no_items) {
        form_set_error('tabla_remisiones', 'No hay remisiones seleccionadas para eliminar.');
      }
      break;

    default:
      //drupal_set_message('No tengo idea que se presiono');
      break;
    }

  }

function datos_viaje_formulario_form_submit($form, &$form_state) {
  guardar_informacion($form_state);
  $data = bring_data();
  //Realizar logica según el boton presionado
  $button_clicked = $form_state['triggering_element']['#name'];

  switch ($button_clicked) {

    case 'submit_limpiar': //LIMPIAR FORMULARIO
      datos_en_blanco();
      $data = bring_data();
      drupal_set_message(t('limpiando información'), 'info');
      $url_redirect = 'viajes';
      drupal_goto($url_redirect);
      break;

    case 'submit_guardar_viaje': //GUARDAR VIAJE
      if (isset($_SESSION['viajesiron_save_ok'])) {
        $is_save_ok = $_SESSION['viajesiron_save_ok'];
      } else {
        $is_save_ok = FALSE;
      }
      if ($is_save_ok){
        //llamamos la funcion para manejar la informacion y llamar el servicio correspondiente
        $response = guardar_viaje($data);
        if ($response){
          drupal_set_message(t('El viaje ha sido guardado correctamente'), 'status');
          datos_en_blanco(); //Borrar formulario
          $_SESSION['viajesiron_data_reporte'] = array(); //eliminar posibles datos en reporte antiguos
          $url_redirect = 'viajes/' . $response;
          drupal_goto($url_redirect);
        } else {
          drupal_set_message(t('El viaje no pudo ser guardado, verifique conexión con la red y que los datos estén correctos'), 'error');
        }
      }
      break;

    case 'submit_agregar_adicional':
      $temp_id = $form_state['values']['campo_tipo_adicional_agregar'];
      $adicional = array(
        'id' => $temp_id,
        'concepto' => $_SESSION['viajesiron_conceptos_adicionales'][$temp_id],
        'costo' => $form_state['values']['campo_costo_adicional_agregar'],
      );
      //guardamos la información actual
      $data['adicionales'][] = $adicional;
      drupal_set_message ('Costo adicional agregado');
      break; 

    case 'submit_eliminar_adicional':
      // Obtener opcion del select y eliminar ese dato de la variable temporal
      $tabla_info = $form_state['input']['tabla_adicionales'];
      if (is_numeric($tabla_info)){
        unset($data['adicionales'][$tabla_info]);
        $data['adicionales'] = array_values($data['adicionales']);
        variable_set('viajesiron_temporal_data', $data);
      }
      break; 

    case 'submit_anexar_remision':
      $no_anexados = array();
      foreach ($data['busqueda'] as $resultado){
        if (is_null($resultado['viaje']) && $resultado['sin_errores']){
          if ($resultado != array()) {
            //Buscar por duplicados
            $repetido = FALSE;
            foreach ($data['remisiones'] as $remision){
              if ($remision['remision'] == $resultado['id']){
                $repetido = TRUE;
              }
            }
            //duplicado
            if ($repetido == TRUE){
              drupal_set_message(t('El WHS @whs que desea anexar ya se encuentra incluido en el viaje actual', array('@whs' => $resultado['id'])),'info');
            } else {
              //no duplicado
              $new_remision = array(
                'remision' => $resultado['id'],
              );
              $data_plus = array(
                'poblacion_expedidor' => $resultado['info']['poblacionExpedidor'],
                'direccion_expedidor' => $resultado['info']['direccionExpedidor'],
                'poblacion_receptor' => $resultado['info']['poblacionReceptor'],
                'direccion_receptor' => $resultado['info']['direccionReceptor'],
                'zonaVentas' => $resultado['info']['zonaVentas'],
                'numeroPoblacion' => $resultado['info']['numeroPoblacion'],
                'fecha_entrega' => $resultado['info']['fechaEntregaPlanificadaFormato'],
                'peso' => $resultado['info']['pesoBruto'],
                'unidad_peso' => $resultado['info']['unidadPesoBruto'],
              );
              //sumamos el peso anexado al viaje
              if ($resultado['info']['unidadPesoBruto'] == 'KG') {
                $data['peso_remisiones_kg'] += $resultado['info']['pesoBruto'];
              } else {
                $data['peso_remisiones_kg'] += ($resultado['info']['pesoBruto'] / 2);
              }
              
              //guardamos la información actual
              $data['remisiones'][] = array_merge($new_remision, $data_plus);
              drupal_set_message (t('Remisión @whs anexada', array('@whs' => $resultado['id'])), 'status');

            }
          }
        } else{
          $no_anexados[] = $resultado['id'];
        }
      }
      if ($no_anexados != array()){
        drupal_set_message (t('Las remisiones : @remisiones no fueron anexadas porque ya pertenecen a otro viaje o tienen errores.', array('@remisiones' => implode(', ', $no_anexados))), 'info');
      }
      $data['busqueda'] = array();

      break;

    case 'submit_eliminar_remision':
      // Obtener opcion del select y eliminar ese dato de la variable temporal
      $tabla_info = $form_state['input']['tabla_remisiones'];
      foreach ($tabla_info as $id_remision_borrar) {
        if (is_numeric($id_remision_borrar)){
          if ($data['remisiones'][$id_remision_borrar]['unidad_peso'] == 'KG') {
            $data['peso_remisiones_kg'] -= $data['remisiones'][$id_remision_borrar]['peso'];
          } else {
            $data['peso_remisiones_kg'] -= ($data['remisiones'][$id_remision_borrar]['peso'] / 2);
          }
          unset($data['remisiones'][$id_remision_borrar]);
        }
      }
      $data['remisiones'] = array_values($data['remisiones']);
      break;

    case 'submit_buscar_whs':
      //buscar el WHS y guardarlo en como 'busqueda'
      $whss = $form_state['values']['campo_whs'];
      //pasar de espacios a una coma
      $whss = preg_replace('/\s+/', ' ', trim($whss));
      $whss = str_replace(' ', ',', $whss);
      $data = buscar_varios_whs_por_id($whss, $data);
      break;

    default:
      //drupal_set_message('No tengo idea que se presiono');
      break;
  }

  /*/guardar cambios
  if (!isset($data['id'])) {
    $data['id'] = 'new'; //para distingirlo de datos de prueba - MODO DESARROLLADOR (no afecta)
  } */
  send_data($data);
}


function guardar_informacion(&$form_state){
  //guardar los campos para que no se pierda el cambio
  $data = bring_data();
  $data['nombre'] = $form_state['values']['nombre'];
  $data['tipo_carga'] = $form_state['values']['tipo_carga'];
  if ($form_state['values']['tipo_carga'] == 'Paqueteo') {
    $data['tipo_carro'] = '';
  }else {
    $data['tipo_carro'] = $form_state['values']['tipo_carro'];
  }
  $data['transportadora'] = $form_state['values']['transportadora'];
  if (!isset($data['id'])) {
    $data['id'] = 'new'; //para distingirlo de datos de prueba - MODO DESARROLLADOR (no afecta)
  } 
  send_data($data);
}