#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    heatmap      = new graph();
    info         = new information(ui->user_birth,ui->user_tall,ui->user_kg,ui->user_male,ui->user_female,ui->user_input_next_btn);
    serialworker = new request();
    timer        = new QTimer();
    mode_timer   = new QTimer();
    mode_        = new mode();
    db           = new Database("/home/neoable/Neo_Air_Mat/Neo_Air_Mat.db");

    dist_btn_style   = ui->dist_mode_btn->styleSheet();
    dist_h_btn_style = ui->dist_H_btn->styleSheet();
    dist_l_btn_style = ui->dist_L_btn->styleSheet();
    dist_m_btn_style = ui->dist_M_btn->styleSheet();
    alter_btn_style  = ui->alter_mode_btn->styleSheet();

    virtual_keyboard_setting();
    for(int i = 0; i <28; i++)
    {
        pop_dist.append(i);
    }

    serialworker->moveToThread(&thread);
    //db->moveToThread(&DB_thread);
    ui->graphicsView->setScene(heatmap->initialize_graph());
    info->set_2_btn(ui->user_birth_2,ui->user_tall_2,ui->user_kg_2,ui->user_male_2,ui->user_female_2,ui->modi_btn);
    ui->calibration->setVisible(false);

    for(int year = 1930; year<=2024; ++year)
    {
        ui->user_birth->addItem(QString::number(year));
        ui->user_birth_2->addItem(QString::number(year));
    }
    for(int tall = 140; tall<=190; ++tall)
    {
        ui->user_tall->addItem(QString::number(tall));
        ui->user_tall_2->addItem(QString::number(tall));
    }
    for(int kg = 35; kg<=100; ++kg)
    {
        ui->user_kg->addItem(QString::number(kg));
        ui->user_kg_2->addItem(QString::number(kg));
    }
    ui->user_birth->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->user_birth->setMaxVisibleItems(5);
    ui->user_tall->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->user_tall->setMaxVisibleItems(5);
    ui->user_kg->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->user_kg->setMaxVisibleItems(5);

    //page_1
    connect(ui->TP0001S_start_btn,  SIGNAL(clicked(bool)), this, SLOT(page_2s()));
    //page_2
    connect(ui->user_input_next_btn,  SIGNAL(clicked(bool)), this, SLOT(page_10s_first_btn()));
    //page_3
    QObject::connect(ui->setting_btn, &QPushButton::clicked, this,[this](){ui->stackedWidget->setCurrentIndex(3);});
    QObject::connect(ui->detail_btn, &QPushButton::clicked, this,[this](){is_detail = !is_detail;});
    connect(ui->dist_mode_btn, SIGNAL(clicked(bool)), this, SLOT(dist_mode()));
    connect(ui->alter_mode_btn, SIGNAL(clicked(bool)), this, SLOT(alter_mode()));
    connect(ui->dist_H_btn, SIGNAL(clicked(bool)), this, SLOT(dist_H_mode()));
    connect(ui->dist_L_btn, SIGNAL(clicked(bool)), this, SLOT(dist_L_mode()));
    connect(ui->dist_M_btn, SIGNAL(clicked(bool)), this, SLOT(dist_M_mode()));
    //page_4
    QObject::connect(ui->secret_btn, &QPushButton::clicked, this,[this](){ui->stackedWidget->setCurrentIndex(4);});
    connect(ui->modi_btn, SIGNAL(clicked(bool)), this, SLOT(update_info()));
    connect(ui->reset_btn, SIGNAL(clicked(bool)), this, SLOT(add_patient()));
    QObject::connect(ui->cancle_btn, &QPushButton::clicked, this,[this](){ui->stackedWidget->setCurrentIndex(2);});
    //page_5
    QObject::connect(ui->secret_cancle_btn, &QPushButton::clicked, this,[this](){ui->stackedWidget->setCurrentIndex(3);});
    QObject::connect(ui->secret_ok_btn, &QPushButton::clicked, this,[this](){ui->stackedWidget->setCurrentIndex(5);});
    //page_6
    connect(ui->kpa_ok_btn, SIGNAL(clicked(bool)), this, SLOT(apply_setting()));
    QObject::connect(ui->kpa_cancle_btn, &QPushButton::clicked, this,[this](){ui->stackedWidget->setCurrentIndex(3);});
    connect(ui->cal_page, SIGNAL(clicked(bool)), this, SLOT(cali_page()));
    //(calibration page)
    QObject::connect(ui->low_cal, &QPushButton::clicked, this,[this](){mode_->Cal_L();});
    QObject::connect(ui->high_cal, &QPushButton::clicked, this,[this](){mode_->Cal_H();});
    QObject::connect(ui->cal_out_open, &QPushButton::clicked, this,[this](){mode_->all_out_open();});
    QObject::connect(ui->cal_out_close, &QPushButton::clicked, this,[this](){mode_->all_out_close();});
    QObject::connect(ui->cal_in_open, &QPushButton::clicked, this,[this](){mode_->all_in_open();});
    QObject::connect(ui->cal_in_close, &QPushButton::clicked, this,[this](){mode_->all_in_close();});
    QObject::connect(ui->cal_pump_off, &QPushButton::clicked, this,[this](){mode_->pump_off();});
    QObject::connect(ui->cal_pump_on, &QPushButton::clicked, this,[this](){mode_->pump_on();});
    connect(ui->end_cal, SIGNAL(clicked(bool)), this, SLOT(cali_end()));
    //thread
    connect(&thread, SIGNAL(started()), serialworker,SLOT(start_request()));
    //connect(&DB_thread, SIGNAL(started()), db,SLOT(insert_serial_data()));
    //TIMER
    connect(timer,SIGNAL(timeout()),SLOT(start_thread()));
    connect(mode_timer, SIGNAL(timeout()), SLOT(start_DB()));
    //SIGNAL
    connect(serialworker, SIGNAL(end_response()), this, SLOT(mode_condi()));
}

