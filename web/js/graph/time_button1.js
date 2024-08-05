//시간 입력
function push_time(from_date,from_time,to_date,to_time){
    clearInterval(refresh_data);
    let data = base_data(from_date,from_time,to_date,to_time);
    return_buffer(data,'/api/graph1/');
    document.getElementById('fromDate').value = from_date;
    document.getElementById('fromTime').value = from_time;
    document.getElementById('toDate').value = to_date;
    document.getElementById('toTime').value = to_time;

    visualization();
}
//시간 프리셋 눌렀을때
function time_preset(number, check_time_preset){
    let now_time = Date.parse(new Date());
    isreal = false;
    clearInterval(refresh_data);
    clearInterval(refresh_heatmap);
    if(typeof check_time_preset === 'string'){
        push_time(timeTF_Date(now_time-number),
                  timeTF_Time(now_time-number),    
                  timeTF_Date(now_time),
                  timeTF_Time(now_time)
                );
    }
}

//실시간
function realtime_(){
    isreal = true;
    clearInterval(refresh_data);
    clearInterval(refresh_heatmap);
   
    refresh_data=setInterval(realtime_graph,1000*3);
    refresh_heatmap = setInterval(realtime_heatmap_graph,1000);
}

//시간 계산 
const minus_1=1000*60*60;
const minus_2=1000*60*60*2;
const minus_3=1000*60*60*3;
const minus_7 = 7*1000*60*60*24;
const minus_30 = 30*1000*60*60*24;
const minus_day =1000*60*60*24;

let now_time = Date.parse(new Date());

//프리셋 버튼 클릭 이벤트 관리 
$(document).off('click').on('click','#time_buttons', function(e){
    let check_time_preset = e.target.getAttribute('value');
    let calclulate_num;

    switch(check_time_preset){
        case 'one_hour':
            calclulate_num = minus_1;
            time_preset(calclulate_num,check_time_preset);
            break;
        case 'two_hour':
            calclulate_num = minus_2;
            time_preset(calclulate_num,check_time_preset);
            break;
        case 'three_hour':
            calclulate_num = minus_3;
            time_preset(calclulate_num,check_time_preset);
            break;
        case 'one_month':
            calclulate_num = minus_30;
            time_preset(calclulate_num,check_time_preset);
            break;
        case 'one_week':
            calclulate_num = minus_7;
            time_preset(calclulate_num,check_time_preset);
            break;
        case 'one_day':
            calclulate_num = minus_day;
            time_preset(calclulate_num,check_time_preset);
            break;
        case 'realtime':
            if(document.getElementById('fromDate').value){
                realtime_();
                visualization();
            }
            else{
                push_time(timeTF_Date(now_time-minus_1),
                          timeTF_Time(now_time-minus_1),    
                          timeTF_Date(now_time),
                          timeTF_Time(now_time)
                        );
                realtime_();
            }
            break;
        case null:
            alert('잠시만 기다려주세요');
            break;
    }
})

// 기기 id 선택시 정보 가져오기
$('#deivceSelect').off('change').on('change', function() {  
    let data = {
        'key':$('#deivceSelect').val(),
        'csrfmiddlewaretoken': csrftoken,
    }
    
    //기기 선택시 바로 실시간 데이터 받아오는 것 실행
    $.ajax({
        type: 'GET',
        url: '/api/graph1',
        data: data,
        async:false,
        success: function(data) {
            $('#module_alias').text(data['module_alias'])
            $('#device_ip').text(data['board_ip'])
            $('#serial_number').text(data['board_serial_number'])
            module_patient_id = data['board_serial_number'];
            console.log('기기 정보 받아오기 성공'+new Date())
            init_view('no_data');
           
            push_time(timeTF_Date(now_time-minus_1),
                      timeTF_Time(now_time-minus_1),    
                      timeTF_Date(now_time),
                      timeTF_Time(now_time)
                      );
            realtime_();
        },
        error: function(xhr, errmsg, err) {
            console.log(xhr.status + ": " + xhr.responseText); 
            alert('기기정보 데이터가 없습니다');
            pool = new Array();
        }
    }); 
    
  });