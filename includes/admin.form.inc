<?php
/**
 * @file
 * Houses the administration for the proai module.
 */

include_once 'rels_ext_utilities.inc';

/**
 * Creates the main administration form for the proai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the proai administration form.
 */
function proai_admin_form($form, $form_state) {
  //get configuration info
  $config = _proai_config_text();
  $prefix = variable_get('islandora_proai_prefix','');
  $form['islandora_proai_enable'] = array(
    '#type' => 'fieldset',
    '#title' => t('PROAI configuration'),
  );
  $form['islandora_proai_enable']['islandora_oai_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('OAI prefix'),
    '#size' => 80,
    '#weight' => -1,
    '#default_value' => $prefix,
    '#description' => t('The prefix for your institution. After changing the prefix purge and ingest aux:xslt.'),
  );
  $form['islandora_proai_enable']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit OAI Prefix'),
    '#weight' => 10,
  );
  
  $form['islandora_proai_required'] = array(
    '#type' => 'fieldset',
    '#title' => t('PROAI required objects'),
    '#prefix' => $config,
  );
  $form['islandora_proai_required']['ingest_all'] = array(
    '#type' => 'submit',
    '#value' => t('Ingest required objects'),
    '#weight' => 10,
  );
  $form['islandora_proai_required']['purge_all'] = array(
    '#type' => 'submit',
    '#value' => t('Purge required objects'),
    '#weight' => 20,
  );
  $form['islandora_proai_required']['update_objects'] = array(
    '#type' => 'submit',
    '#value' => t('Update objects (max 50)'),
    '#weight' => 30,
  );
  $form['islandora_proai_required']['sdep'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage sdep:oai'),
    '#weight' => 40,
  );
  $form['islandora_proai_required']['sdep']['sdep_purge'] = array(
    '#type' => 'submit',
    '#value' => t('Purge sdep:oai'),
    '#weight' => 50,
  );
  $form['islandora_proai_required']['sdep']['sdep_ingest'] = array(
    '#type' => 'submit',
    '#value' => t('Ingest sdep:oai'),
    '#weight' => 60,
  );
  $form['islandora_proai_required']['xslt'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manage aux:xslt'),
    '#weight' => 70,
  );
  $form['islandora_proai_required']['xslt']['xslt_purge'] = array(
    '#type' => 'submit',
    '#value' => t('Purge aux:xslt'),
    '#weight' => 80,
  );
  $form['islandora_proai_required']['xslt']['xslt_ingest'] = array(
    '#type' => 'submit',
    '#value' => t('Ingest aux:xslt'),
    '#weight' => 90,
  );
  
  return $form;
}

/**
 * Submit handler for the main administration form for the proai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function proai_admin_form_submit($form, $form_state) {
  if ($form_state['clicked_button']['#value'] == 'Submit OAI Prefix') {
    variable_del('islandora_proai_prefix');
    $prefix = $form_state['values']['islandora_oai_prefix'];
    $prefix = trim($prefix);
    $prefix = preg_replace('/(.*):$/', '$1', $prefix);
    if ($prefix && (strlen($prefix) > 0)) {
      variable_set('islandora_proai_prefix',$prefix);
    }
  }
  else if ($form_state['clicked_button']['#value'] == 'Ingest required objects') {
    $message = islandora_proai_ingest_objects();
  }
  else if ($form_state['clicked_button']['#value'] == 'Purge required objects') {
    $message = islandora_proai_purge_objects();
  }
  else if ($form_state['clicked_button']['#value'] == 'Update objects (max 50)') {
    $message = islandora_proai_pids();
  }
  else if ($form_state['clicked_button']['#value'] == 'Ingest aux:xslt') {
    $message = islandora_proai_ingest_objects('aux:xslt');
  }
  else if ($form_state['clicked_button']['#value'] == 'Purge aux:xslt') {
    $message = islandora_proai_purge_objects('aux:xslt');
  }
  else if ($form_state['clicked_button']['#value'] == 'Ingest sdep:oai') {
    $message = islandora_proai_ingest_objects('sdep:oai');
  }
  else if ($form_state['clicked_button']['#value'] == 'Purge sdep:oai') {
    $message = islandora_proai_purge_objects('sdep:oai');
  }
}

/**
 * Creates a table after getting the configuration.
 *
 */
