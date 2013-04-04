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
    $tx = $transaction instanceof DatabaseTransaction ? $transaction : db_transaction();
    if (empty($snippet->log)) {
      $snippet->log = '';
    }
    if (empty($snippet->timestamp)) {
      $snippet->timestamp = time();
    }
    parent::save($snippet, $tx);
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
