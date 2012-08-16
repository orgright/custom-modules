<?php
// $Id$

/**
 * @file
 * Provides project schedule calculation functions for project module.
 */

function project_recalculate_schedule($pnid, $pdatype, $pdate) {
  // recalculate the schedule for the project passed as $pnid
  //orgright_debug_msg('project','Fn: project_recalculate_schedule, project='.$pnid);
  // Some functions from the php4 date calculation library are needed here, make sure
  // they are available.
  require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_calc.inc');

  // Get an array to translate the project date constraint type
  $projdatypes = project_get_projdatypes('keys');
  // Get an array to translate the activity date constraint type
  $actividatypes = project_get_actividatypes('keys');
  // Get an array showing the number or days required for each unit of druratype
  $daysarray = project_get_duratypes('days');
  // Specify the minimum and maximum date values permitted
  $mindate = '1900-01-01';
  $maxdate = '2199-12-31';
  // set project start and end dates
  $psdate = $mindate;
  $pfdate = $maxdate;
  if ($projdatypes[$pdatype] == 'S') {
    $psdate = $pdate;
    $psflag = TRUE;
  }
  else {
    $pfdate = $pdate;
    $psflag = FALSE;
  }
  // convert dates to day nums (params are: day, month, year)
  $psdaynum = date_calc_date_to_days(substr($psdate,8,2),substr($psdate,5,2),substr($psdate,0,4));
  $pfdaynum = date_calc_date_to_days(substr($pfdate,8,2),substr($pfdate,5,2),substr($pfdate,0,4));
  //orgright_debug_msg('proj','Project details: '.$psflag.', '.$psdaynum.', '.$pfdaynum);
  $daycount = 0;
  // build an array of all activities for this project
  $sql = "SELECT a.nid, a.vid, a.activity, a.activistatus, a.duration, a.duratype, a.actividatype, a.actividate, a.actualstart, a.actualfinish, a.schedstart, a.schedfinish from {node} n JOIN {org_projactivities} a ON a.nid = n.nid where a.vid = n.vid AND a.pnid = %d";
  $dbactivities = db_query($sql, $pnid);
  while ($activiobj = db_fetch_object($dbactivities)) {
    $activiobj->numdays = $daysarray[$activiobj->duratype] * $activiobj->duration;
    $daycount += $activiobj->numdays; // keep a count of the total days involved
    $activiobj->datype = $actividatypes[$activiobj->actividatype];
    if ($activiobj->datype != 'NA') {
      $adate = $activiobj->actividate;
      $activiobj->adaynum = date_calc_date_to_days(substr($adate,8,2),substr($adate,5,2),substr($adate,0,4));
    }
    // check to see if actual start and/or finish dates are recorded
    if ($activiobj->actualstart) {
      $sdate = $activiobj->actualstart;
      $activiobj->sdaynum = date_calc_date_to_days(substr($sdate,8,2),substr($sdate,5,2),substr($sdate,0,4));
      $activiobj->sdone = TRUE;
    }
    else {
      $activiobj->sdaynum = 0;
      $activiobj->sdone = FALSE;
    }
    if ($activiobj->actualfinish) {
      $fdate = $activiobj->actualfinish;
      $activiobj->fdaynum = date_calc_date_to_days(substr($fdate,8,2),substr($fdate,5,2),substr($fdate,0,4));
      $activiobj->fdone = TRUE;
    }
    else {
      $activiobj->fdaynum = 0;
      $activiobj->fdone = FALSE;
    }
    // Make numdays the offset to go from start to finish or vice-versa
    $activiobj->numdays = $activiobj->numdays - 1;
    // add other attributes required for procesing
    $activiobj->minsdaynum = $psdaynum;
    $activiobj->maxsdaynum = $pfdaynum - $activiobj->numdays;
    $activiobj->minfdaynum = $psdaynum + $activiobj->numdays;
    $activiobj->maxfdaynum = $pfdaynum;
    $activiobj->error = array();
    // and convert this object to an array
    $activities[$activiobj->nid] = (array)$activiobj;
  }
  // set project start or end date based on assumption that all activities are sequential
  if ($psflag) {
    // adjust the project end date
    $pfdaynum = $psdaynum + $daycount;
  }
  else {
    // adjust the project start date
    $psdaynum = $pfdaynum - $daycount;
  }
  //orgright_debug_msg('proj','Project details: '.$psflag.', '.$psdaynum.', '.$pfdaynum);
  // Iterate through array of activities adjusting max and min start and finish dates
  // based on revised project start and finish
  foreach ($activities as &$activity) {
    if ($psflag) {
      $activity['maxsdaynum'] = $pfdaynum - $activity['numdays'];
      $activity['maxfdaynum'] = $pfdaynum;
    }
    else {
      $activity['minsdaynum'] = $psdaynum;
      $activity['minfdaynum'] = $psdaynum + $activity['numdays'];
    }
  }
//==orgright_debug_dump('array of activities, now built',$activities);

  // find all the linkages between activities for this project and build an array
  // containing the links (store both directions in the array)
  $activilinks = array();
  $sql = "SELECT l.anid1, l.anid2, l.link FROM {org_projactivitylinks} l WHERE l.pnid = %d";
  $dblinks = db_query($sql, $pnid);
  while ($linkobj = db_fetch_object($dblinks)) {
    $activilinks[$linkobj->anid1][$linkobj->anid2] = $linkobj->link;
    $linkarray = project_invert_link($linkobj->anid1, $linkobj->anid2, $linkobj->link);
    $activilinks[$linkarray['1st']][$linkarray['2nd']] = $linkarray['link'];
  }
//==orgright_debug_dump('array of links, now built',$activilinks);
  //======  Main Processing Starts Here ======
  // Initial iteration of activities, looking for easy to schedule ones
  // Set a flag showing whether there is more work to do
  $moretodo = FALSE;
  // Now go through the activities looking for specified start or finish dates
  // or for activities that are not constrained and not linked to any other
  foreach ($activities as $activid=>&$activity) {
    // skip if this activity is already sceduled (actual dates present)
    if ($activity['sdone'] && $activity['fdone']) {
      continue;
    }
    // see if this activity can be sceduled on initial scan
    $linkflag = isset($activilinks[$activid]);
    if ($activity['datype'] == 'SO') { // constraint = start on "date"
      // check to see that date constraint is not in conflict with project date
      if ($psflag && ($activity['adaynum'] < $psdaynum)) {
        $activity['error'][] = 'ERR01';
      }
      // set the activity start and finish dates
      if (!$activity['sdone']) {
        $activity['sdaynum'] = $activity['adaynum'];
      }
      if (!$activity['fdone']) {
        $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
        // Check that activity will finish by project end date
        if (!$psflag && ($activity['fdaynum'] > $pfdaynum)) {
          $activity['error'][] = 'ERR04';
        }
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    elseif ($activity['datype'] == 'FO') { // constraint = finish on "date"
      // check to see that date constraint is not in conflict with project date
      if (!$psflag && ($activity['adaynum'] > $pfdaynum)) {
        $activity['error'][] = 'ERR02';
      }
      // set the activity start and finish dates
      if (!$activity['sdone']) {
        $activity['sdaynum'] = $activity['adaynum'] - $activity['numdays'];
      }
      if (!$activity['fdone']) {
        $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
        // check that the activity can finish by the specified date
        if ($activity['fdaynum'] > $activity['adaynum']) {
          $activity['error'][] = 'ERR03';
        }
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    elseif ($activity['datype'] == 'SB' && !$linkflag) { // start before constraint, but no links
      // check to see that date constraint is not in conflict with project date
      if ($activity['adaynum'] < $psdaynum) {
        $activity['error'][] = 'ERR01';
      }
      // set the activity start date
      if (!$activity['sdone']) {
        if ($psflag) {
          $activity['sdaynum'] = $psdaynum;
        }
        else {
          $activity['sdaynum'] = $activity['adaynum'] - 1;
        }
      }
      // adjust start date if earlier than project start
      if ($activity['sdaynum'] < $psdaynum) {
        $activity['sdaynum'] = $psdaynum;
      }
      // set the activity finish date
      if (!$activity['fdone']) {
        $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    elseif ($activity['datype'] == 'SA' && !$linkflag) { // start after constraint, but no links
      // check to see that date constraint is not in conflict with project date
      if ($activity['adaynum'] > $pfdaynum) {
        $activity['error'][] = 'ERR02';
      }
      // set the activity start date
      if (!$activity['sdone']) {
        if ($psflag) {
          $activity['sdaynum'] = $activity['adaynum'] + 1;
        }
        else {
          $activity['sdaynum'] = $pfdaynum - $activity['numdays'];
        }
      }
      // adjust start date if earlier than project start
      if ($activity['sdaynum'] < $psdaynum) {
        $activity['sdaynum'] = $psdaynum;
      }
      // set the activity finish date
      if (!$activity['fdone']) {
        $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    elseif ($activity['datype'] == 'FA' && !$linkflag) { // finish after constraint, but no links
      // check to see that date constraint is not in conflict with project date
      if ($activity['adaynum'] + 1 > $pfdaynum) {
        $activity['error'][] = 'ERR02';
      }
      // set the activity start date
      if (!$activity['sdone']) {
        if ($psflag) {
          $activity['sdaynum'] = $activity['adaynum'] + 1 - $activity['numdays'];
        }
        else {
          $activity['sdaynum'] = $pfdaynum - $activity['numdays'];
        }
      }
      // set the activity finish date
      if (!$activity['fdone']) {
        $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    elseif ($activity['datype'] == 'FB' && !$linkflag) { // finish before constraint, but no links
      // check to see that date constraint is not in conflict with project date
      if ($activity['adaynum'] - $activity['numdays'] < $psdaynum) {
        $activity['error'][] = 'ERR01';
      }
      // set the activity start date
      if (!$activity['sdone']) {
        if ($psflag) {
          $activity['sdaynum'] = $psdaynum;
        }
        else {
          $activity['sdaynum'] = $activity['adaynum'] - 1 - $activity['numdays'];
        }
      }
      // set the activity finish date
      if (!$activity['fdone']) {
        $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    elseif ($activity['datype'] == 'NA' && !$linkflag) { // no date constraint or links
      if ($psflag) { // project has a start date
        if (!$activity['sdone']) {
          $activity['sdaynum'] = $psdaynum;
        }
        if (!$activity['fdone']) {
          $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
        }
      }
      else { // project has a finish date
        if (!$activity['sdone']) {
          $activity['sdaynum'] = $pfdaynum - $activity['numdays'];
        }
        if (!$activity['fdone']) {
          $activity['fdaynum'] = $activity['sdaynum'] + $activity['numdays'];
        }
      }
      $activity['sdone'] = $activity['fdone'] = TRUE;
    }
    else {  // default case for other activities
      // have not been able to schedule this activity on initial scan
      $moretodo = TRUE;
    }
  }
  //orgright_debug_msg('proj','initial scan of activities done');
//==orgright_debug_dump('initial scan of activities',$activities);
  // Have completed initial scan of activities and scheduled where possible
  // Next step is to scan through the activities adjusting the start and finish date ranges
  // to satisfy the link constraints while staying within project start and finish dates
  $loopcount = 0;
  $changeflag = TRUE;
  while ($changeflag) {
    $changeflag = FALSE;
    ++$loopcount;
    foreach ($activities as $activid=>&$activity) {
      $linkflag = isset($activilinks[$activid]);
      if ($activity['sdone'] && $activity['fdone']) {
        // activity scheduled, so restrict start/finish date range to actual
        $activity['minsdaynum'] = $activity['sdaynum'];
        $activity['maxsdaynum'] = $activity['sdaynum'];
        $activity['minfdaynum'] = $activity['fdaynum'];
        $activity['maxfdaynum'] = $activity['fdaynum'];
        $changeflag = TRUE;
        continue;
      }
      // search links to find any activites that finish before this starts
      // or start after this finishes so can restrict start/finish date range
      $currentlinks = $activilinks[$activid];
      foreach ($currentlinks as $anid=>$link) {
        $linkedactivity = $activities[$anid];
        switch ($link) {
        case 'SAF':
          // linked activity starts after current finishes
          if ($activity['maxfdaynum'] >= $linkedactivity['maxsdaynum']) {
            $activity['maxfdaynum'] = $linkedactivity['maxsdaynum'] - 1;
            if ($activity['minfdaynum'] > $activity['maxfdaynum']) {
              $activity['minfdaynum'] = $activity['maxfdaynum'];
            }
            $changeflag = TRUE;
          }
          break;
        case 'FBS':
          // linked activity finishes before current starts
          if ($activity['minsdaynum'] <= $linkedactivity['minfdaynum']) {
            $activity['minsdaynum'] = $linkedactivity['minfdaynum'] + 1;
            if ($activity['maxsdaynum'] < $activity['minsdaynum']) {
              $activity['maxsdaynum'] = $activity['minsdaynum'];
            }
            $changeflag = TRUE;
          }
          break;
        case 'SAS':
          // linked activity starts after current starts
          if ($activity['minsdaynum'] >= $linkedactivity['minsdaynum']) {
            $activity['minsdaynum'] = $linkedactivity['minsdaynum'] - 1;
            $changeflag = TRUE;
          }
          break;
        case 'SBS':
          // linked activity starts before current starts
          if ($activity['minsdaynum'] <= $linkedactivity['minsdaynum']) {
            $activity['minsdaynum'] = $linkedactivity['minsdaynum'] + 1;
            if ($activity['maxsdaynum'] < $activity['minsdaynum']) {
              $activity['maxsdaynum'] = $activity['minsdaynum'];
            }
            $changeflag = TRUE;
          }
          break;
        case 'FAF':
          // linked activity finishes after current finishes
          if ($activity['maxfdaynum'] >= $linkedactivity['maxfdaynum']) {
            $activity['maxfdaynum'] = $linkedactivity['maxfdaynum'] - 1;
            if ($activity['minfdaynum'] > $activity['maxfdaynum']) {
              $activity['minfdaynum'] = $activity['maxfdaynum'];
            }
            $changeflag = TRUE;
          }
          break;
        case 'FBF':
          // linked activity finishes before current finishes
          if ($activity['maxfdaynum'] <= $linkedactivity['maxfdaynum']) {
            $activity['maxfdaynum'] = $linkedactivity['maxfdaynum'] + 1;
            $changeflag = TRUE;
          }
          break;
        case 'FAS':
          // linked activity finishes after current starts
          if ($activity['minsdaynum'] >= $linkedactivity['minfdaynum']) {
            $activity['minsdaynum'] = $linkedactivity['minfdaynum'] - 1;
            $changeflag = TRUE;
          }
          if ($activity['maxsdaynum'] >= $linkedactivity['maxfdaynum']) {
            $activity['maxsdaynum'] = $linkedactivity['maxfdaynum'] - 1;
            if ($activity['minsdaynum'] > $activity['maxsdaynum']) {
              $activity['minsdaynum'] = $activity['maxsdaynum'];
            }
            $changeflag = TRUE;
          }
          break;
        case 'SBF':
          // linked activity starts before current finishes
          if ($activity['minfdaynum'] <= $linkedactivity['minsdaynum']) {
            $activity['minfdaynum'] = $linkedactivity['minsdaynum'] + 1;
            if ($activity['maxfdaynum'] < $activity['minfdaynum']) {
              $activity['maxfdaynum'] = $activity['minfdaynum'];
            }
            $changeflag = TRUE;
          }
          if ($activity['maxfdaynum'] <= $linkedactivity['maxsdaynum']) {
            $activity['maxfdaynum'] = $linkedactivity['maxsdaynum'] + 1;
            $changeflag = TRUE;
          }
          break;
        default:
        } // end of switch ($link)
      } // end of foreach ($currentlinks)
    } // end of foreach ($activities)
    if ($loopcount > 99) {
      //orgright_debug_msg('project','Loop count exceeded for date range adjustment process');
      break;
    }
  } // end of while ($changeflag)
 //orgright_debug_msg('proj','adjust activity date ranges done');
//==orgright_debug_dump('adjust activity date ranges',$activities);
  
  // Now iterate though looking for linked activities that can be scheduled
  // Strategy is to iterate through the list looking for activities that are
  // linked to activities that have already been scheuled, or for activities
  // where the links do not constrain scheduling the current one.
  $loopcount = 0;
  while ($moretodo) {
    $moretodo = FALSE;
    // set an escape flag and a count to avoid infinite loop
    $changeflag = FALSE;
    ++$loopcount;
 //orgright_debug_msg('proj','Main processing loop, count='.$loopcount);
    // go through the activities, one at a time
    foreach ($activities as $activid=>&$activity) {
 //orgright_debug_msg('proj','This activity = '.$activid);
      $linkflag = isset($activilinks[$activid]);
      if ($activity['sdone'] && $activity['fdone']) {
        // nothing to do for this activity, so check next
        continue;
      }
      // there are still activities to schedule
      $moretodo = TRUE;
 //orgright_debug_msg('proj','=== processing activity '.$activid);
      // get a list of the activities linked to the current one
      $currentlinks = $activilinks[$activid];
      // and iterate through these linked activities to see if they are all done
      // or whether the link does not restrict the scheduling of the current
      // activity (eg project has start date and link is start or finish after)
      $linkedetails = array();
      $todoflag = TRUE;
      foreach ($currentlinks as $anid=>$link) {
        $linkedetails[$anid]['link'] = $link;
        $linkedactivity = $activities[$anid];
        // check whether this linked activity is already scheduled
        if ($linkedactivity['sdone'] && $linkedactivity['fdone']) {
 //orgright_debug_msg('proj','X: save real start & finish dates for activity '.$anid);
          // yes - save the start & finish dates for this linked activity
          $linkedetails[$anid]['sdaynum'] = $linkedactivity['sdaynum'];
          $linkedetails[$anid]['fdaynum'] = $linkedactivity['fdaynum'];
          // continue with next linked activity
          continue;
        }
        // Get the length of the linked activity
        $linkednumdays = $activities[$anid]['numdays'];
        // Now see whether a pseudo start and finish date can be assigned to this linked activity
        // Use different approach depending on whether project has start of finish date
        if ($psflag) {
          // project scheduled from a start date
          switch ($link) {
            // can process if this link does not constrain the current activity start
          case 'SAF':
            // set pseudo start to be after the latest finish date for current activity
            $linkedetails[$anid]['sdaynum'] = $activity['maxfdaynum'] + 1;
            $linkedetails[$anid]['fdaynum'] = $linkedetails[$anid]['sdaynum'] + $linkednumdays;
 //orgright_debug_msg('proj','S1: save pseudo start & finish dates for activity '.$anid);
            break;
          case 'SAS':
            // set pseudo start to be after the latest start date for current activity
            $linkedetails[$anid]['sdaynum'] = $activity['maxsdaynum'] + 1;
            $linkedetails[$anid]['fdaynum'] = $linkedetails[$anid]['sdaynum'] + $linkednumdays;
 //orgright_debug_msg('proj','S2: save pseudo start & finish dates for activity '.$anid);
            break;
          case 'FAS':
            // set pseudo finish to be after the latest start date for current activity
            $linkedetails[$anid]['fdaynum'] = $activity['maxsdaynum'] + 1;
            $linkedetails[$anid]['sdaynum'] = $linkedetails[$anid]['fdaynum'] - $linkednumdays;
 //orgright_debug_msg('proj','S3: save pseudo start & finish dates for activity '.$anid);
            break;
          case 'FAF':
            // set pseudo finish to be after the latest finish date for current activity
            $linkedetails[$anid]['fdaynum'] = $activity['maxfdaynum'] + 1;
            $linkedetails[$anid]['sdaynum'] = $linkedetails[$anid]['fdaynum'] - $linkednumdays;
 //orgright_debug_msg('proj','S4: save pseudo start & finish dates for activity '.$anid);
            break;
          default:
            // cannot process this activity
            $todoflag = FALSE;
 //orgright_debug_msg('proj','cannot deal with link '.$link. ' to '.$anid);
          }
        }
        else {
          // project scheduled to a finish date
          switch ($link) {
            // can process if this link does not constrain the current activity finish
          case 'FBS':
            // set pseudo finish to be before the earliest start date for current activity
            $linkedetails[$anid]['fdaynum'] = $activity['minsdaynum'] - 1;
            $linkedetails[$anid]['sdaynum'] = $linkedetails[$anid]['fdaynum'] - $linkednumdays;
 //orgright_debug_msg('proj','F1: save pseudo start & finish dates for activity '.$anid);
            break;
          case 'SBS':
            // set pseudo start to be before the earliest start date for current activity
            $linkedetails[$anid]['sdaynum'] = $activity['minsdaynum'] - 1;
            $linkedetails[$anid]['fdaynum'] = $linkedetails[$anid]['sdaynum'] + $linkednumdays;
 //orgright_debug_msg('proj','F2: save pseudo start & finish dates for activity '.$anid);
            break;
          case 'SBF':
            // set pseudo start to be before the earliest finish date for current activity
            $linkedetails[$anid]['sdaynum'] = $activity['minfdaynum'] - 1;
            $linkedetails[$anid]['fdaynum'] = $linkedetails[$anid]['sdaynum'] + $linkednumdays;
 //orgright_debug_msg('proj','F3: save pseudo start & finish dates for activity '.$anid);
            break;
          case 'FBF':
            // set pseudo finish to be before the earliest finish date for current activity
            $linkedetails[$anid]['fdaynum'] = $activity['minfdaynum'] - 1;
            $linkedetails[$anid]['sdaynum'] = $linkedetails[$anid]['fdaynum'] - $linkednumdays;
 //orgright_debug_msg('proj','F4: save pseudo start & finish dates for activity '.$anid);
            break;
          default:
            // cannot process this activity
            $todoflag = FALSE;
 //orgright_debug_msg('proj','cannot deal with link '.$link. ' to '.$anid);
          }
        }
$msg = $anid.': '.$linkedetails[$anid]['link'].' '.$linkedetails[$anid]['sdaynum'].' '.$linkedetails[$anid]['fdaynum'];
 //orgright_debug_msg('proj','... processed link '.$msg);
      }
//==orgright_debug_dump('$linkedetails for activity '.$activid,$linkedetails);
      // check whether all linked activities are done or do not constrain curr activity
      if (!$todoflag) {
        // no - so go on to check next activity
 //orgright_debug_msg('proj','cannot process current activity '.$activid);
        continue;
      }
      // have found an activity that needs scheduling, and for which all
      // linked activities are already scheduled or do not constrain this one
 //orgright_debug_msg('proj','can process this one');
      $datype = $activity['datype'];
      $adaynum = $activity['adaynum'];
      $numdays = $activity['numdays'];
      $minsdaynum = $activity['minsdaynum'];
      $maxsdaynum = $activity['maxsdaynum'];
      $minfdaynum = $activity['minfdaynum'];
      $maxfdaynum = $activity['maxfdaynum'];
      // restrict the possible start and finish date ranges for current activity
      // from details held in the link info array (may be pseudo start/finish days)
      foreach ($linkedetails as $anid=>$details) {
        switch ($details['link']) {
        case 'SAF':
          // linked activity starts after current finishes
          if ($maxfdaynum >= $details['sdaynum']) {
            $maxfdaynum = $details['sdaynum'] - 1;
          }
          break;
        case 'FBS':
          // linked activity finishes before current starts
          if ($minsdaynum <= $details['fdaynum']) {
            $minsdaynum = $details['fdaynum'] + 1;
          }
          break;
        case 'SAS':
          // linked activity starts after current starts
          if ($maxsdaynum >= $details['sdaynum']) {
            $maxsdaynum = $details['sdaynum'] - 1;
          }
          break;
        case 'SBS':
          // linked activity starts before current starts
          if ($minsdaynum <= $details['sdaynum']) {
            $minsdaynum = $details['sdaynum'] + 1;
          }
          break;
        case 'FAF':
          // linked activity finishes after current finishes
          if ($maxfdaynum >= $details['fdaynum']) {
            $maxfdaynum = $details['fdaynum'] - 1;
          }
          break;
        case 'FBF':
          // linked activity finishes before current finishes
          if ($minfdaynum <= $details['fdaynum']) {
            $minfdaynum = $details['fdaynum'] + 1;
          }
          break;
        case 'FAS':
          // linked activity finishes after current starts
          if ($maxsdaynum >= $details['fdaynum']) {
            $maxsdaynum = $details['fdaynum'] - 1;
          }
          break;
        case 'SBF':
          // linked activity starts before current finishes
          if ($minfdaynum <= $details['sdaynum']) {
            $minfdaynum = $details['sdaynum'] + 1;
          }
          break;
        }
      }
      // now adjust the start and finish date ranges to recognise constraint
      switch ($datype) {
      case 'SB':
        // activity starts before specified date
        if ($maxsdaynum >= $adaynum) {
          $maxsdaynum = $adaynum - 1;
        }
        break;
      case 'SO':
        // activity starts on specified date
        if ($maxsdaynum > $adaynum) {
          $maxsdaynum = $adaynum;
        }
        if ($minsdaynum < $adaynum) {
          $minsdaynum = $adaynum;
        }
        break;
      case 'SA':
        // activity starts after specified date
        if ($minsdaynum <= $adaynum) {
          $minsdaynum = $adaynum + 1;
        }
        break;
      case 'FB':
        // activity finishes before specified date
        if ($maxfdaynum >= $adaynum) {
          $maxfdaynum = $adaynum - 1;
        }
        break;
      case 'FO':
        // activity finishes on specified date
        if ($maxfdaynum > $adaynum) {
          $maxfdaynum = $adaynum;
        }
        if ($minfdaynum < $adaynum) {
          $minfdaynum = $adaynum;
        }
        break;
      case 'FA':
        // activity finishes after specified date
        if ($minfdaynum <= $adaynum) {
          $minfdaynum = $adaynum + 1;
        }
      }
      // Check that ranges are still valid, and reset if necessary
      if ($maxsdaynum < $minsdaynum) {
        //orgright_debug_msg('project','Schedule error - invalid start date range');
        if ($psflag) {
          $maxsdaynum = $minsdaynum;
        }
        else {
          $minsdaynum = $maxsdaynum;
        }
      }
      if ($maxfdaynum < $minfdaynum) {
        //orgright_debug_msg('project','Schedule error - invalid finish date range');
        if ($psflag) {
          $maxfdaynum = $minfdaynum;
        }
        else {
          $minfdaynum = $maxfdaynum;
        }
      }
      // Test the final date ranges
      if ($maxfdaynum - $minsdaynum < $numdays) {
         $activity['error'][] = 'ERR05';
      }
      if ($minfdaynum - $maxsdaynum > $numdays) {
         $activity['error'][] = 'ERR06';
      }
      // Can now schedule the activity
      if ($psflag) {
        // Schedule for earliest possible start date
        if ($minsdaynum + $numdays < $minfdaynum) {
          $minsdaynum = $minfdaynum - $numdays;
        }
        $activity['sdaynum'] = $minsdaynum;
        $activity['fdaynum'] = $activity['sdaynum'] + $numdays;
        $activity['sdone'] = $activity['fdone'] = TRUE;
        if ($activity['fdaynum'] > $maxfdaynum) {
          $activity['error'][] = 'ERR03';
        }
      }
      else {
        // schedule for latest possible start date
        if ($maxsdaynum + $numdays > $maxfdaynum) {
          $maxsdaynum = $maxfdaynum - $numdays;
        }
        $activity['sdaynum'] = $maxsdaynum;
        $activity['fdaynum'] = $activity['sdaynum'] + $numdays;
        $activity['sdone'] = $activity['fdone'] = TRUE;
        if ($activity['sdaynum'] < $minsdaynum) {
          $activity['error'][] = 'ERR05';
        }
      }
      // Save final start and finish date ranges
      $activity['minsdaynum'] = $minsdaynum;
      $activity['maxsdaynum'] = $maxsdaynum;
      $activity['minfdaynum'] = $minfdaynum;
      $activity['maxfdaynum'] = $maxfdaynum;
      // Have scheduled that activity, so flag changes
      $changeflag = TRUE;
 //orgright_debug_msg('proj','--- scheduled: '.$activity['sdaynum'].', '.$activity['fdaynum']);
    } // end of foreach $activities loop
    // if there is still more to do, but nothing changed on that iteration through the activities,
    // then try a different strategy
    if ($moretodo && !$changeflag) {
 //orgright_debug_msg('proj','... no changes on main loop, try alternative');
      // Find an activity that is scheduled and then look for activities that start after
      // or finish before depending on $psflag.  These can possibly then be scheduled.
      foreach ($activities as $activid=>&$activity) {
        if ($activity['sdone'] && $activity['fdone']) {
 //orgright_debug_msg('proj','found a done activity: '.$activid);
          // get a list of the activities linked to the current one
          $currentlinks = $activilinks[$activid];
          if ($psflag) {
            // look for linked activities that start after this one but are not yet scheduled
            $todoflag = TRUE;
            foreach ($currentlinks as $anid=>$link) {
              if ($activities[$anid]['sdone'] && $activities[$anid]['fdone']) {
                // This linked activity is already scheduled
                continue;
              }
 //orgright_debug_msg('proj','found a pre-link candidate: '.$anid.', link='.$link);
              switch ($link) {
              case 'SAS':
                $activities[$anid]['sdaynum'] = $activity['sdaynum'] + 1;
                $activities[$anid]['fdaynum'] = $activities[$anid]['sdaynum'] + $activities[$anid]['numdays'];
                $activities[$anid]['sdone'] = $activities[$anid]['fdone'] = TRUE;
                $changeflag = TRUE;
 //orgright_debug_msg('proj','... scheduled activity '.$anid.' after '.$activid);
                break;
              case 'SAF':
                $activities[$anid]['sdaynum'] = $activity['fdaynum'] + 1;
                $activities[$anid]['fdaynum'] = $activities[$anid]['sdaynum'] + $activities[$anid]['numdays'];
                $activities[$anid]['sdone'] = $activities[$anid]['fdone'] = TRUE;
                $changeflag = TRUE;
 //orgright_debug_msg('proj','... scheduled activity '.$anid.' after '.$activid);
                break;
              default:
                $todoflag = FALSE;
 //orgright_debug_msg('proj','did not schedule this candidate');
              } // end of switch ($link)
            } // end of foreach ($currentlinks)
          } // end of if ($psflag)
          else {
            // look for linked activities that finish before this one but are not yet scheduled
            $todoflag = TRUE;
            foreach ($currentlinks as $anid=>$link) {
              if ($activities[$anid]['sdone'] && $activities[$anid]['fdone']) {
                // This linked activity is already scheduled
                continue;
              }
 //orgright_debug_msg('proj','found a post-link candidate: '.$anid.', link='.$link);
              switch ($link) {
              case 'FBS':
                $activities[$anid]['fdaynum'] = $activity['sdaynum'] - 1;
                $activities[$anid]['sdaynum'] = $activities[$anid]['fdaynum'] - $activities[$anid]['numdays'];
                $activities[$anid]['sdone'] = $activities[$anid]['fdone'] = TRUE;
                $changeflag = TRUE;
 //orgright_debug_msg('proj','... scheduled activity '.$anid.' before '.$activid);
                break;
              case 'FBF':
                $activities[$anid]['fdaynum'] = $activity['fdaynum'] - 1;
                $activities[$anid]['sdaynum'] = $activities[$anid]['fdaynum'] - $activities[$anid]['numdays'];
                $activities[$anid]['sdone'] = $activities[$anid]['fdone'] = TRUE;
                $changeflag = TRUE;
 //orgright_debug_msg('proj','... scheduled activity '.$anid.' before '.$activid);
                break;
              default:
                $todoflag = FALSE;
 //orgright_debug_msg('proj','did not schedule this candidate');
              } // end of switch ($link)
            } // end of foreach ($currentlinks)
          } // end of else ($psflag)
        } // end of if ($activity['sdone'] && $activity['fdone'])
      } // end of foreach ($activities)
    }  // end of if (!changeflag && $moretodo)
    // have iterated through all activities and scheduled where we can
    // exit from outer "while" loop if there were no changes on the last pass
    if ($moretodo && !$changeflag) {
      //orgright_debug_msg('project','ERR - cannot find any activities to update!',2);
      break;
    }
    // exit from the outer "while" loop if it has been going too long
    if ($moretodo && $loopcount > 99) {
      //orgright_debug_msg('project','ERR - maximum loop count exceeded!',2);
      break;
    }
  } // end of outer "while ($moretodo)" loop
  // schedule all done, convert the daynum values back to dates and write to database
  $activisort = array();
  foreach ($activities as $activid=>&$activity) {
    $activity['schedstart'] = date_calc_days_to_date($activity['sdaynum'],'%Y-%m-%d');
    $activity['schedfinish'] = date_calc_days_to_date($activity['fdaynum'],'%Y-%m-%d');
    db_query("UPDATE {org_projactivities} SET schedstart = '%s', schedfinish = '%s' WHERE vid = %d",
      $activity['schedstart'],
      $activity['schedfinish'],
      $activity['vid']);
    // generate an array of values to be sorted later
    $activisort[$activid] = $activity['schedstart'] . '-' . $activity['schedfinish'];
  } // end of foreach ($activities)
//==orgright_debug_dump('end of schedule - all activities',$activities);
  // sort activities by start date, end date and generate graphic
  asort($activisort);
  $activicount = 0;
  $projpicture = '<div>';
  // now loop through the activities and generate a graphic
  foreach ($activisort as $activid=>$activisortkey) {
    ++$activicount;
    $activiname = substr($activities[$activid]['activity'].'                                ',0,32);
    $projpicture .= '<p>' . $activicount . ': ' . $activiname;
    $projpicture .= ' Start: ' . $activities[$activid]['schedstart'];
    $projpicture .= ' Finish: ' . $activities[$activid]['schedfinish'];
    $projpicture .= '</p>';
  } // end of foreach ($activities)
  $projpicture .= '</div>';
  // return the graphic picture
  return $projpicture;
}
