class pressure_selector_list{
    constructor(id){
        this.id = id;
        this.time = '';
        this.cur_time = '';
        this.cnt = 0;

        this.prev;
        this.next;
        this.cur;
        this.total_page;
        this.total_data_cnt;
        this.page_id;

        this.group_position_storage = new Array();
        this.g_p = {};

        this.selector_class;
        this.memo_class;
        this.position_class;
        this.heatmap_class;
        this.area_calss;

        this.issuccess = false;
        this.ispatient = false;
        this.issave = false;

        this.rect_size = 6;

    }

    get set_time(){return this.time;}
    set set_time(value){return this.time=value;}

    get set_cur_time(){return this.cur_time;}
    set set_cur_time(value){return this.cur_time=value;}

    get set_group_position_storage(){return this.group_position_storage;}
    set set_group_position_storage(value){return this.group_position_storage=value;}

    get set_selector_class(){return this.selector_class;}
    set set_selector_class(value){return this.selector_class=value;}

    get set_memo_class(){return this.memo_class;}
    set set_memo_class(value){return this.memo_class=value;}

    get set_position_class(){return this.position_class;}
    set set_position_class(value){return this.position_class=value;}

    get set_heatmap_class(){return this.heatmap_class;}
    set set_heatmap_class(value){return this.heatmap_class=value;}

    get set_area_calss(){return this.area_calss;}
    set set_area_calss(value){return this.area_calss=value;}
   

    //리스트 그룹자세 추가
    add(table,cnt,index,container){
        let group_position = {};
        //let cnt = this.group_position_storage.length
        let list_name ='s'+cnt;
        
        //환자 ID가 있던 없던 테이블에 무조건 하나는 생성해놔라 
        $(table).append(`<tr data-index="${cnt}"><td class="bs-checkbox " style="width: 36px; "><input name="position_checkbox" id="${cnt}" type="checkbox"></td><td>${this.id}</td><td>${this.time}</td><td><select id="${list_name}" class="btn-outline-dark btn-sm" data-style="btn-inverse"><option value="자세1" selected="selected">자세1</option></select></td><td>${this.cur_time}</td><td><input type="radio" name="radios" id="modify_mode${cnt}"></td></tr>`)
        
        let ori = document.getElementById(list_name);
        $(`#${list_name}`).after(`   /  ${ori.length}`);
        
        //group_position['name'] = '그룹'+cnt;
        group_position['from'] = this.time;
        group_position['to'] = this.cur_time;
        for(let i =0; i<ori.length; i++){
            group_position[`${i}`] = {
                            'position_name':`${ori.options[i].text}`,
                            'selectors':[{x:20,y:20,w:40,h:40,name:0},{x:70,y:70,w:40,h:40,name:1},{x:150,y:70,w:40,h:40,name:2}],
                            'memos':[{0:{x:20,y:20-1,memo:'영역1'}},{1:{x:70,y:70-1,memo:'영역2'}},{2:{x:150,y:70-1,memo:'영역3'}}],
                          }
        }
        this.group_position_storage.push(group_position);
        let new_data =[];
        new_data.push(cnt);
        //해당 아이디가 있나없나 검증부터 하고 저장된 셀렉터가 있는 아이디 일때만 저장해라 
        if(this.issuccess || this.ispatient){ 
            
            this.save(new_data,table);

            if(index || index ==0){
                let array = [index];
                this.modify(array,table,container);
            }
        }
    }


    id_validate(list){
        let all_items = document.querySelectorAll(`[name=${list}]`);
        let ids = [...all_items[0].options].map(item=>item.value);
        if(ids.indexOf(`${this.id}`)>-1){
            this.ispatient = true;
        }else{
            this.ispatient = false;
        }


    }

    save(data,table){
        let save_dict = {};
        let issave = false;
        for(let index=0; index<data.length; index++){
            this.group_position_storage[data[index]]['to'] = timeTF_Date(new Date())+' '+timeTF_Time(new Date());
            save_dict = {"data": this.group_position_storage[data[index]]};
            $.ajax({
                url:'/api/selectors/',
                method:"POST",
                contentType:"application/json; charset=utf-8;",
                data: JSON.stringify({
                    "module_patient": this.id,
                    "content": [save_dict]
                }),
                async:false,
                success:function(){
                    issave = true;
                },
                error:function(e){
                    issave = false;
                  }
               })
        }
        this.issave = issave;

        this.load(table,'reload');  
    }

