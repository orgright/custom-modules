<?php
// $Id$

/**
 * @file
 * Provides additional functions for meeting module.
 */

/**
 * Menu call back functions to display pages to users
 */
function meeting_management() {
  // Home page of the meeting management function - display links and description
  $output = '<p>The following meeting management functions are available:</p>';
  $output .= '<h2>' . l('Manage Meetings', 'meeting/meetings') . '</h2>';
  $output .= '<p>List, add, update or delete meetings from the organisation\'s database.</p>';
  $output .= '<h2>' . l('Manage agenda items', 'meeting/agenda_items') . '</h2>';
  $output .= '<p>List, add, update or delete agenda items for specific meetings.</p>';
  $output .= '<h2>' . l('Manage meeting items', 'meeting/mtg_items') . '</h2>';
  $output .= '<p>List, add, update or delete meeting items including actions arising from meetings.</p>';
  // set the breadcrumb trail
  orgright_breadcrumb(array());
  return $output;
}

function meeting_meetings() {
  // entry page for managing meetings
  return meeting_meetings_list();
}

function meeting_agenda_items() {
  // entry page for managing agenda items
  return meeting_agenda_items_list();
}

function meeting_mtg_items() {
  // entry page for managing meeting items
  return meeting_mtg_items_list();
}

function meeting_meetings_list($template = FALSE) {
  // Generate a meetings list
  $output = '';
  if ($template) {
    $sql = "SELECT g.nid, g.cnid, g.meeting, g.mtgtype, g.mtgstatus, g.mtgdate, g.start FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.mtgstatus = 'template' ORDER BY g.cnid, g.mtgdate";
  }
  else {
    // get a list of meeting status values to select
    $statuses = meeting_get_meeting_list_statuses();
    $sql = "SELECT g.nid, g.cnid, g.meeting, g.mtgtype, g.mtgstatus, g.mtgdate, g.start FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND (g.mtgstatus = ''";
    foreach ($statuses as $status) {
      $sql .= " OR g.mtgstatus = '" . $status . "'";
    }
    $sql .= ") ORDER BY g.cnid, g.mtgdate";
  }
  $cnid = -1;
  $result = db_query(db_rewrite_sql($sql));
  while ($meeting = db_fetch_object($result)) {
    if ($meeting->cnid <> $cnid) {
      if ($cnid > -1) {
        $output .= '</ul>';
      }
      if ($meeting->cnid) {
        // Extract the committee name
        $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_result(db_query($sql, $meeting->cnid));
      }
      $output .= '<p>' . (($meeting->cnid) ? l($committee, 'node/' . $meeting->cnid) . ' committee' : '[no committee]') . ', meetings:<p><ul>';
      $cnid = $meeting->cnid;
    }
    // Prepare the date and time for display
    $mtgdate = date_format_date(date_make_date($meeting->mtgdate), 'custom', 'd M Y');
    $start = date_format_date(date_make_date($meeting->mtgdate . ' ' . $meeting->start), 'custom', 'h:i a');
    // Generate a line of listing for this meeting
    $meeting_output = '';
    $meeting_output .= l($meeting->meeting, 'node/' . $meeting->nid) . ' ';
    $meeting_output .= l('[edit]', 'node/' . $meeting->nid . '/edit') . ', ';
    $meeting_output .= 'type: ' . $meeting->mtgtype;
    $meeting_output .= ', date: ' . $mtgdate;
    $meeting_output .= ', ' . $start;
    if ($meeting->mtgstatus) {
      $meeting_output .= ', status: ' . $meeting->mtgstatus;
    }
    $output .= '<li>' . $meeting_output . '</li>';
  }
  if ($cnid > -1) {
    $output .= '</ul>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting'));
  return $output;
}

