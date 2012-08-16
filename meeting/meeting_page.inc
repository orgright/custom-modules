<?php
// $Id$

/**
 * @file
 * Provides additional functions for meeting module.
 */

/**
 * Called from hook_view() in module file
 */
function meeting_meeting_view(&$node, $teaser, $page) {
  //orgright_debug_msg('meeting','Fn: meeting_meeting_view');
  // Get metadata for this node type - used for labeling body field
  // note: the $node->content['body'] will always have a weight of 0.
  $nodetype = node_get_types('type', $node);
  // Prepare the date and times for display
  $mtgdate = date_format_date(date_make_date($node->mtgdate), 'custom', 'd M Y');
  $start = date_format_date(date_make_date($node->mtgdate.' '.$node->start), 'custom', 'g:i a');
  if ($node->finish) { $finish = date_format_date(date_make_date($node->mtgdate.' '.$node->finish), 'custom', 'g:i a'); }
  else { $finish = ''; }
  $description = t('Meeting');
  $node->content['meeting'] = array(
    '#value' => meeting_theme_item('meeting', $description, $node->meeting),
    '#weight' => -9,
  );
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => meeting_theme_heading('body-label', $description),
    '#weight' => -1,
  );
  if ($node->cnid) {
    $description = t('Committee');
    $node->content['committee'] = array(
      '#value' => meeting_theme_item('committee', $description, $node->committee, $node->cnid),
      '#weight' => 1,
    );
  }
  else {
    $node->content['committee'] = array(
      '#value' => meeting_theme_item('committee', '[no committee]', ''),
      '#weight' => 1,
    );
  }
  $description = t('Type');
  $node->content['mtgtype'] = array(
    '#value' => meeting_theme_item('mtgtype', $description, $node->mtgtype),
    '#weight' => 2,
  );
  $description = t('Status');
  $node->content['mtgstatus'] = array(
    '#value' => meeting_theme_item('mtgstatus', $description, $node->mtgstatus),
    '#weight' => 3,
  );
  $description = t('Meeting date');
  $node->content['mtgdate'] = array(
    '#value' => meeting_theme_item('mtgdate', $description, $mtgdate),
    '#weight' => 4,
  );
  $description = t('Meeting location');
  $node->content['location'] = array(
    '#value' => meeting_theme_item('location', $description, $node->location),
    '#weight' => 5,
  );
  $description = t('Start time');
  $node->content['start'] = array(
    '#value' => meeting_theme_item('start', $description, $start),
    '#weight' => 6,
  );
  $description = t('Target finish time');
  $node->content['finish'] = array(
    '#value' => meeting_theme_item('finish', $description, $finish),
    '#weight' => 7,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) { return; }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => meeting_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 20;
  // Check whether there are any agenda item details
  if (isset($node->agenda_items)) {
    //orgright_debug_msg('meeting','agenda item details found',7);
    $heading = t('The following agenda items are recorded for this meeting:');
    $node->content['agenda-items'] = array(
      '#value' => meeting_theme_heading('agenda-items', $heading),
      '#weight' => $weight,
    );
    $item_num = 0;
    foreach ($node->agenda_items as $this_item) {
      ++$weight;
      ++$item_num;
      $subitem = $this_item['agendaitem'];
      $ref = $this_item['nid'];
      $attributes = array(
        ', sequence ' . $this_item['sequence'],
      );
      $node->content['agenda-items'][] = array(
        '#value' => meeting_theme_subitem('agenda-items', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new agenda item
  ++$weight;
  if ($node->mtgstatus == 'template') {
    $linktext = l(t('Add new agenda item'), 'meeting/agenda_items/add/meeting~'.$node->nid.'/template~0');
  }
  else {
    $linktext = l(t('Add new agenda item'), 'meeting/agenda_items/add/meeting~'.$node->nid);
  }
  $node->content['add-agenda-item'] = array(
    '#value' => meeting_theme_link('add-agenda-item', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any meeting item details
  if (isset($node->mtg_items)) {
    //orgright_debug_msg('meeting','assignment details found',7);
    ++$weight;
    $heading = t('The following meeting items are recorded for this meeting:');
    $node->content['mtg-items'] = array(
      '#value' => meeting_theme_heading('mtg-items', $heading),
      '#weight' => $weight,
    );
    $item_num = 0;
    foreach ($node->mtg_items as $this_item) {
      ++$weight;
      ++$item_num;
      // Prepare the assigndate for display
      $itemdate = date_format_date(date_make_date($this_item['itemdate']), 'custom', 'd M Y');
      $subitem = $this_item['itemname'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' ' . $this_item['itemtype'],
        ' ' . $itemdate,
        ' ' . l($this_item['member'], 'node/'.$this_item['mnid']),
      );
      $node->content['mtg-items'][] = array(
        '#value' => meeting_theme_subitem('mtg-items', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new meeting item
  ++$weight;
  $linktext = l(t('Add new meeting item'), 'meeting/mtg_items/add/meeting~'.$node->nid);
  $node->content['add-mtg-item'] = array(
    '#value' => meeting_theme_link('add-mtg-item', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Meetings')=>'meeting/meetings'));
}

function meeting_agenda_item_view(&$node, $teaser, $page) {
  // Get metadata for this node type - used for labeling body field
  // note: the $node->content['body'] will always have a weight of 0.
  $nodetype = node_get_types('type', $node);
  // Prepare the date meeting date for display
  $mtgdate = date_format_date(date_make_date($node->mtgdate), 'custom', 'd M Y');

  $description = t('Agenda item');
  $node->content['agenda-item'] = array(
    '#value' => meeting_theme_item('agenda-item', $description, $node->agendaitem),
    '#weight' => -4,
  );
  $description = t('Sequence no.');
  $node->content['sequence'] = array(
    '#value' => meeting_theme_item('sequence', $description, $node->sequence),
    '#weight' => -3,
  );
  $description = t('Status');
  $node->content['agitstatus'] = array(
    '#value' => meeting_theme_item('agitstatus', $description, $node->agitstatus),
    '#weight' => -2,
  );
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => meeting_theme_item('body-label', $description, ''),
    '#weight' => -1,
  );
  if ($node->cnid) {
    $description = t('Committee');
    $node->content['committee'] = array(
      '#value' => meeting_theme_item('committee', $description, $node->committee, $node->cnid),
      '#weight' => 1,
    );
  }
  if ($node->gnid) {
    $description = t('Meeting');
    $node->content['meeting'] = array(
      '#value' => meeting_theme_item('meeting', $description, $node->meeting, $node->gnid),
      '#weight' => 2,
    );
    $description = t(' Meeting type');
    $node->content['mtgtype'] = array(
      '#value' => meeting_theme_item('mtgtype', $description, $node->mtgtype),
      '#weight' => 3,
    );
    $description = t(' Meeting date');
    $node->content['mtgdate'] = array(
      '#value' => meeting_theme_item('mtgdate', $description, $mtgdate),
      '#weight' => 4,
    );
  }
  // If a teaser is being generated, then we are done
  if ($teaser) { return; }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => meeting_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Include a link to add new agenda item
  ++$weight;
  if ($node->agitstatus == 'template') {
    $linktext = l(t('Add new agenda item'), 'meeting/agenda_items/add/meeting~'.$node->gnid.'/template~0');
  }
  else {
    $linktext = l(t('Add new agenda item'), 'meeting/agenda_items/add/meeting~'.$node->gnid);
  }
  $node->content['add-agenda-item'] = array(
    '#value' => meeting_theme_link('add-agenda-item', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Agenda items')=>'meeting/agenda_items'));
}

function meeting_mtg_item_view(&$node, $teaser, $page) {
  // Get metadata for this node type - used for labeling body field
  // note: the $node->content['body'] will always have a weight of 0.
  $nodetype = node_get_types('type', $node);
  // Prepare the item and meeting dates for display
  $itemdate = date_format_date(date_make_date($node->itemdate), 'custom', 'd M Y');
  $mtgdate = date_format_date(date_make_date($node->mtgdate), 'custom', 'd M Y');

  $description = t('Meeting item type');
  $node->content['itemtype'] = array(
    '#value' => meeting_theme_item('itemtype', $description, $node->itemtype),
    '#weight' => -10,
  );
  $description = t('Meeting item');
  $node->content['itemname'] = array(
    '#value' => meeting_theme_item('itemname', $description, $node->itemname),
    '#weight' => -9,
  );
  if ($node->mnid) {
    $description = t('Person responsible');
    $node->content['member'] = array(
      '#value' => meeting_theme_item('member', $description, $node->member, $node->mnid),
      '#weight' => -8,
    );
  }
  $description = t('Date for completion');
  $node->content['itemdate'] = array(
    '#value' => meeting_theme_item('itemdate', $description, $itemdate),
    '#weight' => -7,
  );
  $description = t('Meeting item status');
  $node->content['itemstatus'] = array(
    '#value' => meeting_theme_item('itemstatus', $description, $node->itemstatus),
    '#weight' => -5,
  );
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => meeting_theme_item('body-label', $description, ''),
    '#weight' => -1,
  );
  if ($node->cnid) {
    $description = t(' Committee');
    $node->content['committee'] = array(
      '#value' => meeting_theme_item('committee', $description, $node->committee, $node->cnid),
      '#weight' => 1,
    );
  }
  if ($node->gnid) {
    $description = t('Meeting');
    $node->content['meeting'] = array(
      '#value' => meeting_theme_item('meeting', $description, $node->meeting, $node->gnid),
      '#weight' => 2,
    );
    $description = t(' Meeting type');
    $node->content['mtgtype'] = array(
      '#value' => meeting_theme_item('mtgtype', $description, $node->mtgtype),
      '#weight' => 3,
    );
    $description = t(' Meeting date');
    $node->content['mtgdate'] = array(
      '#value' => meeting_theme_item('mtgdate', $description, $mtgdate),
      '#weight' => 4,
    );
  }
  // If a teaser is being generated, then we are done
  if ($teaser) { return; }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => meeting_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Include a link to add new meeting item
  ++$weight;
  $linktext = l(t('Add new meeting item'), 'meeting/mtg_items/add/meeting~'.$node->gnid);
  $node->content['add-mtg-item'] = array(
    '#value' => meeting_theme_link('add-mtg-item', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Meeting')=>'meeting', t('Meeting items')=>'meeting/mtg_items'));
}

/**
 * Specific theming functions
 */
function meeting_theme_heading($heading_class, $heading_value) {
  return theme('orgright_heading', 'meeting', $heading_class, $heading_value);
}

function meeting_theme_item($item_class, $description, $item_value, $node_ref = 0) {
  // General function for themeing an item value to be displayed.
  return theme('orgright_item', 'meeting', $item_class, $description, $item_value, $node_ref);
}

function meeting_theme_subitem($subitem_class, $subitem_value, $node_ref, $attributes) {
  // General function for themeing a sub-item set of values to be displayed.
  return theme('orgright_subitem', 'meeting', $subitem_class, $subitem_value, $node_ref, $attributes);
}

function meeting_theme_link($link_class, $link_value) {
  return theme('orgright_link', 'meeting', $link_class, $link_value);
}