    delete(data,table,index,container){
        let isdel = true;
        if(index || index ==0){
            let array = [index];
            this.modify(array,table,container);
        }

        for(let index=0; index<data.length; index++){
            let del = this.group_position_storage[data[index]]['id'];
            $.ajax({
                url:`/api/selectors/${del}`,
                method:"DELETE",
                async:false,
                success:function(){
                    isdel = true;
                },
                error:function(){
                    isdel = false;
                  }
               })
               if(!isdel) break;
        }
        if(isdel){
            alert('삭제를 완료했습니다.')
        }else{
            alert('삭제를 실패했습니다.')
        }
        this.load(table,'reload');
    }

    modify(data,table,container){
        let modify_dict = {};
        let issave = false;
        console.log(this.group_position_storage);
        for(let index=0; index<data.length; index++){
            this.group_position_storage[data[index]]['to'] = timeTF_Date(new Date())+' '+timeTF_Time(new Date());
            this.group_position_storage[data[index]]['from'] = (container.text()).slice(5);
            modify_dict = {"data": this.group_position_storage[data[index]]};
            let modify_id = this.group_position_storage[data[index]]['id'];
            $.ajax({
                url:`/api/selectors/${modify_id}`,
                method:"PATCH",
                contentType:"application/json; charset=utf-8;",
                data: JSON.stringify({
                    "content": [modify_dict]
                }),
                async:false,
                success:function(){
                    issave =true;
                },
                error:function(){
                    issave = false;
                  }
               })
        }
        this.issave = issave;
        this.load(table,'reload');
    }


    copy(data,table,index,container){
        let new_data = [];
        if(data.length>0){
            if(index || index ==0){
                let array = [index];
                this.modify(array,table,container);
            }
            for(let index=0; index<data.length; index++){
            
                let copy = JSON.parse(JSON.stringify(this.group_position_storage[data[index]]));
                copy['name'] = this.group_position_storage[data[index]]['name']+'copy';
                copy['to'] = timeTF_Date(new Date())+' '+timeTF_Time(new Date());
                delete copy['id'];
                let table_index = this.group_position_storage.length;
                this.group_position_storage[table_index] = copy;
                new_data.push(table_index);
                //여기서 인덱스는 
                this.table_append(table,copy,table_index);
                
            }
            this.save(new_data,table);
        }else{
            alert('복사대상이 없습니다')
        }
        
        
    }

    data_classify(contain_data){
        
        for(let index=0; index<contain_data['results'].length; index++){
            if(contain_data['results'][index]['content'].length>0 && typeof contain_data['results'][index]['content'][0]['data'] != 'undefined'){
                this.group_position_storage.push(contain_data['results'][index]['content'][0]['data']);//제대로된 데이터가 들어왔을때
                this.group_position_storage[index]['id'] = contain_data['results'][index]['id'];
            }else{
                this.group_position_storage.push(contain_data['results'][index]['content']);//잘못된 데이터일때
                this.group_position_storage[index] = {id:contain_data['results'][index]['id']};
            }
        }
       
    }

    sql_classify(contain_data){
        return contain_data ? true : false;
    }

    table_append(table,contain_data,index){
        let time=contain_data['from'];
        let cur_time=contain_data['to'];
        let list_name ='s'+index;
        $(table).append(`<tr data-index="${index}"><td class="bs-checkbox " style="width: 36px; "><input name="position_checkbox" id="${index}" type="checkbox"></td><td>${this.id}</td><td>${time}</td><td><select id="${list_name}" class="btn-outline-dark btn-sm" data-style="btn-inverse"></select></td><td>${cur_time}</td><td><input type="radio" name="radios" id="modify_mode${index}"></td></tr>`)
        
        let dropdown_array = Object.keys(contain_data);
        for(let dropdownlist_index=0; dropdownlist_index<dropdown_array.length; dropdownlist_index++){
            if(dropdown_array[dropdownlist_index] != 'from' && dropdown_array[dropdownlist_index] != 'to' && dropdown_array[dropdownlist_index] != 'name' && dropdown_array[dropdownlist_index] != 'id'){
                let dropdown_list_position_name = contain_data[dropdown_array[dropdownlist_index]]['position_name']
                
                $(`#${list_name}`).append(`<option value="${dropdown_list_position_name}" selected="selected">${dropdown_list_position_name}</option>`);
            }
        }
        
        let ori = document.getElementById(list_name);
        
        $(`#${list_name}`).after(`   /  ${ori.length}`);
    }

