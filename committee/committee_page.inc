<?php
// $Id$

/**
 * @file
 * Provides additional functions for committee module.
 */

/**
 * Called from hook_view() in module file
 */
function committee_committee_view(&$node, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_committee_view');
  // Get metadata for this node type - used for labeling body field
  $nodetype = node_get_types('type', $node);

  $description = t('Committee name');
  $node->content['committee'] = array(
    '#value' => committee_theme_item('committee', $description, $node->committee),
    '#weight' => -1,
  );
  // the body always has a '#weight' of 0
  //$node->content['body']['#weight'] = 0;
  $description = t('Parent committee');
  $node->content['parent'] = array(
    '#value' => committee_theme_item('parent', $description, $node->parent, $node->pnid),
    '#weight' => 5,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => committee_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Check whether there are any sub-committee details
  if (isset($node->subcommittees)) {
    //orgright_debug_msg('committee','subcommittee details found');
    $heading = t('The following sub-committees are attached to this committee');
    $node->content['subcommittees'] = array(
      '#value' => committee_theme_heading('subcommittee', $heading),
      '#weight' => $weight,
    );
    foreach ($node->subcommittees as $this_subcommittee) {
      $subitem = $this_subcommittee['committee'];
      $ref = $this_subcommittee['nid'];
      $attributes = array();
      ++$weight;
      $node->content['subcommittees'][] = array(
        '#value' => committee_theme_subitem('subcommittee', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new sub-committee
  ++$weight;
  $linktext = l(t('Add new sub-committee'), 'committee/committees/add/parent~' . $node->nid);
  $node->content['add-subcommittee'] = array(
    '#value' => committee_theme_link('add-subcommittee', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any role details
  if (isset($node->roles)) {
    //orgright_debug_msg('committee','role details found');
    $heading = t('The following roles are defined for this committee');
    ++$weight;
    $node->content['roles'] = array(
      '#value' => committee_theme_heading('roles', $heading),
      '#weight' => $weight,
    );
    foreach ($node->roles as $this_role) {
      $subitem = $this_role['role'];
      $ref = $this_role['nid'];
      $attributes = array();
      if (isset($this_role['assignment'])) {
        foreach ($this_role['assignment'] as $this_assignment) {
          $attributes[] = ' : ' . l($this_assignment['member'], 'node/' . $this_assignment['mnid']);
          $attributes[] = ', ' . strtolower($this_assignment['assigntype']) . ' on ' . $this_assignment['assigndate'];
        }
      }
      else {
        $attributes[] = '&nbsp;';
      }
      ++$weight;
      $node->content['roles'][] = array(
        '#value' => committee_theme_subitem('roles', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new role
  ++$weight;
  $linktext = l(t('Add new committee role'), 'committee/roles/add/committee~' . $node->nid);
  $node->content['add-role'] = array(
    '#value' => committee_theme_link('add-role', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any appointment details
  if (isset($node->appointments)) {
    //orgright_debug_msg('committee','appointment details found');
    $heading = t('The following appointment details are held for this committee');
    ++$weight;
    $node->content['appointments'] = array(
      '#value' => committee_theme_heading('appointments', $heading),
      '#weight' => $weight,
    );
    foreach ($node->appointments as $this_appointment) {
      $adate = date_make_date($this_appointment['appointdate']);
      $appointdate = date_format_date($adate, 'custom', 'd M Y');
      $subitem = 'Appointment';
      $ref = $this_appointment['nid'];
      $attributes = array(
        ': ' . l($this_appointment['member'], 'node/' . $this_appointment['mnid']),
        ', ' . strtolower($this_appointment['appointtype']) . ' on ' . $appointdate,
      );
      ++$weight;
      $node->content['appointments'][] = array(
        '#value' => committee_theme_subitem('appointments', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new appointment
  ++$weight;
  $linktext = l(t('Add new appointment'), 'committee/appointments/add/committee~' . $node->nid);
  $node->content['add-appointment'] = array(
    '#value' => committee_theme_link('add-appointment', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee') => 'committee', t('Committees') => 'committee/committees'));
}

function committee_role_view(&$node, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_role_view');
  $description = t('Role name');
  $node->content['role'] = array(
    '#value' => committee_theme_item('role', $description, $node->role),
    '#weight' => -2,
  );
  $description = t('Committee name');
  $node->content['committee'] = array(
    '#value' => committee_theme_item('committee', $description, $node->committee, $node->cnid),
    '#weight' => -1,
  );
  // the body always has a '#weight' of 0
  //$node->content['body']['#weight'] = 0;
  $description = t('Multiple persons');
  $node->content['multiperson'] = array(
    '#value' => committee_theme_item('multiperson', $description, ($node->multiperson ? 'Yes' : 'No')),
    '#weight' => 5,
  );
  $description = t('Allow vacant');
  $node->content['allowvacant'] = array(
    '#value' => committee_theme_item('allowvacant', $description, ($node->allowvacant ? 'Yes' : 'No')),
    '#weight' => 6,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => committee_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Check whether there are any assignment details
  if (isset($node->assignments)) {
    //orgright_debug_msg('committee','assignment details found');
    $today = orgright_get_date_ymd();
    $heading = t('The following assignment details are held for this role');
    ++$weight;
    $node->content['assignments'] = array(
      '#value' => committee_theme_heading('assignments', $heading),
      '#weight' => $weight,
    );
    foreach ($node->assignments as $this_assignment) {
      $assigndate = date_format_date(date_make_date($this_assignment['assigndate']), 'custom', 'd M Y');
      if ($this_assignment['ceasedate']) {
        $ceasedate = date_format_date(date_make_date($this_assignment['ceasedate']), 'custom', 'd M Y');
      }
      else {
        $ceasedate = '';
      }
      $subitem = 'Assignment';
      if ($this_assignment['assigndate'] > $today) {
        $subitem = 'Pending assignment';
      }
      $ref = $this_assignment['nid'];
      $attributes = array(
        ': ' . l($this_assignment['member'], 'node/' . $this_assignment['mnid']),
        ', ' . strtolower($this_assignment['assigntype']),
        ' on ' . $assigndate,
      );
      if ($ceasedate) {
        $attributes[] = (($this_assignment['ceasedate'] > $today) ? ', will cease on ' : ', ceased ') . $ceasedate;
      }
      ++$weight;
      $node->content['assignments'][] = array(
        '#value' => committee_theme_subitem('assignments', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new assignment
  ++$weight;
  $linktext = l(t('Add new assignment'), 'committee/assignments/add/role~' . $node->nid . '/committee~' . $node->cnid);
  $node->content['add-assignment'] = array(
    '#value' => committee_theme_link('add-assignment', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee') => 'committee', t('Roles') => 'committee/roles'));
}

function committee_appointment_view(&$node, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_appointment_view');
  $description = t('Member');
  $node->content['member'] = array(
    '#value' => committee_theme_item('member', $description, $node->member, $node->mnid),
    '#weight' => 1,
  );
  $description = t('Committee name');
  $node->content['committee'] = array(
    '#value' => committee_theme_item('committee', $description, $node->committee, $node->cnid),
    '#weight' => 2,
  );
  $description = t('Appointment type');
  $node->content['appointtype'] = array(
    '#value' => committee_theme_item('appointtype', $description, $node->appointtype),
    '#weight' => 3,
  );
  // Prepare the appointdate for display
  $adate = date_make_date($node->appointdate);
  $appointed = date_format_date($adate, 'custom', 'd M Y');
  $description = t('Start date');
  $node->content['appointed'] = array(
    '#value' => committee_theme_item('appointed', $description, $appointed),
    '#weight' => 5,
  );
  // Prepare the retiredate for display
  $rdate = date_make_date($node->retiredate);
  if ($rdate) {
    $retired = date_format_date($rdate, 'custom', 'd M Y');
    $description = t('End date');
    $node->content['retired'] = array(
      '#value' => committee_theme_item('retired', $description, $retired),
      '#weight' => 6,
    );
  }
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => committee_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee') => 'committee', t('Appointments') => 'committee/appointments'));
}

function committee_assignment_view(&$node, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_assignment_view');
  $description = t('Member');
  $node->content['member'] = array(
    '#value' => committee_theme_item('member', $description, $node->member, $node->mnid),
    '#weight' => 1,
  );
  $description = t('Role');
  $node->content['role'] = array(
    '#value' => committee_theme_item('role', $description, $node->role, $node->rnid),
    '#weight' => 2,
  );
  $description = t('Committee name');
  $node->content['committee'] = array(
    '#value' => committee_theme_item('committee', $description, $node->committee, $node->cnid),
    '#weight' => 3,
  );
  $description = t('Assignment type');
  $node->content['assigntype'] = array(
    '#value' => committee_theme_item('assigntype', $description, $node->assigntype),
    '#weight' => 5,
  );
  // Prepare the date assigndate for display
  $adate = date_make_date($node->assigndate);
  $assigned = date_format_date($adate, 'custom', 'd M Y');
  $description = t('Assignment date');
  $node->content['assigned'] = array(
    '#value' => committee_theme_item('assigned', $description, $assigned),
    '#weight' => 6,
  );
  // Prepare the date ceasedate for display
  $cdate = date_make_date($node->ceasedate);
  if ($cdate) {
    $ceased = date_format_date($cdate, 'custom', 'd M Y');
    $description = t('Date ceased');
    $node->content['ceased'] = array(
      '#value' => committee_theme_item('ceased', $description, $ceased),
      '#weight' => 7,
    );
  }
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => committee_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee') => 'committee', t('Assignments') => 'committee/assignments'));
}

/**
 * Specific theming functions called when items are being prepared for display
 */
function committee_theme_heading($heading_class, $heading_value) {
  return theme('orgright_heading', 'committee', $heading_class, $heading_value);
}

function committee_theme_item($item_class, $description, $item_value, $node_ref = 0) {
  // General function for themeing an item value to be displayed.
  return theme('orgright_item', 'committee', $item_class, $description, $item_value, $node_ref);
}

function committee_theme_subitem($subitem_class, $subitem_value, $node_ref, $attributes) {
  // General function for themeing a sub-item set of values to be displayed.
  return theme('orgright_subitem', 'committee', $subitem_class, $subitem_value, $node_ref, $attributes);
}

function committee_theme_link($link_class, $link_value) {
  return theme('orgright_link', 'committee', $link_class, $link_value);
}
