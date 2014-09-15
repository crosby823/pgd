<?php
// $Id$

/**
 * @file
 * Custom template file for all nodes
 */
?>

<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">

  <?php if ($picture) {print $picture;}?>

  <?php if ($page == 0) { ?>
    <h2 class="title">
      <a href="<?php print $node_url?>">
        <?php print $title?>
      </a>
    </h2>
  <?php }; ?>

<span class="date">
    <b><?php print "Posted: "?></b> <?php print $date?>
    </span> 
    
  <span class="submitted">
    <?php print $submitted?>
  </span>

  <div class="content">
    <?php print $content?>
  </div>

<div class="taxonomy">
    <b><?php print "Tags:  "?></b> <?php print $terms?>
  </div>
    

</div>
