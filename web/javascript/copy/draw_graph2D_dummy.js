var short_data = []
var re_data = [];
var i=0

  var bar_data = [
    {
      x: [100,20,0,0,0,0.6,0],
      type: 'histogram'
    }];
  
  var line_data = [{
    x: [1, 2, 3, 4],
    y: [10, 15, 13, 17],
    type: 'lines'
  }];
  
  var layout ={
    autosize: false,
    width: 500,
    height: 500,
    margin: {
      l: 50,
      r: 50,
      b: 100,
      t: 100,
      pad: 2
    }
  };
  var layout_line ={
    autosize: false,
    width: 1100,
    height: 500,
    margin: {
      l: 50,
      r: 50,
      b: 100,
      t: 100,
      pad: 2
    }
  };
  
 
  //Plotly.newPlot('second', bar_data,layout);
  //Plotly.newPlot('third', line_data,layout_line);