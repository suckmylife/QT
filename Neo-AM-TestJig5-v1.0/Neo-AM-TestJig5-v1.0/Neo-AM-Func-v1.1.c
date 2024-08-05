/*
 * Neo-AM-Func-v1.0.c
 *
 *  Created on: 2023. 11. 21.
 *      Author: admin
 */
#include <Neo-AM-Func-v1.1.h>



void control_init(void)
{
    //motor
    P5DIR |= BIT0 | BIT1 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7 | BIT4;
    VALVE_OUT1_Low();

}

