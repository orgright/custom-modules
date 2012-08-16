<?php
// $Id$

/**
 * @file
 * Provides additional functions for filecabinet module.
 */

/**
 * Load functions called from hook_load() in module file
 */
function filecabinet_drawer_load($node) {
  $query = "SELECT w.wnid, w.ouid, w.drawer, w.drawerstatus, w.drawerperm, w.notes FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}

function filecabinet_folder_load($node) {
  $query = "SELECT f.wnid, f.fnid, f.folder, f.folderstatus, f.folderperm, f.notes FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
  $nobj = db_fetch_object(db_query($query, $node->nid));
  //orgright_debug_msg('filecabinet','folder load, fnid='.$nobj->fnid);
  return $nobj;
}

function filecabinet_document_load($node) {
  $query = "SELECT d.fnid, d.dnid, d.mnid, d.afid, d.scanned, d.docname, d.docversion, d.docdate, d.doctype, d.docstatus, d.docdesc, d.notes FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
  return db_fetch_object(db_query($query, $node->nid));
}


/**
 * Node manipulation functions called from hook_nodeapi() in module file
 */
function filecabinet_drawer_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_drawer_nodeapi',6);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      if ($node->ouid) {
        // Load owner name
        $node->owner = db_result(db_query("SELECT u.name FROM {users} u WHERE u.uid = %d",$node->ouid));
      }
      if ($node->wnid) {
        // Load parent drawer reference
        $sql = "SELECT w.drawer FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
        $node->parent = db_result(db_query($sql, $node->wnid));
      }
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('wnid','ouid','drawer','drawerstatus','drawerperm');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of subsidiary drawers
      $sql = "SELECT w.nid, w.ouid, w.drawer, w.drawerstatus, w.drawerperm FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.wnid = %d ORDER BY w.drawer";
      $result = db_query($sql, $node->nid);
      while ($drawer = db_fetch_object($result)) {
        // find name of owner (user)
        $drawer->owner = db_result(db_query("SELECT u.name FROM {users} u WHERE u.uid = %d",$drawer->ouid));
        $node->drawers[] = (array)$drawer;
      }
      // Load an array of folders
      $sql = "SELECT f.nid, f.folder, f.folderstatus, f.folderperm FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.wnid = %d ORDER BY f.folder";
      $result = db_query($sql, $node->nid);
      while ($folder = db_fetch_object($result)) {
        $node->folders[] = (array)$folder;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_drawer_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_drawer_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('filecabinet_event_drawer_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_drawer_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from drawers table
      db_query("DELETE FROM {org_drawers} WHERE vid = %d", $node->vid);
      return;
  }
}

function filecabinet_folder_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_subscription_nodeapi',6);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      if ($node->wnid) {
        // Load drawer reference
        $sql = "SELECT w.ouid, w.drawer, w.drawerstatus, w.drawerperm FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
        $drawer = db_fetch_object(db_query($sql, $node->wnid));
        $node->ouid = $drawer->ouid;
        $node->drawer = $drawer->drawer;
        $node->drawerstatus = $drawer->drawerstatus;
        $node->drawerperm = $drawer->drawerperm;
        // find associated user name
        $sql = "SELECT u.name FROM {users} u WHERE u.uid = %d";
        $node->owner = db_result(db_query($sql, $drawer->ouid));
      }
      if ($node->fnid) {
        // Load parent folder reference
        $sql = "SELECT f.folder, f.folderstatus, f.folderperm FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
        $parent = db_fetch_object(db_query($sql, $node->fnid));
        $node->parent = $parent->folder;
        $node->parentstatus = $parent->folderstatus;
        $node->parentperm = $parent->folderperm;
      }
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('fnid','wnid','folder','folderstatus','folderperm');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of sub-folders
      $sql = "SELECT f.nid, f.folder, f.folderstatus, f.folderperm FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.fnid = %d ORDER BY f.folder";
      $result = db_query($sql, $node->nid);
      while ($folder = db_fetch_object($result)) {
        $node->folders[] = (array)$folder;
      }
      // Load an array of documents
      $sql = "SELECT d.nid, d.mnid, d.afid, d.docname, d.docversion, d.docdate, d.doctype, d.docstatus FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.fnid = %d ORDER BY d.docname";
      $result = db_query($sql, $node->nid);
      while ($document = db_fetch_object($result)) {
        // find associated member name
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $document->member = db_result(db_query($sql, $document->mnid));
        // find associated attached file
        $sql = "SELECT filename, filepath, filesize FROM {files} where fid = %d";
        $file = db_fetch_object(db_query($sql, $document->afid));
        $document->filename = $file->filename;
        $document->filepath = $file->filepath;
        $document->filesize = $file->filesize;
        $node->documents[] = (array)$document;
      }
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_folder_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_folder_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('filecabinet_event_folder_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_folder_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from agenda items table
      db_query("DELETE FROM {org_folders} WHERE vid = %d", $node->vid);
      return;
  }
}

