<?php
// $Id$

/**
 * @file
 * Provides additional functions for committee module.
 */

/**
 * Menu call back functions to display pages to users
 */
function committee_management() {
  // Home page of the committee management function - display links and description
  $output = '<p>The following committee management functions are available:</p>';
  $output .= '<h2>' . l('Manage Committees', 'committee/committees') . '</h2>';
  $output .= '<p>List, add, update or delete committees from the organisation\'s database.</p>';
  $output .= '<h2>' . l('Manage Committee Roles', 'committee/roles') . '</h2>';
  $output .= '<p>List, add, update or delete committee roles from the organisation\'s database.</p>';
  $output .= '<h2>' . l('Manage Committee Appointments', 'committee/appointments') . '</h2>';
  $output .= '<p>List, add, update or delete appointments of members to partcular committees.</p>';
  $output .= '<h2>' . l('Manage Role Assignments', 'committee/assignments') . '</h2>';
  $output .= '<p>List, add, update or delete assignments of members to specified committee roles.</p>';
  // set the breadcrumb trail
  orgright_breadcrumb(array());
  return $output;
}

function committee_committees() {
  // entry page for managing committees
  return committee_committees_list();
}

function committee_roles() {
  // entry page for managing roles
  return committee_roles_list();
}

function committee_appointments() {
  // entry page for managing appointments
  return committee_appointments_list();
}

function committee_assignments() {
  // entry page for managing appointment payments
  return committee_assignments_list();
}

