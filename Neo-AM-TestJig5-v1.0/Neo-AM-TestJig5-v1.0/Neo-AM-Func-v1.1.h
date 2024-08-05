/*
 * Neo-AM-Func-v1.0.h
 *
 *  Created on: 2023. 11. 21.
 *      Author: admin
 */

#ifndef NEO_AM_FUNC_V1_1_H_
#define NEO_AM_FUNC_V1_1_H_

#include <msp430x14x.h>
#include <stdbool.h>


#define VALVE_IN1   BIT0;
#define VALVE_OUT1  BIT1;
#define VALVE_IN2   BIT2;
#define VALVE_OUT2  BIT3;
#define VALVE_IN3   BIT4;
#define VALVE_OUT3  BIT5;
#define VALVE_IN4   BIT6;
#define VALVE_OUT4  BIT7;

//valve
#define Valve_IN1_High()   P5OUT|=VALVE_IN1;
#define Valve_IN1_Low()    P5OUT&=~VALVE_IN1;
#define VALVE_OUT1_High()  P5OUT|=VALVE_OUT1;
#define VALVE_OUT1_Low()   P5OUT&=~VALVE_OUT1;

#define Valve_IN2_High()   P5OUT|=VALVE_IN2;
#define Valve_IN2_Low()    P5OUT&=~VALVE_IN2;
#define VALVE_OUT2_High()  P5OUT|=VALVE_OUT2;
#define VALVE_OUT2_Low()   P5OUT&=~VALVE_OUT2;

#define Valve_IN3_High()   P5OUT|=VALVE_IN3;
#define Valve_IN3_Low()    P5OUT&=~VALVE_IN3;
#define VALVE_OUT3_High()  P5OUT|=VALVE_OUT3;
#define VALVE_OUT3_Low()   P5OUT&=~VALVE_OUT3;

#define Valve_IN4_High()   P5OUT|=VALVE_IN4;
#define Valve_IN4_Low()    P5OUT&=~VALVE_IN4;
#define VALVE_OUT4_High()  P5OUT|=VALVE_OUT4;
#define VALVE_OUT4_Low()   P5OUT&=~VALVE_OUT4;


#define IN1_ValveRUN        P5OUT|=VALVE_IN1;
#define IN1_ValveSTOP       P5OUT&=~VALVE_IN1;
#define OUT1_ValveRUN       P5OUT|=VALVE_OUT1;
#define OUT1_ValveSTOP      P5OUT&=~VALVE_OUT1;

#define IN2_ValveRUN        P5OUT|=VALVE_IN2;
#define IN2_ValveSTOP       P5OUT&=~VALVE_IN2;
#define OUT2_ValveRUN       P5OUT|=VALVE_OUT2;
#define OUT2_ValveSTOP      P5OUT&=~VALVE_OUT2;

#define IN3_ValveRUN        P5OUT|=VALVE_IN3;
#define IN3_ValveSTOP       P5OUT&=~VALVE_IN3;
#define OUT3_ValveRUN       P5OUT|=VALVE_OUT3;
#define OUT3_ValveSTOP      P5OUT&=~VALVE_OUT3;

#define IN4_ValveRUN        P5OUT|=VALVE_IN4;
#define IN4_ValveSTOP       P5OUT&=~VALVE_IN4;
#define OUT4_ValveRUN       P5OUT|=VALVE_OUT4;
#define OUT4_ValveSTOP      P5OUT&=~VALVE_OUT4;




void control_init();

#endif /* NEO_AM_FUNC_V1_1_H_ */