function meeting_meetings_add() {
  // Add a new meeting
  //orgright_debug_msg('meeting','Fn: meeting_meetings_add',6);
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('meeting');
  // Set the page title to display
  drupal_set_title(t('Create Meeting'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('meeting_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting'));
  // and return results
  return $output;
}

function meeting_meetings_template_list() {
  return meeting_meetings_list(TRUE);
}

function meeting_meetings_template_add() {
  // Add a new meeting template
  // Use standard meeting form, which will be altered later
  return meeting_meetings_add('template~0');
}

function meeting_meetings_template_use() {
  // prepare a two part form to select the meeting template and then to
  // collect the information required to generate a meeting from the template
  return drupal_get_form('meeting_template_use_form');
}

function meeting_meetings_agenda($meeting = '') {
  // Display the agenda for a meeting
  // If there is no meeting specified, then select a meeting
  if (!$meeting) {
    return drupal_get_form('meeting_selection_form', '', 'meeting/meetings/agenda');
  }
  // Check that the passed value is the nid or name of a meeting node
  module_load_include('inc', 'meeting', 'meeting_form');
  if (!$gnid = meeting_validate_meeting($meeting)) {
    $msg = t('The passed parameter (@mtg) is not a meeting', array('@mtg' => $meeting));
    return drupal_get_form('meeting_selection_form', $msg, 'meeting/meetings/agenda');
  }
  // OK, have a meeting, so build and display the agenda
  //return t('Agenda for meeting @mtg', array('@mtg'=>$meeting));
  $mtgnode = node_load($gnid);
  // set the page title
  drupal_set_title(str_replace('Meeting:', 'Meeting Agenda:', $mtgnode->title));
  $mtgdate = date_format_date(date_make_date($mtgnode->mtgdate), 'custom', 'd M Y');
  $mtgtime = date_format_date(date_make_date($mtgnode->mtgdate . ' ' . $mtgnode->start), 'custom', 'h:i a');
  $mtgfini = ($mtgnode->finish) ? date_format_date(date_make_date($mtgnode->mtgdate . ' ' . $mtgnode->finish), 'custom', 'h:i a') : 'unspecified';
  // start generating the output text
  $output = '<div class="agenda-display"><div class="agenda-header">';
  $output .= '<div class="agenda-h1">' . t('Agenda for meeting @mtg', array('@mtg' => $meeting)) . '</div>';
  $output .= '<div class="agenda-h2">' . t('of @cmte on @date', array('@cmte' => $mtgnode->committee, '@date' => $mtgdate)) . '</div>';
  $output .= '<div class="agenda-h2">' . t('Start time: @tim, target finish time: @fin', array('@tim' => $mtgtime, '@fin' => $mtgfini)) . '</div><br />';
  // Now add details of any documents linked to this meeting
  $docnum = 0;
  $docnids = array();
  if (isset($mtgnode->doclinks)) {
    $output .= '<div class="agenda-doclinks"><div class="agenda-h3">' . t('Documents linked to meeting:') . '</div><ul>';
    foreach ($mtgnode->doclinks as $doclink) {
      $docnum++;
      $docdate = date_format_date(date_make_date($doclink['docdate']), 'custom', 'd M Y');
      $output .= '<li>' . $doclink['docname'] . ', date: ' . $docdate;
      if ($doclink['version']) {
        $output .= ', version: ' . $doclink['version'];
      }
      if ($doclink['docstatus']) {
        $output .= ', status: ' . $doclink['docstatus'];
      }
      $output .= '<br />Refer to Annex ' . $docnum . ' for complete details.</li>';
      // save document nid for later use
      $docnids[$docnum] = $doclink['nid'];
    }
    $output .= '</ul></div><!-- /.agenda-doclinks -->';
  }
  $output .= '</div><!-- /.agenda-header -->';
  if (!isset($mtgnode->agenda_items)) {
    $output .= '<div class="agenda-content">' . t('*** No agenda items defined ***') . '</div>';
    return $output;
  }
  $output .= '<div class="agenda-content"><div class="agenda-h2">' . t('Agenda items:') . '</div><ol>';
  foreach ($mtgnode->agenda_items as $agitem) {
    // load the agenda item node
    $aginode = node_load($agitem['nid']);
    $output .= '<li><div class="agenda-item">' . $aginode->agendaitem . '</div>';
    $output .= '<div class="agenda-body">' . $aginode->body_content . '</div>';
    // Now add details of any documents linked to this agenda item
    if (isset($aginode->doclinks)) {
      $output .= '<div class="agenda-doclinks"><div class="agenda-h3">' . t('Documents linked to agenda item:') . '</div><ul>';
      foreach ($aginode->doclinks as $doclink) {
        $docnum++;
        $docdate = date_format_date(date_make_date($doclink['docdate']), 'custom', 'd M Y');
        $output .= '<li>' . $doclink['docname'] . ', date: ' . $docdate;
        if ($doclink['version']) {
          $output .= ', version: ' . $doclink['version'];
        }
        if ($doclink['docstatus']) {
          $output .= ', status: ' . $doclink['docstatus'];
        }
        $output .= '<br />Refer to Annex ' . $docnum . ' for complete details.</li>';
        // save document nid for later use
        $docnids[$docnum] = $doclink['nid'];
      }
      $output .= '</ul></div><!-- /.agenda-doclinks -->';
    }
    $output .= '</li>';
  }
  $output .= '</ol></div><!-- /.agenda-content -->';
  // done the main agenda, check for linked documents
  if ($docnum) {
    // prepare annexes for linked documents
    $output .= '<div class="agenda-annexes"><div class="agenda-h2">' . t('Agenda Annexes:') . '</div><ol>';
    foreach ($docnids as $docnid) {
      // load the referenced document node
      $docnode = node_load($docnid);
      $docdate = date_format_date(date_make_date($docnode->docdate), 'custom', 'd M Y');
      $output .= '<li><span class="agenda-h3">' . $docnode->docname . '</span><br />Date: ' . $docdate;
      if ($docnode->version) {
        $output .= ', version: ' . $docnode->version;
      }
      if ($docnode->doctype) {
        $output .= ', type: ' . $docnode->doctype;
      }
      if ($docnode->docstatus) {
        $output .= ', status: ' . $docnode->docstatus;
      }
      if ($docnode->docdesc) {
        $output .= '<div class="agenda-docdesc">' . $docnode->docdesc . '</div>';
      }
      if ($docnode->afid) {
        // there is an associated file, so provide details
        $output .= '<div>Associated file: <span class="agenda-h3">' . l($docnode->filename, $docnode->filepath) . '</span> (' . $docnode->filemime . ', size = ' . $docnode->filesize . ')</div>';
        $output .= '<div class="agenda-docbody">' . $docnode->body . '</div>';
      }
      $output .= '</li>';
    }
    $output .= '</div><!-- /.agenda-annexes -->';
  }
  $output .= '</div><!-- /.agenda-display -->';
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting', t('Meetings') => 'meeting/meetings'));
  return $output;
}

function meeting_agenda_items_list($template = FALSE) {
  // Generate a list of agenda items
  $output = '';
  if ($template) {
    $sql = "SELECT a.nid, a.gnid, a.agendaitem, a.sequence FROM {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.agitstatus = 'template' ORDER BY a.gnid, a.sequence";
  }
  else {
    // get a list of meeting status values to select
    $statuses = meeting_get_agenda_item_list_statuses();
    $sql = "SELECT a.nid, a.gnid, a.agendaitem, a.sequence FROM {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND (a.agitstatus = ''";
    foreach ($statuses as $status) {
      $sql .= " OR a.agitstatus = '" . $status . "'";
    }
    $sql .= ") ORDER BY a.gnid, a.sequence";
  }
  $result = db_query(db_rewrite_sql($sql));
  $gnid = -1;
  while ($agenda_item = db_fetch_object($result)) {
    if ($agenda_item->gnid <> $gnid) {
      if ($gnid > -1) {
        $output .= '</ul>';
      }
      // Extract the meeting name
      $sql = "SELECT g.meeting, g.mtgdate FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
      $mtg = db_fetch_object(db_query($sql, $agenda_item->gnid));
      $mtgdate = date_format_date(date_make_date($mtg->mtgdate), 'custom', 'd M Y');
      $output .= '<p class="meeting-list">Meeting: ' . l($mtg->meeting, 'node/' . $agenda_item->gnid) . ' on ' . $mtgdate . '</p><ul>';
      $gnid = $agenda_item->gnid;
    }
    // Generate a line of listing for this agenda_item
    $agenda_item_output = '';
    $agenda_item_output .= l($agenda_item->agendaitem, 'node/' . $agenda_item->nid) . ' ';
    $agenda_item_output .= l('[edit]', 'node/' . $agenda_item->nid . '/edit') . ': ';
    $agenda_item_output .= 'sequence: ' . $agenda_item->sequence;
    $output .= '<li>' . $agenda_item_output . '</li>';
  }
  if ($gnid > -1) {
    $output .= '</ul>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting'));
  return $output;
}

