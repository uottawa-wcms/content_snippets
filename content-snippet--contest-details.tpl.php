<?php global $language; ?>
<div>
  <?php print 'bananananza'; ?>
  <a href="<?php print $snippet->uottawa_link[$language->language][0]['url']; ?>" class="biglink">
    <h2><span><?php print $snippet->uottawa_short_title[$language->language][0]['value']; ?></span></h2>
    <p><?php print $snippet->uottawa_summary[$language->language][0]['value']; ?></p>
    <img src="<?php print file_create_url($snippet->uottawa_primary_image['und'][0]['uri']); ?>" alt="<?php print $snippet->uottawa_image_alt[$language->language][0]['value']; ?>"/>
  </a>
</div>