function filecabinet_document_nodeapi(&$node, $op, $teaser, $page) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_subpayment_nodeapi',6);
  switch ($op) {
    case 'load':
      // Node is being loaded, so add any additional data to $node
      if ($node->fnid) {
        // Load folder reference
        $sql = "SELECT f.wnid, f.folder, f.folderstatus FROM {node} n JOIN {org_folders} f ON f.nid = n.nid WHERE f.vid = n.vid AND f.nid = %d";
        $folder = db_fetch_object(db_query($sql, $node->fnid));
        $node->wnid = $folder->wnid;
        $node->folder = $folder->folder;
        $node->folderstatus = $folder->folderstatus;
        // find associated drawer name
        $sql = "SELECT w.drawer FROM {node} n JOIN {org_drawers} w ON w.nid = n.nid WHERE w.vid = n.vid AND w.nid = %d";
        $node->drawer = db_result(db_query($sql, $folder->wnid));
      }
      if ($node->dnid) {
        // Load head document reference
        $sql = "SELECT d.fnid, d.docname, d.doctype, d.docstatus FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.nid = %d";
        $document = db_fetch_object(db_query($sql, $node->dnid));
        $node->headoc = $document->docname;
      }
      if ($node->mnid) {
        // Load member reference
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $node->member = db_result(db_query($sql, $node->mnid));
      }
      if ($node->afid) {
        // Load attached file reference
        $sql = "SELECT filename, filepath, filesize, filemime FROM {files} where fid = %d";
        $file = db_fetch_object(db_query($sql, $node->afid));
        $node->filename = $file->filename;
        $node->filepath = $file->filepath;
        $node->filesize = $file->filesize;
        $node->filemime = $file->filemime;
      }
      // Stop here if a teaser display is required
      if ($teaser) { return; }
      // Specify the fields that are used for rules event triggers on change
      $fields = array('fnid','mnid','afid','docname','docversion','docdate','doctype','docstatus');
      $original = array();
      foreach ($fields as $field) { $original[$field] = $node->$field; }
      $node->original = serialize($original);
      $node->rules_update = FALSE;
      // Load an array of attached documents
      $sql = "SELECT d.nid, d.mnid, d.afid, d.docname, d.docversion, d.docdate, d.doctype, d.docstatus FROM {node} n JOIN {org_documents} d ON d.nid = n.nid WHERE d.vid = n.vid AND d.dnid = %d ORDER BY d.docname";
      $result = db_query($sql, $node->nid);
      while ($document = db_fetch_object($result)) {
        // find associated member name
        $sql = "SELECT m.member FROM {node} n JOIN {org_members} m ON m.nid = n.nid WHERE m.vid = n.vid AND m.nid = %d";
        $document->member = db_result(db_query($sql, $document->mnid));
        // find associated attached file
        $sql = "SELECT filename, filepath, filesize FROM {files} where fid = %d";
        $file = db_fetch_object(db_query($sql, $document->afid));
        $document->filename = $file->filename;
        $document->filepath = $file->filepath;
        $document->filesize = $file->filesize;
        $node->attachments[] = (array)$document;
      }
      // load an array of doclinks
      $result = db_query("SELECT * FROM {org_doclinks} WHERE dnid = %d", $node->nid);
      $num = 0;
      while ($doclink = db_fetch_object($result)) {
        $nodetitle = db_result(db_query("SELECT title FROM {node} WHERE nid = %d", $doclink->xnid));
        $node->doclinks[$num]['xnid'] = $doclink->xnid;  
        $node->doclinks[$num]['nodetype'] = $doclink->nodetype;  
        $node->doclinks[$num]['linktype'] = $doclink->linktype;
        $node->doclinks[$num]['nodetitle'] = $nodetitle;
        $num++;  
      }       
      return;
    case 'insert':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_document_create', $node);
      }
      return;
    case 'update':
      // Ignore updates caused by rules actions
      if ($node->rules_update) { return; }
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_document_update', $node);
        if ($original = unserialize($node->original)) {
          foreach ($original as $field => $value) {
            if ($node->$field != $value) {
              rules_invoke_event('filecabinet_event_document_'.$field.'_update', $node);
            }
          }
        }
      }
      return;
    case 'delete':
      // Trigger the specified rules events
      if (module_exists('rules')) {
        rules_invoke_event('filecabinet_event_document_delete', $node);
      }
      return;
    case 'delete revision':
      // Node revision is being deleted, so delete associated row from filecabinet table
      db_query("DELETE FROM {org_documents} WHERE vid = %d", $node->vid);
      // if there was an attached file which is not used, delete it
      if ($node->afid) {
        filecabinet_delete_file($node->afid);
      }
      return;
  }
}


/**
 * Insert functions called from hook_insert() in module file
 */
function filecabinet_drawer_insert($node) {
  db_query("INSERT INTO {org_drawers} (nid, vid, wnid, ouid, drawer, drawerstatus, drawerperm, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->wnid,
    $node->ouid,
    $node->drawer,
    $node->drawerstatus,
    $node->drawerperm,
    $node->notes,
    $node->uid);
}

