$(document).ready(function() {
    $("button[name='createSubmit']").click(function(){
        $.ajax({
            type: "GET",
            url: '/patients',
            success: function(data){
                console.log(data)
            },
            error: function(xhr, errmsg, err) {
                console.log(err)
            }
        });
    });
});