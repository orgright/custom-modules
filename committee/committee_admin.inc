<?php
// $Id$

/**
 * @file
 * Provides administration functions for committee module.
 */

function committee_admin() {
  //orgright_debug_msg('committee','Fn: committee_admin',5);
  return drupal_get_form('committee_admin_form');
}

/**
 * Build system settings form
 * @ingroup forms
 * @see system_settings_form()
 */
function committee_admin_form_build(&$form_state = NULL) {
  //orgright_debug_msg('committee','Fn: committee_admin_form_build',5);
  $form = array();
  // Check to see whether this is a new form, or a rebuild
  $form_state['storage']['new_form'] = TRUE;
  if (isset($form_state['values'])) {
    // The form has been submitted for edit or delete of a value
    $form_state['storage']['new_form'] = FALSE;
  }
  if ($form_state['storage']['new_form']) {
    // This is a new form request
    $form['appointtypes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Committee Appointment Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $appointtype_list = committee_get_appointtypes();
    $form['appointtypes']['appointtype_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current appointment types'),
      '#description' => t('Select an appointment type to edit or delete'),
      '#options' => $appointtype_list,
      '#weight' => 1,
    );
    $form['appointtypes']['appointtype_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit appointment type'),
      '#weight' => 2,
    );
    $form['appointtypes']['appointtype_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete appointment type'),
      '#weight' => 3,
    );
    $form['appointtypes']['appointtype_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Appointment type'),
      '#description' => t('Specify a new committee appointment type to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['appointtypes']['appointtype_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add appointment type'),
      '#weight' => 5,
    );
    // Now specify the "assigntypes" fieldset
    $form['assigntypes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Role Assignment Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $assigntype_list = committee_get_assigntypes();
    $form['assigntypes']['assigntype_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current role assignment types'),
      '#description' => t('Select an assignment type to edit or delete'),
      '#options' => $assigntype_list,
      '#weight' => 1,
    );
    $form['assigntypes']['assigntype_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit assignment type'),
      '#weight' => 2,
    );
    $form['assigntypes']['assigntype_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete assignment type'),
      '#weight' => 3,
    );
    $form['assigntypes']['assigntype_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Assignment type'),
      '#description' => t('Specify a new role assignment type to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['assigntypes']['assigntype_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add assignment type'),
      '#weight' => 5,
    );
  }
  else {
    // This is a form rebuild
    if (isset($form_state['storage']['this_appointtype'])) {
      // Are either editing or deleting an appointment type
      $appointtype_list = committee_get_appointtypes();
      $appointtype_value = $appointtype_list[$form_state['storage']['this_appointtype']];
      //orgright_debug_msg('admin','admin form rebuild, appointtype='.$appointtype_value,7);
      if ($form_state['storage']['operation'] == 'edit appointtype') {
        $form['appointtype'] = array(
          '#type' => 'textfield',
          '#title' => t('Appointment type'),
          '#description' => t('Edit committee appointment type'),
          '#default_value' => $appointtype_value,
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
        $form['appointtype'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $appointtype_value,
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
    elseif (isset($form_state['storage']['this_assigntype'])) {
      // Are either editing or deleting a role assignment type
      $assigntype_list = committee_get_assigntypes();
      $assigntype_value = $assigntype_list[$form_state['storage']['this_assigntype']];
      //orgright_debug_msg('admin','admin form rebuild, assigntype='.$assigntype_value,7);
      if ($form_state['storage']['operation'] == 'edit assigntype') {
        $form['assigntype'] = array(
          '#type' => 'textfield',
          '#title' => t('Assignment type'),
          '#description' => t('Edit role assignment type'),
          '#default_value' => $assigntype_value,
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
        $form['assigntype'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $assigntype_value,
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
      //orgright_debug_msg('committee','ERROR - unknown admin form rebuild requested',3);
    }
  }
  $form['#validate'] = array('committee_form_validate');
  $form['#submit'] = array('committee_form_submit');
  //return system_settings_form($form);
  return $form;
}

function committee_form_committee_admin_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_form_committee_admin_form_alter',5);
  // Remove submit and reset buttons
  unset($form['buttons']['submit']);
  unset($form['buttons']['reset']);
}

function committee_admin_form_validate($form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_admin_form_validate',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('committee','clicked button='.$clicked_button, 6);
  if ($clicked_button == $form['appointtypes']['appointtype_add']['#value']) {
    //orgright_debug_msg('committee','new appointtype='.$form_state['values']['appointtype_new'],8);
    if (!$form_state['values']['appointtype_new']) {
      form_set_error('appointtype_new', t('A value must be specified for the new appointment type'));
    }
    if ($form_state['values']['appointtype_new'] != strip_tags($form_state['values']['appointtype_new'])) {
      form_set_error('appointtype_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['appointtypes']['appointtype_edit']['#value']) {
    $form_state['storage']['this_appointtype'] = $form_state['values']['appointtype_list'];
    $form_state['storage']['operation'] = 'edit appointtype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['appointtype']['appointtype_delete']['#value']) {
    $form_state['storage']['this_appointtype'] = $form_state['values']['appointtype_list'];
    $form_state['storage']['operation'] = 'delete appointtype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['assigntypes']['assigntype_add']['#value']) {
    //orgright_debug_msg('committee','new assigntype='.$form_state['values']['assigntype_new'],8);
    if (!$form_state['values']['assigntype_new']) {
      form_set_error('assigntype_new', t('A value must be specified for the new assignment type'));
    }
    if ($form_state['values']['assigntype_new'] != strip_tags($form_state['values']['assigntype_new'])) {
      form_set_error('assigntype_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['assigntypes']['assigntype_edit']['#value']) {
    $form_state['storage']['this_assigntype'] = $form_state['values']['assigntype_list'];
    $form_state['storage']['operation'] = 'edit assigntype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['assigntypes']['assigntype_delete']['#value']) {
    $form_state['storage']['this_assigntype'] = $form_state['values']['assigntype_list'];
    $form_state['storage']['operation'] = 'delete assigntype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit appointtype') {
      if (!$form_state['values']['appointtype']) {
        form_set_error('appointtype', t('A value must be specified for the appointment type'));
      }
      if ($form_state['values']['appointtype'] != strip_tags($form_state['values']['appointtype'])) {
        form_set_error('appointtype', t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit assigntype') {
      if (!$form_state['values']['assigntype']) {
        form_set_error('assigntype', t('A value must be specified for the assignment type'));
      }
      if ($form_state['values']['assigntype'] != strip_tags($form_state['values']['assigntype'])) {
        form_set_error('assigntype', t('HTML or PHP tags are not permitted in this field'));
      }
    }
  }
}

function committee_admin_form_submit($form, &$form_state) {
  //orgright_debug_msg('committee','Fn: committee_admin_form_submit',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('committee','clicked button='.$clicked_button, 6);
  if ($clicked_button == $form['appointtypes']['appointtype_add']['#value']) {
    //orgright_debug_msg('committee','new appointtype='.$form_state['values']['appointtype_new'],8);
    $appointtype_list = committee_get_appointtypes();
    $appointtype_list[] = $form_state['values']['appointtype_new'];
    variable_set('committee_appointtypes', array_values($appointtype_list));
    $message = t('The committee appointment type has been added.');
  }
  elseif ($clicked_button == $form['appointtypes']['appointtype_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['appointtypes']['appointtype_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['assigntypes']['assigntype_add']['#value']) {
    //orgright_debug_msg('committee','new assigntype='.$form_state['values']['assigntype_new'],8);
    $assigntype_list = committee_get_assigntypes();
    $assigntype_list[] = $form_state['values']['assigntype_new'];
    variable_set('committee_assigntypes', array_values($assigntype_list));
    $message = t('The role assignment type has been added.');
  }
  elseif ($clicked_button == $form['assigntypes']['assigntype_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['assigntypes']['assigntype_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit appointtype') {
      $appointtype_list = committee_get_appointtypes();
      $this_appointtype = $form_state['storage']['this_appointtype'];
      $appointtype_list[$this_appointtype] = $form_state['values']['appointtype'];
      variable_set('committee_appointtypes', array_values($appointtype_list));
      $message = t('The committee appointment type has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete appointtype') {
      $appointtype_list = committee_get_appointtypes();
      $this_appointtype = $form_state['storage']['this_appointtype'];
      unset($appointtype_list[$this_appointtype]);
      variable_set('committee_appointtypes', array_values($appointtype_list));
      $message = t('The committee appointment type has been deleted.');
    }
    elseif ($form_state['storage']['operation'] == 'edit assigntype') {
      $assigntype_list = committee_get_assigntypes();
      $this_assigntype = $form_state['storage']['this_assigntype'];
      $assigntype_list[$this_assigntype] = $form_state['values']['assigntype'];
      variable_set('committee_assigntypes', array_values($assigntype_list));
      $message = t('The role assignment type has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete assigntype') {
      $assigntype_list = committee_get_assigntypes();
      $this_assigntype = $form_state['storage']['this_assigntype'];
      unset($assigntype_list[$this_assigntype]);
      variable_set('committee_assigntypes', array_values($assigntype_list));
      $message = t('The role assignment type has been deleted.');
    }
  }
  drupal_set_message($message, 'status');
  $form_state['storage'] = array();
  $form_state['redirect'] = 'admin/orgright/committee';
}

