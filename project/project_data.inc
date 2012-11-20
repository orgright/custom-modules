<?php
// $Id$

/**
 * @file
 * Provides additional functions for project module.
 */

/**
 * Load functions called from hook_load() in module file
 */
function project_project_load($node) {
  $query = "SELECT p.mnid, p.project, p.projcategory, p.projstatus, p.projdatype, p.projdate, p.recalc, p.projsched, p.notes FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
  $obj = db_fetch_object(db_query($query, $node->nid));
  return $obj;
}

function project_projactivity_load($node) {
  $query = "SELECT a.pnid, a.mnid, a.activity, a.activicategory, a.activistatus, a.duration, a.duratype, a.actividatype, a.actividate, a.actualstart, a.actualfinish, a.schedstart, a.schedfinish, a.notes FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function project_projreport_load($node) {
  $query = "SELECT r.pnid, r.anid, r.mnid, r.projreport, r.projrepdate, r.notes FROM {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}


/**
 * Node manipulation functions called from hook_nodeapi() in module file
 */
function project_project_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('project','Fn: project_project_nodeapi');
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      // Load project manager reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $node->manager = db_result(db_query($sql, $node->mnid));
      // Stop here if a teaser display is required
      if ($teaser) {
        return;
      }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('mnid', 'project', 'projcategory', 'projstatus', 'projdatype', 'projdate');
      $original = array();
      foreach ($fields as $field) {
        $original[$field] = $node->$field;
      }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of activities
      $sql = "SELECT a.nid, a.mnid, a.activity, a.activicategory, a.activistatus, a.duration, a.duratype, a.schedstart, a.schedfinish FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.pnid = %d ORDER BY a.schedstart";
      $result = db_query($sql, $node->nid);
      while ($activity = db_fetch_object($result)) {
        // Extract the responsible person's name
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $activity->member = db_result(db_query($sql, $activity->mnid));
        $node->activities[] = (array)$activity;
      }
      // Load an array of project reports
      $sql = "SELECT r.nid, r.anid, r.mnid, r.projreport, r.projrepdate FROM {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.pnid = %d ORDER BY r.projrepdate DESC";
      $result = db_query($sql, $node->nid);
      while ($report = db_fetch_object($result)) {
        // Extract the activity name if required
        if ($report->anid) {
          $sql = "SELECT a.activity FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
          $report->activity = db_result(db_query($sql, $report->anid));
        }
        // Extract the responsible person's name
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $report->member = db_result(db_query($sql, $report->mnid));
        $node->reports[] = (array)$report;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_project_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) {
        return;
      }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_project_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('project_event_project_' . $field . '_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_project_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from project table
      db_query("DELETE FROM {org_projects} WHERE vid = %d", $node->vid);
      return;
  }
}

function project_projactivity_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('project','Fn: project_projactivity_nodeapi');
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      // Load project reference
      $sql = "SELECT p.project FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
      $node->project = db_result(db_query($sql, $node->pnid));
      // Load responsible person reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $node->member = db_result(db_query($sql, $node->mnid));
      // Stop here if a teaser display is required
      if ($teaser) {
        return;
      }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('pnid', 'mnid', 'activity', 'activicategory', 'activistatus', 'duration', 'duratype',
        'actividatype', 'actividate', 'actualstart', 'actualfinish');
      $original = array();
      foreach ($fields as $field) {
        $original[$field] = $node->$field;
      }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of project reports
      $sql = "SELECT r.nid, r.mnid, r.projreport, r.projrepdate FROM {node} n JOIN {org_projreports} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.anid = %d ORDER BY r.projrepdate DESC";
      $result = db_query($sql, $node->nid);
      while ($report = db_fetch_object($result)) {
        // Extract the reporting person's name
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $report->member = db_result(db_query($sql, $report->mnid));
        $node->reports[] = (array)$report;
      }
      // Load an array of linked activities
      $sql = "SELECT a.activity, a.activicategory, a.activistatus, a.duration, a.duratype, a.schedstart, a.schedfinish, l.link FROM ({node} n JOIN {org_projactivities} a ON a.nid = n.nid) JOIN {org_projactivitylinks} l ON l.pnid = a.pnid WHERE a.vid = n.vid AND (l.anid1 = %d OR l.anid2 = %d) ORDER BY a.schedstart";
      $result = db_query($sql, $node->nid, $node->nid);
      while ($activity = db_fetch_object($result)) {
        $node->linked_activities[] = (array)$activity;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_projactivity_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) {
        return;
      }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_projactivity_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('project_event_projactivity_' . $field . '_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_projactivity_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from project table
      db_query("DELETE FROM {org_projactivities} WHERE vid = %d", $node->vid);
      return;
  }
}

