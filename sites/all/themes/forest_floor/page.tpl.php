<?php
// $Id$

/**
 * @file
 * The template for all pages.
 */
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">

<head>
  <title>
    <?php print $head_title ?>
  </title>
  <?php print $head ?>
  <?php print $styles ?>
  <!--[if lt IE 7]>
    <link rel="stylesheet" href="<?php print $front_page . $directory; ?>/ie_lt7.css" type="text/css">
  <![endif]-->
  <?php print $scripts ?>
  <script type="text/javascript"><?php // Needed to avoid Flash of Unstyle Content in IE ?> </script>
</head>

<body<?php print phptemplate_body_attributes($is_front, $layout); ?>>
  
	<div id="header-region">
      
  		<?php print $header ?>
       	<div id="header" class="clear-block">
     
     		<div id="logo-search">
        
        		<?php if ($search_box) : ?>
        		<div id="search-box">
        		<?php print $search_box ?>
        		</div>
        		<?php endif; ?>
        
        	</div>
      
        	<?php if ($logo) { ?>
         	<div id="logo">
          	<a href="<?php print $front_page ?>" title="<?php print t('Home') ?>">
            <img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" />
          	</a>
         	</div>
        	<?php } ?>
      
        	<?php if ($site_name) { ?>
         	<div class='site-name'>
          	<a href="<?php print $front_page ?>" title="<?php print t('Home') ?>">
            <?php print $site_name ?>
          	</a>
         	</div>
        	<?php } ?>
      
        	<?php if ($site_slogan) { ?>
         	<div class='site-slogan'>
          	<?php print $site_slogan ?>
         	</div>
        	<?php } ?>
        
        	<?php if (isset($primary_links)) { ?>
         	<div id="primary">
          	<?php print theme('links', $primary_links, array('class' =>'links', 'id' => 'navlist')) ?>
         	</div>
        	<?php } ?>    
      
		</div>
	</div>
    
    <div id="wrapper">
       
		<?php if ($left) { ?>
        <div id="sidebar-left" class="column">
        <?php print $left ?>
        </div>
       	<?php } ?>
     
       	<div id="main" class="column">
        <div id="main-inner">
         
        	<?php if ($mission) { ?>
            <div id="mission">
            <?php print $mission ?>
            </div>
          	<?php } ?>
          
          	<div class="inner">
           	<div class="block-inner">        
             
            	<?php //print $breadcrumb ?>
              	<h1 class="title">
                <?php print $title ?>
              	</h1>
              
             	<?php if ($tabs){ ?>
               	<div class="tabs">
                <?php print $tabs ?>
                </div>
              	<?php } ?>
              
              	<?php print $help ?>
              
              	<?php if ($show_messages && $messages) print $messages; ?>
              
              	<?php print $content_top; ?>
              
              	<?php print $content; ?>
              
              	<?php print $feed_icons; ?>
            
            </div>
          	</div>
        </div>
      	</div>

      	<?php if ($right): ?>
        <div id="sidebar-right" class="column">
        <?php print $right ?>
        </div>
      	<?php endif; ?>
 	
 	</div>
 
	<div id="footer-region">
 
 		<?php if ($footer) { ?>
        <div id="footer">
        <?php print $footer; ?>
        </div>
      	<?php } ?>

 	</div>   

  <?php print $closure ?>

</body>
</html>
