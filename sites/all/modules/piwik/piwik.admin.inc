<?php

/**
 * @file
 * Administrative page callbacks for the piwik module.
 */

/**
 * Implementation of hook_admin_settings() for configuring the module
 */
function piwik_admin_settings_form(&$form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['account']['piwik_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Piwik site ID'),
    '#default_value' => variable_get('piwik_site_id', ''),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('The user account number is unique to the websites domain. Click the <strong>Settings</strong> link in your Piwik account, then the <strong>Websites</strong> tab and enter the appropriate site <strong>ID</strong> into this field.'),
  );
  $form['account']['piwik_url_http'] = array(
    '#type' => 'textfield',
    '#title' => t('Piwik HTTP URL'),
    '#default_value' => variable_get('piwik_url_http', ''),
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#description' => t('The URL to your Piwik base directory. Example: "http://www.example.com/piwik/".'),
  );
  $form['account']['piwik_url_https'] = array(
    '#type' => 'textfield',
    '#title' => t('Piwik HTTPS URL'),
    '#default_value' => variable_get('piwik_url_https', ''),
    '#size' => 80,
    '#maxlength' => 255,
    '#description' => t('The URL to your Piwik base directory with SSL certificate installed. Required if you track a SSL enabled website. Example: "https://www.example.com/piwik/".'),
  );
  // Required for automated form save testing only.
  $form['account']['piwik_url_skiperror'] = array(
    '#type' => 'hidden',
    '#default_value' => FALSE,
  );

  $form['domain_tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Domains'),
  	'#collapsible' => TRUE,
  );

  global $cookie_domain;
  $multiple_sub_domains = array();
  foreach (array('www', 'app', 'shop') as $subdomain) {
    if (count(explode('.', $cookie_domain)) > 2 && !is_numeric(str_replace('.', '', $cookie_domain))) {
      $multiple_sub_domains[] = $subdomain . $cookie_domain;
    }
    // IP addresses or localhost.
    else {
      $multiple_sub_domains[] = $subdomain . '.example.com';
    }
  }

  $form['domain_tracking']['piwik_domain_mode'] = array(
    '#type' => 'radios',
    '#title' => t('What are you tracking?'),
    '#options' => array(
      0 => t('A single domain (default)') . '<div class="description">' . t('Domain: @domain', array('@domain' => $_SERVER['HTTP_HOST'])) . '</div>',
      1 => t('One domain with multiple subdomains') . '<div class="description">' . t('Examples: @domains', array('@domains' => implode(', ', $multiple_sub_domains))) . '</div>',
    ),
    '#default_value' => variable_get('piwik_domain_mode', 0),
  );

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('piwik_visibility_pages', 0);
  $pages = variable_get('piwik_pages', PIWIK_PAGES);

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['piwik_visibility_pages'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['piwik_pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['piwik_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['piwik_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
      '#wysiwyg' => FALSE,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific tracking settings'),
    '#collapsible' => TRUE,
  );

  $form['role_vis_settings']['piwik_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('piwik_visibility_roles', 0),
  );

  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings']['piwik_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('piwik_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Standard tracking configurations.
  $form['user_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('User specific tracking settings'),
    '#collapsible' => TRUE,
  );
  $form['user_vis_settings']['piwik_custom'] = array(
    '#type' => 'radios',
    '#title' => t('Custom tracking settings'),
    '#options' => array(
      t('Users cannot control whether they are tracked or not.'),
      t('Track users by default, but let individual users to opt out.'),
      t('Do not track users by default, but let individual users to opt in.')
    ),
    '#description' => t('Allow individual users to customize the visibility of tracking in their account settings. Only users with %permission permission are allowed to set their own preference.', array('%permission' => t('opt-in or out of tracking'))),
    '#default_value' => variable_get('piwik_custom', 0),
  );
  $form['user_vis_settings']['piwik_trackuserid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track User ID'),
    '#default_value' => variable_get('piwik_trackuserid', 0),
    '#description' => t('User ID enables the analysis of groups of sessions, across devices, using a unique, persistent, and non-personally identifiable ID string representing a user. <a href="@url">Learn more about the benefits of using User ID</a>.', array('@url' => 'http://piwik.org/docs/user-id/')),
  );

  // Link specific configurations.
  $form['linktracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['linktracking']['piwik_trackmailto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track clicks on mailto links'),
    '#default_value' => variable_get('piwik_trackmailto', 1),
  );
  $form['linktracking']['piwik_track'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track download and outbound links'),
    '#default_value' => variable_get('piwik_track', 1),
    '#description' => t('Enables tracking of clicks on download and outbound links.')
  );
  $form['linktracking']['piwik_trackfiles_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('File extensions to track'),
    '#default_value' => variable_get('piwik_trackfiles_extensions', PIWIK_TRACKFILES_EXTENSIONS),
    '#description' => t('A pipe separated list of file extensions that should be tracked when clicked with regular expression support. Example: !extensions', array('!extensions' => PIWIK_TRACKFILES_EXTENSIONS)),
    '#maxlength' => 500,
  );

  // Privacy specific configurations.
  $form['tracking']['privacy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privacy'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['tracking']['privacy']['piwik_privacy_donottrack'] = array(
    '#type' => 'checkbox',
    '#title' => t('Universal web tracking opt-out'),
    '#description' => t('If enabled and your Piwik server receives the <a href="http://donottrack.us/">Do-Not-Track</a> header from the client browser, the Piwik server will not track the user. Compliance with Do Not Track could be purely voluntary, enforced by industry self-regulation, or mandated by state or federal law. Please accept your visitors privacy. If they have opt-out from tracking and advertising, you should accept their personal decision.'),
    '#default_value' => variable_get('piwik_privacy_donottrack', 1),
  );

  // Piwik page title tree view settings.
  $form['page_title_hierarchy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page titles hierarchy'),
    '#description' => t('This functionality enables a dynamically expandable tree view of your site page titles in your Piwik statistics. See in Piwik statistics under <em>Actions</em> > <em>Page titles</em>.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['page_title_hierarchy']['piwik_page_title_hierarchy'] = array(
    '#type' => 'checkbox',
    '#title' => t("Show page titles as hierarchy like breadcrumbs"),
    '#description' => t('By default Piwik tracks the current page title and shows you a flat list of the most popular titles. This enables a breadcrumbs like tree view.'),
    '#default_value' => variable_get('piwik_page_title_hierarchy', FALSE),
  );
  $form['page_title_hierarchy']['piwik_page_title_hierarchy_exclude_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide home page from hierarchy'),
    '#description' => t('If enabled, the "Home" item will be removed from the hierarchy to flatten the structure in the Piwik statistics. Hits to the home page will still be counted, but for other pages the hierarchy will start at level Home+1.'),
    '#default_value' => variable_get('piwik_page_title_hierarchy_exclude_home', TRUE),
  );

  // Advanced feature configurations.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['piwik_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache tracking code file locally'),
    '#description' => t("If checked, the tracking code file is retrieved from Piwik and cached locally. It is updated daily from your Piwik servers to ensure updates to tracking code are reflected in the local copy."),
    '#default_value' => variable_get('piwik_cache', 0),
  );
  if (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PRIVATE) {
    $form['advanced']['piwik_cache']['#disabled'] = TRUE;
    $form['advanced']['piwik_cache']['#description'] .= ' '. t('<a href="@url">Public file transfers</a> must be enabled to allow local caching.', array('@url' => url('admin/settings/file-system', array('query' => drupal_get_destination()))));
  }

  // Allow for tracking of the originating node when viewing translation sets.
  if (module_exists('translation')) {
    $form['advanced']['piwik_translation_set'] = array(
      '#type' => 'checkbox',
      '#title' => t('Track translation sets as one unit'),
      '#description' => t('When a node is part of a translation set, record statistics for the originating node instead. This allows for a translation set to be treated as a single unit.'),
      '#default_value' => variable_get('piwik_translation_set', 0),
    );
  }

  $site_search_dependencies = '<div class="admin-dependencies">';
  $site_search_dependencies .= t('Depends on: !dependencies', array('!dependencies' => (module_exists('search') ? t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => 'Search')) : t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => 'Search')))));
  $site_search_dependencies .= '</div>';

  $form['advanced']['piwik_site_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track internal search'),
    '#description' => t('If checked, internal search keywords are tracked.') . $site_search_dependencies,
    '#default_value' => variable_get('piwik_site_search', FALSE),
    '#disabled' => (module_exists('search') ? FALSE : TRUE),
  );

  $form['advanced']['codesnippet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom JavaScript code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('You can add custom Piwik <a href="@snippets">code snippets</a> here. These will be added to every page that Piwik appears on. <strong>Do not include the &lt;script&gt; tags</strong>, and always end your code with a semicolon (;).', array('@snippets' => 'http://piwik.org/docs/javascript-tracking/'))
  );
  $form['advanced']['codesnippet']['piwik_codesnippet_before'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (before)'),
    '#default_value' => variable_get('piwik_codesnippet_before', ''),
    '#rows' => 5,
    '#wysiwyg' => FALSE,
    '#description' => t('Code in this textarea will be added <strong>before</strong> _paq.push(["trackPageView"]).')
  );
  $form['advanced']['codesnippet']['piwik_codesnippet_after'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (after)'),
    '#default_value' => variable_get('piwik_codesnippet_after', ''),
    '#rows' => 5,
    '#wysiwyg' => FALSE,
    '#description' => t('Code in this textarea will be added <strong>after</strong> _paq.push(["trackPageView"]). This is useful if you\'d like to track a site in two accounts.')
  );

  /* hook_footer() is not able to add code to head. Upgrade to D7 required.
  $form['advanced']['piwik_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#description' => t("<strong>Warning:</strong> Adding the external JavaScript files to the footer region is recommended for performance reasons."),
    '#options' => array(
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    '#default_value' => variable_get('piwik_js_scope', 'footer'),
  ); */

  return system_settings_form($form);
}

