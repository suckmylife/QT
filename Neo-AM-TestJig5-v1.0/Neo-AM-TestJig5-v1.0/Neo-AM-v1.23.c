/*=======================================================================================
//  Title:          Neo-AM-v1.0.c
//  Description:    이 프로그램은 네오에이블의 Air Matttress를 시험하기 위한 Jig 프로그램이다.
//  MCU:            MSP430F149, 11.0592 MHz XT2 External Crystal
//  PCB :           Neo-AM-v1.0
//  Author:         song ye lim
//  Start Date:     2023-11-21
//  SW Tool:        Code Composer Studio(10.4.0.00006)
//  History
//      2023.06.13 ; Program Start(AM-TestJig-v1.0)
//                   . Clock 초기화, IO Port 초기화, Timer 초기화, UART 초기화
//                   . Address Get
//                   . 압력값을 FND에 표시
// * 12-05 포토 인터럽트 센서 실험
// *
// *
=======================================================================================*/
#include <msp430.h>
#include <msp430x14x.h>
#include <Neo-AM-Func-v1.1.h>
#include <Neo-AM-v1.2.h>

void clock_init(void);
void IOPort_init(void);
void Timer_B7_init(void);
void UART_Init(void);
void control_init();
uint8_t Addr_Get(void);

// ADC
unsigned long ReadCount1(void);
unsigned long ReadCount2(void);
unsigned long ReadCount3(void);
unsigned long ReadCount4(void);
void Init_Input(void);
void sensor_init(void);
void flash_write(void);
void flash_read(void);
void flashData_init(void);
void rcv_proc1(void);
void UCA1_strout_length(UBYTE *tx_str, UBYTE length);
void UCA1_chout(unsigned char tx_ch);
void Pressure_Data_Send1(void);
void pressureCount(void);
void flash_24bit_write(void);
void flash_24bit_read(void);

int main(void){

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    //WDTCTL = WDTPW + WDTHOLD;
    P1OUT &= ~BIT7;     // P1.7을 low 설정
    P1DIR |= (BIT7);    // P1.7 방향을 출력으로 설정
    __no_operation();

    unsigned char i;
    for(i=0;i<5;i++){   //for reset Monitor
                    LED_TOGLE;
                    __delay_cycles(100000);     //0.1sec
                    LED_TOGLE;
                    __delay_cycles(100000);
                    }

    _DINT();            // disable global interrupts
    clock_init();
    IOPort_init();
    Timer_B7_init();
    UART_Init();
    //flashData_init();
    control_init();
    Addr_485 = Addr_Get();
    _EINT();        // enable global interrupts
    __no_operation();

    //flash_24bit_read();
    //flash_24bit_write();

/*
    IN_Close_M15();
    OUT_Close_M15();
    f_500ms=0;  while(!f_500ms);
*/
    IN_Open_M15();
    OUT_Open_M15();
    LED_TOGLE;
    f_500ms=0;  while(!f_500ms);

    IN_Open_M15();
    OUT_Open_M15();
    LED_TOGLE;


//----------------------------------------------------------
// rs485 Q&A Time=2~5mS x 30EA = 60mS ~ 150mS
//----------------------------------------------------------
    while(1)
    {
        if(f_rcv_proc1 == true) rcv_proc1();

//        if(P_Read_Flag == true){    // 압력센서 값 읽기
//                    P_Read_Flag = false;
//                    }
        __no_operation();

    if(INValve_OpFlag){INValve_OpFlag=false;  Cmd_Dat &= 0x0F;
        switch(Cmd_Dat){
            case 1: IN_Open_M1(); break;
            case 2: IN_Open_M2(); break;
            case 3: IN_Open_M3(); break;
            case 4: IN_Open_M4(); break;
            case 5: IN_Open_M5(); break;
            case 6: IN_Open_M6(); break;
            case 7: IN_Open_M7(); break;
            case 8: IN_Open_M8(); break;
            case 9: IN_Open_M9(); break;
            case 10:IN_Open_M10();break;
            case 11:IN_Open_M11();break;
            case 12:IN_Open_M12();break;
            case 13:IN_Open_M13();break;
            case 14:IN_Open_M14();break;
            case 15:IN_Open_M15();break;
            default:  break;
            }
           }
    if(INValve_ClFlag){INValve_ClFlag=false;  Cmd_Dat &= 0x0F;
       switch(Cmd_Dat){
           case 1: IN_Close_M1(); break;
           case 2: IN_Close_M2(); break;
           case 3: IN_Close_M3(); break;
           case 4: IN_Close_M4(); break;
           case 5: IN_Close_M5(); break;
           case 6: IN_Close_M6(); break;
           case 7: IN_Close_M7(); break;
           case 8: IN_Close_M8(); break;
           case 9: IN_Close_M9(); break;
           case 10:IN_Close_M10();break;
           case 11:IN_Close_M11();break;
           case 12:IN_Close_M12();break;
           case 13:IN_Close_M13();break;
           case 14:IN_Close_M14();break;
           case 15:IN_Close_M15();break;
           default:  break;
           }
          }
   if(OUTValve_OpFlag){OUTValve_OpFlag=false; Cmd_Dat &= 0x0F;
       switch(Cmd_Dat){
           case 1: OUT_Open_M1(); break;
           case 2: OUT_Open_M2(); break;
           case 3: OUT_Open_M3(); break;
           case 4: OUT_Open_M4(); break;
           case 5: OUT_Open_M5(); break;
           case 6: OUT_Open_M6(); break;
           case 7: OUT_Open_M7(); break;
           case 8: OUT_Open_M8(); break;
           case 9: OUT_Open_M9(); break;
           case 10:OUT_Open_M10();break;
           case 11:OUT_Open_M11();break;
           case 12:OUT_Open_M12();break;
           case 13:OUT_Open_M13();break;
           case 14:OUT_Open_M14();break;
           case 15:OUT_Open_M15();break;
           default:  break;
           }
          }

   if(OUTValve_ClFlag){OUTValve_ClFlag=false;  Cmd_Dat &= 0x0F;
       switch(Cmd_Dat){
           case 1: OUT_Close_M1(); break;
           case 2: OUT_Close_M2(); break;
           case 3: OUT_Close_M3(); break;
           case 4: OUT_Close_M4(); break;
           case 5: OUT_Close_M5(); break;
           case 6: OUT_Close_M6(); break;
           case 7: OUT_Close_M7(); break;
           case 8: OUT_Close_M8(); break;
           case 9: OUT_Close_M9(); break;
           case 10:OUT_Close_M10();break;
           case 11:OUT_Close_M11();break;
           case 12:OUT_Close_M12();break;
           case 13:OUT_Close_M13();break;
           case 14:OUT_Close_M14();break;
           case 15:OUT_Close_M15();break;
           default:  break;
           }
          }
   if(Motor_CmdFlag){Motor_CmdFlag=false;
       switch(Cmd_Dat){
           case 1: IN_Open_M15();   break;
           case 2: IN_Close_M15();  break;
           case 3: OUT_Open_M15();  break;
           case 4: OUT_Close_M15(); break;
           case 5: IN_Open_M15();   OUT_Open_M15();  break;
           case 6: IN_Close_M15();  OUT_Close_M15(); break;
           default:  break;
           }
          }

   if(Pressure_SetFlag){
                       Pressure_SetFlag=false;
                  char Pres_Cell_Num = Cmd_Dat; Pres_Cell_Num &= 0x0F;           //low  nibble = cell pressure

       switch(Pres_Cell_Num){                           //pressure = 0~40Kpa
           case 1: Preessure_M1(Pres_Dat);  break;      //aircell number 1
           case 2: Preessure_M2(Pres_Dat);  break;      //aircell number 2
           case 3: Preessure_M3(Pres_Dat);  break;      //aircell number 3
           case 4: Preessure_M4(Pres_Dat);  break;      //aircell number 4
           case 5: Preessure_M4(Pres_Dat);  break;      //aircell number 1,2
           case 6: Preessure_M4(Pres_Dat);  break;      //aircell number 2,3
           case 7: Preessure_M4(Pres_Dat);  break;      //aircell number 3,4
           case 8: Preessure_M4(Pres_Dat);  break;      //aircell number 4,1
           default:  break;
           }
          }

   if(Open_TimeFlag){
                   Open_TimeFlag=false;
               char Open_Cell_Num=Cmd_Dat; Open_Cell_Num &= 0x0F;
           switch(Open_Cell_Num){
           case 1: OUT_Open_Time_M1(Time_Dat);  break;      //cell number 1
           case 2: Preessure_M2(Time_Dat);  break;      //cell number 2
           case 3: Preessure_M3(Time_Dat);  break;      //cell number 3
           case 4: Preessure_M4(Time_Dat);  break;      //cell number 4
           case 5: Preessure_M4(Time_Dat);  break;      //cell number 1,2
           case 6: Preessure_M4(Time_Dat);  break;      //cell number 2,3
           case 7: Preessure_M4(Time_Dat);  break;      //cell number 3,4
           case 8: Preessure_M4(Time_Dat);  break;      //cell number 4,1
           default:  break;
           }
          }

   if(Calib_CmdFlag){Calib_CmdFlag=false;
       switch(Cmd_Dat){
           case 1: flash_read();   break;
           case 2: flash_read();   break;
           }
          }

   if (f_500ms){
               f_500ms = 0;
               P1OUT ^= POWER_LED;         // Program Run Monitor
               Addr_485 = Addr_Get();
               }

    }
  }



