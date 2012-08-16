// $Id$
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    // 
  });
}

function showCabinet(id) {
  $.get("/node/" + id, function(data) {
    $("#filexplorer").empty().html(data);
    $('#scrollbar1').tinyscrollbar_update();
  });
    
}

function showdrawer(id) {
  if ($("#content" + id).html().length == 0) {
    $.get("/node/" + id, function(data) {
      $("#content" + id).html(data);
      $('#scrollbar1').tinyscrollbar_update();
    });
  } else {
    $("#content" + id).empty();
    $('#scrollbar1').tinyscrollbar_update();
  }
}

function showfolder(id) {
  showdrawer(id);
}

function showdocument(id) {
  $.get("/node/" + id, function(data) {
    $("#filedetails").empty().html(data);
  });
}
