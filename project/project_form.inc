<?php
// $Id$

/**
 * @file
 * Provides additional functions for project module.
 */

/**
 * Functions called from hook_form() in project.module
 */
function project_project_form_build(&$form_state, $node) {
  //orgright_debug_msg('project','Fn: project_project_form_build');
  // Specify the form for adding or updating projects
  $form = array();
  $fix = 0;
  $fix++;
  $form['project'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => project_field_description('project-details', 'project', t('Enter the name of the project')),
    '#default_value' => $node->project,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['manager'] = array(
      '#type' => 'textfield',
      '#title' => t('Project manager'),
      '#description' => project_field_description('project-details', 'manager', t('Enter the name of the person resonsible for this project')),
      '#default_value' => $node->manager,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['manager'] = array(
      '#type' => 'select',
      '#title' => t('Project manager'),
      '#description' => project_field_description('project-details', 'manager', t('Enter the name of the person resonsible for this project')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $category_list = project_get_categories();
  $form['projcategory'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#description' => project_field_description('project-details', 'projcategory', t('Chose project category from the list')),
    '#default_value' => $node->projcategory,
    '#options' => $category_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $status_list = project_get_statuses();
  $form['projstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => project_field_description('project-details', 'projstatus', t('Chose the project status from the list')),
    '#default_value' => $node->projstatus,
    '#options' => $status_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => project_field_description('project-details', 'body', t('Enter a description of this project')),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['projdate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Project start or finish date'),
    '#description' => project_field_description('project-details', 'projdate', t('Specify either the date this project should start or should finish')),
    '#default_value' => $node->projdate,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#date_year_range' => '-1:+5',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $projdatype_list = project_get_projdatypes('types');
  //orgright_debug_dump('projdatypes',$projdatype_list);
  $form['projdatype'] = array(
    '#type' => 'select',
    '#title' => t('Project date constraint'),
    '#description' => project_field_description('project-details', 'projdatype', t('Specify whether the following date is a start or finish date')),
    '#default_value' => $node->projdatype,
    '#options' => $projdatype_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['pschedule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project Schedule'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  if ($node->recalc) {
    $recalculate = t('The project schedule needs to be re-calculated!');
  }
  else {
    $recalculate = t('The project schedule is up to date');
  }
  $form['pschedule']['recalculate'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#value' => $recalculate,
    '#suffix' => '</div>',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['pschedule']['projsched'] = array(
    '#prefix' => '<div>',
    '#value' => $node->projsched,
    '#suffix' => '</div>',
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => project_field_description('project-details', 'notes', t('General notes about this project')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['recalc'] = array(
    '#type' => 'value',
    '#value' => $node->recalc,
  );
  // save values for later checking
  $form['save_projdatype'] = array(
    '#type' => 'value',
    '#value' => $node->projdatype,
  );
  $form['save_projdate'] = array(
    '#type' => 'value',
    '#value' => $node->projdate,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('project_form_validate');
  $form['#submit'] = array('project_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Projects')=>'project/projects'));
  return $form;
}

function project_projactivity_form_build(&$form_state, $node) {
  //orgright_debug_msg('project','Fn: project_projactivity_form_build');
  // Specify the form for adding or updating project activities
  $form = array();
  $fix = 0;
  $fix++;
  $form['activity'] = array(
    '#type' => 'textfield',
    '#title' => t('Activity'),
    '#description' => project_field_description('activity-details', 'activity', t('Enter the project activity title')),
    '#default_value' => $node->activity,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['project'] = array(
    '#type' => 'textfield',
    '#title' => t('project'),
    '#description' => project_field_description('activity-details', 'project', t('Name of project for this activity')),
    '#default_value' => $node->project,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'project/autocomplete/projects',
  );
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Responsible person'),
      '#description' => project_field_description('activity-details', 'member', t('Enter the name of the person resonsible for this activity')),
      '#default_value' => $node->member,
      '#maxlength' => 64,
      '#size' => 40,
      '#resizable' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['member'] = array(
      '#type' => 'select',
      '#title' => t('Responsible person'),
      '#description' => project_field_description('activity-details', 'member', t('Enter the name of the person resonsible for this activity')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $category_list = project_get_categories();
  $form['activicategory'] = array(
    '#type' => 'select',
    '#title' => t('Activity category'),
    '#description' => project_field_description('activity-details', 'activicategory', t('Select activity category')),
    '#default_value' => $node->activicategory,
    '#options' => $category_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $status_list = project_get_statuses();
  $form['activistatus'] = array(
    '#type' => 'select',
    '#title' => t('Activity status'),
    '#description' => project_field_description('activity-details', 'activistatus', t('Select activity status')),
    '#default_value' => $node->activistatus,
    '#options' => $status_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => project_field_description('activity-details', 'body', t('Enter details of this project activity')),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Activity duration'),
    '#description' => project_field_description('activity-details', 'duration', t('Enter duration of the activity')),
    '#default_value' => $node->duration,
    '#maxlength' => 16,
    '#size' => 16,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $duratype_list = project_get_duratypes('types');
  $form['duratype'] = array(
    '#type' => 'select',
    '#title' => t('Duration type'),
    '#description' => project_field_description('activity-details', 'duratype', t('Time units for duration')),
    '#default_value' => $node->duratype,
    '#options' => $duratype_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['actividate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Activity date'),
    '#description' => project_field_description('activity-details', 'actividate', t('Enter the constraint date of the activity')),
    '#default_value' => $node->actividate,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $actividatypes_list = project_get_actividatypes('types');
  $form['actividatype'] = array(
    '#type' => 'select',
    '#title' => t('Activity date constraint'),
    '#description' => project_field_description('activity-details', 'actividatype', t('Select a date constraint type')),
    '#default_value' => $node->actividatype,
    '#options' => $actividatypes_list,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['actualstart'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Actual start'),
    '#description' => project_field_description('activity-details', 'actualstart', t('Enter the actual start date of the activity')),
    '#default_value' => $node->actualstart,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['actualfinish'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Actual finish'),
    '#description' => project_field_description('activity-details', 'actualfinish', t('Enter the actual finish date of the activity')),
    '#default_value' => $node->actualfinish,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => project_field_description('activity-details', 'notes', t('General notes about this project activity')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['pnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['recalc'] = array(
    '#type' => 'value',
    '#value' => FALSE,
  );
  // save values for later checking
  $form['save_duration'] = array(
    '#type' => 'value',
    '#value' => $node->duration,
  );
  $form['save_duratype'] = array(
    '#type' => 'value',
    '#value' => $node->duratype,
  );
  $form['save_actividatype'] = array(
    '#type' => 'value',
    '#value' => $node->actividatype,
  );
  $form['save_actividate'] = array(
    '#type' => 'value',
    '#value' => $node->actividate,
  );
  $form['save_actualstart'] = array(
    '#type' => 'value',
    '#value' => $node->actualstart,
  );
  $form['save_actualfinish'] = array(
    '#type' => 'value',
    '#value' => $node->actualfinish,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('project_form_validate');
  $form['#submit'] = array('project_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Project activities')=>'project/projactivities'));
  return $form;
}

function project_projreport_form_build(&$form_state, $node) {
  //orgright_debug_msg('project','Fn: project_projreport_form_build');
  // Specify the form for adding or updating project reports
  $form = array();
  $fix = 0;
  $fix++;
  $form['projreport'] = array(
    '#type' => 'textfield',
    '#title' => t('Report'),
    '#description' => project_field_description('report-details', 'projreport', t('Specify the name of this project report')),
    '#default_value' => $node->projreport,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Responsible person'),
      '#description' => project_field_description('report-details', 'member', t('Enter the name of the person making this report')),
      '#default_value' => $node->member,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['member'] = array(
      '#type' => 'select',
      '#title' => t('Responsible person'),
      '#description' => project_field_description('report-details', 'member', t('Enter the name of the person making this report')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['project'] = array(
    '#type' => 'textfield',
    '#title' => t('Project'),
    '#description' => project_field_description('report-details', 'project', t('Specify the project being reported on')),
    '#default_value' => $node->project,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'project/autocomplete/projects',
  );
  $fix++;
  $form['projactivity'] = array(
    '#type' => 'textfield',
    '#title' => t('Activity'),
    '#description' => project_field_description('report-details', 'projactivity', t('If this report relates to a specific activity, specify the activity being reported on')),
    '#default_value' => $node->projactivity,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'project/autocomplete/activities',
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['projrepdate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Report date'),
    '#description' => project_field_description('report-details', 'projrepdate', t('Enter the date of the report')),
    '#default_value' => ($node->projrepdate) ? $node->projrepdate : orgright_get_date_ymd(),
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => project_field_description('report-details', 'body', t('Enter details of this project report')),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => project_field_description('report-details', 'notes', t('General notes about this project report')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create a placeholder to be filled in during validation
  $form['pnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['anid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('project_form_validate');
  $form['#submit'] = array('project_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Project reports')=>'project/projreports'));
  return $form;
}

function project_calculate_schedule_form_build(&$form_state, $args=NULL) {
  // Build the form for specifying project schedule re-caculation
  //orgright_debug_msg('project','Fn: project_calculate_schedule_form_build');
  // Check whether any additional arguments were passed
  if ($args) {
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'project':
        // fill in the project field information
        $sql = "SELECT project FROM {node} n JOIN {org_projects} p on p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
        $form_state['storage']['project'] = db_result(db_query($sql, $parvalue));
        $form_state['storage']['pnid'] = $parvalue;
        break;
      default:
      }
    }
  }
  // test whether the form is being initialised 
  if (!isset($form_state['values'])) {
    $form_state['storage']['initialise'] = TRUE;
  }
  // Set the title for this page
  drupal_set_title(t('Update Project Schedule'));
  // If the form is being initialised, then add first field and submit button and handlers
  if ($form_state['storage']['initialise']) {
    $form = array();
    $fix = 0;
    $fix++;
    $form['project'] = array(
      '#type' => 'textfield',
      '#title' => t('Project'),
      '#description' => project_field_description('schedule-details', 'project', t('Name of project for this activity')),
      '#default_value' => $form_state['storage']['project'],
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'project/autocomplete/projects',
    );
    // create a placeholder to be filled in during validation
    $form['pnid'] = array(
      '#type' => 'value',
      '#value' => $form_state['storage']['pnid'],
    );
    $fix++;
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Get project schedule'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $form['#validate'] = array('project_form_validate');
    $form['#submit'] = array('project_form_submit');
    return $form;
  }
  // Have the project, so now need to load the project node
  $node = node_load($form_state['storage']['pnid']);
  $form = array();
  $fix = 0;
  $fix++;
  $form['project'] = array(
    '#type' => 'item',
    '#title' => $form_state['storage']['project'],
    '#description' => t('Selected project'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['manager'] = array(
    '#type' => 'item',
    '#title' => $node->manager,
    '#description' => t('Person resonsible for this project'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $projdatype_list = project_get_projdatypes('types');
  $form['projdatype'] = array(
    '#type' => 'select',
    '#title' => t('Project date constraint'),
    '#description' => project_field_description('schedule-details', 'projdatype', t('Specify whether the following date is a start or finish date')),
    '#default_value' => $node->projdatype,
    '#options' => $projdatype_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['projdate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Project start or finish date'),
    '#description' => project_field_description('schedule-details', 'projdate', t('Specify either the date this project should start or should finish')),
    '#default_value' => $node->projdate,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#date_year_range' => '-1:+5',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['pschedule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project Schedule'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  if ($node->recalc) {
    $recalculate = t('The project schedule needs to be re-calculated!');
  }
  else {
    $recalculate = t('The project schedule is up to date');
  }
  $form['pschedule']['recalculate'] = array(
    '#type' => 'markup',
    '#prefix' => '<div>',
    '#value' => $recalculate,
    '#suffix' => '</div>',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['pschedule']['projsched'] = array(
    '#prefix' => '<div>',
    '#value' => $node->projsched,
    '#suffix' => '</div>',
  );
  $fix++;
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Re-calculate schedule'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $form['#validate'] = array('project_form_validate');
  $form['#submit'] = array('project_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Projects')=>'project/projects'));
  return $form;
}

function project_activity_links_form_build(&$form_state, $args=NULL) {
  // Build the form for specifying project activity links
  //orgright_debug_msg('project','Fn: project_activity_links_form_build',7);
  // Check whether any additional arguments were passed
  if ($args) {
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'project':
        // fill in the project field information
        $sql = "SELECT project FROM {node} n JOIN {org_projects} p on p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
        $form_state['storage']['project'] = db_result(db_query($sql, $parvalue));
        $form_state['storage']['pnid'] = $parvalue;
        break;
      case 'activity':
        // fill in the project field information
        $sql = "SELECT activity FROM {node} n JOIN {org_projactivities} a on a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
        $form_state['storage']['activity'] = db_result(db_query($sql, $parvalue));
        $form_state['storage']['anid'] = $parvalue;
        break;
      default:
      }
    }
  }
  // test whether the form is being initialised 
  if (!isset($form_state['values'])) {
    $form_state['storage']['initialise'] = TRUE;
  }
  // If the form is being initialised, then add first two fields and submit button and handlers
  if ($form_state['storage']['initialise']) {
    $form = array();
    $fix = 0;
    $fix++;
    $form['project'] = array(
      '#type' => 'textfield',
      '#title' => t('project'),
      '#description' => project_field_description('activity-links', 'project', t('Name of project to process')),
      '#default_value' => $form_state['storage']['project'],
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'project/autocomplete/projects',
    );
    $fix++;
    $form['activity'] = array(
      '#type' => 'textfield',
      '#title' => t('Activity'),
      '#description' => project_field_description('activity-links', 'activity', t('Specify an activity that will be linked to others in this project')),
      '#default_value' => $form_state['storage']['activity'],
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'project/autocomplete/activities',
    );
    // create a placeholder to be filled in during validation
    $form['pnid'] = array(
      '#type' => 'value',
      '#value' => $form_state['storage']['pnid'],
    );
    $form['anid'] = array(
      '#type' => 'value',
      '#value' => $form_state['storage']['anid'],
    );
    $fix++;
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Get project activities'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $form['#validate'] = array('project_form_validate');
    $form['#submit'] = array('project_form_submit');
    return $form;
  }
  elseif ($form_state['storage']['change_activity']) {
    // Have the project, but need to change the activity
    $form = array();
    $fix = 0;
    $fix++;
    $form['project'] = array(
      '#type' => 'item',
      '#title' => $form_state['storage']['project'],
      '#description' => 'Selected project',
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $form['activity'] = array(
      '#type' => 'textfield',
      '#title' => t('Activity'),
      '#description' => project_field_description('activity-links', 'activity', t('Specify an activity that will be linked to others in this project')),
      '#default_value' => $form_state['activity'],
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'project/autocomplete/activities',
    );
    // create a placeholder to be filled in during validation
    $form['anid'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
    $fix++;
    $form['submit_button'] = array(
      '#type' => 'submit',
      '#value' => t('Get project activities'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $form['#validate'] = array('project_form_validate');
    $form['#submit'] = array('project_form_submit');
    return $form;
  }
  // otherwise building the complete form with all the activities
  $form = array();
  $fix = 0;
  $fix++;
  $form['project'] = array(
    '#type' => 'item',
    '#title' => $form_state['storage']['project'],
    '#description' => 'Selected project',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['activity'] = array(
    '#type' => 'item',
    '#title' => $form_state['storage']['activity'],
    '#description' => 'Current activity',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $linktype_list = project_get_activity_linktypes($form_state['storage']['activity']);
  // find all the existing linkages involving this activity
  // First, load the additional file to handle data access
  module_load_include('inc', 'project', 'project_data');
  $activity_links = project_get_activity_links($form_state['storage']['anid']);
  // and save array (keyed by activity nid) for later use
  $form_state['storage']['activity_links'] = $activity_links;
  // find all the other activities belonging to this project
  $sql = "SELECT a.nid, a.activity FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.pnid = %d AND NOT a.nid = %d";
  $result = db_query($sql, $form_state['storage']['pnid'], $form_state['storage']['anid']);
  // and build a field for each
  while ($activity = db_fetch_object($result)) {
    $form['activi'.$activity->nid] = array(
      '#type' => 'select',
      '#title' => $activity->activity,
      '#description' => project_field_description('activity-links', 'linkage', t('Select type of linkage to first activity')),
      '#default_value' => $activity_links[$activity->nid],
      '#options' => $linktype_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
  }
  // now add the submit buttons and handlers
  $fix++;
  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save activity links'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['change_button'] = array(
    '#type' => 'submit',
    '#value' => t('Select new current activity'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $form['#validate'] = array('project_form_validate');
  $form['#submit'] = array('project_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Project activities')=>'project/projactivities'));
  return $form;
}

/**
 *  Function to generate a field description with popup help for each form field
 */
function project_field_description($helpfile, $field, $description) {
  return theme('helpservice_topic','project',$helpfile.'/'.$field).$description;
}

/**
 * Implementation of hook_form_alter()
 */
function project_form_project_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('project','Fn: project_form_project_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'project':
        // fill in the project field information
        $sql = "SELECT project FROM {node} n JOIN {org_projects} p on p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
        $project = db_result(db_query($sql, $parvalue));
        $form['project']['#default_value'] = $project;
        break;
      default:
      }
    }
  }
  // Suppress the formating information
  unset($form['body_filter']['filter']);
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add Project';
    $form['#goto'] = 'project/projects/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function project_form_projactivity_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('project','Fn: project_form_projactivity_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'project':
        // fill in the project field information
        $sql = "SELECT project FROM {node} n JOIN {org_projects} p on p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
        $project = db_result(db_query($sql, $parvalue));
        $form['project']['#default_value'] = $project;
        break;
      default:
      }
    }
  }
  // Suppress the formating information
  unset($form['body_filter']['filter']);
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add Project Activity';
    $form['#goto'] = 'project/projactivities/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function project_form_projreport_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('project','Fn: project_form_projreport_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'project':
        // fill in the project field information
        $sql = "SELECT project FROM {node} n JOIN {org_projects} p on p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
        $project = db_result(db_query($sql, $parvalue));
        $form['project']['#default_value'] = $project;
        break;
      case 'activity':
        // fill in the project field information
        $sql = "SELECT activity FROM {node} n JOIN {org_projactivities} a on a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
        $activity = db_result(db_query($sql, $parvalue));
        $form['projactivity']['#default_value'] = $activity;
        break;
      default:
      }
    }
  }
  // Suppress the formating information
  unset($form['body_filter']['filter']);
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add Report';
    $form['#goto'] = 'project/projreports/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

/**
 * Form validate routines, called from project_form_validate() in project.module
 */
function project_project_form_validate($form, &$form_state) {
  //orgright_debug_msg('project','Fn: project_project_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $project = strip_tags($form_state['values']['project']);
    $manager = strip_tags($form_state['values']['manager']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($project != $form_state['values']['project']) {
        form_set_error('project',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($manager != $form_state['values']['manager']) {
        form_set_error('address',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "manager" is a valid member
    // Load the additional file to handle member name validation
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($manager);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('manager',t('The project manager specified is not a known member'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.project = '%s'";
    $nid = db_result(db_query($sql, $project));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('project',t('The project name is already in use'));
      }
    }
    // If the date-type or start/finish date has changed, then flag project for schedule re-calulation
    if (($form_state['values']['projdatype'] != $form_state['values']['save_projdatype']) ||
        (substr($form_state['values']['projdate'],0,10) != $form_state['values']['save_projdate'])) {
      form_set_value($form['recalc'], TRUE, $form_state);
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Project: !proj', array('!proj'=>$project));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached activities - delete not permitted in this case
    $sql = "SELECT COUNT(a.nid) from {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.pnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('project',t('Cannot delete - this project has @num activities attached!', array('@num'=>$count)));
    }
    // Check for attached project reports - delete not permitted in this case
    $sql = "SELECT COUNT(r.nid) from {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.pnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('project',t('Cannot delete - this project has @num reports attached!', array('@num'=>$count)));
    }
  }
  //orgright_debug_dump('form_state',$form_state);
}

function project_projactivity_form_validate($form, &$form_state) {
  //orgright_debug_msg('project','Fn: project_projactivity_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $activity = strip_tags($form_state['values']['activity']);
    $project = strip_tags($form_state['values']['project']);
    $member = strip_tags($form_state['values']['member']);
    $duration = strip_tags($form_state['values']['duration']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($activity != $form_state['values']['activity']) {
        form_set_error('scode',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($project != $form_state['values']['project']) {
        form_set_error('amount',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($member != $form_state['values']['member']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($duration != $form_state['values']['duration']) {
        form_set_error('duration',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "project" is valid
    $pnid = project_validate_project($project);
    if ($pnid) {
      form_set_value($form['pnid'], $pnid, $form_state);
    }
    else {
      form_set_error('project',t('The name specified is not a known project'));
    }
    // Check that the name returned in "member" is a valid member
    // Load the additional file to handle member name validation
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The responsible person specified is not a known member'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.activity = '%s' AND a.pnid = %d";
    $nid = db_result(db_query($sql, $activity, $pnid));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('activity',t('Activity already exists for this project'));
      }
    }
    // If the duration or dates have changed, then flag project for a schedule re-calulation
    if (($form_state['values']['duration'] != $form_state['values']['save_duration']) ||
        ($form_state['values']['duratype'] != $form_state['values']['save_duratype']) ||
        ($form_state['values']['actividatype'] != $form_state['values']['save_actividatype']) ||
        (substr($form_state['values']['actividate'],0,10) != $form_state['values']['save_actividate']) ||
        (substr($form_state['values']['actualstart'],0,10) != $form_state['values']['save_actualstart']) ||
        (substr($form_state['values']['actualfinish'],0,10) != $form_state['values']['save_actualfinish'])) {
      form_set_value($form['recalc'], TRUE, $form_state);
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Project activity: !proj - !act', array('!proj'=>$project, '!act'=>$activity));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached reports - delete not permitted in this case
    $sql = "SELECT COUNT(r.nid) from {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.anid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('activity',t('Cannot delete - this activity still has @num reports attached!', array('@num'=>$count)));
    }
  }
}

function project_projreport_form_validate($form, &$form_state) {
  //orgright_debug_msg('project','Fn: project_projreport_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $projreport = strip_tags($form_state['values']['projreport']);
    $project = strip_tags($form_state['values']['project']);
    $activity = strip_tags($form_state['values']['activity']);
    $member = strip_tags($form_state['values']['member']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($projreport != $form_state['values']['projreport']) {
        form_set_error('projreport',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($project != $form_state['values']['project']) {
        form_set_error('project',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($activity != $form_state['values']['activity']) {
        form_set_error('activity',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Prepare date values
    $projrepdate = date_format_date(date_make_date($form_state['values']['projrepdate']), 'custom', 'd M Y');
    // Check that the name returned in "project" is valid
    $pnid = project_validate_project($project);
    if ($pnid) {
      form_set_value($form['pnid'], $pnid, $form_state);
    }
    else {
      form_set_error('project',t('The name specified is not a known project'));
    }
    // Check that any name returned in "activity" is valid
    if ($activity) {
      $anid = project_validate_activity($activity);
      if ($anid) {
        // check that the returned activity belongs to the specified project
        $sql = "SELECT pnid FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
        if ($pnid == db_result(db_query($sql, $anid))) {
          form_set_value($form['anid'], $anid, $form_state);
        }
        else {
          form_set_error('activity',t('The activity selected does not belong to the specified project'));
        }
      }
      else {
        form_set_error('activity',t('The name specified is not a known activity'));
      }
    }
    else { $anid = 0; }
    // Check that the name returned in "member" is a valid member
    // Load the additional file to handle member name validation
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The person specified is not a known member'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.projreport = '%s' AND r.pnid = %d AND r.anid = %d";
    $nid = db_result(db_query($sql, $projreport, $pnid, $anid));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('projreport',t('Report name in use for this project/activity'));
      }
    }
    // Build a composite node title and save in $form_state
    $args = array('!rpt'=>$projreport, '@proj'=>$project, '!act'=>$activity, '@date'=>$projrepdate);
    if ($activity) {
      $form_state['values']['title'] = t('Project activity report: !rpt for !proj - !act on !date', $args);
    }
    else {
      $form_state['values']['title'] = t('Project report: !rpt for !proj on !date', $args);
    }
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //orgright_debug_msg('project','--- Delete button clicked ---');
  }
}

function project_calculate_schedule_form_validate($form, &$form_state) {
  // validate callback function specified for form
  //orgright_debug_msg('project','Fn: project_calculate_schedule_form_validate');
  if ($form_state['storage']['initialise']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $project = strip_tags($form_state['values']['project']);
    // Validate the project and first activity
    if ($project != $form_state['values']['project']) {
      form_set_error('project',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "project" is valid
    $pnid = project_validate_project($project);
    if ($pnid) {
      form_set_value($form['pnid'], $pnid, $form_state);
    }
    else {
      form_set_error('project',t('The name specified is not a known project'));
    }
    // If there have been no errors, then rebuild form with extra fields
    if (!form_get_errors()) {
      $form_state['storage']['pnid'] = $form_state['values']['pnid'];
      $form_state['storage']['project'] = $form_state['values']['project'];
      $form_state['storage']['initialise'] = FALSE;
      $form_state['rebuild'] = TRUE;
    }
    return;
  }
  // now any further validation of project date constraint
}

function project_activity_links_form_validate($form, &$form_state) {
  // validate callback function specified for form
  //orgright_debug_msg('project','Fn: project_activity_links_form_validate');
  //orgright_debug_dump('$form_state',$form_state);
  //orgright_debug_dump('$form',$form);
  // Test whether the user has clicked the "change activity" button
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['change_button']['#value']) {
    unset($form_state['storage']['anid']);
    unset($form_state['storage']['activity']);
    $form_state['storage']['change_activity'] = TRUE;
    $form_state['rebuild'] = TRUE;
    //orgright_debug_msg('project','change activity button clicked');
    return;
  }
  // else a normal submit, so check values
  if ($form_state['storage']['initialise']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $project = strip_tags($form_state['values']['project']);
    $activity = strip_tags($form_state['values']['activity']);
    // Validate the project and first activity
    if ($project != $form_state['values']['project']) {
      form_set_error('project',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($activity != $form_state['values']['activity']) {
      form_set_error('activity',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "project" is valid
    $pnid = project_validate_project($project);
    if ($pnid) {
      form_set_value($form['pnid'], $pnid, $form_state);
    }
    else {
      form_set_error('project',t('The name specified is not a known project'));
    }
    // Check that any name returned in "activity" is valid
    $anid = project_validate_activity($activity);
    if ($anid) {
      // check that the returned activity belongs to the specified project
      $sql = "SELECT pnid FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
      if (db_result(db_query($sql, $anid)) == $pnid) {
        form_set_value($form['anid'], $anid, $form_state);
      }
      else {
        form_set_error('activity',t('The activity selected does not belong to the specified project'));
      }
    }
    else {
      form_set_error('activity',t('The name specified is not a known activity'));
    }
    // If there have been no errors, then rebuild form with extra fields
    if (!form_get_errors()) {
      $form_state['storage']['pnid'] = $form_state['values']['pnid'];
      $form_state['storage']['project'] = $form_state['values']['project'];
      $form_state['storage']['anid'] = $form_state['values']['anid'];
      $form_state['storage']['activity'] = $form_state['values']['activity'];
      $form_state['storage']['initialise'] = FALSE;
      $form_state['rebuild'] = TRUE;
    }
    return;
  }
  if ($form_state['storage']['change_activity']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $activity = strip_tags($form_state['values']['activity']);
    // Validate first activity
    if ($activity != $form_state['values']['activity']) {
      form_set_error('activity',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that any name returned in "activity" is valid
    $anid = project_validate_activity($activity);
    if ($anid) {
      // check that the returned activity belongs to the specified project
      $sql = "SELECT pnid FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
      if (db_result(db_query($sql, $anid)) == $form_state['storage']['pnid']) {
        form_set_value($form['anid'], $anid, $form_state);
      }
      else {
        form_set_error('activity',t('The activity selected does not belong to the specified project'));
      }
    }
    else {
      form_set_error('activity',t('The name specified is not a known activity'));
    }
    // If there have been no errors, then rebuild form with extra fields
    if (!form_get_errors()) {
      $form_state['storage']['anid'] = $form_state['values']['anid'];
      $form_state['storage']['activity'] = $form_state['values']['activity'];
      $form_state['storage']['change_activity'] = FALSE;
      $form_state['rebuild'] = TRUE;
    }
    return;
  }
  // else validate the complete form
}

function project_validate_project($project) {
  // Check that the value passed in "$project" is valid, and return the associated nid
  //orgright_debug_msg('project',t('Validate project name: @mbr', array('@mbr' => $project)));
  // Query the database to ensure that the project name is valid, and to find the mnid
  $sql = "SELECT nid FROM {org_projects} WHERE project = '%s'";
  //$args = array($project);
  //$result = db_query(db_rewrite_sql($sql, '{org_projects}', 'vid', $args));
  return db_result(db_query($sql, $project));
}

function project_validate_activity($activity) {
  // Check that the value passed in "$activity" is valid, and return the associated nid
  //orgright_debug_msg('project',t('Validate activity name: @mbr', array('@mbr' => $project)));
  // Query the database to ensure that the activity name is valid, and to find the nid
  $sql = "SELECT nid FROM {org_projactivities} WHERE activity = '%s'";
  //$args = array($activity);
  //$result = db_query(db_rewrite_sql($sql, '{org_projects}', 'vid', $args));
  return db_result(db_query($sql, $activity));
}

/**
 * Form submit routines, called from project_form_submit() in project.module
 */
function project_project_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('project','Fn: project_project_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function project_projactivity_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('project','Fn: project_projactivity_form_submit');
  // If dates on this activity have changed, then set the flag on the
  // parent project to force a re-calulation of the schedule
  if ($form_state['values']['recalc']) {
    //orgright_debug_msg('project','need to update project schedule flag!',7);
    db_query("UPDATE {node} n JOIN {org_projects} p ON p.nid = n.nid SET p.recalc = TRUE WHERE p.vid = n.vid AND p.nid = %d", $form_state['values']['pnid']);
    //orgright_debug_dump('form_state', $form_state);
  }
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function project_projreport_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('project','Fn: project_projreport_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function project_calculate_schedule_form_submit($form, &$form_state) {
  // submit callback function specified for form
  //orgright_debug_msg('project','Fn: project_calculate_schedule_form_submit');
  // Load the additional file to handle project schedule re-calculation
  module_load_include('inc', 'project', 'project_calc'); 
  // recalculate the schedule, will return a graphic picture of scheule
  $pnid = $form_state['storage']['pnid'];
  $projdatype = $form_state['values']['projdatype'];
  $projdate = substr($form_state['values']['projdate'], 0, 10);
  $graphic = project_recalculate_schedule($pnid, $projdatype, $projdate);
  // save graphic and indicate that the project does not require re-calculation
  db_query("UPDATE {node} n JOIN {org_projects} p ON p.nid = n.nid SET p.projdatype = '%s', p.projdate = '%s', p.recalc = FALSE, projsched = '%s' WHERE p.vid = n.vid AND p.nid = %d", $projdatype, $projdate, $graphic, $pnid);
  // clear form_state storage and set redirect to view the project
  $form_state['storage'] = array();
  $form_state['redirect'] = 'node/'.$pnid;
}

function project_activity_links_form_submit($form, &$form_state) {
  // Callback specified for the submit button
  //orgright_debug_msg('project','Fn: project_activity_links_form_submit');
  // Now save all of the activity links
  $project = $form_state['storage']['project'];
  $activity = $form_state['storage']['activity'];
  $first = $form_state['storage']['anid'];
  foreach($form_state['values'] as $field => $value) {
    if (substr($field,0,6) == 'activi') {
      $second = substr($field,6);
      if ($value == 'N/A') {
        // delete any link between these two activities
        db_query("DELETE FROM {org_projactivitylinks} WHERE (anid1 = %d AND anid2 = %d) OR (anid1 = %d AND anid2 = %d)", $first, $second, $second, $first);
      }
      else {
        // first normalise the link specification
        if ($first < $second) {
          $linkarray = array('1st'=>$first, '2nd'=>$second, 'link'=>$value);
        }
        else {
          $linkarray = project_invert_link($first, $second, $value);
        }
        // create or update a link between these two activities
        if (isset($form_state['storage']['activity_links'][$second])) {
          // update existing link
          db_query("UPDATE {org_projactivitylinks} SET link = '%s' WHERE anid1 = %d AND anid2 = %d",
            $linkarray['link'],
            $linkarray['1st'],
            $linkarray['2nd']);
        }
        else {
          // insert new link
          global $user;
          db_query("INSERT INTO {org_projactivitylinks} (anid1, anid2, link, pnid, notes, user) VALUES (%d, %d, '%s', %d, '%s', %d)",
            $linkarray['1st'],
            $linkarray['2nd'],
            $linkarray['link'],
            $form_state['storage']['pnid'],
            '',
            $user->uid);
        }
      }
    } 
  }
  // now flag the project for re-calulation
  db_query("UPDATE {node} n JOIN {org_projects} p ON p.nid = n.nid SET p.recalc = TRUE WHERE p.vid = n.vid AND p.nid = %d", $form_state['storage']['pnid']);
}

