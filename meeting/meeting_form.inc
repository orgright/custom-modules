<?php
// $Id$

/**
 * @file
 * Provides additional functions for meeting module.
 */

/**
 * Functions called from hook_form() in meeting.module
 */
function meeting_meeting_form_build(&$form_state, $node) {
  //orgright_debug_msg('meeting','Fn: meeting_meeting_form_build',5);
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating meetings
  $form = array();
  $fix = 0;
  $fix++;
  $form['meeting'] = array(
    '#type' => 'textfield',
    '#title' => t('Meeting'),
    '#description' => meeting_field_description('meeting', 'meeting', t('Enter a reference for this meeting')),
    '#default_value' => $node->meeting,
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
      '#description' => meeting_field_description('meeting', 'committee', t('Enter the name of the committee')),
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
      '#description' => meeting_field_description('meeting', 'committee', t('Enter the name of the committee')),
      '#default_value' => $node->cnid,
      '#options' => $committees_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => t('Location'),
    '#description' => meeting_field_description('meeting', 'location', t('Specify the planned meeting location')),
    '#default_value' => $node->location,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
    '#autocomplete_path' => 'meeting/autocomplete/locations',
  );
  $fix++;
  $status_list = array(t('[unspecified]')) + meeting_get_meeting_statuses();
  $form['mtgstatus'] = array(
    '#type' => 'select',
    '#title' => t('Meeting status'),
    '#description' => meeting_field_description('meeting', 'mtgstatus', t('Select the meeting status')),
    '#default_value' => $node->mtgstatus,
    '#options' => $status_list,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $type_list = meeting_get_meeting_types();
  $form['mtgtype'] = array(
    '#type' => 'select',
    '#title' => t('Meeting type'),
    '#description' => meeting_field_description('meeting', 'mtgtype', t('Select the type of meeting')),
    '#default_value' => $node->mtgtype,
    '#options' => $type_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $mtgdate = ($node->mtgdate) ?  $node->mtgdate : orgright_get_date_ymd();
  $form['mtgdate'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Meeting date'),
    '#description' => meeting_field_description('meeting', 'mtgdate', t('Specify the meeting date')),
    '#default_value' => $mtgdate.'T00:00:00',
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#date_year_range' => '-2:+7',
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $tformat = 'h:i a'; // Specify the time format
  $start = ($node->start) ? $mtgdate.'T'.$node->start: '';
  $form['start'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Start time'),
    '#description' => meeting_field_description('meeting', 'start', t('Specify start time (hh:mm am/pm)')),
    '#default_value' => $start,
    '#date_format' => $tformat,
    //'#date_label_position' => 'within',
    '#date_increment' => 5,
    //'#maxlength' => 16,
    //'#size' => 16,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $finish = ($node->finish) ? $mtgdate.'T'.$node->finish: '';
  $form['finish'] = array(
    //'#type' => 'date_select',
    '#type' => 'date_popup',
    '#title' => t('Finish time'),
    '#description' => meeting_field_description('meeting', 'finish', t('Specify a target finish time')),
    '#default_value' => $finish,
    '#date_format' => $tformat,
    //'#date_label_position' => 'within',
    '#date_increment' => 5,
    //'#maxlength' => 16,
    //'#size' => 16,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => meeting_field_description('meeting', 'body', t('Describe the purpose and expected outcome of this meeting')),
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
    '#description' => meeting_field_description('meeting', 'notes', t('General notes about this meeting')),
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
  $form['#validate'] = array('meeting_form_validate');
  $form['#submit'] = array('meeting_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Meetings')=>'meeting/meetings'));
  return $form;
}

/*function meeting_template_form_build(&$form_state, $node) {
  // build a form for a meeting template, based on a standard meeting form
  $form = meeting_meeting_form_build($form_state, $node);
  // now make some changes
  // Meeting status should be fixed as 'template'
  $form['mtgstatus']['#default_value'] = 'template';
  $form['mtgstatus']['#options'] = array('template'=>'Meeting Template');
  $form['mtgstatus']['#required'] = TRUE;
  // Meeting date should be just some text notes
  $form['mtgdate']['#type'] = 'textfield';
  $form['mtgdate']['#description'] = t('Notes about the meeting date (max 10 chars)');
  $form['mtgdate']['#default_value'] = '';
  $form['mtgdate']['#maxlength'] = 10;
  $form['mtgdate']['#size'] = 10;
  $form['mtgdate']['#required'] = TRUE;
  // Add a submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create Template'
  );
  // Specify alternative validation and submit handlers
  $form['#validate'] = array('meeting_template_form_validate');
  $form['#submit'] = array('meeting_template_form_submit');
  // return customied form
  return $form;
}*/

function meeting_agenda_item_form_build(&$form_state, $node) {
  //orgright_debug_msg('meeting','Fn: meeting_agenda_item_form_build',6);
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating agenda items
  $form = array();
  $fix = 0;
  $fix++;
  $meetings_list = meeting_get_meetings('nid');
  if (count($meetings_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field
    $form['meeting'] = array(
      '#type' => 'textfield',
      '#title' => t('Meeting'),
      '#description' => meeting_field_description('agenda-item', 'meeting', t('Specify the meeting that this agenda item is for')),
      '#default_value' => $node->meeting,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'meeting/autocomplete/meetings',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['meeting'] = array(
      '#type' => 'select',
      '#title' => t('Meeting'),
      '#description' => meeting_field_description('agenda-item', 'meeting', t('Specify the meeting that this agenda item is for')),
      '#default_value' => $node->gnid,
      '#options' => array('-- select meeting --') + $meetings_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['agendaitem'] = array(
    '#type' => 'textfield',
    '#title' => t('Agenda item'),
    '#description' => meeting_field_description('agenda-item', 'adendaitem', t('Specify the agenda item')),
    '#default_value' => $node->agendaitem,
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  // Get an array of agenda_item sequence numbers
  $agenda_item_list = meeting_get_agenda_item_seqnos('nid');
  $form['sequence'] = array(
    '#type' => 'textfield',
    '#title' => t('Item number'),
    '#description' => meeting_field_description('agenda-item', 'sequence', t('Enter the agenda item number')),
    '#default_value' => $node->sequence,
    '#maxlength' => 9,
    '#size' => 9,
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  // Get an array of agenda_item status values
  $agitstatus_list = meeting_get_agenda_item_statuses();
  $form['agitstatus'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#description' => meeting_field_description('agenda-item', 'status', t('Select agenda item status')),
    '#default_value' => $node->agitstatus,
    '#options' => array('-- unspecified --') + $agitstatus_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => meeting_field_description('agenda-item', 'body', t('Describe the purpose of this agenda item')),
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
    '#description' => meeting_field_description('agenda-item', 'notes', t('General notes about this agenda item')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['gnid'] = array(
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
  $form['#validate'] = array('meeting_form_validate');
  $form['#submit'] = array('meeting_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Agenda items')=>'meeting/agenda_items'));
  return $form;
}

function meeting_mtg_item_form_build(&$form_state, $node) {
  //orgright_debug_msg('meeting','Fn: meeting_mtg_item_form_build');
  // Get metadata for this node type we use it for labeling title and body fields).
  $type = node_get_types('type', $node);
  // Specify the form for adding or updating meeting items
  $form = array();
  $fix = 0;
  $fix++;
  $meetings_list = meeting_get_meetings('nid');
  if (count($meetings_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field
    $form['meeting'] = array(
      '#type' => 'textfield',
      '#title' => t('Meeting'),
      '#description' => meeting_field_description('mtg-item', 'meeting', t('Meeting that this item derives from')),
      '#default_value' => $node->meeting,
      '#maxlength' => 64,
      '#size' => 40,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
      '#autocomplete_path' => 'meeting/autocomplete/meetings',
    );
  }
  else {
    // use a select type field with the values from the array
    $form['meeting'] = array(
      '#type' => 'select',
      '#title' => t('Meeting'),
      '#description' => meeting_field_description('mtg-item', 'meeting', t('Meeting that this item derives from')),
      '#default_value' => $node->gnid,
      '#options' => array('[no meeting]') + $meetings_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $form['itemname'] = array(
    '#type' => 'textfield',
    '#title' => t('Item'),
    '#description' => meeting_field_description('mtg-item', 'itemname', t('Title of this meeting item')),
    '#default_value' => $node->itemname,
    '#maxlength' => 255,
    '#size' => 40,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $members_list = membership_get_members('nid');
  if (count($committees_list) > orgright_get_selectfield_max()) {
    // too many entries, so use an autocomplete field
    $form['member'] = array(
      '#type' => 'textfield',
      '#title' => t('Member'),
      '#description' => meeting_field_description('mtg-item', 'member', t('Name of member responsible for this meeting item (if any)')),
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
      '#description' => meeting_field_description('mtg-item', 'member', t('Name of member responsible for this meeting item (if any)')),
      '#default_value' => $node->mnid,
      '#options' => array('-- Select member --') + $members_list,
      '#required' => FALSE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
  }
  $fix++;
  $type_list = meeting_get_mtg_item_types();
  $form['itemtype'] = array(
    '#type' => 'select',
    '#title' => t('Meeting item type'),
    '#description' => meeting_field_description('mtg-item', 'itemtype', t('Select type of meeting item')),
    '#default_value' => $node->itemtype,
    '#options' => $type_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $dformat = 'd M Y'; // Specify the date format
  $form['itemdate'] = array(
    '#type' => 'date_popup',
    '#title' => t('Completion date'),
    '#description' => meeting_field_description('mtg-item', 'itemdate', t('Enter date for completion or bringup')),
    '#default_value' => ($node->itemdate) ?  $node->itemdate : orgright_get_date_ymd(),
    '#date_format' => $dformat,
    //'#date_label_position' => 'within',
    '#date_year_range' => '-2:+7',
    '#required' => FALSE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $status_list = meeting_get_mtg_item_statuses();
  $form['itemstatus'] = array(
    '#type' => 'select',
    '#title' => t('Meeting item status'),
    '#description' => meeting_field_description('mtg-item', 'itemstatus', t('Select meeting item status')),
    '#default_value' => $node->itemstatus,
    '#options' => $status_list,
    '#required' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  $fix++;
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => check_plain($type->body_label),
    '#description' => meeting_field_description('mtg-item', 'body', t('Details of the meeting item')),
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
    '#description' => meeting_field_description('mtg-item', 'notes', t('General notes about this meeting item')),
    '#default_value' => $node->notes,
    '#rows' => 3,
    '#resizable' => TRUE,
    '#attributes' => array('tabindex'=>$fix),
    '#weight' => $fix,
  );
  // create placeholders to be filled in during validation
  $form['gnid'] = array(
    '#type' => 'value',
    '#value' => 0,
  );
  $form['mnid'] = array(
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
  $form['#validate'] = array('meeting_form_validate');
  $form['#submit'] = array('meeting_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Meeting items')=>'meeting/mtg_items'));
  return $form;
}

function meeting_selection_form_build() {
  //$myargs=func_get_args(); //orgright_debug_dump('meeting_selection_form_build',$myargs,'myargs');
  $form = array();
  return $form;
}

function meeting_template_use_form_build(&$form_state) {
  // This is a two part form to select a meeting template and then the
  // information needed to generate a meeting from it.
  $step = (isset($form_state['values']['step'])) ? $form_state['values']['step'] : 0;
  $step++;
  $fix = 0;
  $form = array();
  if ($step == 1) {
    $fix++;
    $template_list = meeting_get_meeting_templates('nid');
    $form['template'] = array(
      '#type' => 'select',
      '#title' => t('Template Meeting'),
      '#description' => meeting_field_description('template', 'meeting', t('Select the template meeting to use')),
      '#default_value' => 0,
      '#options' => array('-- select template --') + $template_list,
      '#required' => TRUE,
      '#attributes' => array('tabindex'=>$fix),
      '#weight' => $fix,
    );
    $submit_label = t('Next');
  }
  elseif ($step == 2) {
    // create a dummy node object
    $node = (object)orgright_create_node('meeting');
    // set default values
    $node->meeting = $form_state['storage']['template']['meeting'];
    $node->cnid = $form_state['storage']['template']['cnid'];
    $node->committee = $form_state['storage']['template']['committee'];
    $node->location = $form_state['storage']['template']['location'];
    $node->mtgtype = $form_state['storage']['template']['mtgtype'];
    $node->start = $form_state['storage']['template']['start'];
    $node->finish = $form_state['storage']['template']['finish'];
    $node->body = $form_state['storage']['template']['body'];
    $node->notes = $form_state['storage']['template']['notes'];
    // get a standard "add meeting" form
    $form = meeting_meeting_form_build($form_state, $node);
    // add a heading giving details of the template being used
    $form['template'] = array(
      '#prefix' => '<div class="form-item">',
      '#value' => t('Using meeting template <strong>"@tpl"</strong>', array('@tpl'=>$form_state['storage']['template']['meeting'])),
      '#suffix' => '</div>',
      '#weight' => -1,
    );
    $submit_label = t('Generate new meeting');
    $fix = $form['fix']['#value'];
  }
  elseif ($step == 3) {
    // add a heading giving details of the template being used
    $form['template'] = array(
      '#prefix' => '<div class="form-item">',
      '#value' => t('Using meeting template <strong>"@tpl"</strong>', array('@tpl'=>$form_state['storage']['template']['meeting'])),
      '#suffix' => '</div>',
      '#weight' => -1,
    );
    // now go through the attached agenda items
    $sql = "SELECT a.nid, a.sequence, a.agendaitem FROM {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.gnid = %d ORDER BY a.sequence";
    $result = db_query($sql, $form_state['storage']['template']['nid']);
    $iz = 0;
    while ($agenda_item = db_fetch_object($result)) {
      $fix++;
      $form['agitem_'.$iz] = array(
        '#type' => 'fieldset',
        '#collapsible' => FALSE,
        '#weight' => $fix,
      );
      $fix++;
      $form['agitem_'.$iz]['create_'.$iz] = array(
        '#type' => 'select',
        '#title' => t('Include agenda item?'),
        '#description' => t('Specify whether to include this agenda item in the meeting'),
        '#default_value' => $agenda_item->nid,
        '#options' => array(0=>'No', $agenda_item->nid=>'Yes'),
        '#required' => TRUE,
        '#attributes' => array('tabindex'=>$fix),
        '#weight' => $fix,
      );
      $fix++;
      $form['agitem_'.$iz]['detail_'.$iz] = array(
        '#prefix' => '<div class="form-item">',
        '#value' => 'Agenda item: <strong>'.$agenda_item->sequence.': '.$agenda_item->agendaitem.'</strong>',
        '#suffix' => '</div>',
      );
      $fix++;
      // Get an array of agenda_item status values
      $agitstatus_list = meeting_get_agenda_item_statuses();
      $form['agitem_'.$iz]['status_'.$iz] = array(
        '#type' => 'select',
        '#title' => t('Agenda item status'),
        '#description' => meeting_field_description('agenda-item', 'status', t('Select agenda item status')),
        '#default_value' => 0,
        '#options' => array('-- unspecified --') + $agitstatus_list,
        '#required' => TRUE,
        '#attributes' => array('tabindex'=>$fix),
        '#weight' => $fix,
      );
      $iz++;
    }
    $form['maxiz'] = array(
      '#type' => 'value',
      '#value' => $iz,
    );
    $submit_label = t('Generate linked agenda items');
  }
  // now add the submit button
  $fix++;
  $form['buttons'] = array(
    '#weight' => $fix,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_label,
    '#attributes' => array('tabindex'=>$fix),
  );
  // save form step for later use
  $form['step'] = array(
    '#type' => 'value',
    '#value' => $step,
  );
  // save the next value of the field index
  $form['fix'] = array(
    '#type' => 'value',
    '#value' => ++$fix,
  );
  // set the validate and submit handlers
  $form['#validate'] = array('meeting_form_validate');
  $form['#submit'] = array('meeting_form_submit');
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Meetings')=>'meeting/meetings'));
  return $form;
  
}

/**
 *  Function to generate a field description with popup help for each form field
 */
function meeting_field_description($type, $field, $description) {
  return theme('helpservice_topic','meeting',$type.'-details/'.$field).$description;
}

/**
 * Function to return an array of mtg_item sequence nos
 */
function meeting_get_agenda_item_seqnos($key) {
  // 

}

/**
 * Implementation of hook_form_alter()
 */
function meeting_form_meeting_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_form_meeting_node_form_alter',6);
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
      case 'template':
        // Meeting status should be fixed as 'template'
        $form['mtgstatus']['#default_value'] = 'template';
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
    $form['buttons']['submit']['#value'] = 'Add Meeting';
    $form['#goto'] = 'meeting/meetings/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Apply changes if this is a template form
  if ($form['mtgstatus']['#default_value'] == 'template') {
    // this is the form for a meeting template, so alter some fields
    $form['mtgstatus']['#options'] = array('template'=>'Meeting Template');
    $form['mtgstatus']['#required'] = TRUE;
    // Meeting date should be optional
    $form['mtgdate']['#required'] = FALSE;
    $form['mtgdate']['#description'] = t('Placeholder for actual meeting date');
    // Change submit button label if necessary
    if ($form['buttons']['submit']['#value'] == 'Add Meeting') {
      $form['buttons']['submit']['#value'] = 'Add Template';
    } 
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function meeting_form_agenda_item_node_form_alter(&$form, &$form_state) {
 //orgright_debug_dump('agenda item form alter',$form,'form-alter');
 //orgright_debug_dump('agenda item form alter',$form_state,'form-state');
  //orgright_debug_msg('meeting','Fn: meeting_form_agenda_item_node_form_alter',6);
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'meeting':
        // fill in the meeting field information
        if ($form['meeting']['#type'] == 'select') {
          // is a select type field, use nid
          $form['meeting']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT meeting FROM {node} n JOIN {org_meetings} g on g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
          $form['meeting']['#default_value'] = db_result(db_query($sql, $parvalue));
        }
        break;
      case 'template':
        // Agenda item status should be fixed as 'template'
        $form['agitstatus']['#default_value'] = 'template';
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
    $form['buttons']['submit']['#value'] = 'Add Agenda Item';
    $form['#goto'] = 'meeting/agenda_items/add';
  }
  else {
    $form['buttons']['submit']['#value'] = 'Save Changes';
    $form['#goto'] = 'node/'.$form['nid']['#value'];
  }
  // Apply any changes if this is a template form
  if ($form['agitstatus']['#default_value'] == 'template') {
    // this is the form for an agenda item template, so alter some fields
    $form['agitstatus']['#options'] = array('template'=>'Agenda Item Template');
    $form['agitstatus']['#required'] = TRUE;
    // Change the meeting field
    $template_list = meeting_get_meeting_templates('nid');
    if ($form['meeting']['#type'] != 'select') {
      $form['meeting']['#type'] = 'select';
      unset($form['meeting']['#size']);
      unset($form['meeting']['#maxlength']);
      unset($form['meeting']['#autocomplete_path']);
    }
    $form['meeting']['#options'] = $template_list;
    // Change submit button label if necessary
    if ($form['buttons']['submit']['#value'] == 'Add Agenda Item') {
      $form['buttons']['submit']['#value'] = 'Add Template';
    } 
  }
  // Set the weight and tab index for the submit button
  $fix = $form['fix']['#value'];
  $form['buttons']['submit']['#attributes'] = array('tabindex'=>$fix);
  $form['buttons']['submit']['#weight'] = $fix;
}

function meeting_form_mtg_item_node_form_alter(&$form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_form_mtg_items_node_form_alter',6);
  // Check to see whether additional arguments were passed to drupal_get_form
  if (isset($form['#parameters'][3])) {
    // Yes, so check and act on each
    $args = $form['#parameters'][3];
    foreach ($args as $parameter) {
      $paraname = substr($parameter,0,strpos($parameter,'~'));
      $parvalue = substr($parameter,strpos($parameter,'~')+1);
      switch ($paraname) {
      case 'meeting':
        // fill in the meeting field information
        if ($form['meeting']['#type'] == 'select') {
          // is a select type field, use nid
          $form['meeting']['#default_value'] = $parvalue;
        }
        else {
          // is a text field, find name
          $sql = "SELECT meeting FROM {node} n JOIN {org_meetings} g on g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
          $form['meeting']['#default_value'] = db_result(db_query($sql, $parvalue));
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
    $form['buttons']['submit']['#value'] = 'Add Meeting Item';
    $form['#goto'] = 'meeting/subpayments/add';
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
 * Form validate routines, called from meeting_form_validate() in meeting.module
 */
function meeting_meeting_form_validate($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_meeting_form_validate',6);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $meeting = strip_tags($form_state['values']['meeting']);
    $committee = strip_tags($form_state['values']['committee']);
    //$mtgtype = strip_tags($form_state['values']['mtgtype']);
    //$mtgdate = strip_tags($form_state['values']['mtgdate']);
    //$start = strip_tags($form_state['values']['start']);
    //$finish = strip_tags($form_state['values']['finish']);
    $location = strip_tags($form_state['values']['location']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($meeting != $form_state['values']['meeting']) {
        form_set_error('meeting',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($committee != $form_state['values']['committee']) {
        form_set_error('committee',t('HTML or PHP tags are not permitted in this field'));
    }
    //if ($mtgtype != $form_state['values']['mtgtype']) {
    //    form_set_error('mtgtype',t('HTML or PHP tags are not permitted in this field'));
    //}
    //if ($mtgdate != $form_state['values']['mtgdate']) {
    //    form_set_error('mtgdate',t('HTML or PHP tags are not permitted in this field'));
    //}
    //if ($start != $form_state['values']['start']) {
    //    form_set_error('start',t('HTML or PHP tags are not permitted in this field'));
    //}
    //if ($finish != $form_state['values']['finish']) {
    //    form_set_error('finish',t('HTML or PHP tags are not permitted in this field'));
    //}
    if ($location != $form_state['values']['location']) {
        form_set_error('location',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
  //$mtgdate = date_format_date(date_make_date($form_state['values']['mtgdate']), 'custom', 'd M Y');
    // Check that any name returned in "committee" is valid
    if ($committee) {
      module_load_include('inc', 'committee', 'committee_form'); 
      $cnid = committee_validate_committee($committee);
      if ($cnid) {
        form_set_value($form['cnid'], $cnid, $form_state);
      }
      else {
        form_set_error('committee',t('The name specified is not a known committee'));
      }
    }
    else {
      form_set_value($form['cnid'], 0, $form_state);
      $committee = '[no committee]';
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.meeting = '%s'";
    $nid = db_result(db_query($sql, $meeting));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('meeting',t('The meeting name is already in use'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Meeting: !cmtte - !mtg', array('!cmtte'=>$committee, '!mtg'=>$meeting));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached agenda items - delete not permitted in this case
    $sql = "SELECT COUNT(a.nid) from {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.gnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('meeting',t('This meeting still has @num agenda items attached!', array('@num'=>$count)));
    }
    // Check for attached meeting items - delete not permitted in this case
    $sql = "SELECT COUNT(i.nid) from {node} n JOIN {org_mtg_items} i ON i.nid = n.nid WHERE i.vid = n.vid AND i.gnid = %d";
    $count = db_result(db_query($sql, $form_state['values']['nid']));
    if ($count) {
      form_set_error('meeting',t('This meeting still has @num meeting items attached!', array('@num'=>$count)));
    }
  }
}

function meeting_agenda_item_form_validate($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_agenda_item_form_validate',6);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $meeting = strip_tags($form_state['values']['meeting']);
    $agendaitem = strip_tags($form_state['values']['agendaitem']);
    $sequence = strip_tags($form_state['values']['sequence']);
    $notes = strip_tags($form_state['values']['notes']);
    // Check that entered details are acceptable
    if ($meeting != $form_state['values']['meeting']) {
        form_set_error('meeting',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($agendaitem != $form_state['values']['agendaitem']) {
        form_set_error('agendaitem',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($sequence != $form_state['values']['sequence']) {
        form_set_error('sequence',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
    }
    $gnid = meeting_validate_meeting($meeting);
    if ($gnid) {
      form_set_value($form['gnid'], $gnid, $form_state);
      form_set_value($form['meeting'], $meeting, $form_state);
    }
    else {
      form_set_error('meeting',t('The name specified is not a known meeting'));
    }
    // Prevent duplicate records
    $sql = "SELECT n.nid FROM {node} n JOIN {org_agenda_items} a ON a.nid = n.nid WHERE a.vid = n.vid AND a.agendaitem = '%s' AND a.gnid = %d";
    $nid = db_result(db_query($sql, $agendaitem, $gnid));
    if ($nid) {
      // Already exists on the database, so flag an error unless it is same record being updated
      if ($nid != $form['nid']['#value']) {
        form_set_error('agendaitem',t('Agenda item already exists for this meeting'));
      }
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Agenda item: !item for !mtg', array('!item'=>$agendaitem, '!mtg'=>$meeting));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    // Check for attached ??? - delete not permitted in this case
  }
}

function meeting_mtg_item_form_validate($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_mtg_item_form_validate',6);
  $clicked_button = $form_state['clicked_button']['#value'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Prepare some local values - use PHP strip_tags function to clean up input
    $itemname = strip_tags($form_state['values']['itemname']);
    $member = strip_tags($form_state['values']['member']);
    $meeting = strip_tags($form_state['values']['meeting']);
    //$itemtype = strip_tags($form_state['values']['itemtype']);
    //$itemdate = strip_tags($form_state['values']['itemdate']);
    $notes = strip_tags($form_state['values']['notes']);

    // Check that entered details are acceptable
    if ($itemname != $form_state['values']['itemname']) {
        form_set_error('itemname',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($member != $form_state['values']['member']) {
        form_set_error('member',t('HTML or PHP tags are not permitted in this field'));
    }
    if ($meeting != $form_state['values']['meeting']) {
        form_set_error('meeting',t('HTML or PHP tags are not permitted in this field'));
    }
    //if ($itemtype != $form_state['values']['itemtype']) {
    //    form_set_error('itemtype',t('HTML or PHP tags are not permitted in this field'));
    //}
    //if ($itemdate != $form_state['values']['itemdate']) {
    //    form_set_error('itemdate',t('HTML or PHP tags are not permitted in this field'));
    //}
    if ($notes != $form_state['values']['notes']) {
        form_set_error('notes',t('HTML or PHP tags are not permitted in this field'));
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
    // Check that the name returned in "meeting" is valid
    $gnid = meeting_validate_meeting($meeting);
    if ($gnid) {
      form_set_value($form['gnid'], $gnid, $form_state);
    }
    else {
      form_set_error('meeting',t('The name specified is not a known meeting'));
    }
    // Build a composite node title and save in $form_state
    $form_state['values']['title'] = t('Meeting item: !type - !item', array('!type'=>$itemtype, '!item'=>$itemname));
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //orgright_debug_msg('meeting','--- Delete button clicked ---');
  }
}

function meeting_template_use_form_validate($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_template_use_form_validate',6);
  // validate the meeting creation form
  $step = $form_state['values']['step'];
  if ($step == 1) {
    // retrieve the meeting template
    if ($form_state['values']['template']) {
      $sql = "SELECT g.nid, g.cnid, g.meeting, g.mtgtype, g.start, g.finish, g.location, g.notes FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.nid = %d";
      $template = db_fetch_array(db_query($sql, $form_state['values']['template']));
      // and save in the storage area
      $form_state['storage']['template'] = $template;
      // Load committee reference
      $sql = "SELECT c.committee FROM {node} n JOIN {org_committees} c ON c.nid = n.nid WHERE c.vid = n.vid AND c.nid = %d";
      $form_state['storage']['template']['committee'] = db_result(db_query($sql, $template['cnid']));
      // Load body text
      $sql = "SELECT r.body FROM {node} n JOIN {node_revisions} r ON r.nid = n.nid WHERE r.vid = n.vid AND r.nid = %d";
      $form_state['storage']['template']['body'] = db_result(db_query($sql, $template['nid']));
    }
    else {
      form_set_error('template',t('A meeting template must be selected'));
    }
  }
  elseif ($step == 2) {
    // validate the supplied meeting details
    meeting_meeting_form_validate($form, $form_state);
    // save the meeting reference
    $form_state['storage']['meeting'] = $form_state['values']['meeting'];
  }
  elseif ($step == 3) {
    // validate any supplied agenda item details
    // nothing to do
  }
}

function meeting_validate_meeting(&$meeting) {
  // Check that the value passed in "$meeting" is valid, and return the associated nid
  //orgright_debug_msg('meeting',t('Validate meeting name: @mbr', array('@mbr' => $meeting)),7);
  if (is_numeric($meeting)) {
    // then the passed value should be the nid of a meeting node
    $gnid = $meeting;
    $sql = "SELECT n.type, m.meeting FROM {node} n JOIN {org_meetings} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $obj = db_fetch_object(db_query($sql, $gnid));
    if ($obj->type == 'meeting') { $meeting = $obj->meeting; }
    else { $gnid = 0; }
    return $gnid;
  }
  else {
    // Query the database to ensure that the meeting name is valid, and to find the gnid
    $sql = "SELECT g.nid FROM {node} n JOIN {org_meetings} g ON g.nid = n.nid WHERE g.vid = n.vid AND g.meeting = '%s'";
    return db_result(db_query($sql, $meeting));
  }
}

/**
 * Form submit routines, called from meeting_form_submit() in meeting.module
 */
function meeting_meeting_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('meeting','Fn: meeting_meeting_form_submit',6);
  // set meeting status if necessary
  if ($form_state['values']['mtgstatus'] == '0') { $form_state['values']['mtgstatus'] = ''; }
  // Massage the date and times ready for storage in the database
  $form_state['values']['mtgdate'] = substr($form_state['values']['mtgdate'],0,10);
  $form_state['values']['start'] = substr($form_state['values']['start'],11,8);
  $form_state['values']['finish'] = substr($form_state['values']['finish'],11,8);
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function meeting_agenda_item_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('meeting','Fn: meeting_agenda_item_form_submit',6);
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function meeting_mtg_item_form_submit($form, &$form_state) {
  // Callback specified for the "submit" button
  //orgright_debug_msg('meeting','Fn: meeting_mtg_item_form_submit',6);
  // Massage the date ready for storage in the database
  $form_state['values']['itemdate'] = substr($form_state['values']['itemdate'],0,10);
  // Make sure that this node is neither promoted nor sticky
  $form_state['values']['promote'] = 0;
  $form_state['values']['sticky'] = 0;
  // Set the redirection
  $form_state['redirect'] = $form['#goto'];
}

function meeting_template_use_form_submit($form, &$form_state) {
  //orgright_debug_msg('meeting','Fn: meeting_template_use_form_submit',6);
  $step = $form_state['values']['step'];
  if ($step == 1) {
    // just return to progress to step 2
    return;
  }
  elseif ($step == 2) {
    // Generate a new meeting from the submitted details
    // specify the form to be used to load the meeting
    $form_id = 'meeting_node_form';
    // create a new node for this meeting
    $mtg_node = node_prepare((object)array('type' => 'meeting'));
    // prepare a copy of $form_state
    $mtg_form_state = $form_state;
    //unset($mtg_form_state['values']);
    // populate meeting $form_state with the values for this meeting
    $mtg_form_state['clicked_button']['#value'] = 'Add Meeting';
    $mtg_form_state['#validate'] = array('meeting_form_validate');
    $mtg_form_state['#submit'] = array('meeting_form_submit');
    // following is required to get node form submit to work correctly
    $mtg_form_state['submit_handlers'] = array('node_form_submit');
    // flag that this subscription is being submitted programmatically
    $mtg_form_state['#template'] = TRUE;
    // now submit the form
    module_load_include('inc', 'node', 'node.pages');
    drupal_execute($form_id, $mtg_form_state, $mtg_node);
    // now progress onto step three - the agenda items
  }
  elseif ($step == 3) {
    // need to create any selected agenda items
    // get the meeting gnid that they will point to
    $gnid = meeting_validate_meeting($form_state['storage']['meeting']);
    for ($iz = 0; $iz < $form_state['values']['maxiz']; $iz++) {
      // check whether this agenda item is to be ignored
      if (!$form_state['values']['create_'.$iz]) { continue; }
      // No, so create a new agenda item
      // First of all, load the template agenda item node
      $agenda_item = node_load($form_state['values']['create_'.$iz]);
      // clear the nid and vid values so it can be saved as a new node
      $agenda_item->nid = 0;
      $agenda_item->vid = 0;
      // set the meeting that this is attached to
      $agenda_item->gnid = $gnid;
      // and change the status to the specified value
      $agenda_item->agitstatus = $form_state['values']['status_'.$iz];
      // then save in the database
      node_save($agenda_item);
    }
    // done
    $form_state['storage'] = array();
    $form_state['redirect'] = 'meeting/meetings';
  }
}