function committee_committees_list() {
  // Generate a committees list
  $output = '';
  $sql = "SELECT c.nid, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid ORDER BY c.nid";
  $result = db_query(db_rewrite_sql($sql));
  while ($committee = db_fetch_object($result)) {
    // Generate a line of listing for this committee
    $committee_output = '';
    $committee_output .= l($committee->committee, 'node/'.$committee->nid) . ' ';
    $committee_output .= l('[edit]', 'node/'.$committee->nid.'/edit');
    $today = orgright_get_date_ymd();
    $app_output = '';
    // Prepare a list of members currently appointed to this committee
    $sql = "SELECT p.nid, p.mnid, p.appointtype, p.appointdate FROM {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.cnid = %d ORDER BY p.appointdate";
    $appointments = db_query(db_rewrite_sql($sql), $committee->nid);
    while ($appt = db_fetch_object($appointments)) {
      // ignore retired appointments
      if ($appt->retiredate && $appt->retiredate < $today) { continue; }
      // Extract the member name
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_fetch_object(db_query($sql, $appt->mnid));
      // Prepare the appointment date for display
      $dateappoint = date_format_date(date_make_date($appt->appointdate), 'custom', 'd M Y');
      $app_output .= '<li>' . l($member->member, 'node/'.$appt->mnid);
      $app_output .= ' ' . strtolower($appt->appointtype);
      $app_output .= ' on ' . $dateappoint;
      $app_output .= '</li>';
    }
    if ($app_output) {
      $committee_output .= ' committee, current appointments:';
      $committee_output .= '<ul>' . $app_output . '</ul>';
    }
    $output .= '<p>' . $committee_output . '</p>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  //orgright_debug_msg('committee',$output);
  return $output;
}

function committee_committees_add() {
  // Add a new committee
  //orgright_debug_msg('committee','Fn: committee_committees_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('committee'); 
  // Set the page title to display
  drupal_set_title(t('Create Committee'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('committee_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  // and return results
  return $output;
}

function committee_roles_list() {
  // Generate a roles list
  $output = '';
  $sql = "SELECT r.nid, r.cnid, r.role, r.multiperson, r.allowvacant FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid ORDER BY r.cnid, r.role";
  $result = db_query(db_rewrite_sql($sql));
  $cnid = -1;
  while ($role = db_fetch_object($result)) {
    if ($role->cnid <> $cnid) {
      if ($cnid > -1) { $output .= '</ul>'; }
      // Extract the committee name
      if ($role->cnid) {
        $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_result(db_query($sql, $role->cnid)) . ' committee';
      }
      else { $committee = '[no committee]'; }
      $output .= '<p class="committee-list">Roles specified for ' . (($role->cnid) ? l($committee, 'node/'.$role->cnid) : $committee) . '</p><ul>';
      $cnid = $role->cnid;
    }
    // Generate a line of listing for this role
    $role_output = '';
    $role_output .= l($role->role, 'node/'.$role->nid) . ' ';
    $role_output .= l('[edit]', 'node/'.$role->nid.'/edit');
    // Find details of any current assignments to this role
    $today = orgright_get_date_ymd();
    $count = 0;
    $assign_output = '';
    $sql = "SELECT s.nid, s.mnid, s.assigntype, s.assigndate, s.ceasedate FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.rnid = %d ORDER BY s.assigndate";
    $assignments = db_query($sql, $role->nid);
    while ($assignment = db_fetch_object($assignments)) {
      // ignored ceased assignments
      if ($assignment->ceasedate && $assignment->ceasedate < $today) { continue; }
      // Extract the member name
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_result(db_query($sql, $assignment->mnid));
      // Prepare the assignment date for display
      $assigndate = date_format_date(date_make_date($assignment->assigndate), 'custom', 'd M Y');
      $assign_output .= '<li>';
      if ($assignment->assigndate > $today) {
        $assign_output .= l('Pending assignment', 'node/'.$assignment->nid);
      }
      else {
        $count++;
        $assign_output .= l('Assignment', 'node/'.$assignment->nid);
      }
      //$assign_output .= ' ' . l('[edit]', 'node/'.$assignment->nid.'/edit');
      $assign_output .= ', ' . l($member, 'node/'.$assignment->mnid);
      $assign_output .= ' ' . strtolower($assignment->assigntype);
      $assign_output .= ' on ' . $assigndate;
      $assign_output .= '</li>';
    }
    if (!$count && !$role->allowvacant) {
      $role_output .= t(' *** role should not be vacant ***');
    }
    if ($count > 1 && !$role->multiperson) {
      $role_output .= t(' *** role should not have multiple assignments ***');
    }
    if ($assign_output) { $role_output .= '<ul>' . $assign_output . '</ul>'; }
    $output .= '<li>' . $role_output . '</li>';
  }
  if ($cnid > -1) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  //orgright_debug_msg('committee',$output);
  return $output;
}

function committee_roles_add() {
  // Add a new role
  //orgright_debug_msg('committee','Fn: committee_roles_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('role'); 
  // Set the page title to display
  drupal_set_title(t('Create Committee Role'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('role_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  // and return results
  return $output;
}

function committee_appointments_list() {
  // Generate a list of appointments
  $output = '';
  $sql = "SELECT p.nid, p.mnid, p.cnid, p.appointtype, p.appointdate, p.retiredate FROM {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid ORDER BY p.cnid, p.appointdate DESC";
  $result = db_query(db_rewrite_sql($sql));
  $today = orgright_get_date_ymd();
  $cnid = -1;
  while ($appt = db_fetch_object($result)) {
    // ignore retired appointments
    if ($appt->retiredate && $appt->retiredate < $today) { continue; }
    // Extract the member name
    $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $member = db_fetch_object(db_query($sql, $appt->mnid));
    if ($appt->cnid <> $cnid) {
      if ($cnid > -1) { $output .= '</ul>'; }
      // Extract the committee name
      $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
      $committee = db_fetch_object(db_query($sql, $appt->cnid));
      $output .= '<p class="committee-list">Appointments to ' . l($committee->committee, 'node/'.$appt->cnid) . ' committee</p><ul>';
      $cnid = $appt->cnid;
    }
    // Prepare the appoint and retire dates for display
    $dateappoint = date_format_date(date_make_date($appt->appointdate), 'custom', 'd M Y');
    // Generate a line of listing for this appointment
    $appointment_output = '';
    if ($appt->appointdate > $today) {
      $appointment_output .= l('Pending appointment', 'node/'.$appt->nid) . ' ';
    }
    else {
      $appointment_output .= l('Appointment', 'node/'.$appt->nid) . ' ';
    }
    $appointment_output .= l('[edit]', 'node/'.$appt->nid.'/edit');
    $appointment_output .= ' of ' . l($member->member, 'node/'.$appt->mnid);
    $appointment_output .= ', ' . strtolower($appt->appointtype);
    $appointment_output .= ' on ' . $dateappoint;
    if ($appt->retiredate) {
      $appointment_output .= ', to retire on ' . date_format_date(date_make_date($appt->retiredate), 'custom', 'd M Y');
    }
    $output .= '<li>' .$appointment_output . '</li>';
  }
  if ($cnid > -1) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  return $output;
}

function committee_appointments_add() {
  // Add a new appointment
  //orgright_debug_msg('committee','Fn: committee_appointments_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('appointment'); 
  // Set the page title to display
  drupal_set_title(t('Create Committee Appointment'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('appointment_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  // and return results
  return $output;
}

function committee_assignments_list() {
  // Generate a list of role assignments
  $output = '';
  $sql = "SELECT s.nid, s.mnid, s.rnid, s.assigntype, s.assigndate, s.ceasedate FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid";
  $result = db_query(db_rewrite_sql($sql));
  $today = orgright_get_date_ymd();
  $assignments = array();
  while ($assign = db_fetch_object($result)) {
    // ignore ceased assignments
    if ($assign->ceasedate && $assign->ceasedate < $today) { continue; }
    // Extract the role name and committee
    $sql = "SELECT r.role, r.cnid FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
    $role = db_fetch_object(db_query($sql, $assign->rnid));
    $assign->role = $role->role;
    $assign->cnid = $role->cnid;
    // Extract the member name
    $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $assign->member = db_result(db_query($sql, $assign->mnid));
    $key = substr('00000'.$assign->cnid,-6) . substr('00000'.$assign->rnid,-6) . substr('00000'.$assign->nid,-6);
    $assignments[$key] = $assign;
  }
  // Now sort the assignments into committee/role sequence
  ksort($assignments);
  $cnid = -1;
  foreach ($assignments as $assign) {
    // Prepare the assign date for display
    $assigndate = date_format_date(date_make_date($assign->assigndate), 'custom', 'd M Y');
    if ($assign->cnid <> $cnid) {
      if ($cnid > -1) { $output .= '</ul>'; }
      // Extract the committee name
      if ($assign->cnid) {
        $sql = "SELECT c.committee from {node} n JOIN org_committees c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_result(db_query($sql, $assign->cnid));
      }
      else { $committee = '[no commitee]'; }
      $output .= '<p class="committee-list">Assignments to roles of ' . (($assign->cnid) ? l($committee, 'node/'.$assign->cnid) : $committee) . ' committee</p>';
      $cnid = $assign->cnid;
      $output .= '<ul>';
    }
    // Generate a line of listing for this payment
    //$assignment_output = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
    $assignment_output = '';
    if ($assign->assigndate > $today) {
      $assignment_output .= l('Pending assignment', 'node/'.$assign->nid) . ' ';
    }
    else {
      $assignment_output .= l('Assignment', 'node/'.$assign->nid) . ' ';
    }
    $assignment_output .= l('[edit]', 'node/'.$assign->nid.'/edit');
    $assignment_output .= ' of ' . l($assign->member, 'node/'.$assign->mnid);
    $assignment_output .= ', ' . strtolower($assign->assigntype);
    $assignment_output .= ' to ' . l($assign->role, 'node/'.$assign->rnid);
    $assignment_output .= ' on ' . $assigndate;
    if ($assign->ceasedate) {
      $ceasedate = date_format_date(date_make_date($assign->ceasedate), 'custom', 'd M Y');
      $assignment_output .= ', to cease on ' . $ceasedate;
    }
    $output .= '<li>' . $assignment_output . '</li>';
  }
  if ($cnid > -1) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  return $output;
}

function committee_assignments_add() {
  // Add a new appointment payment
  //orgright_debug_msg('committee','Fn: committee_assignments_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('assignment'); 
  // Set the page title to display
  drupal_set_title(t('Create Role Assignment'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('assignment_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee'));
  // and return results
  return $output;
}

/**
 * Menu call back functions to generate autocomplete data sets
 */
function committee_autocomplete_committees($string = '') {
  // Generate a list of committees for the autocomplete js function
  $committees = array();
  $qargs = array('%'.strtolower($string).'%');
  $sql = "SELECT DISTINCT committee FROM {org_committees} WHERE LOWER(committee) LIKE '%s'";
  //orgright_debug_msg('committee','Autocomplete committees for: ' . $qargs[0]);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_committees}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while($obj = db_fetch_object($result)) {
    $committees[$obj->committee] = $obj->committee;
  }
  //print drupal_to_js($committees); 
  // requires php 5.2:
  drupal_json($committees);
  exit;
}

function committee_autocomplete_roles($string = '') {
  // Generate a list of roles for the autocomplete js function
  $roles = array();
  $qargs = array('%'.strtolower($string).'%');
  $sql = "SELECT DISTINCT role FROM {org_roles} WHERE LOWER(role) LIKE '%s'";
  //orgright_debug_msg('committee','Autocomplete scodes for: ' . $qargs[0]);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_appointments}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while($obj = db_fetch_object($result)) {
    $roles[$obj->role] = $obj->role;
  }
  //print drupal_to_js($roles); 
  // requires php 5.2:
  drupal_json($roles);
  exit;
}

 
