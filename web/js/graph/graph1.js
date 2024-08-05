///////그래프관련/////////
var urls = "/api/graph1/"
var refresh_data;
var refresh_heatmap;
var isreal = false;
var realtime_pool;
var number_heatmap = new neo_heatmap('number_neo_heatmap',2800,1200);
number_heatmap.set_pixel();
///////그래프관련/////////

// /////////셀렉터////////////// 
setting_property('init');
// /////////셀렉터////////////// 
function get_heatmap(){
    list.set_heatmap_class = heatmap;
    selector.set_heatmap_class = heatmap;
    area.set_heatmap_class = heatmap;
}

function neo_visualization(){
    //////////////기본 세팅/////////////////////
    document.getElementById('neo_heatmap_control').style.display = 'block'
    document.getElementById('number_neo_control').style.display = 'block';
    list = new pressure_selector_list(module_patient_id);
    list.id_validate('module_patient_id_dropdownlist');
    list.page_id = $('#page_state');
    list.load('#position_list','load');

    area.ismodify = false;
   
    get_heatmap();
    
    list.set_selector_class = selector;
    position.set_selector_class = selector;

    list.set_memo_class = memo;
    position.set_memo_class = memo;

    list.set_area_calss = area;
    
    position.set_selector_list_class = list;

    position.set_group_position_storage = list.group_position_storage;//데이터갱신
    //////////////기본 세팅/////////////////////

    /////////////자세 속성버튼////////////////////////
    list_modify_mode('#position_list',$("#neo_heatmap"),list);

    $('#position_add').off('click').on('click',function(){
        position.add(checked_index);
     });

     $('#position_del').off('click').on('click',function(){
        position.del(checked_index);
        let array = [checked_index];
        list.modify(array,'#position_list',$('#neo_heatmap_time'));
        list.draw(checked_index,pool,get_neo_index);
        
        setting_property('position');

        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        
     });

     $('#position_rename').off('click').on('click',function(){
        position.rename(checked_index,$('#naming_position').val());
        let array = [checked_index];
        list.modify(array,'#position_list',$('#neo_heatmap_time'));
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
     });

     $('#position_copy').off('click').on('click',function(){
        position.copy(checked_index); 
     });

     ///////////////자세 속성버튼////////////////////

    //////////////자세목록 버튼 툴바 /////////////
    $('#position_group_add').off('click').on('click',function(){
       
        list.set_time = position_time()[0]+" "+position_time()[1];
        list.set_cur_time = timeTF_Date(new Date())+' '+timeTF_Time(new Date());

        let len = list.group_position_storage.length

        list.add('#position_list',len,checked_index,$('#neo_heatmap_time'));
        
        position.set_group_position_storage = list.group_position_storage;//데이터갱신

        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        checked_index=null;
    });
   
    $('#position_next').off('click').on('click',function(){
        
        list.load('#position_list','next');
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        position.set_group_position_storage = list.group_position_storage;//데이터갱신
        checked_index=null;
        
    });
    $('#position_previous').off('click').on('click',function(){
        
        list.load('#position_list','previous');
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        position.set_group_position_storage = list.group_position_storage;//데이터갱신
        checked_index=null;
        
    });
    $('#position_group_del').off('click').on('click',function(){
       
        lists_mode(list,'delete');
        checked_index=null;
        
    });

    $('#position_group_modify').off('click').on('click',function(){
       
        lists_mode(list,'modify');
        checked_index=null;

    });

    $('#position_group_copy').off('click').on('click',function(){
        lists_mode(list,'copy');
        
        checked_index=null;
    });
    //////////////자세목록 버튼 툴바 /////////////

    //검색
    $('#search').off('click').on('click',function(){
        //$('#search_engine').val();
        list.search('#position_list',$('#search_engine').val());
        position.set_group_position_storage = list.group_position_storage;//데이터갱신
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        checked_index=null;
    });
    //검색
    $(document).off('change').on('change','#module_patient_id_dropdownlist',function(){
        var get_id = document.getElementById('module_patient_id_dropdownlist');
        module_patient_id = get_id.options[get_id.selectedIndex].text; 
        if(module_patient_id == '환자ID목록'){
            return;
        }else{
            neo_visualization();
        }
        checked_index=null;
        
    });
    //검색초기화
    $('#list_refresh').off('click').on('click',function(){
        //neo_visualization();
        list.load('#position_list','reload');
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        checked_index=null;
    })


    //정렬
    $('#cur_date_desc').off('click').on('click',function(){
        list.recreate('#position_list','desc');
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        checked_index=null;
    })
    $('#cur_date_asc').off('click').on('click',function(){
        list.recreate('#position_list','asc');
        list_modify_mode('#position_list',$("#neo_heatmap"),list);
        checked_index=null;
    })

    $('#position_graph').off('click').on('click',function(){
       position_graph();
    })

    // $('#part_graph').off('click').on('click',function(){
    //     part_graph();
    //  })


}

