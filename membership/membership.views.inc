<?php
// $Id$

/**
 * @file
 * Provides views api functions for the membership module.
 */

/**
 * Implementation of hook_views_data()
 */
function membership_views_data() {
  // Define the membership tables to views
  return array(
    //===== the members table
    'org_members' => array(
      'table' => array(
        'group' => t('Membership'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_members.vid = node.vid',
          ),
        ),
      ),
      //--- pnid field
      'pnid' => array(
        'title' => t('Member: Parent Ref'),
        'help' => t('The parent record in the members table for a membership'),
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
      //--- category field
      'category' => array(
        'title' => t('Member: Category'),
        'help' => t('The membership category held in the members table'),
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
      //--- standing field
      'standing' => array(
        'title' => t('Member: Status'),
        'help' => t('The membership status value held in the members table'),
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
      //--- member (ie name) field
      'member' => array(
        'title' => t('Member: Name'),
        'help' => t('The membership name held in the members table'),
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
      //--- address field
      'address' => array(
        'title' => t('Member: Address'),
        'help' => t('The membership address address held in the members table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => FALSE,
        ),
      ),
      //--- city field
      'city' => array(
        'title' => t('Membership City'),
        'help' => t('The membership city value held in the members table'),
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
      //--- postcode field
      'postcode' => array(
        'title' => t('Member: Postcode'),
        'help' => t('The membership postcode value held in the members table'),
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
      //--- email field
      'email' => array(
        'title' => t('Member: Email'),
        'help' => t('The membership email address(es) held in the members table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
      ),
      //--- phone number field
      'phone' => array(
        'title' => t('Member: Phone'),
        'help' => t('The membership phone number(s) held in the members table'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => FALSE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
          'allow empty' => TRUE
        ),
      ),
      //--- joindate field
      'joindate' => array(
        'title' => t('Member: Join Date'),
        'help' => t('The membership start date recorded in the members table'),
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
          'handler' => 'views_handler_sort_date',
        ),
      ),
      //--- leavedate field
      'leavedate' => array(
        'title' => t('Member: Leave Date'),
        'help' => t('The membership end date recorded in the members table'),
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
          'handler' => 'views_handler_sort_date',
        ),
      ),
      //--- <end of fields>
    ),

    //===== the subscriptions table
    'org_subscriptions' => array(
      'table' => array(
        'group' => t('Membership'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_subscriptions.vid = node.vid',
          ),
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Subscription: Member Ref'),
        'help' => t('The member receiving a subscription as recorded in the subscriptions table'),
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
      //--- scode field
      'scode' => array(
        'title' => t('Subscription: Code'),
        'help' => t('The code for a subscription recorded in the subscription table'),
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
      //--- datefrom field
      'datefrom' => array(
        'title' => t('Subscription: Start Date'),
        'help' => t('The start date for a subscription recorded in the subscription table'),
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
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
      ),
      //--- dateto field
      'dateto' => array(
        'title' => t('Subscription: End Date'),
        'help' => t('The end date for a subscription recorded in the subscription table'),
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
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
      ),
      //--- amount field
      'amount' => array(
        'title' => t('Subscription: Amount'),
        'help' => t('The amount of a subscription recorded in the subscription table'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
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

    //===== the subscription payments table
    'org_subpayments' => array(
      'table' => array(
        'group' => t('Membership'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
            'extra' => 'org_subpayments.vid = node.vid',
          ),
        ),
      ),
      //--- mnid field
      'mnid' => array(
        'title' => t('Sub-payment: Member Ref'),
        'help' => t('The member making payment as recorded in the subscription payments table'),
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
      //--- snid field
      'snid' => array(
        'title' => t('Sub-payment: Subscription Ref'),
        'help' => t('The reference to the subscription being paid as recorded in the subscription payments table'),
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
          'base' => 'org_subscriptions',
          'base field' => 'nid',
        ),
      ),
      //--- datepaid field
      'datepaid' => array(
        'title' => t('Sub-payment: Date'),
        'help' => t('The date a subscription was paid as recorded in the subscription payments table'),
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
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
      ),
      //--- payment field
      'payment' => array(
        'title' => t('Sub-payment: Amount'),
        'help' => t('The amount paid against a subscription as recorded in the subscription payments table'),
        'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE,
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

    //===== <end of tables>
  );
}

/**
 * Implementation of hook_views_handlers()
 */
/*function membership_views_handlers() {
  return array(
    'handlers' => array(
      'membership_handler_field_members' => array(
        'parent' => 'views_handler_field_node',
        'file' => 'membership_handler_fields.inc',
      ),
      'membership_handler_field_members_member' => array(
        'parent' => 'membership_handler_field_members',
        'file' => 'membership_handler_fields.inc',
      ),
    ),
  );
}*/