function project_projreport_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('project','Fn: project_projreport_nodeapi');
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      // Load project reference
      $sql = "SELECT p.project FROM {node} n JOIN {org_projects} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
      $node->project = db_result(db_query($sql, $node->pnid));
      // Load projactivity reference if necessary
      if ($node->anid) {
        $sql = "SELECT a.activity FROM {node} n JOIN {org_projactivities} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
        $node->activity = db_result(db_query($sql, $node->anid));
      }
      // Extract the responsible person's name
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $node->member = db_result(db_query($sql, $node->mnid));
      // Specify the fields that are used for rules event triggers on change
      $fields = array('pnid', 'anid', 'mnid', 'projreport', 'projrepdate');
      $original = array();
      foreach ($fields as $field) {
        $original[$field] = $node->$field;
      }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_projreport_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) {
        return;
      }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_projreport_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('project_event_projreport_' . $field . '_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('project_event_projreport_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from project table
      db_query("DELETE FROM {org_projreports} WHERE vid = %d", $node->vid);
      return;
  }
}


/**
 * Insert functions alled from hook_insert() in module file
 */
function project_project_insert($node) {
  db_query("INSERT INTO {org_projects} (nid, vid, mnid, project, projcategory, projstatus, projdatype, projdate, recalc, projsched, notes, user) VALUES (%d, %d, %d, '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->mnid,
    $node->project,
    $node->projcategory,
    $node->projstatus,
    $node->projdatype,
    $node->projdate,
    $node->recalc,
    $node->projsched,
    $node->notes,
    $node->uid);
}

function project_projactivity_insert($node) {
  db_query("INSERT INTO {org_projactivities} (nid, vid, pnid, mnid, activity, activicategory, activistatus, duration, duratype, actividatype, actividate, actualstart, actualfinish, schedstart, schedfinish, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->pnid,
    $node->mnid,
    $node->activity,
    $node->activicategory,
    $node->activistatus,
    $node->duration,
    $node->duratype,
    $node->actividatype,
    $node->actividate,
    $node->actualstart,
    $node->actualfinish,
    $node->schedstart,
    $node->schedfinish,
    $node->notes,
    $node->uid);
}

function project_projreport_insert($node) {
  db_query("INSERT INTO {org_projreports} (nid, vid, pnid, anid, mnid, projreport, projrepdate, notes, user) VALUES (%d, %d, %d, %d, %d, '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->pnid,
    $node->anid,
    $node->mnid,
    $node->projreport,
    $node->projrepdate,
    $node->notes,
    $node->uid);
}

/**
 * Update functions called from hook_update() in module file
 */
function project_project_update($node) {
  db_query("UPDATE {org_projects} SET mnid = %d, project = '%s', projcategory = '%s', projstatus = '%s', projdatype = '%s', projdate = '%s', recalc = %d, projsched = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->mnid,
    $node->project,
    $node->projcategory,
    $node->projstatus,
    $node->projdatype,
    $node->projdate,
    $node->recalc,
    $node->projsched,
    $node->notes,
    $node->uid,
    $node->vid);
}

function project_projactivity_update($node) {
  db_query("UPDATE {org_projactivities} SET pnid = %d, mnid = %d, activity = '%s', activicategory = '%s', activistatus = '%s', duration = %d, duratype = '%s', actividatype = '%s', actividate = '%s', actualstart = '%s', actualfinish = '%s', schedstart = '%s', schedfinish = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->pnid,
    $node->mnid,
    $node->activity,
    $node->activicategory,
    $node->activistatus,
    $node->duration,
    $node->duratype,
    $node->actividatype,
    $node->actividate,
    $node->actualstart,
    $node->actualfinish,
    $node->schedstart,
    $node->schedfinish,
    $node->notes,
    $node->uid,
    $node->vid);
}

function project_projreport_update($node) {
  db_query("UPDATE {org_projreports} SET pnid = %d, anid = %d, mnid = %d, projreport = '%s', projrepdate = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->pnid,
    $node->anid,
    $node->mnid,
    $node->projreport,
    $node->projrepdate,
    $node->notes,
    $node->uid,
    $node->vid);
}


/**
 * Delete functions called from hook_delete() in module file
 */
function project_project_delete($node) {
  db_query("DELETE FROM {org_projects} WHERE nid = %d", $node->nid);
}

function project_projactivity_delete($node) {
  db_query("DELETE FROM {org_projactivities} WHERE nid = %d", $node->nid);
}

function project_projreport_delete($node) {
  db_query("DELETE FROM {org_projreports} WHERE nid = %d", $node->nid);
}

/**
 * Functions dealing with project linkages
 */
function project_get_activity_links($anid) {
  // return an array of activities linked to the specified activity
  $links_list = array();
  $sql = "SELECT anid1, anid2, link FROM {org_projactivitylinks} WHERE anid1 = %d OR anid2 = %d";
  $result = db_query($sql, $anid, $anid);
  while ($link = db_fetch_object($result)) {
    if ($link->anid1 == $anid) {
      $links_list[$link->anid2] = $link->link;
    }
    else {
      $this_link = project_invert_link($link->anid1, $link->anid2, $link->link);
      $links_list[$this_link['2nd']] = $this_link['link'];
    }
  }
  return $links_list;
}

