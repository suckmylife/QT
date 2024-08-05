class draw_2D extends graph_property{
    constructor(pool,w,h){
        super(pool,w,h);
        this.plotly_configuration = {
            modeBarButtonsToRemove:[
                'hoverClosestGl2d','toggleSpikelines',
                'autoScale2d','zoomOut2d',
                'lasso2d','select2d','zoom2d','zoomIn2d',
                'hoverClosestPie','toggleHover',
                'sendDataToCloud','resetViewMapbox','pan2d',
                'hoverClosestCartesian','hoverCompareCartesian',
                'zoomInGeo','zoomOutGeo','resetGeo','hoverClosestGeo'
            ],diplaylogo:false,responsive: true};
        
    }

    line(container){
        console.log('라인그래프 콜'+new Date());
        Plotly.purge(container);
        let sum_data1 = new Array();
        let cnt =this.pool_cnt;
        for(let i=0; i<cnt; i++){
            sum_data1[i] = 0;
            for(let j =0; j<this.wh; j++){
                sum_data1[i] +=this.pool[0]['base_data'][i][j];
            }
        }

        let line_layout = [{
            type:'line',
            mode:'lines+markers',
            x: this.pool[0]['no_slash_time'],
            y: sum_data1,
            marker: {
                color: '#262A76',
                size:1,
                line: {
                    width: 2.5
                }
            },
        }]

        Plotly.react(container, line_layout, {
            title: '압력 합계 선 그래프',
            font: {size: 16},
            xaxis:{showgrid: false},
            yaxis:{showgrid: false}
        },this.plotly_configuration)
       
    }

    all_mean_line(container,data,titles){
      Plotly.purge(container);
      let line_layout = new Array();
      let time_cnt = data[0]['all_static'].length;
      let cnt = data.length;
      let sum = Array.new_matrix(time_cnt,cnt,0);
      let mean = Array.new_matrix(time_cnt,cnt,0);
      let all_sum = (arr1,row) => arr1.map((arr2,index)=>row[index]+arr2);
      let all = new Array();
      let all_wh=0;
      for(let index=0; index<cnt; index++){
        all_wh += data[index]['cell_info'][0].length
        for(let t_index=0; t_index<time_cnt; t_index++){
          mean[index][t_index] = ((data[index]['all_static'][t_index].sum)/(data[index]['cell_info'][0].length)).toFixed(2);
          sum[index][t_index] = data[index]['all_static'][t_index].sum;
        }
        
      }
      
      all = sum.reduce(all_sum);
      
      for(let index=0; index<all.length; index++){
        all[index] = Number((all[index]/all_wh).toFixed(2));
      }
      
      for(let index=0; index<cnt; index++){
        line_layout.push({
            type:'line',
            mode:'lines',
            x: this.pool[0]['no_slash_time'],
            y: mean[index],
            name: data[index]['title'],
        });
    }
  
    line_layout.push({
      type:'line',
      mode:'lines',
      x: this.pool[0]['no_slash_time'],
      y: all,
      name: '자세',
    });
  
      Plotly.react(container, line_layout, {
          title: `${titles} 압력 평균 선`,
          font: {size: 16},
          xaxis:{showgrid: false},
          yaxis:{showgrid: false}
      },this.plotly_configuration)
  
    }

  all_sum_line(container,data,titles){
    Plotly.purge(container);
    let line_layout = new Array();
    let time_cnt = data[0]['all_static'].length;
    let cnt = data.length;
    let sum = Array.new_matrix(time_cnt,cnt,0);
    let all_sum = (arr1,row) => arr1.map((arr2,index)=>row[index]+arr2);
    let all = new Array(); 
    for(let index=0; index<cnt; index++){
      for(let t_index=0; t_index<time_cnt; t_index++){
          sum[index][t_index]=data[index]['all_static'][t_index].sum;
      }
      
    }
    all = sum.reduce(all_sum);
    for(let index=0; index<cnt; index++){
      line_layout.push({
          type:'line',
          mode:'lines',
          x: this.pool[0]['no_slash_time'],
          y: sum[index],
          name: data[index]['title'],
      });
  }

  line_layout.push({
    type:'line',
    mode:'lines',
    x: this.pool[0]['no_slash_time'],
    y: all,
    name: '자세',
  });

    Plotly.react(container, line_layout, {
        title: `${titles} 압력 합계 선`,
        font: {size: 16},
        xaxis:{showgrid: false},
        yaxis:{showgrid: false}
    },this.plotly_configuration)

  }

  bar_classify(data,state,part_index){
    let step_one = new Array();
    let step_two = new Array();
    let step_three = new Array();
    let step_four = new Array();

    let one_ = new Array();
    let two_ = new Array();
    let three_ = new Array();
    let four_ = new Array();

    let one,two,three,four = false;
    function classifies(){
        one ? one = false : one_.push(0);
        two ? two = false : two_.push(0);
        three ? three = false : three_.push(0);
        four ? four = false : four_.push(0);
  
        step_one.push(one_.reduce((prev,cur)=> prev+cur,0));
        step_two.push(two_.reduce((prev,cur)=> prev+cur,0));
        step_three.push(three_.reduce((prev,cur)=> prev+cur,0));
        step_four.push(four_.reduce((prev,cur)=> prev+cur,0));
  
        one_ = new Array();
        two_ = new Array();
        three_ = new Array();
        four_ = new Array();
    }
    function if_classify(d,c){
      if(d >=0 && d<257){
        one_.push(c)
        one = true;
      }else if(d >=257 && d<513){
        two_.push(c)
        two = true;
      }else if(d >=513 && d<769){
        three_.push(c)
        three = true;
      }else if(d >=769 && d<1030){
        four_.push(c)
        four = true;
      }
    }

    //압력 구성성분 데이터 가공
    if(data[0].hasOwnProperty('base_data') && state == 'none'){
      let cnt = data[0]['base_data'].length;
      let info = data[0]['base_data'];
      let unique;
      let contain_data = new Array();
      for(let index = 0; index < cnt; index++){
        unique = _.values(
          _.groupBy(info[index]))
              .map(data=>({'value':data[0],'count':data.length}));
        contain_data.push(unique);
      }
      let uq_cnt = contain_data.length;
      
      for(let index = 0; index < uq_cnt; index++){
        for(let index_=0; index_<1025; index_++){
          if(contain_data[index][index_]){
            let d = contain_data[index][index_]['value'];
            let c = contain_data[index][index_]['count'];
            if_classify(d,c);
    
          }
        }
        classifies();
      }

    }else if(data[0].hasOwnProperty('cell_info') && state == 'none'){
      let info_cnt = data[0].cell_info.length;//시간당 존재하는 셀정보개수
      let cnt = data.length; //영역 개수 
      let groupby = Array.new_matrix(0,info_cnt,0);
      let unique;
      let contain_data = new Array();
      
      for(let index=0; index<info_cnt; index++){
        for(let len =0; len<cnt; len++){
          //groupby[index].push(data[len].cell_info[index])
          groupby[index].push(...data[len].cell_info[index]);
        }
        
      }
      
      for(let index = 0; index < info_cnt; index++){
        unique = _.values(
                    _.groupBy(groupby[index]))
                        .map(data=>({'value':data[0],'count':data.length}));
        contain_data.push(unique);
      }
      let uq_cnt = contain_data.length;
      for(let index = 0; index < uq_cnt; index++){
        for(let index_=0; index_<1025; index_++){
          if(contain_data[index][index_]){
            let d = contain_data[index][index_]['value'];
            let c = contain_data[index][index_]['count'];
            if_classify(d,c);
          }
        }
        classifies();
      }

    }else if(data[0].hasOwnProperty('cell_info') && state == 'parts'){
      let info_cnt = data[0].cell_info.length;//시간당 존재하는 셀정보개수
      let groupby = Array.new_matrix(0,info_cnt,0);
      let unique;
      let contain_data = new Array();
      
      for(let index=0; index<info_cnt; index++){
          groupby[index].push(...data[part_index].cell_info[index]);
      }
      
      for(let index = 0; index < info_cnt; index++){
        unique = _.values(
                    _.groupBy(groupby[index]))
                        .map(data=>({'value':data[0],'count':data.length}));
        contain_data.push(unique);
      }
      let uq_cnt = contain_data.length;
      for(let index = 0; index < uq_cnt; index++){
        for(let index_=0; index_<1025; index_++){
          if(contain_data[index][index_]){
            let d = contain_data[index][index_]['value'];
            let c = contain_data[index][index_]['count'];
            if_classify(d,c);
          }
        }
        classifies();
      }

    }
    else{
      let cnt = data.length;
      for(let index = 0; index < cnt; index++){
        for(let index_=0; index_<1025; index_++){
          if(data[index][index_]){
            let d = data[index][index_]['value'];
            let c = data[index][index_]['count'];
            if_classify(d,c);
          }
        }
        classifies();
      }  

    }

    return [step_one,step_two,step_three,step_four]

  }
  


  part_bar(container,data,titles){
    //console.log('자세 막대 그래프 콜'+new Date());
    Plotly.purge(container);
    let classify_data = this.bar_classify(data,'none',0);
    let composition = [
        {
          x: this.pool[0]['no_slash_time'],
          y: classify_data[0],
          name: '1~256',
          type: 'bar'
        }, {
          x:this.pool[0]['no_slash_time'],
          y: classify_data[1],
          name: '257~512',
          type: 'bar'
        }, {
          x: this.pool[0]['no_slash_time'],
          y: classify_data[2],
          name: '513~768',
          type: 'bar'
        },{
          x: this.pool[0]['no_slash_time'],
          y: classify_data[3],
          name: '767~1024',
          type: 'bar'
        }
      ]
      
      let layout = {
        title:  `${titles} 압력 구성성분 막대 그래프`,
        showlegend: true,
        barmode: 'stack',
        bargap: 0.618
      };
      
      Plotly.react(container, composition, layout,this.plotly_configuration);

  }

  multi_bar(container,data,titles,state,index){
    Plotly.purge(container);
    let classify_data = this.bar_classify(data,state,index);
    let composition = [
        {
          x: this.pool[0]['no_slash_time'],
          y: classify_data[0],
          name: '1~256',
          type: 'bar'
        }, {
          x:this.pool[0]['no_slash_time'],
          y: classify_data[1],
          name: '257~512',
          type: 'bar'
        }, {
          x: this.pool[0]['no_slash_time'],
          y: classify_data[2],
          name: '513~768',
          type: 'bar'
        },{
          x: this.pool[0]['no_slash_time'],
          y: classify_data[3],
          name: '767~1024',
          type: 'bar'
        }
      ]
      
      let layout = {
        title:  `${titles} 압력 구성성분 막대 그래프`,
        showlegend: true,
        barmode: 'stack',
        bargap: 0.618
      };
      
      Plotly.react(container, composition, layout,this.plotly_configuration);

  }
    update_line(container,data){
        //console.log('업데이트 함수가 불러와졌다'+new Date())
        let sum_data1 = 0;
        sum_data1 = data[0]['base_data'][0].reduce(function add(sum,cur){return sum+cur;},0);
       
        Plotly.extendTraces(container,
        { 
         x: [[data[0]['no_slash_time'][0]]],
          y:[[sum_data1]],
        },[0])
    }

}