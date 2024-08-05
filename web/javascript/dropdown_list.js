function unupper(str){
    str = String(str);
    return str.replace(/\"/gi,'');
}

function create_ip_list(data){
    var device_id_str = [];
    var device_id_parsing = [];
    var device_id = [];
    for(var i=0; i<data['cnt']; i++)
    {
        device_id_str.push(data['data'][i]['device_data']['device_ip']);
    }

    device_id_parsing = unupper(device_id_str)
    if(data['cnt'] >1)
    {
        device_id = device_id_parsing.split(',').map(function(item)
        {
            return item
        });
    }
    else
    {
        device_id.push(data['data'][0]['device_data']['device_ip']);
    };
  
    let Create_Device_Id = document.getElementById('dropdowns');
    for(var i=0; i<data['cnt']; i++)
    {
        let new_aTag = document.createElement('option');
        new_aTag.setAttribute('class', 'dropdown-item');
        new_aTag.setAttribute('id', `item${i}`);
        new_aTag.setAttribute('value', device_id[i]);
        new_aTag.innerHTML = device_id[i];
        Create_Device_Id.appendChild(new_aTag);
    };
  };

  function input_ip()
  {
    document.getElementById('f_t3').value = $('#dropdowns').val();
  }
  $(document).ready(function(){
    $.ajax({
        url: '/main/id/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data)
            {  
                console.log(JSON.stringify(data));
                create_ip_list(data);
            }
            else
            {
              alert("DATA CAN'T READ");
            };
        },
        error:function(){
            alert('function is error, please modify ajax function!');
        }
    });

});