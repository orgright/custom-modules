<?php
// $Id$

/**
 * @file
 * Provides additional functions for project module.
 */

/**
 * Menu call back functions to display pages to users
 */
function project_management() {
  // Home page of the project management function - display links and description
  $output = '<p>The following project management functions are available:</p>';
  $output .= '<h2>' . l('Manage projects', 'project/projects') . '</h2>';
  $output .= '<p>List, add, update or delete projects from the orgainsation\'s database.</p>';
  $output .= '<h2>' . l('Manage project activities', 'project/projactivities') . '</h2>';
  $output .= '<p>List, add, update or delete project activities including attaching them to individual projects and specifying linkages between them.</p>';
  $output .= '<h2>' . l('Manage project reports', 'project/projreports') . '</h2>';
  $output .= '<p>List, add, update or delete project reports including attaching them to individual projects or project activities.</p>';
  // set the breadcrumb trail
  orgright_breadcrumb(array());
  return $output;
}

function project_projects() {
  // entry page for managing projects
  return project_projects_list();
}

function project_projactivities() {
  // entry page for managing projactivities
  return project_projactivities_list();
}

function project_projreports() {
  // entry page for managing projactivity payments
  return project_projreports_list();
}

function project_projects_list() {
  // Generate a projects list
  $output = '';
  $sql = "SELECT p.nid, p.mnid, p.project, p.projcategory, p.projstatus, p.projdatype, p.projdate FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid ORDER BY p.mnid, p.project";
  $result = db_query(db_rewrite_sql($sql));
  $mind = 0;
  while ($project = db_fetch_object($result)) {
    if ($project->mnid <> $mnid) {
      if ($mnid) { $output .= '</ul>'; }
      // Extract the project manager's name
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_result(db_query($sql, $project->mnid));
      $output .= '<p>Projects managed by: ' . l($member, 'node/'.$project->mnid) . '<p><ul>';
      $mnid = $project->mnid;
    }
    // Prepare the project date constraint for display
    $projdate = date_format_date(date_make_date($project->projdate), 'custom', 'd M Y');
    // Generate a line of listing for this project
    $project_output = '';
    $project_output .= l($project->project, 'node/'.$project->nid);
    $project_output .= ' ' . l('[edit]', 'node/'.$project->nid.'/edit');
    $project_output .= ', category: ' . strtolower($project->projcategory);
    $project_output .= ', status: ' . strtolower($project->projstatus);
    $project_output .= ', ' . strtolower($project->projdatype) . ' on ' . $projdate;
    $output .= '<li>' . $project_output . '</li>';
  }
  if ($mnid) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}

