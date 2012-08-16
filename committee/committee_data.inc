<?php
// $Id$

/**
 * @file
 * Provides additional functions for committee module.
 */

/**
 * Load functions called from hook_load() in module file
 */
function committee_committee_load($node) {
  $query = "SELECT c.pnid, c.committee, c.notes FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function committee_role_load($node) {
  $query = "SELECT r.cnid, r.role, r.multiperson, r.allowvacant, r.notes FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function committee_appointment_load($node) {
  $query = "SELECT p.mnid, p.cnid, p.appointtype, p.appointdate, p.retiredate, p.notes FROM {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function committee_assignment_load($node) {
  $query = "SELECT s.mnid, s.rnid, s.assigntype, s.assigndate, s.ceasedate, s.notes FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}


/**
 * Node manipulation functions called from hook_nodeapi() in module file
 */
function committee_committee_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_committee_nodeapi, op='.$op);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      //orgright_debug_msg('committee',t('look for detail records for vid=@vid', array('@vid'=>$node->vid)));
      // Load parent reference if any
      if ($node->pnid) {
        $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_fetch_object(db_query($sql, $node->pnid));
        $node->parent = $committee->committee;
      }
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('pnid');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // information that might be required below
      $today = orgright_get_date_ymd();
      // Load an array of sub-committees
      $sql = "SELECT c.nid, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.pnid = %d ORDER BY c.committee";
      $result = db_query($sql, $node->nid);
      while ($subcommittee = db_fetch_object($result)) {
        $node->subcommittees[] = (array)$subcommittee;
      }
      // Load an array of roles
      $sql = "SELECT r.nid, r.role FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.cnid = %d ORDER BY r.role";
      $result = db_query($sql, $node->nid);
      while ($role = db_fetch_object($result)) {
        // Load assignment details
        $sql = "SELECT s.nid, s.mnid, s.assigntype, s.assigndate, s.ceasedate FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.rnid = %d AND (s.ceasedate = '' OR s.ceasedate > '%s') ORDER BY s.assigndate";
        $assignments = db_query($sql, $role->nid, $today);
        while ($assignment = db_fetch_object($assignments)) {
          // Extract the member name
          $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
          $assignment->member = db_result(db_query($sql, $assignment->mnid));
          // Prepare the assignment date for display
          $assignment->assigndate = date_format_date(date_make_date($assignment->assigndate), 'custom', 'd M Y');
          $role->assignment[] = (array)$assignment;
        }
        $node->roles[] = (array)$role;
      }
      // Load an array of current appointments
      $sql = "SELECT p.nid, p.mnid, p.appointtype, p.appointdate, p.retiredate FROM {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.cnid = %d AND (p.retiredate = '' OR p.retiredate > '%s') ORDER BY p.appointdate";
      $result = db_query($sql, $node->nid, $today);
      while ($appointment = db_fetch_object($result)) {
        // Load member reference
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $member = db_fetch_object(db_query($sql, $appointment->mnid));
        $appointment->member = $member->member;
        // Store current appointment details
        $node->appointments[] = (array)$appointment;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_commitee_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_commitee_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('committee_event_commitee_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_commitee_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from committee table
      db_query("DELETE FROM {org_committees} WHERE vid = %d", $node->vid);
      return;
  }
}

function committee_role_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_role_nodeapi, op='.$op);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      //orgright_debug_msg('committee',t('look for detail records for vid=@vid', array('@vid'=>$node->vid)));
      if ($node->cnid) {
        // Load committee reference
        $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_fetch_object(db_query($sql, $node->cnid));
        $node->committee = $committee->committee;
      }
      else { $node->committee = '[none]'; }
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('cnid','multiperson','allowvacant');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of assignments
      $sql = "SELECT s.nid, s.mnid, s.assigntype, s.assigndate, s.ceasedate FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.rnid = %d ORDER BY s.assigndate DESC";
      $result = db_query($sql, $node->nid);
      while ($assignment = db_fetch_object($result)) {
        // Load member reference
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $member = db_fetch_object(db_query($sql, $assignment->mnid));
        $assignment->member = $member->member;
        // Store current assignment details
        $node->assignments[] = (array)$assignment;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_role_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_role_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('committee_event_role_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_role_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from committee table
      db_query("DELETE FROM {org_roles} WHERE vid = %d", $node->vid);
      return;
  }
}

function committee_appointment_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_appointment_nodeapi, op='.$op);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      //orgright_debug_msg('committee',t('look for detail records for vid=@vid', array('@vid'=>$node->vid)));
      // Load member reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_fetch_object(db_query($sql, $node->mnid));
      $node->member = $member->member;
      // Load committee reference
      $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
      $committee = db_fetch_object(db_query($sql, $node->cnid));
      $node->committee = $committee->committee;
      // Specify the fields that are used for rules event triggers on change
      $fields = array('mnid','cnid','appointtype','appointdate','retiredate');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_appointment_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_appointment_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('committee_event_appointment_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_appointment_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from committee table
      db_query("DELETE FROM {org_appointments} WHERE vid = %d", $node->vid);
      return;
  }
}

