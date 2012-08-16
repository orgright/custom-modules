<?php
// $Id$

/**
 * @file
 * Provides additional functions for membership module.
 */

/**
 * Load functions called from hook_load() in module file
 */
function membership_member_load($node) {
  $query = "SELECT m.pnid, m.category, m.standing, m.member, m.address, m.city, m.postcode, m.email, m.phone, m.joindate, m.leavedate, m.notes FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function membership_subscription_load($node) {
  $query = "SELECT s.mnid, s.scode, s.datefrom, s.dateto, s.amount, s.notes FROM {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function membership_subpayment_load($node) {
  $query = "SELECT p.mnid, p.snid, p.datepaid, p.payment, p.notes FROM {node} n JOIN {org_subpayments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}



/**
 * Node manipulation functions called from hook_nodeapi() in module file
 */
function membership_member_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('membership','Fn: membership_member_nodeapi, op='.$op);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      // Load parent reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_fetch_object(db_query($sql, $node->pnid));
      $node->parent = $member->member;
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('pnid','category','standing','email','joindate', 'leavedate');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of children
      $sql = "SELECT m.nid, m.member, m.address, m.city, m.postcode, m.email, m.phone FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.pnid = %d ORDER BY m.member";
      $result = db_query($sql, $node->nid);
      while ($child = db_fetch_object($result)) {
        $node->children[] = (array)$child;
      }
      // Load an array of subscriptions
      $sql = "SELECT s.nid, s.scode, s.datefrom, s.dateto, s.amount FROM {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.mnid = %d ORDER BY scode DESC";
      $result = db_query($sql, $node->nid);
      while ($subscription = db_fetch_object($result)) {
        $node->subscriptions[] = (array)$subscription;
      }
      // If committee module enabled, also load appointments and assignments
      if (function_exists('committee_init')) {
        // Load an array of appointments
        $sql = "SELECT p.nid, p.cnid, p.appointtype, p.appointdate, p.retiredate FROM {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.mnid = %d ORDER BY p.appointdate DESC";
        $result = db_query($sql, $node->nid);
        while ($appointment = db_fetch_object($result)) {
          // extract the committee name
          $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
          $committee = db_result(db_query($sql, $appointment->cnid));
          $appointment->committee = $committee;
          $node->appointments[] = (array)$appointment;
        }
        // Load an array of assignments
        $sql = "SELECT s.nid, s.rnid, s.assigntype, s.assigndate, s.ceasedate FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.mnid = %d ORDER BY s.assigndate DESC";
        $result = db_query($sql, $node->nid);
        while ($assignment = db_fetch_object($result)) {
          // extract the role name
          $sql = "SELECT r.role FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
          $role = db_result(db_query($sql, $assignment->rnid));
          $assignment->role = $role;
          $node->assignments[] = (array)$assignment;
        }
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_member_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_member_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('membership_event_member_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_member_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from membership table
      db_query("DELETE FROM {org_members} WHERE vid = %d", $node->vid);
      return;
  }
}

function membership_subscription_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('membership','Fn: membership_subscription_nodeapi');
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      // Load member reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_fetch_object(db_query($sql, $node->mnid));
      $node->member = $member->member;
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('mnid','scode','datefrom'.'dateto'.'amount');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of payments
      $totpaid = 0;
      $sql = "SELECT p.nid, p.datepaid, p.payment FROM {node} n JOIN {org_subpayments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.snid = %d ORDER BY datepaid DESC";
      $result = db_query($sql, $node->nid);
      while ($subpayment = db_fetch_object($result)) {
        $totpaid = $totpaid + $subpayment->payment;
        $node->subpayments[] = (array)$subpayment;
      }
      $node->totpaid = $totpaid;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_subscription_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_subscription_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('membership_event_subscription_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_subscription_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from membership table
      db_query("DELETE FROM {org_subscriptions} WHERE vid = %d", $node->vid);
      return;
  }
}

