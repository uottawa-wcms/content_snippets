<?php

class ContentSnippetController extends DrupalDefaultEntityController {

  public function delete($snippet) {
    $tx = db_transaction();
    try {
      $query = db_delete('content_snippets_revisions');
      $query->condition('csid', $snippet->csid);
      $query->execute();
      $query = db_delete('content_snippets');
      $query->condition('csid', $snippet->csid);
      $query->execute();
      field_attach_delete('content_snippet', $snippet);
      module_invoke_all('entity_delete', $snippet, 'content_snippet');
      return TRUE;
    } catch (Exception $e) {
      $tx->rollback();
      throw $e;
    }
    return FALSE;
  }

  public function save($snippet) {
    $tx = db_transaction();
    try {
      $pk = array('csid');
      $new = TRUE;
      if (!((!empty($snippet->csid)) && (empty($snippet->is_new)))) {
        $base = array(
          'label' => '',
          'type' => '',
        );
        drupal_write_record('content_snippets', $base);
        $snippet->csid = $base->csid;
      }
      if (!empty($snippet->revision)) {
        $snippet->old_vid = $snippet->vid;
        $snippet->vid = NULL;
        drupal_write_record('content_snippets_revisions', $snippet);
      }
      drupal_write_record('content_snippets', $snippet);
      return TRUE;
    } catch (Exception $e) {
      $tx->rollback();
      throw $e;
    }
    return FALSE;
  }

}