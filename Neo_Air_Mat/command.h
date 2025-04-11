#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QSerialPort>
#include <pigpio.h>
#include <QSet>
#include <QString>
#include <QList>
#include <iostream>

#include "communication.h"
//#include "request.h"

#define PUMP_NUMBER 23
using namespace std;

class command : public QObject
{
    Q_OBJECT

public:
    explicit command(QObject *parent = nullptr);
    ~command();

    void start_pump();
    void end_pump();
    void cali_l();
    void cali_h();

    void in_all_o();
    void in_all_o(int num);
    void out_all_o();
    void out_all_o(int num);
    void in_all_c(int num);
    void in_all_c();
    void out_all_c();
    void out_all_c(int num);
    void alter_2_1(int num);
    void alter_2_2(int num);
    void alter_3_1(int num);
    void alter_3_2(int num);
    void alter_4_1(int num);
    void alter_4_2(int num);
    void alter_5_1(int num);
    void alter_5_2(int num);
    void H_1(int num);
    void H_2(int num);
    void M_1(int num);
    void M_2(int num);
    void L_1(int num);
    void L_2(int num);

private:
    QMap<QString,QString> valve_command;
    QMap<QString,QString> develop_command;
    int hex_len  = 1;
    int hex_len2 = 2;
    int hex_spot = 8;
    const int plus_time = 2000;
    const int base_time = 3500;
    const int sec_30    = 30000;
    const int min_5     = 300000;

    QTimer *timer;

    void send_cmd(QString cmd);

    bool is_5min = false;
    bool alter_3_1_end = false;
    bool alter_3_1_start = false;
    QSet<int> container;
    QList<QString> numbers;
};

#endif // COMMAND_H
