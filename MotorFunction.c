#include <pic18f4550.h>
#include <stdio.h>
#include <math.h>
#include <xc.h>

#define MINTHR              8000
#define RESOLUTION          488
#define InternalOsc_8MHz    8000000
#define InternalOsc_4MHz    4000000
#define InternalOsc_2MHz    2000000
#define InternalOsc_1MHz    1000000
#define InternalOsc_500KHz  500000
#define InternalOsc_250KHz  250000
#define InternalOsc_125KHz  125000
#define InternalOsc_31KHz   31000
#define Timer2Prescale_1    1
#define Timer2Prescale_4    4
#define Timer2Prescale_16   16

void PWM_Init()            /* Initialize PWM */
{
    TRISCbits.TRISC2 = 0;  /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x0C;        /* Set PWM mode */
}

void PWMdoor_Init() {
    TRISCbits.TRISC1 = 0;
    CCP1CON = 0x0C; 
}


int setPeriodTo(unsigned long FPWM)/* Set period */
{
    int clockSelectBits, TimerPrescaleBits;
    int TimerPrescaleValue;
    float period;
    unsigned long FOSC, _resolution = RESOLUTION;

    if (FPWM < MINTHR)    {TimerPrescaleBits = 2; TimerPrescaleValue = Timer2Prescale_16;}
    else                  {TimerPrescaleBits = 0; TimerPrescaleValue = Timer2Prescale_1;}

    if (FPWM > _resolution)               {clockSelectBits = 7; FOSC = InternalOsc_8MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 6; FOSC = InternalOsc_4MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 5; FOSC = InternalOsc_2MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 4; FOSC = InternalOsc_1MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 3; FOSC = InternalOsc_500KHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 2; FOSC = InternalOsc_250KHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 1; FOSC = InternalOsc_125KHz;}
    else                                  {clockSelectBits = 0; FOSC = InternalOsc_31KHz;}

    period = ((float)FOSC / (4.0 * (float)TimerPrescaleValue * (float)FPWM)) - 1.0;
    period = round(period);

    OSCCON = ((clockSelectBits & 0x07) << 4) | 0x02;
    PR2 = (int)period;
    T2CON = TimerPrescaleBits;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;  /* Turn ON Timer2 */
    return (int)period;
}

void SetDutyCycleTo(float Duty_cycle, int Period)
{
    int PWM10BitValue;

    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR1L = (PWM10BitValue >> 2);
    CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}

void doorSetDutyCycleTo(float Duty_cycle, int Period)
{
    int PWM10BitValue;

    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR2L = (PWM10BitValue >> 2);
    CCP2CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}


void delay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<10;j++);
}

int motor_init(){
    int Period;
    PWM_Init();                /* Initialize PWM */
    Period = setPeriodTo(50);
}

int motorlock_unlock(){
    int Period;
    PWM_Init();                /* Initialize PWM */
    Period = setPeriodTo(50);
    SetDutyCycleTo(12.0, Period);/* 3% duty cycle */
    delay(7000);
    return;
}

int motorlock_lock(){
    int Period;
    PWM_Init();                /* Initialize PWM */
    Period = setPeriodTo(50);
    SetDutyCycleTo(7.0, Period); 
    delay(7000);
    return;
}

int motordoor_open() {
    int Period;
    PWMdoor_Init();
    Period = setPeriodTo(50);
    doorSetDutyCycleTo(12.0, Period);
    delay(7000);
    return;
}

int motordoor_close() {
    int Period;
    PWMdoor_Init(); /* Initialize PWM */
    Period = setPeriodTo(50);
    // Move the door to closed position (180 degrees)
    doorSetDutyCycleTo(3.0, Period); /* 12% duty cycle */
    delay(7000); // Adjust this delay as needed for the door to close
    return;
}