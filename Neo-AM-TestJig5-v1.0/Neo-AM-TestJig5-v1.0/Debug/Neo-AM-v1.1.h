/*=======================================================================================
//  Title:      Neo-AM-v1.0.h
//  MCU:        MSP430F149
//  History: 2023.11.21 ; Neo-AM-v1.0.h
=======================================================================================*/
typedef unsigned char   UBYTE;
typedef unsigned int    UINT;
typedef unsigned long   ULONG;
#include <math.h>

#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#define CLK_11MHz       11059200
#define POWER_LED       BIT7;           // P1.7
//P2
#define IN3_1 BIT0
#define IN3_2 BIT1
#define OUT3_1 BIT2
#define OUT3_2 BIT3
#define IN4_1 BIT4
#define IN4_2 BIT5
#define OUT4_1 BIT6
#define OUT4_2 BIT7
//P6
#define IN1_1 BIT0
#define IN1_2 BIT1
#define OUT1_1 BIT2
#define OUT1_2 BIT3
#define IN2_1 BIT4
#define IN2_2 BIT5
#define OUT2_1 BIT6
#define OUT2_2 BIT7
//P1
#define RESET BIT5

UINT    cnt_1ms=0;          // Timer용 Counter
UINT    cnt_5ms=0;
UINT    cnt_10ms=0;
UINT    cnt_20ms=0;
UINT    cnt_50ms=0;
UINT    cnt_100ms=0;
UINT    cnt_200ms=0;
UINT    cnt_250ms=0;
UINT    cnt_500ms=0;
UINT    cnt_2sec=0;
UINT    cnt_1sec=0;
UINT    cnt_5sec=0;
UINT    cnt_10sec=0;
UINT    cnt_30sec=0;

UBYTE   f_1ms=0;            // Timer interrupt에 의해 SET 되는 Flag들
UBYTE   f_5ms=0;
UBYTE   f_10ms=0;
UBYTE   f_20ms=0;
UBYTE   f_50ms=0;
UBYTE   f_100ms=0;
UBYTE   f_200ms=0;
UBYTE   f_250ms=0;
UBYTE   f_500ms=0;
UBYTE   f_1sec=0;
UBYTE   f_2sec=0;
UBYTE   f_5sec=0;
UBYTE   f_10sec=0;
UBYTE   f_30sec=0;

UINT    waiting_tm = 0;

UBYTE   Addr_485;
UBYTE   rcv_address;

#define WRITE_DATA_MODE_Z   0x40 // add address automatically
#define WRITE_DATA_MODE_G   0x44 // fixed address
#define START_DATA          0xC0 // Display Address : 00H
#define DISPLAY_EN          0x8A // open display
#define DISPLAY_DIS         0x80 // off display
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

uint16_t P1_Value = 0;
uint16_t P2_Value = 0;
uint16_t P3_Value = 0;
uint16_t P4_Value = 0;

uint16_t uint16_temp;

uint8_t P1_Buf[3];
uint8_t P2_Buf[3];
uint8_t P3_Buf[3];
uint8_t P4_Buf[3];
uint8_t Addr_Buf[3];

unsigned long P1_adc_value;
unsigned long P2_adc_value;
unsigned long P3_adc_value;
unsigned long P4_adc_value;

#define ADDO1   BIT0
#define ADSK1   BIT1
#define ADDO2   BIT2
#define ADSK2   BIT3
#define ADDO3   BIT4
#define ADSK3   BIT5
#define ADDO4   BIT6
#define ADSK4   BIT7

float Max_Pressure = 40.0;
float Min_Pressure = 0.0;
float Pressure_KPA;

bool P_Read_Flag = false;

float float_temp1;
// 압력센서 초기값 변수
uint16_t P_Init_Val1 = 204;
uint16_t P_Init_Val2 = 204;
uint16_t P_Init_Val3 = 204;
uint16_t P_Init_Val4 = 204;

bool sensor_init_en = false;
bool pump_on = false;
uint8_t     Init_in_stat;
uint8_t     Init_input_stat;
uint8_t     Init_old_input_stat;
uint8_t     Init_old_input_stat;
uint16_t    Init_zero_count = 0;
uint8_t     Init_key_counter=0;

#define Init_SW_Port   0x10 // P1.4

uint8_t bf_data[17];
uint8_t free_byte0;

UBYTE   rcv_buf1[80] ={0};
UBYTE   first_flag1 = 0;
UBYTE   f_rcv_start1 = 0;
UBYTE   f_rcv_cnt1 = 0;
UBYTE   rcv_data1;
UBYTE   rcv_cnt1 = 0;
UBYTE   *p_rcv_buf1;
UBYTE   rcv_end_cnt1 = 0;
UBYTE   rcv_start_cnt1 = 0;
UBYTE   compare_cnt1;
bool f_rcv_proc1 = false;

uint8_t rcv_command;
uint8_t rcv_command1;

UBYTE   trs_buf0[100] = {0};
UBYTE   trs_buf1[100] = {0};
UBYTE   trs_buf2[100] = {0};

#define RX485_On()  P3OUT&=~BIT5
#define TX485_On()  P3OUT|=BIT5


