$(document).ready(function() {
    $("button[name='patchSubmit']").click(function(){
        $.ajax({
            type: "PATCH",
            url: window.location.href,
            data: {
                'alias': $('#alias').val(),
                'activation': $('#activation').val(),
                'module': $('#module').val(),
                'patient': $('#patient').val(),
            },
            success: function(data){
                alert('성공적으로 수정되었습니다')
                window.location.replace('/keys')
            },
            error: function(xhr, errmsg, err) {
                console.log(err)
                alert('수정에 실패하였습니다.', err)
        }
        })
    });
    $("button[name='deleteSubmit']").click(function(){
        $.ajax({
            type: "DELETE",
            url: window.location.href,
            success: function(data){
                alert('성공적으로 삭제되었습니다')
                window.location.replace('/keys')
            },
            error: function(xhr, errmsg, err) {
                console.log(err)
                alert('삭제에 실패하였습니다. 잠시후 다시 시도해주세요')
                window.location.replace('/keys')
            }
        })
    });
});