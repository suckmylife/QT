/*=======================================================================================
//  Title:      Neo-SM-v1.1
//  MCU:        MSP430F149
//  History: 2023.09.22 ; Neo-SM-Mode-v1.1.c
 *            모드관련 시간 맵핑
 *
=======================================================================================*/
#include <Neo-SM-Mode-v2.1.9.h>
void control_init(void)
{
    //pump, valve
    P5DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT4;

    //pump초기화
    P5OUT &= ~BIT0;
}
void Power_Off(void)
{
     Pump_Low();
     Left_Valve_Low();
     Center_Valve_Low();
     Right_Valve_Low();
     Out_Valve_Low();
}

void controlValves(bool pump, bool right_valve, bool left_valve, bool center_valve, bool out_valve) {
    if (pump) {
        Pump_High();
    } else {
        Pump_Low();
    }

    if (right_valve) {
        Right_Valve_High();
    } else {
        Right_Valve_Low();
    }

    if (left_valve) {
        Left_Valve_High();
    } else {
        Left_Valve_Low();
    }

    if (center_valve) {
        Center_Valve_High();
    } else {
        Center_Valve_Low();
    }

    if (out_valve) {
        Out_Valve_High();
    } else {
        Out_Valve_Low();
    }
}


/////////////////////



