

function unupper(str){
    str = String(str);
    return str.replace(/\"/gi,'');
}

function draw_heatmap(){
    var data = document.querySelectorAll('[id="select_ip_data"]');
    var data_arr = [...data].map(input=>input.value);
    var selected_ip = $('#select_ip').val();
    var cnt = $('#select_cnt').val();

    var zaxis_max = 50;
    var tf_data = new Array();
    var del = new Array();
    //히트맵 데이터에 맞게 다시 데이터 리빌딩 function
    Array.matrix = function (m, n, initial) {
        var a, i, j, mat = [];
        var k = 0;
        for (i = 0; i < m; i += 1) {
            a = [];
            for (j = 0; j < n; j += 1) {
                a[j] = initial[k];
                k++;
            }
            mat[i] = a;
        }
        return mat;
    };
    
    //쉼표 기준으로 나눠서 다시 배열로 만드는 코드
    for(var i =0; i<cnt; i++)
    {
        del[i] = data_arr[i].split(',').map(function(item){
            return parseInt(item,10)
            });
    }
    
    for(var i =0; i<cnt; i++)
    {
        tf_data[i] = Array.matrix(48, 48, del[i]);
    }
    
    
    var layout ={
                title: {text: selected_ip},
                autosize: false,
                width: 600,
                height: 600,
                margin: {
                    l: 50,
                    r: 50,
                    b: 100,
                    t: 100,
                    pad: 4
                },
                yaxis : {range:[0,48]},
                xaxis : {range:[0,48]}
                };

    var heatmap_data = [
        {
            z: tf_data[0],
            type: 'heatmap',
            //zsmooth: 'best',
            zmin:0,
            zmax:zaxis_max
        }
    ];
    Plotly.newPlot('heatmap', heatmap_data,layout);
}

function draw_heatmap_ani()
{
    Array.matrix = function (m, n, initial) {
        var a, i, j, mat = [];
        var k = 0;
        for (i = 0; i < m; i += 1) {
            a = [];
            for (j = 0; j < n; j += 1) {
                a[j] = initial[k];
                k++;
            }
            mat[i] = a;
        }
        return mat;
    };

    var data = document.querySelectorAll('[id="select_ip_data"]');
    var data_arr = [...data].map(input=>input.value);

    var time_data = document.querySelectorAll('[id="select_time_str"]');
    var time_data_arr = [...time_data].map(input=>input.value);

    var cnt = $('#select_cnt').val();
    var selected_ip = $('#select_ip').val();

    var zaxis_max = 50;

    var tf_data = new Array();
    var del = new Array();
    var time_data_1 = new Array();
    var time_data_2 = new Array();
    var time_data_3 = new Array();

    for(var i =0; i<time_data_arr.length; i++)
    {
        time_data_1[i] = time_data_arr[i].replace(/,/g, "");
        time_data_2[i] = time_data_1[i].split(' ').map(function(item){
            return item
        });
        time_data_3[i] = time_data_2[i][1] + " / frame : "+(i+1);
    }

    //쉼표 기준으로 나눠 넣는 코드
    //쉼표 기준으로 나눠서 다시 배열로 만드는 코드
    for(var i =0; i<cnt; i++)
    {
        del[i] = data_arr[i].split(',').map(function(item){
            return parseInt(item,10)
            });
    }
    for(var i =0; i<cnt; i++)
    {
        tf_data[i] = Array.matrix(48, 48, del[i]);
    }
   
    //히트맵 형식에 맞게 데이터를 다시 재구성 하는 코드 48*48
    var j =0;
    //
    var traces = [];
    traces.push({
        name: time_data_3[0],
        z: tf_data[0],
        type:'heatmap',
        zsmooth: 'best',
        zmin:0,
        zmax:zaxis_max
    });
   
    var frames = [];
    for (i = 0; i < cnt; i++) {
        frames.push({
        name: time_data_3[i],
        data:( [
            {
                z: tf_data[i],
                type: 'heatmap',
                //zsmooth: 'best',
                zmin:0,
                zmax:zaxis_max
            }
            ])
        })
    }

    var sliderSteps = [];
    for (i = 0; i < cnt; i++) {
        sliderSteps.push({
        method: 'animate',
        label: time_data_3[i],
        args: [[time_data_3[i]], {
            mode: 'immediate',
            transition: {duration: 400},
            frame: {duration: 400, redraw: true},
        }]
        });
    }
    
    var layout = {
        title: selected_ip,
        autosize: false,
        width: 700,
        height: 700,
        margin: 
        {
            l: 100,
            r: 100,
            b: 50,
            t: 50,
            pad: 4
        },
        xaxis: {
        title: 'x',
        range: [0, 48]
        },
        yaxis: {
        title: 'y',
        range: [0, 48]
        },
        hovermode: 'closest',
        updatemenus: [{
        x: 0,
        y: 0,
        yanchor: 'top',
        xanchor: 'left',
        showactive: false,
        direction: 'left',
        type: 'buttons',
        pad: {t: 167, r: 10},
        buttons: [{
            method: 'animate',
            args: [null, {
            mode: 'immediate',
            fromcurrent: true,
            transition: {duration: 400},
            frame: {duration: 600, redraw: true}
            }],
            label: '재생'
        }, {
            method: 'animate',
            args: [[null], {
            mode: 'immediate',
            transition: {duration: 0},
            frame: {duration: 0, redraw: true}
            }],
            label: '정지'
        }]
        }],

        sliders: [{
        pad: {l: 60, t: 55},
        currentvalue: {
            visible: true,
            prefix: '시간 : ',
            xanchor: 'right',
            font: {size: 20, color: '#666'}
        },
        steps: sliderSteps
        }]
    };
    
    // Create the plot:
    Plotly.plot('heatmap_ani', {
        data: traces,
        layout: layout,
        frames: frames
    });
}

class Queue {
    constructor() {
      this.storage = {};
      this.front = 0;
      this.rear = 0;
    }
    size() {
      if (this.storage[this.rear] === undefined) {
        return 0;
      } else {
        return this.rear - this.front + 1
      }
    }
    
    // 처음 값이 들어올 때를 제외하고는, rear가 1씩 증가하도록 구현했습니다.
    add(element) {
      if (this.size() === 0) {
        this.storage['0'] = element;
      } else {
        this.rear += 1;
        this.storage[this.rear] = element;
      }
    }
    popleft() {
      let temp;
      if (this.front === this.rear) {
        temp = this.storage[this.front];
        delete this.storage[this.front];
        this.front = 0;
        this.rear = 0;
        return temp;
      } 
      else 
      {
        let dump;
        if(this.storage[this.front] > this.storage[this.rear]){
            dump = this.storage[this.front];
            this.storage[this.front] = this.storage[this.rear];
            this.storage[this.rear] = dump;

            temp = this.storage[this.front];
            delete this.storage[this.front];
            this.front += 1;
        }
        else{
            temp = this.storage[this.front];
            delete this.storage[this.front];
            this.front += 1;
        }
        
        return temp;
      }
    }
  }

function linear_graph(){
    var line_data = [{
        x: ['2022-05-02T16:24:56Z', '2022-05-02T16:24:57Z','2022-05-02T16:24:58Z','2022-05-02T16:24:59Z'],
        y: [50, 10, 150, 17],
        type: 'lines'
    }];
    var layout_line ={
        title: "시간에 따른 압력센서의 누적량",
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

    Plotly.newPlot('line', line_data,layout_line);

    var from_to_data = document.getElementById('line');
    var q = new Queue();

    var select_xaxis = from_to_data.on('plotly_click',function(data){
        q.add(data.points[0].x);
        if(q.size()===2)
        {
            document.getElementById('f_t1').value = q.popleft(); 
            document.getElementById('f_t2').value = q.popleft();
        }
    });
}

function bar_graph(){
  
    var bar_data = [
        {
        x: [100,20,1,1,1,0.6,1],
        type: 'histogram'
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
    
    Plotly.newPlot('bar', bar_data,layout);
    
}
function draw_sequence(){
    var data = document.querySelectorAll('[id="select_ip_data"]');
    var data_arr = [...data].map(input=>input.value);
    var cnt = $('#select_cnt').val();
    var zaxis_max = 50;

    //데이터 가공
    Array.matrix = function (m, n, initial) {
        var a, i, j, mat = [];
        var k = 0;
        for (i = 0; i < m; i += 1) {
            a = [];
            for (j = 0; j < n; j += 1) {
                a[j] = initial[k];
                k++;
            }
            mat[i] = a;
        }
        return mat;
    };

    var del = new Array();
    var tf_data = new Array();
     //쉼표 기준으로 나눠서 다시 배열로 만드는 코드
     for(var i =0; i<cnt; i++)
     {
         del[i] = data_arr[i].split(',').map(function(item){
             return parseInt(item,10)
             });
     }
     
     for(var i =0; i<cnt; i++)
     {
         tf_data[i] = Array.matrix(48, 48, del[i]);
     }
    
    var layout1 =
    {
        autosize: true,
        width: 300,
        height: 300,
        margin: 
        {
            l:20,
            r:20,
            b: 20,
            t: 20,
            pad: 4
        },
    };
    var heatmap_data =new Array();
    for(var i=0; i<cnt; i++)
    {
        heatmap_data[i] = ( [
            {
                z: tf_data[i],
                type: 'heatmap',
                zsmooth: 'best',
                zmin:0,
                zmax:zaxis_max,
                showscale:false
            }
            ]);
        
    }
    let Create_Device_Id = document.getElementById('sequence_ul');
    for(var i=0; i<cnt; i++)
    {
        let new_aTag = document.createElement('li');
        new_aTag.setAttribute('id', `heatmap${i}`);
        Create_Device_Id.appendChild(new_aTag);
        Plotly.newPlot(`heatmap${i}`, heatmap_data[i],layout1);
    };

}