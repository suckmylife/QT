
//클럭확인
//타이머확인
//버튼확인
//버튼 + LED 동작 확인
//모터 동작확인
//밸브 동작 확인
//부저음 확인-> 음까지 확인
//어댑터 인터럽트 확인
//UART 초기화 확인
//flash, pressure 함수 초기화 확인

//딜레이 + 전원 버튼 인터럽트 확인 필요
//블루투스 활성화 기능 추가
// 디버깅 중
//ADC 인터럽트 멈춤 현상 해소 -> EINT DINT 로 해결
//블루투스 모드 기능 활성화가 왜 안되는지 -> 모든 선을 연결하였을때 디버깅중 -> 버튼을 인터럽트로 바꿔야한다는 의견이 있어서 바꾸는 중
//모드 세부 변경1.8.6
//자동모드 검수 및 변경, 모드 동작 자동모드5번 기본모드4번 적용
//블루투스 버튼 모두 누르기 기능 추가
//베터리 완충 관련 코드 수정 -> 베터리 완충 충전중 감지 확인
//전원 버튼이 안되는 이유 -> 카운트 낮춤
//전압체크 깜빡임 수정 3회 후 꺼지도록
//readcount에서 flash_write 추가 + 압력 초기화 버튼 함수 추가
//1.8.7
//전원꺼짐 현상과 충전어댑터 인터럽트 충돌현상 감지 -> 전원이 꺼졌을때 제대로 충전어댑터가 작동하도록 수정
//1.8.8
//전원 버튼 제외 눌렸을때 소리나도록 설정
//전원 기능 딜레이 없음
//1초 점멸 로직 수정(모드 조건문 초기화를 수정하여 1초 주기 토글되도록 수정)
//점멸등 6.4이하 활성화시에 버튼이 눌리지 않도록 수정
//5분 카운트동안 전원 버튼을 누르면 전원이 켜지는 데 후에 버튼이 작동되지 않는 현상 수정 -> 전원꺼짐 모드 임으로 전원버튼 인터럽트가 되어도 led가 켜지지 않도록 수정
//어댑터를 뺏을때 아직 5분 루틴이 돌고 있는 문제점 발견
//1.8.9
//전원 버튼으로 동작
//전원 연결시 부팅과 같은 led 꺼짐 켜짐
//어댑터 인터럽트 하강 상승 둘다 활성화
//동작중에 어댑터 들어가면 동작이 멈추지 않는 현상 수정
//1.9.0
//베터리 없이 외부전원만 넣었을때 동작되지 않도록 수정 -> ADC이용하여 수정완료
//충전시에 충전 LED 깜빡거리게 수정
//1.9.1
//6.8이하 충전필요 깜빡임 + 6.4이하 동작 멈춤 및 전원이 꺼져도 충필 led 깜빡이도록
//버튼 다시 눌리지 않게 수정
//1.9.2
//코드 수정
//2.1.2
//타임테이블 변경
//2.1.3
//타임테이블 최종 수정
//2.1.6
//자세조정led
//2.1.9
//
#include <msp430.h>
#include <Neo-SM-Mode-v2.1.9.h>
#include <Neo-SM-v2.1.9.h>
//buzzer
void Beep1(void);
void Beep2(void);
void Timer_A_init(void);
//basic init
void clock_init(void);
void IOPort_init(void);
void Timer_B7_init(void);
void UART_Init(void);
//flash & pressure
void flash_write(void);
void flash_read(void);
void flashData_init(void);
void rcv_proc(void);
void UCA0_strout_length(UBYTE *tx_str, UBYTE length);
void UCA0_chout(unsigned char tx_ch);
void Pressure_Data_Send(void);
unsigned long ReadCount1(void);
unsigned long ReadCount2(void);
unsigned long ReadCount3(void);
//btn
void Default_Button_Input(void);
void Auto_Button_Input(void);
void Air_Button_Input(void);
void Power_Button_Input(void);
void Blt_Input(void);
void Init_Pressure_Input(void);
//power
void Power_off_ready(void);
//mode
void setValveAndPumpState(bool is_pump, bool is_right_valve, bool is_left_valve, bool is_center_valve, bool is_out_valve);