//-------------------------------------------------------------------
//--------압력센서 읽기 LOOP -------------------------------------
//-------------------------------------------------------------------

void pressureCount(){
    sensor_init_en = true;
    P1_Value = ReadCount1();
    P2_Value = ReadCount2();
    P3_Value = ReadCount3();
    P4_Value = ReadCount4();
    }
void pressureRnOUT(){
    sensor_init_en = true;
    P1_Value = ReadCount1();
    P2_Value = ReadCount2();
    P3_Value = ReadCount3();
    P4_Value = ReadCount4();
    Pressure_Data_Send1();
    }

//-------------------------------------------------------------------
//-------- IN Valve Motor 구동 LOOP ----------------------------------
//-------------------------------------------------------------------
void IN_Open_M1(void){
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread)) return;       //밸브가 해당위치에 있으면 pass
    IN1_ValveRUN;                                   //Motor start
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);                          //for 2'nd photo senser hall pass
    f_5sec=0; cnt_5sec = 0;                         //timer value clear
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();           //time_out time set=5secound
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)f_5sec=1;}     //밸브가 해당위치에 있으면 Stop & break
    }
    IN1_ValveSTOP;                                      //밸브 stop 재확인
    }

void IN_Close_M1(void){

    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread)) return;
    IN1_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)f_5sec=1;}
    }
    IN1_ValveSTOP;
    }
//-------------------------------------------------------------------
void IN_Open_M2(void){

    LED_OFF;
    if((!IN21_Sread) && (IN22_Sread)) return;
    IN2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)f_5sec=1;}
    }
    IN2_ValveSTOP;
    }

void IN_Close_M2(void){

    LED_ON;
    if((!IN21_Sread) && (!IN22_Sread)) return;
    IN2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)f_5sec=1;}
    }
    IN2_ValveSTOP;
    }

//-------------------------------------------------------------------
void IN_Open_M3(void){

    LED_OFF;
    if((!IN31_Sread) && (IN32_Sread)) return;
    IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)f_5sec=1;}
    }
    IN3_ValveSTOP;
    }

void IN_Close_M3(void){

    LED_ON;
    if((!IN31_Sread) && (!IN32_Sread)) return;
    IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)f_5sec=1;}
    }
    IN3_ValveSTOP;
    }

//-------------------------------------------------------------------
void IN_Open_M4(void){

    LED_OFF;
    if((!IN41_Sread) && (IN42_Sread)) return;
    IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)f_5sec=1;}
    }
    IN4_ValveSTOP;
    }

void IN_Close_M4(void){
    LED_ON;
    if((!IN41_Sread) && (!IN42_Sread)) return;
    IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)f_5sec=1;}
    }
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------
//-------------------------------------------------------------------
void IN_Open_M5(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if(VI1_State && VI2_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(VI1_State && VI2_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    }

void IN_Close_M5(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if(VI1_State && VI2_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI1_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(VI1_State && VI2_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    }


//-------------------------------------------------------------------
//-------------------------------------------------------------------
void IN_Open_M6(void){
    bool    VI1_State=0;
    bool    VI3_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if(VI1_State && VI3_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(VI1_State && VI3_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN3_ValveSTOP;
    }

void IN_Close_M6(void){
    bool    VI1_State=0;
    bool    VI3_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if(VI1_State && VI3_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(VI1_State && VI3_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN3_ValveSTOP;
    }
//-------------------------------------------------------------------
void IN_Open_M7(void){
    bool    VI1_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI1_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI1_State && VI4_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M7(void){
    bool    VI1_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI1_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI1_State && VI4_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------
void IN_Open_M8(void){
    bool    VI2_State=0;
    bool    VI3_State=0;
    LED_OFF;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if(VI2_State && VI3_State) return;
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI2_State=0;
    VI3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(VI2_State && VI3_State)f_5sec=1;
    }
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    }

void IN_Close_M8(void){
    bool    VI2_State=0;
    bool    VI3_State=0;
    LED_ON;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if(VI2_State && VI3_State) return;
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI2_State=0;
    VI3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(VI2_State && VI3_State)f_5sec=1;
        }
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    }

//-------------------------------------------------------------------
void IN_Open_M9(void){
    bool    VI2_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI2_State && VI4_State) return;
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI2_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI2_State && VI4_State)f_5sec=1;
    }
    IN2_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M9(void){
    bool    VI2_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI2_State && VI4_State) return;
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI2_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI2_State && VI4_State)f_5sec=1;
        }
    IN2_ValveSTOP;
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------
void IN_Open_M10(void){
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI3_State && VI4_State) return;
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI3_State && VI4_State)f_5sec=1;
    }
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M10(void){
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI3_State && VI4_State) return;
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI3_State && VI4_State)f_5sec=1;
        }
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------

