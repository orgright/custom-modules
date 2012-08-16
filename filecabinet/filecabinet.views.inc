<?php
// $Id$

/**
 * @file
 * Provides views api functions for the filecabinet module.
 */

/**
 * Implementation of hook_views_data()
 */
function filecabinet_views_data() {
  // Define the filecabinet tables to views
  return array(
    //===== the drawers table
    'org_drawers' => array(
      'table' => array(
        'group' => t('Filecabinet'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_drawers.vid = node.vid',
          ),
        ),
      ),
      //--- wnid field
      'wnid' => array(
        'title' => t('Drawer: Parent Ref'),
        'help' => t('Parent drawer reference (if any) for a drawer recorded in the drawers table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_drawers',
          'base field' => 'nid',
        ),
      ),
      //--- ouid field
      'ouid' => array(
        'title' => t('Drawer: Owner Ref'),
        'help' => t('The login name of the owner of a drawer recorded in the drawers table'),
        'field' => array(
          'handler' => 'views_handler_field_user_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'users',
          'base field' => 'uid',
        ),
      ),
      //--- drawer field
      'drawer' => array(
        'title' => t('Drawer: Title'),
        'help' => t('The title of a drawer recorded in the drawers table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- drawerstatus field
      'drawerstatus' => array(
        'title' => t('Drawer: Status'),
        'help' => t('The status of the a drawer recorded in the drawers table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- drawerperm field
      'drawerperm' => array(
        'title' => t('Drawer: Permissions'),
        'help' => t('The permissions assigned to a drawer recorded in the drawers table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- <end of fields>
    ),

    //===== the folders table
    'org_folders' => array(
      'table' => array(
        'group' => t('Filecabinet'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_folders.vid = node.vid',
          ),
        ),
      ),
      //--- fnid field
      'fnid' => array(
        'title' => t('Folder: Parent Ref'),
        'help' => t('The parent folder reference for a folder recorded in the folders table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_folders',
          'base field' => 'nid',
        ),
      ),
      //--- wnid field
      'wnid' => array(
        'title' => t('Folder: Drawer Ref'),
        'help' => t('The drawer that is storing a folder recorded in the folders table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_drawers',
          'base field' => 'nid',
        ),
      ),
      //--- folder field
      'folder' => array(
        'title' => t('Folder: Title'),
        'help' => t('The title of a folder recorded in the folders table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- folderstatus field
      'folderstatus' => array(
        'title' => t('Folder: Status'),
        'help' => t('The status of a folder recorded in the folders table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- folderperm field
      'folderperm' => array(
        'title' => t('Folder: Permissions'),
        'help' => t('The permissions assigned to a folder in the folders table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- <end of fields>
    ),

    //===== the documents table
    'org_documents' => array(
      'table' => array(
        'group' => t('Filecabinet'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_documents.vid = node.vid',
          ),
        ),
      ),
      //--- fnid field
      'fnid' => array(
        'title' => t('Document: Folder Ref'),
        'help' => t('The reference for the folder holding a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_folders',
          'base field' => 'nid',
        ),
      ),
      //--- dnid field
      'dnid' => array(
        'title' => t('Document: Head Doc Ref'),
        'help' => t('The head document reference for a sub-document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_documents',
          'base field' => 'nid',
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Document: Member Ref'),
        'help' => t('The member responsible for a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_members',
          'base field' => 'nid',
        ),
      ),
      //--- afid field
      'afid' => array(
        'title' => t('Document: File Ref'),
        'help' => t('The reference for a file attached to a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_file_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'files',
          'base field' => 'fid',
        ),
      ),
      //--- scanned field
      'scanned' => array(
        'title' => t('Document: Scanned'),
        'help' => t('Flag showing whether a scan has been completed for a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- docname field
      'docname' => array(
        'title' => t('Document: Title'),
        'help' => t('The title of a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- docversion field
      'docversion' => array(
        'title' => t('Document: Version'),
        'help' => t('The version of a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- docdate field
      'docdate' => array(
        'title' => t('Document: Date'),
        'help' => t('The date of a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- doctype field
      'doctype' => array(
        'title' => t('Document: Type'),
        'help' => t('The type of a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- docstatus field
      'docstatus' => array(
        'title' => t('Document: Status'),
        'help' => t('The status of a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- docdesc field
      'docdesc' => array(
        'title' => t('Document: Description'),
        'help' => t('The description of a document recorded in the documents table'),
        'field' => array(
          'handler' => 'views_handler_field_node_link',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- <end of fields>
    ),

    //===== <end of tables>
  );
}