int main(void)
{
    unsigned char poweroff_cnt;
    unsigned char init_cnt;
    unsigned char cnt_delay;
    init_cnt     = 0;
    poweroff_cnt = 0;
    cnt_delay    = 0;
    WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer

    _DINT();
    clock_init();
    IOPort_init();
    control_init();
    Timer_B7_init();
    Timer_A_init();
    UART_Init();
    flashData_init();
    __delay_cycles(3000000);          //Test
    _EINT();

    while(1)
    {
        _DINT();
        ADC12CTL0 |= ADC12SC; //베터리전압
        __low_power_mode_0();

        __no_operation();
        _EINT();
        if(!is_under_6_4 && !is_charging_mode)//충전필요 모드가 아닐때 버튼 감지
        {
            Power_Button_Input();
        }

        if(!is_charging_mode && (P1IN&IN_SENSE))
        {
          controlValves(is_pump, is_right_valve, is_left_valve, is_center_valve, is_out_valve);

          if(cnt_basic_L == 1 || cnt_basic_F == 1 || cnt_auto == 1 || cnt_auto_basic == 1 || cnt_auto_basic == 2400)
          {
              setValveAndPumpState(false,true,true,true,true);
          }
          else if(cnt_auto_basic == 30 || cnt_basic_L==31 || cnt_basic_L==1861 || cnt_basic_F == 31 || cnt_basic_F == 1861) //Basic_Valve_Close
          {
              setValveAndPumpState(is_pump,false,false,false,false);
          }
          else if( cnt_auto_basic == 32 || cnt_basic_L==33 || cnt_basic_L==1863 || cnt_basic_F == 33 || cnt_basic_F == 1863) //Center_in
          {
              setValveAndPumpState(true,is_right_valve ,is_left_valve ,true,is_out_valve );
          }
          else if(cnt_auto == 541 || cnt_auto == 631 || cnt_auto == 721 || cnt_auto == 811
                  || cnt_auto == 1441 || cnt_auto == 1531 || cnt_auto == 1621 || cnt_auto == 1711)//Center_close_open
          {
              setValveAndPumpState(true,is_right_valve ,is_left_valve ,true,false );
          }
          else if( cnt_auto_basic == 42 || cnt_basic_L==43 || cnt_basic_F == 45) //Center_Low_right_left_open
          {
              setValveAndPumpState(is_pump, true, true, false, is_out_valve);
          }
          else if(cnt_auto == 1801) //Center_out
          {
             setValveAndPumpState(is_pump, is_right_valve, is_left_valve, false, false);
          }
          else if(cnt_auto == 1393 || cnt_auto == 943)//Center_close_Right_open_4
          {
              setValveAndPumpState(is_pump, true, is_left_valve, false, is_out_valve);
          }
          else if(cnt_auto == 550 || cnt_auto == 640 || cnt_auto== 1453 || cnt_auto == 1543)//Center_close_Right_open_2
          {
            setValveAndPumpState(false, true, is_left_valve, false, true);
          }
          else if(cnt_auto == 730 || cnt_auto == 1633)//Center_close_Right_open_Left_open_1
          {
              setValveAndPumpState(false, true, true, false, true);
          }
          else if(cnt_auto == 820 || cnt_auto == 1723)//Center_close_Left_open
          {
              setValveAndPumpState(false, is_right_valve, true, false, true);
          }
          else if( cnt_auto_basic == 59 || cnt_basic_L == 59 || cnt_basic_F == 65) //Right_High
          {
              setValveAndPumpState(false, false, false, is_center_valve, is_out_valve);
          }
          else if( cnt_basic_L==1889 || cnt_basic_L==1961
                  || cnt_basic_F == 1889 || cnt_basic_F == 1961
                  || cnt_basic_F == 2021 || cnt_auto == 67 || cnt_auto == 157 || cnt_auto == 247
                  || cnt_auto == 337 || cnt_auto == 427 || cnt_auto == 757 || cnt_auto == 970
                  || cnt_auto == 1060 || cnt_auto == 1150 || cnt_auto == 1240 || cnt_auto == 1330
                  || cnt_auto == 1660 || cnt_basic_L == 2021 )//Left_close
          {
              setValveAndPumpState(false, is_right_valve, false, is_center_valve, is_out_valve);

          }
          else if(cnt_auto == 517 || cnt_auto == 697 || cnt_auto == 1420 || cnt_auto == 1510
                  || cnt_auto == 1600 || cnt_auto == 607)//Left_close_Center_open
           {
              setValveAndPumpState(false, is_right_valve, false, true, true);
           }
           else if(cnt_auto == 847 || cnt_auto == 1750)//Left_close_Right_open
           {
               setValveAndPumpState(false, true, false, is_center_valve, true);
           }
           else if (cnt_auto == 1231)
           {
               setValveAndPumpState(true, is_right_valve, true, is_center_valve, false);
           }
          else if(cnt_basic_L==1833 || cnt_basic_F == 1833 || cnt_auto == 451
                  || cnt_auto == 1351 || cnt_auto == 901) //All_Air_Out
          {
              setValveAndPumpState(is_pump, true, true, true, true);
          }
          else if(cnt_auto == 31)//Valve_Close
          {
              setValveAndPumpState(true, false, false, is_center_valve, false);
          }
          else if( cnt_auto == 481)//Valve_Close_Center_Open
          {
              setValveAndPumpState(true, false, false, true, false);
          }
          else if(cnt_auto == 40 || cnt_auto == 490)//Center_close_Right_open
          {
              setValveAndPumpState(is_pump, true, is_left_valve, false, is_out_valve);
          }
          else if(cnt_basic_L==1873 || cnt_basic_F == 1873) //Center_close_Right_open_3
          {
              setValveAndPumpState(is_pump, true, is_left_valve, false, is_out_valve);
          }
          else if(cnt_basic_L==1881 || cnt_basic_F == 1881) //Right_close_Left_open_2
          {
              setValveAndPumpState(is_pump, false, true, is_center_valve, is_out_valve);
          }
          else if(cnt_auto == 121 || cnt_auto == 211 || cnt_auto == 301 || cnt_auto == 391 ||
                  cnt_auto == 571 || cnt_auto == 661 || cnt_auto == 871 || cnt_auto == 1021 ||
                  cnt_auto == 1111 || cnt_auto == 1201 || cnt_auto == 1291 || cnt_auto == 1471 ||
                  cnt_auto == 1561 || cnt_auto == 1771)//Right_close_open
          {
              setValveAndPumpState(true, true, is_left_valve, is_center_valve, false);
          }
          else if(cnt_basic_L==1911 || cnt_basic_L==1971 || cnt_basic_F == 1911 ||
                  cnt_basic_F == 1971 || cnt_auto == 91 || cnt_auto == 181 || cnt_auto == 271 ||
                  cnt_auto == 361 || cnt_auto == 991 || cnt_auto == 1081 || cnt_auto == 1171 ||
                  cnt_auto == 1261) //Right_out
          {
              setValveAndPumpState(is_pump, true, is_left_valve, is_center_valve, true);

          }
          else if(cnt_auto == 931 || cnt_auto == 1381)//Right_close_Left_close_Center_open
          {
              setValveAndPumpState(true, false, false, true, false);
          }
          else if(cnt_auto == 127 || cnt_auto == 217 || cnt_auto == 307 || cnt_auto == 397 ||
                  cnt_auto == 577 || cnt_auto == 667 || cnt_auto == 1030 || cnt_auto == 1120 ||
                  cnt_auto == 1210 || cnt_auto == 1300 || cnt_auto == 1480 || cnt_auto == 1570)//Right_close_Left_open
          {
              setValveAndPumpState(false, false, true, is_center_valve, true);
          }
          else if(cnt_auto == 151 || cnt_auto == 241 || cnt_auto == 331 || cnt_auto == 421 ||
                  cnt_auto == 601 || cnt_auto == 691 || cnt_auto == 841 ||
                  cnt_auto == 1051 || cnt_auto == 1141 || cnt_auto == 1321 || cnt_auto == 1501 ||
                  cnt_auto == 1591 || cnt_auto == 1741 )//Left_close_open
          {
              setValveAndPumpState(true, is_right_valve, true, is_center_valve, false);
          }
          else if(cnt_auto == 751 || cnt_auto == 1651)//Right_close_Left_close_open
          {
              setValveAndPumpState(true, false, true, is_center_valve, false);
          }
          else if(cnt_basic_L==1921 || cnt_basic_L==1981 || cnt_basic_F == 1921 || cnt_basic_F == 1981) //Right_in
          {
              setValveAndPumpState(is_pump, false, is_left_valve, is_center_valve, false);
          }
          else if(cnt_basic_L==1923 || cnt_basic_F == 1923 || cnt_auto == 781 || cnt_auto == 1681
                  || cnt_basic_L == 1983 || cnt_basic_F == 1983 ) //Right_open
          {
              setValveAndPumpState(true, true, is_left_valve, is_center_valve, is_out_valve);
          }
          else if(cnt_basic_L==1991 || cnt_basic_F == 1931 || cnt_basic_F == 1991 || cnt_auto == 46
                  || cnt_auto == 496 || cnt_auto == 952 || cnt_auto == 1402 || cnt_basic_L == 1931) //Right_close
          {
              setValveAndPumpState(false, false, is_left_valve, is_center_valve, is_out_valve);
          }
          else if(cnt_auto == 787 || cnt_auto == 877 || cnt_auto == 1690 || cnt_auto == 1780) //Right_close_Center_open
          {
              setValveAndPumpState(false, false, is_left_valve, true, true);
          }
          else if(cnt_basic_L==1941 || cnt_basic_L==2001 || cnt_basic_F == 1941 || cnt_basic_F == 2001)//Left_out
          {
              setValveAndPumpState(is_pump, is_right_valve, true, is_center_valve, true);
          }
          else if(cnt_basic_L==1951 || cnt_basic_L==2011 || cnt_basic_F == 1951 || cnt_basic_F == 2011)//Left_in
          {
              setValveAndPumpState(is_pump, is_right_valve, false, is_center_valve, false);
          }
          else if(cnt_basic_L == 1953|| cnt_basic_L==2013 || cnt_basic_F == 1953 || cnt_basic_F == 2013
                  || cnt_auto == 61 || cnt_auto == 511 || cnt_auto == 961 || cnt_auto == 1411)//Left_open
          {
              setValveAndPumpState(true, is_right_valve, true, is_center_valve, is_out_valve);
          }
          else if(cnt_auto_basic>2402)
          {
              cnt_auto_basic = 0;
              is_auto_basic  = 0;
              is_auto = 1;
          }
          else if(cnt_basic_L>2041)
          {
              cnt_basic_L = 0;
          }
          else if(cnt_basic_F>2041)
          {
                cnt_basic_F = 0;
          }
          else if(cnt_auto>1805)
          {
              cnt_auto = 0;
              is_auto = 0;
              cnt_auto_mode++;
              if(cnt_auto_mode < 5)
              {
                  is_auto_basic = 1;
              }
          }
          else if(cnt_auto_mode==5)
          {
              is_auto = 0;
              is_auto_basic = 0;
              cnt_auto_mode = 0;
              is_basic = 1;
              is_air_L = 1;
              is_air_F = 0;
              cnt_auto = 0;
              beep1_en = true;
          }

            if (f_50ms) // 기본+ 자동 모드 버튼, 공기조절버튼 확인
            {
                f_50ms = 0;
                if(is_power && !is_charging_mode)
                {
                    Default_Button_Input();
                    Auto_Button_Input();
                    Air_Button_Input();
                    Blt_Input();
                    Init_Pressure_Input();
                }
                if(is_power == 0 && is_first)//켜진 상태에서 전원 종료
                {
                    Power_off_ready();
                    P5OUT &= ~(air_low_led | air_full_led);
                    P6OUT &= ~(charged_led | charging_led | basic_led | auto_led);

                    __bis_SR_register(LPM3_bits + GIE); //슬립모드 진입
                }
                if(is_power)
                {
                      if(Air_btn && B_btn && A_btn)
                      {
                          //P2.2 블루투스
//                          if (P2OUT & BIT2)
//                          {
//                              P2OUT &= ~BIT2; // P2.2 핀의 출력을 낮춤
//                              beep1_en = true;
//                          }
//                          else
//                          {
//                              P2OUT |= BIT2; // P2.2 핀의 출력을 높임
//                              beep2_en = true;
//                          }

                          Air_btn = 0;
                          A_btn   = 0;
                          B_btn   = 0;
                      }
                      else if(!is_first) //전원이 켜질때
                      {
                        if(f_500ms)
                        {
                            f_500ms=0;
                            is_first_6_4=true;
                            P6OUT &= ~charging_led;
                            beep1_en = true;
                            is_first = true;
                            is_basic    = 1;
                            is_air_L    = 1;
                            is_air_F    = 0;
                            is_auto     = 0;
                            cnt_auto    = 0;
                            cnt_basic_F = 0;
                            cnt_basic_L = 0;
                        }
                    }
                    if(is_first)
                    {
                        if(is_basic && is_air_L)//공기량 보통 LED
                        {
                            if(is_auto==0)
                            {
                                P5OUT &= ~(air_low_led | air_full_led );
                                P5OUT |= (air_low_led);
                            }

                        }
                        else if(is_basic && is_air_F)//공기량 많음 LED
                        {
                            P5OUT &= ~(air_low_led | air_full_led);
                            P5OUT |= (air_full_led);

                        }

                        if(is_basic && is_auto==0) //기본모드 LED
                        {
                            P6OUT &= ~auto_led;
                            P6OUT |= basic_led;

                        }
                        else if(is_auto) // 자동모드 LED
                        {
                            P5OUT &= ~(air_low_led | air_full_led);
                            P6OUT &= ~basic_led;
                            P5OUT |= air_low_led;
                            P6OUT |= auto_led;

                        }
                    }
                }


                Pressure_Data_Send();
            } //50ms
            if(f_100ms)
            {
                  _DINT();
                  P1_Value = ReadCount1();
                  P2_Value = ReadCount2();
                  P3_Value = ReadCount3();
                  _EINT();
                  f_100ms = 0;
                  battery_voltage = (float)adc_value * 8.4 / 3276.0; // 12비트 ADC 해상도, 0~2.5V 입력 범위를 12V까지 높임

                  if (P1_Value > P3_Value) {
                      position_abs = P1_Value - P3_Value;
                      position_max = P1_Value*0.2;
                  } else {
                      position_abs = P3_Value - P1_Value;
                      position_max = P3_Value*0.2;
                  }
                  if(is_auto)
                  {
                      is_position  = false;
                      is_low_pos   = false;
                      is_high_pos  = false;
                      cnt_low_pos  = 0;
                      cnt_position = 0;
                      P6OUT &= ~position_led;
                  }
                  else if((is_auto_basic || is_basic) && position_abs > 3)
                  {
                      is_position = true;
                  }

                  if(cnt_low_pos >59)//59
                  {
                      cnt_low_pos = 0;
                      cnt_position = 0;
                      is_position  = false;
                      P6OUT &= ~position_led;
                  }
                  if(cnt_position > 179)
                  {
                      cnt_position = 0;
                      P6OUT |= position_led;
                  }

             }

              __no_operation();
       }//전원 잭이 꼽혀 있지 않을때 동작하는 것
          if (f_1sec)
          {
              if(!is_init_connect && is_position && !isnan(position_result))
              {
                 if(position_abs > position_max)
                 {
                     cnt_low_pos = 0;
                     cnt_position++;
                 }
                 else if(position_abs < position_max)
                 {
                     cnt_position = 0;
                     cnt_low_pos++;
                 }
              }
              if(is_poweroff_5min) //5분후에 끌 수 있도록 카운팅 시작하는 조건
              {
                  poweroff_cnt++;
                  is_under_6_4 = true;
              }
              if(is_1sec_led)
              {
                  P6OUT ^= charging_led;
              }
              else if((is_1sec_led && poweroff_cnt > 300)) // 5분후에 절전모드 진입
              {
                  P6OUT &= ~charging_led;
                  poweroff_cnt = 0;
                  is_under_6_4 = false;
                  is_1sec_led = false;
                  __bis_SR_register(LPM3_bits + GIE);

              }
              if(battery_voltage<=6.4 && is_first_6_4 && is_power)//전원 키자마자 6.4이하일때
              {
                  is_1sec_led = true;
                  if(is_first)
                  {
                      cnt_delay++; //3초를 세기 위한 카운트
                  }

                 if(cnt_delay>=4) //3초 후에 종료 루틴 시작(5분 깜빡거리는)
                 {
                      is_first_6_4 = false;
                      Power_off_ready();
                      is_poweroff_5min = true;
                      poweroff_cnt = 0;
                      cnt_delay=0;

                 }
              }
              if(battery_voltage<=6.8 && is_power)
              {
                  is_1sec_led = true;
                  if(battery_voltage<=6.4)
                  {
                      Power_off_ready();
                      is_1sec_led = false;
                      poweroff_cnt = 0;
                      __bis_SR_register(LPM3_bits + GIE);
                  }
              }
             f_1sec = 0;
         }
          if(f_20ms)
          {
              if(is_init_connect && (battery_voltage>6.4)) //맨처음 hello world
              {
                  init_cnt++;
                  //P4OUT |= BIT7;
                  P5OUT |= (air_low_led | air_full_led);
                  P6OUT |= (position_led | charged_led | charging_led | basic_led | auto_led);
                  if(is_init_beep)
                  {
                      beep1_en = true;
                      is_init_beep = false;
                  }
              }
              if(init_cnt>10)
              {
                  is_init_connect = false;
                  init_cnt = 0;
                  P5OUT &= ~(air_low_led | air_full_led);
                  P6OUT &= ~(position_led | charged_led | charging_led | basic_led | auto_led);
              }


                if(beep1_en)
                {
                    Beep1();
                }
                if(beep2_en)
                {
                    Beep2();
                }
                f_20ms=0;

          }
         if (f_250ms)
         {

            if(!(P1IN&IN_SENSE)&& !is_charging_mode)//충전어댑터 체크 후 처음 넣었을때
            {
                Power_off_ready();
                P5OUT &= ~(air_low_led | air_full_led);
                P6OUT &= ~(charged_led | charging_led | basic_led | auto_led);

                is_charging_mode = true;

                //5분 루틴 exit
                poweroff_cnt = 0;
                is_under_6_4 = false;
                is_power = 0;
                is_1sec_led = true;

            }
            else if(P1IN&IN_SENSE) // 어댑터 뺐을때
            {
                if(is_charging_mode)
                {
                    is_charging_mode = false;
                    is_1sec_led      = false;
                    P6OUT &= ~( charged_led | charging_led );
                    __bis_SR_register(LPM3_bits + GIE);
                }
            }

            if(!(P1IN&IN_SENSE) && is_charging_mode) //어댑터가 넣어져 있고 충전모드일때
            {
                if((P1IN&STAT1) && (P1IN&STAT2) && is_charging_first)//완충
                {
                    is_charging_first=false;
                    is_1sec_led      = false;
                    P6OUT &= ~charging_led;
                    P6OUT |= charged_led;


                }
                else if((P1IN&STAT1) && !(P1IN&STAT2))
                {
                    P6OUT &= ~charged_led; //Off
                    is_charging_first=true;
                    is_1sec_led = true; //on
                }
            }

           f_250ms = 0;
        }//f_250ms

  }
}