function committee_assignment_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('committee','Fn: committee_assignment_nodeapi, op='.$op);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      //orgright_debug_msg('committee',t('look for detail records for vid=@vid', array('@vid'=>$node->vid)));
      // Load member reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_fetch_object(db_query($sql, $node->mnid));
      $node->member = $member->member;
      // Load role reference
      $sql = "SELECT r.cnid, r.role FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
      $role = db_fetch_object(db_query($sql, $node->rnid));
      $node->role = $role->role;
      $node->cnid = $role->cnid;
      if ($role->cnid) {
        // Load committee reference
        $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_fetch_object(db_query($sql, $role->cnid));
        $node->committee = $committee->committee;
      }
      else { $node->committee = '[none]'; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('mnid','rnid','assigntype','assigndate','ceasedate');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_assignment_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_assignment_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('committee_event_assignment_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('committee_event_assignment_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from committee table
      db_query("DELETE FROM {org_assignments} WHERE vid = %d", $node->vid);
      return;
  }
}


/**
 * Insert functions called from hook_insert() in module file
 */
function committee_committee_insert($node) {
  db_query("INSERT INTO {org_committees} (nid, vid, pnid, committee, notes, user) VALUES (%d, %d, %d, '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->pnid,
    $node->committee,
    $node->notes,
    $node->uid);
}

function committee_role_insert($node) {
  db_query("INSERT INTO {org_roles} (nid, vid, cnid, role, multiperson, allowvacant, notes, user) VALUES (%d, %d, %d, '%s', %d, %d, '%s', %d)",
    $node->nid,
    $node->vid,
    $node->cnid,
    $node->role,
    $node->multiperson,
    $node->allowvacant,
    $node->notes,
    $node->uid);
}

function committee_appointment_insert($node) {
  db_query("INSERT INTO {org_appointments} (nid, vid, mnid, cnid, appointtype, appointdate, retiredate, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->mnid,
    $node->cnid,
    $node->appointtype,
    $node->appointdate,
    $node->retiredate,
    $node->notes,
    $node->uid);
}

function committee_assignment_insert($node) {
  db_query("INSERT INTO {org_assignments} (nid, vid, mnid, rnid, assigntype, assigndate, ceasedate, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->mnid,
    $node->rnid,
    $node->assigntype,
    $node->assigndate,
    $node->ceasedate,
    $node->notes,
    $node->uid);
}



/**
 * Update functions called from hook_update() in module file
 */
function committee_committee_update($node) {
  db_query("UPDATE {org_committees} SET pnid = %d, committee = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->pnid,
    $node->committee,
    $node->notes,
    $node->uid,
    $node->vid);
}

function committee_role_update($node) {
  db_query("UPDATE {org_roles} SET cnid = %d, role = '%s', multiperson = %d, allowvacant = %d, notes = '%s', user = %d WHERE vid = %d",
    $node->cnid,
    $node->role,
    $node->multiperson,
    $node->allowvacant,
    $node->notes,
    $node->uid,
    $node->vid);
}

function committee_appointment_update($node) {
  db_query("UPDATE {org_appointments} SET mnid = %d, cnid = %d, appointtype = '%s', appointdate = '%s', retiredate = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->mnid,
    $node->cnid,
    $node->appointtype,
    $node->appointdate,
    $node->retiredate,
    $node->notes,
    $node->uid,
    $node->vid);
}

function committee_assignment_update($node) {
  db_query("UPDATE {org_assignments} SET mnid = %d, rnid = %d, assigntype = '%s', assigndate = '%s', ceasedate = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->mnid,
    $node->rnid,
    $node->assigntype,
    $node->assigndate,
    $node->ceasedate,
    $node->notes,
    $node->uid,
    $node->vid);      
}

/**
 * Delete functions called from hook_delete() in module file
 */
function committee_committee_delete ($node) {
  db_query("DELETE FROM {org_committees} WHERE nid = %d", $node->nid);
}

function committee_role_delete ($node) {
  db_query("DELETE FROM {org_roles} WHERE nid = %d", $node->nid);
}

function committee_appointment_delete ($node) {
  db_query("DELETE FROM {org_appointments} WHERE nid = %d", $node->nid);
}

function committee_assignment_delete ($node) {
  db_query("DELETE FROM {org_assignments} WHERE nid = %d", $node->nid);
}

/**
 * Actions called when triggers are fired
 */
function committee_committee_save_action() {
  //orgright_debug_msg('committee','Fn: committee_committee_save_action');
}
