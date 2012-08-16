<?php
// $Id$

/**
 * @file
 * Provides additional functions for committee module.
 */

/**
 * Called from hook_form() in module file
 */
function committee_committee_form_build(&$form_state, $node) {
  //orgright_debug_msg('committee','Fn: committee_committee_form_build');
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating committees
  $form = array();
  $fix = 0;
  $fix++;
  $form['committee'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => committee_field_description('committee', 'committee', t('Enter the name of the committee')),
    '#default_value' => $node->committee,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $committees_list = committee_get_committees('nid');
  if (count($committees_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['parent'] = array(
    '#type' => 'textfield',
    '#title' => t('Parent'),
    '#description' => committee_field_description('committee', 'parent', t('Enter the name of the parent committee')),
    '#default_value' => $node->parent,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'committee/autocomplete/committees',
    );
  }
  else {
    // use a select type field with the values from the array
    $committees_list = array('[none]') + $committees_list;
    $form['parent'] = array(
      '#type' => 'select',
      '#title' => t('Parent'),
      '#description' => committee_field_description('committee', 'parent', t('Enter the name of the parent committee')),
      '#default_value' => $node->pnid,
      '#options' => $committees_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => committee_field_description('committee', 'body', t('Describe the committee and its functions')),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#required' => FALSE,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => committee_field_description('committee', 'notes', t('General notes about this committee')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['pnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('committee_form_validate');
  $form['#submit'] = array('committee_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee', t('Committees')=>'committee/committees'));
  return $form;
}

function committee_role_form_build(&$form_state, $node) {
  //orgright_debug_msg('committee','Fn: committee_role_form_build');
  // Specify the form for adding or updating roles
  // Get metadata for this node type we use it for labeling title and body fields).
  $form = array();
  $fix = 0;
  $fix++;
  $type = node_get_types('type', $node);
  $form['role'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => committee_field_description('role', 'role', t('Enter the name of the role')),
    '#default_value' => $node->role,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $committees_list = committee_get_committees('nid');
  if (count($committees_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['committee'] = array(
      '#type' => 'textfield',
      '#title' => t('Committee'),
      '#description' => committee_field_description('role', 'committee', t('Enter the name of the committee')),
      '#default_value' => $node->committee,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'committee/autocomplete/committees',
    );
  }
  else {
    // use a select type field with the values from the array
    $committees_list = array('[none]') + $committees_list;
    $form['committee'] = array(
      '#type' => 'select',
      '#title' => t('Committee'),
      '#description' => committee_field_description('role', 'committee', t('Enter the name of the committee')),
      '#default_value' => $node->cnid,
      '#options' => $committees_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['multiperson'] = array(
    '#type' => 'radios',
    '#title' => t('Multiple persons'),
    '#description' => committee_field_description('role', 'multiperson', t('Specify whether more than one person can be assigned to this role at the same time')),
    '#default_value' => $node->multiperson,
    '#options' => array(t('No'), t('Yes')),
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    
  );
  $fix++;
  $form['allowvacant'] = array(
    '#type' => 'radios',
    '#title' => t('Allow vacant'),
    '#description' => committee_field_description('role', 'allowvacant', t('Specify whether this role may be vacant at any time')),
    '#default_value' => $node->allowvacant,
    '#options' => array(t('No'), t('Yes')),
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => committee_field_description('role', 'body', t('Describe the role and its responsibilities')),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#required' => FALSE,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => committee_field_description('role', 'notes', t('General notes about this role')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['cnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('committee_form_validate');
  $form['#submit'] = array('committee_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee', t('Roles')=>'committee/roles'));
  return $form;
}

function committee_appointment_form_build(&$form_state, $node) {
  //orgright_debug_msg('committee','Fn: committee_appointment_form_build');
  // Specify the form for adding or updating appointments
  $form = array();
  $fix = 0;
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Member'),
      '#description' => committee_field_description('appointment', 'member', t('Enter the name of the member')),
      '#default_value' => $node->member,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['member'] = array(
      '#type' => 'select',
      '#title' => t('Member'),
      '#description' => committee_field_description('appointment', 'member', t('Enter the name of the member')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $committees_list = committee_get_committees('nid');
  if (count($committees_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['committee'] = array(
      '#type' => 'textfield',
      '#title' => t('Committee'),
      '#description' => committee_field_description('appointment', 'committee', t('Enter the name of the committee')),
      '#default_value' => $node->committee,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'committee/autocomplete/committees',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['committee'] = array(
      '#type' => 'select',
      '#title' => t('Committee'),
      '#description' => committee_field_description('appointment', 'committee', t('Enter the name of the committee')),
      '#default_value' => $node->cnid,
      '#options' => array('-- select committee --') + $committees_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $appointtype_list = committee_get_appointtypes();
  $form['appointtype'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#description' => committee_field_description('appointment', 'appointtype', t('Chose appointment type from the list')),
    '#default_value' => $node->appointtype,
    '#options' => $appointtype_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd-m-Y'; // Specify the date format
  $form['appointdate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Appointment date'),
    '#description' => committee_field_description('appointment', 'appointdate', t('Enter the date of appointment to the committee')),
    '#default_value' => $node->appointdate,
    '#date_format' => $dformat,
    '#date_year_range' => '-5:+5',
    //'#date_label_position' => 'within',
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['retiredate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Retirement date'),
    '#description' => committee_field_description('appointment', 'retiredate', t('Enter the date of retirement from the committee')),
    '#default_value' => $node->retiredate,
    '#date_format' => $dformat,
    '#date_year_range' => '-5:+5',
    //'#date_label_position' => 'within',
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => committee_field_description('appointment', 'notes', t('General notes about this committee appointment')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['cnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('committee_form_validate');
  $form['#submit'] = array('committee_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee', t('Appointments')=>'committee/appointments'));
  return $form;
}

function committee_assignment_form_build(&$form_state, $node) {
  //orgright_debug_msg('committee','Fn: committee_assignment_form_build');
  // Specify the form for adding or updating appointment payments
  $form = array();
  $fix = 0;
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Member'),
      '#description' => committee_field_description('assignment', 'member', t('Name of assigned member')),
      '#default_value' => $node->member,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['member'] = array(
      '#type' => 'select',
      '#title' => t('Member'),
      '#description' => committee_field_description('assignment', 'member', t('Name of assigned member')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  // Prepare an array of roles for a select field
  $fix++;
  $roles_list = committee_get_roles('nid');
  if (count($roles_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['role'] = array(
      '#type' => 'textfield',
      '#title' => t('Role name'),
      '#description' => committee_field_description('assignment', 'role', t('The name of this role')),
      '#default_value' => $node->role,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'committee/autocomplete/roles',
    );
    // and will require a committee selection field as well
    $committees_list = committee_get_committees('nid');
    if (count($committees_list) > orgright_get_selectfield_max()) {
      // too many entries, so use an autocomplete field instead
      $form['committee'] = array(
        '#type' => 'textfield',
        '#title' => t('Committee'),
        '#description' => committee_field_description('assignment', 'committee', t('Enter the name of the committee')),
        '#default_value' => $node->committee,
        '#maxlength' => 64,
        '#size' => 40,
        '#required' => TRUE,
        '#attributes' => array('tabindex'=>$fix),
        '#weight' => $fix,
        '#autocomplete_path' => 'committee/autocomplete/committees',
      );
    }
    else {
      // use a select type field with the values from the array
      $form['committee'] = array(
        '#type' => 'select',
        '#title' => t('Committee'),
        '#description' => committee_field_description('assignment', 'committee', t('Enter the name of the committee')),
        '#default_value' => $node->cnid,
        '#options' => array('-- select committee --') + $committees_list,
        '#required' => TRUE,
        '#attributes' => array('tabindex'=>$fix),
        '#weight' => $fix,
      );
    }
  }
  else {
    $form['role'] = array(
      '#type' => 'select',
      '#title' => t('Role name'),
      '#description' => committee_field_description('assignment', 'role', t('The name of this role')),
      '#default_value' => $node->rnid,
      '#options' => array('-- select role --') + $roles_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $assigntype_list = committee_get_assigntypes();
  $form['assigntype'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#description' => committee_field_description('assignment', 'assigntype', t('Chose assignment type from the list')),
    '#default_value' => $node->assigntype,
    '#options' => $assigntype_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd-m-Y'; // Specify the date format
  $form['assigndate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Assignment date'),
    '#description' => committee_field_description('assignment', 'assigndate', t('Enter the date the role was assigned')),
    '#default_value' => $node->assigndate,
    '#date_format' => $dformat,
    '#date_year_range' => '-5:+5',
    //'#date_label_position' => 'within',
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['ceasedate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Cease date'),
    '#description' => committee_field_description('assignment', 'ceasedate', t('Enter the date the assignment to the role ceased')),
    '#default_value' => $node->ceasedate,
    '#date_format' => $dformat,
    '#date_year_range' => '-5:+5',
    //'#date_label_position' => 'within',
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => committee_field_description('assignment', 'notes', t('General notes about this role assignment')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['cnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['rnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  // Save a value for later use
  $form['save_assigndate'] = array(
    '#type' => 'value',
    '#value' => $node->assigndate,
  );
  $form['save_ceasedate'] = array(
    '#type' => 'value',
    '#value' => $node->ceasedate,
  );
  // save the serialised array of original values
  $form['original'] = array(
    '#type' => 'value',
    '#value' => $node->original,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  $form['#validate'] = array('committee_form_validate');
  $form['#submit'] = array('committee_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Committee')=>'committee', t('Assignments')=>'committee/assignments'));
  return $form;
}

/**
 *  Function to generate a field description with popup help for each form field
 */
function committee_field_description($type, $field, $description) {
  return theme('helpservice_topic','committee',$type.'-details/'.$field).$description;
}

/**
 * Implementation of hook_form_alter()
 */
function committee_form_committee_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_form_committee_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'parent':
        // fill in the parent field information
        if ($form['parent']['#type'] == 'select') {
          // is a select type field, use nid
          $form['parent']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT committee FROM {node} n JOIN {org_committees} c on c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
          $form['parent']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      default:
      }
    }
  }
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add committee';
    $form['#goto'] = 'committee/committees/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function committee_form_role_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_form_role_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'committee':
        // fill in the committee field information
        if ($form['committee']['#type'] == 'select') {
          // is a select type field, use nid
          $form['committee']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT committee FROM {node} n JOIN {org_committees} c on c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
          $form['committee']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      default:
      }
    }
  }
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add role';
    $form['#goto'] = 'committee/roles/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function committee_form_appointment_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_form_appointment_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'committee':
        // fill in the committee field information
        if ($form['committee']['#type'] == 'select') {
          // is a select type field, use nid
          $form['committee']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT committee FROM {node} n JOIN {org_committees} c on c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
          $form['committee']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'member':
        // fill in the member field information
        $sql = "SELECT member FROM {node} n JOIN {org_members} m on m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $member = db_result(db_query($sql, $parvalue));
        $form['member']['#default_value'] = $member;
        break;
      default:
      }
    }
  }
  // Suppress the formating information
  unset($form['body_filter']['filter']);
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add appointment';
    $form['#goto'] = 'committee/appointments/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function committee_form_assignment_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_form_assignment_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'role':
        // fill in the role field information
        if ($form['role']['#type'] == 'select') {
          // is a select type field, use nid
          $form['role']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT role FROM {node} n JOIN {org_roles} r on r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
          $form['role']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'committee':
        // fill in the committee field information
        if ($form['committee']['#type'] == 'select') {
          // is a select type field, use nid
          $form['committee']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT committee FROM {node} n JOIN {org_committees} c on c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
          $form['committee']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'member':
        // fill in the member field information
        $sql = "SELECT member FROM {node} n JOIN {org_members} m on m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $member = db_result(db_query($sql, $parvalue));
        $form['member']['#default_value'] = $member;
        break;
      default:
      }
    }
  }
  // Suppress the formating information
  unset($form['body_filter']['filter']);
  // Don't want to create a revision
  unset($form['revision_information']);
  // Unset default promote and sticky flags
  $form['options']['promote']['#default_value'] = 0;
  $form['options']['sticky']['#default_value'] = 0;
  // Remove preview button and change label on submit button
  // and specify the redirect page on successful submission
  unset($form['buttons']['preview']);
  if (empty($form['nid']['#value'])) {  
    $form['buttons']['submit']['#value'] = 'Add assignment';
    $form['#goto'] = 'committee/assignments/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

/**
 * Validation callback routines
 * Called from function comittee_form_validate() in module file
 */
function committee_committee_form_validate($form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_committee_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $committee = strip_tags($form_state['values']['committee']);
    $parent = strip_tags($form_state['values']['parent']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($committee != $form_state['values']['committee']) {
        form_set_error('committee',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($parent != $form_state['values']['parent']) {
        form_set_error('parent',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that any name returned in "parent" is a valid committee
    if ($parent) {
      if (is_numeric($parent)) {
        $sql = "SELECT n.type, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $cobj = db_fetch_object(db_query($sql, $parent));
        if ($cobj->type == 'committee') {
          $pnid = $parent;
          $parent = $cobj->committee;
        }
        else {
          form_set_error('parent',t('The name specified is not a known committee'));
          $pnid = 0;
          $parent = '';
        }
      }
      else {
        $pnid = committee_validate_committee($form_state['values']['parent']);
      }
      if ($pnid) {
        form_set_value($form['pnid'], $pnid, $form_state);
      }
      else {
        form_set_error('parent',t('The name specified is not a known committee'));
      }
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.committee = '%s'";
    $nid = db_result(db_query($sql, $committee));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('committee',t('The committee name is already in use'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Committee: !cmtte', array('!cmtte'=>$committee));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached sub-committees - delete not permitted in this case
    $sql = "SELECT COUNT(c.nid) from {node} n JOIN {org_committee} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.pnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('committee',t('This committee still has @num sub-committees attached!', array('@num'=>$count)));
    }
    // Check for specified roles - delete not permitted in this case
    $sql = "SELECT COUNT(r.nid) from {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.cnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('committee',t('This committee still has @num roles specified!', array('@num'=>$count)));
    }
    // Check for appointments - delete not permitted in this case
    $sql = "SELECT COUNT(p.nid) from {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.cnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('scode',t('This committee still has @num people appointed!', array('@num'=>$count)));
    }
  }
}

function committee_role_form_validate($form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_role_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $role = strip_tags($form_state['values']['role']);
    $committee = strip_tags($form_state['values']['committee']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($role != $form_state['values']['role']) {
        form_set_error('role',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($committee != $form_state['values']['committee']) {
        form_set_error('committee',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // If there is a value in "committee", then check
    if ($committee) {
      // If "committee" is numeric, then it should be the nid of a committee node
      if (is_numeric($committee)) {
        $sql = "SELECT n.type, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $cobj = db_fetch_object(db_query($sql, $committee));
        if ($cobj->type == 'committee') {
          $cnid = $committee;
          $committee = $cobj->committee;
        }
        else {
          form_set_error('committee',t('The name specified is not a known committee'));
          $cnid = 0;
          $committee = '';
        }
      }
      else {
        // Check that any name returned in "committee" is a valid committee
        $cnid = committee_validate_committee($form_state['values']['committee']);
      }
      if ($cnid) {
        form_set_value($form['cnid'], $cnid, $form_state);
      }
      else {
        form_set_error('committee',t('The name specified is not a known committee'));
      }
    }
    else { $cnid = 0; }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.role = '%s' AND r.cnid = %d";
    $nid = db_result(db_query($sql, $role, $cnid));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('role',t('Role already specified for this committee'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Role: !role', array('!role'=>$role));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for assignments - delete not permitted in this case
    $sql = "SELECT COUNT(s.nid) from {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.rnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('role',t('This role still has @num people assigned!', array('@num'=>$count)));
    }
  }
}

function committee_appointment_form_validate($form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_appointment_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $member = strip_tags($form_state['values']['member']);
    $committee = strip_tags($form_state['values']['committee']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($committee != $form_state['values']['committee']) {
        form_set_error('committee',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check date values
    if ($form_state['values']['retiredate'] && $form_state['values']['retiredate'] < $form_state['values']['appointdate']) {
      form_set_error('retiredate',t('The retirement date must not be earlier then the appointment date'));
    }
    // Check that the name returned in "member" is valid
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The name specified is not a known member'));
    }
    // If "committee" is numeric, then it should be the nid of a committee node
    if (is_numeric($committee)) {
      $sql = "SELECT n.type, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
      $cobj = db_fetch_object(db_query($sql, $committee));
      if ($cobj->type == 'committee') {
        $cnid = $committee;
        $committee = $cobj->committee;
      }
      else {
        form_set_error('committee',t('The name specified is not a known committee'));
        $cnid = 0;
        $committee = '';
      }
    }
    else {
      // Check that the name returned in "committee" is valid
      $cnid = committee_validate_committee($committee);
    }
    if ($cnid) {
      form_set_value($form['cnid'], $cnid, $form_state);
    }
    else {
      form_set_error('committee',t('The name specified is not a known committee'));
    }
    // Build a composite node title and save in $form_state
    $adate = date_format_date(date_make_date($form_state['values']['appointdate']), 'custom', 'd M Y');
    $form_state['values']['title'] = t('Committee appointment: !mbr to !cmtte on !date', array('!mbr'=>$member, '!cmtte'=>$committee, '!date'=>$adate));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //membership_debug_msg('--- Delete button clicked ---');
  }
}

function committee_assignment_form_validate($form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_assignment_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $member = strip_tags($form_state['values']['member']);
    $committee = strip_tags($form_state['values']['committee']);
    $role = strip_tags($form_state['values']['role']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($member != strip_tags($form_state['values']['member'])) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    /*if ($committee != strip_tags($form_state['values']['committee'])) {
        form_set_error('committee',t('HTML or PHP tags are not permitted in this field'));
    }*/
    if ($role != strip_tags($form_state['values']['role'])) {
        form_set_error('role',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != strip_tags($form_state['values']['notes'])) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check date values
    if ($form_state['values']['ceasedate'] && $form_state['values']['ceasedate'] < $form_state['values']['assigndate']) {
      form_set_error('ceasedate',t('The cease date must not be earlier then the assignment date'));
    }
    // Check that the name returned in "member" is valid
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The name specified is not a known member'));
    }
    // If the value in $role is numeric, then is should be the nid of a role node
    if (is_numeric($role)) {
      $sql = "SELECT n.type, r.role FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
      $robj = db_fetch_object(db_query($sql, $role));
      if ($robj->type == 'role') {
        $rnid = $role;
        $role = $robj->role;
      }
      else {
        form_set_error('role',t('The name specified is not a known committee role'));
        $rnid = 0;
        $role = '';
      }
    }
    else {
      // If the value in $committee is numeric, it should be the nid of a committee node
      if (is_numeric($committee)) {
        $sql = "SELECT n.type, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
        $cobj = db_fetch_object(db_query($sql, $committee));
        if ($cobj->type == 'committee') {
          $cnid = $committee;
          $committee = $cobj->committee;
        }
        else {
          form_set_error('committee',t('The name specified is not a known committee'));
          $cnid = 0;
          $committee = '';
        }
      }
      else {
        // Check that the name returned in "committee" is valid
        $cnid = committee_validate_committee($committee);
      }
      if ($cnid) {
        form_set_value($form['cnid'], $cnid, $form_state);
      }
      else {
        form_set_error('committee',t('The name specified is not a known committee'));
      }
      // Check that the name returned in "role" is valid for this committee (already have cnid)
      $sql = "SELECT r.nid FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.cnid = %d AND r.role = '%s'";
      $rnid = db_result(db_query($sql, $cnid, $role));
    }
    if ($rnid) {
      form_set_value($form['rnid'], $rnid, $form_state);
    }
    else {
      form_set_error('role',t('The name specified is not a known role for this committee'));
    }
    if (!$form_state['values']['ceasedate']  && $cnid) {
      // Check that the assigned member is also currently on the committee
      $sql = "SELECT p.nid FROM {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.cnid = %d AND p.mnid = %d AND p.retiredate = ''";
      if (!db_result(db_query($sql, $cnid, $mnid))) {
        drupal_set_message(t('WARNING: The specified member is not currently appointed to the committee.'), 'warning');
      }
    }
    // Now load some details for this role from the database to check multiperson or vacant status
    $sql = "SELECT r.multiperson, r.allowvacant FROM {node} n JOIN {org_roles} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
    $roleobj = db_fetch_object(db_query($sql, $rnid));
    $sql = "SELECT COUNT(s.nid) FROM {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.rnid = %d AND s.ceasedate = ''";
    $numassignments = db_result(db_query($sql, $rnid));
    if (!count(form_get_errors())) {
      // adjust the number of assignments to reflect this update
      if ($form_state['values']['save_ceasedate']) {
        // assignment was previously ceased
        if (!$form_state['values']['ceasedate']) { $numassignments += 1; }
      }
      else {
        if ($form_state['values']['save_assigndate']) {
          // assignment was previously active
          if ($form_state['values']['ceasedate']) { $numassignments -= 1; }
        }
        else {
          // new assignment
          if (!$form_state['values']['ceasedate']) { $numassignments += 1; }
        }
      }
    }
    //orgright_debug_msg('committee','number of assignments='.$numassignments);
    // If the role is already filled, but is not multiperson, then issue a warning
    if ($numassignments > 1 && !$roleobj->multiperson) {
      drupal_set_message(t('WARNING: The role now has @num people assigned, but is not specified as permiting multiple simultaneous assignments.', array('@num'=>$numassignments)), 'warning');
    }
    // If this assignment is ceased, then check whether the role is vacant when it is not permitted to be
    if ($numassignments < 1 && !$roleobj->allowvacant) {
      // role is vacant but not allowed to be so - issue warning
      drupal_set_message(t('WARNING: The role is now vacant, but is specified as being required to be filled.'), 'warning');
    }
    // Build a composite node title and save in $form_state
    $adate = date_format_date(date_make_date($form_state['values']['assigndate']), 'custom', 'd M Y');
    $form_state['values']['title'] = t('Role assignment: !mbr to !role on !date', array('!mbr'=>$member, '!role'=>$role, '!date'=>$adate));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //membership_debug_msg('--- Delete button clicked ---');
  }
}

function committee_validate_committee(&$committee) {
  // Check that the value passed in "$committee" is valid, and return the associated nid
  //orgright_debug_msg('committee',t('Validate committee name: @cmt', array('@cmt' => $committee)));
  if (is_numeric($committee)) {
    // then the passed value should be the nid of a committee node
    $cnid = $committee;
    $sql = "SELECT n.type, c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
    $obj = db_fetch_object(db_query($sql, $cnid));
    if ($obj->type == 'committee') { $committee = $obj->committee; }
    else { $cnid = 0; }
    return $cnid;
  }
  else {
    // Query the database to ensure that the committee name is valid, and to find the mnid
    $sql = "SELECT c.nid FROM {org_committees} c WHERE c.committee = '%s'";
    return db_result(db_query($sql, $committee));
  }
}

/**
 * committee form submit callback functions
 * Specifed in the form building routines
 */
function committee_committee_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('committee','Fn: committee_committee_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function committee_role_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('committee','Fn: committee_role_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function committee_appointment_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('committee','Fn: committee_appointment_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function committee_assignment_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('committee','Fn: committee_assignment_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}


