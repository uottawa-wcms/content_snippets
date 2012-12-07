<?php

$plugin = array(
  'label' => 'Content Snippet',
  'handler' => array(
    'class' => 'EntityCRUDUIContentSnippet',
  ),
);


class EntityCRUDUIContentSnippet extends EntityCRUDUIBase {

  public function __construct() {
    parent::__construct('content_snippet');
  }

  public function initializeEntity(&$entity) {
    $entity->label = '';
  }

  public function customAccess($op, $entity = NULL) {
    switch($op) {
      case 'revision view':
      case 'revision update':
      case 'revision delete':
        return content_snippets_access('edit', $entity);
    }
    return FALSE;
  }

  public function getPropertyFields($entity, $langcode = 'und') {
    $form = array(
      'label' => array(
        '#title' => t('Label'),
        '#type' => 'textfield',
        '#required' => TRUE,
        '#maxlength' => 255,
        '#weight' => -100,
        '#default_value' => $entity->label,
      ),
      'additional_settings' => array(
        '#type' => 'vertical_tabs',
        '#weight' => 5000,
        '#title' => t('Settings'),
        'revision_info' => array(
          '#title' => t('Revision information'),
          '#type' => 'fieldset',
          'revision' => array(
            '#type' => 'checkbox',
            '#title' => t('Create new revision'),
            '#default_value' => 0,
            '#access' => user_access('administer nodes'),
          ),
          'log' => array(
            '#type' => 'textarea',
            '#title' => t('Revision log message'),
            '#rows' => 4,
            '#default_value' => '',
            '#description' => t('Provide an explanation of the changes you are making. This will help other authors understand your motivations.'),
          ),
        ),
      ),
    );
    return $form;
  }

  public function propertyAttach(&$entity, $form, &$form_state) {
    parent::propertyAttach($entity, $form, $form_state);
    $loadProperties = array(
      'label', 'revision',
    );
    foreach ($loadProperties as $propkey) {
      if (!empty($form_state['values'][$propkey])) {
        $entity->$propkey = $form_state['values'][$propkey];
      }
    }
    if (!empty($form_state['values']['revision'])) {
      if (!empty($form_state['values']['log'])) {
        $entity->log = $form_state['values']['log'];
      }
    }
  }

}