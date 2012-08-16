<?php
// $Id$

/**
 * @file
 * Provides orgRight Filing Cabinet suplementary functionality triggered by Cron
 *
 * Will scan a file embedded in a document to extract the contents and save in the node body
 */

function filecabinet_document_scan($document, &$context) {
  //orgright_debug_msg('filecabinet', 'Fn: filecabinet_document_scan, nid='.$document->nid);
  // The passed argument is a document object
  if (!isset($context['sandbox']['count'])) {
    $context['sandbox']['count'] = 0;
  }
  // First step is to load the file object and find the file path
  $file = db_fetch_object(db_query("SELECT * FROM {file} f WHERE fid = %d", $document->afid));
  // now the command to scan and extract the contents
  $command = filecabinet_get_docscan_command() . ' '. $file->path;
  // and scan the file, with the results returned in $output
  $result = exec($command, $output, $retval);
  $document->scan_retval = $retval;
  $document->scan_result = $result;
  if ($retval) {
    // now load the node and save the results
    $node = node_load($document->nid, $document->vid);
    $node->body = implode('\n', $output);
    $bodylen = strlen($node->body);
    $document->bodylen = $bodylen;
    $teaserlen = variable_get('teaser_length', 600);
    if (strlen($bodylen > $teaserlen+24)) {
      $node->teaser = substr($node->body,0, strpos($node->body, '\n', $teaserlen));
    }
    else {
      $node->teaser = $node->body;
    }
    $node->scanned = 1;
    node_save($node);
  }  
  $docindex = $context['sandbox']['count']++;
  $context['results']['document'][$docindex] = $document;
  $context['results']['count'] = $context['sandbox']['count'];
  $context['finished'] = 1;
  // all done
}

function filecabinet_document_finished_scan($success, $results, $operations) {
  //orgright_debug_msg('filecabinet', 'Fn: filecabinet_document_finished_scan, result='.$result);
  if ($success) {
    drupal_set_message(t('Successfully scanned @num embedded files', array('@num'=>$results['count'])));
  }
  else {
    // an error occurred during batch processing
    $errorop = reset($operations);
    $errfunc = array_shift($errorop);
    $errargs = array_shift($errorop);
    $strargs = implode(', ', $errargs);
    watchdog('filecabinet', "Error when calling operation '%s'('%s')", array($errfunc, $strargs));
    drupal_set_message(t('An error occured, which has been recorded in the system log.'));
  }
}
