/*=======================================================================================
//  Title:      Neo-SM-v1.1
//  MCU:        MSP430F149
//  History: 2023.09.13 ; Neo-SM-v1.0.h
 *           2023.09.22 ; Neo-SM-v1.1.h
 *                        . 모드 관련 flag 추가, 모드관련 카운트 변수 추가
 *                        . 버튼관련 flag 추가
 *           2023.10.04 ; Neo-SM-v1.4.h
 *                        . 헤더 중복 방지를 위해 일부 코드 Neo-SM-co.h로 분리
 *                        . 압력값 관련 ReadCount -> float_temp 개별화 함. 1,2,3으로
=======================================================================================*/
#ifndef NEO_SM_V2_1_1_H
#define NEO_SM_V2_1_1_H

#include <msp430x14x.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


typedef unsigned char   UBYTE;
typedef unsigned int    UINT;
typedef unsigned long   ULONG;

bool    beep3_en   = false;
bool    beep2_en   = false;
bool    beep1_en   = false;

bool    is_pump          = false;
bool    is_right_valve   = false;
bool    is_left_valve    = false;
bool    is_center_valve  = false;
bool    is_out_valve     = false;

bool test_toggle = false;
uint8_t beep3_seq  = 0;
uint8_t beep2_seq  = 0;
uint8_t beep1_seq  = 0;

UINT    waiting_tm = 0;

#define CLK_11MHz       11059200

UINT    cnt_1ms   = 0;          // Timer용 Counter
UINT    cnt_5ms   = 0;
UINT    cnt_20ms  = 0;
UINT    cnt_50ms  = 0;
UINT    cnt_100ms = 0;
UINT    cnt_250ms = 0;
UINT    cnt_500ms = 0;
UINT    cnt_1sec  = 0;

UINT    beep_300ms = 0;
UINT    beep_100ms = 0;

UBYTE   f_1ms   = 0;            // Timer interrupt에 의해 SET 되는 Flag들
UBYTE   f_5ms   = 0;
UBYTE   f_20ms  = 0;
UBYTE   f_50ms  = 0;
UBYTE   f_100ms = 0;
UBYTE   f_250ms = 0;
UBYTE   f_500ms = 0;
UBYTE   f_1sec  = 0;

UINT cnt_auto       = 0; //자동모드용 Counter
UINT cnt_basic_F    = 0; //기본모드용 Counter
UINT cnt_basic_L    = 0; //기본모드용 Counter
UINT cnt_auto_mode  = 0; //자동모드 횟수 Counter
UINT cnt_auto_basic = 0; //자동모드의 기본기능 Counter
UINT cnt_position   = 0; //자제조정의 Counter
UINT cnt_cal_pos    = 0; //자세조정 10초마다 계산 Counter
UINT cnt_low_pos    = 0; //자세조정 led 끄기 Counter

//모드전환 및 공기량 조절 버튼용 Flag들
UBYTE is_auto  = 0;
UBYTE is_air_F = 0;
UBYTE is_air_L = 0;
UBYTE is_basic = 0;
UBYTE is_power = 0;
UBYTE is_auto_basic = 0;

UBYTE Air_btn  = 0;
UBYTE B_btn    = 0;
UBYTE A_btn    = 0;

UBYTE   rcv_address;

uint8_t DisBuf1[9];

#define PWM1                0x88
#define PWM2                0x89
#define PWM4                0x8A
#define PWM10               0x8B
#define PWM14               0x8F

uint8_t ch_table[]={
    0x3F, //"0"
    0x06, //"1"
    0x5B, //"2"
    0x4F, //"3"
    0x66, //"4"
    0x6D, //"5"
    0x7D, //"6"
    0x07, //"7"
    0x7F, //"8"
    0x6F, //"9"
    0x77, //"A"
    0x7C, //"B"
    0x39, //"C"
    0x5E, //"D"
    0x79, //"E"
    0x71, //"F"
    0x76, //"H"
    0x38, //"L"
    0x37, //"n"
    0x3E, //"u"
    0x73, //"P"
    0x5C, //"o"
    0x40, //"-"
    0x00, //off
};

uint16_t P1_Value        = 0;
uint16_t P2_Value        = 0;
uint16_t P3_Value        = 0;
uint16_t aa       = 0;
uint16_t bb        = 0;
uint16_t cc        = 0;
uint16_t position_abs    = 0;
uint16_t position_max    = 0;
float position_result = 0;

uint16_t uint16_temp;

uint8_t P1_Buf[3];
uint8_t P2_Buf[3];
uint8_t P3_Buf[3];
uint8_t Addr_Buf[3];

//처음진입했는지에 대한 flag
bool is_first          = false; //전원버튼 클릭 후 동작 추가
bool is_charging_first = true; //충전어댑터 꽂은 후 동작 추가

