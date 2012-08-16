<?php
// $Id$

/**
 * @file
 * Provides additional functions for filecabinet module.
 */

function filecabinet_doclink_node_load(&$node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_load');
  // The node is being ploaded, so add any doclink details to node object
  $result = db_query("SELECT * FROM {org_doclinks} WHERE xnid = %d", $node->nid);
  $doclinks = array();
  $num = 0;
  $sql = "SELECT d.nid, d.docname, d.docversion, d.docdate, d.doctype, d.docstatus FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
  while ($link = db_fetch_object($result)) {
    $document = db_fetch_object(db_query($sql, $link->dnid));
    $doclinks[$num]['nid'] = $document->nid;
    $doclinks[$num]['docname'] = $document->docname;
    $doclinks[$num]['docversion'] = $document->docversion;
    $doclinks[$num]['docdate'] = $document->docdate;
    $doclinks[$num]['docstatus'] = $document->docstatus;
    $doclinks[$num]['linktype'] = $link->linktype;
    $num++;
  }
  if ($num) {
    $node->doclinks = $doclinks;
  }
}

function filecabinet_doclink_node_view(&$node, $teaser) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_view');
  // The node is being prepared for viewing, so format any doclink details
  if (!$teaser) {
    // load the required output routines
    module_load_include('inc', 'filecabinet', 'filecabinet_page');
    $weight = 40;
    if (isset($node->doclinks)) {
      $heading = t('The following filecabinet documents are linked to this item:');
      $node->content['doclinks'] = array(
        '#value' => filecabinet_theme_heading('doclinks', $heading),
        '#weight' => $weight,
      );
      foreach ($node->doclinks as $doclink) {
        // Prepare the document date for display
        $docdate = date_format_date(date_make_date($doclink['docdate']), 'custom', 'd M Y');
        $subitem = $doclink['docname'];
        $ref = $doclink['nid'];
        $attributes = array(
          ', version '.$doclink['docversion'],
          ', '.$docdate,
          ', status '.$doclink['docstatus'],
          ' '. l('[edit link]', 'filecabinet/documents/doclink/edit/document~'.$doclink['nid'].'/node~'.$node->nid),
        );
        $weight++;
        $node->content['doclinks'][] = array(
          '#value' => filecabinet_theme_subitem('doclinks', $subitem, $ref, $attributes),
          '#weight' => $weight,
        );
      }
    }
    // Include a link to add new doclink
    ++$weight;
    $linktext = l(t('Link a filecabinet document to this item'), 'filecabinet/documents/doclink/node~'.$node->nid);
    $node->content['add-doclink'] = array(
      '#value' => filecabinet_theme_link('add-doclink', $linktext),
      '#weight' => $weight,
    );
  }
}


function filecabinet_doclink_node_validate(&$node, $form) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_validate');
  //orgright_debug_dump('filecabinet - node',$node);
  //orgright_debug_dump('filecabinet - form',$form);
  // The node form is being submitted, so validate the doclink field
  $clicked_button = $form['#post']['op'];
  if ($clicked_button == $form['buttons']['submit']['#value']) {
    // Check that any returned values for previous doclinks are OK
    $num = 1;
    while (isset($form['#post']['savename_'.$num])) {
      // previous doclink to check
      if ($form['#post']['doc_'.$num] != $form['#post']['savename_'.$num]) {
        // name has changed
        if ($form['#post']['doc_'.$num] != strip_tags($form['#post']['doc_'.$num])) {
          form_set_error('doc_'.$num,t('HTML or PHP tags are not permitted in this field'));
        }
        // Check that the name returned in "docname" is valid
        module_load_include('inc', 'filecabinet', 'filecabinet_form');
        $dnid = filecabinet_validate_docname($form['#post']['doc_'.$num]);
        if (!$dnid) {
          form_set_error('doc_'.$num,t('The name specified is not a known document'));
        }
      }
      $num++;
    }
    // Check that any name returned in "docname" is valid
    if (!empty($form['#post']['docname'])) {
      // Check that entered details of new doc are acceptable - use PHP strip_tags function to check
      if ($form['#post']['docname'] != strip_tags($form['#post']['docname'])) {
        form_set_error('docname',t('HTML or PHP tags are not permitted in this field'));
      }
      module_load_include('inc', 'filecabinet', 'filecabinet_form');
      $dnid = filecabinet_validate_docname($form['#post']['docname']);
      if (!$dnid) {
        form_set_error('docname',t('The name specified is not a known document'));
      }
    //orgright_debug_msg('filecabinet','validate: dnid='.$node->dnid.', xnid='.$node->nid);
    }
  }
  elseif ($clicked_button == $form['buttons']['delete']['#value']) {
    //orgright_debug_msg('filecabinet','--- Delete button clicked ---');
    // Check if this item has linked documents
    $count = db_result(db_query("SELECT COUNT(xnid) FROM {org_doclinks} WHERE xnid = %d", $node->nid));
    if ($count) {
      form_set_error('docname',t('This item is still linked to @num documents!', array('@num'=>$count)));
    }
  }
}