void setValveAndPumpState(bool pump, bool right_valve, bool left_valve, bool center_valve, bool out_valve) {
    is_pump = pump;
    is_right_valve = right_valve;
    is_left_valve = left_valve;
    is_center_valve = center_valve;
    is_out_valve = out_valve;
}

void Power_off_ready(void)
{
      Power_Off();
      TACCR0 = 0;
      TACCR2 = 0;
      is_right_valve   = false;
      is_left_valve    = false;
      is_center_valve  = false;
      is_out_valve     = false;
      is_pump          = false;
      is_power = 0;
      is_basic = 0;
      is_auto  = 0;
      is_air_L = 0;
      is_air_F = 0;
      is_auto_basic = 0;
      P4OUT &= ~BIT7;
      P2OUT &= ~BIT2; //블루투스 끄기
      P5OUT &= ~(air_low_led | air_full_led);
      P6OUT &= ~(position_led | charged_led | charging_led | basic_led | auto_led);
      is_low_pos = false;
      is_position = false;
      cnt_position = 0;
      cnt_low_pos = 0;
}

void clock_init(void)
{
    uint8_t ii;
    DCOCTL = 0;
    BCSCTL1 &= ~XT2OFF;                       // XT2= HF XTAL .

    do {
        IFG1 &= ~OFIFG;                       // Clear OSCFault flag
        for (ii = 0xFF; ii > 0; ii--);        // Time for flag to set
    }
    while ((IFG1 & OFIFG));                   // OSCFault flag still set?
    BCSCTL2 |= SELM_2 | SELS;                 // MCLK= XT2 (safe), SMCLK = XT2
}

