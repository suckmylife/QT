class pagination{
    constructor(currentPage, totalData,page_html){
        this.currentPage = currentPage;
        this.totalData = totalData;
        this.page_html = page_html;
        this.pre_data_page = 20;
    }

    get curr_page(){return this.currentPage;}
    set curr_page(value){this.currentPage = value;}

    setting_value(){
        
        var data_page = (this.totalData/this.pre_data_page)>0 ? this.pre_data_page : this.totalData;
        var pageCount = (this.totalData/this.pre_data_page)<10 ?  Math.ceil(this.totalData/this.pre_data_page):10;
        
        var totalPage = Math.ceil(this.totalData / data_page);

        if(totalPage<pageCount){pageCount=totalPage;}
    
        let pageGroup = Math.ceil(this.currentPage / pageCount); // 페이지 그룹
        let last = pageGroup * pageCount; //화면에 보여질 마지막 페이지 번호
        
        if (last > totalPage) {last = totalPage;}
    
        let first = last - (pageCount - 1); //화면에 보여질 첫번째 페이지 번호
        let next = last + 1;
        let prev = first - 1;
        
        return [first,next,prev,totalPage,last]
    }
    make_page(){
        var values = this.setting_value();
        let pageHtml = "";
    
        if (values[2] > 0) {
        pageHtml += "<li class='page-item'><button type='button' class='page-link' id='prev'> 이전 </button></li>";
        }
        var cur_num = this.curr_page;
        //페이징 번호 표시 
        for (var i = values[0]; i <= values[4]; i++) {
        if (cur_num == i) {
            pageHtml +=
            "<li id='selected' class='page-item'><button type='button' id=" + i + " class='page-link'>"+i+"</button></li>";
        } else {
            pageHtml += "<li class='page-item'><button type='button' class='page-link'  id=" + i + ">" + i + "</button></li>";
        }
        }
    
        if (values[4] < values[3]) {
        pageHtml += "<li class='page-item'><button type='button' class='page-link'  id='next'> 다음 </button></li>";
        }
        $(this.page_html).html(pageHtml);
    }
    
}