function filecabinet_doclink_node_form_alter(&$form, $form_state, $node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_form_alter');
  // The node form is being prepared for display, so add doclink field
  $form['doclink'] = array(
    '#type' => 'fieldset',
    '#title' => t('Linked Documents'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div class="attachments">',
    '#suffix' => '</div>',
    '#weight' => 25,
  );
  // create some fields for existing doclinks
  $doclink_linktype_list = filecabinet_get_doclink_linktypes();
  if (isset($node->doclinks)) {
    $num = 0;
    foreach ($node->doclinks as $doclink) {
      $num++;
      $form['doclink']['doc_'.$num] = array(
        '#type' => 'textfield',
        '#title' => t('Document'),
        '#description' => t('Document linked to this information item'),
        '#default_value' => $doclink['docname'],
        '#maxlength' => 64,
        '#size' => 40,
        '#required' => FALSE,
        '#weight' => $num*2,
        '#autocomplete_path' => 'filecabinet/autocomplete/docnames',
      );
      $form['doclink']['link_'.$num] = array(
        '#type' => 'select',
        '#title' => t('Link type'),
        '#description' => t('Type of link'),
        '#default_value' => $doclink['linktype'],
        '#options' => $doclink_linktype_list,
        '#required' => FALSE,
        '#weight' => $num*2+1,
      );
      $form['doclink']['savename_'.$num] = array(
        '#type' => 'value',
        '#value' => $doclink['docname'],
      );
      $form['doclink']['savednid_'.$num] = array(
        '#type' => 'value',
        '#value' => $doclink['dnid'],
      );
    }
    $description = t('Seclect another filecabinet document to be linked to this information item');
  }
  else {
    $description = t('Seclect a filecabinet document to be linked to this information item');
  }
  $form['doclink']['docname'] = array(
    '#type' => 'textfield',
    '#title' => t('Document'),
    '#description' => $description,
    '#default_value' => '',
    '#maxlength' => 64,
    '#size' => 40,
    '#required' => FALSE,
    '#weight' => $num*2+2,
    '#autocomplete_path' => 'filecabinet/autocomplete/docnames',
  );
  $form['doclink']['linktype'] = array(
    '#type' => 'select',
    '#title' => t('Link type'),
    '#description' => t('Select the type of link'),
    '#default_value' => $linktype,
    '#options' => $doclink_linktype_list,
    '#required' => FALSE,
    '#weight' => $num*2+3,
  );

}

function filecabinet_doclink_node_insert(&$node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_insert');
  // The node is being inserted, so add any doclink details specified
  if (!empty($node->docname)) {
    // a new doclink has been specified
    filecabinet_doclink_insert_row($node->docname, $node->nid, $node->type, $node->linktype);
  }
  if (isset($node->savename_1)) {
    // there were previous attached documents
    // convert $node to array for easier processing
    $anode = (array)$node;
    $num = 1;
    while (isset($anode['savename_'.$num])) {
      if (!empty($anode['doc_'.$num])) {
        filecabinet_doclink_insert_row($anode['doc_'.$num], $anode['nid'], $anode['type'], $anode['link_'.$num]);
      }
      $num++;
    }
  }
}

function filecabinet_doclink_node_update(&$node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_update');
  //orgright_debug_msg('filecabinet','update: dnid='.$node->dnid.', xnid='.$node->nid);
  // The node is being updated, so first delete any existing doclink details
  // and then insert any that are currently defined
  filecabinet_doclink_node_delete($node);
  filecabinet_doclink_node_insert($node);
}

function filecabinet_doclink_node_delete(&$node) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_node_delete');
  // The node is being deleted, so delete any doclink details
  db_query("DELETE FROM {org_doclinks} WHERE xnid = %d", $node->nid);
}

function filecabinet_doclink_insert_row($docname, $xnid, $nodetype, $linktype) {
  // Insert a row into the doclinks table
  module_load_include('inc', 'filecabinet', 'filecabinet_form');
  $dnid = filecabinet_validate_docname($docname);
  db_query("INSERT INTO {org_doclinks} (dnid, xnid, nodetype, linktype) VALUES (%d, %d, '%s', '%s')",
    $dnid,
    $xnid,
    $nodetype,
    $linktype);
}
