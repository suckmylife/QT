

function unupper(str){
    str = String(str);
    return str.replace(/\"/gi,'');
}

function draw_heatmap(data){

    var filter_ip_data_1 = new Array();
    var filter_ip_data_2 = new Array();
    var filter_ip_data_3 = new Array();
    var filter_ip_data_4 = new Array();

    var cnt = data['cnt'];
    var selected_ip = $('#dropdowns option:selected').val();

    var j = 0;
    var zaxis_max = 50;

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

    
    for(i=0; i <cnt; i++)
    {
        if(data['data'][i]['ip'] == selected_ip)
        {
            //선택된 아이피의 데이터를 받아오는 코드
            filter_ip_data_1[j] = JSON.stringify(data['data'][i]['data']);
            //쌍따옴표 찌우는 코드
            filter_ip_data_2[j] = unupper(filter_ip_data_1[j]);
            //쉼표 기준으로 나눠 넣는 코드
            filter_ip_data_3[j] = filter_ip_data_2[j].split(',').map(function(item){
            return parseInt(item,10)
            });
            //히트맵 형식에 맞게 데이터를 다시 재구성 하는 코드 48*48
            filter_ip_data_4[j] = Array.matrix(48, 48, filter_ip_data_3[j]);
            j++;
        }
        else
        {
            continue;
        }
        
    }

    var layout ={
                title: selected_ip,
                autosize: false,
                width: 500,
                height: 500,
                margin: {
                    l: 100,
                    r: 100,
                    b: 100,
                    t: 100,
                    pad: 4
                },
                yaxis : {range:[0,48]},
                xaxis : {range:[0,48]}
                };

    var heatmap_data = [
        {
            z: filter_ip_data_4[0],
            type: 'heatmap',
            zsmooth: 'best',
            zmin:0,
            zmax:zaxis_max
        }
    ];
    Plotly.newPlot('first', heatmap_data,layout);
}

function draw_heatmap_ani(data)
{
    var selected_ip = $('#dropdowns option:selected').val();
    var cnt = data['cnt'];
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
    //filter function 
    var filter_ip_data_1 = new Array();
    var filter_ip_data_2 = new Array();
    var filter_ip_data_3 = new Array();
    var filter_ip_data_4 = new Array();

    var filter_time_data_1 = new Array();
    var filter_time_data_2 = new Array();
    var filter_time_data_3 = new Array();

    var j =0;
    //필터링 구현
    for(i=0; i <cnt; i++)
    {
        if(data['data'][i]['ip'] == selected_ip)
        {
            //선택된 아이피의 데이터를 받아오는 코드
            filter_ip_data_1[j] = JSON.stringify(data['data'][i]['data']);
            filter_time_data_1[j] = JSON.stringify(data['data'][i]['time'])
            //쌍따옴표 찌우는 코드
            filter_ip_data_2[j] = unupper(filter_ip_data_1[j]);
            filter_time_data_2[j] = unupper(filter_time_data_1[j]);
            //쉼표 기준으로 나눠 넣는 코드
            filter_ip_data_3[j] = filter_ip_data_2[j].split(',').map(function(item){
            return parseInt(item,10)
            });
            filter_time_data_3[j] = filter_time_data_2[j].split(',').map(function(item){
                return parseInt(item,10)
                });
            //히트맵 형식에 맞게 데이터를 다시 재구성 하는 코드 48*48
            filter_ip_data_4[j] = Array.matrix(48, 48, filter_ip_data_3[j]);
            j++;
        }
        else
        {
            continue;
        }
        
    }
    
    
    var max_frame = 100;

    // Create the main traces, one for each continent:
    var traces = [];
    traces.push({
        name: filter_time_data_2[0],
        z: filter_ip_data_4[0],
        type:'heatmap',
        zsmooth: 'best',
        zmin:0,
        zmax:30
    });
    

    var frames = [];
    for (i = 1; i < max_frame; i++) {
        frames.push({
        name: filter_time_data_2[i],
        data:( [
            {
                z: filter_ip_data_4[i],
                type: 'heatmap',
                zsmooth: 'best',
                zmin:0,
                zmax:30
            }
            ])
        })
    }

    var sliderSteps = [];
    for (i = 0; i < max_frame; i++) {
        sliderSteps.push({
        method: 'animate',
        label: filter_time_data_2[i],
        args: [[filter_time_data_2[i]], {
            mode: 'immediate',
            transition: {duration: 90},
            frame: {duration: 90, redraw: true},
        }]
        });
    }
    
    var layout = {
        title: $('#dropdowns option:selected').val(),
        autosize: false,
        width: 500,
        height: 500,
        margin: 
        {
            l: 100,
            r: 100,
            b: 100,
            t: 100,
            pad: 1
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
        pad: {t: 87, r: 10},
        buttons: [{
            method: 'animate',
            args: [null, {
            mode: 'immediate',
            fromcurrent: true,
            transition: {duration: 90},
            frame: {duration: 120, redraw: true}
            }],
            label: 'Play'
        }, {
            method: 'animate',
            args: [[null], {
            mode: 'immediate',
            transition: {duration: 0},
            frame: {duration: 0, redraw: true}
            }],
            label: 'Pause'
        }]
        }],

        sliders: [{
        pad: {l: 130, t: 55},
        currentvalue: {
            visible: true,
            prefix: 'Time:',
            xanchor: 'right',
            font: {size: 20, color: '#666'}
        },
        steps: sliderSteps
        }]
    };
    
    // Create the plot:
    Plotly.plot('sequence', {
        data: traces,
        layout: layout,
        frames: frames
    });
}



