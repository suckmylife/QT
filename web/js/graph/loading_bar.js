
$(function(){
	$(document).ajaxStart(function() {
        var width = 0;
        var height = 0;
        var left = 0;
        var top = 0;

        width = 100;
        height = 100;
        

        top = ( $(window).height() - height ) / 2 + $(window).scrollTop();
        left = ( $(window).width() - width ) / 2 + $(window).scrollLeft();



        if($("#div_ajax_load_image").length != 0 && !isreal) {
               $("#div_ajax_load_image").css({
                      "top": top+"px",
                      "left": left+"px"
               });
               $("#div_ajax_load_image").show();
               //
               $('#div_ajax_load_fade').show();
              
        }
        else {
               if(!isreal){
                     $('body').append('<div id="div_ajax_load_image" style="position:absolute; top:' 
                     + top + 'px; left:' 
                     + left + 'px; width:' 
                     + width + 'px; height:' 
                     + height + `px; z-index:9999; "><img src="/static/img/loading.gif" style="width:${width}px; height:${height}px;"></div>`);
                     $("#div_ajax_load_image").show();

                     $('body').append('<div id="div_ajax_load_fade" style="position:absolute; top:' 
                     + 0 + '%; left:' 
                     + 0 + '%; width:' 
                     + 100 + '%; height:' 
                     + 100 + `%; z-index:9998; 
                     background:#666666;
                     -moz-opacity:0.8;
                     opacity:.80;
                     filter:alpha(opacity=60);"></div>`);
                     $("#div_ajax_load_fade").show();
               }
            }

    }).ajaxStop(function() {
        $("#div_ajax_load_image").hide();
        $("#div_ajax_load_fade").hide();
    
    });

});