function project_projects_add() {
  // Add a new project
  //orgright_debug_msg('project','Fn: project_projects_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('project'); 
  // Set the page title to display
  drupal_set_title(t('Create Project'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('project_node_form', $node, $args);
  // and return results
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}

function project_projects_schedule() {
  // Re-calculate the project schedule
  // Initialise the form array
  $args = func_get_args();
  $output = drupal_get_form('project_calculate_schedule_form', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}


function project_projactivities_list() {
  // Generate a list of project activities
  $output = '';
  $sql = "SELECT a.nid, a.pnid, a.mnid, a.activity, a.activicategory, a.activistatus, a.duration, a.duratype, a.actividatype, a.actividate FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid ORDER BY a.pnid, a.activity";
  $result = db_query(db_rewrite_sql($sql));
  $pnid = 0;
  while ($activity = db_fetch_object($result)) {
    if ($activity->pnid <> $pnid) {
      if ($pnid) { $output .= '</ul>'; }
      // Extract the project name
      $sql = "SELECT p.project FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
      $project = db_result(db_query($sql, $activity->pnid));
      $output .= '<p>Activities of project: ' . l($project, 'node/'.$activity->pnid) . '<p><ul>';
      $pnid = $activity->pnid;
    }
    // Extract the responsible person's name
    $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $member = db_result(db_query($sql, $activity->mnid));
    // Prepare the activity date constraint for display
    $actividate = date_format_date(date_make_date($activity->actividate), 'custom', 'd M Y');
    // Generate a line of listing for this activity
    $activity_output = '';
    $activity_output .= l($activity->activity, 'node/'.$activity->nid);
    $activity_output .= ' ' . l('[edit]', 'node/'.$activity->nid.'/edit');
    $activity_output .= ', allocated to ' . l($member, 'node/'.$activity->mnid);
    $activity_output .= ', ' . strtolower($activity->activicategory);
    $activity_output .= ', ' . strtolower($activity->activistatus);
    $activity_output .= ', ' . $activity->duration . ' ' . strtolower($activity->duratype);
    $activity_output .= ', ' . strtolower($activity->actividatype) . ' ' . $actividate;
    $output .= '<li>' . $activity_output . '</li>';
  }
  if ($pnid) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}

function project_projactivities_add() {
  // Add a new project activity
  //orgright_debug_msg('project','Fn: project_projactivities_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('projactivity'); 
  // Set the page title to display
  drupal_set_title(t('Create Project Activity'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('projactivity_node_form', $node, $args);
  // and return results
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}

function project_projactivities_link() {
  // Link project activities to create a schedule
  // Initialise the form array
  $args = func_get_args();
  $output = drupal_get_form('project_activity_links_form', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}

function project_projreports_list() {
  // Generate a list of project reports
  $output = '';
  $sql = "SELECT r.nid, r.pnid, r.anid, r.mnid, r.projreport, r.projrepdate FROM {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid ORDER BY r.pnid, r.projrepdate";
  $result = db_query(db_rewrite_sql($sql));
  $pnid = 0;
  while ($projreport = db_fetch_object($result)) {
    if ($projreport->pnid <> $pnid) {
      if ($pnid) { $output .= '</ul>'; }
      // Extract the project name
      $sql = "SELECT p.project FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
      $project = db_result(db_query($sql, $projreport->pnid));
      $output .= '<p>Reports for project: ' . l($project, 'node/'.$projreport->pnid) . '<p><ul>';
      $pnid = $projreport->pnid;
    }
    // Extract the project activity name if required
    if ($projreport->anid) {
      $sql = "SELECT a.activity FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
      $projactivity = db_result(db_query($sql, $projreport->anid));
    }
    // Extract the responsible person's name
    $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $member = db_result(db_query($sql, $projreport->mnid));
    // Prepare the project report date for display
    $reportdate = date_format_date(date_make_date($projreport->projrepdate), 'custom', 'd M Y');
    // Generate a line of listing for this project report
    $projreport_output = '';
    $projreport_output .= l($projreport->projreport, 'node/'.$projreport->nid);
    $projreport_output .= ' ' . l('[edit]', 'node/'.$projreport->nid.'/edit');
    if ($projreport->anid) {
      $projreport_output .= ', activity ' . l($activity, 'node/'.$projreport->anid);
    }
    $projreport_output .= ' by ' . l($member, 'node/'.$projreport->mnid);
    $projreport_output .= ', ' . $reportdate;
    $output .= '<li>' . $projreport_output . '</li>';
  }
  if ($pnid) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  return $output;
}

function project_projreports_add() {
  // Add a new projactivity payment
  //orgright_debug_msg('project','Fn: project_projreports_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('projreport'); 
  // Set the page title to display
  drupal_set_title(t('Create Project Report'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('projreport_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Project')=>'project'));
  // and return results
  return $output;
}

/**
 * Menu call back functions to generate autocomplete data sets
 */
function project_autocomplete_projects($string = '') {
  // Generate a list of projects for the autocomplete js function
  $projects = array();
  $qargs = array('%'.strtolower($string).'%');
  $sql = "SELECT DISTINCT project FROM {org_projects} WHERE LOWER(project) LIKE '%s'";
  //orgright_debug_msg('project','Autocomplete projects for: ' . $qargs[0]);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_projects}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while($obj = db_fetch_object($result)) {
    $projects[$obj->project] = $obj->project;
  }
  //print drupal_to_js($projects); 
  // requires php 5.2:
  drupal_json($projects);
  exit;
}

function project_autocomplete_activities($string = '') {
  // Generate a list of project activities for the autocomplete js function
  $activities = array();
  $qargs = array('%'.strtolower($string).'%');
  $sql = "SELECT DISTINCT activity FROM {org_projactivities} WHERE LOWER(activity) LIKE '%s'";
  //orgright_debug_msg('project','Autocomplete activities for: ' . $qargs[0]);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_projactivities}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while($obj = db_fetch_object($result)) {
    $activities[$obj->activity] = $obj->activity;
  }
  //print drupal_to_js($activities); 
  // requires php 5.2:
  drupal_json($activities);
  exit;
}

