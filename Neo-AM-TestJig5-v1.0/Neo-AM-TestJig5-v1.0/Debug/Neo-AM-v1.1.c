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
 * 12-05 포토 인터럽트 센서 실험
=======================================================================================*/
#include <msp430x14x.h>
#include <Neo-AM-Func-v1.1.h>
#include <Neo-AM-v1.1.h>

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

int main(void)
{    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    _DINT();
    clock_init();
    IOPort_init();
    Timer_B7_init();
    UART_Init();
    flashData_init();
    control_init();
    _EINT();

    Addr_485 = Addr_Get();

    __no_operation();

    while(1)
    {
        if(f_rcv_proc1 == true) rcv_proc1();
        if (f_500ms)
        {
            f_500ms = 0;
            P1OUT ^= POWER_LED;         // Program Run Monitor
            //Pressure_Data_Send1();
        }
        if (f_10ms)
        {
            f_10ms = 0;
            Init_Input();
            Addr_485 = Addr_Get();
            P_Read_Flag = true;
            if(pump_on == true){
                Valve_IN1_High();
                VALVE_OUT1_High();
            }else{
                Valve_IN1_Low();
                VALVE_OUT1_Low();
            }
            if(P6IN&IN1_1){
                Valve_IN1_High();
            }else{
                Valve_IN1_Low();
            }
            if(P6IN&IN1_2){
                VALVE_OUT1_High();
            }else{
                VALVE_OUT1_Low();
            }

        }
//        if(f_1sec){
//            f_1sec = 0;
//            Pressure_Data_Send1();
//        }

        // 압력센서 값 읽기
        if(P_Read_Flag == true){
            P_Read_Flag = false;
            P1_Value = ReadCount1();
            P2_Value = ReadCount2();
            P3_Value = ReadCount3();
            P4_Value = ReadCount4();
        }
        __no_operation();
    }
}

void clock_init(void)
{
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

void IOPort_init(void)
{
    // P1.7 LED를 Output으로 설정
    P1DIR |= BIT7;
    // P1.0, P1.1, P1.2, P1.3, P1.4 address 와 switch P1.5를 Input으로 설정
    P1DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | RESET);
    // P2.0, P2.1, P2.2, P2.3, P2.4, P2.5, P2.6, P2.7 photo sensor를 Input으로 설정
    P2DIR &= ~(IN3_1 | IN3_2 | OUT3_1 | OUT3_2 | IN4_1 | IN4_2 | OUT4_1 | OUT4_2);

    // P6.0, P6.1, P6.2, P6.3, P6.4, P6.5, P6.6, P6.7 photo sensor를 Input으로 설정
    P6DIR &= ~(IN1_1 | IN1_2 | OUT1_1 | OUT1_2 | IN2_1 | IN2_2 | OUT2_1 | OUT2_2);

    // P4.0, P4.2, P4.4, P4.6을 Input으로 설정 - Data In from HX710B
    P4DIR &= ~(BIT0 | BIT2 | BIT4 | BIT6);

    // P4.1, P4.3, P4.5, P4.7을 Output으로 설정 - Clock Out to HX710B
    P4DIR |= BIT1 | BIT3 | BIT5 | BIT7;

//    // P6.0, P6.1, P6.2, P6.3을 Output으로 설정 - Display용 Port 설정
//    P6DIR |= BIT0 | BIT1 | BIT2 | BIT3;
}

//==============================================================================
// Timer_B7 초기화
//------------------------------------------------------------------------------
void Timer_B7_init(void)
{
    TBCCTL0 = CCIE;                           // TBCCR0 interrupt enabled
    TBCCR0 = (UINT)(CLK_11MHz/1000);
    TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, upmode
}

void UART_Init(void)
{
    // UART Port1 설정 및 속도, UART Interrupt 설정
    // RS-485
    P3DIR |= BIT5;                  // P3.5을 Output으로 설정, 485 방향 설정
    P3OUT &= ~BIT5;                 // P3.5 Output을 "0"으로 만들어 수신으로 설정
    P3DIR &= ~BIT7;                 // P3.7 Port를 Input으로 설정(RX)
    P3SEL |= 0xc0;                  // P3.6,P3.7 = USART1 Tx1/Rx1
    UCTL1 |= CHAR;                  // 8-bit character
    UTCTL1 = SSEL1+SSEL0;           // UCLK = SMCLK

    // Baud Rate 설정 : 38400
    //UBR01 = 0x20;                   // 11.0592Mhz/38400 = 288 = 0x120.
    //UBR11 = 0x01;                   // 11.0592Mhz/19200 = 576 = 0x240.
                                    // 11.0592Mhz/9600 = 1162 =0x480
    //57600
    //UBR01 = 0xC0;
    //UBR11 = 0x00;

    //115200
    UBR01 = 0x60;
    UBR11 = 0x00;

    UMCTL1 = 0x00;                  // no modulation
    ME2 |= UTXE1 + URXE1;           // Enable USART1 TXD/RXD

    UCTL1 &= ~SWRST;                // Initalize USART state machine
    IE2 |= URXIE1;                  // Enable USART1 RX interrupt
}

