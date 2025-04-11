#include "mode.h"

mode::mode(QObject *parent) : QObject(parent)
{
    cmd = new command();
    timer = new QTimer();
}

mode::~mode()
{
    delete cmd;
    delete timer;
}

void mode::all_out_close()
{
    cmd->out_all_c();
}

void mode::all_out_open()
{
    cmd->out_all_o();
}

void mode::all_in_close()
{
    cmd->in_all_c();
}

void mode::all_in_open()
{
    cmd->in_all_o();
}

void mode::all_in_close(int num)
{
    cmd->in_all_c(num);
}

void mode::all_in_open(int num)
{
    cmd->in_all_o(num);
}

void mode::finish_mode(int num)
{
    time_cnt++;
    if(time_cnt>0 and time_cnt < finish_1_start)
    {
        all_in_open(cmd_cnt);
    }
    else if(time_cnt >= finish_2_start and time_cnt < finish_3_start)
    {
        all_in_close(cmd_cnt);
    }
    else if(time_cnt >= finish_3_start)
    {
        set_end(true);
    }
    cmd_cnt++;
    if(cmd_cnt>27)
    {
        cmd_cnt = 0;
    }
}

void mode::pump_on()
{
    cmd->start_pump();
}

void mode::pump_off()
{
    cmd->end_pump();
}

void mode::Cal_L()
{
   cmd->cali_l();
}

void mode::Cal_H()
{
    cmd->cali_h();
}

int mode::get_alter_circu()
{
    return alter_circu_cnt;
}

bool mode::get_dist()
{
    return dist_end;
}

bool mode::get_L2()
{
    return l2_end;
}

bool mode::get_H2()
{
    return h2_end;
}

bool mode::get_M2()
{
    return m2_end;
}

bool mode::get_end()
{
    return end_end;
}

void mode::set_dist(bool b)
{
    this->dist_end = b;
}

void mode::set_H2(bool b)
{
    this->h2_end = b;
}

void mode::set_L2(bool b)
{
    this->l2_end = b;
}

void mode::set_M2(bool b)
{
    this->m2_end = b;
}

void mode::set_end(bool b)
{
    this->end_end = b;
}

void mode::set_alter_circu(int cnt)
{
    this->alter_circu_cnt = cnt;
}

void mode::set_time_cnt(int cnt) //init
{
    this->time_cnt = cnt;
    this->cmd_cnt  = 0;
    is_5min = false;
    alter_start = false;
}

void mode::alter_mode(int num)
{
    if(alter_start)
    {
        if(time_cnt >=0 and time_cnt < alter_1_start)
        {
            cmd->alter_2_1(cmd_cnt);// 1_2 out valve open during 30sec
            time_cnt++;
        }
        else if(time_cnt >= alter_1_start and time_cnt < alter_2_start)
        {
            cmd->alter_2_2(cmd_cnt); // 1_2 out valve close
            time_cnt++;
        }
        else if(time_cnt == alter_2_start)
        {
            time_cnt = alter_2_start + 1;
            timer->singleShot(1000,[=](){
                   is_5min = true;// rest 5 min
            });
        }
        else if(time_cnt == alter_6_start)
        {
            time_cnt = alter_6_start + 1;
            is_5min = false;
            timer->singleShot(1000,[=](){
                   is_5min = true;// rest 5 min
            });
        }
        if(is_5min)
        {
            time_cnt++;
            if(time_cnt > alter_2_start and time_cnt < alter_3_start)
            {
                cmd->alter_3_1(cmd_cnt); // 1_2 in valve open pump start
            }
            else if(time_cnt >= alter_3_start and time_cnt < alter_4_start)
            {
                cmd->alter_3_2(cmd_cnt); // 1_2 in valve close pump stop
            }
            else if(time_cnt >= alter_4_start and time_cnt < alter_5_start)
            {
                cmd->alter_4_1(cmd_cnt); // 3_4 out valve open during 30sec
            }
            else if(time_cnt >= alter_5_start and time_cnt < alter_6_start)
            {
                cmd->alter_4_2(cmd_cnt); // 3_4 out valve close
            }
            else if(time_cnt > alter_6_start and time_cnt < alter_7_start)
            {
                cmd->alter_5_1(cmd_cnt); //3_4 in valve open pump start
            }
            else if(time_cnt >= alter_7_start and time_cnt < alter_8_start)
            {
                cmd->alter_5_2(cmd_cnt); //3_4 in valve close pump stop
            }
            else if(time_cnt >= alter_8_start and time_cnt < alter_9_start)
            {
                alter_circu_cnt++;
                set_alter_circu(alter_circu_cnt);
                time_cnt = 0;
            }
        }
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            cmd_cnt = 0;
        }
    }
    else
    {
        if(get_dist())
        {
            alter_start = true;
            time_cnt    = 0;
            set_dist(false);
        }
        if(!alter_start and !get_dist() )
        {
            dist_mode(num);
        }
    }
}

