<?php
// $Id$

/**
 * @file
 * Provides additional functions for membership module.
 */

/**
 * Functions called from hook_form() in membership.module
 */
function membership_member_form_build(&$form_state, $node) {
  //orgright_debug_msg('membership','Fn: membership_member_form_build');
  // Specify the form for adding or updating members
  $form = array();
  $fix = 0;
  $fix++;
  $form['member'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => membership_field_description('member-details', 'member', t('Enter the name of the member')),
    '#default_value' => $node->member,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['address'] = array(
    '#type' => 'textarea',
    '#title' => t('Address'),
    '#description' => membership_field_description('member-details', 'address', t('Enter the street address and suburb')),
    '#default_value' => $node->address,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#description' => membership_field_description('member-details', 'city', t('Enter the address city or town')),
    '#default_value' => $node->city,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['postcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode'),
    '#description' => membership_field_description('member-details', 'postcode', t('Enter the address postcode')),
    '#default_value' => $node->postcode,
    '#maxlength' => 16,
    '#size' => 16,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['phone'] = array(
    '#type' => 'textarea',
    '#title' => t('Phone'),
    '#description' => membership_field_description('member-details', 'phone', t('Enter phone number(s), one per line.  Show type (Home, Work, Mobile, etc) as well')),
    '#default_value' => $node->phone,
    '#rows' => 2,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['email'] = array(
    '#type' => 'textarea',
    '#title' => t('Email'),
    '#description' => membership_field_description('member-details', 'email', t('Enter email address(es), one per line.  The preferred email address should be the first.')),
    '#default_value' => $node->email,
    '#rows' => 2,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $category_list = membership_get_categories();
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Category'),
    '#description' => membership_field_description('member-details', 'category', t('Chose membership category from the list')),
    '#default_value' => $node->category,
    '#options' => $category_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $standing_list = membership_get_standings();
  $form['standing'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => membership_field_description('member-details', 'standing', t('Chose the membership status from the list')),
    '#default_value' => $node->standing,
    '#options' => $standing_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['joindate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Date joined'),
    '#description' => membership_field_description('member-details', 'joindate', t('Specify the date this member joined the organisation')),
    '#default_value' => $node->joindate,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#date_year_range' => '-7:+1',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['leavedate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Date left'),
    '#description' => membership_field_description('member-details', 'leavedate', t('Specify the date this member left the organisation')),
    '#default_value' => $node->leavedate,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#date_year_range' => '-7:+1',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['parent'] = array(
      '#type' => 'textfield',
      '#title' => t('Primary member'),
      '#description' => membership_field_description('member-details', 'parent', t('If this is a subsidiary membership, enter the name of the primary member')),
      '#default_value' => $node->parent,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['parent'] = array(
      '#type' => 'select',
      '#title' => t('Primary member'),
      '#description' => membership_field_description('member-details', 'parent', t('If this is a subsidiary membership, enter the name of the primary member')),
      '#default_value' => $node->pnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => membership_field_description('member-details', 'notes', t('General notes about this member')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['pnid'] = array(
    '#type' => 'value',
    '#value' => $node->pnid,
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
  $form['#validate'] = array('membership_form_validate');
  $form['#submit'] = array('membership_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership', t('Members')=>'membership/members'));
  return $form;
}

function membership_subscription_form_build(&$form_state, $node) {
  //orgright_debug_msg('membership','Fn: membership_subscription_form_build');
  // Specify the form for adding or updating subscriptions
  $form = array();
  $fix = 0;
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Member'),
      '#description' => membership_field_description('subscription-details', 'member', t('Name of member for this subscription')),
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
      '#description' => membership_field_description('subscription-details', 'member', t('Name of member for this subscription')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['scode'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription code'),
    '#description' => membership_field_description('subscription-details', 'scode', t('Enter the subscription code')),
    '#default_value' => $node->scode,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['datefrom'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Start date'),
    '#description' => membership_field_description('subscription-details', 'datefrom', t('Enter the start date of the subscription')),
    '#default_value' => $node->datefrom,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['dateto'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('End date'),
    '#description' => membership_field_description('subscription-details', 'dateto', t('Enter the end date of the subscription')),
    '#default_value' => $node->dateto,
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription amount'),
    '#description' => membership_field_description('subscription-details', 'amount', t('Enter the amount of the subscription')),
    '#default_value' => $node->amount,
    '#maxlength' => 16,
    '#size' => 16,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => membership_field_description('subscription-details', 'notes', t('General notes about this subscription')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => $node->mnid,
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
  $form['#validate'] = array('membership_form_validate');
  $form['#submit'] = array('membership_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership', t('Subscriptions')=>'membership/subscriptions'));
  return $form;
}

function membership_subpayment_form_build(&$form_state, $node) {
  //orgright_debug_msg('membership','Fn: membership_subpayment_form_build');
  // Specify the form for adding or updating subscription payments
  $form = array();
  $fix = 0;
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Member'),
      '#description' => membership_field_description('payment-details', 'member', t('Name of member for this subscription payment')),
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
      '#description' => membership_field_description('payment-details', 'member', t('Name of member for this subscription payment')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['scode'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription code'),
    '#description' => membership_field_description('payment-details', 'scode', t('Subscription that this payment is for')),
    '#default_value' => $node->scode,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'membership/autocomplete/scodes',
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['datepaid'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Payment date'),
    '#description' => membership_field_description('payment-details', 'datepaid', t('Enter the date of the payment')),
    '#default_value' => ($node->datepaid) ? $node->datepaid : orgright_get_date_ymd(),
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['payment'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment amount'),
    '#description' => membership_field_description('payment-details', 'payment', t('Enter the amount of this payment')),
    '#default_value' => $node->payment,
    '#maxlength' => 16,
    '#size' => 16,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => membership_field_description('payment-details', 'notes', t('General notes about this payment')),
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
  $form['snid'] = array(
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
  $form['#validate'] = array('membership_form_validate');
  $form['#submit'] = array('membership_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership', t('Payments')=>'membership/subpayments'));
  return $form;
}

function membership_import_members_form_build(&$form_state) {
  //orgright_debug_msg('membership','Fn: membership_import_members_form_build');
  // Build the forms required for the membership members import
  drupal_set_title(t('Membership - Import Member Details'));
  $form = array();
  $fix = 0;
  $fix++;
  $form['instructions'] = array(
    '#title' => t('Import Member Details'),
    '#type' => 'item',
    '#description' => t('Member details can be imported from an existing membership database using a prepared CSV file.  The first line in this file must contain field names corresponding to the orgRight member details fields (see below).  These can be in any order. The "name" field is required, but all others are optional. Information from any unrecognised fields will be imported into the "notes" field. Any html and php tags will be removed from all imported fields, so convert all angle brackets ("&lt;" and "&gt;") to round, square, or curly brackets.'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['field_names'] = array(
    '#title' => t('Member Details Fields'),
    '#type' => 'item',
    '#description' => t('The following fields are available for member details: "name", "address", "city", "postcode", "phone", "email", "category", "status", "date joined", "date left", "primary member", and "notes". The "address", "phone", "phone" and "notes" fields are multi-line capable, so encode line-breaks with semi-colon + question mark pairs (";?"). The "date joined" and "date left" fields should be formatted as "dd-mm-yyyy". Imported category and status values will be checked against the list of currently defined values, but there are options below to add new values to the current list.'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // CSV file fieldset
  $fix++;
  $form['csv_file'] = array(
    '#type' => 'fieldset',
    '#access' => TRUE,
    '#title' => t('CSV file'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    //'#description' => t(''),
    '#prefix' => '<div class="attachments">',
    '#suffix' => '</div>',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $max_file_size = 1024 * 1024;
  $description = membership_field_description('member-import', 'selectfile', t('The maximum upload size is %filesize.', array('%filesize' => format_size($max_file_size))));
  $form['csv_file']['import'] = array(
    '#type' => 'file',
    '#title' => t('Select file'),
    '#size' => 40,
    '#description' => $description,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $category_list = membership_get_categories();
  $form['default_category'] = array(
    '#type' => 'select',
    '#title' => t('Default category'),
    '#description' => membership_field_description('member-import', 'defaultcategory', t('Select a category to be given to imported members without a category specified')),
    '#default_value' => 0,
    '#options' => $category_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['add_categories'] = array(
    '#type' => 'select',
    '#title' => t('Add new categories'),
    '#description' => membership_field_description('member-import', 'addnewcategory', t('Should new categories in the import file be added to the current list?')),
    '#default_value' => 1,
    '#options' => array('No', 'Yes'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $standing_list = membership_get_standings();
  $form['default_standing'] = array(
    '#type' => 'select',
    '#title' => t('Default status'),
    '#description' => membership_field_description('member-import', 'defaultstatus', t('Select a status to be given to imported members without a status specified')),
    '#default_value' => 0,
    '#options' => $standing_list,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['add_standings'] = array(
    '#type' => 'select',
    '#title' => t('Add new status values'),
    '#description' => membership_field_description('member-import', 'addnewstatus', t('Should new status values in the import file be added to the current list?')),
    '#default_value' => 1,
    '#options' => array('No', 'Yes'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import members'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // Create placeholder field
  $form['afid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['#validate'] = array('membership_form_validate');
  $form['#submit'] = array('membership_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership', t('Members')=>'membership/members'));
  return $form;
}

function membership_export_members_form_build(&$form_state) {
  //orgright_debug_msg('membership','Fn: membership_export_members_form_build');
  // Build the forms required for the membership members export
  // Find out which step we are on. At beginning if $form_state is not set.
  $step = isset($form_state['values']) ? (int)$form_state['storage']['step'] : 0;
  ++$step;
  // Save this step for next time
  $form_state['storage']['step'] = $step;
  $any = t('--any--');
  drupal_set_title(t('Membership - Export Member Details'));
  if ($step == 1) {
    $form = array();
    $fix = 0;
    $fix++;
    $form['instructions'] = array(
      '#title' => t('Export Member Details'),
      '#type' => 'item',
      '#description' => t('Member details will be exported from the membership database to CSV file.  The first line in this file will contain field names corresponding to the orgRight member details fields.'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $category_list = array($any=>$any) + membership_get_categories();
    $form['category'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#description' => membership_field_description('member-export', 'category', t('Select member category (if required) to be exported')),
      '#default_value' => $any,
      '#options' => $category_list,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $standing_list = array($any=>$any) + membership_get_standings();
    $form['standing'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#description' => membership_field_description('member-export', 'standing', t('Select member status (if required) to be exported')),
      '#default_value' => $any,
      '#options' => $standing_list,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    // Create a fieldset for selection of fields
    $fix++;
    $form['fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Membership Fields'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => $fix,
    );
    $field_list = membership_get_member_fields();
    $form['fields']['field_list'] = array(
      '#type' => 'checkboxes',
      //'#title' => t('Membership Fields'),
      '#description' => membership_field_description('member-export', 'fields', t('Select membership field values to export')),
      '#options' => $field_list,
      '#default_value' => $field_list,
      '#weight' => 1,
    );
    $form['fields']['newlines'] = array(
      '#type' => 'radios',
      '#title' => 'Multi-line fields',
      '#description' => membership_field_description('member-export', 'newlines', t('Specify how to handle new-lines in the data')),
      '#options' => array(t('Leave field with embedded new-lines'), t('Replace new-lines with token: "@tkn"', array("@tkn"=>orgright_get_newline_token()))),
      '#default_value' => 1,	// Replace with token
      '#weight' => 2,
    );
    $fix++;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    // Create placeholder field
    $form['afid'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
    $form['#attributes']['enctype'] = 'multipart/form-data';
    $form['#validate'] = array('membership_form_validate');
    $form['#submit'] = array('membership_form_submit');
  }
  else {
    // Find a list of members and create form elements for each
    $members = array();
    $sql = "SELECT m.nid, m.member, m.address, m.city FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid";
    if ($form_state['values']['category'] != $any) { $sql .= " AND m.category = '%s'"; }
    if ($form_state['values']['standing'] != $any) { $sql .= " AND m.standing = '%s'"; }
    $sql .= " ORDER BY m.member";
    $result = db_query($sql, $form_state['values']['category'], $form_state['values']['standing']);
    while ($mbr = db_fetch_object($result)) {
      // Generate an array entry for this member
      $members[$mbr->nid] = $mbr->member . ', ' . $mbr->address . ' ' . $mbr->city;
    }
    // Now generate the form defintion
    $form = array();
    $fix = 0;
    $fix++;
    $form['instructions'] = array(
      '#title' => t('Member Export - Category "@cat", Status "@sts"', array('@cat'=>$form_state['values']['category'], '@sts'=>$form_state['values']['standing'])),
      '#type' => 'item',
      '#description' => t('Select the members from the list below to export'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $form['select'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Select Members',
      '#options' => $members,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Export member details'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $form['#validate'] = array('membership_form_validate');
    $form['#submit'] = array('membership_form_submit');
    $form['#goto'] = 'membership/members';
  }
  // Save step number in form
  $form['step'] = array(
    '#type' => 'value',
    '#value' => $step,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership', t('Members')=>'membership/members'));
  return $form;
}

function membership_subscription_wizard_build (&$form_state) {
  //orgright_debug_msg('membership','Fn: membership_subscription_wizard_build');
  // Build the forms required for the membership subscription wizard
  // Find out which step we are on. At beginning if $form_state is not set.
  $step = isset($form_state['values']) ? (int)$form_state['storage']['step'] : 0;
  ++$step;
  // Save this step for next time
  $form_state['storage']['step'] = $step;
  // Set the title for this page
  drupal_set_title(t('Membership Subscription Wizard'));
  if ($step == 1) {
    // Build a dummy $node
    $node = new stdClass;
    $node->member = '';
    $node->scode = '';
    $node->datefrom = '';
    $node->dateto = '';
    $node->amount = '';
    $node->notes = '';
    // Initiate the first page of the subscription form
    $form = drupal_retrieve_form('membership_subscription_form', $form_state, $node);
    // Now unset the "member" field, add some instructions, and change submit button
    unset($form['mnid']);
    unset($form['member']);
    $fix = $form['fix']['#value'];
    $form['instructions'] = array(
      '#title' => t('Subscription Wizard'),
      '#type' => 'item',
      '#description' => t('Enter the required subscription parameters on this page. On subsequent pages, you can select the members to receive this subscription notice.'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $category_list = membership_get_categories();
    $form['category'] = array(
      '#title' => t('Category'),
      '#type' => 'select',
      '#description' => membership_field_description('subscription-details', 'category', t('Select membership category')),
      '#options' => $category_list,
      '#multiple' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#validate' => array('membership_subscription_wizard_validate'),
      '#submit' => array('membership_subscription_wizard_submit'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  else {
    // Find a list of members and create form elements for each
    $members = array();
    $sql = "SELECT m.nid, m.member, m.address, m.city FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.category = '%s' ORDER BY m.member";
    $result = db_query($sql, $form_state['values']['category']);
    while ($mbr = db_fetch_object($result)) {
      // Generate an array entry for this member
      $members[$mbr->member] = $mbr->member . ', ' . $mbr->address . ' ' . $mbr->city;
    }
    // Now generate the form defintion
    $form = array();
    $fix = 0;
    $fix++;
    $form['instructions'] = array(
      '#title' => t('Subscription Wizard - Category "@cat"', array('@cat'=>$form_state['values']['category'])),
      '#type' => 'item',
      '#description' => t('Select the members from the list below to receive this subscription notice.'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $form['select'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Select Members',
      '#options' => $members,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $fix++;
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Generate subscriptions'),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $form['#validate'] = array('membership_subscription_wizard_validate');
    $form['#submit'] = array('membership_subscription_wizard_submit');
    $form['#goto'] = '/membership/subscriptions';
  }
  // Save step number in form
  $form['step'] = array(
    '#type' => 'value',
    '#value' => $step,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership')=>'membership', t('Subscriptions')=>'membership/subscriptions'));
  return $form;
}

/**
 *  Function to generate a field description with popup help for each form field
 */
function membership_field_description($helpfile, $field, $description) {
  return theme('helpservice_topic','membership',$helpfile.'/'.$field).$description;
}

/**
 * Implementation of hook_form_alter()
 */
function membership_form_member_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_form_member_node_form_alter');
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
        $sql = "SELECT member FROM {node} n JOIN {org_members} m on m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $parent = db_result(db_query($sql, $parvalue));
        $form['parent']['#default_value'] = $parent;
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
    $form['buttons']['submit']['#value'] = 'Add Member';
    $form['#goto'] = 'membership/members/add';
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

function membership_form_subscription_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_form_subscription_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
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
    $form['buttons']['submit']['#value'] = 'Add Subscription';
    $form['#goto'] = 'membership/subscriptions/add';
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

function membership_form_subpayment_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_form_subpayment_node_form_alter');
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'member':
        // fill in the member field information
        $sql = "SELECT member FROM {node} n JOIN {org_members} m on m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $member = db_result(db_query($sql, $parvalue));
        $form['member']['#default_value'] = $member;
        break;
      case 'subscription':
        // fill in the subscription field information
        $sql = "SELECT scode FROM {node} n JOIN {org_subscriptions} s on s.nid = n.nid WHERE s.vid = n.vid AND s.nid = %d";
        $scode = db_result(db_query($sql, $parvalue));
        $form['scode']['#default_value'] = $scode;
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
    $form['buttons']['submit']['#value'] = 'Add Payment';
    $form['#goto'] = 'membership/subpayments/add';
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

function membership_form_membership_subscription_wizard_alter(&$form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_form_membership_subscription_wizard_alter');
  $step = $form_state['storage']['step'];
  if ($step == 1) {
  }
  else {
    // set the form redirect value
    $form['#goto'] = '/membership/subscriptions';
  }
}

/**
 * Form validate routines, called from membership_form_validate() in membership.module
 */
function membership_member_form_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_member_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value'] || $form_state['#import']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $member = strip_tags($form_state['values']['member']);
    $address = strip_tags($form_state['values']['address']);
    $city = strip_tags($form_state['values']['city']);
    $postcode = strip_tags($form_state['values']['postcode']);
    $phone = strip_tags($form_state['values']['phone']);
    $email = strip_tags($form_state['values']['email']);
    $notes = strip_tags($form_state['values']['notes']);
    $parent = strip_tags($form_state['values']['parent']);
    // Check that entered details are acceptable
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($address != $form_state['values']['address']) {
        form_set_error('address',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($city != $form_state['values']['city']) {
        form_set_error('city',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($postcode != $form_state['values']['postcode']) {
        form_set_error('postcode',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($phone != $form_state['values']['phone']) {
        form_set_error('phone',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($email != $form_state['values']['email']) {
        form_set_error('email',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($parent != $form_state['values']['parent']) {
        form_set_error('parent',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that any name returned in "parent" is a valid member and is different from this member
    if ($parent) {
      $pnid = membership_validate_member($parent);
      if ($pnid) {
        form_set_value($form['pnid'], $pnid, $form_state);
      }
      else {
        form_set_error('parent',t('The parent member specified is not a known member'));
      }
      if ($pnid == $form_state['values']['nid']) {
        form_set_error('parent',t('The parent member specified is the same as this member'));
      }
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.member = '%s'";
    $nid = db_result(db_query($sql, $member));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('member',t('The member name is already in use'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Member: !mbr', array('!mbr'=>$member));
 //orgright_debug_dump('Fn: membership_member_form_validate - $form_state',$form_state);
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached children - delete not permitted in this case
    $sql = "SELECT COUNT(m.nid) from {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.pnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('member',t('This member still has @num children attached!', array('@num'=>$count)));
    }
    // Check for attached subscriptions - delete not permitted in this case
    $sql = "SELECT COUNT(s.nid) from {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.mnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('member',t('This member still has @num subscriptions attached!', array('@num'=>$count)));
    }
    // Check for attached payments - delete not permitted in this case
    $sql = "SELECT COUNT(p.nid) from {node} n JOIN {org_subpayments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.mnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('member',t('This subscription still has @num payments attached!', array('@num'=>$count)));
    }
    // Also check for committee appointments and role assignments if the committee module is enabled
    if (module_exists('committee')) {
      // Check for appointments - delete not permitted in this case
      $sql = "SELECT COUNT(p.nid) from {node} n JOIN {org_appointments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.mnid = %d";
      $count = db_result(db_query($sql, $form_state['values']['nid']));
      if ($count) {
        form_set_error('member',t('This member still has @num committee appointments!', array('@num'=>$count)));
      }
      // Check for assignments - delete not permitted in this case
      $sql = "SELECT COUNT(s.nid) from {node} n JOIN {org_assignments} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.rnid = %d";
      $count = db_result(db_query($sql, $form_state['values']['nid']));
      if ($count) {
        form_set_error('member',t('This member still has @num role assigments!', array('@num'=>$count)));
      }
    }
    // Check for file cabinet documents if the filecabinet module is enabled
    if (module_exists('filecabinet')) {
      // Check for document - delete not permitted in this case
      $sql = "SELECT COUNT(d.nid) from {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.mnid = %d";
      $count = db_result(db_query($sql, $form_state['values']['nid']));
      if ($count) {
        form_set_error('member',t('This member still has @num documents in the file cabinet!', array('@num'=>$count)));
      }
    }
  }
}

function membership_subscription_form_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_subscription_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value'] || $form_state['#wizard']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $member = strip_tags($form_state['values']['member']);
    $scode = strip_tags($form_state['values']['scode']);
    $amount = strip_tags($form_state['values']['amount']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($scode != $form_state['values']['scode']) {
        form_set_error('scode',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($amount != $form_state['values']['amount']) {
        form_set_error('amount',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "member" is valid
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The name specified is not a known member'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.scode = '%s' AND s.mnid = %d";
    $nid = db_result(db_query($sql, $scode, $mnid));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('scode',t('Subscription already exists for this member'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Subscription: !mbr - !sub', array('!mbr'=>$member, '!sub'=>$scode));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached payments - delete not permitted in this case
    $sql = "SELECT COUNT(p.nid) from {node} n JOIN {org_subpayments} p ON p.nid = n.nid WHERE p.vid = n.vid AND p.snid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('scode',t('This subscription still has @num payments attached!', array('@num'=>$count)));
    }
  }
}

function membership_subpayment_form_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_subpayment_form_validate');
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $member = strip_tags($form_state['values']['member']);
    $scode = strip_tags($form_state['values']['scode']);
    $payment = strip_tags($form_state['values']['payment']);
    $notes = strip_tags($form_state['values']['notes']);
  //$pdate = date_format_date(date_make_date($form_state['values']['datepaid']), 'custom', 'd M Y');
    // Check that entered details are acceptable
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($scode != $form_state['values']['scode']) {
        form_set_error('scode',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($payment != $form_state['values']['payment']) {
        form_set_error('payment',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "member" is valid
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The name specified is not a known member'));
    }
    // Check that the code returned in "scode" is valid for this member (already have mnid)
    // Query the database to ensure that the subscription code is valid, and to find the snid
    $sql = "SELECT s.nid FROM {node} n JOIN {org_subscriptions} s ON s.nid = n.nid WHERE s.vid = n.vid AND s.mnid = %d AND s.scode = '%s'";
    $args = array($mnid, $scode);
    $snid = db_result(db_query($sql, $args));
    if ($snid) {
      form_set_value($form['snid'], $snid, $form_state);
    }
    else {
      form_set_error('scode',t('The subscription specified is not valid for this member'));
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Subscription payment: !mbr - !date', array('!mbr'=>$member, '!date'=>$pdate));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //orgright_debug_msg('membership','--- Delete button clicked ---');
  }
}

function membership_import_members_form_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_import_members_form_validate');
  // Check upload file
  if (isset($_FILES['files']['name']['import'])) {
    if ($_FILES['files']['error']['import']) {
      form_set_error('import',t('File upload error: @err', array('@err'=>$_FILES['files']['error']['import'])));
    }
    elseif ($file = membership_validate_import_file($form, $form_state)) {
      form_set_value($form['afid'], $file->fid, $form_state);
      $form_state['storage']['file'] = (array)$file;
    }
    else {
      form_set_error('import',t('Error saving import CSV file'));
    }
  }
  else {
    form_set_error('import',t('Must specify an import CSV file'));
  }
  // If we have a valid file, then check contents
  if (isset($form_state['storage']['file'])) {
    $import_file = $form_state['storage']['file']['filepath'];
    $import_lines = file($import_file, FILE_IGNORE_NEW_LINES|FILE_SKIP_EMPTY_LINES);
    if ($import_lines) {
      // process the first array element (line) to find the field names
      $import_fields = membership_import_csv_line($import_lines[0], TRUE);
      $num_fields = count($import_fields);
      // and remove from the begining of the array
      array_shift($import_lines);
      // check the field names
      if (!isset($import_fields[t('name')])) {
        form_set_error('import',t('Import CSV file must include a "name" field'));
      }
      // check for any custom fields
      $custom_fields = array();
      if (variable_get('orgright_custom_fields_member',0)) {
        // are allowed, so check to see whether any are defined
        $sql = "SELECT field_name, label FROM {content_node_field_instance} WHERE type_name = '%s' ORDER BY weight";
        $result = db_query($sql, 'member');
        while ($field = db_fetch_object($result)) {
          $custom_fields[strtolower($field->label)] = $field->field_name;
        }
      }
      // Process the remaining lines
      $import_count = 0;
      foreach ($import_lines as $this_line) {
        ++$import_count;
        // trim line and skip if empty
        $this_line = trim($this_line);
        if (strlen($this_line) < 1) { continue; }
        $this_member = membership_import_csv_line($this_line);
        // check that there are the correct number of fields
        if (count($this_member) != $num_fields) {
          drupal_set_message(t('Member record has wrong number of fields - record @num ignored', array('@num'=>$import_count)), 'warning');
          continue;
        }
        // Define an empty member details array
        $member_details = array(
          'member' => '',
          'address' => '',
          'city' => '',
          'postcode' => '',
          'phone' => '',
          'email' => '',
          'category' => '',
          'standing' => '',
          'joindate' => array('day'=>'', 'month'=>'', 'year'=>''),
          'date joined' => '',
          'leavedate' => array('day'=>'', 'month'=>'', 'year'=>''),
          'date left' => '',
          'parent' => '',
          'notes' => '',
          'pnid' => 0,
        );
        // add any custom fields to this
        foreach ($custom_fields as $field) {
          $member_details[$field] = '';
        }
        // now validate imported membership details and
        // translate field names to database column names
        $unknown_fields = '';
        foreach ($import_fields as $field=>$index) {
          switch ($field) {
            case t('name'): $column = 'member'; break;
            case t('address'): $column = 'address'; break;
            case t('city'): $column = 'city'; break;
            case t('postcode'): $column = 'postcode'; break;
            case t('phone'): $column = 'phone'; break;
            case t('email'): $column = 'email'; break;
            case t('category'): $column = 'category'; break;
            case t('status'): $column = 'standing'; break;
            case t('date joined'): $column = 'date joined'; break;
            case t('date left'): $column = 'date left'; break;
            case t('primary member'): $column = 'parent'; break;
            case t('notes'): $column = 'notes'; break;
            default: $column = 'unknown'; break;
          }
          if ($column == 'unknown') {
            if (isset($custom_fields[$field])) {
              $column = $custom_fields[$field];
              $member_details[$column] = $this_member[$index];
            }
            else {
              $unknown_fields .= "\r\n" . $field . ':' . $this_member[$index];
            }
          }
          else {
            $member_details[$column] = $this_member[$index];
          }
        }
        $member_details['notes'] .= $unknown_fields;
        $ignore = FALSE;
        // Check that a name is specified and not already in the database
        if ($member_details['member']) {
          $mnid = membership_validate_member($member_details['member']);
          if ($mnid) {
            drupal_set_message(t('Member @name already exists in database - record @num ignored', array('@name'=>$member_details['member'], '@num'=>$import_count)), 'warning');
            $ignore = TRUE;
          }
        }
        else {
          drupal_set_message(t('Name not specified - record @num ignored', array('@num'=>$import_count)), 'warning');
          $ignore = TRUE;
        }
        // If a parent member specified, then check that it is valid
        if ($member_details['parent']) {
          $pnid = membership_validate_member($member_details['parent']);
          if ($pnid) {
            $member_details['pnid'] = $pnid;
          }
          else {
            drupal_set_message(t('Primary member @name not a valid member - record @num ignored', array('@name'=>$member_details['parent'], '@num'=>$import_count)), 'warning');
            $ignore = TRUE;
          }
        }
        // Check member category value
        if ($member_details['category']) {
          $category_list = membership_get_categories();
          if (!isset($category_list[$member_details['category']])) {
            if ($form_state['values']['add_categories']) {
              $category_list[] = $member_details['category'];
              variable_set('membership_categories', array_values($category_list));
              drupal_set_message (t('The membership category "@cat" has been added.', array('@cat'=>$member_details['category'])));
            }
            else {
              drupal_set_message (t('The membership category "@cat" does not exist - record @num ignored.', array('@cat'=>$member_details['category'], '@num'=>$import_count)));
              $ignore = TRUE;
            }
          }
        }
        else {
          $member_details['category'] = $form_state['values']['default_category'];
        }
        // Check member standing value
        if ($member_details['standing']) {
          $standing_list = membership_get_standings();
          if (!isset($standing_list[$member_details['standing']])) {
            if ($form_state['values']['add_standings']) {
              $standing_list[] = $member_details['standing'];
              variable_set('membership_standings', array_values($standing_list));
              drupal_set_message (t('The membership status "@status" has been added.', array('@status'=>$member_details['standing'])));
            }
            else {
              drupal_set_message (t('The membership status "@status" does not exist - record @num ignored.', array('@status'=>$member_details['standing'], '@num'=>$import_count)));
              $ignore = TRUE;
            }
          }
        }
        else {
          $member_details['standing'] = $form_state['values']['default_standing'];
        }
        // Check the join date if one supplied
        if ($member_details['date joined']) {
          // massage joindate to get in a consistent format
          $joindate = str_replace(array('/', '.', ':'), '-', $member_details['date joined']);
          if (strlen($joindate) == 10) {
            // reformat yyyy-mm-dd dates
            if (substr($joindate,4,1) == '-' && substr($joindate,7,1) == '-') {
              $joindate = substr($joindate,8,2) . substr($joindate,4,4) . substr($joindate,0,4);
            }
            if (substr($joindate,2,1) == '-' && substr($joindate,5,1) == '-') {
              $member_details['joindate']['day'] = substr($joindate,0,2);
              if (substr($joindate,0,1) == '0') { $member_details['joindate']['day'] = substr($joindate,1,1); }
              $member_details['joindate']['month'] = substr($joindate,3,2);
              if (substr($joindate,3,1) == '0') { $member_details['joindate']['month'] = substr($joindate,4,1); }
              $member_details['joindate']['year'] = substr($joindate,6,4);
              unset($member_details['date joined']);
            }
            else {
              drupal_set_message (t('The date joined "@date" is in wrong format - record @num ignored.', array('@date'=>$member_details['date joined'], '@num'=>$import_count)));
              $ignore = TRUE;
            }
          }
          else {
            drupal_set_message (t('The date joined "@date" is wrong length - record @num ignored.', array('@date'=>$member_details['date joined'], '@num'=>$import_count)));
            $ignore = TRUE;
          }
        }
        // Check the leave date if one supplied
        if ($member_details['date left']) {
          // massage leavedate to get in a consistent format
          $leavedate = str_replace(array('/', '.', ':'), '-', $member_details['date left']);
          if (strlen($leavedate) == 10) {
            // reformat yyyy-mm-dd dates
            if (substr($leavedate,4,1) == '-' && substr($leavedate,7,1) == '-') {
              $leavedate = substr($leavedate,8,2) . substr($leavedate,4,4) . substr($leavedate,0,4);
            }
            if (substr($leavedate,2,1) == '-' && substr($leavedate,5,1) == '-') {
              $member_details['leavedate']['day'] = substr($leavedate,0,2);
              if (substr($leavedate,0,1) == '0') { $member_details['leavedate']['day'] = substr($leavedate,1,1); }
              $member_details['leavedate']['month'] = substr($leavedate,3,2);
              if (substr($leavedate,3,1) == '0') { $member_details['leavedate']['month'] = substr($leavedate,4,1); }
              $member_details['leavedate']['year'] = substr($leavedate,6,4);
              unset($member_details['date left']);
            }
            else {
              drupal_set_message (t('The date left "@date" is in wrong format - record @num ignored.', array('@date'=>$member_details['date left'], '@num'=>$import_count)));
              $ignore = TRUE;
            }
          }
          else {
            drupal_set_message (t('The date left "@date" is wrong length - record @num ignored.', array('@date'=>$member_details['date left'], '@num'=>$import_count)));
            $ignore = TRUE;
          }
        }
        // Check that the "notes" field has not overflowed
        if ($member_details['notes'] && strlen($member_details['notes']) > 255) {
          $member_details['notes'] = substr($member_details['notes'],0,255);
          drupal_set_message (t('The "notes" field overflowed and has been truncated to 255 chars'));
        }
        // save this member details if there have been no errors
        $member_details['title'] = t('Member: !mbr', array('!mbr'=>$member_details['member']));
        if (!$ignore) { $form_state['storage']['members'][] = $member_details; }
      }
      //orgright_debug_dump('imported members',$form_state);
      // save the array of custom fields
      $form_state['storage']['custom_fields'] = $custom_fields;
    }
    else {
      // The specified file was empty or there is another error
      form_set_error('import',t('Unable to read records from import CSV file'));
    }
  }    
}

function membership_export_members_form_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_import_members_form_validate');
  $step = $form_state['storage']['step'];
  if ($step == 1) {
    // ensure that at least one field is selected
    if (count(array_filter($form_state['values']['field_list'])) < 1) {
      form_set_error('field_list',t('You must select at least one field value to export'));
    }
  }
  else {
    // ensure that at least one member has been selected
    if (count(array_filter($form_state['values']['select'])) < 1) {
      form_set_error('select',t('You must select at least one member to export'));
    }
  }
}

function membership_subscription_wizard_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_subscription_wizard_validate');
  $step = $form_state['storage']['step'];
  if ($step == 1) {
    // Check that entered details are acceptable - use PHP strip_tags function to check
    if ($form_state['values']['scode'] != strip_tags($form_state['values']['scode'])) {
        form_set_error('scode',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($form_state['values']['amount'] != strip_tags($form_state['values']['amount'])) {
        form_set_error('amount',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($form_state['values']['notes'] != strip_tags($form_state['values']['notes'])) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
  }
  else {
  }
}

function membership_validate_member(&$member) {
  // Check that the value passed in "$member" is valid, and return the associated nid
  //orgright_debug_msg('membership',t('Validate member name: @mbr', array('@mbr' => $member)));
  // Query the database to ensure that the member name is valid, and to find the mnid
  if (is_numeric($member)) {
    // then the passed value should be the nid of a member node
    $mnid = $member;
    $sql = "SELECT n.type, m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $obj = db_fetch_object(db_query($sql, $mnid));
    if ($obj->type == 'member') { $member = $obj->member; }
    else { $mnid = 0; }
    return $mnid;
  }
  else {
    $sql = "SELECT m.nid FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.member = '%s'";
    return db_result(db_query($sql, $member));
  }
}

function membership_validate_import_file($form, &$form_state) {
  // Specify some values required for validating the import file
  $extensions = 'csv';
  $max_file_size = 1024*1024;
  $validators = array(
    'file_validate_extensions' => array($extensions),
    'file_validate_size' => array($max_file_size),
  );
  // save the file in the import/member sub-directory of the filecabinet
  // check that the base level directory exists, create if not
  $subpath = 'filecabinet/import';
  $filepath = file_create_path($subpath);
  file_check_directory($filepath, FILE_CREATE_DIRECTORY, 'import');
  // check that the 2nd level exists, create if not
  $subpath .= '/member';
  $filepath = file_create_path($subpath);
  file_check_directory($filepath, FILE_CREATE_DIRECTORY, 'import');
  // save the uploaded file and return the file object
  $file = file_save_upload('import', $validators, $filepath, FILE_EXISTS_RENAME);
  return $file;
}

function membership_import_csv_line($csv_line, $invert = FALSE) {
  // This helper function will take a CSV line and return an array of clean values
  // If invert is TRUE, the return array will have keys and values swapped
  $csv_fields = array();
  $fieldno = 0;
  $currfield = '';
  for ($i = 0; $i < strlen($csv_line); $i++) {
    if ($csv_line{$i} == '"') {
      if ($insidequotes) $insidequotes = false;
      else $insidequotes = true;
    }
    elseif ($csv_line{$i} == ',') {
      if ($insidequotes) {
        $currfield .= $csv_line{$i};
      }
      else {
        $csv_fields[$fieldno++] = $currfield;
        $currfield = '';
      }
    }
    else {
      $currfield .= $csv_line{$i};
    }
  }
  $csv_fields[$fieldno++] = $currfield;

  $csv_array = array();
  foreach ($csv_fields as $key=>$field) {
    $field = strip_tags(trim($field));
    if ($invert) {
      $csv_array[strtolower($field)] = $key;
    }
    else {
      $csv_array[$key] = $field;
    }
  }
  return $csv_array;
}

/**
 * Form submit routines, called from membership_form_submit() in membership.module
 */
function membership_member_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('membership','Fn: membership_member_form_submit');
  // Set up some fields if the form has been submitted programmatically, becuase
  // a Drupal bug means that the form does not get validated after the first time
  if ($form_state['#import'] > 1) {
    // Set the title
    $member = strip_tags($form_state['values']['member']);
    $form_state['values']['title'] = t('Member: !mbr', array('!mbr'=>$member));
    // Recover the joining and leaving dates
    $form_state['values']['joindate'] = $form_state['storage']['joindate'];
    $form_state['values']['leavedate'] = $form_state['storage']['leavedate'];
    $form_state['redirect'] = 'membership/members';
  }
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['storage'] = array();
  $form_state['redirect'] = $form['#goto'];
}

function membership_subscription_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('membership','Fn: membership_subscription_form_submit');
  // Set up some fields if the form has been submitted programmatically, becuase
  // a Drupal bug means that the form does not get validated after the first time
  if ($form_state['#wizard'] > 1) {
    // Set the title
    $form_state['values']['title'] = 'Subscription: '.$form_state['values']['member'].' - '.$form_state['values']['scode'];
    // Massage the dates
    $form_state['values']['datefrom'] = $form_state['storage']['datefrom_string'];
    $form_state['values']['dateto'] = $form_state['storage']['dateto_string'];
    // Add the mnid value
    $form_state['values']['mnid'] = membership_validate_member($form_state['values']['member']);
  }
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['storage'] = array();
  $form_state['redirect'] = $form['#goto'];
}

function membership_subpayment_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('membership','Fn: membership_subpayment_form_submit');
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['storage'] = array();
  $form_state['redirect'] = $form['#goto'];
}

function membership_import_members_form_submit($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_import_members_form_submit');
  if (isset($form_state['storage']['members'])) {
    // There are member details to be loaded to the database
    // specify the form to be used to load each member record
    $form_id = 'member_node_form';
    module_load_include('inc', 'node', 'node.pages');
    // Process the member details
    $import_count = 0;
    foreach ($form_state['storage']['members'] as $this_member) {
      // replace any of the special line delimiters with newlines in specified fields
      $multi_fields = array('address','phone','email','notes');
      foreach ($multi_fields as $field) {
        $this_member[$field] = orgright_string_to_multi_field($this_member[$field]);
      }
      // prepare membership details and submit a form
      // create a new node for this member
      $this_node = node_prepare((object)array('type' => 'member'));
      // prepare a copy of $form_state
      $this_form_state = $form_state;
      unset($this_form_state['values']);
      unset($this_form_state['storage']);
      $this_form_state['storage'] = array();
      // populate $form_state with the values for this member
      $this_form_state['values'] = $this_member;
      // change any custom fields
      if (variable_get('orgright_custom_fields_member',0)) {
        $custom_fields = $form_state['storage']['custom_fields'];
        foreach ($custom_fields as $field) {
          if (isset($this_form_state['values'][$field])) {
            $value = $this_form_state['values'][$field];
          }
          else { $value = NULL; }
          $this_form_state['values'][$field] = array(
            array('value' => orgright_string_to_multi_field($value))
          );
        }
      }
      // Save the date fields for later use
      $this_form_state['storage']['joindate'] = $this_member['joindate']['year'].'-'.$this_member['joindate']['month'].'-'.$this_member['joindate']['day'];
      $this_form_state['storage']['leavedate'] = $this_member['leavedate']['year'].'-'.$this_member['leavedate']['month'].'-'.$this_member['leavedate']['day'];
      // Build a composite node title and save in $form_state
      //$this_form_state['values']['title'] = t('Member: !mbr', array('!mbr'=>$this_member['member']));
      $this_form_state['#validate'] = array('membership_form_validate');
      $this_form_state['#submit'] = array('membership_form_submit');
      // following is required to get node form submit to work correctly
      $this_form_state['submit_handlers'] = array('node_form_submit');
      // flag that this member is being submitted programmatically
      $this_form_state['#import'] = ++$import_count;
      // now submit the form
      drupal_execute($form_id, $this_form_state, $this_node);
    }
  }
  else {
    // no member details to import
  }
  // Done, so clear storage and set redirect
  $form_state['storage'] = array();
  $form_state['redirect'] = 'membership/members';
}

function membership_export_members_form_submit($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_import_members_form_submit');
  $step = $form_state['storage']['step'];
  if ($step == 1) {
    // Step 1 - save the category and standing values
    $form_state['storage']['category'] = $form_state['values']['category'];
    $form_state['storage']['standing'] = $form_state['values']['standing'];
    // and the list of fields to export
    $form_state['storage']['field_list'] = $form_state['values']['field_list'];
    $form_state['storage']['newlines'] = $form_state['values']['newlines'];
    // just return to generate the next step of the wizard
    return;
  }
  else {
    // Step 2 - generate the export file and download
    // generate an array of selected members
    $selected = array_filter($form_state['values']['select']);
    // create a temporary file to hold the export
    $file_name = 'member-export-'.orgright_get_date_ymd().'.csv';
    $file_temp = file_directory_temp() . '/' . $file_name;
    $csv_file = fopen($file_temp, 'wb');
    $field_list = array_filter($form_state['storage']['field_list']);
    $mbr_fields = membership_get_member_fields();
    // specify the CSV field header line using selected fields
    $csv_fields = '';
    foreach ($field_list as $field_name) {
      $csv_fields .= '"' . $mbr_fields[$field_name] . '",';
    }
    $csv_fields = substr($csv_fields, 0, -1) . "\r\n";
    // and write header record to the temporary file
    fwrite($csv_file, $csv_fields);
    // check for any member custom fields
    if (variable_get('orgright_custom_fields_member',0)) {
      // are allowed, so check to see whether any are defined
      $sql = "SELECT field_name, label, widget_type FROM {content_node_field_instance} WHERE type_name = '%s' ORDER BY weight";
      $custom_fields = array();
      $result = db_query($sql, 'member');
      while ($field = db_fetch_object($result)) {
        $custom_fields[$field->field_name] = $field->label;
      }
    }
    $export_count = 0;
    $sql = "SELECT * FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    foreach ($selected as $mnid) {
      $member = db_fetch_object(db_query($sql, $mnid));
      $mbr_details = '';
      if (isset($field_list['member'])) { $mbr_details .= '"' . $member->member . '",'; }
      if (isset($field_list['address'])) { $mbr_details .= '"' . $member->address . '",'; }
      if (isset($field_list['city'])) { $mbr_details .= '"' . $member->city . '",'; }
      if (isset($field_list['postcode'])) { $mbr_details .= '"' . $member->postcode . '",'; }
      if (isset($field_list['phone'])) { $mbr_details .= '"' . $member->phone . '",'; }
      if (isset($field_list['email'])) { $mbr_details .= '"' . $member->email . '",'; }
      if (isset($field_list['category'])) { $mbr_details .= '"' . $member->category . '",'; }
      if (isset($field_list['standing'])) { $mbr_details .= '"' . $member->standing . '",'; }
      if (isset($field_list['joindate'])) { $mbr_details .= '"' . date_format_date(date_make_date($member->joindate), 'custom', 'd M Y') . '",'; }
      if (isset($field_list['leavedate'])) { $mbr_details .= '"' . date_format_date(date_make_date($member->leavedate), 'custom', 'd M Y') . '",'; }
      if (isset($field_list['parent'])) { $mbr_details .= '"' . $member->parent . '",'; }
      if (isset($field_list['notes'])) { $mbr_details .= '"' . $member->notes . '",'; }
      // and add any custom fields if required
      if ($custom_fields) {
        $sql2 = "SELECT * FROM {content_type_member} WHERE vid = %d";
        $result = db_query($sql2, $member->vid);
        while ($field_values = db_fetch_array($result)) {
          foreach ($custom_fields as $name=>$label) {
            if (isset($field_list[$name])) { $mbr_details = '"' . $field_values[$name.'_value'] . '",'; }
          }
        }
      }
      // now convert newlines if required and add a new-line at the end of the record
      if ($form_state['storage']['newlines']) { $mbr_details = orgright_multi_field_to_string($mbr_details); }
      $mbr_details = substr($mbr_details, 0, -1) . "\r\n";
      fwrite($csv_file, $mbr_details);
    }
    // now close temporary file and download to user
    fclose($csv_file);
    file_transfer($file_temp, array(
      'Content-Type: octet/stream',
      'Content-Disposition: attachment; filename="'.$file_name.'";',
      'Content-Length: '.filesize($file_temp),
      ));
    // clear the form_state storage area so that the redirect will work and set redirect
    $form_state['storage'] = array();
    $form_state['redirect'] = $form['#goto'];
  }
}

function membership_subscription_wizard_submit ($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('membership','Fn: membership_subscription_wizard_submit');
  // Check the step that thw wizard has got to
  $step = $form_state['storage']['step'];
  if ($step == 1) {
    // Step 1 - save the subscription details
    $form_state['storage']['scode'] = $form_state['values']['scode'];
    $form_state['storage']['datefrom_string'] = $form_state['values']['datefrom'];
    $form_state['storage']['datefrom_array'] = array(
      'year' => (int)substr($form_state['values']['datefrom'], 0, 4),
      'month' => (int)substr($form_state['values']['datefrom'], 5, 2),
      'day' => (int)substr($form_state['values']['datefrom'], 8, 2),
    );
    $form_state['storage']['dateto_string'] = $form_state['values']['dateto'];
    $form_state['storage']['dateto_array'] = array(
      'year' => (int)substr($form_state['values']['dateto'], 0, 4),
      'month' => (int)substr($form_state['values']['dateto'], 5, 2),
      'day' => (int)substr($form_state['values']['dateto'], 8, 2),
    );
    $form_state['storage']['amount'] = $form_state['values']['amount'];
    $form_state['storage']['notes'] = $form_state['values']['notes'];
    // just return to generate the next step of the wizard
    return;
  }
  else {
    // Step 2 - generate subscription records for each selected member
    // specify the form to be used to load each subscription
    $form_id = 'subscription_node_form';
    module_load_include('inc', 'node', 'node.pages');
    // generate an array of selected members
    $mbr_array = $form_state['values']['select'];
    $selected = array_filter($mbr_array);
    $wizard_count = 0;
    foreach ($selected as $member) {
      // create a new node for this subscription
      $this_node = node_prepare((object)array('type' => 'subscription'));
      // prepare a copy of $form_state
      $this_form_state = $form_state;
      unset($this_form_state['values']);
      // populate $form_sate with the values for this subscription
      $this_form_state['values'] = array(
        'member' => $member,
        'scode' => $form_state['storage']['scode'],
        'datefrom' => $form_state['storage']['datefrom_array'],
        'dateto' => $form_state['storage']['dateto_array'],
        'amount' => $form_state['storage']['amount'],
        'notes' => $form_state['storage']['notes'],
      );
      $this_form_state['#validate'] = array('membership_form_validate');
      $this_form_state['#submit'] = array('membership_form_submit');
      // following is required to get node form submit to work correctly
      $this_form_state['submit_handlers'] = array('node_form_submit');
      // flag that this subscription is being submitted programmatically
      $this_form_state['#wizard'] = ++$wizard_count;
      // now submit the form
      //orgright_debug_msg('membership','add subscription for '.$member,6);
      drupal_execute($form_id, $this_form_state, $this_node);
      //orgright_debug_msg('membership','add subscription done ',6);
    }
    // Done, so clear form_state storage and set redirect
    $form_state['storage'] = array();
    $form_state['redirect'] = $form['#goto'];
  }
}
