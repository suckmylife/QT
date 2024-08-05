// Search
$(document).ready(function(){
    $("button[name='searchBtn']").click(function(){
        const url = '/patients/' + $('#patientId').val() + '/histories/'
        $.ajax({
            type: "GET",
            url: url,

            success: function(res){
                window.location.replace(url)
            },
            error: function(xhr, errmsg, err) {
                alert('해당 환자 번호의 내역이 존재하지 않습니다.', err)
        }
        });
    });
});


// Create History
$(document).ready(function(){
    $("button[name='modalCreateBtn']").click(function(){
        const url = '/patients/' + $('#modalPatientId').val() + '/histories/'
        $.ajax({
            type: "POST",
            url: url,
            data: {
                "patient": $("#modalPatientId").val(),
                "memo": $("#modalMemo").val(),
                "diagnosis": $("#modalDiagnosis").val(),
                "impairment": $("#modalImpairment").val(),
                "bmi": $("#modalBmi").val(),
                "adm_duration": $("#modalAdmDuration").val(),
                "risk_assessment": $("#modalRiskAssessment").val(),
                "body_position": $("#modalBodyPosition").val(),
                "matress_position": $("#modalMatressPosition").val(),
            },
            success: function(res){
                alert('내역이 성공적으로 생성되었습니다.')
                window.location.replace(url)
            },
            error: function(xhr, errmsg, err) {
                alert('생성에 실패하였습니다.', err)
        }
        });
    });
});

// Detail History
function get_history(url) {
    $.ajax({
        type: "GET",
        url: url,
        success: function(res){
            $('#memo').html(res.histories.memo)
            $('.diagnosis_p').html(res.histories.t_diagnosis)
            $('.impairment_p').html(res.histories.t_impairment)
            $('.body_position_p').html(res.histories.t_body_position)
            $('.matress_position_p').html(res.histories.t_matress_position)
            $('.adm_duration_p').html(res.histories.adm_duration)
            $('.bmi_p').html(res.histories.bmi)
            $('.bmi_cat_p').html(res.histories.t_bmi_cat)
            $('.risk_assessment_p').html(res.histories.risk_assessment)
            $('.risk_assessment_cat_p').html(res.histories.t_risk_assessment_cat)

            $('#id').val(res.histories.id)
            $('#diagnosis').val(res.histories.diagnosis)
            $('#impairment').val(res.histories.impairment)
            $('#body_position').val(res.histories.body_position)
            $('#matress_position').val(res.histories.matress_position)
            $('#adm_duration').val(res.histories.adm_duration)
            $('#bmi').val(res.histories.bmi)
            $('#bmi_cat').val(res.histories.bmi_cat)
            $('#risk_assessment').val(res.histories.risk_assessment)
            $('#risk_assessment_cat').val(res.histories.risk_assessment_cat)
        },
        error: function(xhr, errmsg, err) {
            alert('해당 환자 번호의 내역이 존재하지 않습니다.', err)
            window.location.replace('/patients/histories')
        }
    });
};


// 이전기록 불러오기
function SelectOption(obj, optVal){
    var objNum = obj.children('option').length

    for (i=0; i < objNum; i++) {
        optionObj = obj.children('option')[i]
        if (optionObj.value == optVal) {
            optionObj.selected=true;
            break
        }
        
    }
}

