<?php
// $Id$

/**
 * @file
 * Provides additional functions for Filing Cabinet module.
 */

/**
 * Menu call back functions to display pages to users
 */
function filecabinet_management() {
  // Home page of the filecabinet management function - display links and description
  $output = '<p>The following filing cabinet management functions are available:</p>';
  $output .= '<h2>' . l('Manage file cabinet drawers', 'filecabinet/drawers') . '</h2>';
  $output .= '<p>List, add, update or delete filing cabinet drawers from the organisation\'s database.</p>';
  $output .= '<h2>' . l('Manage file folders', 'filecabinet/folders') . '</h2>';
  $output .= '<p>List, add, update or delete file folders for specific drawers.</p>';
  $output .= '<h2>' . l('Manage documents in the file cabinet', 'filecabinet/documents') . '</h2>';
  $output .= '<p>List, add, update or delete documents including linking them to other information details.</p>';
  //=IAM= additions follow
  $output .= filecabinet_create_top_level_cabinets_list();
  $output .= filecabinet_create_structure_container();
  // set the breadcrumb trail
  orgright_breadcrumb(array());
  return $output;
}

function filecabinet_drawers() {
  // entry page for managing drawerss
  return filecabinet_drawers_list();
}

function filecabinet_folders() {
  // entry page for managing folders
  return filecabinet_folders_list();
}

function filecabinet_documents() {
  // entry page for managing documents
  return filecabinet_documents_list();
}