void IN_Open_M11(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    bool    VI3_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if(VI1_State && VI2_State && VI3_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    VI3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(VI1_State && VI2_State && VI3_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    }

void IN_Close_M11(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    bool    VI3_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if(VI1_State && VI2_State && VI3_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    VI3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(VI1_State && VI2_State && VI3_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    }



//-------------------------------------------------------------------

void IN_Open_M12(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI1_State && VI2_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI1_State && VI2_State && VI4_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M12(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI1_State && VI2_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI1_State && VI2_State && VI4_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------

void IN_Open_M13(void){
    bool    VI1_State=0;
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI1_State && VI3_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI1_State && VI3_State && VI4_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M13(void){
    bool    VI1_State=0;
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI1_State && VI3_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI1_State && VI3_State && VI4_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------

void IN_Open_M14(void){
    bool    VI2_State=0;
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI2_State && VI3_State && VI4_State) return;
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI2_State=0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI2_State && VI3_State && VI4_State)f_5sec=1;
    }
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M14(void){
    bool    VI2_State=0;
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI2_State && VI3_State && VI4_State) return;
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI2_State=0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI2_State && VI3_State && VI4_State)f_5sec=1;
        }
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

//-------------------------------------------------------------------

void IN_Open_M15(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (IN42_Sread))VI4_State=1;
    if(VI1_State && VI2_State && VI3_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)VI1_State=1;}
    if(!IN21_Sread){IN2_ValveSTOP; if(IN22_Sread)VI2_State=1;}
    if(!IN31_Sread){IN3_ValveSTOP; if(IN32_Sread)VI3_State=1;}
    if(!IN41_Sread){IN4_ValveSTOP; if(IN42_Sread)VI4_State=1;}
    if(VI1_State && VI2_State && VI3_State && VI4_State)f_5sec=1;
    }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }

void IN_Close_M15(void){
    bool    VI1_State=0;
    bool    VI2_State=0;
    bool    VI3_State=0;
    bool    VI4_State=0;
    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread))VI1_State=1;
    if((!IN21_Sread) && (!IN22_Sread))VI2_State=1;
    if((!IN31_Sread) && (!IN32_Sread))VI3_State=1;
    if((!IN41_Sread) && (!IN42_Sread))VI4_State=1;
    if(VI1_State && VI2_State && VI3_State && VI4_State) return;
    if(!VI1_State)IN1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI2_State)IN2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI3_State)IN3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VI4_State)IN4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VI1_State=0;
    VI2_State=0;
    VI3_State=0;
    VI4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)VI1_State=1;}
        if(!IN21_Sread){IN2_ValveSTOP; if(!IN22_Sread)VI2_State=1;}
        if(!IN31_Sread){IN3_ValveSTOP; if(!IN32_Sread)VI3_State=1;}
        if(!IN41_Sread){IN4_ValveSTOP; if(!IN42_Sread)VI4_State=1;}
        if(VI1_State && VI2_State && VI3_State && VI4_State)f_5sec=1;
        }
    IN1_ValveSTOP;
    IN2_ValveSTOP;
    IN3_ValveSTOP;
    IN4_ValveSTOP;
    }
//-------------------------------------------------------------------
//-------------------------------------------------------------------




//-------------------------------------------------------------------
//------------ OUT VALVE MOTOR CONTROL ------------------------------
//-------------------------------------------------------------------

void OUT_Open_M1(void){
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread)) return;
    OUT1_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)f_5sec=1;}
    }
    OUT1_ValveSTOP;
    }

void OUT_Close_M1(void){
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread)) return;
    OUT1_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)f_5sec=1;}
    }
    OUT1_ValveSTOP;
    }

//-------------------------------------------------------------------

void OUT_Open_M2(void){
    LED_OFF;
    if((!OUT21_Sread) && (OUT22_Sread)) return;
    OUT2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)f_5sec=1;}
    }
    OUT2_ValveSTOP;
    }

void OUT_Close_M2(void){
    LED_ON;
    if((!OUT21_Sread) && (!OUT22_Sread)) return;
    OUT2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)f_5sec=1;}
    }
    OUT2_ValveSTOP;
    }

//-------------------------------------------------------------------

void OUT_Open_M3(void){
    LED_OFF;
    if((!OUT31_Sread) && (OUT32_Sread)) return;
    OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)f_5sec=1;}
    }
    OUT3_ValveSTOP;
    }

void OUT_Close_M3(void){
    LED_ON;
    if((!OUT31_Sread) && (!OUT32_Sread)) return;
    OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)f_5sec=1;}
    }
    OUT3_ValveSTOP;
    }

//-------------------------------------------------------------------

void OUT_Open_M4(void){
    LED_OFF;
    if((!OUT41_Sread) && (OUT42_Sread)) return;
    OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)f_5sec=1;}
    }
    OUT4_ValveSTOP;
    }