void MainWindow::start_DB()
{
    db->insert_serial_data();
}

MainWindow::~MainWindow()
{
    delete heatmap;
    delete mode_;
    delete serialworker;
    delete info;
    delete timer;
    delete mode_timer;
}

void MainWindow::cali_page()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->calibration->setVisible(true);
}

void MainWindow::cali_end()
{
    ui->stackedWidget->setCurrentIndex(5);
    ui->calibration->setVisible(false);
}

void MainWindow::keyboard_exit()
{
    if(dev_pass->text().length()>3)
    {
        mode_timer->singleShot(500,[=](){
            dev_pass->process->terminate();
            dev_pass->process->waitForFinished();
        });
    }

}
void MainWindow::virtual_keyboard_setting()
{
    //virtual keyboard setting
    QFont font("PibotoLt",20,QFont::Bold);
    dev_pass = new LineEdit(ui->frame_3);
    dev_pass->setFixedSize(339,42);
    dev_pass->setAlignment(Qt::AlignCenter);
    dev_pass->setFont(font);
    dev_pass->setStyleSheet("QLineEdit{lineedit-password-character:42;background-color: rgb(255, 255, 255); cursor:IBeam;}");
    dev_pass->setEchoMode(QLineEdit::Password);
    dev_pass->move(50,150);

    set_press = new LineEdit(ui->setting_frame_2);
    set_one_press = new LineEdit(ui->setting_frame_2);
    set_two_press = new LineEdit(ui->setting_frame_2);
    set_two_press2 = new LineEdit(ui->setting_frame_2);
    set_three_press = new LineEdit(ui->setting_frame_2);
    set_three_press2 = new LineEdit(ui->setting_frame_2);
    set_four_press = new LineEdit(ui->setting_frame_2);
    set_four_press2 = new LineEdit(ui->setting_frame_2);
    set_five_press = new LineEdit(ui->setting_frame_2);
    set_five_press2 = new LineEdit(ui->setting_frame_2);
    set_six_press = new LineEdit(ui->setting_frame_2);
    set_six_press2 = new LineEdit(ui->setting_frame_2);
    set_seven_press = new LineEdit(ui->setting_frame_2);
    set_seven_press2 = new LineEdit(ui->setting_frame_2);
    line_edits.append(set_press);
    line_edits.append(set_one_press);
    line_edits.append(set_two_press);
    line_edits.append(set_two_press2);
    line_edits.append(set_three_press);
    line_edits.append(set_three_press2);
    line_edits.append(set_four_press);
    line_edits.append(set_four_press2);
    line_edits.append(set_five_press);
    line_edits.append(set_five_press2);
    line_edits.append(set_six_press);
    line_edits.append(set_six_press2);
    line_edits.append(set_seven_press);
    line_edits.append(set_seven_press2);
    QFont font1("PibotoLt",16);
    QDoubleValidator *valid = new QDoubleValidator();
    valid->setRange(0.0,20.0,1);

    for(int i = 0; i < line_edits.size(); ++i)
    {
        line_edits[i]->setFont(font1);
        line_edits[i]->setStyleSheet("border-radius:0px;background-color: rgb(255, 255, 255); cursor:IBeam;");

        line_edits[i]->setValidator(valid);
        line_edits[i]->setAlignment(Qt::AlignCenter);
        if(i == 0)
        {
            line_edits[i]->setText("1.0");
            line_edits[i]->setFixedSize(71,38);
            line_edits[i]->move(30,200);
        }
        else if(i==1)
        {
            line_edits[i]->setText("0.0");
            line_edits[i]->setFixedSize(71,30);
            line_edits[i]->move(70,315);
        }
        else if(i == line_edits.size()-1)
        {
            line_edits[i]->setFixedSize(71,30);
            line_edits[i]->move(165,355+40*((i-3)/2));
            line_edits[i]->setText("6.0");
        }
        else if(i % 2 == 0)
        {
            double numbers = 0.1+(0.5*(i/2-1));
            QString origin = QString::number(numbers,'f',1);
            QString contain = "%2";
            QString default_ = contain.arg(origin);
            line_edits[i]->setFixedSize(71,30);
            line_edits[i]->move(70,315+40*(i/2));
            line_edits[i]->setText(default_);
        }
        else
        {
            double numbers = 0.5+(0.5*(i/2-1));
            QString origin = QString::number(numbers,'f',1);
            QString contain = "%2";
            QString default_ = contain.arg(origin);
            line_edits[i]->setFixedSize(71,30);
            line_edits[i]->move(165,355+40*((i-3)/2));
            line_edits[i]->setText(default_);
        }
    }

    connect(dev_pass, SIGNAL(textChanged(QString)),this,SLOT(keyboard_exit()));

}