function filecabinet_drawers_list() {
  // Generate a drawers list
  $output = '';
  $sql = "SELECT w.nid, w.wnid, w.ouid, w.drawer, w.drawerstatus, w.drawerperm FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid ORDER BY w.ouid, w.drawer";
  $result = db_query(db_rewrite_sql($sql));
  $ouid = 0;
  while ($drawer = db_fetch_object($result)) {
    if ($drawer->ouid <> $ouid) {
      if ($ouid) {
        $output .= '</ul>';
      }
      // Extract the owner's name
      $sql = "SELECT u.name FROM {users} u WHERE u.uid = %d";
      $owner = db_result(db_query($sql, $drawer->ouid));
      $output .= '<p class="filecabinet-list">Drawers owned by ' . $owner . '</p><ul>';
      $ouid = $drawer->ouid;
    }
    // Extract the parent drawer name (if any)
    if ($drawer->wnid) {
      $sql = "SELECT w.drawer FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
      $parent = db_result(db_query($sql, $drawer->wnid));
    }
    // Generate a line of listing for this drawer
    $drawer_output = '';
    $drawer_output .= l($drawer->drawer, 'node/' . $drawer->nid) . ' ';
    $drawer_output .= l('[edit]', 'node/' . $drawer->nid . '/edit');
    if ($drawer->wnid) {
      $drawer_output .= ' within drawer ' . l($parent, 'node/' . $drawer->wnid);
    }
    $drawer_output .= ', ' . strtolower($drawer->drawerstatus);
    $drawer_output .= ', ' . strtolower($drawer->drawerperm);
    $output .= '<li>' . $drawer_output . '</li>';
  }
  if ($ouid) {
    $output .= '</ul>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  return $output;
}

function filecabinet_drawers_add() {
  // Add a new drawer
  //orgright_debug_msg('filecabinet','Fn: filecabinet_drawers_add',6);
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('drawer');
  // Set the page title to display
  drupal_set_title(t('Create Drawer'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('drawer_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  // and return results
  return $output;
}

function filecabinet_folders_list() {
  // Generate a list of folders
  $output = '';
  $sql = "SELECT f.nid, f.wnid, f.fnid, f.folder, f.folderstatus, f.folderperm FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid ORDER BY f.wnid, f.folder";
  $result = db_query(db_rewrite_sql($sql));
  $folders = array();
  while ($folder = db_fetch_object($result)) {
    // find the drawer details and save object in an array
    if (!$folder->wnid) {
      $folder->wnid = filecabinet_get_folder_drawer($folder->fnid);
    }
    $sql = "SELECT w.drawer FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
    $folder->drawer = db_result(db_query($sql, $folder->wnid));
    $folders[$folder->drawer . '-' . $folder->folder] = $folder;
  }
  if (!$folders) {
    return '';
  }
  // now sort the array on the drawer-folder key
  ksort($folders);
  $wnid = 0;
  foreach ($folders as $folder) {
    if ($folder->wnid <> $wnid) {
      if ($wnid) {
        $output .= '</ul>';
      }
      $output .= '<p class="filecabinet-list">Folders in drawer ' . l($folder->drawer, 'node/' . $folder->wnid) . '</p><ul>';
      $wnid = $folder->wnid;
    }
    // Extract the parent folder (if any)
    if ($folder->fnid) {
      $sql = "SELECT f.folder FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
      $parent = db_result(db_query($sql, $folder->fnid));
    }
    // Generate a line of listing for this folder
    $folder_output = '';
    $folder_output .= l($folder->folder, 'node/' . $folder->nid) . ' ';
    $folder_output .= l('[edit]', 'node/' . $folder->nid . '/edit');
    $folder_output .= ', status ' . strtolower($folder->folderstatus);
    if ($folder->fnid) {
      $folder_output .= ', within folder ' . l($parent, 'node/' . $folder->fnid);
    }
    $output .= '<ul>' . $folder_output . '</ul>';
  }
  if ($wnid > -1) {
    $output .= '</ul>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  return $output;
}

function filecabinet_folders_add() {
  // Add a new folder
  //orgright_debug_msg('filecabinet','Fn: filecabinet_folders_add',6);
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('folder');
  // Set the page title to display
  drupal_set_title(t('Create Folder'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('folder_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  // and return results
  return $output;
}

function filecabinet_documents_list() {
  // Generate a list of documents
  $output = '';
  $sql = "SELECT d.nid, d.fnid, d.dnid, d.mnid, d.afid, d.docname, d.docversion, d.docdate, d.doctype, d.docstatus FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid ORDER BY d.fnid, d.docname, d.docversion";
  $result = db_query(db_rewrite_sql($sql));
  $fnid = 0;
  while ($document = db_fetch_object($result)) {
    // find the folder details and save object in an array
    if (!$document->fnid) {
      $document->fnid = filecabinet_get_document_folder($document->dnid);
    }
    $sql = "SELECT f.folder FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
    $document->folder = db_result(db_query($sql, $document->fnid));
    $documents[$document->folder . '-' . $document->docname] = $document;
  }
  if (!$documents) {
    return '';
  }
  // now sort the array on the folder-document key
  ksort($documents);
  $fnid = 0;
  foreach ($documents as $document) {
    if ($document->fnid <> $fnid) {
      if ($fnid) {
        $output .= '</ul>';
      }
      $output .= '<p class="filecabinet-list">Documents in folder ' . l($document->folder, 'node/' . $document->fnid) . '</p><ul>';
      $fnid = $document->fnid;
    }
    if ($document->dnid) {
      // Extract the head document name
      $sql = "SELECT d.docname FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
      $headoc = db_result(db_query($sql, $document->dnid));
    }
    // Extract the member name
    $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
    $member = db_result(db_query($sql, $document->mnid));
    if ($document->afid) {
      // Load attached file reference
      $sql = "SELECT filename, filepath, filesize, filemime FROM {files} where fid = %d";
      $file = db_fetch_object(db_query($sql, $document->afid));
      // Prepare the filepath URL
      $file_url = file_create_url($file->filepath);
    }
    // Prepare the date for display
    $docdate = date_format_date(date_make_date($document->docdate), 'custom', 'd M Y');
    // Generate a line of listing for this payment
    $document_output = '';
    $document_output .= l($document->docname . ' v:' . $document->docversion, 'node/' . $document->nid) . ' ';
    $document_output .= l('[edit]', 'node/' . $document->nid . '/edit');
    $document_output .= ', ' . $docdate;
    $document_output .= ', ' . l($member, 'node/' . $document->mnid);
    $document_output .= ', ' . strtolower($document->doctype);
    $document_output .= ', ' . strtolower($document->docstatus);
    if ($document->dnid) {
      $document_output .= ' attached to ' . l($headoc, 'node/' . $document->dnid);
    }
    if ($document->afid) {
      $document_output .= ', with file ' . l($file->filename, $file_url, array('attributes' => array('target' => '_blank')));
      $document_output .= ', ' . filecabinet_display_filesize($file->filesize);
    }
    $output .= '<li>' . $document_output . '</li>';
  }
  if ($fnid) {
    $output .= '</ul>';
  }
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  return $output;
}

function filecabinet_documents_add() {
  // Add a new subscription payment
  //orgright_debug_msg('filecabinet','Fn: filecabinet_documents_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // Create a new node data structure of the correct type
  $node = orgright_create_node('document');
  // Set the page title to display
  drupal_set_title(t('Create Document'));
  // Load the required include file
  module_load_include('inc', 'node', 'node.pages');
  // then call the form constructor function
  $output = drupal_get_form('document_node_form', $node, $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  // and return results
  return $output;
}

function filecabinet_doclink_add() {
  // Link a document to an item of information
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_add');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // call the form constructor function
  $output = drupal_get_form('filecabinet_doclink_form', 'add', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  // and return the results
  return $output;
}

function filecabinet_doclink_edit() {
  // Edit a document link
  //orgright_debug_msg('filecabinet','Fn: filecabinet_doclink_edit');
  // First get an array of any arguments passed to this function
  $args = func_get_args();
  // call the form constructor function
  $output = drupal_get_form('filecabinet_doclink_form', 'edit', $args);
  // Set page breadcrumb
  orgright_breadcrumb(array(t('Filing cabinet') => 'filecabinet'));
  // and return the results
  return $output;
}

/**
 * Menu call back functions to generate autocomplete data sets
 */
function filecabinet_autocomplete_drawers($string = '') {
  // Generate a list of drawers for the autocomplete js function
  $drawers = array();
  $qargs = array('%' . strtolower($string) . '%');
  $sql = "SELECT DISTINCT drawer FROM {org_drawers} WHERE LOWER(drawer) LIKE '%s'";
  $msg = 'Autocomplete drawers for: ' . $qargs[0];
  //orgright_debug_msg('filecabinet',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_drawers}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $drawers[$obj->drawer] = $obj->drawer;
  }
  //print drupal_to_js($drawers);
  // requires php 5.2:
  drupal_json($drawers);
  exit;
}

function filecabinet_autocomplete_folders($string = '') {
  // Generate a list of folders for the autocomplete js function
  $folders = array();
  $qargs = array('%' . strtolower($string) . '%');
  $sql = "SELECT DISTINCT folder FROM {org_folders} WHERE LOWER(folder) LIKE '%s'";
  $msg = 'Autocomplete folders for: ' . $qargs[0];
  //orgright_debug_msg('filecabinet',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_folders}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $folders[$obj->folder] = $obj->folder;
  }
  //print drupal_to_js($folders);
  // requires php 5.2:
  drupal_json($folders);
  exit;
}

function filecabinet_autocomplete_docnames($string = '') {
  // Generate a list of folders for the autocomplete js function
  $docnames = array();
  $qargs = array('%' . strtolower($string) . '%');
  $sql = "SELECT DISTINCT docname FROM {org_documents} WHERE LOWER(docname) LIKE '%s'";
  $msg = 'Autocomplete documents for: ' . $qargs[0];
  //orgright_debug_msg('filecabinet',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_documents}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $docnames[$obj->docname] = $obj->docname;
  }
  //print drupal_to_js($docnames);
  // requires php 5.2:
  drupal_json($docnames);
  exit;
}

function filecabinet_autocomplete_infoitems($string = '') {
  // Generate a list of folders for the autocomplete js function
  // get a list of node types that qualify as targets
  $infoitems = array();
  $qargs = array('%' . strtolower($string) . '%');
  $nodetypes = filecabinet_get_doclink_nodetypes();
  $where = '';
  foreach ($nodetypes as $type) {
    $where .= " OR type = '%s'";
    $qargs[] = $type;
  }
  if ($where) {
    $where = ' AND (' . substr($where, 4) . ')';
  }
  $sql = "SELECT DISTINCT title FROM {node} WHERE LOWER(title) LIKE '%s'" . $where;
  $msg = 'Autocomplete documents for: ' . $qargs[0];
  //orgright_debug_msg('filecabinet',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{org_documents}', 'vid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $infoitems[$obj->title] = $obj->title;
  }
  //print drupal_to_js($infoitems);
  // requires php 5.2:
  drupal_json($infoitems);
  exit;
}

function filecabinet_autocomplete_owners($string = '') {
  // Generate a list of owners for the autocomplete js function
  $owners = array();
  $qargs = array(O_MAX_PROTECTED_USER, '%' . strtolower($string) . '%');
  $sql = "SELECT DISTINCT name FROM {users} WHERE uid > %d AND LOWER(name) LIKE '%s'";
  //$msg = 'Autocomplete owners for: ' . $qargs[0];
  //orgright_debug_msg('filecabinet',$msg,8);
  //$result = db_query_range(db_rewrite_sql($sql, '{users}', 'uid', $qargs), 0, 12);
  $result = db_query_range($sql, $qargs, 0, 12);
  while ($obj = db_fetch_object($result)) {
    $owners[$obj->name] = $obj->name;
  }
  //print drupal_to_js($owners);
  // requires php 5.2:
  drupal_json($owners);
  exit;
}

//=IAM= updates follow
//Generate a drawers list (as an array)
function filecabinet_drawers_list_array() {
  // Generate a drawers list
  $output = array();
  $sql = "SELECT w.nid, w.wnid, w.ouid, w.drawer, w.drawerstatus, w.drawerperm FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid ORDER BY w.ouid, w.drawer";
  $result = db_query(db_rewrite_sql($sql));
  $ouid = 0;
  while ($drawer = db_fetch_object($result)) {
    $this_ouid = $drawer->ouid;
    $this_owners_drawers = $output[$this_ouid];
    if (!isset($this_owners_drawers)) {
      $this_owners_drawers = array();

      // Extract the owner's name
      $sql = "SELECT u.name FROM {users} u WHERE u.uid = %d";
      $owner = db_result(db_query($sql, $drawer->ouid));
      $this_owners_drawers['owner'] = $owner;
      $ouid = $this_ouid;

      $this_owners_drawers['drawers'] = array();
      $output[$this_ouid] = $this_owners_drawers;
    }
    $output[$this_ouid]['drawers'][$drawer->drawer] = array();

    // Extract the parent drawer name (if any)
    if ($drawer->wnid) {
      $sql = "SELECT w.drawer FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
      $parent = db_result(db_query($sql, $drawer->wnid));
      $output[$this_ouid]['drawers'][$drawer->drawer]['parent'] = $parent;
      $output[$this_ouid]['drawers'][$drawer->drawer]['parentId'] = $drawer->wnid;
    }
    $output[$this_ouid]['drawers'][$drawer->drawer]['name'] = $drawer->drawer;
    $output[$this_ouid]['drawers'][$drawer->drawer]['id'] = $drawer->nid;

  }
  return $output;
}

function filecabinet_create_top_level_cabinets_list() {
  $content = '<div id="scrollbar2">';
  $content .= '<div class="viewport">';
  $content .= '<div class="overview" id="scrollInners">';

  $drawerCount = 0;

  $drawersList = filecabinet_drawers_list_array();

  //$content .= print_r($drawersList, true);
  foreach ($drawersList as $user => $drawers) {
    foreach ($drawers['drawers'] as $drawer => $details) {
      //Only display it if it doesn't have a parent
      if (!isset($details['parentId'])) {
        $content .= '<div id="cabinet' . $details['id'] . '"><a style="cursor: pointer" onclick="showCabinet(' . $details['id'] . ');"><img src="' . drupal_get_path("module", "filecabinet") . '/images/cabinet.png" style="float: left" />' . $drawer . '</a></div>';
        $drawerCount++;
      }
    }
  }

  $content .= '</div></div><div class="scrollbar"><div class="track"><div class="thumb"><div class="end"></div></div></div></div></div>';

  //Also write out a script to reset the width of the scroll content appropriately
  $content .= '<script><!--' . "\n";
  $content .= '$(document).ready(function(){';
  $content .= 'document.getElementById("scrollInners").style.width = "' . ($drawerCount * 165) . 'px";';
  $content .= "$('#scrollbar2').tinyscrollbar({axis: 'x'});";
  $content .= '});';
  $content .= '//--></script>';

  return $content;
}

function filecabinet_create_structure_container() {
  $content .= '<div id="filedetails"></div>';
  $content .= '<div id="scrollbar1">';
  $content .= '<div class="scrollbar"><div class="track"><div style="top: 0px;" class="thumb"><div class="end"></div></div></div></div>';
  $content .= '<div class="viewport"><div class="overview" id="filexplorer"></div></div></div>';
  $content .= '<script><!--' . "\n";
  $content .= '$(document).ready(function(){';
  $content .= "$('#scrollbar1').tinyscrollbar({axis: 'y'});";
  $content .= '});';
  $content .= '//--></script>';

  return $content;
}
