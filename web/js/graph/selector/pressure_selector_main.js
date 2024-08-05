var area_name;//셀렉터 영역과 메모를 연결하는 매개변수
var checked_index; //체크된 라디오 인덱스 가져오는 변수
var static=new Array();//부위 통계량 
var info;
/////////셀렉터////////////// 
var module_patient_id =  $('#serial_number').text();

var heatmap = new neo_heatmap('neo_heatmap',720,312);//셀렉터가 그려지는 곳 
var selector = new pressure_selector('neo_heatmap',720,312);
var area = new pressure_selector_area('neo_heatmap',720,312);
var memo = new pressure_selector_memo('neo_heatmap',720,312);

var position = new pressure_selector_position(module_patient_id);
var list = new pressure_selector_list(module_patient_id);

heatmap.set_pixel();
default_setting_list(module_patient_id,list);

/////////셀렉터//////////////

function setting_property(property){//마우스 이벤트시 바뀌는 속성창 태그 관리
    let positions,areas;
    
    (property === 'position') ? (setting_position(),positions='block') : positions='none';
    (property === 'area') ? (setting_area(),areas='block') : areas='none';
    
    document.querySelectorAll('#position').forEach(function(q){q.style.display = positions});
    document.querySelectorAll('#area').forEach(function(q){q.style.display = areas});
}

function setting_position(){ // 자세에 관한 속성 관리
    if(checked_index || checked_index == 0){
        let name = document.getElementById('s'+checked_index);
        $('#naming_position').val(name.options[name.selectedIndex].value);
    }
    
}

function setting_area(){ // 부위에 관한 속성 관리
    $('#selector_memo').val(memo.property_memo_text(area_name));
}

/////////////모드 세팅//////////////////////

function modify_mode(canvas){
    if(pool.length>0){
        setting_property('position');
    }
    
    canvas.mousedown(function (e) { 
        //let start = performance.now();
        if(!isplaying){
            area.picking(e,pool,get_neo_index);
        }
        
        area.ismodify ? modify_mode_mouse_down(e,pool): mouse_down_draw(e);
        
        memo.redraw();
        
        //let end = performance.now() - start;
        //let consoles = {
        //log:`info ,${timeTF_Date(new Date())} ${timeTF_Time(new Date())},${end.toFixed(3)}, pressure_selector_main.js, class , function modify_mode, line 63, 마우스다운 기능`
        //}//log
        //js_logger(consoles);//log
    });
    canvas.mousemove(function (e) { 
        //let start = performance.now();
        
        area.ismodify ? modify_mode_mouse_move(e,pool) : mouse_move_draw(e,pool);
        
        // let end = performance.now() - start;
        // let consoles = {
        // log:`info ,${timeTF_Date(new Date())} ${timeTF_Time(new Date())},${end.toFixed(3)}, pressure_selector_main.js, class , function modify_mode, line 47, 마우스무브 기능`
        // }//log
        // js_logger(consoles);//log
        });
        canvas.mouseup(function (e) { 
        //let start = performance.now();
        
        area.ismodify ? modify_mode_mouse_up() : mouse_up_draw(e);

        // let end = performance.now() - start;
        // let consoles = {
        // log:`info ,${timeTF_Date(new Date())} ${timeTF_Time(new Date())},${end.toFixed(3)}, pressure_selector_main.js, class , function modify_mode, line 85, 마우스업 기능`
        // }//log
        // js_logger(consoles);//log
        });
}
///////modify모드가 아닐때///////////////
function mouse_up_draw(){
    if(checked_index || checked_index == 0){
        selector.save();
        // if(checked_index>=0){
        //     info = list.get_part_data(checked_index);
        //     parts_info(info,$('#heatmap_info'));
        // }
        update_static(checked_index,get_neo_index);
    }
    
}

function mouse_move_draw(e,data){
    if(checked_index || checked_index == 0){
        selector.draw_move(e,data,get_neo_index);
    }
}

function mouse_down_draw(e){
    if(checked_index || checked_index == 0){
        setting_property('position');
        selector.draw(e);
    }
}
///////modify모드가 아닐때///////////////

