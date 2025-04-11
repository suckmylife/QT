#ifndef MODE_H
#define MODE_H

#include <QObject>
#include <QSet>

#include "command.h"

class mode : public QObject
{
    Q_OBJECT
public:
    explicit mode(QObject *parent = nullptr);
    ~mode();
    void alter_mode(int num); // 교대부양
    void dist_mode(int num); //압력 분산
    void dist_H_mode(int num); //공기 많음 모드
    void dist_M_mode(int num); // 공기 보통 모드
    void dist_L_mode(int num); //공기 적음 모드
    void finish_mode(int num); // 모드에 공통적으로 마무리되는 마무리 모드 
    int get_alter_circu(); //교대부양 모드 반복 횟수 가져오기
    bool get_dist(); // 압력분산 모드가 끝났는지 확인하는 함수 
    bool get_dist_close(); 
    bool get_H2(); // 공기 많음 모드가 끝났는지 확인하는 함수
    bool get_L2(); // 공기 적음 모드가 끝났는지 확인하는 함수 
    bool get_M2(); // 공기 보통 모드가 끝났는지 확인하는 함수 
    bool get_end(); // 마무리 모드가 끝났는지 확인하는 함수 
    void set_dist(bool b);// 압력분산 모드가 끝났는지 아닌지 설정하는 함수 
    void set_H2(bool b); // 공기 많음 모드가 끝났는지 아닌지 설정하는 함수 
    void set_L2(bool b);// 공기 적음 모드가 끝났는지 아닌지 설정하는 함수 
    void set_M2(bool b);// 공기 보통 모드가 끝났는지 아닌지 설정하는 함수 
    void set_end(bool b); //마무리 모드가  끝났는지 아닌지 설정하는 함수 
    void set_alter_circu(int cnt); // 교대부양 모드 반복횟수 설정 
    void set_time_cnt(int cnt); //모드 초기화 함수 

    void Cal_L(); //로우 캘리브레이션
    void Cal_H(); //하이 캘리브레이션

    void all_out_open();
    void all_out_close();
    void all_in_open(int num);
    void all_in_close(int num);
    void all_in_open();
    void all_in_close();
    void pump_on();
    void pump_off();

private:
    command* cmd;


    int alter_circu_cnt = 0;
    int dist_cnt        = 0;
    int dist_basic_cnt  = 0;
    int term_time       = 4500;
    int comm_time       = 30;
    int time_cnt   = 0;
    int cmd_cnt         = 0;

    //mainWindow -> timer_t = 50 //ms -> ex. calculate = 100 * 50 = 5000ms = 5sec
    int dist_out_close  = 5; //
    int dist_in_open    = 60; //
    int dist_in_close   = 115;
    int dist_end_time   = 170;//

    int dist_1_start    = 300;
    int dist_2_start    = 450;

    int alter_1_start   = 90;//270;  //+30sec 1_2 out open
    int alter_2_start   = 180;//420;  //1_2 out close
    int alter_3_start   = 270;//570;  //1_2 in open pump
    int alter_4_start   = 360;//720;  //1_2 in close
    int alter_5_start   = 450;//990;  //+30sec 3_4 out open
    int alter_6_start   = 540;//1140; //3_4 out close
    int alter_7_start   = 630;//1290; //3_4 out open pump
    int alter_8_start   = 720;//1440; //3_4 out close
    int alter_9_start   = 810;//1590; //end
    const int min_5     = 300000;

    int finish_1_start  = 570;
    int finish_2_start  = 720;
    int finish_3_start  = 870;

    bool alter_start        = false;
    bool alter_end          = false;
    bool dist_start_close   = false;
    bool dist_end           = false;
    bool end_end            = false;
    bool h2_end             = false;
    bool m2_end             = false;
    bool l2_end             = false;
    bool is_close           = false;
    bool dist_shot          = false;
    bool is_5min            = false;

    QSet<int> addr_contain;
    QTimer *timer;

    void set_dist_close(bool close);

signals:
    void end_dist_request();
    void start_timer();
    void stop_timer();

};

#endif // MODE_H