function _proai_config_text() {
  $configuration = islandora_proai_get_configuration();
  $table = array(
    'empty' => '',
    'sticky' => FALSE,
    'colgroups' => array(),
    'caption' => 'Configuration status:',
    'header'=>array(),
    'rows'=>array(),
    'attributes'=>array('class'=>'islandora_proai_table')
  );
  $prefix = variable_get('islandora_proai_prefix','No prefix defined.');
  $table['rows'][] =  array(array('data'=>'OAI prefix','header'=>TRUE),array('data'=>'Value','header'=>TRUE));
  $table['rows'][] =  array('prefix',$prefix);

  $table['rows'][] =  array(array('data'=>'Required object','header'=>TRUE),array('data'=>'Status','header'=>TRUE));
  foreach ($configuration['required'] as $k=>$v) {
    $table['rows'][] = array($k,$v);
  }  
  $table['rows'][] =  array(array('data'=>'Content model','header'=>TRUE),array('data'=>'Status','header'=>TRUE));
  foreach ($configuration['cmodels'] as $k=>$v) {
    $table['rows'][] = array($k,$v);
  }  
  $table['rows'][] =  array(array('data'=>'Collection','header'=>TRUE),array('data'=>'Set name','header'=>TRUE));
  foreach ($configuration['collections'] as $k=>$v) {
    $table['rows'][] = array($k,$v);
  }  
  
  return theme_table($table);
}

/**
 * Creates the content models administration form for the proai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the proai administration form.
 */
function proai_cmodels_form($form, &$form_state) {
  module_load_include('inc', 'islandora', 'includes/content_model.autocomplete');
  $cmodel_to_add = FALSE;
  $cmodels_saved = variable_get('islandora_proai_cmodels', array());
  $cmodels_associated = [];
  foreach ($cmodels_saved as $cm) {
    $cmodels_associated[] = array('cmodel'=>$cm);
  }

  //handle ajax callback
  if (isset($form_state['triggering_element'])) {

    //add cmodel
    if ($form_state['triggering_element']['#name'] == 'islandora-proai-cmodels-add-cmodel') {
      $cmodel_to_add = array('cmodel'=>$form_state['values']['islandora_proai_cmodels']['table_wrapper']['cmodel_options']['cmodel_select']);
    }
    //delete model
    if ($form_state['triggering_element']['#name'] == 'islandora-proai-cmodels-remove-selected') {
      foreach ($form_state['values']['islandora_proai_cmodels']['table_wrapper']['table'] as $key => $row) {
        if ($row !== 0) {
          unset($form_state['complete form']['islandora_proai_cmodels']['table_wrapper']['table']['#options'][$key]);
        }
      }
    }
  }

  $form = array(
    '#tree' => TRUE,
  );
  $form['islandora_proai_cmodels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Models'),
    '#collapsed' => TRUE,
    '#collapsible' => FALSE,
  );
  $form['islandora_proai_cmodels']['table_wrapper'] = array(
    '#prefix' => '<div id="islandora-proai-cmodels-wrapper">',
    '#suffix' => '</div>',
  );

  // If there are values in the form_state use these. Could be changed by the AJAX callbacks
  if (isset($form_state['values'])) {
    if (isset($form_state['values']['islandora_proai_cmodels']['table_wrapper']['table'])) {
      $cmodels_associated = $form_state['complete form']['islandora_proai_cmodels']['table_wrapper']['table']['#options'];
    }
  }

  if ($cmodel_to_add !== FALSE) {
    $cmodels_associated[]=$cmodel_to_add;
  }

  $form['islandora_proai_cmodels']['table_wrapper']['table'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'cmodel' => array('data' => t('Content Model Name')),
    ),
    '#options' => $cmodels_associated,
    '#empty' => t('No content models associated.'),
  );

  if (count($cmodels_associated)) {
    $form['islandora_proai_cmodels']['table_wrapper']['remove_selected'] = array(
      '#type' => 'button',
      '#value' => t('Remove selected'),
      '#name' => 'islandora-proai-cmodels-remove-selected',
      '#ajax' => array(
        'callback' => 'islandora_proai_cmodels_ajax',
        'wrapper' => 'islandora-proai-cmodels-wrapper',
      ),
    );
  }
  
  // Retrieve all content models and unset those currently in use in this
  // configuration and any others from other configurations.
  $add_options = islandora_get_content_model_names();
  foreach ($cmodels_associated as $entry) {
    unset($add_options[$entry['cmodel']]);
  }

  if (!empty($add_options)) {
    $form['islandora_proai_cmodels']['table_wrapper']['cmodel_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add content model'),
      '#collapsed' => TRUE,
      '#collapsible' => FALSE,
    );

    $form['islandora_proai_cmodels']['table_wrapper']['cmodel_options']['cmodel_select'] = array(
      '#type' => 'select',
      '#options' => $add_options,
    );
    $form['islandora_proai_cmodels']['table_wrapper']['cmodel_options']['cmodel_add'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#name' => 'islandora-proai-cmodels-add-cmodel',
      '#ajax' => array(
        'callback' => 'islandora_proai_cmodels_ajax',
        'wrapper' => 'islandora-proai-cmodels-wrapper',
      ),
    );
  }
  $form['islandora_proai_cmodels']['islandora_proai_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 10,
  );
  
  return $form;
}

