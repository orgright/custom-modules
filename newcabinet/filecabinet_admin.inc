<?php
// $Id$

/**
 * @file
 * Provides administration functions for filecabinet module.
 */

function filecabinet_admin() {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_admin',5);
  return drupal_get_form('filecabinet_admin_form');
}

/**
 * Build system settings form
 * @ingroup forms
 * @see system_settings_form()
 */
function filecabinet_admin_form_build (&$form_state = NULL) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_admin_form_build',5);
  $form = array();
  // Check to see whether this is a new form, or a rebuild
  $form_state['storage']['new_form'] = TRUE;
  if (isset($form_state['values'])) {
    // The form has been submitted for edit or delete of a value
    $form_state['storage']['new_form'] = FALSE;
  }
  if ($form_state['storage']['new_form']) {
    // This is a new form request
    // Specify a fieldset for default drawer owner
    $form['default_owner'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default Drawer Owner'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $drawer_default_owner = filecabinet_get_default_owner_name();
    $form['default_owner']['default_owner_name'] = array(
      '#type' => 'textfield',
      '#title' => t('User name'),
      '#description' => t('Select the name of the user to be the default drawer owner'),
      '#default_value' => $drawer_default_owner,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 1,
      '#autocomplete_path' => 'filecabinet/autocomplete/owners',
    );
    $form['default_owner']['default_owner_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Set default owner'),
      '#weight' => 2,
    );
    // Specify a fieldset for default values for mail capture
    $form['mail_capture'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default Email Capture Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $mail_default_folder = filecabinet_get_default_mail_folder();
    $form['mail_capture']['default_mail_folder'] = array(
      '#type' => 'textfield',
      '#title' => t('Mail folder'),
      '#description' => t('Select the folder to hold captured email documents'),
      '#default_value' => $mail_default_folder,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 1,
      '#autocomplete_path' => 'filecabinet/autocomplete/folders',
    );
    $mail_default_member = filecabinet_get_default_mail_member();
    $form['mail_capture']['default_mail_member'] = array(
      '#type' => 'textfield',
      '#title' => t('Mail member'),
      '#description' => t('Select the default member to own captured email documents'),
      '#default_value' => $mail_default_member,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 2,
      '#autocomplete_path' => 'membership/autocomplete/members',
    );
    $document_type_list = filecabinet_get_document_types();
    $form['mail_capture']['default_mail_doctype'] = array(
      '#type' => 'select',
      '#title' => t('Default type'),
      '#description' => t('Select the default type of captured email documents'),
      '#default_value' => filecabinet_get_default_mail_doctype(),
      '#options' => $document_type_list,
      '#required' => TRUE,
      '#weight' => 3,
    );
    $document_status_list = filecabinet_get_document_statuses();
    $form['mail_capture']['default_mail_docstatus'] = array(
      '#type' => 'select',
      '#title' => t('Default status'),
      '#description' => t('Select the default status of captured email documents'),
      '#default_value' => filecabinet_get_default_mail_docstatus(),
      '#options' => $document_status_list,
      '#required' => TRUE,
      '#weight' => 4,
    );
    $form['mail_capture']['default_mail_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Set default values'),
      '#weight' => 5,
    );
    // Specify a fieldset for drawer status values
    $form['drawer_statuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drawer Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $drawer_status_list = filecabinet_get_drawer_statuses();
    $form['drawer_statuses']['drawer_status_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current drawer status values'),
      '#description' => t('Select a value to edit or delete'),
      '#options' => $drawer_status_list,
      '#weight' => 1,
    );
    $form['drawer_statuses']['drawer_status_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit drawer status'),
      '#weight' => 2,
    );
    $form['drawer_statuses']['drawer_status_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete drawer status'),
      '#weight' => 3,
    );
    $form['drawer_statuses']['drawer_status_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Drawer status value'),
      '#description' => t('Specify a new status value to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );  
    $form['drawer_statuses']['drawer_status_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add drawer status'),
      '#weight' => 5,
    );
    // Specify a fieldset for folder status values
    $form['folder_statuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Folder Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $folder_status_list = filecabinet_get_folder_statuses();
    $form['folder_statuses']['folder_status_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current folder status values'),
      '#description' => t('Select a value to edit or delete'),
      '#options' => $folder_status_list,
      '#weight' => 1,
    );
    $form['folder_statuses']['folder_status_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit folder status'),
      '#weight' => 2,
    );
    $form['folder_statuses']['folder_status_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete folder status'),
      '#weight' => 3,
    );
    $form['folder_statuses']['folder_status_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Folder status value'),
      '#description' => t('Specify a new status value to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );  
    $form['folder_statuses']['folder_status_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add folder status'),
      '#weight' => 5,
    );
    // Specify a fieldset for document status values
    $form['document_statuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Document Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $document_status_list = filecabinet_get_document_statuses();
    $form['document_statuses']['document_status_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current document status values'),
      '#description' => t('Select a value to edit or delete'),
      '#options' => $document_status_list,
      '#weight' => 1,
    );
    $form['document_statuses']['document_status_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit document status'),
      '#weight' => 2,
    );
    $form['document_statuses']['document_status_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete document status'),
      '#weight' => 3,
    );
    $form['document_statuses']['document_status_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Document status value'),
      '#description' => t('Specify a new status value to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );  
    $form['document_statuses']['document_status_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add document status'),
      '#weight' => 5,
    );
    // Now specify the Document Types fieldset
    $form['document_types'] = array(
      '#type' => 'fieldset',
      '#title' => t('Document Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $document_type_list = filecabinet_get_document_types();
    $form['document_types']['document_type_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current document types'),
      '#description' => t('Select a type to edit or delete'),
      '#options' => $document_type_list,
      '#weight' => 1,
    );
    $form['document_types']['document_type_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit document type'),
      '#weight' => 2,
    );
    $form['document_types']['document_type_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete document type'),
      '#weight' => 3,
    );
    $form['document_types']['document_type_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Document type'),
      '#description' => t('Specify a new document type to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );  
    $form['document_types']['document_type_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add document type'),
      '#weight' => 5,
    );
    // Now specify the doclinks node type fieldset
    $form['doclink_nodetypes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Document Link Targets'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    // get a list of all current node types
    $nodetypes_name_list = node_get_types('names');
    // but don't allow the document type to be a doclink target
    unset($nodetypes_name_list['document']);
    $doclink_nodetype_list = filecabinet_get_doclink_nodetypes();
    $form['doclink_nodetypes']['doclink_nodetype_list'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Current document link targets'),
      '#description' => t('Select content types permitted to have linked documents'),
      '#default_value' => $doclink_nodetype_list,
      '#options' => $nodetypes_name_list,
      '#weight' => 1,
    );
    $form['doclink_nodetypes']['doclink_nodetype_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save list'),
      '#weight' => 2,
    );
    // Now specify the Doclink Types fieldset
    $form['doclink_linktypes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Document Link Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $doclink_linktype_list = filecabinet_get_doclink_linktypes();
    $form['doclink_linktypes']['doclink_linktype_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current document link types'),
      '#description' => t('Select a type to edit or delete'),
      '#options' => $doclink_linktype_list,
      '#weight' => 1,
    );
    $form['doclink_linktypes']['doclink_linktype_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit link type'),
      '#weight' => 2,
    );
    $form['doclink_linktypes']['doclink_linktype_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete link type'),
      '#weight' => 3,
    );
    $form['doclink_linktypes']['doclink_linktype_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Document link type'),
      '#description' => t('Specify a new link type to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );  
    $form['doclink_linktypes']['doclink_linktype_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add link type'),
      '#weight' => 5,
    );
    // Specify a fieldset for the upload file settings
    $form['upload_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Upload File Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['upload_settings']['upload_extensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Valid file extensions'),
      '#description' => t('Specify the file extensions that will be allowed for uploads in a space separated list'),
      '#default_value' => filecabinet_get_upload_extensions(),
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 1,
    );
    $form['upload_settings']['upload_filesize'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum file size'),
      '#description' => t('Specify the maximum size allowed for indivudual uploaded file (MB)'),
      '#default_value' => filecabinet_get_upload_filesize(),
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 2,
    );
    $form['upload_settings']['upload_usersize'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum user storage'),
      '#description' => t('Specify the maximum space allowed for uploaded files by an indivudual user (MB)'),
      '#default_value' => filecabinet_get_upload_usersize(),
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 3,
    );
    $form['upload_settings']['upload_resolution'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum image resolution'),
      '#description' => t('Specify the maximum resolution allowed for an uploaded image as WIDTHxHEIGHT.  Set to 0 for no restriction.'),
      '#default_value' => filecabinet_get_upload_resolution(),
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#weight' => 4,
    );
    $form['upload_settings']['upload_settings_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Set values'),
      '#weight' => 5,
    );
  }
  else {
    // This is a form rebuild
    if (isset($form_state['storage']['this_drawer_status'])) {
      // Are either editing or deleting a drawer status
      $drawer_status_list = filecabinet_get_drawer_statuses();
      $drawer_status_value = $drawer_status_list[$form_state['storage']['this_drawer_status']];
      //orgright_debug_msg('admin','admin form rebuild, drawer_status='.$drawer_status_value,7);
      if ($form_state['storage']['operation'] == 'edit_drawer_status') {
        $form['drawer_status'] = array(
          '#type' => 'textfield',
          '#title' => t('Drawer status'),
          '#description' => t('Edit status value'),
          '#default_value' => $drawer_status_value,
          '#maxlength' => 16,
          '#size' => 16,
          '#required' => TRUE,
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save changes'),
          '#weight' => 2,
        );
      }
      else {
        $form['drawer_status'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $drawer_status_value,
          '#suffix' => '</div>',
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Confirm deletion'),
          '#weight' => 2,
        );
      } 
    }
    elseif (isset($form_state['storage']['this_folder_status'])) {
      // Are either editing or deleting a folder status
      $folder_status_list = filecabinet_get_folder_statuses();
      $folder_status_value = $folder_status_list[$form_state['storage']['this_folder_status']];
      //orgright_debug_msg('admin','admin form rebuild, folder_status='.$folder_status_value,7);
      if ($form_state['storage']['operation'] == 'edit_folder_status') {
        $form['folder_status'] = array(
          '#type' => 'textfield',
          '#title' => t('folder status'),
          '#description' => t('Edit status value'),
          '#default_value' => $folder_status_value,
          '#maxlength' => 16,
          '#size' => 16,
          '#required' => TRUE,
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save changes'),
          '#weight' => 2,
        );
      }
      else {
        $form['folder_status'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $folder_status_value,
          '#suffix' => '</div>',
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Confirm deletion'),
          '#weight' => 2,
        );
      } 
    }
    if (isset($form_state['storage']['this_document_status'])) {
      // Are either editing or deleting a document status
      $document_status_list = filecabinet_get_document_statuses();
      $document_status_value = $document_status_list[$form_state['storage']['this_document_status']];
      //orgright_debug_msg('admin','admin form rebuild, document_status='.$document_status_value,7);
      if ($form_state['storage']['operation'] == 'edit_document_status') {
        $form['document_status'] = array(
          '#type' => 'textfield',
          '#title' => t('document status'),
          '#description' => t('Edit status value'),
          '#default_value' => $document_status_value,
          '#maxlength' => 16,
          '#size' => 16,
          '#required' => TRUE,
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save changes'),
          '#weight' => 2,
        );
      }
      else {
        $form['document_status'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $document_status_value,
          '#suffix' => '</div>',
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Confirm deletion'),
          '#weight' => 2,
        );
      } 
    }
    elseif (isset($form_state['storage']['this_document_type'])) {
      // Are either editing or deleting a document type
      $document_type_list = filecabinet_get_document_types();
      $document_type_value = $document_type_list[$form_state['storage']['this_document_type']];
      //orgright_debug_msg('admin','admin form rebuild, document type='.$document_type_value,7);
      if ($form_state['storage']['operation'] == 'edit_document_type') {
        $form['document_type'] = array(
          '#type' => 'textfield',
          '#title' => t('Document type'),
          '#description' => t('Edit document type'),
          '#default_value' => $document_type_value,
          '#maxlength' => 16,
          '#size' => 16,
          '#required' => TRUE,
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save changes'),
          '#weight' => 2,
        );
      }
      else {
        $form['document_type'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $document_type_value,
          '#suffix' => '</div>',
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Confirm deletion'),
          '#weight' => 2,
        );
      }
    }
    elseif (isset($form_state['storage']['this_doclink_linktype'])) {
      // Are either editing or deleting a doclink type
      $doclink_linktype_list = filecabinet_get_doclink_linktypes();
      $doclink_linktype_value = $doclink_linktype_list[$form_state['storage']['this_doclink_linktype']];
      //orgright_debug_msg('admin','admin form rebuild, doclink type='.$doclink_linktype_value,7);
      if ($form_state['storage']['operation'] == 'edit_doclink_linktype') {
        $form['doclink_linktype'] = array(
          '#type' => 'textfield',
          '#title' => t('Document link type'),
          '#description' => t('Edit link type'),
          '#default_value' => $doclink_linktype_value,
          '#maxlength' => 16,
          '#size' => 16,
          '#required' => TRUE,
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Save changes'),
          '#weight' => 2,
        );
      }
      else {
        $form['document_type'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $doclink_linktype_value,
          '#suffix' => '</div>',
          '#weight' => 1,
        );  
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Confirm deletion'),
          '#weight' => 2,
        );
      }
    }
    else {
      //orgright_debug_msg('filecabinet','ERROR - unknown admin form rebuild requested',3);
    }
  }
  $form['#validate'] = array('filecabinet_form_validate');
  $form['#submit'] = array('filecabinet_form_submit');
  //return system_settings_form($form);
  return $form;
}

