<?php
// $Id$

/**
 * @file
 * Provides additional functions for meeting module.
 */

/**
 * Load functions called from hook_load() in module file
 */
function meeting_meeting_load($node) {
  $query = "SELECT g.cnid, g.meeting, g.mtgtype, g.mtgstatus, g.mtgdate, g.start, g.finish, g.location, g.notes FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function meeting_agenda_item_load($node) {
  $query = "SELECT a.gnid, a.sequence, a.agendaitem, a.agitstatus, a.notes FROM {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function meeting_mtg_item_load($node) {
  $query = "SELECT i.gnid, i.mnid, i.itemname, i.itemtype, i.itemstatus, i.itemdate, i.notes FROM {node} n JOIN {org_mtg_items} i ON i.nid = n.nid WHERE i.vid = n.vid AND i.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}



/**
 * Node manipulation functions called from hook_nodeapi() in module file
 */
function meeting_meeting_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('meeting','Fn: meeting_meeting_nodeapi',6);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      if ($node->cnid) {
        // Load committee reference
        $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $committee = db_result(db_query($sql, $node->cnid));
        //orgright_debug_msg('meeting','... committee='.$committee,8);
        $node->committee = $committee;
      }
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('cnid','meeting','mtgtype','mtgstatus', 'mtgdate','start','finish','location');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of agenda items
      $sql = "SELECT a.nid, a.sequence, a.agendaitem, a.agitstatus FROM {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.gnid = %d ORDER BY a.sequence";
      $result = db_query($sql, $node->nid);
      while ($agenda_item = db_fetch_object($result)) {
        $node->agenda_items[] = (array)$agenda_item;
      }
      // Load an array of meeting items
      $sql = "SELECT i.nid, i.mnid, i.itemname, i.itemtype, i.itemstatus, i.itemdate FROM {node} n JOIN {org_mtg_items} i ON i.nid = n.nid WHERE i.vid = n.vid AND i.gnid = %d ORDER BY i.itemdate DESC";
      $result = db_query($sql, $node->nid);
      while ($mtg_item = db_fetch_object($result)) {
        // find the associated member's name
        if ($mtg_item->mnid) {
          $member = db_result(db_query("SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d",$mtg_item->mnid));
          $mtg_item->member = $member;
        }
        $node->mtg_items[] = (array)$mtg_item;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_meeting_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_meeting_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('meeting_event_meeting_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_meeting_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from meetings table
      db_query("DELETE FROM {org_meetings} WHERE vid = %d", $node->vid);
      return;
  }
}

function meeting_agenda_item_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('meeting','Fn: meeting_subscription_nodeapi',6);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      if ($node->gnid) {
        // Load meeting reference
        $sql = "SELECT g.cnid, g.meeting, g.mtgtype, g.mtgstatus, g.mtgdate FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
        $meeting = db_fetch_object(db_query($sql, $node->gnid));
        // find associated committee name
        $committee = db_result(db_query("SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d",$meeting->cnid));
        $node->cnid = $meeting->cnid;
        $node->committee = $committee;
        $node->meeting = $meeting->meeting;
        $node->mtgtype = $meeting->mtgtype;
        $node->mtgstatus = $meeting->mtgstatus;
        $node->mtgdate = $meeting->mtgdate;
      }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('gnid','sequence', 'agitstatus');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_agenda_item_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_agenda_item_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('meeting_event_agenda_item_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_agenda_item', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from agenda items table
      db_query("DELETE FROM {org_agenda_items} WHERE vid = %d", $node->vid);
      return;
  }
}

function meeting_mtg_item_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('meeting','Fn: meeting_subpayment_nodeapi',6);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      if ($node->gnid) {
        // Load meeting reference
        $sql = "SELECT g.cnid, g.meeting, g.mtgtype, g.mtgstatus, g.mtgdate FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
        $meeting = db_fetch_object(db_query($sql, $node->gnid));
        // find associated committee name
        $committee = db_result(db_query("SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d",$meeting->cnid));
        $node->cnid = $meeting->cnid;
        $node->committee = $committee;
        $node->meeting = $meeting->meeting;
        $node->mtgtype = $meeting->mtgtype;
        $node->mtgstatus = $meeting->mtgstatus;
        $node->mtgdate = $meeting->mtgdate;
      }
      if ($node->mnid) {
        // Load member reference
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $member = db_result(db_query($sql, $node->mnid));
        $node->member = $member;
      }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('gnid','mnid','itemname','itemtype','itemstatus','itemdate');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_mtg_item_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_mtg_item_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('meeting_event_mtg_item_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('meeting_event_mtg_item_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from meeting table
      db_query("DELETE FROM {org_mtg_items} WHERE vid = %d", $node->vid);
      return;
  }
}


/**
 * Insert functions called from hook_insert() in module file
 */
function meeting_meeting_insert($node) {
  db_query("INSERT INTO {org_meetings} (nid, vid, cnid, meeting, mtgtype, mtgstatus, mtgdate, start, finish, location, notes, user) VALUES (%d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->cnid,
    $node->meeting,
    $node->mtgtype,
    $node->mtgstatus,
    $node->mtgdate,
    $node->start,
    $node->finish,
    $node->location,
    $node->notes,
    $node->uid);
}

function meeting_agenda_item_insert($node) {
  db_query("INSERT INTO {org_agenda_items} (nid, vid, gnid, sequence, agendaitem, agitstatus, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->gnid,
    $node->sequence,
    $node->agendaitem,
    $node->agitstatus,
    $node->notes,
    $node->uid);
}

function meeting_mtg_item_insert($node) {
  db_query("INSERT INTO {org_mtg_items} (nid, vid, gnid, mnid, itemname, itemtype, itemstatus, itemdate, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->gnid,
    $node->mnid,
    $node->itemname,
    $node->itemtype,
    $node->itemstatus,
    $node->itemdate,
    $node->notes,
    $node->uid);
}

/**
 * Update functions called from hook_update() in module file
 */
function meeting_meeting_update($node) {
  db_query("UPDATE {org_meetings} SET cnid = %d, meeting = '%s', mtgtype = '%s', mtgstatus = '%s', mtgdate = '%s', start = '%s', finish = '%s', location = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->cnid,
    $node->meeting,
    $node->mtgtype,
    $node->mtgstatus,
    $node->mtgdate,
    $node->start,
    $node->finish,
    $node->location,
    $node->notes,
    $node->uid,
    $node->vid);
}

function meeting_agenda_item_update($node) {
  db_query("UPDATE {org_agenda_items} SET gnid = %d, sequence = %d, agendaitem = '%s', agitstatus = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->gnid,
    $node->sequence,
    $node->agendaitem,
    $node->agitstatus,
    $node->notes,
    $node->uid,
    $node->vid);
}

function meeting_mtg_item_update($node) {
  db_query("UPDATE {org_mtg_items} SET gnid = %d, mnid = %d, itemname = '%s', itemtype = '%s', itemstatus = '%s', itemdate = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->gnid,
    $node->mnid,
    $node->itemname,
    $node->itemtype,
    $node->itemstatus,
    $node->itemdate,
    $node->notes,
    $node->uid,
    $node->vid);      
}


/**
 * Delete functions called from hook_delete() in module file
 */
function meeting_member_delete($node) {
    db_query("DELETE FROM {org_meetings} WHERE nid = %d", $node->nid);
}

function meeting_subscription_delete($node) {
    db_query("DELETE FROM {org_agenda_items} WHERE nid = %d", $node->nid);
}

function meeting_subpayment_delete($node) {
    db_query("DELETE FROM {org_mtg_items} WHERE nid = %d", $node->nid);
}