void MainWindow::filter_kPa()
{
    for(int i = 0; i<28; i++)
    {
        int group = (i / 4) * 4 + 4;
        int val   = group - (i % 4);
        if(contain_state[val])
        {
            if(pop_dist.contains(val) and !dupli_dist.contains(val))//기준　충족된　압력값　중복　제외시키기　위한　코드
            {
                int index = pop_dist.indexOf(val); //처음　기준충족된　인덱스　추출　후　제거，　중복리스트에　추가하여　교차검증
                pop_dist.remove(index);
                dupli_dist.insert(val);
            }
        }
    }
}

void MainWindow::mode_condi()
{
    if(is_dist)
    {
        sleep_time++;
        if(sleep_time > sleep_time_interval)
        {
            mode_->dist_mode(0);
            sleep_time = 0;
        }
        if(mode_->get_dist()) //dist mode end trigger
        {
            is_dist = false;
            mode_->set_dist(false);
            sleep_time = 0;
            what_is_mode = "None";
        }
    }
    else if(is_alter)
    {
        sleep_time++;
        if(mode_->get_alter_circu() < alter_repeat_time)
        {
            if(sleep_time > sleep_time_interval)
            {
                sleep_time = 0;
                mode_->alter_mode(0);
            }
        }
        else
        {
            is_alter = false;
            mode_->set_alter_circu(0);
            timer->singleShot(wait_t,[=](){
                is_finish_mode = true;
            });
        }
    }
    else if(is_H)
    {
        sleep_time++;
        if(!mode_->get_H2())
        {
            if(sleep_time > sleep_time_interval)
            {
                sleep_time = 0;
                mode_->dist_H_mode(0);
            }
        }
        else
        {
            is_H = false;
            mode_->set_H2(false);
            timer->singleShot(wait_t,[=](){
                is_finish_mode = true;
            });
        }
    }
    else if(is_M)
    {
        sleep_time++;
        if(!mode_->get_M2())
        {
            if(sleep_time > sleep_time_interval)
            {
                sleep_time = 0;
                mode_->dist_M_mode(0);
            }
        }
        else
        {
            is_M = false;
            mode_->set_M2(false);
            timer->singleShot(wait_t,[=](){
                is_finish_mode = true;
            });
        }
    }
    else if(is_L)
    {
        sleep_time++;
        if(!mode_->get_L2())
        {
            if(sleep_time > sleep_time_interval)
            {
                sleep_time = 0;
                mode_->dist_L_mode(0);
            }
        }
        else
        {
            is_L = false;
            mode_->set_L2(false);
            timer->singleShot(wait_t,[=](){
                is_finish_mode = true;
            });
        }
    }
    else if(is_finish_mode)
    {
        sleep_time++;
        if(!mode_->get_end())
        {
            if(sleep_time > sleep_time_interval)
            {
                sleep_time = 0;
                mode_->finish_mode(0);
            }
        }
        else
        {
            is_finish_mode = false;
            mode_->set_end(false);
            what_is_mode = "None";
        }
    }
}

