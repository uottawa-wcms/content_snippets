<?php

$plugin = array(
  'title' => t('Content Snippet'),
  'description' => t('Insert a content snippet'),
  'category' => t('Content'),
  'single' => TRUE,
  'defaults' => array(
    'csid' => NULL,
    'view_mode' => 'full',
  ),
  'js' => 'misc/autocomplete.js',
);

function content_snippets_snippet_content_type_render($subtype, $conf) {
  $block = new stdClass();
  $block->title = NULL;
  if (!empty($conf['csid'])) {
    $block->content = content_snippets_view(
        content_snippets_load($conf['csid']),
        $conf['view_mode']
    );
  } else {
    $block->content = '<p>' . t('Dummy content') . '</p>';
  }
  return $block;
}

function content_snippets_snippet_content_type_edit_form($form, &$form_state) {

  /* TODO: autocomplete
   * drupal_add_js('misc/autocomplete.js');
  $form['csid'] = array(
    '#title' => t('Snippet'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'content_snippets/autocomplee',
    '#default_value' => $form_state['conf']['csid'],
  );
   *
   */
  $snippets = entity_get_controller('content_snippet')->load(FALSE);
  $snippet_opts = array();
  foreach ($snippets as $snippet) {
    $snippet_opts[$snippet->csid] = $snippet->label;
  }
  $form['csid'] = array(
    '#options' => $snippet_opts,
    '#title' => t('Snippet'),
    '#type' => 'select',
    '#default_value' => $form_state['conf']['csid'],
    '#required' => TRUE,
  );
  $info = entity_get_info('content_snippet');
  $options = array();
  foreach ($info['view modes'] as $key => $opts) {
    $options[$key] = $opts['label'];
  }
  $form['view_mode'] = array(
    '#title' => t('View Mode'),
    '#type' => 'select',
    '#default_value' => $form_state['conf']['view_mode'],
    '#options' => $options,
    '#required' => TRUE,
  );
  return $form;
}

function content_snippets_snippet_content_type_edit_form_submit($form, &$form_state) {
  $conf = array(
    'csid' => $form_state['values']['csid'],
    'view_mode' => $form_state['values']['view_mode'],
  );
  $form_state['conf'] = $conf;
}

function content_snippets_snippet_content_type_admin_title($subtype, $conf) {
  $snippet = content_snippets_load($conf['csid']);
  return $snippet->label;
}

