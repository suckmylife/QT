#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QAbstractItemView>
#include <bitset>
#include <QMessageBox>
#include <QSet>
#include <QLayout>

#include "ui_Neo_Air_mat.h"
#include "graph.h"
#include "request.h"
#include "information.h"
#include "mode.h"
#include "lineedit.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void page_2s(); //첫페이지 버튼 클릭후 다음 페이지로 이동 
    void page_10s_first_btn(); // 환자정보 입력후 압력읽는 타이머 시작, 데이터베이스에 환자정보 저장 후에 압력저장 타이머 시작
    void start_thread(); // 압력 읽고 그래프 업데이트
    void start_DB(); // 데이터베이스에 압력값 저장 
    void update_info(); //환자정보 수정 
    void add_patient(); // 처음화면 부터 다시 시작
    void apply_setting(); //설정값 변경(그래프 색상, 압력 주입값) (24.09.19 현재에선 의미없음)

    void dist_M_mode(); // 공기 보통 모드
    void dist_L_mode(); // 공기 적음 모드
    void dist_H_mode(); // 공기 많음 모드
    void dist_mode();   // 압력 분산 모드
    void alter_mode();  // 교대부양 모드

    void mode_condi(); // 모드 통합 관리
    void keyboard_exit(); // 가상키보드 종료

    void cali_page(); // 캘리브레이션 페이지 이동 
    void cali_end(); // 캘리브레이션 종료후 전 화면으로 이동 

private:
    Ui::MainWindow *ui;
    graph *heatmap;
    information* info;
    Database* db;
    request* serialworker;
    QThread thread;
    QThread DB_thread;
    QTimer* timer;
    QTimer* mode_timer;
    QMap<QString,QString> datas;
    mode *mode_;
    QVector<int> pop_dist;
    QSet<int> dupli_dist;
    bitset<28> contain_state;

    QString dist_btn_style;
    QString dist_h_btn_style;
    QString dist_l_btn_style;
    QString dist_m_btn_style;
    QString alter_btn_style;
    QString what_is_mode = "None";

    LineEdit* dev_pass;
    LineEdit* set_press;
    LineEdit* set_one_press;
    LineEdit* set_two_press;
    LineEdit* set_two_press2;
    LineEdit* set_three_press;
    LineEdit* set_three_press2;
    LineEdit* set_four_press;
    LineEdit* set_four_press2;
    LineEdit* set_five_press;
    LineEdit* set_five_press2;
    LineEdit* set_six_press;
    LineEdit* set_six_press2;
    LineEdit* set_seven_press;
    LineEdit* set_seven_press2;
    LineEdit* cal_high;
    QVector<LineEdit*> line_edits;

    int  timer_t              = 50;
    int  db_timer             = 1000;
    int  wait_t               = 6000;
    int  db_id;
    int  sleep_time           = 0;
    int  sleep_time_interval  = 6;
    int  alter_repeat_time    = 5;

    bool is_detail       = false;
    bool is_dist         = false;
    bool is_alter        = false;
    bool is_H            = false;
    bool is_L            = false;
    bool is_M            = false;
    bool is_mode         = false;
    bool is_db_write     = false;
    bool is_finish_mode  = false;

    void virtual_keyboard_setting(); //가상 키보드 세팅 
    void mode_basic(); // 모드마다 공통적으로 들어가야하는 설정 
    void filter_kPa(); 

};
#endif // MAINWINDOW_H
