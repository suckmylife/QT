/*=======================================================================================
//  Title:      Neo-SM-v1.1
//  MCU:        MSP430F149
//  History: 2023.09.22 ; Neo-SM-Mode-v1.1.h
 *            모드관련 시간 맵핑함수 선언
 * 1.8.6 power off 추가
=======================================================================================*/
//
#ifndef NEO_SM_MODE_V2_1_1_H
#define NEO_SM_MODE_V2_1_1_H

#include <msp430x14x.h>
#include <stdbool.h>


#define Left   BIT1;
#define Right  BIT3;
#define Center BIT2;
#define Out    BIT4;
#define Pump   BIT0;

//valve
#define Left_Valve_High()   P5OUT|=Left;
#define Left_Valve_Low()    P5OUT&=~Left;
#define Center_Valve_High() P5OUT|=Center;
#define Center_Valve_Low()  P5OUT&=~Center;
#define Right_Valve_High()  P5OUT|=Right;
#define Right_Valve_Low()   P5OUT&=~Right;
#define Out_Valve_High()    P5OUT|=Out;
#define Out_Valve_Low()     P5OUT&=~Out;

//pump
#define Pump_High() P5OUT|=Pump;
#define Pump_Low()  P5OUT&=~Pump;


void Power_Off();
void control_init();
void controlValves(bool pump, bool right_valve, bool left_valve, bool center_valve, bool out_valve);


#endif // NEO_SM_SM_MODE_V1_1_H