    create(table,contain_data){
        $(`${table} > tbody:last`).empty();
        this.group_position_storage = new Array();
        switch(this.sql_classify(contain_data)){
            case true://sql에 데이터가 있을때 
                this.prev = contain_data['previous'];
                this.next = contain_data['next'];
                this.cur = contain_data['current'];
                this.total_data_cnt = contain_data['count'];
                this.total_page = contain_data['total'];
                if(contain_data['count']>0){
                    this.data_classify(contain_data);//sql에 저장된 데이터 분류 하여 로컬에 저장하기 
                    for(let index=0; index<this.group_position_storage.length; index++){
                        if(typeof this.group_position_storage[index] != "undefined"){
                            this.table_append(table,this.group_position_storage[index],index)
                        }
                        else{
                            $(table).append(`<tr data-index="${index}"><td class="bs-checkbox " style="width: 36px; "><input name="position_checkbox" id="${index}" type="checkbox"></td><td>${this.id}</td><td>undefined</td><td><select id="${'s'+index}" class="btn-outline-dark btn-sm" data-style="btn-inverse"><option value="자세1" selected="selected">자세1</option></select></td><td>undefined</td><td><input type="radio" name="radios" id="modify_mode${index}"></td></tr>`)
                        }
                    }
                    this.page(this.page_id);
                }else{
                    this.add(table,0);
                }

                break;
            
            case false://sql에 데이터가 없을때
                //여러페이지중 마지막페이지에서 데이터가 1개밖에 없을 경우 
                if(this.total_page>1 && this.issuccess && this.group_position_storage.length>0){
                    this.load(table,'load');
                }else{
                    this.add(table,0);
                }
                
                break;
        }
        
    }


    load(table,state){
        let contain_data= null;
        let message = '';
        switch(state){
            case 'load':
                state = `/api/selectors/?module_patient=${this.id}`;
                break;
            case 'reload':
                state = `/api/selectors/?module_patient=${this.id}&page=${this.cur}`;
                break;
            case 'previous':
                state = this.prev;
                message = '첫 페이지 입니다.'
                break;
            case 'next':
                state = this.next;
                message = '마지막 페이지 입니다.'
                break;
        }
        let check = this.issuccess;
        if(typeof state === 'string'){
            //console.log('현재 환자 아이디 : %s',this.id);
            $.ajax({
                        url: state,
                        method:"GET",
                        async:false,
                        success:function(data){

                            contain_data = data;
                            check=true;
                            console.log(data);
                        },
                        error:function(){
                            alert('현재 저장된 셀렉터가 존재하지 않습니다');
                            check = false;
                          }
                       })
            this.create(table,contain_data);
            
        }
        else{
           alert(message);
        }
    }

    //리스트 개별자세 그리기 
    draw(index,data,cur_index){
        if((index && data) || index == 0){
            let name = document.getElementById('s'+index);
            let keys = Object.keys(this.group_position_storage[index]);
            let search_data;
            for(let key_index=0; key_index<name.length; key_index++){
                if(this.group_position_storage[index][keys[key_index]]['position_name'] == name.options[name.selectedIndex].value){
                    search_data = this.group_position_storage[index][keys[key_index]];
                    //console.log(search_data);
                    this.selector_class.load(search_data);
                    this.memo_class.load(search_data);
    
                    this.selector_class.redraw(data,cur_index);
                    this.memo_class.redraw();
    
                    let index_num = search_data['selectors'] ? search_data['selectors'].length : 0;
    
                    this.selector_class.set_selector_index =index_num;
                }
            }
            //console.log(this.group_position_storage);
            this.area_calss.ismodify = false;
            this.area_calss.set_selector_rects = this.selector_class.selector_rects;
        }
    }



    page(page){
        if(page){
            page.text(this.cur+'/'+this.total_page);
        }
    }

