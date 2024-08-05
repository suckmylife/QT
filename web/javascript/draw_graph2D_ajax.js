$(document).ready(function(){
    $.ajax({
        url: '/main/sensor/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data){
                draw_heatmap(data);
                draw_heatmap_ani(data); 
            }
            else{
              alert("DATA CAN'T READ");
            };
        },
        error:function(){
            alert('ip dropdowns function is error, please modify ajax function!');
        }
    });
    $.ajax({
        url: '/main/id/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data){  
                create_ip_list(data);
            }
            else{
              alert("DATA CAN'T READ");
            };
        },
        error:function(){
            alert('function is error, please modify ajax function!');
        }
    });
});


$('#dropdowns').change(function(){
    $.ajax({
        url: '/main/sensor/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data){
                draw_heatmap(data);
                draw_heatmap_ani(data);
            }
            else{
              alert("DATA CAN'T READ");
            };
        },
        error:function(){
            alert('ip dropdowns change function is error, please modify ajax function!');
        }
    });
    
});