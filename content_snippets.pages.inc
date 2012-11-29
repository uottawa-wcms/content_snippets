<?php


function content_snippets_page_list_types() {
  $types = content_snippets_types_get_all();
  $content = array();
  foreach ($types as $type) {
    $content[] = array(
      'title' => $type->label,
      'href' => 'admin/structure/content_snippets/manage/' . $type->bundle_name,
      'description' => !empty($type->description) ? $type->description : '',
      'localized_options' => array(),
    );
  }
  return theme('admin_block_content', array('content' => $content));
}

function content_snippets_type_form($form, &$form_state, $snippet_type = NULL) {
  if (empty($snippet_type)) {
    $snippet_type = array();
  } else {
    $snippet_type = (array) $snippet_type;
  }
  $snippet_type += array(
    'bundle_name' => '',
    'label' => '',
  );
  $form += array(
    'is_new' => array(
      '#type' => 'value',
      '#value' => empty($snippet_type['bundle_name']),
    ),
    'label' => array(
      '#title' => t('Label'),
      '#maxlength' => 255,
      '#type' => 'textfield',
      '#required' => TRUE,
      '#default_value' => $snippet_type['label'],
    ),
    'bundle_name' => array(
      '#title' => t('Machine Name'),
      '#machine_name' => array(
        'exists' => 'content_snippets_type_exists',
        'source' => array('label'),
      ),
      '#maxlength' => 32,
      '#type' => 'machine_name',
      '#default_value' => $snippet_type['bundle_name'],
      '#disabled' => $snippet_type['bundle_name'] != '',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save Snippet Type',
    ),
  );
  return $form;
}

function content_snippets_type_form_submit($form, &$form_state) {
  $bundle = (object) array(
    'bundle_name' => $form_state['values']['bundle_name'],
    'label' => $form_state['values']['label'],
    'is_new' => $form_state['values']['is_new'],
  );
  content_snippets_type_save($bundle);
  drupal_set_message(t('Your content snippet type was created'));
  $form_state['redirect'] = 'admin/structure/content_snippets';
}

function content_snippets_page_list_form($form, &$form_state) {
  // todo: sort by permissions
  $snippets = entity_get_controller('content_snippet')->load(FALSE);
  $header = array(
    'title' => t('Title'),
    'actions' => t('Actions'),
  );
  $rows = array();
  foreach ($snippets as $snippet) {
    $row = array();
    $row['title'] = $snippet->label;
    $row['actions'] = implode(' | ', array(
        l('edit', 'admin/content/content_snippets/' . $snippet->csid . '/edit'),
        l('delete', 'admin/content/content_snippets/' . $snippet->csid . '/delete'),
    ));
    $rows[] = $row;
  }
  $form['snippets'] = array(
    '#type' => 'tableselect',
    '#options' => $rows,
    '#header' => $header,
  );
  return $form;
}

function content_snippets_create_list() {
  // need access control here
  $types = content_snippets_types_get_all();
  $content = array();
  foreach ($types as $type) {
    $content[] = array(
      'title' => $type->label,
      'href' => 'admin/content/content_snippets/add/' . $type->bundle_name,
      'description' => !empty($type->description) ? $type->description : '',
      'localized_options' => array(),
    );
  }
  return theme('admin_block_content', array('content' => $content));
}

function content_snippets_create_preform($type) {
  $snippet = (object) array(
    'type' => $type->bundle_name,
    'is_new' => TRUE,
    'label' => '',
  );
  return drupal_get_form('content_snippets_form', $snippet);
}

function content_snippets_form($form, &$form_state, $snippet) {
  global $language;
  $lang = $language->language;
  if (isset($form_state['values']['language'])) {
    $lang = $form_state['values']['language'];
  }
  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $snippet,
  );
  // todo: don't hard code list of languages
  $langs = array();
  $languages = language_list();
  foreach ($languages as $key => $language) {
    $langs[$key] = t($language->name);
  }
  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('Current Language'),
    '#options' => $langs,
    '#default_value' => $lang,
    '#ajax' => array(
      'callback' => 'content_snippets_reload_language',
      'wrapper' => 'snippet-field-container',
      'effect' => 'fade',
    ),
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('This will only appear in the administrative back-end'),
    '#maxlength' => 255,
    '#required' => TRUE,
    '#default_value' => $snippet->label,
  );
  $form['fields'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="snippet-field-container">',
    '#suffix' => '</div>',
  );
  field_attach_form('content_snippet', $snippet, $form['fields'], $form_state, $lang);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Snippet',
    '#weight' => 1000,
  );
  return $form;
}

function content_snippets_form_validate($form, &$form_state) {
  field_attach_form_validate(
      'content_snippet',
      $form_state['values']['entity'],
      $form,
      $form_state
  );
}

function content_snippets_form_submit($form, &$form_state) {
  $snippet = $form_state['values']['entity'];
  $snippet->label = $form_state['values']['label'];
  if (!empty($snippet->is_new)) {
    global $user;
    $snippet->created = time();
    $snippet->uid = $user->uid;
  }
  $snippet->changed = time();
  field_attach_submit(
      'content_snippet',
      $snippet,
      $form,
      $form_state
  );
  content_snippets_save_snippet($snippet);
  drupal_set_message(t('Your content snippet has been saved'));
  $form_state['redirect'] = 'admin/content/content_snippets';
}

function content_snippets_delete_form($form, &$form_state, $snippet) {
  $form['snippet'] = array('#type' => 'value', '#value' => $snippet);
  return confirm_form($form,
      t('Are you sure you want to delete this snippet?'),
      'admin/content/content_snippets');
}

function content_snippets_delete_form_submit($form, &$form_state) {
  content_snippets_delete($form_state['values']['snippet']);
  drupal_set_message(t('Content snippet deleted'));
  $form_state['redirect'] = 'admin/content/content_snippets';
}

function content_snippets_revisions_form($form, &$form_state, $snippet) {

}