///////modify모드 일때///////////////

function modify_mode_mouse_down(event,data){
    setting_property('area');
    area.select(event,data,get_neo_index);
    area_name = area.collision.length>0 ? area.collision[0]['name'] : false;
    //memo save가 안되는건 area collision이 name을 제대로 넘기지 못해서 생긴다.
    
    $('#selector_memo').on('keyup',function(e){
        e.stopPropagation();
        memo.draw($(this).val()); 
        area_name===false ? alert('메모할 영역을 선택하세요') : memo.save(area_name);
    });

    memo.redraw();

    $('body').on('keydown',function(e){
        e.stopPropagation();
        let key = e.key || e.keyCode;
        if((key === 'Delete' ||key === 46) && area.ismodify == true){
            let pick_delete = area.delete();

            selector.load(pick_delete);
            selector.redraw(pool,get_neo_index);
            memo.delete(pick_delete);
        }
    })
    area.modify_mousedown(event,data,get_neo_index);
}

function modify_mode_mouse_move(e,data){
    //let start = performance.now();

    setting_property('area');

    memo.set_memo_x = area.collision.length>0 ? area.collision[0]['x'] : false;
    memo.set_memo_y = area.collision.length>0 ? area.collision[0]['y'] : false;
    
    area.modify_mousemove(e,data,get_neo_index);
    
    selector.load(area.modify_save());
    
    memo.modify(area.modify_save());
    // let end = performance.now() - start;
    // let consoles = {
    // log:`info ,${timeTF_Date(new Date())} ${timeTF_Time(new Date())},${end.toFixed(2)}, pressure_selector_main.js, class , function modify_mode_mouse_move, line 153, 마우스 무브`
    // }//log
    // js_logger(consoles);//log
}

function modify_mode_mouse_up(){
    setting_property('area')
    area.modify_mouseup();
    memo.modify(area.modify_save());
    memo.redraw();
    ///mouse_down에 있던 코드 //시간이 많이 걸리는 곳 
    update_static(checked_index,get_neo_index);
}
///////modify모드 일때///////////////
function position_graph(){ 
    if(checked_index>=0 && checked_index !== 'null'){
        //자세 그래프
        let part_graph = new draw_2D(pool,120,52);
        let name = document.getElementById('s'+checked_index);
        let name_val = name.options[name.selectedIndex].value;
        //info = list.get_part_data(checked_index);
        update_static(checked_index,get_neo_index);
        //mean을 알려면 셀렉터의 wh 를 알아야 함
        part_graph.all_sum_line('position_sum_line',info,name_val);
        part_graph.all_mean_line('position_mean_line',info,name_val);
    }
    part_graph();
}

function part_graph(){
    
        if(checked_index>=0 && checked_index !== 'null'){
            $('#part_bar').empty();
            let part_graph = new draw_2D(pool,120,52);
            let len = memo.memo_scope.length;
            for(let index=0; index<len; index++){
                $('#part_bar').append(`<div id='part_bar${index}'></div>`);
            }
            let real_index =0;
            for(let index=0; index<len; index++){
                if(memo.memo_scope[index]){
                    let title = memo.memo_scope[index][index].memo;
                    part_graph.multi_bar(`part_bar${index}`,info,title,'parts',real_index);
                    real_index++;
                }
            }
            
            part_graph.part_bar('all_part_bar',info,'부위 통합');

        }
}
/////////////모드 세팅//////////////////////
function collect_static(){//부위 통계량 계산
    let sum = new Array();
    let groupby = new Array();
    for(let index=0; index<static.length; index++){
        sum.push(static[index]['sum']);
        groupby.push(static[index]['groupby']);
    }
    
    return {'sum':sum, 'groupby':groupby};
}

function parts_info(data,label){//속성창에 부위 정보 표시 
    label.empty();
    for(let index=0; index<data.length; index++){
        label.append(`부위 이름 : ${data[index].title}<br>최소 / 최대 : ${data[index]['all_static'][get_neo_index].min} / ${data[index]['all_static'][get_neo_index].max}<br> 평균 / 합 : ${data[index]['all_static'][get_neo_index].mean} / ${data[index]['all_static'][get_neo_index].sum}<br><br>`)
    } 
}