void OUT_Close_M4(void){
    LED_ON;
    if((!OUT41_Sread) && (!OUT42_Sread)) return;
    OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)f_5sec=1;}
    }
    OUT4_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M5(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if(VO1_State && VO2_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(VO1_State && VO2_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    }

void OUT_Close_M5(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if(VO1_State && VO2_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(VO1_State && VO2_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M6(void){
    bool    VO1_State=0;
    bool    VO3_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if(VO1_State && VO3_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(VO1_State && VO3_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT3_ValveSTOP;
    }

void OUT_Close_M6(void){
    bool    VO1_State=0;
    bool    VO3_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if(VO1_State && VO3_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(VO1_State && VO3_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT3_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M7(void){
    bool    VO1_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO1_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO1_State && VO4_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M7(void){
    bool    VO1_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO1_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO1_State && VO4_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT4_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M8(void){
    bool    VO2_State=0;
    bool    VO3_State=0;
    LED_OFF;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if(VO2_State && VO3_State) return;
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO2_State=0;
    VO3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(VO2_State && VO3_State)f_5sec=1;
    }
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    }

void OUT_Close_M8(void){
    bool    VO2_State=0;
    bool    VO3_State=0;
    LED_ON;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if(VO2_State && VO3_State) return;
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO2_State=0;
    VO3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(VO2_State && VO3_State)f_5sec=1;
        }
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    }
//-------------------------------------------------------------------
void OUT_Open_M9(void){
    bool    VO2_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO2_State && VO4_State) return;
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO2_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO2_State && VO4_State)f_5sec=1;
    }
    OUT2_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M9(void){
    bool    VO2_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO2_State && VO4_State) return;
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO2_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO2_State && VO4_State)f_5sec=1;
        }
    OUT2_ValveSTOP;
    OUT4_ValveSTOP;
    }


//-------------------------------------------------------------------
void OUT_Open_M10(void){
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO3_State && VO4_State) return;
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO3_State && VO4_State)f_5sec=1;
    }
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M10(void){
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO3_State && VO4_State) return;
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO3_State && VO4_State)f_5sec=1;
        }
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M11(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO3_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if(VO1_State && VO2_State && VO3_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(VO1_State && VO2_State && VO3_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    }

void OUT_Close_M11(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO3_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if(VO1_State && VO2_State && VO3_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO3_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(VO1_State && VO2_State && VO3_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M12(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO1_State && VO2_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO1_State && VO2_State && VO4_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M12(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO1_State && VO2_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO1_State && VO2_State && VO4_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT4_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M13(void){
    bool    VO1_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO1_State && VO3_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO1_State && VO3_State && VO4_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M13(void){
    bool    VO1_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO1_State && VO3_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO1_State && VO3_State && VO4_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M14(void){
    bool    VO2_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO2_State && VO3_State && VO4_State) return;
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO2_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO2_State && VO3_State && VO4_State)f_5sec=1;
    }
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M14(void){
    bool    VO2_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO2_State && VO3_State && VO4_State) return;
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO2_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO2_State && VO3_State && VO4_State)f_5sec=1;
        }
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

//-------------------------------------------------------------------
void OUT_Open_M15(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO1_State && VO2_State && VO3_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO1_State && VO2_State && VO3_State && VO4_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }

void OUT_Close_M15(void){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    LED_ON;
    if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
    if(VO1_State && VO2_State && VO3_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    //__delay_cycles(T_cnt);
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
        if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
        if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
        if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
        if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
        if(VO1_State && VO2_State && VO3_State && VO4_State)f_5sec=1;
        }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;
    }
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void OUT_Open_Time_M1(unsigned char Time_Value){
    bool    VO1_State=0;
    bool    VO2_State=0;
    bool    VO3_State=0;
    bool    VO4_State=0;
    char    Tcnt=0;
    LED_OFF;
    if((!OUT11_Sread) && (OUT12_Sread))VO1_State=1;
    if((!OUT21_Sread) && (OUT22_Sread))VO2_State=1;
    if((!OUT31_Sread) && (OUT32_Sread))VO3_State=1;
    if((!OUT41_Sread) && (OUT42_Sread))VO4_State=1;
    if(VO1_State && VO2_State && VO3_State && VO4_State) return;
    if(!VO1_State)OUT1_ValveRUN;
    M485_Delay();
    if(!VO2_State)OUT2_ValveRUN;
    M485_Delay();
    if(!VO3_State)OUT3_ValveRUN;
    M485_Delay();
    if(!VO4_State)OUT4_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    f_5sec=0; cnt_5sec = 0;
    VO1_State=0;
    VO2_State=0;
    VO3_State=0;
    VO4_State=0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!OUT31_Sread){OUT3_ValveSTOP; if(OUT32_Sread)VO3_State=1;}
    if(!OUT11_Sread){OUT1_ValveSTOP; if(OUT12_Sread)VO1_State=1;}
    if(!OUT21_Sread){OUT2_ValveSTOP; if(OUT22_Sread)VO2_State=1;}
    if(!OUT41_Sread){OUT4_ValveSTOP; if(OUT42_Sread)VO4_State=1;}
    if(VO1_State && VO2_State && VO3_State && VO4_State)f_5sec=1;
    }
    OUT1_ValveSTOP;
    OUT2_ValveSTOP;
    OUT3_ValveSTOP;
    OUT4_ValveSTOP;

    M485_Delay();
    while(Tcnt < Time_Value) {if(f_1sec) {cnt_1sec = 0; f_1sec=0; Tcnt++;}}

//close
        LED_ON;
        if((!OUT11_Sread) && (!OUT12_Sread))VO1_State=1;
        if((!OUT21_Sread) && (!OUT22_Sread))VO2_State=1;
        if((!OUT31_Sread) && (!OUT32_Sread))VO3_State=1;
        if((!OUT41_Sread) && (!OUT42_Sread))VO4_State=1;
        if(VO1_State && VO2_State && VO3_State && VO4_State) return;
        if(!VO1_State)OUT1_ValveRUN;
        M485_Delay();
        if(!VO2_State)OUT2_ValveRUN;
        M485_Delay();
        if(!VO3_State)OUT3_ValveRUN;
        M485_Delay();
        if(!VO4_State)OUT4_ValveRUN;
        cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
        f_5sec=0; cnt_5sec = 0;
        VO1_State=0;
        VO2_State=0;
        VO3_State=0;
        VO4_State=0;
        while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
            if(!OUT31_Sread){OUT3_ValveSTOP; if(!OUT32_Sread)VO3_State=1;}
            if(!OUT11_Sread){OUT1_ValveSTOP; if(!OUT12_Sread)VO1_State=1;}
            if(!OUT21_Sread){OUT2_ValveSTOP; if(!OUT22_Sread)VO2_State=1;}
            if(!OUT41_Sread){OUT4_ValveSTOP; if(!OUT42_Sread)VO4_State=1;}
            if(VO1_State && VO2_State && VO3_State && VO4_State)f_5sec=1;
            }
        OUT1_ValveSTOP;
        OUT2_ValveSTOP;
        OUT3_ValveSTOP;
        OUT4_ValveSTOP;
        }



//-------------------------------------------------------------------
//-------------------------------------------------------------------

void M485_Delay(void){
                    cnt_100ms=0; f_100ms=0;
                    while(!f_100ms){if(f_rcv_proc1==true) rcv_proc2();}
                    }

/*
void delay_500mS(void){
    cnt_500ms = 0;
    while(!f_500ms){if(f_rcv_proc1==true) rcv_proc2();}
    }

void delay_ms(unsigned int ms)
{
    while (ms)
    {
        __delay_cycles(1000); //10000 for 10MHz and 16000 for 16MHz
        ms--;
    }
}
*/

//-------------------------------------------------------------------
//-------- IN Valve Pressure set LOOP ----------------------------------
//-------------------------------------------------------------------

void Preessure_M1(char P_Value){
    LED_OFF;
    if((!IN11_Sread) && (IN12_Sread)) return;       //밸브가 해당위치에 있으면 pass
    IN1_ValveRUN;                                   //Motor start
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);                          //for 2'nd photo senser hall pass
    f_5sec=0; cnt_5sec = 0;                         //timer value clear
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();           //time_out time set=5secound
    if(!IN11_Sread){IN1_ValveSTOP; if(IN12_Sread)f_5sec=1;}     //밸브가 해당위치에 있으면 Stop & break
    }
    IN1_ValveSTOP;                                      //밸브 stop 재확인


    LED_ON;
    if((!IN11_Sread) && (!IN12_Sread)) return;
    IN1_ValveRUN;
    cnt_2sec=0; f_2sec=0; while(!f_2sec){if(f_rcv_proc1==true) rcv_proc2();}
    //__delay_cycles(C_cnt);
    f_5sec=0; cnt_5sec = 0;
    while(!f_5sec){if(f_rcv_proc1==true) rcv_proc2();
    if(!IN11_Sread){IN1_ValveSTOP; if(!IN12_Sread)f_5sec=1;}
    }
    IN1_ValveSTOP;
    }

void Preessure_M2(char P_Value){

    }

void Preessure_M3(char P_Value){

    }
void Preessure_M4(char P_Value){

    }










//-------------------------------------------------------------------
//-------------------------------------------------------------------

void clock_init(void){
        UBYTE ii;
        DCOCTL = 0;
        BCSCTL1 &= ~XT2OFF;                       // XT2= HF XTAL .

    do {
        IFG1 &= ~OFIFG;                       // Clear OSCFault flag
        for (ii = 0xFF; ii > 0; ii--);        // Time for flag to set
        }
    while ((IFG1 & OFIFG));                   // OSCFault flag still set?
        BCSCTL2 |= SELM_2 | SELS;                 // MCLK= XT2 (safe), SMCLK = XT2
        }

void IOPort_init(void){
        // P1.7 LED를 Output으로 설정
        P1DIR |= BIT7;
        // P1.0, P1.1, P1.2, P1.3, P1.4 address 와 switch P1.5를 Input으로 설정
        P1DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | RESET);
        // P2.0, P2.1, P2.2, P2.3, P2.4, P2.5, P2.6, P2.7 photo sensor를 Input으로 설정
        //P2DIR &= ~(IN3_1 | IN3_2 | OUT3_1 | OUT3_2 | IN4_1 | IN4_2 | OUT4_1 | OUT4_2);
        P2DIR |= 0x00;      //photo senser P2Pin all input

        // P6.0, P6.1, P6.2, P6.3, P6.4, P6.5, P6.6, P6.7 photo sensor를 Input으로 설정
        //P6DIR &= ~(IN1_1 | IN1_2 | OUT1_1 | OUT1_2 | IN2_1 | IN2_2 | OUT2_1 | OUT2_2);
        P6DIR |= 0x00;      //photo senser P6Pin all input

        // P4.0, P4.2, P4.4, P4.6을 Input으로 설정 - Data In from HX710B
        P4DIR &= ~(BIT0 | BIT2 | BIT4 | BIT6);

        // P4.1, P4.3, P4.5, P4.7을 Output으로 설정 - Clock Out to HX710B
        P4DIR |= BIT1 | BIT3 | BIT5 | BIT7;

        // P6.0, P6.1, P6.2, P6.3을 Output으로 설정 - Display용 Port 설정
        // P6DIR |= BIT0 | BIT1 | BIT2 | BIT3;
        }

