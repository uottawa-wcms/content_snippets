<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of ContentSnippetEntityController
 *
 * @author rturnbul
 */
class ContentSnippetEntityController extends EntityAPIController {

  public function save($snippet, $transaction = NULL) {
    $tx = !empty($transaction) ? $transaction : db_transaction();
    try {
      parent::save($snippet, $tx);
      global $user;
      if ($snippet->revision || $snippet->is_new) {
        $rev = (object) array(
          'csid' => $snippet->csid,
          'label' => $snippet->label,
          'uid' => $user->uid,
          'log' => !empty($snippet->log) ? $snippet->log : '',
          'timestamp' => time(),
        );
        drupal_write_record('content_snippets_revisions', $rev);
        $snippet->vid = $rev->vid;
        drupal_write_record('content_snippets', $snippet, array('csid'));
      } else {
        $rev = (object) array(
          'csid' => $snippet->csid,
          'label' => $snippet->label,
          'uid' => $user->uid,
          'timestamp' => time(),
          'vid' => $snippet->vid,
        );
        drupal_write_record('content_snippets_revisions', $rev, array('vid'));
      }
    } catch (Exception $e) {
      $tx->rollback();
      throw $e;
    }
  }

  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    $snippet = reset($entities);
    return theme('content_snippet', array(
      'snippet' => $snippet,
      'content' => $this->buildContent($snippet, $view_mode, $langcode),
      'view_mode' => $view_mode,
    ));
  }


}

?>