function radio_check(list,index,canvas){ 
    
    if(pool.length>0){
        list.draw(index,pool,get_neo_index);
        checked_index = index;
        update_static(index,get_neo_index);
    }

    modify_mode(canvas);
}
function selector_view_init(){
    //화면 초기화
    area.set_selector_rects = new Array();
    area.set_collision = new Array();
    area.set_rect = {
        startX:undefined,
        startY:undefined,
        w:undefined,
        h:undefined,
        modify_name:undefined
    };
    memo.clear();
    if(!isplaying){//자체히트맵 영상 재생 안 할 때 실행하라
        //neo_heatmap 초기화면 설정
        heatmap.drawing(pool,get_neo_index);
        setting_property('init');
    }
    
}
function list_modify_mode(table,canvas,list){
    
    if(!$('input[name="radios"]:checked').val()){//편집모드가 아닐때만 셀렉터를 초기화 해라
        selector_view_init();
    }
    
    ////////////////////////////////////////
    let cnt = list.group_position_storage.length;
    
    for(let index = 0; index<cnt+1; index++){
        let list_name ='s'+index;
        let radio = $(`${table} #modify_mode${index}`);
        $(`#${list_name}`).off('change').on('change',function(){
            if(radio.prop('checked')){
                radio_check(list,index,canvas);
            }
            
        });
    
        radio.off('change').on('change',function(){     
            if(radio.prop('checked')){
                radio_check(list,index,canvas);
            }
        });
        
    }
    
}
/////////////모드 세팅//////////////////////

//////리스트 저장 삭제 수정/////////////////
function lists_mode(list,list_mode) {
    let cnt = list.group_position_storage.length;
    let list_contain = new Array();
    for (let index = 0; index < cnt; index++) {
        let checkBoxes = document.getElementById(`${index}`);
        if(checkBoxes.checked){list_contain.push(index)}
    }
    if(list_contain.length===0){
        alert('선택하려는 행의 체크박스에 체크해주세요');
    }else{
        switch(list_mode){
            case 'save':
                list.save(list_contain,'#position_list');
                break;
            case 'delete':
                if(confirm('삭제하시겠습니까?')){
                    list.delete(list_contain,'#position_list',checked_index,$('#neo_heatmap_time'));
                    break;
                }else{
                    break;
                }
            case 'modify':
                list.modify(list_contain,'#position_list',$('#neo_heatmap_time'));
                if(list.issave){
                    alert('저장을 완료했습니다')
                }else{
                    alert('저장을 실패했습니다')
                }
                break;
            case 'copy':
                list.copy(list_contain,'#position_list',checked_index,$('#neo_heatmap_time'));
                break;
        }
    }
    
    position.set_group_position_storage = list.group_position_storage;
    list_modify_mode('#position_list',$("#neo_heatmap"),list);
    
}
//////리스트 저장 삭제 수정/////////////////

function all_checkbox(selectAll){
    const checkboxes = document.getElementsByName('position_checkbox');
    checkboxes.forEach((checkbox) => {
        checkbox.checked = selectAll.checked;
    });
}

function update_static(index,neo_index){
    if(Number(checked_index)>=0 && checked_index != null){
        if(area.collision.length>0){
            
            heatmap.set_heatmap_info = heatmap.get_part_data(   
                                                                area.collision[0]['x'], 
                                                                area.collision[0]['y'],
                                                                area.collision[0]['w'], 
                                                                area.collision[0]['h'] 
                                                            );
        
            static = heatmap.statics();
    
            $('#static_min').text(static[neo_index]['min']);
            $('#static_max').text(static[neo_index]['max']);
            $('#static_sum').text(static[neo_index]['sum']);
            $('#static_mean').text(static[neo_index]['mean']);
        }
    
        info = list.get_part_data(index);
        parts_info(info,$('#heatmap_info'));
    }

}
