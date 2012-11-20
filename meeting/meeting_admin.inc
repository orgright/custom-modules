<?php
// $Id$

/**
 * @file
 * Provides administration functions for meeting module.
 */

function meeting_admin() {
  //orgright_debug_msg('meeting','Fn: meeting_admin',5);
  return drupal_get_form('meeting_admin_form');
}

/**
 * Build system settings form
 * @ingroup forms
 * @see system_settings_form()
 */
function meeting_admin_form_build(&$form_state = NULL) {
  //orgright_debug_msg('meeting','Fn: meeting_admin_form_build',5);
  $form = array();
  // Check to see whether this is a new form, or a rebuild
  $form_state['storage']['new_form'] = TRUE;
  if (isset($form_state['values'])) {
    // The form has been submitted for edit or delete of a value
    $form_state['storage']['new_form'] = FALSE;
  }
  if ($form_state['storage']['new_form']) {
    // This is a new form request

    // Now specify the "meetingtypes" fieldset
    $meetingtype_list = meeting_get_meeting_types();
    $form['meetingtypes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meeting Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['meetingtypes']['meetingtype_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current meeting types'),
      '#description' => t('Select a type to edit or delete'),
      '#options' => $meetingtype_list,
      '#weight' => 1,
    );
    $form['meetingtypes']['meetingtype_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit meeting type'),
      '#weight' => 2,
    );
    $form['meetingtypes']['meetingtype_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete meeting type'),
      '#weight' => 3,
    );
    $form['meetingtypes']['meetingtype_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Meeting type'),
      '#description' => t('Specify a new meeting type to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['meetingtypes']['meetingtype_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add meeting type'),
      '#weight' => 5,
    );
    // Now specify the "meetingstatus" fieldset
    $meetingstatus_list = meeting_get_meeting_statuses();
    $form['meetingstatuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meeting Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['meetingstatuses']['meetingstatus_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current meeting status values'),
      '#description' => t('Select a status to edit or delete'),
      '#options' => $meetingstatus_list,
      '#weight' => 1,
    );
    $form['meetingstatuses']['meetingstatus_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit meeting status'),
      '#weight' => 2,
    );
    $form['meetingstatuses']['meetingstatus_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete meeting status'),
      '#weight' => 3,
    );
    $form['meetingstatuses']['meetingstatus_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Meeting status'),
      '#description' => t('Specify a new meeting status to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['meetingstatuses']['meetingstatus_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add meeting status'),
      '#weight' => 5,
    );
    // Now specify the "agendaitemstatus" fieldset
    $mtgitemstatus_list = meeting_get_agenda_item_statuses();
    $form['agendaitemstatuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Agenda Item Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['agendaitemstatuses']['agendaitemstatus_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current agenda item status values'),
      '#description' => t('Select a status to edit or delete'),
      '#options' => $mtgitemstatus_list,
      '#weight' => 1,
    );
    $form['agendaitemstatuses']['agendaitemstatus_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit agenda item status'),
      '#weight' => 2,
    );
    $form['agendaitemstatuses']['agendaitemstatus_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete agenda item status'),
      '#weight' => 3,
    );
    $form['agendaitemstatuses']['agendaitemstatus_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Agenda item status'),
      '#description' => t('Specify a new agenda item status to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['agendaitemstatuses']['agendaitemstatus_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add agenda item status'),
      '#weight' => 5,
    );
    // Now specify the "mtgitemtypes" fieldset
    $mtgitemtype_list = meeting_get_mtg_item_types();
    $form['mtgitemtypes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meeting Item Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['mtgitemtypes']['mtgitemtype_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current meeting item types'),
      '#description' => t('Select a type to edit or delete'),
      '#options' => $mtgitemtype_list,
      '#weight' => 1,
    );
    $form['mtgitemtypes']['mtgitemtype_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit meeting item type'),
      '#weight' => 2,
    );
    $form['mtgitemtypes']['mtgitemtype_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete meeting item type'),
      '#weight' => 3,
    );
    $form['mtgitemtypes']['mtgitemtype_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Meeting item type'),
      '#description' => t('Specify a new meeting item type to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['mtgitemtypes']['mtgitemtype_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add meeting item type'),
      '#weight' => 5,
    );
    // Now specify the "mtgitemstatus" fieldset
    $mtgitemstatus_list = meeting_get_mtg_item_statuses();
    $form['mtgitemstatuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meeting Item Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['mtgitemstatuses']['mtgitemstatus_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current meeting item status values'),
      '#description' => t('Select a status to edit or delete'),
      '#options' => $mtgitemstatus_list,
      '#weight' => 1,
    );
    $form['mtgitemstatuses']['mtgitemstatus_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit meeting item status'),
      '#weight' => 2,
    );
    $form['mtgitemstatuses']['mtgitemstatus_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete meeting item status'),
      '#weight' => 3,
    );
    $form['mtgitemstatuses']['mtgitemstatus_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Meeting item status'),
      '#description' => t('Specify a new meeting item status to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['mtgitemstatuses']['mtgitemstatus_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add meeting item status'),
      '#weight' => 5,
    );
    // Specify the meeting status values for inclusion in a list
    $meetingstatuses = meeting_get_meeting_statuses();
    $selectedstatuses = meeting_get_meeting_list_statuses();
    $form['meetinglistatuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meeting Status Values Included in List'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['meetinglistatuses']['meetinglistatus_list'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Currently selected status values'),
      '#description' => t('Select meeting status values that will be included in meeting lists'),
      '#default_value' => $selectedstatuses,
      '#options' => $meetingstatuses,
      '#weight' => 1,
    );
    $form['meetinglistatuses']['unspecified_notice'] = array(
      '#prefix' => '<p>',
      '#value' => t('Note: meetings with unspecified status will always be included'),
      '#suffix' => '</p>',
      '#weight' => 2,
    );
    $form['meetinglistatuses']['meetinglistatus_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save include meeting status list'),
      '#weight' => 3,
    );
    // Specify the agenda item status values for inclusion in a list
    $agendaitemstatuses = meeting_get_agenda_item_statuses();
    $selectedstatuses = meeting_get_agenda_item_list_statuses();
    $form['agendaitemlistatuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Agenda Item Status Values Included in List'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['agendaitemlistatuses']['agendaitemlistatus_list'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Currently selected status values'),
      '#description' => t('Select agenda item status values that will be included in lists'),
      '#default_value' => $selectedstatuses,
      '#options' => $agendaitemstatuses,
      '#weight' => 1,
    );
    $form['agendaitemlistatuses']['unspecified_notice'] = array(
      '#prefix' => '<p>',
      '#value' => t('Note: agenda items with unspecified status will always be included'),
      '#suffix' => '</p>',
      '#weight' => 2,
    );
    $form['agendaitemlistatuses']['agendaitemlistatus_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save include agenda item status list'),
      '#weight' => 3,
    );
    // Specify the meeting item status values for inclusion in a list
    $mtgitemstatuses = meeting_get_mtg_item_statuses();
    $selectedstatuses = meeting_get_mtg_item_list_statuses();
    $form['mtgitemlistatuses'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meeting Item Status Values Included in List'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['mtgitemlistatuses']['mtgitemlistatus_list'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Currently selected status values'),
      '#description' => t('Select meeting item status values that will be included in lists'),
      '#default_value' => $selectedstatuses,
      '#options' => $mtgitemstatuses,
      '#weight' => 1,
    );
    $form['mtgitemlistatuses']['unspecified_notice'] = array(
      '#prefix' => '<p>',
      '#value' => t('Note: meeting items with unspecified status will always be included'),
      '#suffix' => '</p>',
      '#weight' => 2,
    );
    $form['mtgitemlistatuses']['mtgitemlistatus_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save include meeting item status list'),
      '#weight' => 3,
    );

  }
  else {
    // This is a form rebuild
    if (isset($form_state['storage']['this_meetingtype'])) {
      // Are either editing or deleting a meetingtype
      $meetingtype_list = meeting_get_meeting_types();
      $meetingtype_value = $meetingtype_list[$form_state['storage']['this_meetingtype']];
      //orgright_debug_msg('admin','admin form rebuild, meetingtype='.$meetingtype_value,7);
      if ($form_state['storage']['operation'] == 'edit meetingtype') {
        $form['meetingtype'] = array(
          '#type' => 'textfield',
          '#title' => t('Meeting type'),
          '#description' => t('Edit meeting type'),
          '#default_value' => $meetingtype_value,
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
        $form['meetingtype'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $meetingtype_value,
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
    elseif (isset($form_state['storage']['this_meetingstatus'])) {
      // Are either editing or deleting a meetingstatus
      $meetingstatus_list = meeting_get_meeting_statuses();
      $meetingstatus_value = $meetingstatus_list[$form_state['storage']['this_meetingstatus']];
      //orgright_debug_msg('admin','admin form rebuild, meetingstatus='.$meetingstatus_value,7);
      if ($form_state['storage']['operation'] == 'edit meetingstatus') {
        $form['meetingstatus'] = array(
          '#type' => 'textfield',
          '#title' => t('Meeting status'),
          '#description' => t('Edit meeting status'),
          '#default_value' => $meetingstatus_value,
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
        $form['meetingstatus'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $meetingstatus_value,
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
    elseif (isset($form_state['storage']['this_agendaitemstatus'])) {
      // Are either editing or deleting a agendaitemstatus
      $agendaitemstatus_list = meeting_get_agenda_item_statuses();
      $agendaitemstatus_value = $agendaitemstatus_list[$form_state['storage']['this_agendaitemstatus']];
      //orgright_debug_msg('admin','admin form rebuild, agendaitemstatus='.$agendaitemstatus_value,7);
      if ($form_state['storage']['operation'] == 'edit agendaitemstatus') {
        $form['agendaitemstatus'] = array(
          '#type' => 'textfield',
          '#title' => t('Agenda item status'),
          '#description' => t('Edit agenda item status'),
          '#default_value' => $agendaitemstatus_value,
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
        $form['agendaitemstatus'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $agendaitemstatus_value,
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
    elseif (isset($form_state['storage']['this_mtgitemtype'])) {
      // Are either editing or deleting a mtgitemtype
      $mtgitemtype_list = meeting_get_mtg_item_types();
      $mtgitemtype_value = $mtgitemtype_list[$form_state['storage']['this_mtgitemtype']];
      //orgright_debug_msg('admin','admin form rebuild, mtgitemtype='.$mtgitemtype_value,7);
      if ($form_state['storage']['operation'] == 'edit mtgitemtype') {
        $form['mtgitemtype'] = array(
          '#type' => 'textfield',
          '#title' => t('Meeting item type'),
          '#description' => t('Edit meeting item type'),
          '#default_value' => $mtgitemtype_value,
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
        $form['mtgitemtype'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $mtgitemtype_value,
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
    elseif (isset($form_state['storage']['this_mtgitemstatus'])) {
      // Are either editing or deleting a mtgitemstatus
      $mtgitemstatus_list = meeting_get_mtg_item_statuses();
      $mtgitemstatus_value = $mtgitemstatus_list[$form_state['storage']['this_mtgitemstatus']];
      //orgright_debug_msg('admin','admin form rebuild, mtgitemstatus='.$mtgitemstatus_value,7);
      if ($form_state['storage']['operation'] == 'edit mtgitemstatus') {
        $form['mtgitemstatus'] = array(
          '#type' => 'textfield',
          '#title' => t('Meeting item status'),
          '#description' => t('Edit meeting item status'),
          '#default_value' => $mtgitemstatus_value,
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
        $form['mtgitemstatus'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $mtgitemstatus_value,
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
      //orgright_debug_msg('meeting','ERROR - unknown admin form rebuild requested',3);
    }
  }
  $form['#validate'] = array('meeting_form_validate');
  $form['#submit'] = array('meeting_form_submit');
  //return system_settings_form($form);
  return $form;
}

function meeting_form_meeting_admin_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_form_meeting_admin_form_alter',5);
  // Remove submit and reset buttons
  unset($form['buttons']['submit']);
  unset($form['buttons']['reset']);
}

function meeting_admin_form_validate($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_admin_form_validate',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('meeting','clicked button='.$clicked_button, 6);

  // ****** meeting types ******
  if ($clicked_button == $form['meetingtypes']['meetingtype_add']['#value']) {
    //orgright_debug_msg('meeting','new meetingtype='.$form_state['values']['meetingtype_new'],8);
    if (!$form_state['values']['meetingtype_new']) {
      form_set_error('meetingtype_new', t('A value must be specified for the new meeting type'));
    }
    if ($form_state['values']['meetingtype_new'] != strip_tags($form_state['values']['meetingtype_new'])) {
      form_set_error('meetingtype_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['meetingtypes']['meetingtype_edit']['#value']) {
    $form_state['storage']['this_meetingtype'] = $form_state['values']['meetingtype_list'];
    $form_state['storage']['operation'] = 'edit meetingtype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['meetingtypes']['meetingtype_delete']['#value']) {
    $form_state['storage']['this_meetingtype'] = $form_state['values']['meetingtype_list'];
    $form_state['storage']['operation'] = 'delete meetingtype';
    $form_state['rebuild'] = TRUE;
  } // ****** meeting status ******
  elseif ($clicked_button == $form['meetingstatuses']['meetingstatus_add']['#value']) {
    //orgright_debug_msg('meeting','new meetingstatus='.$form_state['values']['meetingstatus_new'],8);
    if (!$form_state['values']['meetingstatus_new']) {
      form_set_error('meetingstatus_new', t('A value must be specified for the new meeting status'));
    }
    if ($form_state['values']['meetingstatus_new'] != strip_tags($form_state['values']['meetingstatus_new'])) {
      form_set_error('meetingstatus_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['meetingstatuses']['meetingstatus_edit']['#value']) {
    $form_state['storage']['this_meetingstatus'] = $form_state['values']['meetingstatus_list'];
    $form_state['storage']['operation'] = 'edit meetingstatus';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['meetingstatuses']['meetingstatus_delete']['#value']) {
    $form_state['storage']['this_meetingstatus'] = $form_state['values']['meetingstatus_list'];
    $form_state['storage']['operation'] = 'delete meetingstatus';
    $form_state['rebuild'] = TRUE;
  } // ****** agenda item status ******
  elseif ($clicked_button == $form['agendaitemstatuses']['agendaitemstatus_add']['#value']) {
    //orgright_debug_msg('meeting','new agendaitemstatus='.$form_state['values']['agendaitemstatus_new'],8);
    if (!$form_state['values']['agendaitemstatus_new']) {
      form_set_error('agendaitemstatus_new', t('A value must be specified for the new agenda item status'));
    }
    if ($form_state['values']['agendaitemstatus_new'] != strip_tags($form_state['values']['agendaitemstatus_new'])) {
      form_set_error('agendaitemstatus_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['agendaitemstatuses']['agendaitemstatus_edit']['#value']) {
    $form_state['storage']['this_agendaitemstatus'] = $form_state['values']['agendaitemstatus_list'];
    $form_state['storage']['operation'] = 'edit agendaitemstatus';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['agendaitemstatuses']['agendaitemstatus_delete']['#value']) {
    $form_state['storage']['this_agendaitemstatus'] = $form_state['values']['agendaitemstatus_list'];
    $form_state['storage']['operation'] = 'delete agendaitemstatus';
    $form_state['rebuild'] = TRUE;
  } // ****** meeting item type ******
  elseif ($clicked_button == $form['mtgitemtypes']['mtgitemtype_add']['#value']) {
    //orgright_debug_msg('meeting','new mtgitemtype='.$form_state['values']['mtgitemtype_new'],8);
    if (!$form_state['values']['mtgitemtype_new']) {
      form_set_error('mtgitemtype_new', t('A value must be specified for the new meeting item type'));
    }
    if ($form_state['values']['mtgitemtype_new'] != strip_tags($form_state['values']['mtgitemtype_new'])) {
      form_set_error('mtgitemtype_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['mtgitemtypes']['mtgitemtype_edit']['#value']) {
    $form_state['storage']['this_mtgitemtype'] = $form_state['values']['mtgitemtype_list'];
    $form_state['storage']['operation'] = 'edit mtgitemtype';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['mtgitemtypes']['mtgitemtype_delete']['#value']) {
    $form_state['storage']['this_mtgitemtype'] = $form_state['values']['mtgitemtype_list'];
    $form_state['storage']['operation'] = 'delete mtgitemtype';
    $form_state['rebuild'] = TRUE;
  } // ****** meeting item status ******
  elseif ($clicked_button == $form['mtgitemstatuses']['mtgitemstatus_add']['#value']) {
    //orgright_debug_msg('meeting','new mtgitemstatus='.$form_state['values']['mtgitemstatus_new'],8);
    if (!$form_state['values']['mtgitemstatus_new']) {
      form_set_error('mtgitemstatus_new', t('A value must be specified for the new meeting item status'));
    }
    if ($form_state['values']['mtgitemstatus_new'] != strip_tags($form_state['values']['mtgitemstatus_new'])) {
      form_set_error('mtgitemstatus_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['mtgitemstatuses']['mtgitemstatus_edit']['#value']) {
    $form_state['storage']['this_mtgitemstatus'] = $form_state['values']['mtgitemstatus_list'];
    $form_state['storage']['operation'] = 'edit mtgitemstatus';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['mtgitemstatuses']['mtgitemstatus_delete']['#value']) {
    $form_state['storage']['this_mtgitemstatus'] = $form_state['values']['mtgitemstatus_list'];
    $form_state['storage']['operation'] = 'delete mtgitemstatus';
    $form_state['rebuild'] = TRUE;
  } // ****** meeting list inclusion status specification ******
  elseif ($clicked_button == $form['meetinglistatuses']['meetinglistatus_submit']['#value']) {
    // no validation required
  } // ****** meeting list inclusion status specification ******
  elseif ($clicked_button == $form['agendaitemlistatuses']['agendaitemlistatus_submit']['#value']) {
    // no validation required
  } // ****** meeting list inclusion status specification ******
  elseif ($clicked_button == $form['mtgitemlistatuses']['mtgitemlistatus_strtolower(submit']['#value']) {
    // no validation required
  } // ****** other processing ******
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit meetingtype') {
      if (!$form_state['values']['meetingtype']) {
        form_set_error('meetingtype', t('A value must be specified for the meeting type'));
      }
      if ($form_state['values']['meetingtype'] != strip_tags($form_state['values']['meetingtype'])) {
        form_set_error('meetingtype', t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit mtgitemtype') {
      if (!$form_state['values']['mtgitemtype']) {
        form_set_error('mtgitemtype', t('A value must be specified for the meeting item type'));
      }
      if ($form_state['values']['mtgitemtype'] != strip_tags($form_state['values']['mtgitemtype'])) {
        form_set_error('mtgitemtype', t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit mtgitemstatus') {
      if (!$form_state['values']['mtgitemstatus']) {
        form_set_error('mtgitemstatus', t('A value must be specified for the meeting item status'));
      }
      if ($form_state['values']['mtgitemstatus'] != strip_tags($form_state['values']['mtgitemstatus'])) {
        form_set_error('mtgitemstatus', t('HTML or PHP tags are not permitted in this field'));
      }
    }
  }
}

function meeting_admin_form_submit($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_admin_form_submit',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('meeting','clicked button='.$clicked_button, 6);

  // ****** meeting types ******
  if ($clicked_button == $form['meetingtypes']['meetingtype_add']['#value']) {
    //orgright_debug_msg('meeting','new meetingtype='.$form_state['values']['meetingtype_new'],8);
    $meetingtype_list = meeting_get_meeting_types();
    $meetingtype_list[] = $form_state['values']['meetingtype_new'];
    variable_set('meeting_meeting_types', array_values($meetingtype_list));
    $message = t('The meeting type has been added.');
  }
  elseif ($clicked_button == $form['meetingtypes']['meetingtype_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['meetingtypes']['meetingtype_delete']['#value']) {
    return;
  } // ****** meeting status ******
  elseif ($clicked_button == $form['meetingstatuses']['meetingstatus_add']['#value']) {
    //orgright_debug_msg('meeting','new meetingstatus='.$form_state['values']['meetingstatus_new'],8);
    $meetingstatus_list = meeting_get_meeting_statuses();
    $meetingstatus_list[] = $form_state['values']['meetingstatus_new'];
    variable_set('meeting_meeting_statuses', array_values($meetingstatus_list));
    $message = t('The meeting status has been added.');
  }
  elseif ($clicked_button == $form['meetingstatuses']['meetingstatus_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['meetingstatuses']['meetingstatus_delete']['#value']) {
    return;
  } // ****** meeting agenda status ******
  elseif ($clicked_button == $form['agendaitemstatuses']['agendaitemstatus_add']['#value']) {
    //orgright_debug_msg('meeting','new agendaitemstatus='.$form_state['values']['agendaitemstatus_new'],8);
    $agendaitemstatus_list = meeting_get_agenda_item_statuses();
    $agendaitemstatus_list[] = $form_state['values']['agendaitemstatus_new'];
    variable_set('meeting_agenda_item_statuses', array_values($agendaitemstatus_list));
    $message = t('The agenda item status has been added.');
  }
  elseif ($clicked_button == $form['agendaitemstatuses']['agendaitemstatus_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['agendaitemstatuses']['agendaitemstatus_delete']['#value']) {
    return;
  } // ****** meeting item types ******
  elseif ($clicked_button == $form['mtgitemtypes']['mtgitemtype_add']['#value']) {
    //orgright_debug_msg('meeting','new mtgitemtype='.$form_state['values']['mtgitemtype_new'],8);
    $mtgitemtype_list = meeting_get_mtg_item_types();
    $mtgitemtype_list[] = $form_state['values']['mtgitemtype_new'];
    variable_set('meeting_mtg_item_types', array_values($mtgitemtype_list));
    $message = t('The meeting item type has been added.');
  }
  elseif ($clicked_button == $form['mtgitemtypes']['mtgitemtype_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['mtgitemtypes']['mtgitemtype_delete']['#value']) {
    return;
  } // ****** meeting item status ******
  elseif ($clicked_button == $form['mtgitemstatuses']['mtgitemstatus_add']['#value']) {
    //orgright_debug_msg('meeting','new mtgitemstatus='.$form_state['values']['mtgitemstatus_new'],8);
    $mtgitemstatus_list = meeting_get_mtg_item_statuses();
    $mtgitemstatus_list[] = $form_state['values']['mtgitemstatus_new'];
    variable_set('meeting_mtg_item_statuses', array_values($mtgitemstatus_list));
    $message = t('The meeting item status has been added.');
  }
  elseif ($clicked_button == $form['mtgitemstatuses']['mtgitemstatus_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['mtgitemstatuses']['mtgitemstatus_delete']['#value']) {
    return;
  } // ****** meeting list inclusion status specification ******
  elseif ($clicked_button == $form['meetinglistatuses']['meetinglistatus_submit']['#value']) {
    $status_list = array();
    foreach ($form_state['values']['meetinglistatus_list'] as $status) {
      if ($status) {
        $status_list[] = $status;
      }
    }
    variable_set('meeting_meeting_list_statuses', array_values($status_list));
    $message = t('The list of included meeting status values has been saved');
  } // ****** agenda item list inclusion status specification ******
  elseif ($clicked_button == $form['agendaitemlistatuses']['agendaitemlistatus_submit']['#value']) {
    $status_list = array();
    foreach ($form_state['values']['agendaitemlistatus_list'] as $status) {
      if ($status) {
        $status_list[] = $status;
      }
    }
    variable_set('meeting_agenda_item_list_statuses', array_values($status_list));
    $message = t('The list of included agenda item status values has been saved');
  } // ****** meeting item list inclusion status specification ******
  elseif ($clicked_button == $form['mtgitemlistatuses']['mtgitemlistatus_submit']['#value']) {
    $status_list = array();
    foreach ($form_state['values']['mtgitemlistatus_list'] as $status) {
      if ($status) {
        $status_list[] = $status;
      }
    }
    variable_set('meeting_mtg_item_list_statuses', array_values($status_list));
    $message = t('The list of included meeting item status values has been saved');
  } // The save changes or confirm deletion button has been clicked
  elseif ($clicked_button == $form['submit']['#value']) {

    // ****** meeting type ******
    if ($form_state['storage']['operation'] == 'edit meetingtype') {
      $meetingtype_list = meeting_get_meeting_types();
      $this_meetingtype = $form_state['storage']['this_meetingtype'];
      $meetingtype_list[$this_meetingtype] = $form_state['values']['meetingtype'];
      variable_set('meeting_meeting_types', array_values($meetingtype_list));
      $message = t('The meeting type has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete meetingtype') {
      $meetingtype_list = meeting_get_meeting_types();
      $this_meetingtype = $form_state['storage']['this_meetingtype'];
      unset($meetingtype_list[$this_meetingtype]);
      variable_set('meeting_meeting_types', array_values($meetingtype_list));
      $message = t('The meeting type has been deleted.');
    } // ****** meeting status ******
    elseif ($form_state['storage']['operation'] == 'edit meetingstatus') {
      $meetingstatus_list = meeting_get_meeting_statuses();
      $this_meetingstatus = $form_state['storage']['this_meetingstatus'];
      $meetingstatus_list[$this_meetingstatus] = $form_state['values']['meetingstatus'];
      variable_set('meeting_meeting_statuses', array_values($meetingstatus_list));
      $message = t('The meeting status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete meetingstatus') {
      $meetingstatus_list = meeting_get_meeting_statuses();
      $this_meetingstatus = $form_state['storage']['this_meetingstatus'];
      unset($meetingstatus_list[$this_meetingstatus]);
      variable_set('meeting_meeting_statuses', array_values($meetingstatus_list));
      $message = t('The meeting status has been deleted.');
    } // ****** agenda item status ******
    elseif ($form_state['storage']['operation'] == 'edit agendaitemstatus') {
      $agendaitemstatus_list = meeting_get_agenda_item_statuses();
      $this_agendaitemstatus = $form_state['storage']['this_agendaitemstatus'];
      $agendaitemstatus_list[$this_agendaitemstatus] = $form_state['values']['agendaitemstatus'];
      variable_set('meeting_agenda_item_statuses', array_values($agendaitemstatus_list));
      $message = t('The agenda item status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete agendaitemstatus') {
      $agendaitemstatus_list = meeting_get_agenda_item_statuses();
      $this_agendaitemstatus = $form_state['storage']['this_agendaitemstatus'];
      unset($agendaitemstatus_list[$this_agendaitemstatus]);
      variable_set('meeting_agenda_item_statuses', array_values($agendaitemstatus_list));
      $message = t('The agenda item status has been deleted.');
    } // ****** meeting item type ******
    elseif ($form_state['storage']['operation'] == 'edit mtgitemtype') {
      $mtgitemtype_list = meeting_get_mtg_item_types();
      $this_mtgitemtype = $form_state['storage']['this_mtgitemtype'];
      $mtgitemtype_list[$this_mtgitemtype] = $form_state['values']['mtgitemtype'];
      variable_set('meeting_mtg_item_types', array_values($mtgitemtype_list));
      $message = t('The meeting item type has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete mtgitemtype') {
      $mtgitemtype_list = meeting_get_mtg_item_types();
      $this_mtgitemtype = $form_state['storage']['this_mtgitemtype'];
      unset($mtgitemtype_list[$this_mtgitemtype]);
      variable_set('meeting_mtg_item_types', array_values($mtgitemtype_list));
      $message = t('The meeting item type has been deleted.');
    } // ****** meeting item status ******
    elseif ($form_state['storage']['operation'] == 'edit mtgitemstatus') {
      $mtgitemstatus_list = meeting_get_mtg_item_statuses();
      $this_mtgitemstatus = $form_state['storage']['this_mtgitemstatus'];
      $mtgitemstatus_list[$this_mtgitemstatus] = $form_state['values']['mtgitemstatus'];
      variable_set('meeting_mtg_item_statuses', array_values($mtgitemstatus_list));
      $message = t('The meeting item status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete mtgitemstatus') {
      $mtgitemstatus_list = meeting_get_mtg_item_statuses();
      $this_mtgitemstatus = $form_state['storage']['this_mtgitemstatus'];
      unset($mtgitemstatus_list[$this_mtgitemstatus]);
      variable_set('meeting_mtg_item_statuses', array_values($mtgitemstatus_list));
      $message = t('The meeting item status has been deleted.');
    }
  }
  drupal_set_message($message, 'status');
  $form_state['storage'] = array();
  $form_state['redirect'] = 'admin/orgright/meeting';
}