void Timer_B7_init(void)
{
    TBCCTL0 = CCIE;                           // TBCCR0 interrupt enabled
    TBCCR0 = (UINT)(CLK_11MHz/1000);
    TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, upmode

}

void Timer_A_init(void)
{
    // Timer_A 설정
    TA0CCTL0 = CCIE; // CCR0 인터럽트 사용
    TA0CTL = TASSEL_1 + MC_1;  // ACLK을 소스로 사용, Up Mode
    TA0CCTL2 = OUTMOD_7;       // CCR2을 사용하여 출력을 제어

}

void UART_Init(void)
{
    // UART Port0 설정 및 속도, UART Interrupt 설정
    // RS-232C
    P3DIR &= ~BIT5;                 // P3.5 Port를 Input으로 설정(RX)
    P3SEL |= 0x30;                  // P3.4,5 = USART0 Tx0/Rx0
    UCTL0 = CHAR;                   // 8-bit character
    UTCTL0 = SSEL1+SSEL0;           // UCLK = SMCLK

    UBR00 = 0x60;                   // 11.0592Mhz/115200 = 96 = 0x060.
    UBR10 = 0x00;                   //

    UMCTL0 = 0x00;                  // no modulation
    ME1 |= UTXE0 + URXE0;           // Enable USART0 TXD/RXD

    UCTL0 &= ~SWRST;                // Initalize USART state machine
    IE1 |= URXIE0;                  // Enable USART0 RX interrupt
}

