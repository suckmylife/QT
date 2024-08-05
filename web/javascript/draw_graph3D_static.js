function draw_graph3D_static() {
  var select_data = null;
  var graph = null;
  var zaxis_max = 50;
  var style = document.getElementById("style").value;
  var withValue = ['bar-color', 'bar-size', 'dot-size', 'dot-color'].indexOf(style) != -1;

  // Create data
  select_data = new vis.DataSet();
  var data = document.querySelectorAll('[id="select_ip_data"]');
  var data_arr = [...data].map(input=>input.value);
  var del = new Array();
  var i = 0;
  var cnt = $('#select_cnt').val();
  var selected_ip = $('#select_ip').val();

  for(var i =0; i<cnt; i++)
  {
      del[i] = data_arr[i].split(',').map(function(item){
          return parseInt(item,10)
          });
  }
 
  for (var x=0; x<48; x++)
  { 
      for(var y=0; y<48; y++)
      {
        if (withValue) {
          var value = (y - x);
          select_data.add({x: x, y: y, z: del[0][i],style:value });
          i = i+1;
        }
        else {
          select_data.add({x: x, y: y, z: del[0][i]});
          i = i+1;
        }
      }
  }

  var options = {
    width: "700px",
    height: "700px",
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
    xLabel: "센서 1장의 입체 압력 분포도",
    legendLabel: '압력',
    showYAxis:false,
    showXAxis:false
  };

  var camera = graph ? graph.getCameraPosition() : null;

  // create our graph
  var container = document.getElementById("graph3D");
  graph = new vis.Graph3d(container, select_data, options);

  if (camera) graph.setCameraPosition(camera); // restore camera position
}