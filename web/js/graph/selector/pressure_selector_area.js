class pressure_selector_area extends canvas_setting{
    constructor(canvas,w,h){
        super(canvas,w,h);
        //셀렉터 영역 좌표 계산
        this.scale_w=0,this.scale_h=0;
        //셀렉터 영역 선택 
        this.selector_rects = new Array();
        this.collision= new Array();
        //셀렉터 영역 선택 트리거 
        this.ismodify = false;
        
        //셀렉터 영역 수정 관련 선언
        this.radius_size = 7;
        this.rect ={
                        startX:undefined,
                        startY:undefined,
                        w:undefined,
                        h:undefined,
                        modify_name:undefined
                    };
        this.mouseX;
        this.mouseY;
        this.is_top_left = false;
        this.is_top_right = false;
        this.is_bottom_right = false;
        this.is_bottom_left = false;
        this.is_center = false; 
        this.is_select = true;
    } 
    //(외부에서 값을 바꿀 수 있도록)
    get set_ismodify(){return this.ismodify;}
    set set_ismodify(value){return this.ismodify=value;}

    get set_selector_rects(){return this.selector_rects;}
    set set_selector_rects(value){return this.selector_rects=value;}

    get set_collision(){return this.collision;}
    set set_collision(value){return this.collision=value;}

    get set_rect(){return this.rect;}
    set set_rect(value){return this.rect=value;}
    
    init(data,cur_index){//clear
        this.redraw(data,cur_index);
        //선택된 셀렉터 지우기 
        this.collision = new Array();
        this.rect = {
                        startX:undefined,
                        startY:undefined,
                        w:undefined,
                        h:undefined,
                        modify_name:undefined
                    };
    }

    redraw(data,cur_index){
        //let start = performance.now();
        this.set_ctx().clearRect(0,0,this.set_canvas().width,this.set_canvas().height);
        //히트맵 그리는곳
        this.heatmap_class.drawing(data,cur_index);
        //저장된 셀렉터 계속 그리는곳 
        this.selector_rects.forEach(selector=>{
            if(!this.isempty(selector)){
                this.set_ctx().fillStyle=this.get_selector_color(true);
                this.set_ctx().fillRect(selector.x,selector.y,selector.w,selector.h);
            }else{
                return;
            }
        });
        //let end = performance.now() - start;
        // let consoles = {
        //     log:`info ,${timeTF_Date(new Date())} ${timeTF_Time(new Date())},${end.toFixed(3)}, pressure_selector_area.js, class pressure_selector_area, function redraw, line 39, 영역 선택 속도`
        // }//log
        // js_logger(consoles);//log
    }

    picking(e,data,cur_index){ 
        let x = parseInt(e.offsetX);
        let y = parseInt(e.offsetY);
       
        this.scale_w = this.set_border().width/e.target.offsetWidth;
        this.scale_h = this.set_border().height/e.target.offsetHeight;
        
        //console.log(this.selector_rects);
        //if(this.ismodify === false){
            for(let index=0; index<this.selector_rects.length; index++){
                if(!this.isempty(this.selector_rects[index]) && typeof this.selector_rects[index] != null){
                    let left = this.selector_rects[index].w>0 ? this.selector_rects[index].x : this.selector_rects[index].x - Math.abs(this.selector_rects[index].w);
                    let top = this.selector_rects[index].h>0 ? this.selector_rects[index].y : this.selector_rects[index].y - Math.abs(this.selector_rects[index].h);
                   
                    let bottom = this.selector_rects[index].h>0 ? this.selector_rects[index].y+this.selector_rects[index].h : this.selector_rects[index].h;
                    let right = this.selector_rects[index].w>0 ? this.selector_rects[index].x+this.selector_rects[index].w : this.selector_rects[index].x;
                    this.init(data,cur_index);
                    if(right>x && left < x && bottom>y && top <y){
                        this.ismodify =true;
                        
                        this.set_ctx().clearRect(left,top,this.selector_rects[index].w,this.selector_rects[index].h);
                        
                        //this.init(data);
                        
                        this.set_ctx().fillStyle=this.get_selector_color(false);
                        this.set_ctx().fillRect(left,top,this.selector_rects[index].w,this.selector_rects[index].h);
                        this.collision.push(this.selector_rects[index]);
                        break;
                    }else{
                        this.ismodify =false;  
                        //this.init(data); 
                    }
                }
            }
        //}
    }

    select(e,data,cur_index){
        //충돌 후 선택
        if(this.ismodify === true && (this.collision.length>0)){
            let left =this.collision[0].x;
            let right = this.collision[0].x+Math.abs(this.collision[0].w);
            let top =this.collision[0].y;
            let bottom = this.collision[0].y+Math.abs(this.collision[0].h);
            let x = parseInt(e.offsetX);
            let y = parseInt(e.offsetY);
            if(right>x && left < x && bottom>y && top <y){
                this.ismodify =true; 
            }else{
                this.ismodify = false;
                this.redraw(data,cur_index);
            }
        }
    }

    delete(){
        this.ismodify = false;
        let contain_delete_selector_name;
        for(let index =0; index<this.selector_rects.length; index++){
            if(!this.isempty(this.selector_rects[index])){
                if(this.selector_rects[index]['name'] === this.collision[0]['name']){
                    contain_delete_selector_name = this.selector_rects[index]['name'];
                    delete this.selector_rects[index];
                }
            }
        }
        return { 'selectors' : this.selector_rects, 'delete_selector_name':contain_delete_selector_name};
       
    }

    modify_save(){
        
        let contain_modify_selector_name;
        for(let index =0; index<this.selector_rects.length; index++){
            if(!this.isempty(this.selector_rects[index])){
                if(this.selector_rects[index]['name'] === this.rect['modify_name']){
                    this.selector_rects[index]['x'] = this.rect['w']>0 ? this.rect['startX'] : this.rect['startX'] - Math.abs(this.rect['w']);
                    this.selector_rects[index]['y'] = this.rect['h']>0 ? this.rect['startY'] : this.rect['startY'] - Math.abs(this.rect['h']);
                    this.selector_rects[index]['w'] =Math.abs(this.rect['w']);
                    this.selector_rects[index]['h'] =Math.abs(this.rect['h']);
                    contain_modify_selector_name = this.selector_rects[index]['name'];
                }
            }
        }
        return { 'selectors' : this.selector_rects, 'modify_selector_name':contain_modify_selector_name};
    }
    
    modify_draw(data,cur_index){
        
        this.redraw(data,cur_index);
        this.set_ctx().fillStyle = this.get_selector_color(false);
        this.set_ctx().fillRect(this.rect['startX'], this.rect['startY'], this.rect['w'], this.rect['h']);
        this.modify_spot();
    }
    modify_checkspot(number1,number2){
        return Math.abs(number1 - number2) < this.radius_size;
    }

    modify_cricle(x, y) {
        this.set_ctx().fillStyle = "#FF0000";
        this.set_ctx().beginPath();
        this.set_ctx().arc(x, y, this.radius_size, 0, 2 * Math.PI);
        this.set_ctx().fill();
    }
    
    modify_spot() {
        let left_x = this.rect['w']>0 ? this.rect['startX'] + this.radius_size : this.rect['startX'] + this.rect['w'] + this.radius_size;
        let top_y = this.rect['h']>0 ? this.rect['startY'] + this.radius_size :  this.rect['startY'] + this.rect['h'] + this.radius_size;
        let right_x = this.rect['w']>0 ? this.rect['startX'] + this.rect['w'] - this.radius_size : this.rect['startX'] - this.radius_size;
        let bottom_y = this.rect['h']>0 ? this.rect['startY'] + this.rect['h'] - this.radius_size : this.rect['startY'] - this.radius_size;
    
        this.modify_cricle(left_x, top_y); //위쪽의 왼쪽
        this.modify_cricle(right_x, top_y);

        this.modify_cricle(right_x,bottom_y ); //아래의 오른쪽 
        this.modify_cricle(left_x, bottom_y);
        
       
        let centerX = this.rect['startX'] + this.rect['w']*0.5;
        let centerY = this.rect['startY'] + this.rect['h']*0.5;
        this.modify_cricle(centerX,centerY);
    }

    modify_check(){
        let left_x = this.rect['w']>0 ? this.rect['startX'] + this.radius_size : this.rect['startX'] + this.rect['w'] + this.radius_size;
        let top_y = this.rect['h']>0 ? this.rect['startY'] + this.radius_size :  this.rect['startY'] + this.rect['h'] + this.radius_size;
        let right_x = this.rect['w']>0 ? this.rect['startX'] + this.rect['w'] - this.radius_size : this.rect['startX'] - this.radius_size;
        let bottom_y = this.rect['h']>0 ? this.rect['startY'] + this.rect['h'] - this.radius_size : this.rect['startY'] - this.radius_size;
     
        if (this.modify_checkspot(this.mouseX, left_x) && this.modify_checkspot(this.mouseY, top_y)) {
            this.is_top_left = true;
        }
        else if (this.modify_checkspot(this.mouseX, right_x) && this.modify_checkspot(this.mouseY, top_y)) {
            this.is_top_right = true;
        }
        else if (this.modify_checkspot(this.mouseX, left_x) && this.modify_checkspot(this.mouseY, bottom_y)) {
            this.is_bottom_left = true;
        }
        else if (this.modify_checkspot(this.mouseX, right_x) && this.modify_checkspot(this.mouseY, bottom_y)) {
            this.is_bottom_right = true;
        }
         else if (this.modify_checkspot(this.mouseX, this.rect['startX'] + this.rect['w']*0.5) && this.modify_checkspot(this.mouseY, this.rect['startY'] + this.rect['h']*0.5)) {
            this.is_center = true;
        }
    }
    
    modify_mousedown(e,data,cur_index){
        this.mouseX = parseInt(e.offsetX);
        this.mouseY = parseInt(e.offsetY);
        
        if (this.rect['w'] === undefined && (this.collision.length>0)) {
            this.rect['startX'] = this.collision[0]['x'];
            this.rect['startY'] = this.collision[0]['y'];
            this.rect['w'] = this.collision[0]['w']; 
            this.rect['h'] = this.collision[0]['h'];
            this.rect['modify_name'] = this.collision[0]['name'];
            this.modify_check();
           
        }else{
            this.modify_check();
        }

        this.modify_draw(data,cur_index);
    }

    modify_mousemove(e,data,cur_index){
        this.mouseX = e.offsetX;
        this.mouseY = e.offsetY;
        if (this.is_top_left) {
            this.rect['w'] += this.rect['startX'] - this.mouseX;
            this.rect['h'] += this.rect['startY'] - this.mouseY;
            this.rect['startX'] = this.mouseX;
            this.rect['startY'] = this.mouseY;
           
        } else if (this.is_top_right) {
            this.rect['w'] = Math.abs(this.rect['startX'] - this.mouseX);
            this.rect['h'] += this.rect['startY'] - this.mouseY;
            this.rect['startY'] = this.mouseY;
           
        } else if (this.is_bottom_left) {
            this.rect['w'] += this.rect['startX'] - this.mouseX;
            this.rect['h'] = Math.abs(this.rect['startY'] - this.mouseY);
            this.rect['startX'] = this.mouseX; 
        } else if (this.is_bottom_right) {
            this.rect['w'] = Math.abs(this.rect['startX'] - this.mouseX);
            this.rect['h'] = Math.abs(this.rect['startY'] - this.mouseY);
        }
         else if (this.is_center) {
            this.rect['startX'] = this.mouseX - this.rect['w']*0.5;
            this.rect['startY'] = this.mouseY - this.rect['h']*0.5;

            if(this.rect['startX']>this.canvas_width - this.rect['w']){
                this.rect['startX'] = this.canvas_width- this.rect['w'];
            } else if(this.rect['startX']<0){
                this.rect['startX'] = 0;
            }

            if(this.rect['startY']>this.canvas_height - this.rect['h']){
                this.rect['startY'] = this.canvas_height - this.rect['h'];
            } else if(this.rect['startY']<0){
                this.rect['startY'] = 0;
            }
        }

        this.modify_draw(data,cur_index);
    }

    modify_mouseup(){
        this.is_top_left = false;
        this.is_top_right = false;
        this.is_bottom_right = false;
        this.is_bottom_left = false;
        this.is_center = false;
        this.is_select = false;
        this.modify_spot();
    }
}