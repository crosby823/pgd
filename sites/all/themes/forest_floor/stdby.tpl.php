<?php
// $Id$

/**
 * @file
 * Custom template file for all nodes
 */
?>

<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">

  <?php if ($picture) {print $picture;}?>

  <?php if ($page == 0 && !$teaser) { ?>
    <h2 class="title">
      <a href="<?php print $node_url?>">
        <?php print $title?>
      </a>
    </h2>
  <?php }; ?>

<?php if ($page == 0 && $teaser) { ?>
    <h4>
      <a href="<?php print $node_url?>">
        <?php print $title?>
      </a>
    </h4>
  <?php }; ?>

<?php if ($taxonomy && !$teaser) { ?>
  <span class="taxonomy">
    <?php //print $terms?>
  </span>
<?php }; ?>

  <div class="content">
    <?php print $content?>
  </div>

  <?php if ($links && !$teaser) { ?>
    <div class="links">
      &raquo; <?php print $links?>
    </div>
  <?php }; ?>

</div>