//==============================================================================
// Timer_B7 초기화
//------------------------------------------------------------------------------
void Timer_B7_init(void){

//      TBCCR0 = (UINT)(CLK_11MHz/1000);
//      TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, upmode
//      TBCCTL0 = CCIE;                           // TBCCR0 interrupt enabled
/*
        TBCCR0 = (UINT)(CLK_11MHz/1000);
        TBCTL = TBSSEL_2 + MC_1;                    // SMCLK, upmode
        TBCCTL0 = CCIE;                             // TBCCR0 interrupt enabled
        TBCTL = TBSSEL_2 | MC_1 | TBCLR;            //Timer B7의 레지스터에 대한 값들을 설정합니다. timer clear
        TB0CCTL0 |= CCIE;
*/
        TB0CCR0 = TIMER_PERIOD;
        TBCTL = TBSSEL_2 | MC_1 | TBCLR;            //Timer B7의 레지스터에 대한 값들을 설정합니다. timer clear
        TB0CCTL0 |= CCIE;
        }

void UART_Init(void){
        // UART Port1 설정 및 속도, UART Interrupt 설정
        // RS-485
        P3DIR |= BIT5;                  // P3.5을 Output으로 설정, 485 방향 설정
        P3OUT &= ~BIT5;                 // P3.5 Output을 "0"으로 만들어 수신으로 설정
        P3DIR &= ~BIT7;                 // P3.7 Port를 Input으로 설정(RX)
        P3SEL |= 0xc0;                  // P3.6,P3.7 = USART1 Tx1/Rx1
        UCTL1 |= CHAR;                  // 8-bit character
        UTCTL1 = SSEL1+SSEL0;           // UCLK = SMCLK

        //Baud Rate 설정 : 38400
        //UBR01 = 0x20;                   // 11.0592Mhz/38400 = 288 = 0x120.
        //UBR11 = 0x01;                   // 11.0592Mhz/19200 = 576 = 0x240.                                      // 11.0592Mhz/9600 = 1152 = 0x480
        //UBR01 = 0xC0;
        //UBR11 = 0x00;

        UBR01 = 0x60;                   // 115200
        UBR11 = 0x00;                   // 11.0592Mhz/115200 = 96 = 0x0060

        UMCTL1 = 0x00;                  // no modulation
        ME2 |= UTXE1 + URXE1;           // Enable USART1 TXD/RXD

        UCTL1 &= ~SWRST;                // Initalize USART state machine
        IE2 |= URXIE1;                  // Enable USART1 RX interrupt
        }

uint8_t Addr_Get(void){
    uint8_t Addr;
            Addr = ~P1IN;
            Addr &= 0x1F;
            return Addr;
            }

//---------------------------------------------
//read time = 50uS
//---------------------------------------------


unsigned long ReadCount1(void)
{
    unsigned long Count;
    unsigned char i;
    unsigned long Count_24bit;
    uint16_t Return_Value;
    //P4OUT |= ADDO1;
    P4DIR &= ~ADDO1;
    P4OUT &= ~ADSK1;
    P4DIR |= ADSK1;
    Count = 0;

    cnt_10ms=0; f_10ms=0;
    while(P4IN & ADDO1){if(f_10ms)break;}
    //while(!f_10ms){if(!(P4IN & ADDO1))f_10ms=1;}         //time out = 6mS
    //while(P4IN & ADDO1);
    _DINT();        // disable global interrupts
    for (i = 0; i < 24; i++){
        P4OUT |= ADSK1;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK1;
        if(P4IN & ADDO1) Count++;
        }
    _EINT();        // enable interrupts
    P4OUT |= ADSK1;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK1;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
//    if(sensor_init_en == true){
//        P_Init_Val1 = (uint16_t)(Pressure_KPA*10);
//        }
//    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val1);
    float_temp1 = ((Pressure_KPA * 10) - 204);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}
//---------------------------------------------
//---------------------------------------------

unsigned long ReadCount2(void)
{
    unsigned long Count;
    unsigned char i;
    unsigned long Count_24bit;
    uint16_t Return_Value;
    //P4OUT |= ADDO1;
    P4DIR &= ~ADDO2;
    P4OUT &= ~ADSK2;
    P4DIR |= ADSK2;
    Count = 0;

    cnt_10ms=0; f_10ms=0;
    while(P4IN & ADDO2){if(f_10ms)break;}
    //while(!f_10ms){if(!(P4IN & ADDO2))f_10ms=1;}
    //while(P4IN & ADDO2);
    _DINT();        // disable global interrupts
    for (i = 0; i < 24; i++){
        P4OUT |= ADSK2;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK2;
        if(P4IN & ADDO2) Count++;
        }
    _EINT();        // enable interrupts
    P4OUT |= ADSK2;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK2;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
//    if(sensor_init_en == true){
//        P_Init_Val2 = (uint16_t)(Pressure_KPA*10);
//        }
//    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val2);
    float_temp1 = ((Pressure_KPA * 10) - 204);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}
//---------------------------------------------
//---------------------------------------------

unsigned long ReadCount3(void)
{
    unsigned long Count;
    unsigned char i;
    unsigned long Count_24bit;
    uint16_t Return_Value;
    //P4OUT |= ADDO1;
    P4DIR &= ~ADDO3;
    P4OUT &= ~ADSK3;
    P4DIR |= ADSK3;
    Count = 0;

    cnt_10ms=0; f_10ms=0;
    while(P4IN & ADDO3){if(f_10ms)break;}
    //while(!f_10ms){if(!(P4IN & ADDO3))f_10ms=1;}
    //while(P4IN & ADDO3);
    _DINT();        // disable global interrupts
    for (i = 0; i < 24; i++){
        P4OUT |= ADSK3;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK3;
        if(P4IN & ADDO3) Count++;
        }
    _EINT();        // enable interrupts
    P4OUT |= ADSK3;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK3;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
//    if(sensor_init_en == true){
//        P_Init_Val3 = (uint16_t)(Pressure_KPA*10);
//        }
//    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val3);
    float_temp1 = ((Pressure_KPA * 10) - 204);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}
//---------------------------------------------
//---------------------------------------------

unsigned long ReadCount4(void)
{
    unsigned long Count;
    unsigned char i;
    unsigned long Count_24bit;
    uint16_t Return_Value;
    //P4OUT |= ADDO1;
    P4DIR &= ~ADDO4;
    P4OUT &= ~ADSK4;
    P4DIR |= ADSK4;
    Count = 0;

    cnt_10ms=0; f_10ms=0;
    while(P4IN & ADDO4){if(f_10ms)break;}
    //while(!f_10ms){if(!(P4IN & ADDO4))f_10ms=1;}
    //while(P4IN & ADDO4);
    _DINT();        // disable global interrupts
    for (i = 0; i < 24; i++){
        P4OUT |= ADSK4;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK4;
        if(P4IN & ADDO4) Count++;
        }
    _EINT();        // enable interrupts

    P4OUT |= ADSK4;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK4;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
//    if(sensor_init_en == true){
//        P_Init_Val4 = (uint16_t)(Pressure_KPA*10);
//        sensor_init_en = false;
//        flash_write();
//        }
//    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val4);
    float_temp1 = ((Pressure_KPA * 10) - 204);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}

//---------------------------------------------
//---------------------------------------------

