// Update Board
$(document).ready(function(){
    $("button[name='updateBtn']").click(function(){
        const url = "/modules/" + $('#module').val() + "/boards/"+  $('#id').val() + "/"
        $.ajax({
            type: "PATCH",
            url: url,
            data: {
                'id': $('#id').val(),
                'ip_address': $('#ip_address').val(),
                'location': $('#location').val(),
                'alias': $('#alias').val(),
                'serial_num': $('#serial_num').val(),
                'port': $('#port').val(),
                'mac_address': $('#mac_address').val(),
                'health': $('#health').val(),
                'version': $('#version').val(),
                'module': $('#module').val(),
            },
            success: function(res){
                console.log(res)
                alert('내역이 성공적으로 수정되었습니다.')
                window.location.replace(url)
            },
            error: function(xhr, errmsg, err) {
                alert('수정에 실패하였습니다.', err)
        }
        });
    });
});

$(document).ready(function(){
    $("button[name='deleteBtn']").click(function(){
        var success_url = "/modules/" + $('#module').val() + "/"
        console.log('delete')
        $.ajax({
            type: "DELETE",
            url: window.location.href,
            success: function(res){
                alert('보드가 성공적으로 삭제되었습니다.')
                window.location.replace(success_url)
            },
            error: function(xhr, errmsg, err) {
                console.log(errmsg, err)
                alert('보드 삭제에 실패하였습니다.', err)
            }
        });
    });
});