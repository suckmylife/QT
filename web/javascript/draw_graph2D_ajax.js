
$(document).ready(function(){
    $.ajax({
        url: '/main/id/',
        type:'GET',
        dataType:'json',
        success:function(data){
            if(data)
            {  
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
    // bar_graph();
    // draw_heatmap();
    // draw_heatmap_ani();
    // draw_graph3D_static();
    // draw_graph3D_ani();
    // linear_graph();
    draw_sequence();
});

$('#style').change(function(){
    draw_graph3D_static();
    draw_graph3D_ani();
  });

$('#apply').on('click',function(e){
    // draw_heatmap();
    // draw_heatmap_ani();
    // draw_graph3D_static();
    // draw_graph3D_ani();
    draw_sequence();
});

$('#dropdowns').change(function(){
    // draw_heatmap();
    // draw_heatmap_ani();
    draw_sequence();
    //input_ip();
});
