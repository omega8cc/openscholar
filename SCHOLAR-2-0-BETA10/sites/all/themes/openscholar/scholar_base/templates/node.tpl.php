<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>">
  <div class="node-inner">
    <?php print $picture; ?>
    <?php if (!$page): ?>
      <h3 class="title">
        <a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title; ?></a>
      </h3>
    <?php endif; ?>
    <?php if ($unpublished): ?>
      <div class="unpublished"><?php print t('Unpublished'); ?></div>
    <?php endif; ?>
    <?php if ($submitted): ?>
        <p class="submitted"><?php print $submitted; ?></p>
      <?php endif; ?>
     <?php if ($page): ?>
      <?php if ($terms): ?>
      <div class="terms terms-inline"><?php print t(' in ') . $terms; ?></div>
      <?php endif; ?>
     <?php endif; ?>
    <div class="content">
      <?php print $content; ?>
    </div>
    <?php if (!$page): ?>
      <?php if ($terms): ?>
      <div class="terms terms-inline"><?php print t(' in ') . $terms; ?></div>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($links): ?>
    <div class="links clearfix">
      <?php print $links; ?>
    </div>
    <?php endif;?>
  </div> <!-- /node-inner -->
</div> <!-- /node -->