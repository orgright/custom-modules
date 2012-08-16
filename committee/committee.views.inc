<?php
// $Id$

/**
 * @file
 * Provides views api functions for the committee module.
 */

/**
 * Implementation of hook_views_data()
 */
function committee_views_data() {
  // Define the committee tables to views
  return array(
    //===== the committees table
    'org_committees' => array(
      'table' => array(
        'group' => t('Committee'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_committees.vid = node.vid',
          ),
        ),
      ),
      //--- pnid field
      'pnid' => array(
        'title' => t('Committee: Parent Ref'),
        'help' => t('The parent of the committee record in the committees table'),
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
      ),
      //--- committee field
      'committee' => array(
        'title' => t('Committee: Name'),
        'help' => t('The name of the committee recorded in the committees table'),
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

    //===== the roles table
    'org_roles' => array(
      'table' => array(
        'group' => t('Committee'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_roles.vid = node.vid',
          ),
        ),
      ),
      //--- cnid field
      'cnid' => array(
        'title' => t('Role: Committee Ref'),
        'help' => t('The committee associated with the role recorded in the roles table'),
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
      //--- role field
      'role' => array(
        'title' => t('Role: Name'),
        'help' => t('Name of committee role as recorded in the roles table'),
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
      //--- multiperson field
      'multiperson' => array(
        'title' => t('Role: Multi-person'),
        'help' => t('Indicator in the roles table whether more than one person allowed in a role simultaneously'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
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
      //--- allowvacant field
      'allowvacant' => array(
        'title' => t('Role: Allow Vacant'),
        'help' => t('Indicator in the roles table whether a role is allowed to be vacant'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
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

    //===== the appointments table
    'org_appointments' => array(
      'table' => array(
        'group' => t('Committee'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_appointments.vid = node.vid',
          ),
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Appointment: Member Ref'),
        'help' => t('Reference to a member appointed to a committee as recorded in the appointments table'),
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
          'base' => 'org_members',
          'base field' => 'nid',
        ),
      ),
      //--- cnid field
      'cnid' => array(
        'title' => t('Appointment: Committee Ref'),
        'help' => t('Reference to a committee associated with appointment recorded in the appointments table'),
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
          'base' => 'org_committees',
          'base field' => 'nid',
        ),
      ),
      //--- appointtype field
      'appointtype' => array(
        'title' => t('Appointment: Type'),
        'help' => t('The type code of the appointment recorded in the appointments table'),
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
      //--- appointdate field
      'appointdate' => array(
        'title' => t('Appointment: Start Date'),
        'help' => t('The commencement date of the appointment recorded in the appointments table'),
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
      //--- retiredate field
      'retiredate' => array(
        'title' => t('Appointment: Finish Date'),
        'help' => t('The termination date of the appointment recorded in the appointments table'),
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

    //===== the assignments table
    'org_assignments' => array(
      'table' => array(
        'group' => t('Committee'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_assignments.vid = node.vid',
          ),
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Assignment: Member Ref'),
        'help' => t('Reference to a member assigned to this committee role recorded in the assignments table'),
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
          'base' => 'org_members',
          'base field' => 'nid',
        ),
      ),
      //--- rnid field
      'rnid' => array(
        'title' => t('Assignment: Role Ref'),
        'help' => t('Reference to a committee role associated with this assignment in the assignments table'),
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
          'base' => 'org_roles',
          'base field' => 'nid',
        ),
      ),
      //--- assigntype field
      'assigntype' => array(
        'title' => t('Assignment: Type'),
        'help' => t('The assignment type code recorded in the assignments table'),
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
      //--- assigndate field
      'assigndate' => array(
        'title' => t('Assignment: Start Date'),
        'help' => t('The commencement date of the assignment recorded in the assignments table'),
        'field' => array(
          'handler' => 'views_handler_field_date',
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
      //--- ceasedate field
      'ceasedate' => array(
        'title' => t('Assignment: Finish Date'),
        'help' => t('The termination date of the assignment recorded in the assignments table'),
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
