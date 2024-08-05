var data = null;
var graph = null;
var parsing = [];
var str_parsing = [];
var int_parsing = [];
var j = 0;
var frameMax = 20;

$(document).ready(function(){
    $.ajax({
          url: '/main/sensor1/',
          type:'GET',
          dataType:'json',
          success:function(data){
              if(data){
                parsing = data['data'].forEach(function(d){
                  str_parsing.push(d.data);
                  return str_parsing
                });
                //str_parsing = JSON.stringify(parsing);
                var str_container = JSON.stringify(str_parsing);
                int_parsing = str_container.split(',').map(function(item){
                  return parseInt(item,10)
                });
                drawVisualization1(int_parsing);
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
        url: '/main/sensor1/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data){
              parsing = data['data'].forEach(function(d){
                str_parsing.push(d.data);
                return str_parsing
              });
              //str_parsing = JSON.stringify(parsing);
              var str_container = JSON.stringify(str_parsing);
              int_parsing = str_container.split(',').map(function(item){
                return parseInt(item,10)
              });
              drawVisualization1(int_parsing);
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

function drawVisualization1(parsing) {
  var style = document.getElementById("style").value;
  data = new vis.DataSet();
  var tMax = frameMax; //프레임 단위 
  for (var t = 0; t < tMax; t++) {
    for (var x = 0; x < 48; x++) {
      for (var y = 0; y < 48; y++) {
        data.add([{ x: x, y: y, z: parsing[j], filter: t }]);
        j = j+1;
      
      }
    }
}
// specify options
var options = {
  width: "600px",
  height: "600px",
  style: style,
  showPerspective: true,
  showGrid: true,
  showShadow: false,
  // showAnimationControls: false,
  keepAspectRatio: true,
  verticalRatio: 0.5,
  animationInterval: 100, // milliseconds
  animationPreload: true,
  zMin:0,
  zMax:zaxis_max,
};

// create our graph
var container = document.getElementById("3Dgraph_ani");
graph = new vis.Graph3d(container, data, options);
}
