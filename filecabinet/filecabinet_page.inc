<?php
// $Id$

/**
 * @file
 * Provides additional functions for filecabinet module.
 */

/**
 * Called from hook_view() in module file
 */
function filecabinet_drawer_view(&$node, $teaser, $page) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_drawer_view');
  // Get metadata for this node type - used for labeling body field
  $nodetype = node_get_types('type', $node);
  // Generate the content display information
  // note: the $node->content['body'] will always have a weight of 0.
  $description = t('Drawer');
  $node->content['drawer'] = array(
    '#value' => filecabinet_theme_item('drawer', $description, $node->drawer),
    '#weight' => -9,
  );
  if ($node->ouid) {
    $description = t('Owner');
    $node->content['owner'] = array(
      '#value' => filecabinet_theme_item('owner', $description, $node->owner),
      '#weight' => 1,
    );
  }
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => filecabinet_theme_item('body-label', $description, ''),
    '#weight' => -1,
  );
  if ($node->wnid) {
    $description = t('Parent drawer');
    $node->content['parent'] = array(
      '#value' => filecabinet_theme_item('parent', $description, $node->parent, $node->wnid),
      '#weight' => 1,
    );
  }
  $description = t('Status');
  $node->content['drawerstatus'] = array(
    '#value' => filecabinet_theme_item('drawerstatus', $description, $node->drawerstatus),
    '#weight' => 2,
  );
  $description = t('Permissions');
  $node->content['drawerperm'] = array(
    '#value' => filecabinet_theme_item('drawerperm', $description, $node->drawerperm),
    '#weight' => 3,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => filecabinet_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 20;
  // Check whether there are any subsidiary drawer details
  if (isset($node->drawers)) {
    ++$weight;
    //orgright_debug_msg('filecabinet','subsidiary drawer details found',7);
    $heading = t('The following sub-drawers are stored within this drawer:');
    $node->content['subdrawers'] = array(
      '#value' => filecabinet_theme_heading('subdrawers', $heading),
      '#weight' => $weight,
    );
    foreach ($node->drawers as $this_item) {
      $subitem = $this_item['drawer'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' owned by ' . $this_item['owner'],
        ', status: ' . $this_item['drawerstatus'],
        ', permissions: ' . $this_item['drawerperm'],
      );
      ++$weight;
      $node->content['subdrawers'][] = array(
        '#value' => filecabinet_theme_subitem('subdrawers', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new sub-drawer
  ++$weight;
  $linktext = l(t('Add new sub-drawer'), 'filecabinet/drawers/add/parent~' . $node->nid);
  $node->content['add-subdrawer'] = array(
    '#value' => filecabinet_theme_link('add-subdrawer', $linktext),
    '#weight' => $weight,
  );
  // Display details of any folders in this drawer
  if (isset($node->folders)) {
    //orgright_debug_msg('filecabinet','folder details found',7);
    ++$weight;
    $heading = t('The following folders are stored in this drawer:');
    $node->content['folders'] = array(
      '#value' => filecabinet_theme_heading('folders', $heading),
      '#weight' => $weight,
    );
    foreach ($node->folders as $this_item) {
      $subitem = $this_item['folder'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' status: ' . $this_item['folderstatus'],
        ' permissions: ' . $this_item['folderperm'],
      );
      ++$weight;
      $node->content['folders'][] = array(
        '#value' => filecabinet_theme_subitem('folders', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new folder
  ++$weight;
  $linktext = l(t('Add new folder'), 'filecabinet/folders/add/drawer~' . $node->nid);
  $node->content['add-folder'] = array(
    '#value' => filecabinet_theme_link('add-folder', $linktext),
    '#weight' => $weight,
  );
  // Include a link to add a new drawer (with same parent if any)
  ++$weight;
  $reftext = ($node->wnid) ? '/parent~' . $node->wnid : '';
  $linktext = l(t('Add new drawer'), 'filecabinet/drawers/add' . $reftext);
  $node->content['add-drawer'] = array(
    '#value' => filecabinet_theme_link('add-drawer', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet', t('Drawers') => 'filecabinet/drawers'));
}

function filecabinet_folder_view(&$node, $teaser, $page) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_folder_view');
  // Get metadata for this node type - used for labeling body field
  $nodetype = node_get_types('type', $node);
  // Generate the content display information
  // note: the $node->content['body'] will always have a weight of 0.
  $description = t('Folder');
  $node->content['folder'] = array(
    '#value' => filecabinet_theme_item('folder', $description, $node->folder),
    '#weight' => -5,
  );
  if ($node->fnid) {
    $description = t('Parent folder');
    $node->content['parent'] = array(
      '#value' => filecabinet_theme_item('parent', $description, $node->parent, $node->fnid),
      '#weight' => -4,
    );
  }
  if ($node->wnid) {
    $description = t('Drawer');
    $node->content['drawer'] = array(
      '#value' => filecabinet_theme_item('drawer', $description, $node->drawer, $node->wnid),
      '#weight' => -3,
    );
  }
  $description = t('Status');
  $node->content['folderstatus'] = array(
    '#value' => filecabinet_theme_item('folderstatus', $description, $node->folderstatus),
    '#weight' => -2,
  );
  $description = check_plain($nodetype->body_label);
  $node->content['body-label'] = array(
    '#value' => filecabinet_theme_item('body-label', $description, ''),
    '#weight' => -1,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => filecabinet_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Check to see if there are any sub-folder details
  if (isset($node->folders)) {
    //orgright_debug_msg('filecabinet','folder details found',7);
    ++$weight;
    $heading = t('The following sub-folders are stored in this folder:');
    $node->content['subfolders'] = array(
      '#value' => filecabinet_theme_heading('subfolders', $heading),
      '#weight' => $weight,
    );
    foreach ($node->folders as $this_item) {
      $subitem = $this_item['folder'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' status: ' . $this_item['folderstatus'],
        ' permissions: ' . $this_item['folderperm'],
      );
      ++$weight;
      $node->content['subfolders'][] = array(
        '#value' => filecabinet_theme_subitem('subfolders', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new folder
  ++$weight;
  $linktext = l(t('Add new sub-folder'), 'filecabinet/folders/add/folder~' . $node->nid);
  $node->content['add-subfolder'] = array(
    '#value' => filecabinet_theme_link('add-subfolder', $linktext),
    '#weight' => $weight,
  );
  // check whether any stored document details are loaded
  if (isset($node->documents)) {
    //orgright_debug_msg('filecabinet','document details found',7);
    ++$weight;
    $heading = t('The following documents are stored in this folder:');
    $node->content['documents'] = array(
      '#value' => filecabinet_theme_heading('documents', $heading),
      '#weight' => $weight,
    );
    foreach ($node->documents as $this_item) {
      // Prepare the document date for display
      $docdate = date_format_date(date_make_date($this_item['docdate']), 'custom', 'd M Y');
      $subitem = $this_item['docname'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' version: ' . $this_item['docversion'],
        ' dated: ' . $docdate,
        ' by ' . l($this_item['member'], 'node/' . $this_item['mnid']),
        ' type: ' . $this_item['doctype'],
        ' status: ' . $this_item['docstatus'],
      );
      ++$weight;
      $node->content['documents'][] = array(
        '#value' => filecabinet_theme_subitem('documents', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new document
  ++$weight;
  $linktext = l(t('Add new document'), 'filecabinet/documents/add/folder~' . $node->nid);
  $node->content['add-document'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // Include a link to add a new folder
  ++$weight;
  $reftext = ($node->wnid) ? '/drawer~' . $node->wnid : '/folder~' . $node->fnid;
  $linktext = l(t('Add new folder'), 'filecabinet/folders/add' . $reftext);
  $node->content['add-folder'] = array(
    '#value' => filecabinet_theme_link('add-folder', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet', t('Folders') => 'filecabinet/folders'));
}

function filecabinet_document_view(&$node, $teaser, $page) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_document_view');
  // Get metadata for this node type - used for labeling body field
  $nodetype = node_get_types('type', $node);
  // Prepare the document date for display
  $docdate = date_format_date(date_make_date($node->docdate), 'custom', 'd M Y');
  // Generate the content display information
  // note: the $node->content['body'] will always have a weight of 0.
  $description = t('Document');
  $node->content['docname'] = array(
    '#value' => filecabinet_theme_item('docname', $description, $node->docname),
    '#weight' => -10,
  );
  if ($node->afid) {
    $description = t('Associated file');
    // Create details of attached file - name plus size, and indicate whether the contents have been scanned
    $file_url = file_create_url($node->filepath);
    $filedetail = l($node->filename, $file_url, array('attributes' => array('target' => '_blank')));
    $filedetail .= ' (' . filecabinet_display_filesize($node->filesize) . ')';
    $filedetail .= ($node->scanned) ? '' : '<br />*** NOT YET SCANNED ***';
    $node->content['filename'] = array(
      '#value' => filecabinet_theme_item('filename', $description, $filedetail),
      '#weight' => -9,
    );
    $description = t('File mime type');
    $node->content['filemime'] = array(
      '#value' => filecabinet_theme_item('filemime', $description, $node->filemime),
      '#weight' => -8,
    );
  }
  $description = t('Document version');
  $node->content['docversion'] = array(
    '#value' => filecabinet_theme_item('docversion', $description, $node->docversion),
    '#weight' => -7,
  );
  $description = t('Document date');
  $node->content['docdate'] = array(
    '#value' => filecabinet_theme_item('docdate', $description, $docdate),
    '#weight' => -6,
  );
  if ($node->mnid) {
    $description = t('Person');
    $node->content['member'] = array(
      '#value' => filecabinet_theme_item('member', $description, $node->member, $node->mnid),
      '#weight' => -3,
    );
  }
  $description = check_plain($nodetype->body_label);
  $nodebody = $teaser ? check_markup($node->teaser) : check_markup($node->body);
  $node->content['nodebody'] = array(
    '#value' => filecabinet_theme_item('nodebody', $description, $nodebody),
    '#weight' => 0,
  );
  $description = t('Status');
  $node->content['docstatus'] = array(
    '#value' => filecabinet_theme_item('docstatus', $description, $node->docstatus),
    '#weight' => 2,
  );
  $description = t('Type');
  $node->content['doctype'] = array(
    '#value' => filecabinet_theme_item('doctype', $description, $node->doctype),
    '#weight' => 3,
  );
  $description = t('Description');
  $docdesc = check_markup($node->docdesc);
  $node->content['docdesc'] = array(
    '#value' => filecabinet_theme_item('docdesc', $description, $docdesc),
    '#weight' => 4,
  );
  $description = t('Head document');
  $node->content['headoc'] = array(
    '#value' => filecabinet_theme_item('folder', $description, $node->headoc, $node->dnid),
    '#weight' => 5,
  );
  $description = t('Folder');
  $node->content['folder'] = array(
    '#value' => filecabinet_theme_item('folder', $description, $node->folder, $node->fnid),
    '#weight' => 6,
  );
  $description = t('Drawer');
  $node->content['drawer'] = array(
    '#value' => filecabinet_theme_item('drawer', $description, $node->drawer, $node->wnid),
    '#weight' => 7,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => filecabinet_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Include a link to create a new version of this document
  ++$weight;
  $linktext = l(t('Create new version'), 'filecabinet/documents/add/newversion~' . $node->nid);
  $node->content['new-version'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // check whether any stored attachment details are loaded
  if (isset($node->attachments)) {
    //orgright_debug_msg('filecabinet','attachment details found',7);
    ++$weight;
    $heading = t('This document has the following attachments:');
    $node->content['attachments'] = array(
      '#value' => filecabinet_theme_heading('attachments', $heading),
      '#weight' => $weight,
    );
    foreach ($node->attachments as $this_item) {
      // Prepare the attached document date for display
      $docdate = date_format_date(date_make_date($this_item['docdate']), 'custom', 'd M Y');
      $subitem = $this_item['docname'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' version: ' . $this_item['docversion'],
        ' dated: ' . $docdate,
        ' by ' . l($this_item['member'], 'node/' . $this_item['mnid']),
        ' type: ' . $this_item['doctype'],
        ' status: ' . $this_item['docstatus'],
      );
      ++$weight;
      $node->content['attachments'][] = array(
        '#value' => filecabinet_theme_subitem('documents', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new attachment to this document
  ++$weight;
  $linktext = l(t('Add new attachment'), 'filecabinet/documents/add/attachment~' . $node->nid);
  $node->content['add-attachment'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // Include a link to add new document into same folder
  ++$weight;
  $linktext = l(t('Add new document'), 'filecabinet/documents/add/folder~' . $node->fnid);
  $node->content['add-document'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // check whether any stored doclink details are loaded
  if (isset($node->doclinks)) {
    //orgright_debug_msg('filecabinet','doclink details found',7);
    ++$weight;
    $heading = t('This document is linked to the following items:');
    $node->content['doclinks'] = array(
      '#value' => filecabinet_theme_heading('doclinks', $heading),
      '#weight' => $weight,
    );
    foreach ($node->doclinks as $this_link) {
      // Prepare the document date for display
      $docdate = date_format_date(date_make_date($this_item['docdate']), 'custom', 'd M Y');
      $subitem = $this_link['nodetitle'];
      $ref = $this_link['xnid'];
      $attributes = array(
        ', Item type: ' . $this_link['nodetype'],
        ', Link type: ' . $this_link['linktype'],
        ' ' . l('[edit link]', 'filecabinet/documents/doclink/edit/document~' . $node->nid . '/node~' . $this_link['xnid']),
      );
      ++$weight;
      $node->content['doclinks'][] = array(
        '#value' => filecabinet_theme_subitem('doclinks', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new doclink
  ++$weight;
  $linktext = l(t('Link this document'), 'filecabinet/documents/doclink/document~' . $node->nid);
  $node->content['add-doclink'] = array(
    '#value' => filecabinet_theme_link('add-doclink', $linktext),
    '#weight' => $weight,
  );
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet', t('Documents') => 'filecabinet/documents'));
}

/**
 * Specific theming functions
 */
function filecabinet_theme_heading($heading_class, $heading_value) {
  return theme('orgright_heading', 'filecabinet', $heading_class, $heading_value);
}

function filecabinet_theme_item($item_class, $description, $item_value, $node_ref = 0) {
  // General function for themeing an item value to be displayed.
  return theme('orgright_item', 'filecabinet', $item_class, $description, $item_value, $node_ref);
}

function filecabinet_theme_subitem($subitem_class, $subitem_value, $node_ref, $attributes) {
  // General function for themeing a sub-item set of values to be displayed.
  return theme('orgright_subitem', 'filecabinet', $subitem_class, $subitem_value, $node_ref, $attributes);
}

//Function to theme ajax calls - for now, done in line, but the potential
// exists for theming if this is ever felt to be appropriate
function filecabinet_theme_ajax_subitem($subitem_class, $subitem_value, $node_ref) {
  $output = '';
  $output .= '<div id="' . $subitem_class . $node_ref . '">';
  $output .= '<a style="cursor: pointer" onclick="show' . $subitem_class . '(' . $node_ref . ')">';
  $output .= '<img src="' . drupal_get_path('module', 'filecabinet') . '/images/' . $subitem_class . '.png" />';
  $output .= $subitem_value . '</a></div>';
  $output .= '<div id="content' . $node_ref . '" style="margin-left: 20px"></div>';

  return $output;
}

function filecabinet_theme_link($link_class, $link_value) {
  return theme('orgright_link', 'filecabinet', $link_class, $link_value);
}

//=IAM= Updates follow
// Same as _view above but without some of the details
// and all contents fall under the 'nodecontents' heading so that they
// don't get stripped by post processing

function filecabinet_container_ajax(&$node) {
  // for processing drawers and folders
  $weight = 1;
  if (isset($node->drawers)) {
    foreach ($node->drawers as $subdrawer) {
      $drawer = $subdrawer['drawer'];
      $id = $subdrawer['nid'];
      $node->content['nodecontents'][] = array(
        '#value' => filecabinet_theme_ajax_subitem('drawer', $drawer, $id),
        '#weight' => $weight
      );
      ++$weight;
    }
  }

  if (isset($node->folders)) {
    foreach ($node->folders as $subfolder) {
      $folder = $subfolder['folder'];
      $id = $subfolder['nid'];
      $node->content['nodecontents'][] = array(
        '#value' => filecabinet_theme_ajax_subitem('folder', $folder, $id),
        '#weight' => $weight
      );
      ++$weight;
    }
  }

  if (isset($node->documents)) {
    foreach ($node->documents as $document) {
      $doc = $document['docname'];
      $id = $document['nid'];
      $node->content['nodecontents'][] = array(
        '#value' => filecabinet_theme_ajax_subitem('document', $doc, $id),
        '#weight' => $weight
      );
      ++$weight;
    }
  }

  //Is it empty?
  if (!isset($node->content['nodecontents'])) {
    $node->content['nodecontents'][] = array(
      '#value' => 'No contents',
      '#weight' => $weight
    );
  }
}

function filecabinet_document_ajax(&$node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_document_ajax');
  // Get metadata for this node type - used for labeling body field
  $nodetype = node_get_types('type', $node);
  // Prepare the document date for display
  $docdate = date_format_date(date_make_date($node->docdate), 'custom', 'd M Y');
  // Generate the content display information
  // note: the $node->content['body'] will always have a weight of 0.
  $description = t('Document');
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('docname', $description, $node->docname),
    '#weight' => -10,
  );
  if ($node->afid) {
    $description = t('Associated file');
    // Create details of attached file - name plus size, and indicate whether the contents have been scanned
    $file_url = file_create_url($node->filepath);
    $filedetail = l($node->filename, $file_url, array('attributes' => array('target' => '_blank')));
    $filedetail .= ' (' . filecabinet_display_filesize($node->filesize) . ')';
    $filedetail .= ($node->scanned) ? '' : '<br />*** NOT YET SCANNED ***';
    $node->content['nodecontents'][] = array(
      '#value' => filecabinet_theme_item('filename', $description, $filedetail),
      '#weight' => -9,
    );
    $description = t('File mime type');
    $node->content['nodecontents'][] = array(
      '#value' => filecabinet_theme_item('filemime', $description, $node->filemime),
      '#weight' => -8,
    );
  }
  $description = t('Document version');
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('docversion', $description, $node->docversion),
    '#weight' => -7,
  );
  $description = t('Document date');
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('docdate', $description, $docdate),
    '#weight' => -6,
  );
  if ($node->mnid) {
    $description = t('Person');
    $node->content['nodecontents'][] = array(
      '#value' => filecabinet_theme_item('member', $description, $node->member, $node->mnid),
      '#weight' => -3,
    );
  }


//TODO include this???
  $description = check_plain($nodetype->body_label);
  $nodebody = $teaser ? check_markup($node->teaser) : check_markup($node->body);
  $node->content['nodebody'] = array(
    '#value' => filecabinet_theme_item('nodebody', $description, $nodebody),
    '#weight' => 0,
  );
//END TODO

  $description = t('Status');
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('docstatus', $description, $node->docstatus),
    '#weight' => 2,
  );
  $description = t('Type');
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('doctype', $description, $node->doctype),
    '#weight' => 3,
  );
  $description = t('Description');
  $docdesc = check_markup($node->docdesc);
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('docdesc', $description, $docdesc),
    '#weight' => 4,
  );
  $description = t('Head document');
  $node->content['nodecontents'][] = array(
    '#value' => filecabinet_theme_item('folder', $description, $node->headoc, $node->dnid),
    '#weight' => 5,
  );
  // If a teaser is being generated, then we are done
  if ($teaser) {
    return;
  }
  // else continue adding more detail
  $description = t('Notes');
  $notes = check_markup($node->notes, orgright_get_field_filter());
  $node->content['notes'] = array(
    '#value' => filecabinet_theme_item('notes', $description, $notes),
    '#weight' => 9,
  );
  $weight = 10;
  // Include a link to create a new version of this document
  ++$weight;
  $linktext = l(t('Create new version'), 'filecabinet/documents/add/newversion~' . $node->nid);
  $node->content['new-version'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // check whether any stored attachment details are loaded
  if (isset($node->attachments)) {
    //orgright_debug_msg('filecabinet','attachment details found',7);
    ++$weight;
    $heading = t('This document has the following attachments:');
    $node->content['attachments'] = array(
      '#value' => filecabinet_theme_heading('attachments', $heading),
      '#weight' => $weight,
    );
    foreach ($node->attachments as $this_item) {
      // Prepare the attached document date for display
      $docdate = date_format_date(date_make_date($this_item['docdate']), 'custom', 'd M Y');
      $subitem = $this_item['docname'];
      $ref = $this_item['nid'];
      $attributes = array(
        ' version: ' . $this_item['docversion'],
        ' dated: ' . $docdate,
        ' by ' . l($this_item['member'], 'node/' . $this_item['mnid']),
        ' type: ' . $this_item['doctype'],
        ' status: ' . $this_item['docstatus'],
      );
      ++$weight;
      $node->content['attachments'][] = array(
        '#value' => filecabinet_theme_subitem('documents', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new attachment to this document
  ++$weight;
  $linktext = l(t('Add new attachment'), 'filecabinet/documents/add/attachment~' . $node->nid);
  $node->content['add-attachment'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // Include a link to add new document into same folder
  ++$weight;
  $linktext = l(t('Add new document'), 'filecabinet/documents/add/folder~' . $node->fnid);
  $node->content['add-document'] = array(
    '#value' => filecabinet_theme_link('add-document', $linktext),
    '#weight' => $weight,
  );
  // check whether any stored doclink details are loaded
  if (isset($node->doclinks)) {
    //orgright_debug_msg('filecabinet','doclink details found',7);
    ++$weight;
    $heading = t('This document is linked to the following items:');
    $node->content['doclinks'] = array(
      '#value' => filecabinet_theme_heading('doclinks', $heading),
      '#weight' => $weight,
    );
    foreach ($node->doclinks as $this_link) {
      // Prepare the document date for display
      $docdate = date_format_date(date_make_date($this_item['docdate']), 'custom', 'd M Y');
      $subitem = $this_link['nodetitle'];
      $ref = $this_link['xnid'];
      $attributes = array(
        ', Item type: ' . $this_link['nodetype'],
        ', Link type: ' . $this_link['linktype'],
        ' ' . l('[edit link]', 'filecabinet/documents/doclink/edit/document~' . $node->nid . '/node~' . $this_link['xnid']),
      );
      ++$weight;
      $node->content['doclinks'][] = array(
        '#value' => filecabinet_theme_subitem('doclinks', $subitem, $ref, $attributes),
        '#weight' => $weight,
      );
    }
  }
  // Include a link to add new doclink
  ++$weight;
  $linktext = l(t('Link this document'), 'filecabinet/documents/doclink/document~' . $node->nid);
  $node->content['add-doclink'] = array(
    '#value' => filecabinet_theme_link('add-doclink', $linktext),
    '#weight' => $weight,
  );
}