void IOPort_init(void)
{
    // P1.0, P1.1, P1.2, P1.3, P1.4, P1.5, P1.6를 Input으로 설정
    P1DIR &= ~( BIT1 | BIT2 | BIT3 );
    P1DIR &= ~(POWER_SW | DEFAULT_MODE | AUTO_MODE |AIR_VOLUME); //스위치 Input으로 설정

    // P1.0 핀의 하강 에지에 대한 인터럽트를 활성화합니다.
    P1IES &= ~POWER_SW;
    P1IE  |= POWER_SW; // P1.0 인터럽트 활성화

    P1IES &= ~IN_SENSE;
    P1IES |= IN_SENSE;
    P1IE  |= IN_SENSE;

    //P1.7 Input 설정 - Buzzer 추가
    P1SEL |= BIT7;
    P1DIR |= BIT7;

    //블루투스
    P2DIR |= BIT2;
    P2OUT &= ~BIT2;
    //P2OUT |= BIT2;

    // P4.0, P4.2, P4.4을 Input으로 설정 - Data In from HX710B
    P4DIR &= ~(BIT0 | BIT2 | BIT4);

    // P4.1, P4.3, P4.5, P4.7(모니터링 LED)을 Output으로 설정 - Clock Out to HX710B
    P4DIR |= BIT1 | BIT3 | BIT5 | BIT7;
    P4OUT &= ~BIT7;

    // P5.6, P5.7을 Output으로 설정 - LED 2개
    P5DIR |= BIT6 | BIT7;

    // P6.3, P6.4, P6.5,P6.6,P6.7을 Output으로 설정 - LED
    P6DIR |= BIT3 | BIT4 | BIT5 | BIT6 | BIT7;

    // P6.0 베터리전압 ADC 설정
    P6SEL |= BIT0;
    ADC12CTL0 = REFON + REF2_5V+SHT0_2 + ADC12ON; //2클럭 샘플 홀드 타임
    ADC12CTL1 = SHP;
    ADC12MCTL0 = SREF_1;
    ADC12IE = BIT0;
    ADC12CTL0 |= ENC;

    //led 초기화
    P5OUT &= ~(air_low_led | air_full_led);
    P6OUT &= ~(position_led | charged_led | charging_led | basic_led | auto_led);

}


void Power_Button_Input(void)    // P1.0
{
    Init_in_stat = P1IN;
    Init_in_stat &= POWER_SW;

    Init_input_stat = Init_in_stat;

    if((Init_old_input_stat == Init_input_stat) && (Init_input_stat !=0)) {
          if((Init_key_counter >= 2)&&(Init_zero_count>=5)) {
              Init_zero_count = 0;
              P6OUT &= ~( charged_led | charging_led );
              if(is_power)
              {
                  is_power = 0;
              }
              else
              {
                  is_power = 1;
              }

              cnt_auto    = 0;
              cnt_basic_F = 0;
              cnt_basic_L = 0;
              cnt_auto_basic = 0;
              cnt_auto_mode  = 0;

          }
          else {
              ++Init_key_counter;
          }
    }
    else {
          ++Init_zero_count;
          Init_key_counter = 0;                  // 연속 Check timer clear
          Init_old_input_stat = Init_input_stat;
          //input_flag = 0;
    }
}