function meeting_agenda_items_add() {
  // Add a new agenda_item
  //orgright_debug_msg('meeting','Fn: meeting_agenda_items_add',6);
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('agenda_item');
  // Set the page title to display
  drupal_set_title(t('Create Agenda Item'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('agenda_item_node_form', $node, $args);
  // and return results
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting'));
  return $output;
}

function meeting_agenda_items_template_list() {
  return meeting_agenda_items_list(TRUE);
}

function meeting_agenda_items_template_add() {
  // Add a new agenda item template
  // Use standard meeting form, which will be altered later
  return meeting_agenda_items_add('template~0');
}

function meeting_mtg_items_list() {
  // Generate a list of meeting items
  $output = '';
  // get a list of meeting status values to select
  $statuses = meeting_get_mtg_item_list_statuses();
  $sql = "SELECT i.nid, i.gnid, i.mnid, i.itemname, i.itemtype, i.itemdate FROM {node} n JOIN {org_mtg_items} i ON i.nid = n.nid WHERE i.vid = n.vid AND (i.itemstatus = ''";
  foreach ($statuses as $status) {
    $sql .= " OR i.itemstatus = '" . $status . "'";
  }
  $sql .= ") ORDER BY i.gnid, i.itemdate";
  $result = db_query(db_rewrite_sql($sql));
  $gnid = -1;
  while ($mtg_item = db_fetch_object($result)) {
    if ($mtg_item->gnid <> $gnid) {
      if ($gnid > -1) {
        $output .= '</ul>';
      }
      if ($mtg_item->gnid) {
        // Extract the meeting name
        $sql = "SELECT g.meeting, g.mtgdate FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
        $mtg = db_fetch_object(db_query($sql, $mtg_item->gnid));
      }
      $mtgdate = date_format_date(date_make_date($mtg->mtgdate), 'custom', 'd M Y');
      $output .= '<p class="meeting-list">Meeting: ' . l($mtg->meeting, 'node/' . $mtg_item->gnid) . ' on ' . $mtgdate . '</p><ul>';
      $gnid = $mtg_item->gnid;
    }
    // Extract the member name
    $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $member = db_result(db_query($sql, $mtg_item->mnid));
    // Prepare the date for display
    $item_date = date_format_date(date_make_date($mtg_item->itemdate), 'custom', 'd M Y');
    // Generate a line of listing for this payment
    $mtg_item_output = '';
    $mtg_item_output .= l($mtg_item->itemname, 'node/' . $mtg_item->nid) . ' ';
    $mtg_item_output .= l('[edit]', 'node/' . $mtg_item->nid . '/edit') . ' ';
    $mtg_item_output .= $mtg_item->itemtype . ' ';
    $mtg_item_output .= 'on ' . $item_date . ' ';
    $mtg_item_output .= 'by ' . l($member, 'node/' . $mtg_item->mnid) . ' ';
    $output .= '<li>' . $mtg_item_output . '</li>';
  }
  if ($gnid > -1) {
    $output .= '</ul>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting'));
  return $output;
}

function meeting_mtg_items_add() {
  // Add a new subscription payment
  //orgright_debug_msg('meeting','Fn: meeting_mtg_items_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('mtg_item');
  // Set the page title to display
  drupal_set_title(t('Create Meeting Item'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('mtg_item_node_form', $node, $args);
  // and return results
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting') => 'meeting'));
  return $output;
}


/**
 * Menu call back functions to generate autocomplete data sets
 */
function meeting_autocomplete_meetings($string = '') {
  // Generate a list of meetings for the autocomplete js function
  $meetings = array();
  $qargs = array('%' . strtolower($string) . '%');
  $sql = "SELECT DISTINCT meeting FROM {org_meetings} WHERE LOWER(meeting) LIKE '%s'";
  $msg = 'Autocomplete meetings for: ' . $qargs[0];
  //orgright_debug_msg('meeting',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_members}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $meetings[$obj->meeting] = $obj->meeting;
  }
  //print drupal_to_js($meetings);
  // requires php 5.2:
  drupal_json($meetings);
  exit;
}

function meeting_autocomplete_locations($string = '') {
  // Generate a list of locations for the autocomplete js function
  $locations = array();
  $qargs = array('%' . strtolower($string) . '%');
  $sql = "SELECT DISTINCT location FROM {org_meetings} WHERE LOWER(location) LIKE '%s'";
  $msg = 'Autocomplete locations for: ' . $qargs[0];
  //orgright_debug_msg('meeting',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_members}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $locations[$obj->location] = $obj->location;
  }
  //print drupal_to_js($locations);
  // requires php 5.2:
  drupal_json($locations);
  exit;
}