void Init_Input(void)    // P1.5
{
    Init_in_stat = ~P1IN;
    Init_in_stat &= RESET;

    Init_input_stat = Init_in_stat;
    if((Init_old_input_stat == Init_input_stat) && (Init_input_stat !=0)) {
          if((Init_key_counter >= 5)&&(Init_zero_count>=10)) {
              Init_zero_count = 0;

              sensor_init_en = true;
              if(pump_on == true)
              {
                  pump_on = false;
              }else{
                  pump_on = true;
              }
              }else {
                  ++Init_key_counter;
                  }
                }else {
          ++Init_zero_count;
          Init_key_counter = 0;                  // 연속 Check timer clear
          Init_old_input_stat = Init_input_stat;
          //input_flag = 0;
    }
}
//---------------------------------------------
//---------------------------------------------

void sensor_init(void)
{
    sensor_init_en = true;
    Pressure_Data_Send1();
}
//---------------------------------------------
//---------------------------------------------

void flash_write(void)
{
    unsigned char *flash_ptr;       // Flash pointer
    UBYTE   kk;

    bf_data[1] = P_Init_Val1/256;
    bf_data[2] = P_Init_Val1%256;
    bf_data[3] = P_Init_Val2/256;
    bf_data[4] = P_Init_Val2%256;
    bf_data[5] = P_Init_Val3/256;
    bf_data[6] = P_Init_Val3%256;
    bf_data[7] = P_Init_Val4/256;
    bf_data[8] = P_Init_Val4%256;

    free_byte0 = 0;
    for(kk=1; kk<9; ++kk) {
        free_byte0+=bf_data[kk];
    }
    bf_data[0] = 0xFF-free_byte0;
//------------------------------------------------------------------------------
    flash_ptr = (unsigned char *) 0x1000;   // Initialize Flash pointer
        _DINT();                            // disable interrupts
    FCTL2 = FWKEY+FSSEL0+FN4+FN3+FN2+FN1;   // MCLK/30 for Flash Timing Generator
    flash_ptr = (unsigned char *) 0x1000;
    FCTL1 = FWKEY + ERASE;              // Set Erase bit
    FCTL3 = FWKEY;                      // Clear Lock bit
    *flash_ptr = 0;                     // Dummy write to erase Flash segment
    FCTL1 = FWKEY + WRT;                // Set WRT bit for write operation
//------------------------------------------------------------------------------
    for(kk=0; kk<9; ++kk){
        *flash_ptr++ = bf_data[kk];
        }
    FCTL1 = FWKEY;                      // Clear WRT bit
    FCTL3 = FWKEY + LOCK;               // Set LOCK bit
    _EINT();                            // __enable_interrupt() -> D\program\IAR_MSP430\Embed..\430\inc\in430.h
    flash_read();
    }
//---------------------------------------------
//---------------------------------------------
void flash_24bit_write(void){

    unsigned char *flash_ptr;       // Flash pointer
    UBYTE   k;

    Pressure_data[1] = 0X02;
    Pressure_data[2] = 0X03;
    Pressure_data[3] = 0X04;
    Pressure_data[4] = 0X05;
    Pressure_data[5] = 0X06;
    Pressure_data[6] = 0X07;
    Pressure_data[7] = 0X08;
    Pressure_data[8] = 0X09;
    Pressure_data[9] = 0X10;
    Pressure_data[10]= 0X11;
    Pressure_data[11]= 0X12;
    Pressure_data[12]= 0X13;
    Pressure_data[13]= 0X14;
    Pressure_data[14]= 0X15;
    Pressure_data[15]= 0X16;

//------------------------------------------------------------------------------
    flash_ptr = (unsigned char *) 0x1020;   // Initialize Flash pointer
    _DINT();                                // disable interrupts
    FCTL2 = FWKEY+FSSEL0+FN4+FN3+FN2+FN1;   // MCLK/30 for Flash Timing Generator
    flash_ptr = (unsigned char *) 0x1020;
    FCTL1 = FWKEY + ERASE;              // Set Erase bit
    FCTL3 = FWKEY;                      // Clear Lock bit
    *flash_ptr = 0;                     // Dummy write to erase Flash segment
    FCTL1 = FWKEY + WRT;                // Set WRT bit for write operation
//------------------------------------------------------------------------------
    for(k=0; k<16; ++k){ *flash_ptr++ = Pressure_data[k]; }
    FCTL1 = FWKEY;                      // Clear WRT bit
    FCTL3 = FWKEY + LOCK;               // Set LOCK bit
    _EINT();                            // __enable_interrupt() -> D\program\IAR_MSP430\Embed..\430\inc\in430.h
    }

//---------------------------------------------
//---------------------------------------------
void flash_24bit_read(void)
    {
    UBYTE f;
    unsigned char *flash_ptr;       // Flash pointer

    flash_ptr = (unsigned char *) 0x1020;   // Initialize Flash pointer

    for(f=0; f<16; f++){Pressure_data[f] = *flash_ptr++;}
    }

//---------------------------------------------
//---------------------------------------------
void flash_read(void)
    {
    UBYTE f;
    unsigned char *flash_ptr;       // Flash pointer

    flash_ptr = (unsigned char *) 0x1000;   // Initialize Flash pointer

    for(f=0; f<9; ++f){
                        bf_data[f] = *flash_ptr++;
                        }

    P_Init_Val1 = bf_data[1]*256 + bf_data[2];
    P_Init_Val2 = bf_data[3]*256 + bf_data[4];
    P_Init_Val3 = bf_data[5]*256 + bf_data[6];
    P_Init_Val4 = bf_data[7]*256 + bf_data[8];
    }

//---------------------------------------------
//---------------------------------------------

void flashData_init(void)
{
    uint8_t ii;
    // flash memory를 읽어 설정값을 set한다..
    flash_read();
    free_byte0 = 0;
    for(ii=1; ii<9; ++ii) {
        free_byte0+=bf_data[ii];
    }
    free_byte0 = 0xFF-free_byte0;
    if(free_byte0==bf_data[0]) ;
    else {                              // 한번 더 읽음.
        free_byte0 = 0;
        flash_read();
        for(ii=1; ii<9; ++ii) {
            free_byte0+=bf_data[ii];
        }
        free_byte0 = 0xFF-free_byte0;
        if(free_byte0==bf_data[0]) ;
        else {  // 플레쉬 메모리에 데이터가 없는 경우 이므로 데이터를 초기화 시켜줌
            P_Init_Val1 = 204;
            P_Init_Val2 = 204;
            P_Init_Val3 = 204;
            P_Init_Val4 = 204;
            flash_write();
        }
    }
}
//---------------------------------------------
//---------------------------------------------

void rcv_proc2(void){
        Cmd_Dat = 0;
        f_rcv_proc1 = false;
        rcv_address = rcv_buf1[2];
    if (rcv_address == Addr_485) {    //board asress 1~28
        rcv_command1 = rcv_buf1[3];
     if(rcv_command1==0x81)
         Pressure_Data_Send1();
         //pressureRnOUT();
        }
    }
//---------------------------------------------
//---------------------------------------------