void Blt_Input(void) //블루투스3버튼
{
    blt_in_stat1 = ~P1IN;
    blt_in_stat2 = ~P1IN;
    blt_in_stat3 = ~P1IN;

    blt_in_stat1 &= DEFAULT_MODE;
    blt_in_stat2 &= AUTO_MODE;
    blt_in_stat3 &= AIR_VOLUME;

    blt_input_stat1 = blt_in_stat1;
    blt_input_stat2 = blt_in_stat2;
    blt_input_stat3 = blt_in_stat3;

    if((blt_old_input_stat1 == blt_input_stat1) && (blt_input_stat1 !=0)
        && (blt_old_input_stat2 == blt_input_stat2) && (blt_input_stat2 !=0)
        && (blt_old_input_stat3 == blt_input_stat3) && (blt_input_stat3 !=0))
    {
        if((blt_key_counter1 >= 2)&&(blt_zero_count1>=5))
        {
            blt_zero_count1=0;
            Air_btn = 1;
            A_btn   = 1;
            B_btn   = 1;
        }
        else
        {
            ++blt_key_counter1;
        }
    }
    else
    {
        ++blt_zero_count1;
        blt_key_counter1 = 0;
        blt_old_input_stat1 = blt_input_stat1;
        blt_old_input_stat2 = blt_input_stat2;
        blt_old_input_stat3 = blt_input_stat3;
    }
}

void Init_Pressure_Input(void) //압력 센서 초기화
{
    press_in_stat1 = ~P1IN;
    press_in_stat2 = ~P1IN;

    press_in_stat1 &= DEFAULT_MODE;
    press_in_stat2 &= AIR_VOLUME;

    press_input_stat1 = press_in_stat1;
    press_input_stat2 = press_in_stat2;

    if((press_old_input_stat1 == press_input_stat1) && (press_input_stat1 !=0)
        && (press_old_input_stat2 == press_input_stat2) && (press_input_stat2 !=0))
    {
        if((press_key_counter1 >= 2)&&(press_zero_count1>=5))
        {
            press_zero_count1=0;
            init_pressure = true;
        }
        else
        {
            ++press_key_counter1;
        }
    }
    else
    {
        ++press_zero_count1;
        press_key_counter1 = 0;
        press_old_input_stat1 = press_input_stat1;
        press_old_input_stat2 = press_input_stat2;
    }
}

void Default_Button_Input(void)    // P1.4
{
    Init_in_stat1 = ~P1IN;
    Init_in_stat1 &= DEFAULT_MODE;

    Init_input_stat1 = Init_in_stat1;
    if((Init_old_input_stat1 == Init_input_stat1) && (Init_input_stat1 !=0)) {

        if((Init_key_counter1 >= 1)&&(Init_zero_count1>=1)) {
              if(!is_basic)
              {
                  Init_zero_count1 = 0;
                  beep1_en = true;
                  is_basic= 1;
                  is_auto = 0;
                  is_air_F= 0;
                  is_air_L= 1;
                  is_auto_basic = 0;

                  cnt_auto    = 0;
                  cnt_basic_F = 0;
                  cnt_basic_L = 0;
                  cnt_auto_basic = 0;
                  cnt_auto_mode  = 0;
              }
          }
          else {
              ++Init_key_counter1;
          }
    }
    else {
          ++Init_zero_count1;
          // 연속 Check timer clear
          Init_key_counter1 = 0;
          Init_old_input_stat1 = Init_input_stat1;
    }
}

void Auto_Button_Input(void)    // P1.5
{
    Init_in_stat2 = ~P1IN;
    Init_in_stat2 &= AUTO_MODE;

    Init_input_stat2 = Init_in_stat2;
    if((Init_old_input_stat2 == Init_input_stat2) && (Init_input_stat2 !=0)) {

          if((Init_key_counter2 >= 1)&&(Init_zero_count2>=1)) {
                if(!is_auto)
                {
                    Init_zero_count2 = 0;
                    is_auto=1;
                    is_basic=0;
                    is_air_F=0;
                    is_air_L=1;
                    is_auto_basic = 0;

                    beep1_en = true;
                    cnt_auto    = 0;
                    cnt_basic_F = 0;
                    cnt_basic_L = 0;
                    cnt_auto_basic = 0;
                    cnt_auto_mode  = 0;
                }


            }
            else {
                ++Init_key_counter2;
            }

    }
    else {
          ++Init_zero_count2;

          Init_key_counter2 = 0;// 연속 Check timer clear
          Init_old_input_stat2 = Init_input_stat2;
    }
}

void Air_Button_Input(void)    // P1.6
{
    Init_in_stat3 = ~P1IN;
    Init_in_stat3 &= AIR_VOLUME;

    Init_input_stat3 = Init_in_stat3;
    if((Init_old_input_stat3 == Init_input_stat3) && (Init_input_stat3 !=0)) {

        if((Init_key_counter3 >= 1)&&(Init_zero_count3>=1)) {
              if(is_basic)
              {
                  Init_zero_count3 = 0;
                  beep1_en = true;
                  if(is_air_L)
                  {
                    is_air_F=1;
                    is_air_L=0;
                  }
                  else
                  {
                    is_air_F=0;
                    is_air_L=1;
                  }
                  cnt_basic_F = 0;
                  cnt_basic_L = 0;
              }

          }
          else {
              ++Init_key_counter3;
          }
    }
    else {
          ++Init_zero_count3;

          // 연속 Check timer clear
          Init_key_counter3 = 0;
          Init_old_input_stat3 = Init_input_stat3;
    }
}