function visualization(){
    neo_visualization();
    if(Object.keys(pool).length>0){
        let len = pool[0]['base_data'].length;
        let paginate = new pagination(1,len,"#sequence_page");
        
        let line =new draw_2D(pool,120,52);
        let heatmap_ = new draw_heatmaps(pool,120,52);
        let threed = new draw_3D(pool,120,52);   

        neo_heatmap_ani(heatmap_.start,heatmap_.end);
        
        paginate.make_page();
        let cur_num = paginate.currentPage;

        $.ajax({
            type:'GET',
            url:'/api/graph1/',
            success: function(){
                line.line('line');
                line.part_bar('all_cell_bar',pool,'전체');
            }
        })
        $.ajax({
            type:'GET',
            url:'/api/graph1/',
            success: function(){
                heatmap_.heatmap_ani('heatmap_ani');
                heatmap_.heatmap_seq('sequence');
            }
        })
        $.ajax({
            type:'GET',
            url:'/api/graph1/',
            success: function(){
                threed.ani_3d('graph_3d_ani');
                threed.seq_3d('graph_3d_static');
            }
        })
        
        $("body").off('click').on('click','#sequence_page li button',function () {
            init_view('page');
           
            

            let values = paginate.setting_value();
            let $id = $(this).attr("id");
            let selectedPage = $(this).text();

            if ($id == "next") selectedPage = values[1];
            if ($id == "prev") selectedPage = values[2];

            cur_num = selectedPage;
            
            paginate.curr_page = selectedPage;
            heatmap_.curr_page=selectedPage;
            threed.curr_page = selectedPage;

            paginate.make_page();

            $.ajax({
                type:'GET',
                url:'/api/graph1/',
                success: function(){
                    heatmap_.heatmap_ani('heatmap_ani');
                    heatmap_.heatmap_seq('sequence');
                }
            })

            $.ajax({
                type:'GET',
                url:'/api/graph1/',
                success: function(){
                    threed.ani_3d('graph_3d_ani');
                    threed.seq_3d('graph_3d_static');
                }
            })

            
            neo_heatmap_ani(heatmap_.start,heatmap_.end);
           
            get_neo_index = heatmap_.start;

        })

        $('#3d_graph_style').off('change').on('change',function(){
                threed.graph_3d_style_ = $(this).val();
            
                threed.ani_3d('graph_3d_ani');
                threed.seq_3d('graph_3d_static');
        });

        $(window).on('resize',function(){ 
                threed.graph_3d_height_ = (window.innerHeight * 0.55).toString() + 'px';
                threed.graph_3d_width_ =(window.innerWidth *0.4).toString() + 'px';
                threed.ani_3d('graph_3d_ani');
        });

    }else{
       init_view('no_data');
    }
}

