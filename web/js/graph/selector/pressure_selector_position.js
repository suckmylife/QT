class pressure_selector_position extends pressure_selector_list{
    constructor(id){
        super(id);
        this.selector_list_class;
    }
   
    get set_position_cell_info(){return this.position_cell_info;}
    set set_position_cell_info(value){return this.position_cell_info=value;}

    get set_selector_list_class(){return this.selector_list_class;}
    set set_selector_list_class(value){return this.selector_list_class=value;}
  
    add(index){ 
        
        if(index || index ==0){
            alert('자세추가 후에는 꼭 저장버튼을 눌러주세요');
            let name = document.getElementById('s'+index);
            if(name.length>0 && typeof name != 'undefined' && typeof name != 'null'){
                let list_name = 's'+index;
                //중복 타겟
                let dupli = '자세추가';
                //중복 찾기 함수 
                let get_num = this.numbering(dupli,index,'자세추가');
                let result = `자세추가${get_num['number']}`;
                $(`#${list_name}`).append(`<option value="${result}">`+result+'</option>');
                this.group_position_storage[index][get_num['index']] = {'position_name': result,
                                                                    'selectors':[{x:20,y:20,w:40,h:40,name:0},{x:70,y:70,w:40,h:40,name:1},{x:150,y:70,w:40,h:40,name:2}],
                                                                    'memos':[{0:{x:20,y:20-1,memo:'영역1'}},{1:{x:70,y:70-1,memo:'영역2'}},{2:{x:150,y:70-1,memo:'영역3'}}],
                                                                }
            }
            this.selector_list_class.set_group_position_storage = this.group_position_storage;
        }else{
            alert('편집모드가 아닙니다.')
        }
       
    }

    del(index){
        if(index || index ==0){
            let name = document.getElementById('s'+index);
            if(confirm('해당 자세를 삭제하시겠습니까?')){  
                let list_name = 's'+index;
                if(document.getElementById(list_name).options.length ===1)
                {
                    alert('더이상 삭제 할 수 없습니다.')
                }else{
                    let keys = Object.keys(this.group_position_storage[index]);
                    if(keys){
                        for(let key_index=0; key_index<name.length; key_index++){
                            if(this.group_position_storage[index][keys[key_index]]['position_name'] == name.options[name.selectedIndex].value){
                                $(`#${list_name}`).find('option[value="'+this.group_position_storage[index][keys[key_index]]['position_name']+'"]').remove();
                                delete this.group_position_storage[index][keys[key_index]];
                                break;
                            }
                        }
                    }
                }
            }
            this.selector_list_class.set_group_position_storage = this.group_position_storage;
        }else{
            alert('편집모드가 아닙니다.')
        }
        
    }
    copy(index){
        if(index || index ==0){
            alert('자세복사 후에는 꼭 저장버튼을 눌러주세요');
            let name = document.getElementById('s'+index);
            if(name.length>0 && typeof name != 'undefined' && typeof name != 'null'){
                let keys = Object.keys(this.group_position_storage[index]);
                let list_name = 's'+index;
                //중복 타겟
                let dupli = name.options[name.selectedIndex].value;
                //중복 찾기 함수 
                let get_num = this.numbering(dupli,index,'copy');
                
                //복사작업
                for(let key_index=0; key_index<name.length; key_index++){
                    let object = this.group_position_storage[index][keys[key_index]];
                    //복사할 타겟찾기
                    if(object['position_name'] == name.options[name.selectedIndex].value){
                        let position_name = object['position_name'];
                        let basic_number = this.find_dupli(position_name,'copy') + get_num['number']
                        let copy_name = position_name+'copy'+basic_number;
                        
                        //복사한 것을 드롭다운 리스트에 추가
                        $(`#${list_name}`).append(`<option value="${copy_name}">`+copy_name+'</option>');
                        let copy = JSON.parse(JSON.stringify(object));//깊은복사 
                        copy['position_name'] = copy_name;
                        this.group_position_storage[index][get_num['index']] = copy;
                        //object = copy;
                        console.log(this.group_position_storage);
                        break;
                    }
                }
                this.selector_list_class.set_group_position_storage = this.group_position_storage;
            }else{
                alert('복사 대상이 존재하지 않습니다');
            }
        }else{
            alert('편집모드가 아닙니다.')
        }
    }

    rename(index,position_name){//개별자세 이름 수정 
        if(index || index ==0){
            let name = document.getElementById('s'+index);
            let keys = Object.keys(this.group_position_storage[index]);
            if(position_name){
                for(let key_index=0; key_index<name.length; key_index++){
                    if(this.group_position_storage[index][keys[key_index]]['position_name'] === position_name){
                        alert('자세이름이 중복입니다')
                        break;
                    }else{
                        if(this.group_position_storage[index][keys[key_index]]['position_name'] == name.options[name.selectedIndex].value){   
                            this.group_position_storage[index][keys[key_index]]['position_name'] = position_name;
                            name.options[name.selectedIndex].value = position_name;
                            name.options[name.selectedIndex].text = position_name;
                        }
                    }
                    
                }
                this.selector_list_class.set_group_position_storage = this.group_position_storage;
            }else{
                alert('자세 이름을 입력해주세요');
            }
        }else{
            alert('편집모드가 아닙니다.')
        }
        
    }

    numbering(dupli,index,string){
        let name = document.getElementById('s'+index);
        let keys = Object.keys(this.group_position_storage[index]);
        let add_index = name.length-1;

        //키중복 피해서 복사 하기 위한 작업 
        for(let key_index=0; key_index<name.length; key_index++){
            if(keys[key_index] == add_index){
                add_index++;
            }
        }

         //복사 넘버링 매기는 작업  
        let copynum=0;
        let find_string = ''
        if(string == 'copy'){
            find_string = `${dupli}copy`
        }else if(string=='자세추가'){
            find_string = `${dupli}`
        }
        
        for(let key_index=0; key_index<name.length; key_index++){
            let object = this.group_position_storage[index][keys[key_index]];
            if(object['position_name'].search(find_string)>-1){
                copynum++;
            }
        }
        //만약 copynum을 붙인 이름이 존재한다면 다시 copynum에 + 
        for(let key_index=0; key_index<name.length; key_index++){
            let object = this.group_position_storage[index][keys[key_index]];
            let basic_number = this.find_dupli(dupli,string) + copynum;
            if(object['position_name'] == find_string+basic_number){
                copynum++;
            } 
        }

        return {'index':add_index, 'number':copynum}
    }

    find_dupli(position_name,string){//중복 끝 숫자 찾기
        let result;
        let start_index = position_name.indexOf(string);
        let cnt =start_index>-1 ? 1:0;
        //중복을 찾기
        while(start_index !== -1){
            start_index = position_name.indexOf(string,start_index+1);
            cnt++;
        }   
        //추가하거나 복사한 자세들의 숫자를 가져온다 없으면 result=0;
        if(cnt>0){
            let name = position_name.indexOf(string,cnt);
            result = Number(position_name.slice(name+4,-1));
            if(result == 'undefined'){
                console.log(result);
            }
        }else{
            result =0;
        }
             
        return result;
    }
    
    
}