void rcv_proc1(void){
        Cmd_Dat = 0;
        f_rcv_proc1 = false;
        INValve_OpFlag = false;
        INValve_ClFlag = false;
        OUTValve_OpFlag = false;
        OUTValve_ClFlag = false;
        rcv_address = rcv_buf1[2];
    if ((rcv_address == Addr_485) | (rcv_address == 0xF5)){    //board asress 1~28

        rcv_command1 = rcv_buf1[3];
 switch(rcv_command1){

        case 0x81:  Pressure_Data_Send1();  break;

        case 0x83:  INValve_OpFlag=true;    Cmd_Dat=rcv_buf1[4]; Answer_Mquestion(); break;
        case 0x84:  OUTValve_OpFlag=true;   Cmd_Dat=rcv_buf1[4]; Answer_Mquestion(); break;
        case 0x85:  INValve_ClFlag=true;    Cmd_Dat=rcv_buf1[4]; Answer_Mquestion(); break;
        case 0x86:  OUTValve_ClFlag=true;   Cmd_Dat=rcv_buf1[4]; Answer_Mquestion(); break;
        case 0x87:  Calib_CmdFlag=true;     Cmd_Dat=rcv_buf1[4]; Answer_Mquestion(); break;
        case 0x88:  Motor_CmdFlag=true;     Cmd_Dat=rcv_buf1[4]; Answer_Mquestion(); break;

        case 0x89:  Open_TimeFlag=true;     Cmd_Dat=rcv_buf1[4]; Time_Dat=rcv_buf1[5]; Answer_Mquestion(); break;
        case 0x8A:  Pressure_SetFlag=true;  Cmd_Dat=rcv_buf1[4]; Pres_Dat=rcv_buf1[5]; Answer_Mquestion(); break;

        case 0x82:
            Calib_CmdFlag=true; Answer_Mquestion(); break;

        default:
            break;
        }
    }
    __no_operation();
}

//---------------------------------------------
//---------------------------------------------


void Answer_Mquestion(void)
    {
    uint8_t i;
    TX485_On();
    trs_buf1[0] = 0x7E;
    trs_buf1[1] = 0xE7;
    trs_buf1[2] = 05;   //데이터 길이 다시 체크
    trs_buf1[3] = 'S';
    trs_buf1[4] = Addr_485;
    trs_buf1[5] = rcv_buf1[3];
    trs_buf1[6] = rcv_buf1[4];
    trs_buf1[7] = 0xFF;

    free_byte0 = 0;
    for(i = 0; i<7; i++){                  //byte count = 7
        free_byte0 += trs_buf1[i];
    }
    free_byte0 = 0xFF - free_byte0;         // Checksum 계산
    trs_buf1[7] = free_byte0;               // buf7

    UCA1_strout_length(trs_buf1, 8);       //txout cnt=8(last byte=checksum)

    __delay_cycles(3000);                   //-1000- /  __1000cnt/100uS__(360uS)
    RX485_On();
    }

//---------------------------------------------
//---------------------------------------------


void Pressure_Data_Send1(void)
    {
    uint8_t i;
    TX485_On();
    trs_buf1[0] = 0x7E;
    trs_buf1[1] = 0xE7;
    trs_buf1[2] = 12;   //데이터 길이 다시 체크
    trs_buf1[3] = 'S';
    trs_buf1[4] = Addr_485;
    trs_buf1[5] = 0x81;
    trs_buf1[6] = (P1_Value >> 8) & 0xFF;
    trs_buf1[7] = P1_Value & 0xFF;
    trs_buf1[8] = (P2_Value >> 8) & 0xFF;
    trs_buf1[9] = P2_Value & 0xFF;
    trs_buf1[10] = (P3_Value >> 8) & 0xFF;
    trs_buf1[11] = P3_Value & 0xFF;
    trs_buf1[12] = (P4_Value >> 8) & 0xFF;
    trs_buf1[13] = P4_Value & 0xFF;

    free_byte0 = 0;
    for(i = 0; i<14; i++){                  //byte count = 14
        free_byte0 += trs_buf1[i];
    }
    free_byte0 = 0xFF - free_byte0;         // Checksum 계산
    trs_buf1[14] = free_byte0;              // buf14

    UCA1_strout_length(trs_buf1, 15);       //txout cnt=15(last byte=checksum)

    __delay_cycles(3600);                   //-1000- /  __1000cnt/100uS__(360uS)
    RX485_On();
    }

//--------------------------------
void UCA1_strout_length(UBYTE *tx_str, UBYTE length)
    {
    UBYTE i;
    for(i=0; i<length; i++) {
        UCA1_chout(*(tx_str+i));
    }
  }

//--------------------------------
void UCA1_chout(unsigned char tx_ch)
    {
    while(!(IFG2 & UTXIFG1));
    TXBUF1 = tx_ch;
    }

//--------------------------------
//--------------------------------

// Timer B0 interrupt service routine

#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
    TB0CCTL0 &= ~CCIE;              //disable
    TBCCTL0 &= ~CCIFG;              //clr flag
    if(waiting_tm) --waiting_tm;
    //f_10ms=1;
    __no_operation();

    if (++cnt_10ms >= 2) {          //1=6mS/2=12ms
                cnt_10ms = 0;
                f_10ms=1;
                }

    if (++cnt_100ms >= 16) {        //20=96ms
                cnt_100ms = 0;
                f_100ms=1;          //rcv_proc2
                }
    if (++cnt_200ms >= 32) {        //30=192ms
                pressureCount();
                cnt_200ms = 0;
                f_200ms=1;
                }
    /*
    if (++cnt_20ms >= 2) {          //2=12ms
                cnt_20ms = 0;
                f_20ms=1;
                }
    if (++cnt_250ms >= 40) {        //240mS
            cnt_250ms = 0;
            f_250ms=1;
                }
    if (++cnt_50ms >= 8) {          //8=48ms
                cnt_50ms = 0;
                f_50ms=1;
                }
    */
    if (++cnt_1sec >= 160) {        //0.96sec
                cnt_1sec = 0;
                f_1sec=1;
                }
    if (++cnt_2sec >= 250) {        //1.5
                cnt_2sec = 0;
                f_2sec=1;
                }

    if (++cnt_500ms >= 100) {       //600mS
                cnt_500ms = 0;
                f_500ms=1;
                }

    if (++cnt_5sec >= 600) {        //3.6sec
                cnt_5sec = 0;
                f_5sec=1;
                }

    TBCCTL0 &= ~CCIFG;              ////____clr flag
    TB0CCTL0 |= CCIE;               //enable

}

//--------------------------------
//--------------------------------




//--------------------------------
//--------------------------------

// RS-485 Receive Interrupt Vector(P3.7)
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1RX_VECTOR
__interrupt void usart1_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) usart1_rx (void)
#else
#error Compiler not supported!
#endif
{
    while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
        rcv_data1 = RXBUF1;
        __no_operation();
     if(rcv_data1 == 0x7E){                     // 1st Frame Header check : 0x7E
                            first_flag1 = 1;
                            }
     if(rcv_data1 == 0xE7){                     // 2nd Frame Header Check : 0xE7
            if(first_flag1 == 1){
                            f_rcv_cnt1 =1;
                            first_flag1 = 0;
                            rcv_cnt1 = 0;
                            }
                        }
    if(f_rcv_cnt1 == 1){                        // Data Length Extraction
                __no_operation();
                rcv_cnt1++;
             if(rcv_cnt1 == 2){
                     rcv_end_cnt1 = rcv_data1;
                     f_rcv_start1 = 1;
                     f_rcv_cnt1 = 0;
                     rcv_cnt1 = 0;
                     rcv_start_cnt1 = 0;
                     p_rcv_buf1 = &rcv_buf1[0];          // Designate the pointer for rcv_buf
                     }
                    }
    if(f_rcv_start1 == 1){
                *p_rcv_buf1++ = rcv_data1;              // Data Receive and Save
                rcv_start_cnt1++;
                compare_cnt1 = rcv_end_cnt1 + 1;
        if(rcv_start_cnt1 >= compare_cnt1){             // Check Receive End
                f_rcv_start1 = 0;
                rcv_start_cnt1 = 0;
                f_rcv_proc1 = true;                     // Activate the receive procedure:rcv_proc()
                              // rcv_proc() is located in 100ms time routine of main procedure
            }
        }
    }



