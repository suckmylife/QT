//no upper function
function unupper(str){
  str = String(str);
  return str.replace(/\"/gi,'');
}

function draw_graph3D_ani(data) {
  var graph = null;
  var j = 0;
  var cnt = 10;
  var zaxis_max = 50;
  
  //data
  var data = document.querySelectorAll('[id="select_ip_data"]');
  var data_arr = [...data].map(input=>input.value);
  var data_unupper = unupper(data_arr);
  var data_comma = new Array();
  data_comma = data_unupper.split(',').map(function(item){
    return parseInt(item,10)
});

  var style = document.getElementById("style").value;
  var withValue = ['bar-color', 'bar-size', 'dot-size', 'dot-color'].indexOf(style) != -1;
  select_data = new vis.DataSet();
  for (var t = 0; t < cnt; t++) {
    for (var x = 0; x < 48; x++) {
      for (var y = 0; y < 48; y++) {
        if (withValue) {
          var value = (y - x);
          select_data.add([{ x: x, y: y, z: data_comma[j], filter: t,style:value }]);
          j = j+1;
        }
        else {
          select_data.add([{ x: x, y: y, z: data_comma[j], filter: t }]);
          j = j+1;
        }
      }
    }
}
// specify options
var options = {
  width: "700px",
  height: "700px",
  style: style,
  showPerspective: true,
  showGrid: true,
  showShadow: false,
  keepAspectRatio: false,
  verticalRatio: 0.5,
  animationInterval: 200, // milliseconds
  animationPreload: true,
  zMin:0,
  zMax:zaxis_max,
  filterLabel: "프레임",
  xLabel: "센서 1장의 입체 압력 분포도",
  legendLabel: '압력',
  tooltip: function (point) {
    // parameter point contains properties x, y, z
    return "z값: <b>" + point.z + "</b>"+ " x값: <b>" +point.x + "</b>"+ " y값: <b>" +point.y + "</b>";
  },
  xValueLabel: function (value) {
    return value;
  },
  yValueLabel: function (value) {
    return value;
  }
};

// create our graph
var container = document.getElementById("graph3D_ani");
graph = new vis.Graph3d(container, select_data, options);
}