void Beep1(void)
{

    if(beep_300ms>=3)
    {
        beep_300ms=0;
        beep1_en = false;
        TACCR0 = 0;
        TACCR2 = 0;
    }
    else if(beep1_en)
    {
        TACCR0 = 30;   // CCR0 값을 설정
        TACCR2 = 15;   // CCR2 값을 설정 (CCR0의 절반)
    }
}

void Beep2(void)
{

    if(beep_100ms==2)//5
    {
        TACCR0 = 0;
        TACCR2 = 0;
    }
    else if(beep_100ms==5)
    {
        TACCR0 = 30;   // CCR0 값을 설정
        TACCR2 = 15;
    }
    else if(beep_100ms>=8)
    {
        beep_100ms=0;
        beep2_en = false;
        TACCR0 = 0;   // CCR0 값을 설정
        TACCR2 = 0;
    }
    else if(beep2_en)
    {
        TACCR0 = 30;   // CCR0 값을 설정
        TACCR2 = 15;   // CCR2 값을 설정 (CCR0의 절반)
    }
}

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
    bf_data[9] = P_Init_Val5/256;
    bf_data[10] = P_Init_Val5%256;
    bf_data[11] = P_Init_Val6/256;
    bf_data[12] = P_Init_Val6%256;
    bf_data[13] = P_Init_Val7/256;
    bf_data[14] = P_Init_Val7%256;
    bf_data[15] = P_Init_Val8/256;
    bf_data[16] = P_Init_Val8%256;

    free_byte0 = 0;
    for(kk=1; kk<17; ++kk) {
        free_byte0+=bf_data[kk];
    }
    bf_data[0] = 0xFF-free_byte0;
//------------------------------------------------------------------------------
    flash_ptr = (unsigned char *) 0x1000;   // Initialize Flash pointer
        _DINT();                        // disable interrupts
    FCTL2 = FWKEY+FSSEL0+FN4+FN3+FN2+FN1;   // MCLK/30 for Flash Timing Generator
    flash_ptr = (unsigned char *) 0x1000;
    FCTL1 = FWKEY + ERASE;          // Set Erase bit
    FCTL3 = FWKEY;                  // Clear Lock bit
    *flash_ptr = 0;                 // Dummy write to erase Flash segment
    FCTL1 = FWKEY + WRT;            // Set WRT bit for write operation
//------------------------------------------------------------------------------

    for(kk=0; kk<17; ++kk){
        *flash_ptr++ = bf_data[kk];
    }

    FCTL1 = FWKEY;                  // Clear WRT bit
    FCTL3 = FWKEY + LOCK;               // Set LOCK bit
    _EINT();                            // __enable_interrupt() -> D\program\IAR_MSP430\Embed..\430\inc\in430.h

    flash_read();
}

void flash_read(void)
{
    UBYTE jj;
    unsigned char *flash_ptr;       // Flash pointer

    flash_ptr = (unsigned char *) 0x1000;   // Initialize Flash pointer

    for(jj=0; jj<17; ++jj){
        bf_data[jj] = *flash_ptr++;
    }

    P_Init_Val1 = bf_data[1]*256 + bf_data[2];
    P_Init_Val2 = bf_data[3]*256 + bf_data[4];
    P_Init_Val3 = bf_data[5]*256 + bf_data[6];
    P_Init_Val4 = bf_data[7]*256 + bf_data[8];
    P_Init_Val5 = bf_data[9]*256 + bf_data[10];
    P_Init_Val6 = bf_data[11]*256 + bf_data[12];
    P_Init_Val7 = bf_data[13]*256 + bf_data[14];
    P_Init_Val8 = bf_data[15]*256 + bf_data[16];
}

void flashData_init(void)
{
    uint8_t ii;
    // flash memory를 읽어 설정값을 set한다..
    flash_read();
    free_byte0 = 0;
    for(ii=1; ii<17; ++ii) {
        free_byte0+=bf_data[ii];
    }
    free_byte0 = 0xFF-free_byte0;
    if(free_byte0==bf_data[0]) ;
    else {                              // 한번 더 읽음.
        free_byte0 = 0;
        flash_read();
        for(ii=1; ii<17; ++ii) {
            free_byte0+=bf_data[ii];
        }
        free_byte0 = 0xFF-free_byte0;
        if(free_byte0==bf_data[0]) ;
        else {  // 플레쉬 메모리에 데이터가 없는 경우 이므로 데이터를 초기화 시켜줌
            P_Init_Val1 = 204;
            P_Init_Val2 = 204;
            P_Init_Val3 = 204;
            P_Init_Val4 = 204;
            P_Init_Val5 = 204;
            P_Init_Val6 = 204;
            P_Init_Val7 = 204;
            P_Init_Val8 = 204;

            flash_write();
        }
    }
}

void rcv_proc(void)
{
    f_rcv_proc = false;
    rcv_command = rcv_buf[3];
    Pressure_Data_Send();
//    switch(rcv_command)
//    {
//        case 0x81:
//            Pressure_Data_Send();
//            break;
//        default:
//            break;
//    }
    __no_operation();
}

void Pressure_Data_Send(void)
{
    uint8_t i;
    trs_buf0[0] = 0x7E;
    trs_buf0[1] = 0xE7;
    trs_buf0[2] = 0x13;
    trs_buf0[3] = 'S';
    trs_buf0[4] = 0x01;
    trs_buf0[5] = 0x81;
    trs_buf0[6] = (P1_Value >> 8) & 0xFF;
    trs_buf0[7] = P1_Value & 0xFF;
    trs_buf0[8] = (P2_Value >> 8) & 0xFF;
    trs_buf0[9] = P2_Value & 0xFF;
    trs_buf0[10] = (P3_Value >> 8) & 0xFF;
    trs_buf0[11] = P3_Value & 0xFF;

    free_byte0 = 0;
    for(i = 0; i<11; i++){
        free_byte0 += trs_buf0[i];
    }
    free_byte0 = 0xFF - free_byte0; // Checksum 계산
    trs_buf0[12] = free_byte0;

    UCA0_strout_length(trs_buf0, 13);
}