//-----------------------------------------------------------------------------

/*
void delay_us(unsigned int us)
{
    while (us)
    {
        __delay_cycles(10); // 1 for 10 Mhz set 16 for 16 MHz
        us--;
    }
}






































//-----------------------------------------------------
//read time = 1senser=50uSx4=200uS / 4senser=120uS(err)
//-----------------------------------------------------

void pressureCount(void){

    unsigned long Count1,Count2,Count3,Count4;
    unsigned char i;
    unsigned long Count1_24bit,Count2_24bit,Count3_24bit,Count4_24bit;
    //uint16_t Pressure_Value1,Pressure_Value1,Pressure_Value1,Pressure_Value1;

    P4DIR &= 0xAA;     //data input & out Mode
    //P4DIR &= ~ADDO1; P4DIR &= ~ADDO2; P4DIR &= ~ADDO3; P4DIR &= ~ADDO4;     //data input Mode
    P4OUT &= 0x55;     //OUT LOW
    //P4OUT &= ~ADSK1; P4OUT &= ~ADSK1; P4OUT &= ~ADSK1; P4OUT &= ~ADSK1;     //OUT LOW
    //P4DIR |= ADSK1;  P4DIR |= ADSK2;  P4DIR |= ADSK3;  P4DIR |= ADSK4;      //Clock(24bit) out Node
    Count1=0; Count2=0; Count3=0; Count4=0;

    f_10ms=0;
    while(!f_10ms){
                if((P4IN & ADDO1) & (P4IN & ADDO2) & (P4IN & ADDO3) & (P4IN & ADDO4))f_10ms=1;
                    }
    _DINT();        // disable global interrupts
    for (i = 0; i < 24; i++){
        //P4OUT |= ADSK1; P4OUT |= ADSK2; P4OUT |= ADSK3; P4OUT |= ADSK4; //clock high
        P4OUT |= 0xFF; //clock high
        Count1 <<= 1; Count2 <<= 1; Count3 <<= 1; Count4 <<= 1;         //count shift
        //P4OUT &= ~ADSK1; P4OUT &= ~ADSK2; P4OUT &= ~ADSK3; P4OUT &= ~ADSK4; //clock low
        P4OUT &= 0x55; //clock low
        //__no_operation();
     if(P4IN & ADDO4) Count1++;  //bit=high
     if(P4IN & ADDO3) Count1++;
     if(P4IN & ADDO2) Count1++;
     if(P4IN & ADDO1) Count1++;
        }
            P4OUT |= 0xFF; //clock high
            //P4OUT |= ADSK1; P4OUT |= ADSK2; P4OUT |= ADSK3; P4OUT |= ADSK4; //clock high
        Count4 ^= 0x800000;
        Count3 ^= 0x800000;
        Count2 ^= 0x800000;
        Count1 ^= 0x800000; // 24bit Data의 보수를 취한다.
        P4OUT &= 0x55; //clock low
        //P4OUT &= ~ADSK1; P4OUT &= ~ADSK2; P4OUT &= ~ADSK3; P4OUT &= ~ADSK4;     //clock low
        _EINT();        // enable global interrupts

        Count1_24bit = Count4 & 0xFFFFFF;
        Count2_24bit = Count3 & 0xFFFFFF;
        Count3_24bit = Count2 & 0xFFFFFF;
        Count4_24bit = Count1 & 0xFFFFFF;

        Pressure1_KPA = ((float)Count4_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
        Pressure2_KPA = ((float)Count3_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
        Pressure3_KPA = ((float)Count2_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
        Pressure4_KPA = ((float)Count1_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
        if(sensor_init_en == true){
                                    P_Init_Val4 = (uint16_t)(Pressure1_KPA*10);
                                    P_Init_Val3 = (uint16_t)(Pressure2_KPA*10);
                                    P_Init_Val2 = (uint16_t)(Pressure3_KPA*10);
                                    P_Init_Val1 = (uint16_t)(Pressure4_KPA*10);
                                    }

        float_temp4 = ((Pressure1_KPA * 10) - P_Init_Val4);
        float_temp3 = ((Pressure2_KPA * 10) - P_Init_Val3);
        float_temp2 = ((Pressure3_KPA * 10) - P_Init_Val2);
        float_temp1 = ((Pressure4_KPA * 10) - P_Init_Val1);
     if(float_temp4 <= 0) float_temp4 = 0;
     if(float_temp3 <= 0) float_temp3 = 0;
     if(float_temp2 <= 0) float_temp2 = 0;
     if(float_temp1 <= 0) float_temp1 = 0;
        P4_Value = (uint16_t)((float_temp4*2.42));       // 압력센서 전압 1.2V 일때 계산식
        P3_Value = (uint16_t)((float_temp3*2.42));
        P2_Value = (uint16_t)((float_temp2*2.42));
        P1_Value = (uint16_t)((float_temp1*2.42));
        return;
        }

*/

/*
#pragma vector = TIMER0_B0_VECTOR
__interrupt
void Timer_B0_ISR(void){
    //__disable_interrupt();
    TB0CCTL0 &= ~CCIE;              //disable


    if (++cnt_5ms >= 5) {
        cnt_5ms = 0;
        f_5ms=1;
    }
    if (++cnt_10ms >= 10) {
        cnt_10ms = 0;
        f_10ms=1;
    }

    if(waiting_tm) --waiting_tm;
    f_1ms=1;
    __no_operation();

    if (++cnt_5ms >= 5) {
        cnt_5ms = 0;
        f_5ms=1;
    }
    if (++cnt_5ms >= 5) {
        cnt_5ms = 0;
        f_5ms=1;
    }

    if (++cnt_20ms >= 20) {
        cnt_20ms = 0;
        f_20ms=1;
    }
    if (++cnt_50ms >= 50) {
        cnt_50ms = 0;
        f_50ms=1;
    }

    if (++cnt_100ms >= 100) {
        cnt_100ms = 0;
        f_100ms=1;
    }
    if (++cnt_200ms >= 200) {
        cnt_200ms = 0;
        f_200ms=1;
    }
    if (++cnt_250ms >= 250) {
        cnt_250ms = 0;
        f_250ms=1;
    }

    if (++cnt_500ms >= 500) {
        cnt_500ms = 0;
        f_500ms=1;
    }
    if (++cnt_1sec >= 1000) {
        cnt_1sec = 0;
        f_1sec=1;
    }
    if (++cnt_500ms >= 500) {
        cnt_500ms = 0;
        f_500ms=1;
    }
    TBCCTL0 &= ~CCIFG;              //clr flag
    TB0CCTL0 |= CCIE;               //enable
    //__enable_interrupt();
}
*/


//---------------------------------------------
//---------------------------------------------

/*
unsigned long ReadCount1(void)
{
    unsigned long Count;
    unsigned char i;
    unsigned long Count_24bit;
    uint16_t Return_Value;
    //P4OUT |= ADDO1;
    P4DIR &= ~ADDO1;
    P4OUT &= ~ADSK1;
    P4DIR |= ADSK1;
    Count = 0;

    while(P4IN & ADDO1);

    for (i = 0; i < 24; i++){
        P4OUT |= ADSK1;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK1;
        if(P4IN & ADDO1) Count++;
    }
    P4OUT |= ADSK1;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK1;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
    if(sensor_init_en == true){
        P_Init_Val1 = (uint16_t)(Pressure_KPA*10);
    }
    float_temp1 = ((Pressure1_KPA * 10) - P_Init_Val1);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식
    return Return_Value;
}

*/