    descending(prev,next){
        ///날짜를 꺼내와서 비교후
        ///바꾸기
        let date1 = new Date(prev['to']).getTime();
        let date2 = new Date(next['to']).getTime();
        return date1 < date2 ? 1 : -1;
    }

    ascending(prev,next){
        ///날짜를 꺼내와서 비교후
        ///바꾸기
        let date1 = new Date(prev['to']).getTime();
        let date2 = new Date(next['to']).getTime();
        return date1 > date2 ? 1 : -1;
    }

    recreate(table,state){
        $(`${table} > tbody:last`).empty();
        if(state == 'desc'){
            this.set_group_position_storage = this.group_position_storage.sort(this.descending);
        }else if(state == 'asc'){
            this.set_group_position_storage = this.group_position_storage.sort(this.ascending);
        }
        
        for(let index=0; index<this.group_position_storage.length; index++){
            if(typeof this.group_position_storage[index] != "undefined"){
                this.table_append(table,this.group_position_storage[index],index)
            }
            else{
                $(table).append(`<tr data-index="${index}"><td class="bs-checkbox " style="width: 36px; "><input name="position_checkbox" id="${index}" type="checkbox"></td><td>${this.id}</td><td>undefined</td><td><select id="${'s'+index}" class="btn-outline-dark btn-sm" data-style="btn-inverse"><option value="자세1" selected="selected">자세1</option></select></td><td>undefined</td><td><input type="radio" name="radios" id="modify_mode${index}"></td></tr>`)
            }
        }
        this.page(this.page_id);
    }

    search(table,words){
        let find = new Array();
        for(let index =0; index<this.group_position_storage.length; index++){
            let name = document.getElementById('s'+index);
            let keys = Object.keys(this.group_position_storage[index]);
            for(let key_index=0; key_index<name.length; key_index++){
                let object = this.group_position_storage[index][keys[key_index]];
    
                if(object['position_name'].indexOf(words)!=-1){
                    find.push(index);
                }
            }
        }
        if(find.length===0){
            alert('검색결과가 없습니다');
        }
        //배열 중복 제거
        let finds = new Set(find);
        let unique_find = [...finds];
        let lists = new Array();

        for(let index=0; index<unique_find.length; index++){
            lists.push(this.group_position_storage[unique_find[index]]);
        }
        
        this.set_group_position_storage = lists;
        this.recreate(table,'search');
        
    }

    get_part_data(index){
        let name = document.getElementById('s'+index);
        let keys = Object.keys(this.group_position_storage[index]);
        let search_data;
        let part_data = new Array();
        let text = '';
        let top, left, width, height;
        if(Object.keys(this.heatmap_class.pool_data).length>0){
            for(let key_index=0; key_index<name.length; key_index++){
                if(this.group_position_storage[index][keys[key_index]]['position_name'] == name.options[name.selectedIndex].value ){
                    search_data = this.group_position_storage[index][keys[key_index]];
                   
                    let selector = search_data['selectors']
                    let memo = search_data['memos']
                    
                    for(let index = 0; index<selector.length; index++){
                        if(selector[index] && typeof selector[index].x != 'undefined'){
                            if(!isNaN(selector[index].x)){
                                for(let memo_index=0; memo_index<memo.length; memo_index++){
                                    if(memo[memo_index]){
                                        if(selector[index].name == Object.keys(memo[memo_index])[0]){
                                            text = memo[memo_index][selector[index].name].memo;
                                            break;
                                        }else{
                                            text = '';
                                        }
                                    }
                                }
                              
                               
                                top = selector[index].x;
                                left = selector[index].y;
                                width = selector[index].w;
                                height = selector[index].h;

                                this.heatmap_class.set_heatmap_info = 
                                    this.heatmap_class.get_part_data(top,left,width,height);
                                
                                part_data.push({
                                                'title':text,
                                                'static':this.heatmap_class.statics()[0],
                                                'cell_info':this.heatmap_class.get_part_data(top,left,width,height),
                                                'all_static':this.heatmap_class.statics(),
                                            });
                            }
                        }
                    }     
                }
            }
        }else{
            alert('압력데이터가 없습니다. 데이터를 조회해 주세요.')
        }
        
        return part_data;
    }

}