void MainWindow::mode_basic()
{
    is_dist  = false;
    is_alter = false;
    is_H     = false;
    is_M     = false;
    is_L     = false;
    sleep_time = 0;
    mode_->set_alter_circu(0);
    mode_->set_dist(false);
    mode_->set_H2(false);
    mode_->set_L2(false);
    mode_->set_M2(false);
    mode_->set_time_cnt(0);
    dupli_dist.clear();
    ui->dist_mode_btn->setStyleSheet(dist_btn_style);
    ui->dist_H_btn->setStyleSheet(dist_h_btn_style);
    ui->dist_L_btn->setStyleSheet(dist_l_btn_style);
    ui->dist_M_btn->setStyleSheet(dist_m_btn_style);
    ui->alter_mode_btn->setStyleSheet(alter_btn_style);
}

void MainWindow::dist_mode()
{
    mode_basic();
    is_dist = true;
    what_is_mode = "기본압력분산";
    ui->dist_mode_btn->setStyleSheet("border:1px solid black; background-color:blue");
}

void MainWindow::alter_mode()
{
    mode_basic();
    what_is_mode = "교대부양";
    is_alter = true;
    ui->alter_mode_btn->setStyleSheet("border:1px solid black; background-color:blue");
}

void MainWindow::dist_H_mode()
{
    mode_basic();
    what_is_mode = "압력분산공기많음";
    is_H = true;
    ui->dist_H_btn->setStyleSheet("border:1px solid black; background-color:blue");
}

void MainWindow::dist_M_mode()
{
    mode_basic();
    what_is_mode = "압력분산공기보통";
    is_M = true;
    ui->dist_M_btn->setStyleSheet("border:1px solid black; background-color:blue");
}

void MainWindow::dist_L_mode()
{
    mode_basic();
    what_is_mode = "압력분산공기적음";
    is_L = true;
    ui->dist_L_btn->setStyleSheet("border:1px solid black; background-color:blue");
}

void MainWindow::add_patient()
{
    ui->stackedWidget->setCurrentIndex(1);
    timer->stop();
    mode_timer->stop();
    if(thread.isRunning())
    {
        thread.quit();
        thread.wait();
    }
}

void MainWindow::update_info()
{
    db->update_patient_info(ui->user_birth_2->currentText(),ui->user_kg_2->currentText(),ui->user_tall_2->currentText(),info->get_gender(),db_id);
    int age = 2024 - ui->user_birth_2->currentText().toInt() + 1;
    QString gen;
    QString personal_num;
    info->get_gender() == "female" ? gen = "F" : gen = "M";
    personal_num = QString::number(age) + gen + ui->user_tall_2->currentText().right(2) + ui->user_kg_2->currentText();
    ui->personal_num->setText(personal_num);
}
void MainWindow::start_thread()
{
    datas = serialworker->get_data();
    if(datas.size() >1)
    {
        heatmap->get_data(datas);
        ui->graphicsView->setScene(heatmap->update_map(is_detail));
        contain_state = heatmap->get_kPa_bit();
        db->set_Pressure(db_id, datas);
        db->get_mode(what_is_mode);
    }
    if(thread.isRunning())
    {
        thread.quit();
        thread.wait();
    }
    thread.start();
}

void MainWindow::page_2s()
{
    ui->stackedWidget->setCurrentIndex(1);
    communication::instance()->connect_serial();
    timer->start(timer_t);
    mode_timer->start(db_timer);
}

void MainWindow::page_10s_first_btn()
{
    if(info->check_btn())
    {
        ui->stackedWidget->setCurrentIndex(2); //화면전환
        info->set_check_btn(false); // 수정 및 다음 버튼 초기화
        QString personal_num;
        QString gen;
        int age = 2024 - ui->user_birth->currentText().toInt() + 1;
        info->get_gender() == "female" ? gen = "F" : gen = "M";
        personal_num = QString::number(age) + gen + ui->user_tall->currentText().right(2) + ui->user_kg->currentText();
        ui->personal_num->setText(personal_num);
        db_id = db->insert_patient_info(ui->user_birth->currentText(),ui->user_kg->currentText(),ui->user_tall->currentText(),info->get_gender());
        if(timer->isActive())
        {
            timer->stop();
            mode_timer->stop();
        }
        timer->start(timer_t);
        mode_timer->start(db_timer);
    }
}
void MainWindow::apply_setting()
{
    if(info->check_set_btn())
    {
        info->set_sett_btn(false);
        ui->kpa_ok_btn->setStyleSheet("background-color:rgb(198, 198, 198);color:rgb(255, 255, 255);border:solid 1px;");
        QMessageBox::information(nullptr,QString::fromUtf8("알림"), QString::fromUtf8("설정이 적용되었습니다"));
        info->set_range_kPa(set_one_press,set_two_press,set_two_press2,set_three_press,set_three_press2,set_four_press,set_four_press2
                            ,set_five_press,set_five_press2,set_six_press,set_six_press2,set_seven_press,set_press,heatmap,ui->kpa_ok_btn);
    }
}