$(document).ready(function(){
    $("#updateHistory").click(function(){
        $('#modalCreateBtn').text('수정')
        $('#modalCreateBtn').attr('name', 'modalUpdateBtn');
        $('#modalCreateBtn').attr('id', 'modalUpdateBtn');
        $('#modalCreateBtn').attr('onclick', 'updateHistory();');

        var memo = $('#memo').text()
        var diagnosis = $('#diagnosis').val()
        var impairment = $('#impairment').val()
        var body_position = $('#body_position').val()
        var matress_position = $('#matress_position').val()
        var adm_duration = $('#adm_duration').val()
        var bmi = $('#bmi').val()
        var bmi_cat = $('#bmi_cat').val()
        var risk_assessment = $('#risk_assessment').val()
        var risk_assessment_cat = $('#risk_assessment_cat').val()

        // TODO (과거력 파트 추가)
        $('#modalMemo').val(memo)
        SelectOption($('#modalDiagnosis'), diagnosis)
        SelectOption($('#modalImpairment'), impairment)
        SelectOption($('#modalBodyPosition'), body_position)
        SelectOption($('#modalMatressPosition'), matress_position)
        $('#modalAdmDuration').val(adm_duration)
        $('#modalBmi').val(bmi)
        $('#modalBmiCategory').val(bmi_cat)
        $('#modalRiskAssessment').val(risk_assessment)
        $('#modalRiskAssessmentCate').val(risk_assessment_cat)
    });
});


// Update History
function updateHistory(){
    $("button[name='modalUpdateBtn']").click(function(){
        const url = '/patients/' + $('#modalPatientId').val() + '/histories/' + $('#id').val()
        $.ajax({
            type: "PATCH",
            url: url,
            data: {
                csrfmiddlewaretoken: csrftoken,
                "patient": $("#modalPatientId").val(),
                "memo": $("#modalMemo").val(),
                "diagnosis": $("#modalDiagnosis").val(),
                "impairment": $("#modalImpairment").val(),
                "bmi": $("#modalBmi").val(),
                "adm_duration": $("#modalAdmDuration").val(),
                "risk_assessment": $("#modalRiskAssessment").val(),
                "body_position": $("#modalBodyPosition").val(),
                "matress_position": $("#modalMatressPosition").val(),
            },
            success: function(res){
                alert('내역이 성공적으로 수정되었습니다.')
                window.location.replace(url)
            },
            error: function(xhr, errmsg, err) {
                alert('수정에 실패하였습니다.', err)
            }
        });
    });
};

// Delete History
function delete_history(id){
    $.ajax({
        type: "DELETE",
        url: window.location.href + id,
        data: {
            csrfmiddlewaretoken: csrftoken,
        },
        success: function(data){
            alert('성공적으로 삭제되었습니다')
            window.location.replace(window.location.href)
        },
        error: function(xhr, errmsg, err) {
            console.log(err)
            alert('삭제에 실패하였습니다.')
            window.location.replace(window.location.href)
        }
    });
};

// Braden Scale Calculator
$('input[type="radio"]').change(function(){
    var total = 0

    var sensorVal = parseInt($('input[name="sensor"]:checked').val() || 0);
    var humidVal = parseInt($('input[name="humid"]:checked').val() || 0);
    var activityVal = parseInt($('input[name="activity"]:checked').val() || 0);
    var mobilityVal = parseInt($('input[name="mobility"]:checked').val() || 0);
    var nutritionVal = parseInt($('input[name="nutrition"]:checked').val() || 0);
    var frictionVal = parseInt($('input[name="friction"]:checked').val() || 0);
    
    total = sensorVal + humidVal + activityVal + mobilityVal + nutritionVal + frictionVal;

    if(total != NaN){
        $('#total').text(total)
        $('#modalRiskAssessment').val(total)
    }
});

// BMI Calculator
$('#modalBmi').change(function(){
    var bmi =  Number($('#modalBmi').val())

    if(bmi < 18.4){
        $('#modalBmiCategory').val('low');
        // $('#modalBmiCategory').val(1);
    } else if(bmi>=18.4 && bmi <=24.9) {
        $('#modalBmiCategory').val('moderate');
    } else {
        $('#modalBmiCategory').val('high');
    }
});

// Modal
$('#createModal').on('show.bs.modal', function (event) {
    var button = $(event.relatedTarget) // Button that triggered the modal
    var recipient = button.data('whatever') // Extract info from data-* attributes
    // If necessary, you could initiate an AJAX request here (and then do the updating in a callback).
    // Update the modal's content. We'll use jQuery here, but you could use a data binding library or other methods instead.
    var modal = $(this)
});


