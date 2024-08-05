class draw_3D extends graph_property{
  constructor(pool,x,y){
    super(pool,x,y);
    this.graph_3d_style = document.getElementById("3d_graph_style").value;
    this.graph_3d_width = (window.innerWidth *0.4).toString() + 'px';
    this.graph_3d_height = (window.innerHeight * 0.55).toString() + 'px';
  }
  ani_3d(containers){
    let vis_dataset = new vis.DataSet();
    this.start_ = this.view_data*(this.page-1);
    let seq_end_cnt = this.view_data>this.pool_cnt-this.start_ ? this.pool_cnt-this.start_ : this.view_data;
    
    let j =0;
    for (let t = 0; t < seq_end_cnt; t++) {
      for (let y = this.h-1; y >=0; y--) {
        for (let x = 0; x <this.w; x++) {
            vis_dataset.add({x: x, y: y,
                              filter:this.pool[0]['base_time'][this.start_+t], 
                              z: this.pool[0]['threed_data'][j+(this.wh*this.view_data*(this.page-1))] });
            j++;
        }
      }
    }
    let options = {
      width: this.graph_3d_width_,
      height: this.graph_3d_height_,
      style: this.graph_3d_style,
      showPerspective: true,
      showGrid: false,
      showShadow: false,
      keepAspectRatio: true,
      verticalRatio: 0.5,
      animationInterval: 1000, // milliseconds
      animationPreload: true,
      zMin:0,
      zMax:this.zaxis_max,
      filterLabel: "날짜/시간",
      tooltip: function (point) { return "압력 값 : <b>" + point.z + 
                                          "</b>"+ " x값: <b>" +point.x + "</b>"+ 
                                          " y값: <b>" +point.y + "</b>";},
      xValueLabel: function (value) {return value;},
      yValueLabel: function (value) {return value;}
    };

    // 그래프 생성 
    let container = document.getElementById(containers);
    let graph = new vis.Graph3d(container, vis_dataset, options);
    graph.setCameraPosition({horizontal:0,vertical:0.75,distance:1.7});

  }

  seq_3d(containers){
    let vis_dataset = new Array();
    this.start_ = this.view_data*(this.page-1);
    let seq_end_cnt = this.view_data>this.pool_cnt-this.start_ ? this.pool_cnt-this.start_ : this.view_data;
    
    let j =0;
    for(let i=0; i <seq_end_cnt; i++){
        vis_dataset[i] = new vis.DataSet();
    } 
    for (let t = 0; t < seq_end_cnt; t++) {
      for (let y = this.h-1; y >=0; y--) {
        for (let x = 0; x <this.w; x++) {
          vis_dataset[t].add({
                      x: x, 
                      y: y,
                      filter:this.pool[0]['base_time'][this.start_+t], 
                      z: this.pool[0]['threed_data'][j+(this.wh*this.view_data*(this.page-1))] 
                    });
          j++;
        }
      }
    }
   
    let options = {
      width: "600px",
      height: "500px",
      style: this.graph_3d_style,
      showPerspective: true,
      showGrid: true,
      showShadow: false,
      showAnimationControls:false,
      zMin: 0,
      zMax: this.zaxis_max, 
      tooltip: function (point) {
        return "z값: <b>" + point.z + "</b>"+ " x값: <b>" +point.x + "</b>"+ " y값: <b>" +point.y + "</b>";
      },
      xValueLabel: function (value) {return value;},
      yValueLabel: function (value) {return value;},
      keepAspectRatio: true,
      verticalRatio: 0.5,
      filterLabel: "날짜/시간",
      showYAxis:false,
      showXAxis:false
    };
    let container = new Array();
    for(let i=0; i<this.view_data; i++)
    {
        $('a').remove(`#${containers}${i}`);
    };
    let create_seq = document.getElementById(containers);
    let graph = new Array();
      for(let i=0; i<seq_end_cnt; i++)
      {
          let new_aTag = document.createElement('a');
          new_aTag.setAttribute('id', `${containers}${i}`);
          new_aTag.setAttribute('style', 'display:inline-block; padding: 10px 20px; margin-right:10px;');
          create_seq.appendChild(new_aTag);
          container[i] = document.getElementById(`${containers}${i}`);
          graph[i] = new vis.Graph3d(container[i], vis_dataset[i], options);
      }; 
  }
  get graph_3d_width_(){return this.graph_3d_width;}
  set graph_3d_width_(value){this.graph_3d_width = value;}

  get graph_3d_height_(){return this.graph_3d_height;}
  set graph_3d_height_(value){this.graph_3d_height=value;}

  get graph_3d_style_(){return this.graph_3d_style;}
  set graph_3d_style_(value){this.graph_3d_style=value;}

}
