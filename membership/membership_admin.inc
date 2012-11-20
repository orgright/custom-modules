<?php
// $Id$

/**
 * @file
 * Provides administration functions for membership module.
 */

function membership_admin() {
  //orgright_debug_msg('membership','Fn: membership_admin',5);
  //return '*** Membership administration function ***';
  return drupal_get_form('membership_admin_form');
}

/**
 * Build system settings form
 * @ingroup forms
 * @see system_settings_form()
 */
function membership_admin_form_build(&$form_state = NULL) {
  //orgright_debug_msg('membership','Fn: membership_admin_form_build',5);
  $form = array();
  // Check to see whether this is a new form, or a rebuild
  $form_state['storage']['new_form'] = TRUE;
  if (isset($form_state['values'])) {
    // The form has been submitted for edit or delete of a value
    $form_state['storage']['new_form'] = FALSE;
  }
  if ($form_state['storage']['new_form']) {
    // This is a new form request
    $form['categories'] = array(
      '#type' => 'fieldset',
      '#title' => t('Membership Categories'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $category_list = membership_get_categories();
    $form['categories']['category_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current categories'),
      '#description' => t('Select a category to edit or delete'),
      '#options' => $category_list,
      '#weight' => 1,
    );
    $form['categories']['category_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit category'),
      '#weight' => 2,
    );
    $form['categories']['category_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete category'),
      '#weight' => 3,
    );
    $form['categories']['category_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Category'),
      '#description' => t('Specify a new membership category to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['categories']['category_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add new category'),
      '#weight' => 5,
    );
    // Now specify the "standings" fieldset
    $form['standings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Membership Status Values'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $standing_list = membership_get_standings();
    $form['standings']['standing_list'] = array(
      '#type' => 'radios',
      '#title' => t('Current status values'),
      '#description' => t('Select a status to edit or delete'),
      '#options' => $standing_list,
      '#weight' => 1,
    );
    $form['standings']['standing_edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit status'),
      '#weight' => 2,
    );
    $form['standings']['standing_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete status'),
      '#weight' => 3,
    );
    $form['standings']['standing_new'] = array(
      '#type' => 'textfield',
      '#title' => t('Status'),
      '#description' => t('Specify a new membership status to add'),
      '#default_value' => '',
      '#maxlength' => 16,
      '#size' => 16,
      '#required' => FALSE,
      '#weight' => 4,
    );
    $form['standings']['standing_add'] = array(
      '#type' => 'submit',
      '#value' => t('Add new status'),
      '#weight' => 5,
    );
  }
  else {
    // This is a form rebuild
    if (isset($form_state['storage']['this_category'])) {
      // Are either editing or deleting a category
      $category_list = membership_get_categories();
      $category_value = $category_list[$form_state['storage']['this_category']];
      //orgright_debug_msg('admin','admin form rebuild, category='.$category_value,7);
      if ($form_state['storage']['operation'] == 'edit category') {
        $form['category'] = array(
          '#type' => 'textfield',
          '#title' => t('Category'),
          '#description' => t('Edit category'),
          '#default_value' => $category_value,
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
        $form['category'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $category_value,
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
    elseif (isset($form_state['storage']['this_standing'])) {
      // Are either editing or deleting a standing
      $standing_list = membership_get_standings();
      $standing_value = $standing_list[$form_state['storage']['this_standing']];
      //orgright_debug_msg('admin','admin form rebuild, standing='.$standing_value,7);
      if ($form_state['storage']['operation'] == 'edit standing') {
        $form['standing'] = array(
          '#type' => 'textfield',
          '#title' => t('Status'),
          '#description' => t('Edit membership status'),
          '#default_value' => $standing_value,
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
        $form['standing'] = array(
          '#type' => 'markup',
          '#prefix' => '<div class="highlight">',
          '#value' => $standing_value,
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
      //orgright_debug_msg('membership','ERROR - unknown admin form rebuild requested',3);
    }
  }
  $form['#validate'] = array('membership_form_validate');
  $form['#submit'] = array('membership_form_submit');
  //return system_settings_form($form);
  return $form;
}

function membership_form_membership_admin_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_form_membership_admin_form_alter',5);
  // Remove submit and reset buttons
  unset($form['buttons']['submit']);
  unset($form['buttons']['reset']);
}

function membership_admin_form_validate($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_admin_form_validate',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('membership','clicked button='.$clicked_button, 6);
  if ($clicked_button == $form['categories']['category_add']['#value']) {
    //orgright_debug_msg('membership','new category='.$form_state['values']['category_new'],8);
    if (!$form_state['values']['category_new']) {
      form_set_error('category_new', t('A value must be specified for the new category'));
    }
    if ($form_state['values']['category_new'] != strip_tags($form_state['values']['category_new'])) {
      form_set_error('category_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['categories']['category_edit']['#value']) {
    $form_state['storage']['this_category'] = $form_state['values']['category_list'];
    $form_state['storage']['operation'] = 'edit category';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['categories']['category_delete']['#value']) {
    $form_state['storage']['this_category'] = $form_state['values']['category_list'];
    $form_state['storage']['operation'] = 'delete category';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['standings']['standing_add']['#value']) {
    //orgright_debug_msg('membership','new standing='.$form_state['values']['standing_new'],8);
    if (!$form_state['values']['standing_new']) {
      form_set_error('standing_new', t('A value must be specified for the new status'));
    }
    if ($form_state['values']['standing_new'] != strip_tags($form_state['values']['standing_new'])) {
      form_set_error('standing_new', t('HTML or PHP tags are not permitted in this field'));
    }
  }
  elseif ($clicked_button == $form['standings']['standing_edit']['#value']) {
    $form_state['storage']['this_standing'] = $form_state['values']['standing_list'];
    $form_state['storage']['operation'] = 'edit standing';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['standings']['standing_delete']['#value']) {
    $form_state['storage']['this_standing'] = $form_state['values']['standing_list'];
    $form_state['storage']['operation'] = 'delete standing';
    $form_state['rebuild'] = TRUE;
  }
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit category') {
      if (!$form_state['values']['category']) {
        form_set_error('category', t('A value must be specified for the status'));
      }
      if ($form_state['values']['category'] != strip_tags($form_state['values']['category'])) {
        form_set_error('category', t('HTML or PHP tags are not permitted in this field'));
      }
    }
    if ($form_state['storage']['operation'] == 'edit standing') {
      if (!$form_state['values']['standing']) {
        form_set_error('standing', t('A value must be specified for the status'));
      }
      if ($form_state['values']['standing'] != strip_tags($form_state['values']['standing'])) {
        form_set_error('standing', t('HTML or PHP tags are not permitted in this field'));
      }
    }
  }
}

function membership_admin_form_submit($form, &$form_state) {
  //orgright_debug_msg('membership','Fn: membership_admin_form_submit',5);
  $clicked_button = $form_state['clicked_button']['#value'];
  //orgright_debug_msg('membership','clicked button='.$clicked_button, 6);
  if ($clicked_button == $form['categories']['category_add']['#value']) {
    //orgright_debug_msg('membership','new category='.$form_state['values']['category_new'],8);
    $category_list = membership_get_categories();
    $category_list[] = $form_state['values']['category_new'];
    variable_set('membership_categories', array_values($category_list));
    $message = t('The membership category has been added.');
  }
  elseif ($clicked_button == $form['categories']['category_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['categories']['category_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['standings']['standing_add']['#value']) {
    //orgright_debug_msg('membership','new standing='.$form_state['values']['standing_new'],8);
    $standing_list = membership_get_standings();
    $standing_list[] = $form_state['values']['standing_new'];
    variable_set('membership_standings', array_values($standing_list));
    $message = t('The membership status has been added.');
  }
  elseif ($clicked_button == $form['standings']['standing_edit']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['standings']['standing_delete']['#value']) {
    return;
  }
  elseif ($clicked_button == $form['submit']['#value']) {
    // The save changes or confirm deletion button has been clicked
    if ($form_state['storage']['operation'] == 'edit category') {
      $category_list = membership_get_categories();
      $this_category = $form_state['storage']['this_category'];
      $category_list[$this_category] = $form_state['values']['category'];
      variable_set('membership_categories', array_values($category_list));
      $message = t('The membership category has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete category') {
      $category_list = membership_get_categories();
      $this_category = $form_state['storage']['this_category'];
      unset($category_list[$this_category]);
      variable_set('membership_categories', array_values($category_list));
      $message = t('The membership category has been deleted.');
    }
    elseif ($form_state['storage']['operation'] == 'edit standing') {
      $standing_list = membership_get_standings();
      $this_standing = $form_state['storage']['this_standing'];
      $standing_list[$this_standing] = $form_state['values']['standing'];
      variable_set('membership_standings', array_values($standing_list));
      $message = t('The membership status has been saved.');
    }
    elseif ($form_state['storage']['operation'] == 'delete standing') {
      $standing_list = membership_get_standings();
      $this_standing = $form_state['storage']['this_standing'];
      unset($standing_list[$this_standing]);
      variable_set('membership_standings', array_values($standing_list));
      $message = t('The membership status has been deleted.');
    }
  }
  drupal_set_message($message, 'status');
  $form_state['storage'] = array();
  $form_state['redirect'] = 'admin/orgright/membership';
}