void UCA0_strout_length(UBYTE *tx_str, UBYTE length)
{
    UBYTE i;
    for(i=0; i<length; i++) {
        UCA0_chout(*(tx_str+i));
    }
}

void UCA0_chout(unsigned char tx_ch)
{
    while(!(IFG1 & UTXIFG0));
    TXBUF0 = tx_ch;
}

unsigned long ReadCount1(void)
{
    unsigned long Count;
    unsigned char i;
    unsigned long Count_24bit;
    uint16_t Return_Value;

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
    if(init_pressure){
        P_Init_Val1 = (uint16_t)(Pressure_KPA*10);
    }
    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val1);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}

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

    while(P4IN & ADDO2);

    for (i = 0; i < 24; i++){
        P4OUT |= ADSK2;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK2;
        if(P4IN & ADDO2) Count++;
    }
    P4OUT |= ADSK2;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK2;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
    if(init_pressure){
        P_Init_Val2 = (uint16_t)(Pressure_KPA*10);
    }
    float_temp2 = ((Pressure_KPA * 10) - P_Init_Val2);
    if(float_temp2 <= 0) float_temp2 = 0;
    Return_Value = (uint16_t)((float_temp2*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}

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

    while(P4IN & ADDO3);

    for (i = 0; i < 24; i++){
        P4OUT |= ADSK3;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK3;
        if(P4IN & ADDO3) Count++;
    }
    P4OUT |= ADSK3;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK3;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
    if(init_pressure){
        P_Init_Val3 = (uint16_t)(Pressure_KPA*10);
        init_pressure = false;
        flash_write();
    }
    float_temp3 = ((Pressure_KPA * 10) - P_Init_Val3);
    if(float_temp3 <= 0) float_temp3 = 0;
    Return_Value = (uint16_t)((float_temp3*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}

// Timer B0 interrupt service routine
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
    TBCCTL0 &= ~CCIFG;
    if(waiting_tm) --waiting_tm;
    f_1ms=1;
    __no_operation();

    if (++cnt_5ms >= 5)
    {
        cnt_5ms = 0;
        f_5ms=1;
    }
    if (++cnt_20ms >= 20)
    {
        cnt_20ms = 0;
        f_20ms=1;
    }
    if (++cnt_50ms >= 50)
    {
        cnt_50ms = 0;
        f_50ms=1;
    }
    if (++cnt_100ms >= 100)
    {
        cnt_100ms = 0;
        f_100ms=1;
        if(beep1_en)
        {
            beep_300ms++;
        }
        if(beep2_en)
        {
            beep_100ms++;
        }
    }
    if(++cnt_250ms >= 250)
    {
        cnt_250ms = 0;
        f_250ms   = 1;
    }
    if (++cnt_500ms >= 500)
    {
        cnt_500ms = 0;
        f_500ms=1;
        if(is_auto && !is_basic)
        {
            cnt_auto++;
        }
        if(is_air_F && is_basic)
        {
            cnt_basic_F++;
        }
        if(is_air_L && is_basic)
        {
            cnt_basic_L++;
        }
        if(is_auto_basic)
        {
            cnt_auto_basic++;
        }
    }
    if(++cnt_1sec >= 1000)
    {
        cnt_1sec = 0;
        f_1sec=1;
    }
}

// RS-232C Receive Interrupt Vector(P3.5)
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void usart0_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) usart0_rx (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  rcv_data = RXBUF0;
  __no_operation();
//  if(rcv_data == 0x7E){                     // 1st Frame Header check : 0x7E
//    first_flag = 1;
//  }
//  if(rcv_data == 0xE7){                     // 2nd Frame Header Check : 0xE7
//    if(first_flag == 1){
//        f_rcv_cnt =1;
//        first_flag = 0;
//        rcv_cnt = 0;
//    }
//  }
//  if(f_rcv_cnt == 1){                     // Data Length Extraction
//      __no_operation();
//      rcv_cnt++;
//      if(rcv_cnt == 2){
//      rcv_end_cnt = rcv_data;
//      f_rcv_start = 1;
      //f_rcv_cnt = 0;
      //rcv_cnt = 0;
//      rcv_start_cnt = 0;
//      p_rcv_buf = &rcv_buf[0];          // Designate the pointer for rcv_buf
//      }
//    }
//    if(f_rcv_start == 1){
//      *p_rcv_buf++ = rcv_data;              // Data Receive and Save
//      rcv_start_cnt++;
//      compare_cnt = rcv_end_cnt + 1;
//      if(rcv_start_cnt >= compare_cnt){     // Check Receive End
//          f_rcv_start = 0;
//          rcv_start_cnt = 0;
          f_rcv_proc = true;                   // Activate the receive procedure:rcv_proc()
                                            // rcv_proc() is located in 100ms time routine of main procedure
//      }
//    }
  __no_operation();
}


//power port Interrupt vector
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
    // P1.0 핀의 인터럽트 플래그를 클리어합니다.
    if(P1IFG&POWER_SW)
    {
        P1IFG &= ~POWER_SW;
        if(!is_power && is_first)
        {
            is_power = 1;
            is_first = false;
        }
    }
    if(P1IFG&IN_SENSE)
    {
        P1IFG &= ~IN_SENSE;
        P6OUT &= ~(charged_led | charging_led);
        if(is_charging_mode)
        {
            is_power = 0;
        }
    }
    __bic_SR_register_on_exit(LPM3_bits);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0_ISR(void)
{
    TACCTL0 &= ~CCIFG; // CCR0의 인터럽트 플래그 CCIFG를 클리어

}

// ADC12 interrupt vector
#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    adc_value = ADC12MEM0; // ADC 변환 결과 읽기
    ADC12IFG = 0; // 인터럽트 플래그 클리어
    // 배터리 전압 계산
    __low_power_mode_off_on_exit();      // Clear CPUOFF bit from 0(SR)
}
