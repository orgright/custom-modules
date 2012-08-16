<?php
// $Id$

/**
 * @file
 * Provides additional functions for filecabinet module.
 */

/**
 * Functions called from hook_form() in filecabinet.module
 */
function filecabinet_drawer_form_build(&$form_state, $node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_drawer_form_build',5);
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating drawers
  $form = array();
  $fix = 0;
  $fix++;
  $form['drawer'] = array(
    '#type' => 'textfield',
    '#title' => t('Drawer name'),
    '#description' => filecabinet_field_description('drawer', 'drawer', t('Enter a reference for this filing cabinet drawer')),
    '#default_value' => $node->drawer,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $users_list = orgright_get_users('nid');
  if (count($users_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['owner'] = array(
      '#type' => 'textfield',
      '#title' => t('Owner'),
      '#description' => filecabinet_field_description('drawer', 'owner', t('Enter the name of the user owning this drawer')),
      '#default_value' => $node->owner,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/owners',
    );
  }
  else {
    $form['owner'] = array(
      '#type' => 'select',
      '#title' => t('Owner'),
      '#description' => filecabinet_field_description('drawer', 'owner', t('Enter the name of the user owning this drawer')),
      '#default_value' => $node->ouid,
      '#options' => array('-- select owner --') + $users_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => filecabinet_field_description('drawer', 'body', t('Describe the purpose of this filing cabinet drawer')),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#required' => FALSE,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $drawer_list = filecabinet_get_drawers('nid');
  if (count($drawer_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['parent'] = array(
      '#type' => 'textfield',
      '#title' => t('Parent drawer'),
      '#description' => filecabinet_field_description('drawer', 'parent', t('Enter the name of a filing cabinet drawer to contain this drawer')),
      '#default_value' => $node->parent,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/drawers',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['parent'] = array(
      '#type' => 'select',
      '#title' => t('Parent drawer'),
      '#description' => filecabinet_field_description('drawer', 'parent', t('Enter the name of a filing cabinet drawer to contain this drawer')),
      '#default_value' => $node->wnid,
      '#options' => array('[none]') + $drawer_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $status_list = filecabinet_get_drawer_statuses();
  $form['drawerstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => filecabinet_field_description('drawer', 'drawerstatus', t('Select the status of this drawer')),
    '#default_value' => $node->drawerstatus,
    '#options' => $status_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['drawerperm'] = array(
    '#type' => 'textfield',
    '#title' => t('Permissions'),
    '#description' => filecabinet_field_description('drawer', 'drawerperm', t('Specify the permissions that apply to this drawer')),
    '#default_value' => $node->drawerperm,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => filecabinet_field_description('drawer', 'notes', t('General notes about this file cabinet drawer')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['wnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['ouid'] = array(
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
  $form['#validate'] = array('filecabinet_form_validate');
  $form['#submit'] = array('filecabinet_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet')=>'filecabinet', t('Drawers')=>'filecabinet/drawers'));
  return $form;
}

function filecabinet_folder_form_build(&$form_state, $node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_folder_form_build',6);
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating agenda items
  $form = array();
  $fix = 0;
  $fix++;
  $form['folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Folder'),
    '#description' => filecabinet_field_description('folder', 'folder', t('Specify the folder name')),
    '#default_value' => $node->folder,
    '#maxlength' => 255,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $folder_list = filecabinet_get_folders('nid');
  if (count($folder_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['parent'] = array(
      '#type' => 'textfield',
      '#title' => t('Parent folder'),
      '#description' => filecabinet_field_description('folder', 'parent', t('Specify a parent folder name (if any)')),
      '#default_value' => $node->parent,
      '#maxlength' => 255,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/folders',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['parent'] = array(
      '#type' => 'select',
      '#title' => t('Parent folder'),
      '#description' => filecabinet_field_description('folder', 'parent', t('Specify a parent folder name (if any)')),
      '#default_value' => $node->fnid,
      '#options' => array('[none]') + $folder_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $drawer_list = filecabinet_get_drawers('nid');
  if (count($drawer_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['drawer'] = array(
      '#type' => 'textfield',
      '#title' => t('Drawer'),
      '#description' => filecabinet_field_description('folder', 'drawer', t('Specify the drawer that this folder is stored in')),
      '#default_value' => $node->drawer,
      '#maxlength' => 255,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/drawers',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['drawer'] = array(
      '#type' => 'select',
      '#title' => t('Drawer'),
      '#description' => filecabinet_field_description('folder', 'drawer', t('Specify the drawer that this folder is stored in')),
      '#default_value' => $node->wnid,
      '#options' => array('[none]') + $drawer_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $status_list = filecabinet_get_folder_statuses();
  $form['folderstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => filecabinet_field_description('folder', 'folderstatus', t('Select the status of this folder')),
    '#default_value' => $node->folderstatus,
    '#options' => $status_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => filecabinet_field_description('folder', 'body', t('Describe the purpose of this folder')),
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
    '#description' => filecabinet_field_description('folder', 'notes', t('General notes about this folder')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['wnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['fnid'] = array(
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
  $form['#validate'] = array('filecabinet_form_validate');
  $form['#submit'] = array('filecabinet_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet')=>'filecabinet', t('Folders')=>'filecabinet/folders'));
  return $form;
}

function filecabinet_document_form_build(&$form_state, $node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_document_form_build');
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating filecabinet items
  $form = array();
  $fix = 0;
  $fix++;
  $form['docname'] = array(
    '#type' => 'textfield',
    '#title' => t('Document'),
    '#description' => filecabinet_field_description('document', 'docname', t('Specify the document name')),
    '#default_value' => $node->docname,
    '#maxlength' => 255,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $folder_list = filecabinet_get_folders('nid');
  if (count($folder_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['folder'] = array(
      '#type' => 'textfield',
      '#title' => t('Folder'),
      '#description' => filecabinet_field_description('document', 'folder', t('Specify the folder that this document is stored in')),
      '#default_value' => $node->folder,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/folders',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['folder'] = array(
      '#type' => 'select',
      '#title' => t('Folder'),
      '#description' => filecabinet_field_description('document', 'folder', t('Specify the folder that this document is stored in')),
      '#default_value' => $node->fnid,
      '#options' => array('-- Select folder or head document --') + $folder_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $document_list = filecabinet_get_documents('nid');
  if (count($document_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['headoc'] = array(
      '#type' => 'textfield',
      '#title' => t('Head document'),
      '#description' => filecabinet_field_description('document', 'headoc', t('Specify the head document that this one is attached to (if any)')),
      '#default_value' => $node->headoc,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/docnames',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['headoc'] = array(
      '#type' => 'select',
      '#title' => t('Head document'),
      '#description' => filecabinet_field_description('document', 'headoc', t('Specify the head document that this one is attached to (if any)')),
      '#default_value' => $node->dnid,
      '#options' => array('[none]') + $document_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($members_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Member'),
      '#description' => filecabinet_field_description('document', 'member', t('Name of member responsible for this document')),
      '#default_value' => $node->member,
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
    $form['member'] = array(
      '#type' => 'select',
      '#title' => t('Member'),
      '#description' => filecabinet_field_description('document', 'member', t('Name of member responsible for this document')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['docversion'] = array(
    '#type' => 'textfield',
    '#title' => t('Version'),
    '#description' => filecabinet_field_description('document', 'docversion', t('Specify the document version')),
    '#default_value' => $node->docversion,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  // Attachment fieldset
  $form['attachment'] = array(
    '#type' => 'fieldset',
    '#access' => TRUE,
    '#title' => t('Associated file'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    //'#description' => t(''),
    '#prefix' => '<div class="attachments">',
    '#suffix' => '</div>',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // If there is no attached file, then create a file attach field
  if ($node->afid) {
    // File already attached, so generate a detach field
    $fix++;
    $form['attachment']['detach'] = array(
      '#type' => 'checkboxes',
      '#title' => t('File name'),
      '#description' => filecabinet_field_description('document', 'detach', t('Select the checkbox to dis-associate the file from document')),
      '#options' => array('file'=>$node->filename),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  else {
    // File not attached - generate a file attach field
    $fix++;
    $limits = array(
      'file_size' => filecabinet_get_upload_filesize()*1048576,
      'extensions' => filecabinet_get_upload_extensions(),
    );
    $description = ($limits['resolution'] ? t('Images larger than %resolution will be resized. ', array('%resolution' => $limits['resolution'])) : '') . t('The maximum upload size is %filesize. Only files with the following extensions may be uploaded: %extensions. ', array('%filesize' => format_size($limits['file_size']), '%extensions' => $limits['extensions']));
    $form['attachment']['upload'] = array(
      '#type' => 'file',
      '#title' => t('Associate file'),
      '#size' => 64,
      '#description' => filecabinet_field_description('document', 'attach', $description),
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['docdesc'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => filecabinet_field_description('document', 'docdesc', t('Brief description of the associated file')),
    '#default_value' => $node->docdesc,
    '#rows' => 5,
    '#required' => FALSE,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $type_list = filecabinet_get_document_types();
  $form['doctype'] = array(
    '#type' => 'select',
    '#title' => t('Document type'),
    '#description' => filecabinet_field_description('document', 'doctype', t('Select the type of document')),
    '#default_value' => $node->doctype,
    '#options' => $type_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd-m-Y'; // Specify the date format
  $form['docdate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Document date'),
    '#description' => filecabinet_field_description('document', 'docdate', t('Enter the date of the document')),
    '#default_value' => ($node->docdate) ? $node->docdate : orgright_get_date_ymd(),
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $status_list = filecabinet_get_document_statuses();
  $form['docstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => filecabinet_field_description('document', 'docstatus', t('Select the status of this document')),
    '#default_value' => $node->docstatus,
    '#options' => $status_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes'),
    '#description' => filecabinet_field_description('document', 'notes', t('General notes about this document')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['fnid'] = array(
    '#type' => 'value',
    '#value' => $node->fnid,
  );
  $form['dnid'] = array(
    '#type' => 'value',
    '#value' => $node->dnid,
  );
  $form['mnid'] = array(
    '#type' => 'value',
    '#value' => $node->mnid,
  );
  $form['afid'] = array(
    '#type' => 'value',
    '#value' => $node->afid,
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
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['#validate'] = array('filecabinet_form_validate');
  $form['#submit'] = array('filecabinet_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet')=>'filecabinet', t('Documents')=>'filecabinet/documents'));
  return $form;
}

function filecabinet_doclink_form_build(&$form_state) {
  // Generate a form to allow a document to be linked to an information item
  $docname = '';
  $nodetype = '';
  $infoitem = '';
  $linktype = '';
  $dnid = 0;
  $xnid = 0;
  // Specifiy a title for the form
  drupal_set_title(t('Create Document Link'));
  $form = array();
  $fix = 0;
  $fix++;
  $document_list = filecabinet_get_documents('nid');
  if (count($document_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field instead
    $form['docname'] = array(
      '#type' => 'textfield',
      '#title' => t('Document'),
      '#description' => filecabinet_field_description('doclink', 'docname', t('Select the document to be linked to the information item')),
      '#default_value' => $docname,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'filecabinet/autocomplete/docnames',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['docname'] = array(
      '#type' => 'select',
      '#title' => t('Document'),
      '#description' => filecabinet_field_description('doclink', 'docname', t('Select the document to be linked to the information item')),
      '#default_value' => $dnid,
      '#options' => array('-- select document --') + $document_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  /*$fix++;
  $doclink_nodetype_list = filecabinet_get_doclink_nodetypes();
  $form['nodetype'] = array(
    '#type' => 'select',
    '#title' => t('Information type'),
    '#description' => t('Select the type of information to be linked'),
    '#default_value' => $nodetype,
    '#options' => $doclink_nodetype_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );*/
  $fix++;
  $form['infoitem'] = array(
    '#type' => 'textfield',
    '#title' => t('Information item'),
    '#description' => filecabinet_field_description('doclink', 'infoitem', t('Select the information item to be linked to the document')),
    '#default_value' => $infoitem,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'filecabinet/autocomplete/infoitems',
  );
  $fix++;
  $doclink_linktype_list = filecabinet_get_doclink_linktypes();
  $form['linktype'] = array(
    '#type' => 'select',
    '#title' => t('Link type'),
    '#description' => filecabinet_field_description('doclink', 'linktype', t('Select the type of link')),
    '#default_value' => $linktype,
    '#options' => $doclink_linktype_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add link'),
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['dnid'] = array(
    '#type' => 'value',
    '#value' => $dnid,
  );
  $form['xnid'] = array(
    '#type' => 'value',
    '#value' => $xnid,
  );
  $form['nodetype'] = array(
    '#type' => 'value',
    '#value' => $nodetype,
  );
  $form['#validate'] = array('filecabinet_form_validate');
  $form['#submit'] = array('filecabinet_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet')=>'filecabinet', t('Documents')=>'filecabinet/documents'));
  return $form;
}

/**
 *  Function to generate a field description with popup help for each form field
 */
function filecabinet_field_description($type, $field, $description) {
  return theme('helpservice_topic','filecabinet',$type.'-details/'.$field).$description;
}

/**
 * Implementation of hook_form_alter()
 */
function filecabinet_form_drawer_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_form_filecabinet_node_form_alter',6);
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'drawer':
        // fill in the drawer name field information
        if ($form['drawer']['#type'] == 'select') {
          // is a select type field, use nid
          $form['drawer']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT drawer FROM {node} n JOIN {org_drawers} w on w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
          $form['drawer']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'parent':
        // fill in the parent drawer field information
        if ($form['parent']['#type'] == 'select') {
          // is a select type field, use nid
          $form['parent']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT drawer FROM {node} n JOIN {org_drawers} w on w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
          $form['parent']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
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
    $form['buttons']['submit']['#value'] = 'Add Drawer';
    $form['#goto'] = 'filecabinet/drawers/add';
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

function filecabinet_form_folder_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_form_folder_node_form_alter',6);
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'drawer':
        // fill in the drawer field information
        if ($form['drawer']['#type'] == 'select') {
          // is a select type field, use nid
          $form['drawer']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT drawer FROM {node} n JOIN {org_drawers} w on w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
          $form['drawer']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'folder':
        // fill in the parent folder field information
        if ($form['parent']['#type'] == 'select') {
          // is a select type field, use nid
          $form['parent']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT folder FROM {node} n JOIN {org_folders} f on f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
          $form['parent']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
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
    $form['buttons']['submit']['#value'] = 'Add Folder';
    $form['#goto'] = 'filecabinet/folders/add';
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

function filecabinet_form_document_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_form_document_node_form_alter',6);
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'newversion':
        // Create a new version of the specified document
        // First load the existing document node
        $node = node_load($parvalue);
        // and then set the default values
        $form['docname']['#default_value'] = $node->docname;
        $form['folder']['#default_value'] = $node->folder;
        $form['headoc']['#default_value'] = $node->headoc;
        $form['member']['#default_value'] = $node->member;
        $form['docdesc']['#default_value'] = $node->docdesc;
        $form['doctype']['#default_value'] = $node->doctype;
        $form['docstatus']['#default_value'] = $node->docstatus;
        $form['notes']['#default_value'] = $node->notes;
        $form['fnid']['#default_value'] = $node->fnid;
        $form['dnid']['#default_value'] = $node->dnid;
        $form['mnid']['#default_value'] = $node->mnid;
        $form['afid']['#default_value'] = $node->afid;
        $form['original']['#default_value'] = $node->original;
        // calculate a new document version
        $matches = array();
        if (is_numeric($node->docversion)) {
          $docversion = round($node->docversion + 1);
        }
        elseif (preg_match('/(.*)-(\d*)$/', $node->docversion, $matches)) {
          $docversion = $matches[1] . '-' . ($matches[2] + 1);
        }
        else {
          $docversion = $node->docversion . '-1';
        } 
        $form['docversion']['#default_value'] = $docversion;
        // and set the file attachment field set
        if ($node->afid) {
          // File already attached, so generate a detach field
          $form['attachment']['detach'] = array(
            '#type' => 'checkboxes',
            '#title' => t('File name'),
            '#description' => filecabinet_field_description('document', 'detach', t('Select the checkbox to detach file from document')),
            '#options' => array('file'=>$node->filename),
          );
          unset($form['attachment']['upload']);
        }
        break;
      case 'folder':
        // fill in the folder field information
        if ($form['folder']['#type'] == 'select') {
          // is a select type field, use nid
          $form['folder']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT folder FROM {node} n JOIN {org_folders} f on f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
          $form['folder']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'attachment':
        // fill in the head document field information
        if ($form['headoc']['#type'] == 'select') {
          // is a select type field, use nid
          $form['headoc']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT docname FROM {node} n JOIN {org_documents} d on d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
          $form['headoc']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
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
    $form['buttons']['submit']['#value'] = 'Add Document';
    $form['#goto'] = 'filecabinet/documents/add';
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

function filecabinet_form_filecabinet_doclink_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_form_filecabinet_doclink_form_alter',6);
  //orgright_debug_dump('form',$form);
  // Find out whether we are to add or edit a doclink
  $form_state['storage']['action'] = $form['#parameters'][2];
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'document':
        // fill in the document name field information
        if ($form['docname']['#type'] == 'select') {
          // is a select type field, use nid
          $form['docname']['#default_value'] = $parvalue;
          $form_state['storage']['docname'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT docname FROM {node} n JOIN {org_documents} d on d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
          $form['docname']['#default_value'] = db_result(db_query($sql, $parvalue));
          $form_state['storage']['docname'] = $form['docname']['#default_value'];
        }
        $form_state['storage']['dnid'] = $parvalue;
        break;
      case 'node':
        // fill in the info item field information (node title)
        $sql = "SELECT title FROM {node} n WHERE nid = %d";
        $title = db_result(db_query($sql, $parvalue));
        $form['infoitem']['#default_value'] = $title;
        $form_state['storage']['infoitem'] = $title;
        $form_state['storage']['xnid'] = $parvalue;
        break;
      default:
      }
    }
  }
  if ($form_state['storage']['action'] == 'edit') {
    // convert the docname and infoitem fields to markup to prevent changes
    // but fisrt change a "select" field default from dnid to document name
    if ($form['docname']['#type'] == 'select') {
      $sql = "SELECT docname FROM {node} n JOIN {org_documents} d on d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
      $form['docname']['#default_value'] = db_result(db_query($sql, $form['docname']['#default_value']));
      $form_state['storage']['docname'] = $form['docname']['#default_value'];
    }
    $linknid = array(
      'docname' => $form_state['storage']['dnid'],
      'infoitem' => $form_state['storage']['xnid'],
    );
    foreach (array('docname','infoitem') as $field) {
      $link_value = l($form[$field]['#default_value'], 'node/'.$linknid[$field]);
      $form[$field]['#type'] = 'markup';
      $form[$field]['#value'] = $form[$field]['#title'] . ': ' . $link_value;
      $form[$field]['#prefix'] = '<p class="doclink">';
      $form[$field]['#suffix'] = '</p>';
      unset($form[$field]['#default_value']);
      unset($form[$field]['#maxlength']);
      unset($form[$field]['#size']);
      unset($form[$field]['#type']);
      unset($form[$field]['#title']);
      unset($form[$field]['#required']);
      unset($form[$field]['#description']);
      unset($form[$field]['#autocomplete_path']);
    }
    // populate the form with any other details
    $sql = "SELECT * FROM {org_doclinks} WHERE dnid = %d AND xnid = %d";
    $doclink = db_fetch_object(db_query($sql, $form_state['storage']['dnid'], $form_state['storage']['xnid']));
    $form['nodetype']['#value'] = $doclink->nodetype;
    $form['linktype']['#default_value'] = $doclink->linktype;
    $form_state['storage']['linktype'] = $doclink->linktype;
    // Change label on the submit button
    $form['submit']['#value'] = 'Save Changes';
    // also add a "delete" button
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete link'),
      '#weight' => 9,
    );
    // Specifiy a title for the form
    drupal_set_title(t('Edit Document Link'));
  }
}


/**
 * Form validate routines, called from filecabinet_form_validate() in filecabinet.module
 */
function filecabinet_drawer_form_validate($form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_drawer_form_validate',6);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $drawer = strip_tags($form_state['values']['drawer']);
    $parent = strip_tags($form_state['values']['parent']);
    $owner = strip_tags($form_state['values']['owner']);
    $drawerperm = strip_tags($form_state['values']['drawerperm']);
    $notes = strip_tags($form_state['values']['notes']);

    // Check that entered details are acceptable - use PHP strip_tags function to check
    if ($drawer != $form_state['values']['drawer']) {
        form_set_error('drawer',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($parent != $form_state['values']['parent']) {
        form_set_error('parent',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($owner != $form_state['values']['owner']) {
        form_set_error('owner',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($drawerperm != $form_state['values']['drawerperm']) {
        form_set_error('drawerperm',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that the name returned in "parent" is valid
    if ($parent) {
      $wnid = filecabinet_validate_drawer($form_state['values']['parent']);
      if ($wnid) {
        form_set_value($form['wnid'], $wnid, $form_state);
      }
      else {
        form_set_error('parent',t('The name specified is not a known filing cabinet drawer'));
      }
    }
    else {
      $wnid = 0;
      form_set_value($form['wnid'], $wnid, $form_state);
    }
    // Check that the name returned in "owner" is valid
    //module_load_include('inc', 'membership', 'membership_form'); 
    $ouid = filecabinet_validate_owner($owner);
    if ($ouid) {
      form_set_value($form['ouid'], $ouid, $form_state);
    }
    else {
      form_set_error('owner',t('The name specified is not a known user'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.drawer = '%s'";
    $nid = db_result(db_query($sql, $drawer));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('drawer',t('The drawer name is already in use'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('File Cabinet Drawer: !drwr', array('!drwr'=>$drawer));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached child drawers items - delete not permitted in this case
    $sql = "SELECT COUNT(w.nid) from {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.wnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('drawer',t('This drawer is still storing @num sub-drawers!', array('@num'=>$count)));
    }
    // Check for attached folders - delete not permitted in this case
    $sql = "SELECT COUNT(f.nid) from {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.wnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('drawer',t('This drawer is still storing @num folders!', array('@num'=>$count)));
    }
  }
}

function filecabinet_folder_form_validate($form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_folder_form_validate',6);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $folder = strip_tags($form_state['values']['folder']);
    $parent = strip_tags($form_state['values']['parent']);
    $drawer = strip_tags($form_state['values']['drawer']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable - use PHP strip_tags function to check
    if ($folder != $form_state['values']['folder']) {
        form_set_error('folder',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($parent != $form_state['values']['parent']) {
        form_set_error('parent',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($drawer != $form_state['values']['drawer']) {
        form_set_error('drawer',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Check that any name returned in "parent" is valid
    if ($parent) {
      $fnid = filecabinet_validate_folder($parent);
      if ($fnid) {
        form_set_value($form['fnid'], $fnid, $form_state);
      }
      else {
        form_set_error('parent',t('The name specified is not a known file folder'));
      }
    }
    else {
      $fnid = 0;
      form_set_value($form['fnid'], $fnid, $form_state);
    }
    // Check that any name returned in "drawer" is valid
    if ($drawer) {
      $wnid = filecabinet_validate_drawer($drawer);
      if ($wnid) {
        form_set_value($form['wnid'], $wnid, $form_state);
      }
      else {
        form_set_error('drawer',t('The name specified is not a known filing cabinet drawer'));
      }
    }
    else {
      $wnid = 0;
      form_set_value($form['wnid'], $wnid, $form_state);
    }
    // Check that either a parent folder or a drawer has been specified
    if (!$parent && !$drawer) {
        form_set_error('drawer',t('Must specify either a drawer or a parent folder'));
    }
    if ($parent && $drawer) {
      form_set_error('drawer',t('Cannot specify both a drawer and a parent folder'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.folder = '%s' AND f.wnid = %d";
    $nid = db_result(db_query($sql, $folder, $wnid));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('folder',t('Folder already exists for this drawer'));
      }
    }
    // Build a composite node title and save in $form_state
    if ($drawer) {
      $form_state['values']['title'] = t('Folder: !fldr in drawer !drwr', array('!fldr'=>$folder, '!drwr'=>$drawer));
    }
    else {
      $form_state['values']['title'] = t('Folder: !fldr within !prnt', array('!fldr'=>$folder, '!prnt'=>$parent));
    }
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached sub-folders - delete not permitted in this case
    $sql = "SELECT COUNT(f.nid) from {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.fnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('folder',t('This folder is still storing @num sub-folders!', array('@num'=>$count)));
    }
    // Check for attached documents - delete not permitted in this case
    $sql = "SELECT COUNT(d.nid) from {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.fnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('folder',t('This folder is still storing @num documents!', array('@num'=>$count)));
    }
  }
}

function filecabinet_document_form_validate($form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_document_form_validate',6);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $docname = strip_tags($form_state['values']['docname']);
    $docversion = strip_tags($form_state['values']['docversion']);
    $member = strip_tags($form_state['values']['member']);
    $folder = strip_tags($form_state['values']['folder']);
    $headoc = strip_tags($form_state['values']['headoc']);
    $docdesc = strip_tags($form_state['values']['docdesc']);
    $notes = strip_tags($form_state['values']['notes']);

    // Check that entered details are acceptable - use PHP strip_tags function to check
    if ($docname != $form_state['values']['docname']) {
        form_set_error('docname',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($docversion != $form_state['values']['docversion']) {
        form_set_error('docversion',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($folder != $form_state['values']['folder']) {
        form_set_error('folder',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($headoc != $form_state['values']['headoc']) {
        form_set_error('headoc',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($docdesc != $form_state['values']['docdesc']) {
        form_set_error('docdesc',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    // Prepare date values
  //$docdate = date_format_date(date_make_date($form_state['values']['docdate']), 'custom', 'd M Y');
    // Check that the name returned in "member" is valid
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($member);
    if ($mnid) {
      form_set_value($form['mnid'], $mnid, $form_state);
    }
    else {
      form_set_error('member',t('The name specified is not a known member'));
    }
    // Check that any name returned in "folder" is valid
    if ($folder) {
      $fnid = filecabinet_validate_folder($folder);
      if ($fnid) {
        form_set_value($form['fnid'], $fnid, $form_state);
      }
      else {
        form_set_error('folder',t('The name specified is not a known file cabinet folder'));
      }
    }
    else {
      $fnid = 0;
      form_set_value($form['fnid'], $fnid, $form_state);
    }
    // Check that any name returned in "headoc" is valid
    if ($headoc) {
      $dnid = filecabinet_validate_docname($headoc);
      if ($dnid) {
        form_set_value($form['dnid'], $dnid, $form_state);
      }
      else {
        form_set_error('headoc',t('The name specified is not a known document'));
      }
    }
    else {
      $dnid = 0;
      form_set_value($form['dnid'], $dnid, $form_state);
    }
    // Check that either folder or headoc is specified, but not both
    if (!$folder && !$headoc) {
      form_set_error('folder',t('Must specify either a folder or a head document'));
    }
    if ($folder && $headoc) {
      form_set_error('folder',t('Cannot specify both a folder and a head document'));
    }
    // Check upload file if specified
    if (isset($_FILES['files']['name']['upload']) && $_FILES['files']['name']['upload']) {
      if ($_FILES['files']['error']['upload']) {
        form_set_error('upload',t('File upload error: @err', array('@err'=>$_FILES['files']['error']['upload'])));
        // refer to http://php.net/manual/en/features.file-upload.errors.php for details of error codes
      }
      elseif ($file = filecabinet_validate_upload_file($form, $form_state)) {
        form_set_value($form['afid'], $file->fid, $form_state);
        $form_state['storage']['file'] = $file;
      }
      else {
        form_set_error('upload',t('Error saving uploaded file'));
      }
    }
    // Check file detach request
    if ($form_state['values']['detach']['file']) {
      // does this user have permission to detach files?
      if (!user_access('delete file')) {
        // No, so issue a warning
        form_set_error('detach',t('Insufficient priviledge to delete attached file'));
      }       
    }
    // Prevent duplicate records in filing cabinet
    $sql = "SELECT n.nid FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.docname = '%s' AND d.docversion = '%s'";
    $nid = db_result(db_query($sql, $docname, $docversion));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('docname',t('Document with this name and version already exists in filing cabinet'));
      }
    }
    // Build a composite node title and save in $form_state
    if ($docversion) {
      $form_state['values']['title'] = t('Document: !doc - v: !ver', array('!doc'=>$docname, '!ver'=>$docversion));
    }
    else {
      $form_state['values']['title'] = t('Document: !doc - !date', array('!doc'=>$docname, '!date'=>$docdate));
    }
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //orgright_debug_msg('filecabinet','--- Delete button clicked ---');
    // Check if this document has attached sub-documents
    $sql = "SELECT COUNT(d.nid) FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.dnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('docname',t('This document still has @num attachments!', array('@num'=>$count)));
    }
    // Check if this document is linked to other information items
    $sql = "SELECT COUNT(dnid) FROM {org_doclinks} WHERE dnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('docname',t('This document is still linked to @num items!', array('@num'=>$count)));
    }
  }
}

function filecabinet_doclink_form_validate($form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_form_validate',6);
  //orgright_debug_dump('form_state',$form_state);
  //orgright_debug_dump('form',$form);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['submit']['#value']) {
    // check to see whether we are adding a new doclink or editing and existing one
    if ($form_state['storage']['action'] == 'add') {
      // Check that entered details are acceptable - use PHP strip_tags function to check
      if ($form_state['values']['docname'] != strip_tags($form_state['values']['docname'])) {
        form_set_error('docname',t('HTML or PHP tags are not permitted in this field'));
      }
      if ($form_state['values']['infoitem'] != strip_tags($form_state['values']['infoitem'])) {
        form_set_error('infoitem',t('HTML or PHP tags are not permitted in this field'));
      }
      // Check that the name returned in "docname" is valid
      $dnid = filecabinet_validate_docname($form_state['values']['docname']);
      if ($dnid) {
        form_set_value($form['dnid'], $dnid, $form_state);
      }
      else {
        form_set_error('docname',t('The name specified is not a known document'));
      }
      // Check that the name returned in "infoitem" is valid - note return is an object
      $obj = filecabinet_validate_nodetitle($form_state['values']['infoitem']);
      if ($obj->nid) {
        form_set_value($form['xnid'], $obj->nid, $form_state);
        form_set_value($form['nodetype'], $obj->type, $form_state);
      }
      else {
        form_set_error('infoitem',t('The name specified is not a known information item'));
      }
      // Check that this document is not already linked to this item
      $sql = "SELECT linktype from {org_doclinks} WHERE dnid = %d AND xnid = %d";
      $linkexists = db_result(db_query($sql, $dnid, $obj->nid));
      if ($linkexists) {
        form_set_error('docname',t('The document specified is already linked to the selected information item'));
      }
    }
    // If this is an edit, then check to see that docname and infoitem have not been changed
    elseif ($form_state['storage']['action'] == 'edit') {
      //if ($form_state['values']['docname'] != $form_state['storage']['docname']) {
      //  form_set_error('docname',t('Cannot save - linked document has been altered'));
      //}
      //else {
        form_set_value($form['dnid'], $form_state['storage']['dnid'], $form_state);
      //}
      //if ($form_state['values']['infoitem'] != $form_state['storage']['infoitem']) {
      //  form_set_error('infoitem',t('Cannot save - linked information item has been altered'));
      //}
      //else {
        form_set_value($form['xnid'], $form_state['storage']['xnid'], $form_state);
      //}
    }
    // record that the submit button was clicked
    $form_state['storage']['button'] = 'submit';
  }
  elseif ($clicked_button == $form['delete']['#value']) {
    //orgright_debug_msg('filecabinet','--- Delete button clicked ---');
    // Check to see that none of the doclink details have altered
    //if ($form_state['values']['docname'] != $form_state['storage']['docname']) {
    //  form_set_error('docname',t('Cannot delete - linked document has been altered'));
    //}
    //else {
      form_set_value($form['dnid'], $form_state['storage']['dnid'], $form_state);
    //}
    //if ($form_state['values']['infoitem'] != $form_state['storage']['infoitem']) {
    //  form_set_error('infoitem',t('Cannot delete - linked information item has been altered'));
    //}
    //else {
      form_set_value($form['xnid'], $form_state['storage']['xnid'], $form_state);
    //}
    if ($form_state['values']['linktype'] != $form_state['storage']['linktype']) {
      form_set_error('linktype',t('Cannot delete - link type has been altered'));
    }
    // record that the delete button was clicked
    $form_state['storage']['button'] = 'delete';
  }
}

function filecabinet_validate_drawer(&$drawer) {
  // Check that the value passed in "$drawer" is valid, and return the associated nid
  //orgright_debug_msg('filecabinet',t('Validate drawer name: @mbr', array('@mbr' => $drawer)),7);
  if (is_numeric($drawer)) {
    // then the passed value should be the nid of a drawer node
    $dnid = $drawer;
    $sql = "SELECT n.type, w.drawer FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
    $obj = db_fetch_object(db_query($sql, $dnid));
    if ($obj->type == 'drawer') { $drawer = $obj->drawer; }
    else { $dnid = 0; }
    return $dnid;
  }
  else {
    // Query the database to ensure that the drawer name is valid, and to find the wnid
    $sql = "SELECT w.nid FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.drawer = '%s'";
    return db_result(db_query($sql, $drawer));
  }
}

function filecabinet_validate_folder(&$folder) {
  // Check that the value passed in "$folder" is valid, and return the associated nid
  if (is_numeric($folder)) {
    // then the passed value should be the nid of a drawer node
    $fnid = $folder;
    $sql = "SELECT n.type, f.folder FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
    $obj = db_fetch_object(db_query($sql, $fnid));
    if ($obj->type == 'folder') { $folder = $obj->folder; }
    else { $fnid = 0; }
    return $fnid;
  }
  else {
    // Query the database to ensure that the folder name is valid, and to find the fnid
    $sql = "SELECT f.nid FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.folder = '%s'";
    return db_result(db_query($sql, $folder));
  }
}

function filecabinet_validate_docname(&$docname) {
  // Check that the value passed in "$docname" is valid, and return the associated nid
  if (is_numeric($docname)) {
    // then the passed value should be the nid of a document node
    $dnid = $docname;
    $sql = "SELECT n.type, d.docname FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
    $obj = db_fetch_object(db_query($sql, $dnid));
    if ($obj->type == 'document') { $docname = $obj->docname; }
    else { $dnid = 0; }
    return $dnid;
  }
  else {
    // Query the database to ensure that the docname name is valid, and to find the dnid
    $sql = "SELECT d.nid FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.docname = '%s'";
    return db_result(db_query($sql, $docname));
  }
}

function filecabinet_validate_owner(&$owner) {
  // Check that the value passed in "$owner" is valid, and return the associated uid
  if (is_numeric($owner)) {
    // then the passed value should be the uid of a user
    $ouid = $owner;
    $sql = "SELECT uid, name FROM {users} u WHERE uid = %d";
    $obj = db_fetch_object(db_query($sql, $ouid));
    if ($obj->uid > O_MAX_PROTECTED_USER) { $owner = $obj->name; }
    else { $ouid = 0; }
    return $ouid;
  }
  else {
    // Query the database to ensure that the user name is valid, and to find the uid
    $sql = "SELECT uid FROM {users} WHERE uid > %d AND name = '%s'";
    return db_result(db_query($sql, O_MAX_PROTECTED_USER, $owner));
  }
}

function filecabinet_validate_nodetitle($nodetitle) {
  // Check that the value passed in "$nodetitle" is valid, and return the associated nid
  // Query the database to ensure that the node title name is valid, and to find the fnid
  $sql = "SELECT n.nid, n.type FROM {node} n WHERE n.title = '%s'";
  return db_fetch_object(db_query($sql, $nodetitle));
}

function filecabinet_validate_upload_file($form, &$form_state) {
  // Specify some values required for validating the uploaded file
  $limits = array(
    'extensions' => filecabinet_get_upload_extensions(),
    'file_size' => filecabinet_get_upload_filesize()*1048576,
    'user_size' => filecabinet_get_upload_usersize()*1048576,
    'resolution' => filecabinet_get_upload_resolution(),
  );
  $validators = array(
    'file_validate_extensions' => array($limits['extensions']),
    'file_validate_image_resolution' => array($limits['resolution']),
    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
  );
  // Check the filename, remove any unacceptable characters and save back in the globals variable
  $_FILES['files']['name']['upload'] = filecabinet_check_filename($_FILES['files']['name']['upload']);
  // save the file in a sub-directory of "filecabinet" that depends on the date
  $today = getdate();
  // check that the year portion exists, create if not
  $subpath = 'filecabinet/' . $today['year'];
  $filepath = file_create_path($subpath);
  file_check_directory($filepath, FILE_CREATE_DIRECTORY, 'upload');
  // check that the month portion exists, create if not
  $subpath .= '/' . substr(strtolower($today['month']),0,3);
  $filepath = file_create_path($subpath);
  file_check_directory($filepath, FILE_CREATE_DIRECTORY, 'upload');
  // save the uploaded file and return the file object
  $file = file_save_upload('upload', $validators, $filepath);
  return $file;
}

/**
 * Form submit routines, called from filecabinet_form_submit() in filecabinet.module
 */
function filecabinet_drawer_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('filecabinet','Fn: filecabinet_drawer_form_submit',6);
  // Massage the date and times ready for storage in the database
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function filecabinet_folder_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('filecabinet','Fn: filecabinet_folder_form_submit',6);
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function filecabinet_document_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('filecabinet','Fn: filecabinet_document_form_submit',6);
  // Massage the date ready for storage in the database
  $form_state['values']['docdate'] = substr($form_state['values']['docdate'],0,10);
  // make the uploaded file (if any) permanent.
  if (isset($form_state['storage']['file'])) {
    // Make file permanent in the file store
    $file = $form_state['storage']['file'];
    file_set_status($file, FILE_STATUS_PERMANENT);
    // and remove from form_state storage
    unset($form_state['storage']['file']);
  }
  // detach (and delete if necessary) the attached file
  if ($form_state['values']['detach']['file']) {
    $afid = $form['afid']['#value'];
    form_set_value($form['afid'], 0, $form_state);
    module_load_include('inc', 'filecabinet', 'filecabinet_data');
    filecabinet_delete_file($afid, 1);
  }        
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function filecabinet_doclink_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_form_submit',6);
  $dnid = $form_state['values']['dnid'];
  $xnid = $form_state['values']['xnid'];
  $nodetype = $form_state['values']['nodetype'];
  $linktype = $form_state['values']['linktype'];
  if ($form_state['storage']['button'] == 'submit') {
    // check whether we are adding a new link or editing an existing link
    if ($form_state['storage']['action'] == 'add') {
      // Create and save the link
      //orgright_debug_msg('filecabinet','insert doclink, dnid='.$dnid.', xnid='.$xnid);
      db_query("INSERT INTO {org_doclinks} (dnid, xnid, nodetype, linktype) VALUES (%d, %d, '%s', '%s')", $dnid, $xnid, $nodetype, $linktype);
    }
    else {
      // update existing link
      //orgright_debug_msg('filecabinet','update doclink, dnid='.$dnid.', xnid='.$xnid);
      db_query("UPDATE {org_doclinks} SET nodetype = '%s', linktype = '%s' WHERE dnid = %d AND xnid = %d", $nodetype, $linktype, $dnid, $xnid);
    }
  }
  elseif ($form_state['storage']['button'] == 'delete') {
    //orgright_debug_msg('filecabinet','delete doclink, dnid='.$dnid.', xnid='.$xnid);
    db_query("DELETE FROM {org_doclinks} WHERE dnid = %d AND xnid = %d", $dnid, $xnid);
    drupal_set_message(t('The link associating document @doc with @item has been deleted.', array('@doc'=>$form_state['storage']['docname'], '@item'=>$form_state['storage']['infoitem'])));
  }
  // Delete form_state storage items and set the redirection
  $form_state['storage'] = array();
  $form_state['redirect'] = 'filecabinet/documents';
}