function filecabinet_form_filecabinet_admin_form_alter(&$form, &$form_state) {
 //orgright_debug_msg('filecabinet','Fn: filecabinet_form_filecabinet_admin_form_alter',5);
  // Remove submit and reset buttons
  unset($form['buttons']['submit']);
  unset($form['buttons']['reset']);
}

function filecabinet_admin_form_validate($form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_admin_form_validate',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('filecabinet','clicked button='.$clicked_button, 6);
  // check for clicked button in default drawer owner fieldset
  if ($clicked_button == $form['default_owner']['default_owner_submit']['#value']) {
    // Check that the name returned is valid
    module_load_include('inc', 'filecabinet', 'filecabinet_form'); 
    $ouid = filecabinet_validate_owner($form_state['values']['default_owner_name']);
    if ($ouid) {
      $form_state['storage']['ouid'] = $ouid;
    }
    else {
      form_set_error('default_owner_name',t('The name specified is not a known user'));
    }
  }
  // check for clicked button in default mail capture fieldset
  elseif ($clicked_button == $form['mail_capture']['default_mail_submit']['#value']) {
    // Check that the name returned in "default_mail_folder" valid
    module_load_include('inc', 'filecabinet', 'filecabinet_form'); 
    $fnid = filecabinet_validate_folder($form_state['values']['default_mail_folder']);
    if ($fnid) {
      $form_state['storage']['fnid'] = $fnid;
    }
    else {
      form_set_error('default_mail_folder',t('The name specified is not a valid filecabinet folder'));
    }
    // Check that the name returned in "default_mail_member" valid
    module_load_include('inc', 'membership', 'membership_form'); 
    $mnid = membership_validate_member($form_state['values']['default_mail_member']);
    if ($mnid) {
      $form_state['storage']['mnid'] = $mnid;
    }
    else {
      form_set_error('default_mail_member',t('The name specified is not a valid member'));
    }
    // Check that the value returned in "default_mail_doctype" is valid
    $doctype = $form_state['values']['default_mail_doctype'];
    if (in_array($doctype, filecabinet_get_document_types())) {
      $form_state['storage']['doctype'] = $doctype;
    }
    else {
      form_set_error('default_mail_doctype',t('The type specified is not valid'));
    }
    // Check that the value returned in "default_mail_docstatus" is valid
    $docstatus = $form_state['values']['default_mail_docstatus'];
    if (in_array($docstatus, filecabinet_get_document_statuses())) {
      $form_state['storage']['docstatus'] = $docstatus;
    }
    else {
      form_set_error('default_mail_docstatus',t('The status specified is not valid'));
    }
  }
  // check for clicked button in drawer status fieldset
  elseif ($clicked_button == $form['drawer_statuses']['drawer_status_add']['#value']) {
    //orgright_debug_msg('filecabinet','new drawer status='.$form_state['values']['drawer_status_new'],8);
    if (!$form_state['values']['drawer_status_new']) {
      form_set_error('drawer_status_new',t('A value must be specified for the new drawer status'));
    }
    if ($form_state['values']['drawer_status_new'] != strip_tags($form_state['values']['drawer_status_new'])) {
      form_set_error('drawer_status_new',t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['drawer_statuses']['drawer_status_edit']['#value']) {
    $form_state['storage']['this_drawer_status'] = $form_state['values']['drawer_status_list'];
    $form_state['storage']['operation'] = 'edit_drawer_status';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['drawer_statuses']['drawer_status_delete']['#value']) {
    $form_state['storage']['this_drawer_status'] = $form_state['values']['drawer_status_list'];
    $form_state['storage']['operation'] = 'delete_drawer_status';
    $form_state['rebuild'] = TRUE;
  }
  // check for clicked button in folder status fieldset
  elseif ($clicked_button == $form['folder_statuses']['folder_status_add']['#value']) {
    //orgright_debug_msg('filecabinet','new folder status='.$form_state['values']['folder_status_new'],8);
    if (!$form_state['values']['folder_status_new']) {
      form_set_error('folder_status_new',t('A value must be specified for the new folder status'));
    }
    if ($form_state['values']['folder_status_new'] != strip_tags($form_state['values']['folder_status_new'])) {
      form_set_error('folder_status_new',t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['folder_statuses']['folder_status_edit']['#value']) {
    $form_state['storage']['this_folder_status'] = $form_state['values']['folder_status_list'];
    $form_state['storage']['operation'] = 'edit_folder_status';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['folder_statuses']['folder_status_delete']['#value']) {
    $form_state['storage']['this_folder_status'] = $form_state['values']['folder_status_list'];
    $form_state['storage']['operation'] = 'delete_folder_status';
    $form_state['rebuild'] = TRUE;
  }
  // check for clicked button in document status fieldset
  elseif ($clicked_button == $form['document_statuses']['document_status_add']['#value']) {
    //orgright_debug_msg('filecabinet','new document status='.$form_state['values']['document_status_new'],8);
    if (!$form_state['values']['document_status_new']) {
      form_set_error('document_status_new',t('A value must be specified for the new document status'));
    }
    if ($form_state['values']['document_status_new'] != strip_tags($form_state['values']['document_status_new'])) {
      form_set_error('document_status_new',t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['document_statuses']['document_status_edit']['#value']) {
    $form_state['storage']['this_document_status'] = $form_state['values']['document_status_list'];
    $form_state['storage']['operation'] = 'edit_document_status';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['document_statuses']['document_status_delete']['#value']) {
    $form_state['storage']['this_document_status'] = $form_state['values']['document_status_list'];
    $form_state['storage']['operation'] = 'delete_document_status';
    $form_state['rebuild'] = TRUE;
  }
  // check for clicked button in document type fieldset
  elseif ($clicked_button == $form['document_types']['document_type_add']['#value']) {
    //orgright_debug_msg('filecabinet','new document_type='.$form_state['values']['document_type_new'],8);
    if (!$form_state['values']['document_type_new']) {
      form_set_error('document_type_new',t('A value must be specified for the new document type'));
    }
    if ($form_state['values']['document_type_new'] != strip_tags($form_state['values']['document_type_new'])) {
      form_set_error('document_type_new',t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['document_types']['document_type_edit']['#value']) {
    $form_state['storage']['this_document_type'] = $form_state['values']['document_type_list'];
    $form_state['storage']['operation'] = 'edit_document_type';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['document_types']['document_type_delete']['#value']) {
    $form_state['storage']['this_document_type'] = $form_state['values']['document_type_list'];
    $form_state['storage']['operation'] = 'delete_document_type';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['doclink_nodetypes']['doclink_nodetype_submit']['#value']) {
    // The submit button for doclink target node types has been clicked
  }
  // check for clicked button in doclink linktype fieldset
  elseif ($clicked_button == $form['doclink_linktypes']['doclink_linktype_add']['#value']) {
    //orgright_debug_msg('filecabinet','new doclink_linktype='.$form_state['values']['doclink_linktype_new'],8);
    if (!$form_state['values']['doclink_linktype_new']) {
      form_set_error('doclink_linktype_new',t('A value must be specified for the new document link type'));
    }
    if ($form_state['values']['doclink_linktype_new'] != strip_tags($form_state['values']['doclink_linktype_new'])) {
      form_set_error('doclink_linktype_new',t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['doclink_linktypes']['doclink_linktype_edit']['#value']) {
    $form_state['storage']['this_doclink_linktype'] = $form_state['values']['doclink_linktype_list'];
    $form_state['storage']['operation'] = 'edit_doclink_linktype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['doclink_linktypes']['doclink_linktype_delete']['#value']) {
    $form_state['storage']['this_doclink_linktype'] = $form_state['values']['doclink_linktype_list'];
    $form_state['storage']['operation'] = 'delete_doclink_linktype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['upload_settings']['upload_settings_submit']['#value']){
    // The submit button for the upload file seetings has been clicked
    if (preg_replace('/[,.-]/', '//', $form_state['values']['upload_extensions']) != $form_state['values']['upload_extensions']) {
        form_set_error('upload_extensions', t('Specify a list of allowed upload extensions, each separated by spaces'));
    }
    if (!is_numeric($form_state['values']['upload_filesize']) || $form_state['values']['upload_filesize'] < 0) {
        form_set_error('upload_filesize', t('A number greater than zero must be specified for the upload file size'));
      }
    if (is_numeric($form_state['values']['upload_filesize']) && $form_state['values']['upload_filesize'] > file_upload_max_size()) {
        form_set_error('upload_filesize', t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))));
    }
    if (!is_numeric($form_state['values']['upload_usersize']) || $form_state['values']['upload_usersize'] < 0) {
        form_set_error('upload_usersize', t('A number greater than zero must be specified for the upload user space'));
      }
    if ($form_state['values']['upload_resolution'] != 0) {
      if (!preg_match('/^[0-9]+x[0-9]+$/', $form_state['values']['upload_max_resolution'])) {
        form_set_error('upload_max_resolution', t('The maximum allowed image resolution should be expressed as WIDTHxHEIGHT (e.g. 640x480).'));
      }
    }
  }
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit_drawer_status') {
      if (!$form_state['values']['drawer_status']) {
        form_set_error('drawer_status',t('A value must be specified for the drawer status'));
      }
      if ($form_state['values']['drawer_status'] != strip_tags($form_state['values']['drawer_status'])) {
        form_set_error('drawer_status',t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit_folder_status') {
      if (!$form_state['values']['folder_status']) {
        form_set_error('folder_status',t('A value must be specified for the folder status'));
      }
      if ($form_state['values']['folder_status'] != strip_tags($form_state['values']['folder_status'])) {
        form_set_error('folder_status',t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit_document_status') {
      if (!$form_state['values']['document_status']) {
        form_set_error('document_status',t('A value must be specified for the document status'));
      }
      if ($form_state['values']['document_status'] != strip_tags($form_state['values']['document_status'])) {
        form_set_error('document_status',t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit_document_type') {
      if (!$form_state['values']['document_type']) {
        form_set_error('document_type',t('A value must be specified for the document type'));
      }
      if ($form_state['values']['document_type'] != strip_tags($form_state['values']['document_type'])) {
        form_set_error('document_type',t('HTML or PHP tags are not permitted in this field'));
      }
    }
  }
}

function filecabinet_admin_form_submit($form, &$form_state) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_admin_form_submit',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('filecabinet','clicked button='.$clicked_button, 6);
  // check for clicked button in default drawer owner fieldset
  if ($clicked_button == $form['default_owner']['default_owner_submit']['#value']) {
    variable_set('filecabinet_default_owner_uid', $form_state['storage']['ouid']);
    $message = t('The default drawer owner has been set.');
  }
  // check for clicked button in default mail capture fieldset
  elseif ($clicked_button == $form['mail_capture']['default_mail_submit']['#value']) {
    variable_set('filecabinet_default_mail_fnid', $form_state['storage']['fnid']);
    variable_set('filecabinet_default_mail_mnid', $form_state['storage']['mnid']);
    variable_set('filecabinet_default_mail_doctype', $form_state['storage']['doctype']);
    variable_set('filecabinet_default_mail_docstatus', $form_state['storage']['docstatus']);
    $message = t('The default mail capture values have been set.');
  }
  elseif ($clicked_button == $form['drawer_statuses']['drawer_status_add']['#value']) {
    //orgright_debug_msg('filecabinet','new drawer status='.$form_state['values']['drawer_status_new'],8);
    $drawer_status_list = filecabinet_get_drawer_statuses();
    $drawer_status_list[] = $form_state['values']['drawer_status_new'];
    variable_set('filecabinet_drawer_statuses', array_values($drawer_status_list));
    $message = t('The drawer status has been added.');
  }
  elseif ($clicked_button == $form['drawer_statuses']['drawer_status_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['drawer_statuses']['drawer_status_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['folder_statuses']['folder_status_add']['#value']) {
    //orgright_debug_msg('filecabinet','new folder status='.$form_state['values']['folder_status_new'],8);
    $folder_status_list = filecabinet_get_folder_statuses();
    $folder_status_list[] = $form_state['values']['folder_status_new'];
    variable_set('filecabinet_folder_statuses', array_values($folder_status_list));
    $message = t('The folder status has been added.');
  }
  elseif ($clicked_button == $form['folder_statuses']['folder_status_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['folder_statuses']['folder_status_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['document_statuses']['document_status_add']['#value']) {
    //orgright_debug_msg('filecabinet','new document status='.$form_state['values']['document_status_new'],8);
    $document_status_list = filecabinet_get_document_statuses();
    $document_status_list[] = $form_state['values']['document_status_new'];
    variable_set('filecabinet_document_statuses', array_values($document_status_list));
    $message = t('The document status has been added.');
  }
  elseif ($clicked_button == $form['document_statuses']['document_status_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['document_statuses']['document_status_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['document_types']['document_type_add']['#value']) {
    //orgright_debug_msg('filecabinet','new document type='.$form_state['values']['document_type_new'],8);
    $document_type_list = filecabinet_get_document_types();
    $document_type_list[] = $form_state['values']['document_type_new'];
    variable_set('filecabinet_document_types', array_values($document_type_list));
    $message = t('The document type has been added.');
  }
  elseif ($clicked_button == $form['document_types']['document_type_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['document_types']['document_type_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['doclink_nodetypes']['doclink_nodetype_submit']['#value']) {
    // The submit button for doclink target node types has been clicked
    //orgright_debug_dump('doclink formstate',$form_state);
    $nodetype_list = array();
    foreach ($form_state['values']['doclink_nodetype_list'] as $nodetype) {
      if ($nodetype) { $nodetype_list[] = $nodetype; }
    }
    variable_set('filecabinet_doclink_nodetypes',array_values($nodetype_list));
    $message = t('The list of target node types has been saved');
  }
  // Process doclink linktype fieldset
  elseif ($clicked_button == $form['doclink_linktypes']['doclink_linktype_add']['#value']) {
    //orgright_debug_msg('filecabinet','new doclink type='.$form_state['values']['doclink_linktype_new'],8);
    $doclink_linktype_list = filecabinet_get_doclink_linktypes();
    $doclink_linktype_list[] = $form_state['values']['doclink_linktype_new'];
    variable_set('filecabinet_doclink_linktypes',array_values($doclink_linktype_list));
    $message = t('The document link type has been added.');
  }
  elseif ($clicked_button == $form['doclink_linktypes']['doclink_linktype_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['doclink_linktypes']['doclink_linktype_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['upload_settings']['upload_settings_submit']['#value']) {
    variable_set('filecabinet_upload_extensions', $form_state['values']['upload_extensions']);
    variable_set('filecabinet_upload_filesize', $form_state['values']['upload_filesize']);
    variable_set('filecabinet_upload_usersize', $form_state['values']['upload_usersize']);
    variable_set('filecabinet_upload_resolution', $form_state['values']['upload_resolution']);
    $message = t('The file upload values have been set.');
  }
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit_drawer_status') {
      $drawer_status_list = filecabinet_get_drawer_statuses();
      $this_drawer_status = $form_state['storage']['this_drawer_status'];
      $drawer_status_list[$this_drawer_status] = $form_state['values']['drawer_status'];
      variable_set('filecabinet_drawer_statuses',array_values($drawer_status_list));
      $message = t('The drawer status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete_drawer_status') {
      $drawer_status_list = filecabinet_get_drawer_statuses();
      $this_drawer_status = $form_state['storage']['this_drawer_status'];
      unset($drawer_status_list[$this_drawer_status]);
      variable_set('filecabinet_drawer_statuses',array_values($drawer_status_list));
      $message = t('The drawer status has been deleted.');
    }
    elseif ($form_state['storage']['operation'] == 'edit_folder_status') {
      $folder_status_list = filecabinet_get_folder_statuses();
      $this_folder_status = $form_state['storage']['this_folder_status'];
      $folder_status_list[$this_folder_status] = $form_state['values']['folder_status'];
      variable_set('filecabinet_folder_statuses',array_values($folder_status_list));
      $message = t('The folder status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete_folder_status') {
      $folder_status_list = filecabinet_get_folder_statuses();
      $this_folder_status = $form_state['storage']['this_folder_status'];
      unset($folder_status_list[$this_folder_status]);
      variable_set('filecabinet_folder_statuses',array_values($folder_status_list));
      $message = t('The folder status has been deleted.');
    }
    elseif ($form_state['storage']['operation'] == 'edit_document_status') {
      $document_status_list = filecabinet_get_document_statuses();
      $this_document_status = $form_state['storage']['this_document_status'];
      $document_status_list[$this_document_status] = $form_state['values']['document_status'];
      variable_set('filecabinet_document_statuses',array_values($document_status_list));
      $message = t('The document status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete_document_status') {
      $document_status_list = filecabinet_get_document_statuses();
      $this_document_status = $form_state['storage']['this_document_status'];
      unset($document_status_list[$this_document_status]);
      variable_set('filecabinet_document_statuses',array_values($document_status_list));
      $message = t('The document status has been deleted.');
    }
    elseif ($form_state['storage']['operation'] == 'edit_document_type') {
      $document_type_list = filecabinet_get_document_types();
      $this_document_type = $form_state['storage']['this_document_type'];
      $document_type_list[$this_document_type] = $form_state['values']['document_type'];
      variable_set('filecabinet_document_types',array_values($document_type_list));
      $message = t('The document type has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete_document_type') {
      $document_type_list = filecabinet_get_document_types();
      $this_document_type = $form_state['storage']['this_document_type'];
      unset($document_type_list[$this_document_type]);
      variable_set('filecabinet_document_types',array_values($document_type_list));
      $message = t('The document type has been deleted.');
    }
    elseif ($form_state['storage']['operation'] == 'edit_doclink_linktype') {
      $doclink_linktype_list = filecabinet_get_doclink_linktypes();
      $this_doclink_linktype = $form_state['storage']['this_doclink_linktype'];
      $doclink_linktype_list[$this_doclink_linktype] = $form_state['values']['doclink_linktype'];
      variable_set('filecabinet_doclink_linktypes',array_values($doclink_linktype_list));
      $message = t('The document link type has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete_doclink_linktype') {
      $doclink_linktype_list = filecabinet_get_doclink_linktypes();
      $this_doclink_linktype = $form_state['storage']['this_doclink_linktype'];
      unset($doclink_linktype_list[$this_doclink_linktype]);
      variable_set('filecabinet_doclink_linktypes',array_values($doclink_linktype_list));
      $message = t('The document link type has been deleted.');
    }
    else {
      // unknown operation
    }
  }
  drupal_set_message($message,'status');
  $form_state['storage'] = array();
  $form_state['redirect'] = 'admin/orgright/filecabinet';
}
