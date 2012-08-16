<?php
// $Id$

/**
 * @file
 * Provides additional functions for project module.
 */

/**
 * Called from hook_view() in module file
 */
function project_project_view(&$node, $teaser, $page) {
  // Get metadata for this node type - used for labeling body field
  // note: the $node->content['body'] will always have a weight of 0.
  $nodetype = node_get_types('type', $node);
  $description = t('Name');
  $node->content['project'] = array(
    '#value' => project_theme_item('project', $description, $node->project),
    '#weight' => -3,
  );
  $description = t('Manager');
  $node->content['manager'] = array(
    '#value' => project_theme_item('manager', $description, $node->manager, $node->mnid),
    '#weight' => -2,
  );
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => project_theme_item('body-label', $description, ''),
    '#weight' => -1,
  );
  $description = t('Category');
  $node->content['projcategory'] = array(
    '#value' => project_theme_item('projcategory', $description, $node->projcategory),
    '#weight' => 3,
  );
  $description = t('Project status');
  $node->content['projstatus'] = array(
    '#value' => project_theme_item('projstatus', $description, $node->projstatus),
    '#weight' => 4,
  );
  //orgright_debug_msg('project','projdatype='.$node->projdatype);
  $description = t('Project date constraint');
  $node->content['projdatype'] = array(
    '#value' => project_theme_item('projdatype', $description, $node->projdatype),
    '#weight' => 5,
  );
  // Prepare the project date for display
  $projdate = date_format_date(date_make_date($node->projdate), 'custom', 'd M Y');
  $description = t('Project date');
  $node->content['projdate'] = array(
    '#value' => project_theme_item('projdate', $description, $projdate),
    '#weight' => 6,
  );
  $description = t('Re-calculate schedule?');
  $node->content['recalc'] = array(
    '#value' => project_theme_item('recalc', $description, $node->recalc ? 'Yes' : 'No'),
    '#weight' => 7,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) { return; }
  // else continue adding more detail with project schedule
  $description = t('Project schedule?');
  $node->content['projsched'] = array(
    '#value' => project_theme_item('projsched', $description, $node->projsched),
    '#weight' => 10,
  );
  // add link to re-caclulate project schedule
  $linktext = l(t('Re-calulate project schedule'), 'project/projects/schedule/project~'.$node->nid);
  $node->content['recalc-sched'] = array(
    '#value' => project_theme_link('recalc-sched', $linktext),
    '#weight' => 11,
  );
  // and notes
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => project_theme_item('notes', $description, $notes),
    '#weight' => 12,
  );
  $weight = 20;
  // Check whether there are any activity details
  if (isset($node->activities)) {
    //orgright_debug_msg('project','activity details found');
    $heading = t('The following activities are attached to this project');
    ++$weight;
    $node->content['activities'] = array(
      '#value' => project_theme_heading('activities', $heading),
      '#weight' => $weight,
    );
    foreach ($node->activities as $node->this_activity) {
      // Prepare the activity dateconstraint for display
      $actividate = date_format_date(date_make_date($node->this_activity['actividate']), 'custom', 'd M Y');
      $subitem = $node->this_activity['activity'];
      $ref = $node->this_activity['nid'];
      $attributes = array(
        ': ' . l($node->this_activity['member'], 'node/'.$node->this_activity['mnid']),
        ', ' . $node->this_activity['activistatus'],
        ', ' . $node->this_activity['duration'],
        ', ' . $node->this_activity['duratype'],
        ', ' . $node->this_activity['actividatype'],
        ', ' . $actividate,
      );
      ++$weight;
      $node->content['activities'][] = array(
        '#value' => project_theme_subitem('activities', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new activity
  ++$weight;
  $linktext = l(t('Add new activity'), 'project/projactivities/add/project~'.$node->nid);
  $node->content['add-activity'] = array(
    '#value' => project_theme_link('add-activity', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any project reports
  if (isset($node->reports)) {
    //orgright_debug_msg('project','project report details found');
    $heading = t('The following project reports are held for this project');
    ++$weight;
    $node->content['reports'] = array(
      '#value' => project_theme_heading('reports', $heading),
      '#weight' => $weight,
    );
    foreach ($node->reports as $node->this_report) {
      // Prepare the date projrepdate for display
      $projrepdate = date_format_date(date_make_date($node->this_report['projrepdate']), 'custom', 'd M Y');
      $subitem = $node->this_report['projreport'];
      $ref = $node->this_report['nid'];
      $attributes = array(
        ' by ' . l($node->this_report['member'], 'node/'.$node->this_report['mnid']),
        ' on ' . $projrepdate,
      );
      // Add a reference to the activity to the front of the array if required
      if ($node->this_report['anid']) {
        array_unshift($attributes, ' for '.l($node->this_report['activity'], 'node/'.$node->this_report['anid']));
      }
      ++$weight;
      $node->content['reports'][] = array(
        '#value' => project_theme_subitem('reports', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new report
  ++$weight;
  $linktext = l(t('Add new report'), 'project/projreports/add/project~'.$node->nid);
  $node->content['add-report'] = array(
    '#value' => project_theme_link('add-report', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Projects')=>'project/projects'));
}

function project_projactivity_view(&$node, $teaser, $page) {
  // Get metadata for this node type - used for labeling body field
  // note: the $node->content['body'] will always have a weight of 0.
  $nodetype = node_get_types('type', $node);
  $description = t('Project name');
  $node->content['project'] = array(
    '#value' => project_theme_item('project', $description, $node->project, $node->pnid),
    '#weight' => -4,
  );
  $description = t('Activity');
  $node->content['activity'] = array(
    '#value' => project_theme_item('activity', $description, $node->activity),
    '#weight' => -3,
  );
  $description = t('Responsible person');
  $node->content['member'] = array(
    '#value' => project_theme_item('member', $description, $node->member, $node->mnid),
    '#weight' => -2,
  );
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => project_theme_heading('body-label', $description),
    '#weight' => -1,
  );
  $description = t('Activity category');
  $node->content['activicategory'] = array(
    '#value' => project_theme_item('activicategory', $description, $node->activicategory),
    '#weight' => 1,
  );
  $description = t('Activity status');
  $node->content['activistatus'] = array(
    '#value' => project_theme_item('activistatus', $description, $node->activistatus),
    '#weight' => 2,
  );
  $description = t('Duration');
  $node->content['duration'] = array(
    '#value' => project_theme_item('duration', $description, $node->duration),
    '#weight' => 3,
  );
  $description = t('Time units');
  $node->content['time-units'] = array(
    '#value' => project_theme_item('time-units', $description, $node->duratype),
    '#weight' => 4,
  );
  $description = t('Constraint type');
  $node->content['actividatype'] = array(
    '#value' => project_theme_item('actividatype', $description, $node->actividatype),
    '#weight' => 5,
  );
  // Prepare the constraint date for display
  $actividate = date_format_date(date_make_date($node->actividate), 'custom', 'd M Y');
  $description = t('Constraint date');
  $node->content['actividate'] = array(
    '#value' => project_theme_item('actividate', $description, $actividate),
    '#weight' => 6,
  );
  // Prepare the actualstart date for display
  $actualstart = date_format_date(date_make_date($node->actualstart), 'custom', 'd M Y');
  $description = t('Actual start');
  $node->content['actualstart'] = array(
    '#value' => project_theme_item('actualstart', $description, $actualstart),
    '#weight' => 7,
  );
  // Prepare the actualfinish date for display
  $actualfinish = date_format_date(date_make_date($node->actualfinish), 'custom', 'd M Y');
  $description = t('Actual finish');
  $node->content['actualfinish'] = array(
    '#value' => project_theme_item('actualfinish', $description, $actualfinish),
    '#weight' => 8,
  );
  // Prepare the schedstart date for display
  $schedstart = date_format_date(date_make_date($node->schedstart), 'custom', 'd M Y');
  $description = t('Scheduled start');
  $node->content['schedstart'] = array(
    '#value' => project_theme_item('schedstart', $description, $schedstart),
    '#weight' => 9,
  );
  // Prepare the schedfinish date for display
  $schedfinish = date_format_date(date_make_date($node->schedfinish), 'custom', 'd M Y');
  $description = t('Scheduled finish');
  $node->content['schedfinish'] = array(
    '#value' => project_theme_item('schedfinish', $description, $schedfinish),
    '#weight' => 10,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) { return; }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => project_theme_item('notes', $description, $notes),
    '#weight' => 11,
  );
  $weight = 12;
  // Include a link to link other project activities to this one
  ++$weight;
  $linktext = l(t('Link project activities'), 'project/projactivities/links/project~'.$node->pnid.'/activity~'.$node->nid);
  $node->content['link-activities'] = array(
    '#value' => project_theme_link('link-activities', $linktext),
    '#weight' => $weight,
  );
  // Include a link to add new activity
  ++$weight;
  $linktext = l(t('Add new activity'), 'project/projactivities/add/project~'.$node->pnid);
  $node->content['add-activity'] = array(
    '#value' => project_theme_link('add-activity', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any projreport details
  $weight = 20;
  if (isset($node->reports)) {
    //orgright_debug_msg('project','project report details found');
    $heading = t('The following project reports are held for this activity');
      ++$weight;
    $node->content['reports'] = array(
      '#value' => project_theme_heading('reports', $heading),
      '#weight' => $weight,
    );
    foreach ($node->projreports as $node->this_report) {
      // Prepare the date datepaid for display
      $projrepdate = date_format_date(date_make_date($node->this_report['projrepdate']), 'custom', 'd M Y');
      $subitem = $node->this_report['report'];
      $ref = $node->this_report['nid'];
      $attributes = array(
        ' by ' . $node->member,
        ' on ' . $projrepdate,
      );
      ++$weight;
      $node->content['reports'][] = array(
        '#value' => project_theme_subitem('reports', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  else {
    //orgright_debug_msg('project','no payment details');
  }
  // Include a link to add new report
  ++$weight;
  $linktext = l(t('Add new report'), 'project/projreports/add/activity~'.$node->nid.'/project~'.$node->pnid);
  $node->content['add-report'] = array(
    '#value' => project_theme_link('add-report', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Project activities')=>'project/projactivities'));
}

function project_projreport_view(&$node, $teaser, $page) {
  // Get metadata for this node type - used for labeling body field
  // note: the $node->content['body'] will always have a weight of 0.
  $nodetype = node_get_types('type', $node);
  $description = t('Project name');
  $node->content['project'] = array(
    '#value' => project_theme_item('project', $description, $node->project, $node->pnid),
    '#weight' => -3,
  );
  if ($node->anid) {
    $description = t('Activity');
    $node->content['activity'] = array(
      '#value' => project_theme_item('activity', $description, $node->activity, $node->anid),
      '#weight' => -2,
    );
  }
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => project_theme_item('body-label', $description, ''),
    '#weight' => -1,
  );
  $description = t('From');
  $node->content['member'] = array(
    '#value' => project_theme_item('member', $description, $node->member, $node->mnid),
    '#weight' => 4,
  );
  // Prepare the report date for display
  $projrepdate = date_format_date(date_make_date($node->projrepdate), 'custom', 'd M Y');
  $description = t('Report date');
  $node->content['projrepdate'] = array(
    '#value' => project_theme_item('projrepdate', $description, $projrepdate),
    '#weight' => 6,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) { return; }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => project_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project', t('Project reports')=>'project/projreports'));
}

/**
 * Specific theming functions
 */
function project_theme_heading($heading_class, $heading_value) {
  return theme('orgright_heading', 'project', $heading_class, $heading_value);
}

function project_theme_item($item_class, $description, $item_value, $node_ref = 0) {
  // General function for themeing an item value to be displayed.
  return theme('orgright_item', 'project', $item_class, $description, $item_value, $node_ref);
}

function project_theme_subitem($subitem_class, $subitem_value, $node_ref, $attributes) {
  // General function for themeing a sub-item set of values to be displayed.
  return theme('orgright_subitem', 'project', $subitem_class, $subitem_value, $node_ref, $attributes);
}

function project_theme_link($link_class, $link_value) {
  return theme('orgright_link', 'project', $link_class, $link_value);
}
