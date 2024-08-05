class neo_heatmap extends canvas_setting{
    constructor(canvas,w,h){
        super(canvas,w,h);
        this.pool_data;

        this.plotly_heatmap_class;
    }
    get set_pool_data(){return this.pool_data;}
    set set_pool_data(value){return this.pool_data=value;}


    clear(){
        let ctx = this.set_ctx();
        ctx.clearRect(0,0,this.set_canvas().width,this.set_canvas().height);
    }
  
    drawing_number(pool,index){
        let xPos=0,yPos=0;
        let x=0,y=0,z=0;
        let ctx = this.set_ctx();
        //let start = performance.now();
        ctx.clearRect(0,0,this.w,this.h);
        
        if(!this.isempty(pool)){
            let data = pool[0]['base_data'];
            for(y=0; y<this.screen_height; y++){
                for(x=0; x<this.screen_width; x++){
                    ctx.font = "bold 10px";
                    ctx.fillStyle = this.get_color(data[index][z]);
                    ctx.fillText(data[index][z],xPos, yPos);
                    z++;
                    xPos += 23;
                }
                yPos += 23;
                xPos = 0;
            }
        }
    }
    drawing(data,total){
        let xPos=this.graph_s,yPos=this.graph_s;
        let x=0,y=0,z=0;
        let ctx = this.set_ctx()

        if(!this.isempty(data)){
            ctx.clearRect(0,0,this.set_canvas().width,this.set_canvas().height);
            this.pool_data = data;
            
            for(y=0; y<this.screen_height; y++){
                for(x=0; x<this.screen_width; x++){
                    
                    ctx.fillStyle = this.get_color(data[0]['base_data'][total][z]);
                    ctx.fillRect(xPos, yPos,this.rect_size, this.rect_size);
                    z++;
                    xPos += this.rect_size;
                }
                yPos += this.rect_size;
                xPos = this.graph_s;
            }
            
        }
        
    }

    get_part_data(topleft_x,topleft_y,width,height){
        //z값을 구할 수 있는 계산식  
        // z = x좌표 * this.screen_height + y좌표
        this.heatmap_info = new Array();
        let set_heatmap = new Array();
        let w = Math.floor(width/this.rect_size);
        let h = Math.floor(height/this.rect_size);
        let cnt = this.pool_data[0]['base_data'].length;
        let z_data = new Array(); 

        if(!this.isempty(this.pool_data)){
            for(let x=topleft_x; x<topleft_x+width; x+=this.rect_size){
                for(let y=topleft_y; y<topleft_y+height; y+=this.rect_size){
                    
                    let cal_x = Math.floor(x/this.rect_size);
                    let cal_y = Math.floor(y/this.rect_size);
   
                    z_data.push(Number(cal_x+cal_y*this.screen_width));
                }
            }

            for(let index =0; index<cnt; index++){
                for(let z_index=0; z_index<z_data.length; z_index++){
                    set_heatmap.push(this.pool_data[0]['base_data'][index][z_data[z_index]]);
                }
            }
        }
       
        this.heatmap_info = Array.new_matrix(w*h,cnt,set_heatmap);
        return this.heatmap_info;
    }

    statics(){
        let max, min, sum, mean,unique;
        let contain_data = new Array();
        for(let index=0; index<this.heatmap_info.length; index++){
            max = Math.max.apply(null,this.heatmap_info[index]);
            min = Math.min.apply(null,this.heatmap_info[index]);
            sum = this.heatmap_info[index].reduce((prev,cur)=>prev + cur,0);
            mean = (sum/this.heatmap_info[index].length).toFixed(2);
            unique = _.values(
                        _.groupBy(this.heatmap_info[index]))
                            .map(data=>({'value':data[0],'count':data.length}));
            contain_data.push({'max':max, 'min':min, 'sum':sum, 'mean':mean, 'groupby':unique})
        }
     
        return contain_data
    }
   
    real_heatmap(data){
        this.set_ctx().clearRect(0,0,this.set_canvas().width,this.set_canvas().height);
        this.drawing(data,data[0]['base_data'].length-1);
    }

    time(container,data,index){
        if(data.length>0){
            //let cnt = data[0]['base_time'].length;
            let time = '시간 : ' +data[0]['base_time'][index]
            container.text(time);
        } else{
            container.text('');
        }
    }


}
var get_neo_index=0;//네오 히트맵 인덱스를 담는 변수
function canvas_draw(index){
    heatmap.drawing(pool,index);
    
    heatmap.time($('#neo_heatmap_time'),pool,index)
    list.draw(checked_index,pool,index);
    // console.log('재생바 셀렉터 모음')
    // console.log(selector.selector_rects);
    get_neo_index = index;//인덱스 갱신 
    list_modify_mode('#position_list',$("#neo_heatmap"),list);
    update_static(checked_index,index);
}
var isplaying = false;
function neo_heatmap_ani(start_index,end_index){
    //console.log('활성화')
    let tl = new TimelineLite({onUpdate:updateSlider, onComplete:onComplete, paused:true});
    let $slider = $('#neo_heatmap_slider');
    let $play = $('#play');
    let $stop = $('#stop');
    let end_cnt = end_index - start_index;
    let isplay_stop = false;
    
    //console.log('시작 : %d \n 끝 : %d \n 데이터개수: %d',start_index,end_index,end_cnt);
    let inx = isplay_stop ? get_neo_index : start_index;
   
    number_neo_heatmap_slider(start_index,end_index);

    canvas_draw(inx);

    get_heatmap();
    function updateSlider() {
        $slider.slider("value", tl.progress() * end_cnt);
    }
    function onComplete() {
        tl.pause();
    }
    
    tl.staggerTo($slider, end_cnt+2, {}, 0.02);
    
    $slider.slider({
        range: false,
        min: 0,
        max: end_cnt-1,
        step:1,
        slide: function (event, ui) {
        tl.progress( ui.value / end_cnt ).pause();
            let result_index = start_index+ui.value

            canvas_draw(result_index);
            
            get_neo_index = JSON.parse(JSON.stringify(result_index));
            isplay_stop = true;

        },
    });
    
    $play.off('click').on('click',function(){
        //console.log('플레이')
        let total = isplay_stop ? get_neo_index : start_index; 
        area.ismodify = false; // 셀렉터가 지워짐을 방지
        isplaying = true;
        
        if (tl.paused()) {
            if (tl.progress() === 1 ) {
            tl.restart();
            } else {
            tl.resume();
            }
            let interval = setInterval(function(){
                //console.log('반복중')
                
                canvas_draw(total);

                total++;

                $stop.off('click').on('click',function(){
                    isplaying = false;
                    clearInterval(interval);
                    isplay_stop = true;
                    get_neo_index = JSON.parse(JSON.stringify(total-1));
                    //console.log(get_neo_index);
                    tl.pause();
                })
                if(total == end_index){
                    //멈췄을때 같이 멈추는 조건 
                    clearInterval(interval);
                    isplaying = false;
                }
            },1000);
        }
    })
}

function number_neo_heatmap_slider(start_index,end_index){

    let $slider = $('#number_neo_heatmap_slider');
    let end_cnt = end_index - start_index;
    number_heatmap.drawing_number(pool,start_index);
    number_heatmap.time($('#number_time'),pool,start_index);
    $slider.slider({
        range: false,
        min: 0,
        max: end_cnt-1,
        step:1,
        slide: function (event, ui) {
            let result_index = start_index+ui.value

            number_heatmap.drawing_number(pool,result_index);
            number_heatmap.time($('#number_time'),pool,result_index);

        },
    });
}