<?php
// $Id$

/**
 * @file
 * Provides additional functions for membership module.
 */

/**
 * Called from hook_view() in module file
 */
function membership_member_view(&$node, $teaser, $page) {
  $description = t('Name');
  $node->content['member'] = array(
    '#value' => membership_theme_item('member', $description, $node->member),
    '#weight' => 1,
  );
  $description = t('Address');
  $node->content['address'] = array(
    '#value' => membership_theme_item('address', $description, $node->address),
    '#weight' => 2,
  );
  $description = t('City');
  $node->content['city'] = array(
    '#value' => membership_theme_item('city', $description, $node->city),
    '#weight' => 3,
  );
  $description = t('Postcode');
  $node->content['postcode'] = array(
    '#value' => membership_theme_item('postcode', $description, $node->postcode),
    '#weight' => 4,
  );
  $description = t('Phone number(s)');
  $node->content['phone'] = array(
    '#value' => membership_theme_item('phone', $description, $node->phone),
    '#weight' => 5,
  );
  $description = t('Email address(es)');
  $node->content['email'] = array(
    '#value' => membership_theme_item('email', $description, check_markup($node->email, orgright_get_field_filter())),
    '#weight' => 6,
  );
  $description = t('Category');
  $node->content['category'] = array(
    '#value' => membership_theme_item('category', $description, $node->category),
    '#weight' => 7,
  );
  $description = t('Status');
  $node->content['standing'] = array(
    '#value' => membership_theme_item('standing', $description, $node->standing),
    '#weight' => 8,
  );
  // Prepare the joining and leaving dates for display
  $joined = date_format_date(date_make_date($node->joindate), 'custom', 'd M Y');
  $dateleft = date_format_date(date_make_date($node->leavedate), 'custom', 'd M Y');
  $description = t('Date joined');
  $node->content['joined'] = array(
    '#value' => membership_theme_item('joined', $description, $joined),
    '#weight' => 9,
  );
  $description = t('Date left');
  $node->content['dateleft'] = array(
    '#value' => membership_theme_item('dateleft', $description, $dateleft),
    '#weight' => 9,
  );
  $description = t('Primary member');
  $node->content['parent'] = array(
    '#value' => membership_theme_item('parent', $description, $node->parent, $node->pnid),
    '#weight' => 10,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => membership_theme_item('notes', $description, $notes),
    '#weight' => 12,
  );
  $weight = 20;
  // Check whether there are any children details
  if (isset($node->children)) {
    //orgright_debug_msg('membership','children details found');
    $heading = t('The following memberships are attached as children to this member');
    ++$weight;
    $node->content['children'] = array(
      '#value' => membership_theme_heading('children', $heading),
      '#weight' => $weight,
    );
    foreach ($node->children as $node->this_child) {
      $subitem = $node->this_child['member'];
      $ref = $node->this_child['nid'];
      $attributes = array(
        ': ' . $node->this_child['phone'],
        ', ' . $node->this_child['address'],
        ', ' . $node->this_child['city'],
        ', ' . $node->this_subscription['postcode'],
        ', ' . $node->this_subscription['email'],
      );
      ++$weight;
      $node->content['children'][] = array(
        '#value' => membership_theme_subitem('children', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new child member
  ++$weight;
  $linktext = l(t('Add new child membership'), 'membership/members/add/parent~' . $node->nid);
  $node->content['add-child'] = array(
    '#value' => membership_theme_link('add-child', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any role assignment details
  if (isset($node->assignments)) {
    //orgright_debug_msg('membership','assignment details found');
    $heading = t('The following committee role assignments are held for this member');
    ++$weight;
    $node->content['assignments'] = array(
      '#value' => membership_theme_heading('assignments', $heading),
      '#weight' => $weight,
    );
    $ceased = FALSE;
    $today = orgright_get_date_ymd();
    foreach ($node->assignments as $this_appointment) {
      // skip any assignments that have ceased - display later
      if ($this_appointment['ceasedate'] && $this_appointment['ceasedate'] > $today) {
        $ceased = TRUE;
        continue;
      }
      // Prepare the date assigndate for display
      $assigndate = date_format_date(date_make_date($this_appointment['assigndate']), 'custom', 'd M Y');
      $subitem = $this_appointment['assigntype'];
      $ref = $this_appointment['nid'];
      $attributes = array(
        ': ' . l($this_appointment['role'], 'node/' . $this_appointment['rnid']),
        ', role assigned: ' . $assigndate,
      );
      if ($this_appointment['ceasedate']) {
        $ceasedate = date_format_date(date_make_date($this_appointment['ceasedate']), 'custom', 'd M Y');
        $attributes[] = ' to cease on ' . $ceasedate;
      }
      ++$weight;
      $node->content['assignments'][] = array(
        '#value' => membership_theme_subitem('assignments', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
    if ($ceased) {
      $heading = t('Old assignments');
      ++$weight;
      $node->content['old-assignments'] = array(
        '#value' => membership_theme_heading('old-assignments', $heading),
        '#weight' => $weight,
      );
      foreach ($node->assignments as $this_appointment) {
        // skip any assignments that have not ceased
        if (!$this_appointment['ceasedate'] || $this_appointment['ceasedate'] >= $today) {
          continue;
        }
        // Prepare the dates assigndate and ceasedate for display
        $assigndate = date_format_date(date_make_date($this_appointment['assigndate']), 'custom', 'd M Y');
        $ceasedate = date_format_date(date_make_date($this_appointment['ceasedate']), 'custom', 'd M Y');
        $subitem = $this_appointment['assigntype'];
        $ref = $this_appointment['nid'];
        $attributes = array(
          ': ' . l($this_appointment['role'], 'node/' . $this_appointment['rnid']),
          ', role assigned: ' . $assigndate,
          ', ceased: ' . $ceasedate,
        );
        ++$weight;
        $node->content['old-assignments'][] = array(
          '#value' => membership_theme_subitem('old-assignments', $subitem, $ref, $attributes),
          '#weight' => $weight,
        );
      }
    }
  }
  // Include a link to add new role assignment
  ++$weight;
  $linktext = l(t('Add new committee role assignment'), 'committee/assignments/add/member~' . $node->nid);
  $node->content['add-assignment'] = array(
    '#value' => membership_theme_link('add-assignment', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any committee appointment details
  if (isset($node->appointments)) {
    //orgright_debug_msg('membership','appointment details found');
    $heading = t('The following committee appointments are held for this member');
    ++$weight;
    $node->content['appointments'] = array(
      '#value' => membership_theme_heading('appointments', $heading),
      '#weight' => $weight,
    );
    $retired = FALSE;
    $today = orgright_get_date_ymd();
    foreach ($node->appointments as $this_appointment) {
      // skip any appointments that have already retired - display later
      if ($this_appointment['retiredate'] && $this_appointment['retiredate'] < $today) {
        $retired = TRUE;
        continue;
      }
      // Prepare the date appointdate for display
      $appointdate = date_format_date(date_make_date($this_appointment['appointdate']), 'custom', 'd M Y');
      $subitem = $this_appointment['appointtype'];
      $ref = $this_appointment['nid'];
      $attributes = array(
        ' to ' . l($this_appointment['committee'], 'node/' . $this_appointment['cnid']),
        ' on ' . $appointdate,
      );
      if ($this_appointment['retiredate']) {
        $retiredate = date_format_date(date_make_date($this_appointment['retiredate']), 'custom', 'd M Y');
        $attributes[] = ' to retire on ' . $retiredate;
      }
      ++$weight;
      $node->content['appointments'][] = array(
        '#value' => membership_theme_subitem('appointments', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
    if ($retired) {
      $heading = t('Old appointments');
      ++$weight;
      $node->content['old-appointments'] = array(
        '#value' => membership_theme_heading('old-appointments', $heading),
        '#weight' => $weight,
      );
      foreach ($node->appointments as $this_appointment) {
        // skip any appointments that have not yet retired
        if (!$this_appointment['retiredate'] || $this_appointment['retiredate'] >= $today) {
          continue;
        }
        // Prepare the dates appointdate and retiredate for display
        $appointdate = date_format_date(date_make_date($this_appointment['appointdate']), 'custom', 'd M Y');
        $retiredate = date_format_date(date_make_date($this_appointment['retiredate']), 'custom', 'd M Y');
        $subitem = $this_appointment['appointtype'];
        $ref = $this_appointment['nid'];
        $attributes = array(
          ' to ' . l($this_appointment['committee'], 'node/' . $this_appointment['cnid']),
          ' on ' . $appointdate,
          ', retired on ' . $retiredate,
        );
        ++$weight;
        $node->content['old-appointments'][] = array(
          '#value' => membership_theme_subitem('old-appointments', $subitem, $ref, $attributes),
          '#weight' => $weight,
        );
      }
    }
  }
  // Include a link to add new committee appointment
  ++$weight;
  $linktext = l(t('Add new committee appointment'), 'committee/appointments/add/member~' . $node->nid);
  $node->content['add-appointment'] = array(
    '#value' => membership_theme_link('add-appointment', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any membership subscription details
  if (isset($node->subscriptions)) {
    //orgright_debug_msg('membership','subscription details found');
    $heading = t('The following subscription details are held for this member');
    ++$weight;
    $node->content['subscriptions'] = array(
      '#value' => membership_theme_heading('subscriptions', $heading),
      '#weight' => $weight,
    );
    foreach ($node->subscriptions as $node->this_subscription) {
      // Prepare the date datefrom for display
      $adate = date_make_date($node->this_subscription['datefrom']);
      $datefrom = date_format_date($adate, 'custom', 'd M Y');
      // Prepare the date dateto for display
      $adate = date_make_date($node->this_subscription['dateto']);
      $dateto = date_format_date($adate, 'custom', 'd M Y');
      $subitem = $node->this_subscription['scode'];
      $ref = $node->this_subscription['nid'];
      $attributes = array(
        ': subscription from ' . $datefrom,
        ' to ' . $dateto,
        ', for $' . $node->this_subscription['amount'],
      );
      ++$weight;
      $node->content['subscriptions'][] = array(
        '#value' => membership_theme_subitem('subscriptions', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new subscription
  ++$weight;
  $linktext = l(t('Add new subscription'), 'membership/subscriptions/add/member~' . $node->nid);
  $node->content['add-subscription'] = array(
    '#value' => membership_theme_link('add-subscription', $linktext),
    '#weight' => $weight,
  );
  // Check whether there are any subscription payment details
  if (isset($node->subpayments)) {
    //orgright_debug_msg('membership','payment details found');
    $heading = t('The following subscription payments are held for this member');
    ++$weight;
    $node->content['subpayments'] = array(
      '#value' => membership_theme_heading('subpayments', $heading),
      '#weight' => $weight,
    );
    foreach ($node->subpayments as $node->this_subpayment) {
      // Prepare the date datepaid for display
      $pdate = date_make_date($node->this_subpayment['datepaid']);
      $datepaid = date_format_date($pdate, 'custom', 'd M Y');
      $subitem = $node->this_subpayment['payment'];
      $ref = $node->this_subpayment['nid'];
      $attributes = array(
        ' on ' . $datepaid,
      );
      ++$weight;
      $node->content['subpayments'][] = array(
        '#value' => membership_theme_subitem('subpayments', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new subscription payment
  ++$weight;
  $linktext = l(t('Add new subscription payment'), 'membership/subpayments/add/member~' . $node->nid);
  $node->content['add-subpayment'] = array(
    '#value' => membership_theme_link('add-subpayment', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership') => 'membership', t('Members') => 'membership/members'));
}

function membership_subscription_view(&$node, $teaser, $page) {
  $description = t('Member name');
  $node->content['member'] = array(
    '#value' => membership_theme_item('member', $description, $node->member, $node->mnid),
    '#weight' => 1,
  );
  $description = t('Subscription');
  $node->content['scode'] = array(
    '#value' => membership_theme_item('scode', $description, $node->scode),
    '#weight' => 2,
  );
  // Prepare the date datefrom for display
  $jdate = date_make_date($node->datefrom);
  $started = date_format_date($jdate, 'custom', 'd M Y');
  $description = t('Start date');
  $node->content['start'] = array(
    '#value' => membership_theme_item('start', $description, $started),
    '#weight' => 3,
  );
  // Prepare the date dateto for display
  $jdate = date_make_date($node->dateto);
  $finished = date_format_date($jdate, 'custom', 'd M Y');
  $description = t('End date');
  $node->content['finish'] = array(
    '#value' => membership_theme_item('finish', $description, $finished),
    '#weight' => 4,
  );
  $description = t('Subscription amount');
  $node->content['amount'] = array(
    '#value' => membership_theme_item('amount', $description, $node->amount),
    '#weight' => 5,
  );
  $description = t('Total paid');
  $node->content['totpaid'] = array(
    '#value' => membership_theme_item('totpaid', $description, $node->totpaid),
    '#weight' => 6,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $node->content['notes'] = array(
    '#value' => membership_theme_item('notes', $description, $node->notes),
    '#weight' => 7,
  );
  // Check whether there are any subpayment details
  $weight = 20;
  if (isset($node->subpayments)) {
    //orgright_debug_msg('membership','payment details found');
    $heading = t('The following payment details are held for this subscription');
    ++$weight;
    $node->content['subpayments'] = array(
      '#value' => membership_theme_heading('subpayments', $heading),
      '#weight' => $weight,
    );
    foreach ($node->subpayments as $node->this_subpayment) {
      // Prepare the date datepaid for display
      $pdate = date_make_date($node->this_subpayment['datepaid']);
      $datepaid = date_format_date($pdate, 'custom', 'd M Y');
      $subitem = $node->this_subpayment['payment'];
      $ref = $node->this_subpayment['nid'];
      $attributes = array(
        ' on ' . $datepaid,
      );
      ++$weight;
      $node->content['subpayments'][] = array(
        '#value' => membership_theme_subitem('subpayments', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new subscription payment
  ++$weight;
  $linktext = l(t('Add new subscription payment'), 'membership/subpayments/add/subscription~' . $node->nid . '/member~' . $node->mnid);
  $node->content['add-subpayment'] = array(
    '#value' => membership_theme_link('add-subpayment', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership') => 'membership', t('Subscriptions') => 'membership/subscriptions'));
}

function membership_subpayment_view(&$node, $teaser, $page) {
  $description = t('Member name');
  $node->content['member'] = array(
    '#value' => membership_theme_item('member', $description, $node->member, $node->mnid),
    '#weight' => 1,
  );
  $description = t('Subscription');
  $node->content['scode'] = array(
    '#value' => membership_theme_item('scode', $description, $node->scode, $node->snid),
    '#weight' => 3,
  );
  $description = t('Subscription amount');
  $node->content['amount'] = array(
    '#value' => membership_theme_item('amount', $description, $node->amount),
    '#weight' => 4,
  );
  // Prepare the date datepaid for display
  $pdate = date_make_date($node->datepaid);
  $paydate = date_format_date($pdate, 'custom', 'd M Y');
  $description = t('Payment date');
  $node->content['paydate'] = array(
    '#value' => membership_theme_item('paydate', $description, $paydate),
    '#weight' => 6,
  );
  $description = t('Amount paid');
  $node->content['payment'] = array(
    '#value' => membership_theme_item('payment', $description, $node->payment),
    '#weight' => 7,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $node->content['notes'] = array(
    '#value' => membership_theme_item('notes', $description, $node->notes),
    '#weight' => 9,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Membership') => 'membership', t('Payments') => 'membership/subpayments'));
}

/**
 * Specific theming functions
 */
function membership_theme_heading($heading_class, $heading_value) {
  return theme('orgright_heading', 'membership', $heading_class, $heading_value);
}

function membership_theme_item($item_class, $description, $item_value, $node_ref = 0) {
  // General function for themeing an item value to be displayed.
  return theme('orgright_item', 'membership', $item_class, $description, $item_value, $node_ref);
}

function membership_theme_subitem($subitem_class, $subitem_value, $node_ref, $attributes) {
  // General function for themeing a sub-item set of values to be displayed.
  return theme('orgright_subitem', 'membership', $subitem_class, $subitem_value, $node_ref, $attributes);
}

function membership_theme_link($link_class, $link_value) {
  return theme('orgright_link', 'membership', $link_class, $link_value);
}
