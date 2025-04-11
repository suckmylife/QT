#include "command.h"

command::command(QObject *parent)
    :QObject(parent)
{
    //serialports = new communication();
    if(gpioInitialise()<0)
    {
        qDebug() << "cannot gpio";
    }
    gpioSetMode(PUMP_NUMBER, PI_OUTPUT);
    for(int i=1; i<=15; ++i)
    {
        numbers.append(QString("%1").arg(i,2,16,QChar('0')).toUpper());
    }
    for(int i=16; i<=28; ++i)
    {
        numbers.append(QString::number(i,16).toUpper());
    }
    valve_command ={
    //invalve open
    {"in_all_o", "7EE7054D01830FFF"},
    {"in_1_2_o", "7EE7054D018305FF"},
    {"in_3_4_o", "7EE7054D01830AFF"},

    //invalve close
    {"in_all_c", "7EE7054D01850FFF"},
    {"in_1_2_c", "7EE7054D018505FF"},
    {"in_3_4_c", "7EE7054D01850AFF"},
    //    {"",""},

    //outvalve open
    {"out_all_o", "7EE7054D01841FFF"},
    {"out_1_o", "7EE7054D018411FF"},
    {"out_1_2_o", "7EE7054D018415FF"},
    {"out_1_2_3_o", "7EE7054D01841BFF"},
    {"out_3_4_o", "7EE7054D01841AFF"},

    //outvalve close
    {"out_all_c", "7EE7054D01861FFF"},
    {"out_1_c", "7EE7054D018611FF"},
    {"out_1_2_c", "7EE7054D018615FF"},
    {"out_1_2_3_c", "7EE7054D01861BFF"},
    {"outall_c", "7EE7054DF58804FF"},
    {"out_3_4_c", "7EE7054D01861AFF"},
    //nothing
    {"nothig", "7EE7000000000000"}
    };

    develop_command ={
    {"Cali_L",    "7EE7054D01870102"},
    {"Cali_H",    "7EE7054D01870214"},
    {"Normal_Read", "7EE7054D018703FF"},
    {"Cali_Read", "7EE7054D018704FF"}
    };
    timer = new QTimer();
    //serials = communication::getInstance();
}

command::~command()
{
    delete timer;
}

void command::out_all_o()
{
    for(int hex_ = 0; hex_<28; hex_++)
    {
        QString h = numbers[hex_];
        send_cmd(valve_command["out_all_o"].replace(hex_spot,hex_len2,h));
    }
}

void command::out_all_o(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_all_o"].replace(hex_spot,hex_len2,h));
}

void command::cali_l()
{
    //qDebug() << "callibration";
    for(int hex_ = 0; hex_<28; hex_++)
    {
        QString h = numbers[hex_];
        send_cmd(develop_command["Cali_L"].replace(hex_spot,hex_len2,h));
    }
    timer->singleShot(plus_time,[=](){
        qDebug() << "callibration read";
        for(int hex_ = 0; hex_<28; hex_++)
        {
            QString h = numbers[hex_];
            send_cmd(develop_command["Cali_Read"].replace(hex_spot,hex_len2,h));
        }
    });
}

void command::cali_h()
{
    //qDebug() << "callibration high cali";
    for(int hex_ = 0; hex_<28; hex_++)
    {
        QString h = numbers[hex_];
        send_cmd(develop_command["Cali_H"].replace(hex_spot,hex_len2,h));
    }
    timer->singleShot(plus_time,[=](){
        for(int hex_ = 0; hex_<28; hex_++)
        {
            QString h = numbers[hex_];
            send_cmd(develop_command["Cali_Read"].replace(hex_spot,hex_len2,h));
        }
    });
}

void command::send_cmd(QString cmd)
{
    //serialports->send_cmd(cmd);
    communication::instance()->send_cmd(cmd);
}

void command::out_all_c()
{
    for(int hex_ = 0; hex_<28; hex_++)
    {
        QString h = numbers[hex_];
        send_cmd(valve_command["out_all_c"].replace(hex_spot,hex_len2,h));
    }
}

void command::out_all_c(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_all_c"].replace(hex_spot,hex_len2,h));
}

void command::start_pump()
{
    gpioSetMode(PUMP_NUMBER, PI_OUTPUT);
    gpioWrite(PUMP_NUMBER,1);
}

void command::end_pump()
{
    gpioSetMode(PUMP_NUMBER, PI_OUTPUT);
    gpioWrite(PUMP_NUMBER,0);
}

void command::in_all_c()
{
    for(int hex_ = 0; hex_<28; hex_++)
    {
        QString h = numbers[hex_];
        send_cmd(valve_command["in_all_c"].replace(hex_spot,hex_len2,h));
    }
}

void command::in_all_c(int num)
{
    QString h = numbers[num];
    //qDebug() << "In All Close : " << num;
    send_cmd(valve_command["in_all_c"].replace(hex_spot,hex_len2,h));
}

void command::in_all_o()
{
    for(int hex_ = 0; hex_<28; hex_++)
    {
        QString h = numbers[hex_];
        send_cmd(valve_command["in_all_o"].replace(hex_spot,hex_len2,h));
    }
}

void command::in_all_o(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["in_all_o"].replace(hex_spot,hex_len2,h));
}

void command::alter_2_1(int num)
{
    qDebug() << "2-1";
    QString h = numbers[num];
    send_cmd(valve_command["out_1_2_o"].replace(hex_spot,hex_len2,h));
}

void command::alter_2_2(int num)
{
    qDebug() << "2-2";
    QString h = numbers[num];
    send_cmd(valve_command["out_1_2_c"].replace(hex_spot,hex_len2,h));
}

void command::alter_3_1(int num)
{
    qDebug() << "3-1";
    start_pump();
    QString h = numbers[num];
    send_cmd(valve_command["in_1_2_o"].replace(hex_spot,hex_len2,h));
}

void command::alter_3_2(int num)
{
    qDebug() << "3-2";
    QString h = numbers[num];
    send_cmd(valve_command["in_1_2_c"].replace(hex_spot,hex_len2,h));
    end_pump();
}

void command::alter_4_1(int num)
{
    qDebug() << "4-1";
    QString h = numbers[num];
    send_cmd(valve_command["out_3_4_o"].replace(hex_spot,hex_len2,h));
}

void command::alter_4_2(int num)
{
    qDebug() << "4-2";
    QString h = numbers[num];
    send_cmd(valve_command["out_3_4_c"].replace(hex_spot,hex_len2,h));
}

void command::alter_5_1(int num)
{
    qDebug() << "5-1";
    start_pump();
    QString h = numbers[num];
    send_cmd(valve_command["in_3_4_o"].replace(hex_spot,hex_len2,h));
}

void command::alter_5_2(int num)
{
    qDebug() << "5-2";
    QString h = numbers[num];
    send_cmd(valve_command["in_3_4_c"].replace(hex_spot,hex_len2,h));
    end_pump();
}

void command::H_1(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_1_o"].replace(hex_spot,hex_len2,h));
}

void command::H_2(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_1_c"].replace(hex_spot,hex_len2,h));
}

void command::M_1(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_1_2_o"].replace(hex_spot,hex_len2,h));
}

void command::M_2(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_1_2_c"].replace(hex_spot,hex_len2,h));
}

void command::L_1(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_1_2_3_o"].replace(hex_spot,hex_len2,h));
}

void command::L_2(int num)
{
    QString h = numbers[num];
    send_cmd(valve_command["out_1_2_3_c"].replace(hex_spot,hex_len2,h));
}