/**
 * Submit handler for the content models administration form for the proai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function proai_cmodels_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Save configuration') {
    $cmodels_saved  = variable_get('islandora_proai_cmodels', array());

    $cmodels_options = $form_state['complete form']['islandora_proai_cmodels']['table_wrapper']['table']['#options'];
    $cmodels_new = array();
    foreach ($cmodels_options as $co) {
      foreach ($co as $k=>$v) {
        $cmodels_new[] = $v;
      }
    }
    //set new list
    variable_set('islandora_proai_cmodels',$cmodels_new);
    //update Fedora
    $message = islandora_proai_update_content_models($cmodels_saved, $cmodels_new);
  }
}


/**
 * Creates the collections administration form for the proai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the form state.
 *
 * @return array
 *   A renderable array representing the proai administration form.
 */
function proai_collections_form($form, &$form_state) {
  $collection_to_add = FALSE;
  $collections_saved = variable_get('islandora_proai_collections', array());

  $collections_associated = array();
  foreach ($collections_saved as $coll => $set) {
    $collections_associated[] = array(
      'collection'=> $coll,
      'setname'=>array(
        'data'=> array(
          '#type' => 'textfield',
          '#title' => 'Set Name for '.$coll,
          '#title_display'=> 'invisible',
          '#value'=> $set,
          '#name' => 'setname['.$coll.']',
        )
      )
    );
  }

  //handle ajax callback
  if (isset($form_state['triggering_element'])) {
    //add collection
    if ($form_state['triggering_element']['#name'] == 'islandora-proai-collections-add-collection') {
      $collection_value = $form_state['values']['islandora_proai_collections']['table_wrapper']['collection_options']['collection_select'];
      $collection_to_add = array(
        'collection'=> $collection_value,
        'setname'=>array(
          'data'=> array(
            '#type' => 'textfield',
            '#title' => 'Set Name for '.$collection_value,
            '#title_display'=> 'invisible',
            '#default_value'=> '',
            '#name' => 'setname['.$collection_value.']',
          )
        )
      );
    }
    //delete collection
    if ($form_state['triggering_element']['#name'] == 'islandora-proai-collections-remove-selected') {
      foreach ($form_state['values']['islandora_proai_collections']['table_wrapper']['table'] as $key => $row) {
        if ($row !== 0) {
          $coll = $form_state['complete form']['islandora_proai_collections']['table_wrapper']['table']['#options'][$key]['collection'];
          unset($form_state['values']['setname'][$coll]);
          unset($form_state['complete form']['islandora_proai_collections']['table_wrapper']['table']['#options'][$key]);
        }
      }
    }
  }

  $form = array(
    '#tree' => TRUE,
  );
  $form['islandora_proai_collections'] = array(
    '#type' => 'fieldset',
    '#title' => t('Collections'),
    '#collapsed' => TRUE,
    '#collapsible' => FALSE,
  );
  $form['islandora_proai_collections']['table_wrapper'] = array(
    '#prefix' => '<div id="islandora-proai-collections-wrapper">',
    '#suffix' => '</div>',
  );

  // If there are values in the form_state use these. Could be changed by the AJAX callbacks
  if (isset($form_state['values'])) {
    if (isset($form_state['values']['islandora_proai_collections']['table_wrapper']['table'])) {
      //$collections_associated = $form_state['complete form']['islandora_proai_collections']['table_wrapper']['table']['#options'];
      $collections_form = $form_state['values']['setname'];  
      $collections_associated = array();
      foreach ($collections_form as $coll => $set) {
        $collections_associated[] = array(
          'collection'=> $coll,
          'setname'=>array(
            'data'=> array(
              '#type' => 'textfield',
              '#title' => 'Set Name for '.$coll,
              '#title_display'=> 'invisible',
              '#value'=> $set,
              '#name' => 'setname['.$coll.']',
            )
          )
        );
      }
    }
  }

  if ($collection_to_add !== FALSE) {
    $collections_associated[] = $collection_to_add;
  }
  $form['islandora_proai_collections']['table_wrapper']['table'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'collection' => array('data' => t('Collection Name')),
      'setname' => array('data' => t('Set Name')),
    ),
    '#options' => $collections_associated,
    '#empty' => t('No collections associated.'),
  );

  if (count($collections_associated)) {
    $form['islandora_proai_collections']['table_wrapper']['remove_selected'] = array(
      '#type' => 'button',
      '#value' => t('Remove selected'),
      '#name' => 'islandora-proai-collections-remove-selected',
      '#ajax' => array(
        'callback' => 'islandora_proai_collections_ajax',
        'wrapper' => 'islandora-proai-collections-wrapper',
      ),
    );
  }
  
  // Retrieve all collections and unset those currently in use in this
  // configuration and any others from other configurations.
  $root_collection = variable_get('islandora_repository_pid', 'islandora:root');
  $add_options = array();
  $add_options = islandora_proai_get_collections($root_collection,$add_options,' ');
  foreach ($collections_associated as $entry) {
    unset($add_options[$entry['collection']]);
  }

  if (!empty($add_options)) {
    $form['islandora_proai_collections']['table_wrapper']['collection_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add collection'),
      '#collapsed' => TRUE,
      '#collapsible' => FALSE,
    );
    $form['islandora_proai_collections']['table_wrapper']['collection_options']['collection_select'] = array(
      '#type' => 'select',
      '#options' => $add_options,
    );
    $form['islandora_proai_collections']['table_wrapper']['collection_options']['collection_add'] = array(
      '#type' => 'button',
      '#value' => t('Add'),
      '#name' => 'islandora-proai-collections-add-collection',
      '#ajax' => array(
        'callback' => 'islandora_proai_collections_ajax',
        'wrapper' => 'islandora-proai-collections-wrapper',
      ),
    );
  }
  $form['setname'] = array(
    '#type' => 'value',
  ); 
  $form['islandora_proai_collections']['islandora_proai_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 10,
  );
  
  return $form;
  
}

/**
 * Submit handler for the main administration form for the proai module.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function proai_collections_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Save configuration') {
    $collections_saved  = variable_get('islandora_proai_collections',  array());
    $collections_new = $form_state['values']['setname'];
    //set new list
    variable_set('islandora_proai_collections',$collections_new);
    //update Fedora
    $message = islandora_proai_update_collections($collections_saved,$collections_new);
  }
}

/**
 * AJAX callback for the content models portion of the configuration form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array representing the Drupal form state.
 *
 * @return array
 *   An array representing the portion of the form we are rendering.
 */
function islandora_proai_cmodels_ajax(&$form, &$form_state) {
  return $form['islandora_proai_cmodels']['table_wrapper'];
}

/**
 * AJAX callback for the collections portion of the configuration form.
 *
 * @param array $form
 *   An array representing a Drupal form.
 * @param array $form_state
 *   An array representing the Drupal form state.
 *
 * @return array
 *   An array representing the portion of the form we are rendering.
 */
function islandora_proai_collections_ajax(&$form, &$form_state) {
  return $form['islandora_proai_collections']['table_wrapper'];
}