uint8_t Addr_Get(void)
{
    uint8_t Addr;

    Addr = ~P1IN;
    Addr &= 0x1F;
    return Addr;
}

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
    if(sensor_init_en == true){
        P_Init_Val2 = (uint16_t)(Pressure_KPA*10);
    }
    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val2);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

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
    if(sensor_init_en == true){
        P_Init_Val3 = (uint16_t)(Pressure_KPA*10);
    }
    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val3);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}

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

    while(P4IN & ADDO4);

    for (i = 0; i < 24; i++){
        P4OUT |= ADSK4;
        Count <<= 1;
        __no_operation();
        P4OUT &= ~ADSK4;
        if(P4IN & ADDO4) Count++;
    }
    P4OUT |= ADSK4;
    Count ^= 0x800000;  // 24bit Data의 보수를 취한다.
    __no_operation();
    P4OUT &= ~ADSK4;
    Count_24bit = Count & 0xFFFFFF;
    Pressure_KPA = ((float)Count_24bit / 16777216.0) * (Max_Pressure - Min_Pressure) + Min_Pressure;
    if(sensor_init_en == true){
        P_Init_Val4 = (uint16_t)(Pressure_KPA*10);
        sensor_init_en = false;
        flash_write();
    }
    float_temp1 = ((Pressure_KPA * 10) - P_Init_Val4);
    if(float_temp1 <= 0) float_temp1 = 0;
    Return_Value = (uint16_t)((float_temp1*2.42)); // 압력센서 전압 1.2V 일때 계산식

    return Return_Value;
}

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

void sensor_init(void)
{
    sensor_init_en = true;
    Pressure_Data_Send1();
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

    free_byte0 = 0;
    for(kk=1; kk<9; ++kk) {
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

    for(kk=0; kk<9; ++kk){
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

    for(jj=0; jj<9; ++jj){
        bf_data[jj] = *flash_ptr++;
    }

    P_Init_Val1 = bf_data[1]*256 + bf_data[2];
    P_Init_Val2 = bf_data[3]*256 + bf_data[4];
    P_Init_Val3 = bf_data[5]*256 + bf_data[6];
    P_Init_Val4 = bf_data[7]*256 + bf_data[8];
}

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

void rcv_proc1(void)
{
    f_rcv_proc1 = false;
    rcv_address = rcv_buf1[2];
    if(rcv_address == Addr_485)
    {
        rcv_command1 = rcv_buf1[3];
        switch(rcv_command1)
        {
        case 0x81:
            Pressure_Data_Send1();
            break;
        case 0x82:
            sensor_init();
            break;
        default:
            break;
        }
    }
    __no_operation();
}

void Pressure_Data_Send1(void)
{
    uint8_t i;
    TX485_On();
    trs_buf1[0] = 0x7E;
    trs_buf1[1] = 0xE7;
    trs_buf1[2] = 0xF; //데이터 길이 다시 체크
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
    for(i = 0; i<14; i++){
        free_byte0 += trs_buf1[i];
    }
    free_byte0 = 0xFF - free_byte0; // Checksum 계산
    trs_buf1[14] = free_byte0;

    UCA1_strout_length(trs_buf1, 15);

    __delay_cycles(1000);
    RX485_On();
}

void UCA1_strout_length(UBYTE *tx_str, UBYTE length)
{
    UBYTE i;
    for(i=0; i<length; i++) {
        UCA1_chout(*(tx_str+i));
    }
}

void UCA1_chout(unsigned char tx_ch)
{
    while(!(IFG2 & UTXIFG1));
    TXBUF1 = tx_ch;
}


// Timer B0 interrupt service routine
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{
    TBCCTL0 &= ~CCIFG;
    if(waiting_tm) --waiting_tm;
    f_1ms=1;
    __no_operation();

    if (++cnt_5ms >= 5) {
        cnt_5ms = 0;
        f_5ms=1;
    }
    if (++cnt_10ms >= 10) {
        cnt_10ms = 0;
        f_10ms=1;
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

}

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
    if(f_rcv_cnt1 == 1){                     // Data Length Extraction
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
        if(rcv_start_cnt1 >= compare_cnt1){     // Check Receive End
            f_rcv_start1 = 0;
            rcv_start_cnt1 = 0;
            f_rcv_proc1 = true;                 // Activate the receive procedure:rcv_proc()
                                    // rcv_proc() is located in 100ms time routine of main procedure
        }
    }
}



