class draw_heatmaps extends graph_property{
    constructor(pool,w,h){
      super(pool,w,h);
      this.plotly_configuration = {
        modeBarButtonsToRemove:[
            'hoverClosestGl2d','toggleSpikelines',
            'autoScale2d','resetScale2d','zoomOut2d',
            'lasso2d','select2d','zoom2d','zoomIn2d',
            'hoverClosestPie','toggleHover','resetViews',
            'sendDataToCloud','resetViewMapbox','pan2d',
            'hoverClosestCartesian','hoverCompareCartesian',
            'zoomInGeo','zoomOutGeo','resetGeo','hoverClosestGeo'
        ],diplaylogo:false,responsive: true};

      //this.heatmap_data = new Array();
    }
    //x w y h 
    // start end 시간으로 표시 
    rebuild_data(start,end){

        //ary[] = PoolArrayTo3DArray()
        
        let heatmap_data = new Array();
        for(let i =start;  i<end; i++)
        {
            //히트맵 형식에 맞게 데이터를 다시 재구성 하는 코드 48*48
            heatmap_data[i] = Array.matrix(this.h, this.w, this.pool[0]['base_data'][i]);
            //this.heatmap_data[i] = Rotate(y,180,this.heatmap_data[i]);
        }
        return heatmap_data;
    }
    
    real_heatmap(container,data){
        Plotly.purge(container);
        
        //let cnt = data[0]['base_time'].length;
        let layout ={
            title: data[0]['base_time'][0],
            autosize: false,
            width: this.w*10,
            height: this.h*14,
            margin: {
                l: 100,
                r: 100,
                b: 100,
                t: 100,
                pad: 4
            },
            yaxis : {range:[0,this.h]},
            xaxis : {range:[0,this.w]}
            };
        let frames = new Array();
        frames[0] = Array.matrix(this.h,this.w, data[0]['base_data'][0])
        let heatmap_data = [
            {
                z: frames[0],
                type: 'heatmap',
                zmin:0,
                zmax:this.zaxis_max
            }
        ];
        Plotly.react(container, heatmap_data,layout);
    }

    heatmap_ani(container){
    Plotly.purge(container);
    this.start_ = this.view_data*(this.page-1);
    this.end_ = this.view_data > this.pool_cnt-this.start_ ? this.pool_cnt : this.view_data*this.page;
    let data = this.rebuild_data(this.start_,this.end_);
    let durate = 1000;
    // Create the main traces, one for each continent:
    let traces = [];
    traces.push({
        name: data[this.start_],
        z: data[this.start_],
        type:'heatmap',
        zmin:0,
        zmax:this.zaxis_max
    });
    
    let frames = [];
    for (let i =this.start_;  i<this.end_; i++) {
        frames.push({
        name:  this.pool[0]['base_time'][i],
        data:( [{
                z: data[i],
                type:'heatmap',
                zmin:0,
                zmax:this.zaxis_max
            }]
            )
        })
    }

    let sliderSteps = [];
    for (let i = this.start_; i<this.end_; i++) {
            sliderSteps.push({
            method: 'animate',
            label: this.pool[0]['base_time'][i],
            args: [[this.pool[0]['base_time'][i]], {
                mode: 'immediate',
                transition: {duration: durate},
                frame: {duration: durate, redraw: true},
            }]
        });
    }
    
    let layout = {
        title: this.pool[0]['base_time'][this.start_].slice(0,10)+' : '+this.pool[0]['base_time'][this.start_].slice(11,21)+' ~ '+this.pool[0]['base_time'][this.end_-1].slice(11,21),
        autosize: false,
        width: this.w*5,
        height:this.h*7,
        margin:{
            l: 100,
            r: 10,
            b: 30,
            t: 30,
            pad: 4
        },
        xaxis: {range: [0, this.w]},
        yaxis: {range: [0, this.h]},
        //hovermode: 'closest',
        updatemenus: [{
            x: 0,
            y: 0,
            yanchor: 'top',
            xanchor: 'left',
            showactive: false,
            direction: 'left',
            type: 'buttons',
            pad: {t: 80, r: 20},
            buttons: [{
                method: 'animate',
                args: [null, {
                mode: 'immediate',
                fromcurrent: true,
                transition: {duration: durate},
                frame: {duration: durate, redraw: true}
                }],
                label: '▶'
                }, {
                method: 'animate',
                args: [[null], {
                mode: 'immediate',
                transition: {duration: 0},
                frame: {duration: 0, redraw: true}
                }],
                label: '■'
            }]
        }],
        sliders: [{
            pad: {l: 110, t: 55},
            currentvalue: {visible: true,xanchor: 'right'},
            steps: sliderSteps
        }]};

        let plotly_configuration = this.plotly_configuration;
        plotly_configuration.data =traces;
        plotly_configuration.layout = layout;
        plotly_configuration.frames = frames;

        Plotly.react(container, {data:traces,layout:layout,frames:frames});
       
        console.log('히트맵 생성'+new Date());
    }

    heatmap_seq(container){
        this.start_ = this.view_data*(this.page-1);
        this.end_ = this.view_data>this.pool_cnt-this.start_ ? this.pool_cnt : this.view_data*this.page;
        let seq_end_cnt = this.view_data>this.pool_cnt-this.start_ ? this.pool_cnt-this.start_ : this.view_data;
        let data = this.rebuild_data(this.start_,this.end_);
        let sequence_data = [];

    for (let i=0;  i<seq_end_cnt; i++){
        let seq_index = this.start_+i;
        sequence_data[i] = 
        {
            z: data[seq_index],
            type: 'heatmap',
            zmin:0,
            zmax:this.zaxis_max,
            showscale: false,
            xaxis: `x${i==0?0:i+1}`,
            yaxis: `y${i==0?0:i+1}`,
            hovertemplate:"날짜 : "+this.pool[0]['base_time'][seq_index].slice(0,10)+"<br>"
            +"시간 :"+this.pool[0]['base_time'][seq_index].slice(11,21)
            +"<br><b>Z값<b> : %{z}<br>"
            +"<extra></extra>"
        };
    }
    let subplot_layout_size = this.end_-this.start_;
    let subplot_layout = 
    {
        hovermode: "closest",
        hoverlabel: { bgcolor: "#FFF" },
        margin: {
            l: 50,
            r: 50,
            b: 100,
            t: 100,
            pad: 4,
          },
          xaxis: {range: [0, this.w]},
          yaxis: {range: [0, this.h]},
        grid: {rows: 1, columns: subplot_layout_size, pattern: 'independent'},
    }
    
    for(let i=2;  i<seq_end_cnt+1; i++){
        subplot_layout[`xaxis${i}`] = {range:[0,this.w]};
        subplot_layout[`yaxis${i}`] = {range:[0,this.h]};
    }
    

    let responsive_div ={
        width:(this.w*5*(subplot_layout_size)),
        height:this.h*7
    }

    Plotly.purge(container);
    Plotly.react(container,sequence_data,subplot_layout);
    Plotly.relayout(container,responsive_div);
    //let heatmap_plot = document.getElementById('sequence')
        // heatmap_plot.on('plotly_relayout',function(data){
        // })
    console.log('시퀀스 생성 ' + new Date());

    }
}