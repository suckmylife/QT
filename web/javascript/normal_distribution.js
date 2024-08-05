var x1 = [];
var x2 = [];
var x3 = [];
var x4 = [];
var x5 = [];
var x6 = [];
var x7 = [];
var x8 = [];

var y1 = [];
var y2 = [];
var y3 = [];
var y4 = [];
var y5 = [];
var y6 = [];
var y7 = [];
var y8 = [];

for (var i = 1; i < 781; i++) 
{
  k = Math.random();
  x1.push(k*17);
  x2.push(k*18);
  x3.push(k*19);
  x4.push(k*20);
  x5.push(k*5);
  x6.push(k*6);
  x7.push(k*7);
  x8.push(k*8);;

  y1.push(k);
  y2.push(k*2);
  y3.push(k*3);
  y4.push(k*4);
  y5.push(k*13);
  y6.push(k*14);
  y7.push(k*15);
  y8.push(k*16);
}
var trace1 = {
  
    x: x1,
    y:y1,
  name: 'control',
  autobinx: false, 
  histnorm: "count", 
  marker: {
    color: "rgba(255, 100, 102, 0.7)", 
     line: {
      color:  "rgba(255, 100, 102, 1)", 
      width: 1
    }
  },  
  opacity: 0.5, 
  type: "histogram", 
  xbins: {
    end: 2.8, 
    size: 0.06, 
    start: .5
  }
};
var trace2 = {
  x: x2,
  y:y2,
 
  autobinx: false, 
  marker: {
          color: "rgba(100, 200, 102, 0.7)",
           line: {
            color:  "rgba(100, 200, 102, 1)", 
            width: 1
    } 
       }, 
  name: "experimental", 
  opacity: 0.75, 
  type: "histogram", 
  xbins: { 
    end: 4, 
    size: 0.06, 
    start: -3.2

  }
};
var data = [trace1, trace2];
var layout = {
  bargap: 0.05, 
  bargroupgap: 0.2, 
  barmode: "overlay", 
  title: "Sampled Results", 
  xaxis: {title: "Value"}, 
  yaxis: {title: "Count"}
};
Plotly.newPlot('n_d', data, layout);