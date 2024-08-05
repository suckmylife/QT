class pressure_selector_memo extends canvas_setting{
    constructor(canvas,w,h){
        super(canvas,w,h);
        this.memo_x = 0;
        this.memo_y = 0;
        this.memo_ = '';
        this.memo_scope = new Array();
        this.font = "bold 10px";
        this.fillStyle = "black";
    }
    get set_memo_x(){return this.memo_x;}
    set set_memo_x(value){return this.memo_x=value;}

    get set_memo_y(){return this.memo_y;}
    set set_memo_y(value){return this.memo_y=value;}

    get set_memo(){return this.memo_;}
    set set_memo(value){return this.memo_=value;}
    
    clear(){
        this.memo_scope = new Array();
    }

    redraw(){
        if(this.memo_scope.length==0){return;}
        else{
            //console.log(this.memo_scope);
            for(let rememo in this.memo_scope){
                if(!this.isempty(rememo) && !this.isempty(this.memo_scope[rememo])){
                    //console.log('메모스코프 : %d',rememo);
                    let find_second = Object.keys(this.memo_scope[rememo]);
                    //console.log('메모스코프의 셀렉터영역 : %d',find_second);
                    let key = this.memo_scope[rememo][find_second];
                    //console.log('메모스코프의 셀렉터영역 내용 : %s %d %d', key['memo'], key['x'],  key['y']);
                    this.set_ctx().font = this.font;
                    this.set_ctx().fillStyle = this.fillStyle;
                    this.set_ctx().fillText(key['memo'], key['x'],  key['y']);
                }
            }
        }
    }

    draw(text){
        this.set_ctx().clearRect(this.memo_x,this.memo_y-10,90,10);
        this.set_ctx().font = this.font;
        this.set_ctx().fillStyle = this.fillStyle;
		this.set_ctx().fillText(text, this.memo_x, this.memo_y-1);
        this.memo_ = text;
    }

    delete(data){
        let name = data['delete_selector_name'];
        for(let index =0; index<this.memo_scope.length; index++){
            if(!this.isempty(this.memo_scope[index])){
                if(this.memo_scope[index].hasOwnProperty(name)){
                    delete this.memo_scope[index];
                }
            }
        }
        this.load({'memos':this.memo_scope});
        this.redraw();
    }

    modify(data){
        if(!this.isempty(data)){
            let name = data['modify_selector_name'];
            let new_coord = data['selectors'];
            //let contain_index;
            //console.log('받은 데이터 : %s',JSON.stringify(new_coord));
            for(let index =0; index<this.memo_scope.length; index++){
                //저장된 메모만큼 루프를 돌아라 
                if(!this.isempty(this.memo_scope[index])){
                    //메모가 비어있지 않을 때만 실행하라 
                    let key_array = Object.keys(this.memo_scope[index]);
                    //console.log('영역이름: %d, 메모키: %d',name, key_array[0]);
                    if(name == key_array[0]){
                        //충돌된 영역의 이름과 메모의 키가 일치하면 실행하라 
                        this.memo_scope[index][name]['x'] = new_coord[name]['x'];
                        this.memo_scope[index][name]['y'] = new_coord[name]['y'];
                        this.memo_ = this.memo_scope[index][name]['memo'];
                        //keyup할때 x,y 좌표갱신 코드 
                        this.memo_x = new_coord[name]['x'];
                        this.memo_y = new_coord[name]['y'];
                    }
                    
                }
            }
            this.redraw()
        }
    }

    property_memo_text(area_name){
        for(let index =0; index<this.memo_scope.length; index++){
            //저장된 메모만큼 루프를 돌아라 
            if(!this.isempty(this.memo_scope[index])){
                //메모가 비어있지 않을 때만 실행하라 
                let key_array = Object.keys(this.memo_scope[index]);
                //console.log('영역이름: %d, 메모키: %d',name, key_array[0]);
                if(area_name == key_array[0]){
                    //충돌된 영역의 이름과 메모의 키가 일치하면 실행하라 
                    return this.memo_scope[index][area_name]['memo'];
                }
                
            }
        }
    }


    save(selector_num){
        //메모를 저장할 구조체 
        let memo_dict = {};
        memo_dict[`${selector_num}`] ={
            x:this.memo_x,
            y:this.memo_y-1,
            memo:this.memo_
        }
        let max = this.memo_scope.length+1;
        let index =0;
       do{
            if(this.memo_scope[index] && typeof this.memo_scope[index] != 'undefined'){//주어진 dict안에 메모가 있을때
                if(this.memo_scope[index].hasOwnProperty(selector_num)){//중복일때
                  //  console.log('중복이다 index : %d',index)
                    this.memo_scope[index]=memo_dict;
                    break;
                }
            }else{//새로추가해야 할때
                //console.log('새로추가한다 index : %d',selector_num);
                this.memo_scope[selector_num]=memo_dict;
                break;
            }
            index++;
       }while(index < max)
    }

    load(data){
        this.clear();
        this.memo_scope = data['memos'];
    }
}