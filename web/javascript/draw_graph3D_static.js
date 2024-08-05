var data = null;
var graph = null;
let dummy_int = []
var zaxis_max = 100;

//no upper function
function unupper(str){
  str = String(str);
  return str.replace(/\"/gi,'');
}

$(document).ready(function(){
  $.ajax({
        url: '/main/sensor/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data){
              let str = JSON.stringify(data['data']['data']);
              str = unupper(str);
              dummy_int = str.split(',').map(function(item){
                return parseInt(item,10)
              });
              drawVisualization(dummy_int);
            }
            else{
              alert("DATA CAN'T READ");
            };
        },
        error:function(){
            alert('function is error, please modify ajax function!');
        }
    });
});

$('#style').change(function(){
  $.ajax({
        url: '/main/sensor/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data){
              let str = JSON.stringify(data['data']['data']);
              str = unupper(str);
              dummy_int = str.split(',').map(function(item){
                return parseInt(item,10)
              });
              drawVisualization(dummy_int);
            }
            else{
              alert("DATA CAN'T READ");
            };
        },
        error:function(){
            alert('function is error, please modify ajax function!');
        }
    });
});

function drawVisualization(dummy_int) {
  var style = document.getElementById("style").value;
  // Create data
  data = new vis.DataSet();
    var i = 0;
    for (var x=0; x<48; x++)
    { 
        for(var y=0; y<48; y++)
        {
        data.add({x: x, y: y, z: dummy_int[i]});
        i = i+1;
        }
    }

  var options = {
    width: "600px",
    height: "600px",
    style: style,
    showPerspective: true,
    showGrid: true,
    showShadow: false,

    //restrict z-axis
    zMin: 0,
    zMax: zaxis_max, 

    // Option tooltip can be true, false, or a function returning a string with HTML contents
    //tooltip: true,
    tooltip: function (point) {
      // parameter point contains properties x, y, z
      return "z값: <b>" + point.z + "</b>"+ " x값: <b>" +point.x + "</b>"+ " y값: <b>" +point.y + "</b>";
    },
    xValueLabel: function (value) {
      return value;
    },
    yValueLabel: function (value) {
      return value;
    },
    keepAspectRatio: true,
    verticalRatio: 0.5,
  };

  var camera = graph ? graph.getCameraPosition() : null;

  // create our graph
  var container = document.getElementById("3Dgraph_static");
  graph = new vis.Graph3d(container, data, options);

  if (camera) graph.setCameraPosition(camera); // restore camera position
}
