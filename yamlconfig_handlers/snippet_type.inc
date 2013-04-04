<?php

function yamlconfig_snippet_type_handler_validate($def) {
  if (empty($def['bundle_name'])) {
    return FALSE;
  }
  if (empty($def['label'])) {
    return FALSE;
  }
  return TRUE;
}

function yamlconfig_snippet_type_handler_create($def) {
  if (!content_snippets_type_exists($def['bundle_name'])) {
    $def['is_new'] = TRUE;
  }
  content_snippets_type_save((object) $def);
}

function yamlconfig_snippet_type_handler_refresh($def) {
  yamlconfig_snippet_type_handler_create($def);
}

function yamlconfig_snippet_type_handler_uninstall($def) {
  content_snippets_type_delete($def['bundle_name']);
}