function piwik_admin_settings_form_validate($form, &$form_state) {
  if (!preg_match('/^\d{1,}$/', $form_state['values']['piwik_site_id'])) {
    form_set_error('piwik_site_id', t('A valid Piwik site ID is an integer only.'));
  }

  $piwik_url = $form_state['values']['piwik_url_http'];
  if ('/' != drupal_substr($piwik_url, -1, 1)) {
    $piwik_url = $piwik_url . '/';
    $form_state['values']['piwik_url_http'] = $piwik_url;
  }
  $url = $piwik_url . 'piwik.php';
  $result = drupal_http_request($url);
  if ($result->code != 200 && $form_state['values']['piwik_url_skiperror'] == FALSE) {
    form_set_error('piwik_url_http', t('The validation of "@url" failed with error "@error" (HTTP code @code).', array('@url' => check_url($url), '@error' => $result->error, '@code' => $result->code)));
  }

  if (!empty($form_state['values']['piwik_url_https'])) {
    $piwik_url = $form_state['values']['piwik_url_https'];
    if ('/' != drupal_substr($piwik_url, -1, 1)) {
      $piwik_url = $piwik_url . '/';
      $form_state['values']['piwik_url_https'] = $piwik_url;
    }
    $url = $piwik_url . 'piwik.php';
    $result = drupal_http_request($url);
    if ($result->code != 200 && $form_state['values']['piwik_url_skiperror'] == FALSE) {
      form_set_error('piwik_url_https', t('The validation of "@url" failed with error "@error" (HTTP code @code).', array('@url' => check_url($url), '@error' => $result->error, '@code' => $result->code)));
    }
  }

  // Delete obsolete local cache file.
  if (empty($form_state['values']['piwik_cache']) && $form['advanced']['piwik_cache']['#default_value']) {
    piwik_clear_js_cache();
  }

  // This is for the Newbie's who cannot read a text area description.
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['piwik_codesnippet_before'])) {
    form_set_error('piwik_codesnippet_before', t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['piwik_codesnippet_after'])) {
    form_set_error('piwik_codesnippet_after', t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }

  // Trim some text area values.
  $form_state['values']['piwik_site_id'] = trim($form_state['values']['piwik_site_id']);
  $form_state['values']['piwik_pages'] = trim($form_state['values']['piwik_pages']);
  $form_state['values']['piwik_codesnippet_before'] = trim($form_state['values']['piwik_codesnippet_before']);
  $form_state['values']['piwik_codesnippet_after'] = trim($form_state['values']['piwik_codesnippet_after']);
}