function membership_subpayment_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('membership','Fn: membership_subpayment_nodeapi');
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      // Load member reference
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_fetch_object(db_query($sql, $node->mnid));
      $node->member = $member->member;
      // Load subscription reference and amount
      $sql = "SELECT s.scode, s.amount from {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.nid = %d";
      $subscription = db_fetch_object(db_query($sql, $node->snid));
      $node->scode = $subscription->scode;
      $node->amount = $subscription->amount;
      // Specify the fields that are used for rules event triggers on change
      $fields = array('mnid','snid','datepaid','payment');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_subpayment_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_subpayment_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('membership_event_subpayment_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('membership_event_subpayment_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from membership table
      db_query("DELETE FROM {org_subpayments} WHERE vid = %d", $node->vid);
      return;
  }
}


/**
 * Insert functions alled from hook_insert() in module file
 */
function membership_member_insert($node) {
  db_query("INSERT INTO {org_members} (nid, vid, pnid, category, standing, member, address, city, postcode, email, phone, joindate, leavedate, notes, user) VALUES (%d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->pnid,
    $node->category,
    $node->standing,
    $node->member,
    $node->address,
    $node->city,
    $node->postcode,
    $node->email,
    $node->phone,
    $node->joindate,
    $node->leavedate,
    $node->notes,
    $node->uid);
}

function membership_subscription_insert($node) {
  db_query("INSERT INTO {org_subscriptions} (nid, vid, mnid, scode, datefrom, dateto, amount, notes, user) VALUES (%d, %d, %d, '%s', '%s', '%s', %f, '%s', %d)",
    $node->nid,
    $node->vid,
    $node->mnid,
    $node->scode,
    $node->datefrom,
    $node->dateto,
    $node->amount,
    $node->notes,
    $node->uid);
}

function membership_subpayment_insert($node) {
  db_query("INSERT INTO {org_subpayments} (nid, vid, mnid, snid, datepaid, payment, notes, user) VALUES (%d, %d, %d, %d, '%s', %f, '%s', %d)",
    $node->nid,
    $node->vid,
    $node->mnid,
    $node->snid,
    $node->datepaid,
    $node->payment,
    $node->notes,
    $node->uid);
}

/**
 * Update functions called from hook_update() in module file
 */
function membership_member_update($node) {
  db_query("UPDATE {org_members} SET pnid = %d, category = '%s', standing = '%s', member = '%s', address = '%s', city = '%s', postcode = '%s', email = '%s', phone = '%s', joindate = '%s', leavedate = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->pnid,
    $node->category,
    $node->standing,
    $node->member,
    $node->address,
    $node->city,
    $node->postcode,
    $node->email,
    $node->phone,
    $node->joindate,
    $node->leavedate,
    $node->notes,
    $node->uid,
    $node->vid);
}

function membership_subscription_update($node) {
  db_query("UPDATE {org_subscriptions} SET mnid = %d, scode = '%s', datefrom = '%s', dateto = '%s', amount = %f, notes = '%s', user = %d WHERE vid = %d",
    $node->mnid,
    $node->scode,
    $node->datefrom,
    $node->dateto,
    $node->amount,
    $node->notes,
    $node->uid,
    $node->vid);
}

function membership_subpayment_update($node) {
  db_query("UPDATE {org_subpayments} SET mnid = %d, snid = %d, datepaid = '%s', payment = %f, notes = '%s', user = %d WHERE vid = %d",
    $node->mnid,
    $node->snid,
    $node->datepaid,
    $node->payment,
    $node->notes,
    $node->uid,
    $node->vid);      
}


/**
 * Delete functions called from hook_delete() in module file
 */
function membership_member_delete($node) {
    db_query("DELETE FROM {org_members} WHERE nid = %d", $node->nid);
}

function membership_subscription_delete($node) {
    db_query("DELETE FROM {org_subscriptions} WHERE nid = %d", $node->nid);
}

function membership_subpayment_delete($node) {
    db_query("DELETE FROM {org_subpayments} WHERE nid = %d", $node->nid);
}

