<?php
/**
 * $content has all the rendered content items
 * $snippet has all the original fields if you need to directly access the
 *   values (bypassing field rendering).
 */
?>
<?php print drupal_render($content); ?>