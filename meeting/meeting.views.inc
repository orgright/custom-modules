<?php
// $Id$

/**
 * @file
 * Provides views api functions for the meeting module.
 */

/**
 * Implementation of hook_views_data()
 */
function meeting_views_data() {
  // Define the meeting tables to views
  return array(
    //===== the meetings table
    'org_meetings' => array(
      'table' => array(
        'group' => t('Meeting'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_meetings.vid = node.vid',
          ),
        ),
      ),
      //--- cnid field
      'cnid' => array(
        'title' => t('Meeting: Committee Ref'),
        'help' => t('The committee associated with a meeting as recorded in the meetings table'),
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
          'base' => 'org_committees',
          'base field' => 'nid',
        ),
      ),
      //--- meeting field
      'meeting' => array(
        'title' => t('Meeting: Title'),
        'help' => t('The title of the meeting recorded in the meetings table'),
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
      //--- meetingtype field
      'meetingtype' => array(
        'title' => t('Meeting: Type'),
        'help' => t('The meeting type code recorded in the meetings table'),
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
      //--- meetingstatus field
      'meetingstatus' => array(
        'title' => t('Meeting: Status'),
        'help' => t('The meeting status code recorded in the meetings table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
          'name field' => 'title',
          'numeric' => FALSE
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      //--- meetingdate field
      'meetingdate' => array(
        'title' => t('Meeting: Date'),
        'help' => t('The meeting date recorded in the meetings table'),
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
      //--- start field
      'start' => array(
        'title' => t('Meeting: Start Time'),
        'help' => t('The start time of the meeting recorded in the meetings table'),
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
      //--- finish field
      'finish' => array(
        'title' => t('Meeting: Finish Time'),
        'help' => t('The finish time of the meeting recorded in the meetings table'),
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
      //--- location field
      'location' => array(
        'title' => t('Meeting: Location'),
        'help' => t('The meeting location recorded in the meetings table'),
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

    //===== the agenda_items table
    'org_agenda_items' => array(
      'table' => array(
        'group' => t('Meeting'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_agenda_items.vid = node.vid',
          ),
        ),
      ),
      //--- gnid field
      'gnid' => array(
        'title' => t('Agenda Item: Meeting Ref'),
        'help' => t('The meeting associated with an agenda item recorded in the agenda_items table'),
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
          'base' => 'org_meetings',
          'base field' => 'nid',
        ),
      ),
      //--- sequence field
      'sequence' => array(
        'title' => t('Agenda Item: Sequence Number'),
        'help' => t('The sequence number of an agenda item recorded in the agenda_items table'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
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
      //--- agendaitem field
      'agendaitem' => array(
        'title' => t('Agenda Item: Title'),
        'help' => t('The agenda item title recorded in the agenda_items table'),
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
      //--- agitstatus field
      'agitstatus' => array(
        'title' => t('Agenda Item: Status'),
        'help' => t('The agenda item status code recorded in the agenda_items table'),
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
      //--- <end of fields>
    ),

    //===== the mtg_items table
    'org_mtg_items' => array(
      'table' => array(
        'group' => t('Meeting'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_mtg_items.vid = node.vid',
          ),
        ),
      ),
      //--- gnid field
      'gnid' => array(
        'title' => t('Mtg Item: Meeting Ref'),
        'help' => t('The meeting associated with a meeting item recorded in the mtg_items table'),
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
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'org_meetings',
          'base field' => 'nid',
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Mtg Item: Member Ref'),
        'help' => t('The member associated with a meeting item recorded in the mtg_items table'),
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
      //--- itemname field
      'itemname' => array(
        'title' => t('Mtg Item: Title'),
        'help' => t('The title of a meeting item recorded in the mtg_items table'),
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
      //--- itemtype field
      'itemtype' => array(
        'title' => t('Mtg Item: Type'),
        'help' => t('The type of a meeting item recorded in the mtg_items table'),
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
      //--- itemstatus field
      'itemstatus' => array(
        'title' => t('Mtg Item: Status'),
        'help' => t('The status of a meeting item recorded in the mtg_items table'),
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
      //--- itemdate field
      'itemdate' => array(
        'title' => t('Mtg Item: Date'),
        'help' => t('The date associated with a meeting item recorded in the mtg_items table'),
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
      //--- <end of fields>
    ),

    //=== <end of tables>
  );
}
