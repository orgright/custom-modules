<?php
// $Id$

/**
 * @file
 * Provides additional functions for membership module.
 */

/**
 * Menu call back functions to display pages to users
 */
function membership_management() {
  // Home page of the membership management function - display links and description
  $output = '<p>The following membership management functions are available:</p>';
  $output .= '<h2>' . l('Manage members', 'membership/members') . '</h2>';
  $output .= '<p>List, add, update or delete members from the organisation\'s database.</p>';
  $output .= '<h2>' . l('Manage subscriptions', 'membership/subscriptions') . '</h2>';
  $output .= '<p>List, add, update or delete subscriptions including attaching them to individual members.</p>';
  $output .= '<h2>' . l('Manage subscription payments', 'membership/subpayments') . '</h2>';
  $output .= '<p>List, add, update or delete subscription payments including attaching them to individual member subscriptions.</p>';
  // set the breadcrumb trail
  orgright_breadcrumb(array());
  return $output;
}

function membership_members() {
  // entry page for managing members
  return membership_members_list();
}

function membership_subscriptions() {
  // entry page for managing subscriptions
  return membership_subscriptions_list();
}

function membership_subpayments() {
  // entry page for managing subscription payments
  return membership_subpayments_list();
}

function membership_members_list() {
  // Generate a members list
  $output = '';
  // get a list of membership status values to select
  $standings = membership_get_member_list_standings();
  $sql = "SELECT m.nid, m.category, m.member, m.address, m.city, m.postcode, m.email, m.phone FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND (m.standing = ''";
  foreach ($standings as $standing) {
    $sql .= " OR m.standing = '".$standing."'";
  }
  $sql .= ")  ORDER BY m.category, m.member";
  $result = db_query(db_rewrite_sql($sql));
  $category = '[none]';
  while ($member = db_fetch_object($result)) {
    if ($member->category <> $category) {
      if ($category <> '[none]') { $output .= '</ul>'; }
      $category = $member->category;
      $output .= '<p>Membership category: <strong>' . $category . '</strong><p><ul>';
    }
    // Generate a line of listing for this member
    $member_output = '';
    $member_output .= l($member->member, 'node/'.$member->nid) . ' ';
    $member_output .= l('[edit]', 'node/'.$member->nid.'/edit') . ' ';
    $member_output .= orgright_multi_field_to_string($member->phone, '; ') . ', ';
    $member_output .= orgright_multi_field_to_string($member->address, '; ') . ', ';
    $member_output .= $member->city . ', ';
    $member_output .= $member->postcode . ', ';
    $member_output .= orgright_multi_field_to_string($member->email, '; ');
    $output .= '<li>' . $member_output . '</li>';
  }
  if ($category <> '[none]') { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  //orgright_debug_msg('membership',$output);
  return $output;
}

function membership_members_add() {
  // Add a new member
  //orgright_debug_msg('membership','Fn: membership_members_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('member'); 
  // Set the page title to display
  drupal_set_title(t('Create Member'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('member_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  // and return results
  return $output;
}

function membership_members_import() {
  // Function to import member details from a specified CSV file
  //orgright_debug_msg('membership','Fn: membership_members_import');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  module_load_include('inc', 'membership', 'membership_form');
  $output = drupal_get_form('membership_import_members_form', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  return $output;
}

function membership_members_export() {
  // Function to export member details to a CSV file which is downloaded
  //orgright_debug_msg('membership','Fn: membership_members_import');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  module_load_include('inc', 'membership', 'membership_form');
  $output = drupal_get_form('membership_export_members_form', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  return $output;
}

function membership_subscriptions_list() {
  // Generate a list of subscriptions
  $output = '';
  $sql = "SELECT s.nid, s.mnid, s.scode, s.datefrom, s.dateto, s.amount FROM {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid ORDER BY s.mnid, s.scode";
  $result = db_query(db_rewrite_sql($sql));
  $mnid = 0;
  while ($subscription = db_fetch_object($result)) {
    if ($subscription->mnid <> $mnid) {
      if ($mnid) { $output .= '</ul>'; }
      // Extract the member name
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_result(db_query($sql, $subscription->mnid));
      $output .= '<p>Subscriptions for: ' . l($member, 'node/'.$subscription->mnid) . '<p><ul>';
      $mnid = $subscription->mnid;
    }
    // and calculate the total paid against this subscription
    $paysql = "SELECT SUM(p.payment) AS totpaid FROM {node} n JOIN {org_subpayments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.snid = %d";
    $totpaid = db_result(db_query($paysql, $subscription->nid));
    // Prepare the from and to dates for display
    $datefrom = date_format_date(date_make_date($subscription->datefrom), 'custom', 'd M Y');
    $dateto = date_format_date(date_make_date($subscription->dateto), 'custom', 'd M Y');
    // Generate a line of listing for this subscription
    $subscription_output = '';
    $subscription_output .= l('Subscription', 'node/'.$subscription->nid) . ' ';
    $subscription_output .= l('[edit]', 'node/'.$subscription->nid.'/edit') . ': ';
    $subscription_output .= $subscription->scode . ' ';
    $subscription_output .= 'From: ' . $datefrom . ' ';
    $subscription_output .= 'to: ' . $dateto . ', ';
    $subscription_output .= 'Amount: ' . $subscription->amount . ', ';
    $subscription_output .= 'Total paid: ' . $totpaid;
    $output .= '<li>' . $subscription_output . '</li>';
  }
  if ($mnid) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  return $output;
}

function membership_subscriptions_add() {
  // Add a new subscription
  //orgright_debug_msg('membership','Fn: membership_subscriptions_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('subscription'); 
  // Set the page title to display
  drupal_set_title(t('Create Subscription'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('subscription_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  // and return results
  return $output;
}

function membership_subscriptions_wizard() {
  // Wizard to facilitate the creation of multiple subscriptions
  //orgright_debug_msg('membership','Fn: membership_subscriptions_wizard');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  module_load_include('inc', 'membership', 'membership_form');
  $output = drupal_get_form('membership_subscription_wizard', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  return $output;
}

function membership_subpayments_list() {
  // Generate a list of subscription payments
  $output = '';
  $sql = "SELECT p.nid, p.mnid, p.snid, p.datepaid, p.payment FROM {node} n JOIN {org_subpayments} p ON p.nid = n.nid WHERE p.vid = n.vid ORDER BY p.mnid, p.snid";
  $result = db_query(db_rewrite_sql($sql));
  $mnid = 0;
  while ($subpayment = db_fetch_object($result)) {
    if ($subpayment->mnid <> $mnid) {
      if ($mnid) { $output .= '</ul>'; }
      // Extract the member name
      $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
      $member = db_result(db_query($sql, $subpayment->mnid));
      $output .= '<p>Subscription payments from: ' . l($member, 'node/'.$subpayment->mnid) . '<p><ul>';
      $mnid = $subpayment->mnid;
    }
    // Extract the subscription code and amount
    $sql = "SELECT s.scode, s.amount FROM {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.nid = %d";
    $subscription = db_fetch_object(db_query($sql, $subpayment->snid));
    // Prepare the date paid for display
    $datepaid = date_format_date(date_make_date($subpayment->datepaid), 'custom', 'd M Y');
    // Generate a line of listing for this payment
    $subpayment_output = '';
    $subpayment_output .= l('Payment', 'node/'.$subpayment->nid) . ' ';
    $subpayment_output .= l('[edit]', 'node/'.$subpayment->nid.'/edit') . ' ';
    $subpayment_output .= 'of $' . $subpayment->payment . '</a> ';
    $subpayment_output .= 'on ' . $datepaid . ' ';
    $subpayment_output .= 'for subscription: ' . l($subscription->scode, 'node/'.$subpayment->snid);
    $output .= '<li>' . $subpayment_output . '</li>';
  }
  if ($mnid) { $output .= '</ul>'; }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  return $output;
}

function membership_subpayments_add() {
  // Add a new subscription payment
  //orgright_debug_msg('membership','Fn: membership_subpayments_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('subpayment'); 
  // Set the page title to display
  drupal_set_title(t('Create Subscription Payment'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the node form constructor function
  $output = drupal_get_form('subpayment_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership'));
  // and return results
  return $output;
}

/**
 * Menu call back functions to generate autocomplete data sets
 */
function membership_autocomplete_members($string = '') {
  // Generate a list of members for the autocomplete js function
  $members = array();
  $qargs = array('%'.strtolower($string).'%');
  $sql = "SELECT DISTINCT member FROM {org_members} WHERE LOWER(member) LIKE '%s'";
  $msg = 'Autocomplete members for: ' . $qargs[0];
  //orgright_debug_msg('membership',$msg);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_members}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while($obj = db_fetch_object($result)) {
    $members[$obj->member] = $obj->member;
  }
  //print drupal_to_js($members); 
  // requires php 5.2:
  drupal_json($members);
  exit;
}

function membership_autocomplete_scodes($string = '') {
  // Generate a list of scodes for the autocomplete js function
  $scodes = array();
  $qargs = array('%'.strtolower($string).'%');
  $sql = "SELECT DISTINCT scode FROM {org_subscriptions} WHERE LOWER(scode) LIKE '%s'";
  $msg = 'Autocomplete scodes for: ' . $qargs[0];
  //orgright_debug_msg('membership',$msg);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_subscriptions}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while($obj = db_fetch_object($result)) {
    $scodes[$obj->scode] = $obj->scode;
  }
  //print drupal_to_js($scodes); 
  // requires php 5.2:
  drupal_json($scodes);
  exit;
}
