//전역변수
var pool = new Array();

function unupper(str){
  str = String(str);
  return str.replace(/\"/gi,'');
}

function js_logger(log){
  $.ajax({
      url:'/logs/',
      method:"POST",
      contentType:"application/json; charset=utf-8;",
      data: JSON.stringify(log),
      error:function(){
          console.log('로그 저장 못함');
        }
     })
}  

Array.matrix = function (h, w, data) {
  var i, j = [];
  var a = new Array(w);
  for(let num = 0; num<a.length; num++){
    a[num]=new Array(h);
  }
  var k = 0;
 
  for (i = h-1; i>=0; i -= 1) {
    for (j = 0; j < w; j += 1) {
        a[i][j] = data[k];
        k++;
    }
  }
  return a;
}
Array.new_matrix = function (h, w, data) {
  var a = new Array(w);
  for(let num = 0; num<a.length; num++){
    a[num]=new Array(h);
  }
  var k = 0;
 
  for (let i = 0; i<w; i += 1) {
    for (let j = 0; j < h; j += 1) {
        a[i][j] = data[k];
        k++;
    }
  }
  return a;
}
function position_time(){
  let from_date,from_time,to_date,to_time,position_name;
  from_date = document.getElementById('fromDate').value;
  from_time = document.getElementById('fromTime').value;
  to_date = document.getElementById('toDate').value;
  to_time = document.getElementById('toTime').value;
  position_name = document.getElementById('naming_position').value;
  return[from_date,from_time,to_date,to_time,position_name]
}

function timeTF_Date(dt){
  var d = new Date(dt);
  var yyyy = d.getFullYear();
  var MM = d.getMonth()+1;
  var dd = d.getDate();
  return (yyyy+'-'+addzero(MM)+'-'+addzero(dd))
}

function timeTF_Time(dt){
  var d = new Date(dt);
  var hh = d.getHours();
  var mm = d.getMinutes();
  return (addzero(hh)+':'+addzero(mm))
}

function addzero(n){
  return n<10 ? "0"+n:n;
}

function range(size, start) {
  return Array(size).fill(start).map((x, y) => x + y)
}
function minus_range(size, start) {
  return Array(size).fill(start).map((x, y) => x - y)
}
function default_setting_list(id,list){
  list = new pressure_selector_list(id);
  list.page_id = $('#page_state');
  list.id_validate('module_patient_id_dropdownlist');
  list.load('#position_list','load');
}

// from-to를 받기 위한 큐
class Queue {
  constructor() {
    this.storage = {};
    this.front = 0;
    this.rear = 0;
  }
  size() {
    if (this.storage[this.rear] === undefined) {
      return 0;
    } else {
      return this.rear - this.front + 1
    }
  }
  
  // 처음 값이 들어올 때를 제외하고는, rear가 1씩 증가하도록 구현했습니다.
  add(element) {
    if (this.size() === 0) {
      this.storage['0'] = element;
    } else {
      this.rear += 1;
      this.storage[this.rear] = element;
    }
  }
  popleft() {
    let temp;
    if (this.front === this.rear) 
    {
      temp = this.storage[this.front];
      delete this.storage[this.front];
      this.front = 0;
      this.rear = 0;
      return temp;
    } 
    else 
    {
      let dump;
      if(this.storage[this.front]>this.storage[this.rear])
      {
        dump = this.storage[this.front];
        this.storage[this.front] = this.storage[this.rear];
        this.storage[this.rear] = dump;

        temp = this.storage[this.front];
        delete this.storage[this.front];
        this.front +=1;
      }
      else
      {
        temp = this.storage[this.front];
        delete this.storage[this.front];
        this.front += 1;
      }
      return temp;
    }
  }
}

class graph_property{
  constructor(pool,w,h){
      this.page = 1;
      this.pool_cnt = pool[0]['base_data'].length;
      this.view_data = 20;
      
      this.start = this.view_data*(this.page-1);
      this.end = this.view_data>this.pool_cnt-this.start ? this.pool_cnt : this.view_data*this.page;
      this.end_cnt = this.view_data>this.pool_cnt-this.start_ ? this.pool_cnt-this.start_ : this.view_data;
      this.zaxis_max = 1040;

      this.pool = pool;
      this.w = w;
      this.h = h;
      this.wh = w*h;
  }
  get curr_page(){return this.page;}
  set curr_page(value){this.page = value;}
  
  get start_(){return this.start=this.view_data*(this.page-1);}
  set start_(value){this.start = this.view_data*(value-1);}
  
  get end_(){return this.end=this.view_data>this.pool_cnt-this.start ? this.pool_cnt : this.view_data*this.page; }
  set end_(value){this.end =  this.view_data>this.pool_cnt-this.view_data*(value-1) ? this.pool_cnt : this.view_data*value;}

}

function base_data(from_date,from_time,to_date,to_time){  
  var data = {
      key: $('#deivceSelect').val(),
      from_date : from_date,
      from_time : from_time,
      to_date : to_date,
      to_time :to_time,
      csrfmiddlewaretoken: csrftoken
  }
  return data;
}

function buffer_filter(data){
  var cnt = data['cnt'];

  var data_filter1 = new Array();
  var data_filter2 = new Array();

  var data_3d1 = new Array();
  var data_3d2 = new Array();
  
  var time_filter1 = new Array();
  var time_filter2 = new Array();

  for(i=0; i <cnt; i++){
      //data['result'][i]['raw_data'] -> 선택된 데이터를 받음 (선택된 데이터는 최근순으로 나열되어 있음)
      data_filter1[cnt-i-1] = unupper(JSON.stringify(data['result'][i]['raw_data']));
      time_filter1[cnt-i-1] =  unupper(JSON.stringify(data['result'][i]['time']));
  
      //쉼표 기준으로 나눠 넣는 코드
      data_filter2[i] = data_filter1[cnt-i-1].split(',').map(function(item){return parseInt(item,10)});
  
      if(i==cnt-1){
        data_3d1 += [...data_filter2[i]];
      }
      else{
        data_3d1 += [...data_filter2[i],,];
      } 
    
      //T Z 떼는 코드
      time_filter2[i] = time_filter1[cnt-i-1].replace('T',"/").replace('Z','');
  }
  data_3d2 = data_3d1.split(',').map(function(item){return parseInt(item,10)})

return [{'base_data':data_filter2,'base_time':time_filter2,
        'threed_data':data_3d2,'no_slash_time':time_filter1.reverse()}]
}

function return_buffer(datas,urls){
  //console.log('데이터 저장')
  var data = datas;

  $.ajax({
    type: 'POST',
    url: String(urls),
    data: data,
    dataType:'JSON',
    async:false,
    success: function(data) {
      pool = buffer_filter(data);
      console.log(data);
      console.log(new Date());
    },
    error:function(){
      alert('key값 또는 올바른 시간을 입력하셨나요? \n 해당 시간에 존재하는 데이터가 없습니다.');
      pool = new Array();
      init_view('no_data');
    }
    });
    
    return pool;
}