function init_view(state){
    //조회버튼과 페이지버튼으로 데이터 갱신하는 그래프 
    function submit_page_data(){
        //페이지를 넘기면 시간도 같이 바뀌는 그래프들
        Plotly.purge('heatmap_ani');
        Plotly.purge('sequence');
        Plotly.purge('real_heatmap');
       
        if(document.querySelector('div #graph_3d_ani').hasChildNodes()){
            document.getElementById('graph_3d_ani').replaceChildren();
            document.getElementById('graph_3d_static').replaceChildren();

            document.querySelectorAll('.page-item').forEach(el => el.remove());
        } 
        
        number_heatmap.clear();
        heatmap.clear();
        $('#number_time').text('');
        $('#neo_heatmap_time').text('');
        $('#neo_heatmap_slider').slider();
        $('#neo_heatmap_slider').slider('value',0);//자체히트맵 재생바 초기위치 코드
        $('#number_neo_heatmap_slider').slider();
        $('#number_neo_heatmap_slider').slider('value',0);
    }
    //조회버튼으로만 데이터를 갱신하는 그래프
    function submit_data(){
        //페이지를 넘겨도 시간은 그대로인 그래프 
        Plotly.purge('line');
        Plotly.purge('all_cell_bar');
        Plotly.purge('all_part_bar');
        Plotly.purge('position_sum_line');
        Plotly.purge('position_mean_line');
        if(document.querySelector('div #part_bar').hasChildNodes()){
            document.getElementById('part_bar').replaceChildren();
        }
    }
    
    if(state == 'submit' || state == 'no_data'){
        submit_data();
        submit_page_data();

    }else{
        submit_page_data();
    }
}

function realtime_error(){
    console.log('실시간 데이터 없음');
    alert('실시간으로 불러올 수 있는 데이터가 없습니다') 
    isreal = false;
    clearInterval(refresh_data);
    clearInterval(refresh_heatmap);
    document.getElementById('neo_heatmap_control').style.display = 'block'
    document.getElementById('number_neo_control').style.display = 'block';
    realtime_pool = new Array();
}

function realtime_graph(){
    let now_time = Date.parse(new Date());
    let data = base_data(timeTF_Date(now_time),timeTF_Time(now_time-1000*60),
                         timeTF_Date(now_time),timeTF_Time(now_time+1000*60));
    document.getElementById('neo_heatmap_control').style.display = 'none';
    document.getElementById('number_neo_control').style.display = 'none';
    $.ajax({
        type: 'POST',
        url: String(urls),
        data: data,
        dataType:'JSON',
        success: function(result) {
            if(Object.keys(pool).length>0){
                const line =new draw_2D(pool,120,52);
                
                
                console.log('실시간 데이터'+new Date());
                console.log(result);

                realtime_pool = buffer_filter(result);
                let cnt = realtime_pool[0]['base_time'].length;

                line.update_line('line',realtime_pool);

                heatmap.real_heatmap(realtime_pool);
                heatmap.time($('#neo_heatmap_time'),realtime_pool,cnt-1)

                number_heatmap.drawing_number(realtime_pool,cnt-1);
                number_heatmap.set_pool_data = realtime_pool;
                
                number_heatmap.time($('#number_time'),realtime_pool,cnt-1)
               
            }
            
        },
        error:function(){
            realtime_error();
            
        }
    })
 
}
function realtime_heatmap_graph(){
    let now_time = Date.parse(new Date());
    let data = base_data(timeTF_Date(now_time),timeTF_Time(now_time-1000*60),
                         timeTF_Date(now_time),timeTF_Time(now_time+1000*60));
    
    $.ajax({
        type: 'POST',
        url: String(urls),
        data: data,
        dataType:'JSON',
        success: function(result) {
            if(Object.keys(pool).length>0){
                const real_h =new draw_heatmaps(pool,120,52);
                
                let heatmap_pool = buffer_filter(result);
                real_h.real_heatmap('real_heatmap',heatmap_pool);
                
            }
        }
    })
}


$('#blur_apply').off('click').on('click',function(){
    check_blur = $('#blur_apply input:radio:checked').val();
    Plotly.restyle('heatmap_ani','zsmooth',check_blur);
    Plotly.restyle('sequence','zsmooth',check_blur);
});


$('#submit').off('click').on('click', function(e) { 
    //e.stopPropagation();

    /////초기화//////
    selector_view_init();
    init_view('submit');
    get_neo_index = 0;
    checked_index = null;
    /////초기화//////

    isreal = false;
    clearInterval(refresh_data);
    clearInterval(refresh_heatmap);
    
    
    let data = base_data($('#fromDate').val(),$('#fromTime').val(), $('#toDate').val(),$('#toTime').val());
    return_buffer(data,'/api/graph1/');
    
    visualization();
    //neo_visualization();
    //e.preventDefault();
    
});




document.getElementById('toDate').value = timeTF_Date(new Date());
document.getElementById('toTime').value = timeTF_Time(new Date());