//모드를 진입했는지에 대한 flag
bool is_charging_mode = false;
bool is_init_connect  = true;
bool is_init_beep     = true;
bool is_position      = false;
bool is_high_pos      = false;
bool is_low_pos       = false;
//전압 이벤트 flag
bool is_first_6_4     = false;
bool is_under_6_4     = false;
bool is_1sec_led      = false;
bool is_poweroff_5min = false;

//압력읽기 flag
bool P_Read_Flag   = false;
bool f_rcv_proc    = false;
bool init_pressure = false;

//LED
#define air_low_led BIT6   //LED2
#define air_full_led BIT7  //LED3
#define position_led BIT3  //LED4
#define charged_led BIT4   //LED5
#define charging_led BIT5  //LED6
#define basic_led BIT6     //LED7
#define auto_led BIT7      //LED8

//전압
float battery_voltage = 0.0;
uint16_t adc_value    = 0;

//베터리확인
#define STAT1 BIT2
#define STAT2 BIT3

#define ADDO1   BIT0
#define ADSK1   BIT1
#define ADDO2   BIT2
#define ADSK2   BIT3
#define ADDO3   BIT4
#define ADSK3   BIT5
#define ADDO4   BIT6
#define ADSK4   BIT7
#define ADDO5   BIT0
#define ADSK5   BIT1
#define ADDO6   BIT2
#define ADSK6   BIT3
#define ADDO7   BIT4
#define ADSK7   BIT5
#define ADDO8   BIT6
#define ADSK8   BIT7

float Max_Pressure = 40.0;
float Min_Pressure = 0.0;
float Pressure_KPA;

//압력값 관련 ReadCount
float float_temp1;
float float_temp2;
float float_temp3;

// 압력센서 초기값 변수
uint16_t P_Init_Val1 = 204;
uint16_t P_Init_Val2 = 204;
uint16_t P_Init_Val3 = 204;
uint16_t P_Init_Val4 = 204;
uint16_t P_Init_Val5 = 204;
uint16_t P_Init_Val6 = 204;
uint16_t P_Init_Val7 = 204;
uint16_t P_Init_Val8 = 204;

//스위치
uint8_t     Init_in_stat;
uint8_t     Init_input_stat;
uint8_t     Init_old_input_stat;
uint16_t    Init_zero_count = 0;
uint8_t     Init_key_counter=0;

uint8_t     Init_in_stat1;
uint8_t     Init_input_stat1;
uint8_t     Init_old_input_stat1;
uint16_t    Init_zero_count1 = 0;
uint8_t     Init_key_counter1=0;

uint8_t     Init_in_stat2;
uint8_t     Init_input_stat2;
uint8_t     Init_old_input_stat2;
uint16_t    Init_zero_count2 = 0;
uint8_t     Init_key_counter2=0;

uint8_t     Init_in_stat3;
uint8_t     Init_input_stat3;
uint8_t     Init_old_input_stat3;
uint16_t    Init_zero_count3 = 0;
uint8_t     Init_key_counter3=0;

//블루투스 모드용 스위치 변수
uint8_t     blt_input_stat1;
uint8_t     blt_input_stat2;
uint8_t     blt_input_stat3;
uint8_t     blt_in_stat1;
uint8_t     blt_in_stat2;
uint8_t     blt_in_stat3;
uint8_t     blt_old_input_stat1;
uint8_t     blt_old_input_stat2;
uint8_t     blt_old_input_stat3;
uint16_t    blt_zero_count1 = 0;
uint8_t     blt_key_counter1=0;

//압력초기화 버튼
uint8_t     press_input_stat1;
uint8_t     press_input_stat2;
uint8_t     press_in_stat1;
uint8_t     press_in_stat2;
uint8_t     press_old_input_stat1;
uint8_t     press_old_input_stat2;
uint16_t    press_zero_count1 = 0;
uint8_t     press_key_counter1=0;

//스위치 포트
#define POWER_SW BIT0 //P1.0
#define DEFAULT_MODE BIT4// 0x10 //P1.4
#define AUTO_MODE BIT5//0x08 //P1.5
#define AIR_VOLUME BIT6//0x04 //P1.6

uint8_t bf_data[17];
uint8_t free_byte0;
uint8_t p1_1;
uint8_t p1_2;
uint8_t p3_1;
uint8_t p3_2;
unsigned int p1_value_dec;
unsigned int p3_value_dec;
UBYTE   rcv_buf[80] ={0};
UBYTE   first_flag = 0;
UBYTE   f_rcv_start = 0;
UBYTE   f_rcv_cnt = 0;
UBYTE   rcv_data;
UBYTE   rcv_cnt = 0;
UBYTE   *p_rcv_buf;
UBYTE   rcv_end_cnt = 0;
UBYTE   rcv_start_cnt = 0;
UBYTE   compare_cnt;

uint8_t rcv_command;

UBYTE   trs_buf0[100] = {0};

#define RX232_On()  P3OUT&=~BIT0
#define TX232_On()  P3OUT|=BIT0

#define IN_SENSE BIT1//어댑터 포트

#endif // NEO_SM_V1_0_H

