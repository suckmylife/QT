class pressure_selector extends canvas_setting{
    constructor(canvas,w,h){
        super(canvas,w,h);
        //셀렉터 영역 그리는 트리거 
        this.isDown = false;
        //셀렉터 영역 그리는 좌표 계산
        this.start_x_pos = 0, this.start_y_pos = 0,this.end_x_pos = 0,this.end_y_pos = 0;
        this.scale_w=0,this.scale_h=0;
        this.startX =0, this.startY=0,this.endX=0,this.endY=0,this.moveH=0,this.moveW=0;
        //셀렉터 영역 저장
        this.selector_rects = new Array();
        this.selector_index = 0;
    }
    get set_selector_rects(){return this.selector_rects;}
    set set_selector_rects(value){return this.selector_rects=value;}

    get set_selector_index(){return this.selector_index;}
    set set_selector_index(value){return this.selector_index=value;}
    
    init(){
        //초기화 
        this.endX =0;
        this.startX =0;
        this.endY=0;
        this.startY=0;
        this.moveW=0;
        this.moveH=0;
    }

    redraw(data,cur_index){
        //let start = performance.now();
        this.set_ctx().clearRect(0,0,this.set_canvas().width,this.set_canvas().height);
        //히트맵 그리는곳
        //this.heatmap_class.draw(data);
        //console.log(cur_index);
        this.heatmap_class.drawing(data,cur_index);
        //저장된 셀렉터 계속 그리는곳 
        if(!this.isempty(this.selector_rects)){
            this.selector_rects.forEach(selector=>{
                if(!this.isempty(selector)){
                    this.set_ctx().fillStyle=this.get_selector_color(true);
                    this.set_ctx().fillRect(selector.x,selector.y,selector.w,selector.h);
                }else{
                    return;
                }
            });
        }
        // let end = performance.now() - start;
        // let consoles = {
        //     log:`info ,${timeTF_Date(new Date())} ${timeTF_Time(new Date())},${end.toFixed(3)}, pressure_selector.js, class pressure_selector, function redraw, line 53, 영역 그리는 속도`
        // }//log
        // js_logger(consoles);//log
    }

    clear(){
        //자세 초기화 
        this.selector_rects = new Array();
        this.selector_index =0;
    }
  
    draw(e){
        //e.stopPropagation();
        this.isDown = true;
        if(!this.isDown){return;}
        //히트맵의 셀을 알기 위한 좌표(world 좌표)
        this.scale_w = this.set_border().width/e.target.offsetWidth;
        this.scale_h = this.set_border().height/e.target.offsetHeight;
        this.start_x_pos = e.offsetX;
        this.start_y_pos = e.offsetY;
        //셀렉터를 그리기 위한 좌표 (canvas 좌표)
        this.startX = parseInt(this.start_x_pos);
        this.startY = parseInt(this.start_y_pos);
    }

    draw_move(e,data,cur_index){
        
        if (!this.isDown) {return;}

        let mouseX = e.offsetX;
        let mouseY = e.offsetY;
        let width = mouseX - this.startX;
        let height = mouseY - this.startY;
        
        this.redraw(data,cur_index);
        this.set_ctx().fillStyle=this.get_selector_color(true);
        this.set_ctx().fillRect(this.startX, this.startY, width, height);
        this.endX = mouseX;
        this.endY = mouseY;
        this.moveW = width;
        this.moveH = height;

    }


    save(){
        this.isDown = false;

        this.selector_index = this.selector_rects.length>0 ? this.selector_rects.length:0;

        let selector_rect={
            x:Math.min(this.endX,this.startX),
            y:Math.min(this.endY,this.startY),
            w:Math.abs(this.moveW),
            h:Math.abs(this.moveH),
            name:this.selector_index
        }
        
        if(selector_rect.w * selector_rect.h <2){return;}
        else{
            this.selector_rects.push(selector_rect);
        }
        
        //this.selector_index++;
        this.init();
    }

    load(data){
        this.clear();
        this.selector_rects = data['selectors'];
        // console.log('편집 후 셀렉터 모음')
        // console.log(this.selector_rects);
    }
}