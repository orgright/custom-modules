<?php
// $Id$

/**
 * @file
 * Provides views api functions for the project module.
 */

/**
 * Implementation of hook_views_data()
 */
function project_views_data() {
  // Define the project tables to views
  return array(
    //===== the projects table
    'org_projects' => array(
      'table' => array(
        'group' => t('Project'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_projects.vid = node.vid',
          ),
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Project: Member Ref'),
        'help' => t('The project manager for a project recorded in the projects table'),
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
      //--- project field
      'project' => array(
        'title' => t('Project: Title'),
        'help' => t('The title for a project recorded in the projects table'),
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
      //--- projcategory field
      'projcategory' => array(
        'title' => t('Project: Category'),
        'help' => t('The category for a project recorded in the projects table'),
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
      //--- projstatus field
      'mnid' => array(
        'title' => t('Project: Status'),
        'help' => t('The status for a project recorded in the projects table'),
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
      //--- projdatype field
      'projdatype' => array(
        'title' => t('Project: Date Constraint'),
        'help' => t('The date constraint for a project recorded in the projects table'),
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
      //--- projdate field
      'projdate' => array(
        'title' => t('Project: Start/End Date'),
        'help' => t('The start or finish date for a project recorded in the projects table'),
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
      //--- recalc field
      'recalc' => array(
        'title' => t('Project: Recalculate'),
        'help' => t('Flag showing wheter a schedule recalculation is required for a project recorded in the projects table'),
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
      //--- <end of fields>
    ),

    //===== the projactivities table
    'org_projactivities' => array(
      'table' => array(
        'group' => t('Project'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_projactivities.vid = node.vid',
          ),
        ),
      ),
      //--- pnid field
      'pnid' => array(
        'title' => t('Proj Activity: Project Ref'),
        'help' => t('The project associated with an activity recorded in the projactivities table'),
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
          'base' => 'org_projects',
          'base field' => 'nid',
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Proj Activity: Member Ref'),
        'help' => t('Person responsible for a project activity recorded in the projactivities table'),
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
      //--- activity field
      'activity' => array(
        'title' => t('Proj Activity: Title'),
        'help' => t('The title for a project activity recorded in the projactivities table'),
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
      //--- activicategory field
      'activicategory' => array(
        'title' => t('Proj Activity: Category'),
        'help' => t('The category for a project activity recorded in the projactivities table'),
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
      //--- activistatus field
      'activistatus' => array(
        'title' => t('Proj Activity: Status'),
        'help' => t('The status for a project activity recorded in the projactivities table'),
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
      //--- duration field
      'duration' => array(
        'title' => t('Proj Activity: Duration'),
        'help' => t('The duration (in days, weeks, months, etc) for a project activity recorded in the projactivities table'),
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
      //--- duratype field
      'duratype' => array(
        'title' => t('Proj Activity: Duration Type'),
        'help' => t('The duration type: days, weeks, months, etc for a project activity recorded in the projactivities table'),
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
      //--- actividatype field
      'actividatype' => array(
        'title' => t('Proj Activity: Date Constraint'),
        'help' => t('The type of date constraint on a project activity recorded in the projactivities table'),
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
      //--- actividate field
      'actividate' => array(
        'title' => t('Proj Activity: Start/Finish Date'),
        'help' => t('The start or finish date associated with a project activity recorded in the projactivities table'),
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
      //--- actualstart field
      'actualstart' => array(
        'title' => t('Proj Activity: Actual Start'),
        'help' => t('The actual start date of a project activity recorded in the projactivities table'),
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
      //--- actualfinish field
      'actualfinish' => array(
        'title' => t('Proj Activity: Actual Finish'),
        'help' => t('The actual finish date of a project activity recorded in the projactivities table'),
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
      //--- schedstart field
      'schedstart' => array(
        'title' => t('Proj Activity: Scheduled Start'),
        'help' => t('The scheduled start date of a project activity recorded in the projactivities table'),
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
      //--- schedfinish field
      'schedfinish' => array(
        'title' => t('Proj Activity: Scheduled Finish'),
        'help' => t('The scheduled finish date of a project activity recorded in the projactivities table'),
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

    //===== the projreports table
    'org_projreports' => array(
      'table' => array(
        'group' => t('Project'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_projreports.vid = node.vid',
          ),
        ),
      ),
      //--- pnid field
      'pnid' => array(
        'title' => t('Proj Report: Project Ref'),
        'help' => t('The project referenced in a project report recorded in the projreports table'),
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
          'base' => 'org_projects',
          'base field' => 'nid',
        ),
      ),
      //--- anid field
      'anid' => array(
        'title' => t('Proj Report: Activity Ref'),
        'help' => t('The project activity referenced in a project report recorded in the projreports table'),
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
          'base' => 'org_projactivities',
          'base field' => 'nid',
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Proj Report: Member Ref'),
        'help' => t('The member responsible for a project report recorded in the projreports table'),
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
      //--- projreport field
      'projreport' => array(
        'title' => t('Proj Report: Title'),
        'help' => t('The title of a project report recorded in the projreports table'),
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
      //--- projrepdate field
      'projrepdate' => array(
        'title' => t('Proj Report: Date'),
        'help' => t('The date of a project report recorded in the projreports table'),
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

    //===== <end of tables>
  );
}
