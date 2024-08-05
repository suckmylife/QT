$('#updateModule').on('show.bs.modal', function (event) {
    var button = $(event.relatedTarget) // Button that triggered the modal
    var modal = $(this)
});


// Update Module
$(document).ready(function(){
    $("button[name='updateBtn']").click(function(){
        const url = "/modules/" + $('#module_id').val() + "/"
        $.ajax({
            type: "PATCH",
            url: url,
            data: {
                'alias': $('#alias').val(),
                'memo': $('#memo').val()
            },
            success: function(res){
                console.log(res)
                alert('내역이 성공적으로 수정되었습니다.')
                window.location.replace(url)
            },
            error: function(xhr, errmsg, err) {
                console.log($('#module_id').val())
                alert('수정에 실패하였습니다.', err)
        }
        });
    });
});

// Delete Module
$(document).ready(function(){
    $("button[name='deleteBtn']").click(function(){
        $.ajax({
            type: "DELETE",
            url: window.location.href,
            success: function(res){
                alert('모듈이 성공적으로 삭제되었습니다.')
                window.location.replace("/modules")
            },
            error: function(xhr, errmsg, err) {
                console.log(errmsg, err)
                alert('모듈 삭제에 실패하였습니다.\n현재 모듈에 소속된 보드가 존재하거나, Key로 사용중인 모듈이 존재합니다.\n모듈에 소속된 보드를 삭제하거나, 연결된 Key를 삭제하고 다시 시도해주세요.', err)
            }
        });
    });
});