function filecabinet_folder_insert($node) {
  db_query("INSERT INTO {org_folders} (nid, vid, wnid, fnid, folder, folderstatus, folderperm, notes, user) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->wnid,
    $node->fnid,
    $node->folder,
    $node->folderstatus,
    $node->folderperm,
    $node->notes,
    $node->uid);
}

function filecabinet_document_insert($node) {
  db_query("INSERT INTO {org_documents} (nid, vid, fnid, dnid, mnid, afid, scanned, docname, docversion, docdate, doctype, docstatus, docdesc, notes, user) VALUES (%d, %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",
    $node->nid,
    $node->vid,
    $node->fnid,
    $node->dnid,
    $node->mnid,
    $node->afid,
    $node->scanned,
    $node->docname,
    $node->docversion,
    $node->docdate,
    $node->doctype,
    $node->docstatus,
    $node->docdesc,
    $node->notes,
    $node->uid);
}

/**
 * Update functions called from hook_update() in module file
 */
function filecabinet_drawer_update($node) {
  db_query("UPDATE {org_drawers} SET wnid = %d, ouid = %d, drawer = '%s', drawerstatus = '%s', drawerperm = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->wnid,
    $node->ouid,
    $node->drawer,
    $node->drawerstatus,
    $node->drawerperm,
    $node->notes,
    $node->uid,
    $node->vid);
}

function filecabinet_folder_update($node) {
  db_query("UPDATE {org_folders} SET wnid = %d, fnid = %d, folder = '%s', folderstatus = '%s', folderperm = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->wnid,
    $node->fnid,
    $node->folder,
    $node->folderstatus,
    $node->folderperm,
    $node->notes,
    $node->uid,
    $node->vid);
}

function filecabinet_document_update($node) {
  db_query("UPDATE {org_documents} SET fnid = %d, dnid = %d, mnid = %d, afid = %d, scanned = %d, docname = '%s', docversion = '%s', docdate = '%s', doctype = '%s', docstatus = '%s', docdesc = '%s', notes = '%s', user = %d WHERE vid = %d",
    $node->fnid,
    $node->dnid,
    $node->mnid,
    $node->afid,
    $node->scanned,
    $node->docname,
    $node->docversion,
    $node->docdate,
    $node->doctype,
    $node->docstatus,
    $node->docdesc,
    $node->notes,
    $node->uid,
    $node->vid);      
}

/**
 * Delete functions called from hook_delete() in module file
 */
function filecabinet_drawer_delete($node) {
    db_query("DELETE FROM {org_drawers} WHERE nid = %d", $node->nid);
}

function filecabinet_folder_delete($node) {
    db_query("DELETE FROM {org_folders} WHERE nid = %d", $node->nid);
}

function filecabinet_document_delete($node) {
    // First retrieve a list all files associated with the document
    $files = db_query("SELECT DISTINCT afid FROM {org_documents} WHERE nid = %d", $node->nid);
    // Then delete the document node
    db_query("DELETE FROM {org_documents} WHERE nid = %d", $node->nid);
    // delete and doclinks that exist
    db_query("DELETE FROM {org_doclinks} WHERE dnid = %d", $node->nid);
    // Now delete the files if they are not used
    module_load_include('inc', 'filecabinet', 'filecabinet_file');
    while ($file = db_fetch_object($files)) {
      filecabinet_delete_file($file->afid);
    }
}

function filecabinet_delete_file($file_id, $min=0) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_delete_file fid='.$file_id.', min='.$min);
  // The passed $min parameter is the smallest count value that allows file deletion
  // if the file is not used, delete it (it may still be used in old revisions)
  $count = db_result(db_query("SELECT COUNT(afid) FROM {org_documents} WHERE afid = %d", $file_id));
  if ($count <= $min) {
    $filepath = db_result(db_query("SELECT filepath FROM {files} WHERE fid = %d", $file_id));
    if ($filepath) { file_delete($filepath); }
  }
}

/**
 * Called from hook_file_download() in module file.
 */
function filecabinet_file_download_request($filepath) {
  //orgright_debug_msg('filecabinet','Fn: filecabinet_file_download_request file='.$filepath);
  $filepath = file_create_path($filepath);
  $sql = "SELECT d.nid, d.vid, f.filepath, f.filesize, f.filemime FROM {org_documents} d JOIN {files} f ON f.fid = d.afid WHERE f.filepath = '%s'";
  $result = db_query($sql, $filepath);
  while ($document = db_fetch_object($result)) {
    //orgright_debug_msg('file','document->filepath='.$document->filepath);
    if ($filepath === $document->filepath) {
      // Since some database servers sometimes use a case-insensitive
      // comparison by default, double check that the filename is an exact
      // match.
      return array( // temp until user access permissions sorted out
        'Content-Type: ' . $document->filemime,
        'Content-Length: ' . $document->filesize,
      );
      if (user_access('view file') && ($node = node_load($document->nid, $document->vid)) && node_access('view', $node)) {
        return array(
          'Content-Type: ' . $document->filemime,
          'Content-Length: ' . $document->filesize,
        );
      }
      else {
        return -1;
      }
    }
  }
}