void mode::dist_mode(int num)
{
    int cnt = time_cnt++;
    if(cnt > dist_out_close and cnt <= dist_in_open and !get_dist())
    {
        cmd->out_all_c(cmd_cnt);
        addr_contain.clear();
        qDebug() << "1 :" <<cnt << "\n";
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            dist_basic_cnt = 1;
            cmd_cnt        = 0;
        }
    }
    else if(cnt > dist_in_open and cnt < dist_in_close)
    {
        cmd->in_all_o(cmd_cnt);
        cmd->start_pump();
        qDebug() << "2 :" <<cnt << "\n";
        dist_basic_cnt = 2;
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            //dist_basic_cnt = 2;
            cmd_cnt        = 0;
        }
    }
    else if(cnt >= dist_in_close and cnt <= dist_end_time)
    {
        set_dist_close(true);
        cmd->in_all_c(cmd_cnt);
        //qDebug() << "3 :" <<cnt << "\n";
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            cmd_cnt        = 0;
            cmd->end_pump();
        }
    }
    else if(cnt > dist_end_time)
    {
        set_dist(true);
        set_dist_close(false);
        //qDebug() << "dist END" << "\n";
        dist_basic_cnt = 0;
        time_cnt = 0;
    }
}

void mode::set_dist_close(bool close)
{
    this->dist_start_close = close;
}

bool mode::get_dist_close()
{
     return dist_start_close;
}

void mode::dist_H_mode(int num)
{
    if(get_dist())
    {
        alter_start = true;
        time_cnt    = 0;
        set_dist(false);
    }
    if(alter_start == 0 and !get_dist() )
    {
        dist_mode(num);
    }
    if(alter_start)
    {
        if(time_cnt >= 0 and time_cnt < dist_1_start)
        {
            cmd->H_1(cmd_cnt);
            time_cnt++;
        }
        else if(time_cnt >= dist_1_start and time_cnt < dist_2_start)
        {
            cmd->H_2(cmd_cnt);
            time_cnt++;
        }
        else if(time_cnt >= dist_2_start)
        {
            set_H2(true);
        }
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            cmd_cnt = 0;
        }
    }
}

void mode::dist_L_mode(int num)
{
    if(get_dist())
    {
        alter_start = true;
        time_cnt    = 0;
        set_dist(false);
    }
    if(alter_start == 0 and !get_dist() )
    {
        dist_mode(num);
    }
    if(alter_start)
    {
        if(time_cnt >= 0 and time_cnt < dist_1_start)
        {
            cmd->L_1(cmd_cnt);
            time_cnt++;
        }
        else if(time_cnt >= dist_1_start and time_cnt < dist_2_start)
        {
            cmd->L_2(cmd_cnt);
            time_cnt++;
        }
        else if(time_cnt >= dist_2_start)
        {
            set_L2(true);
        }
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            cmd_cnt = 0;
        }
    }
}

void mode::dist_M_mode(int num)
{
    if(get_dist())
    {
        alter_start = true;
        time_cnt    = 0;
        set_dist(false);
    }
    if(alter_start == 0 and !get_dist() )
    {
        dist_mode(num);
    }
    if(alter_start)
    {
        if(time_cnt >= 0 and time_cnt < dist_1_start)
        {
            cmd->M_1(cmd_cnt);
            time_cnt++;
        }
        else if(time_cnt >= dist_1_start and time_cnt < dist_2_start)
        {
            cmd->M_2(cmd_cnt);
            time_cnt++;
        }
        else if(time_cnt >= dist_2_start)
        {
            set_M2(true);
        }
        cmd_cnt++;
        if(cmd_cnt > 27)
        {
            cmd_cnt = 0;
        }
    }
}
