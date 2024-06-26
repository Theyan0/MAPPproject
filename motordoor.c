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

void PWM1_Init()            /* Initialize PWM */
{
    ADCON1 = 0x0F;
    TRISA = 0x00;  /* Set CCP1 pin as output for PWM out */
    CCP1CON = 0x0C;        /* Set PWM mode */
}

int setPeriodTo1(unsigned long FPWM)/* Set period */
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

void SetDutyCycleTo1(float Duty_cycle, int Period)
{
    int PWM10BitValue;

    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR1L = (PWM10BitValue >> 2);
    CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}


void delay1(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<10;j++);
}

int motor_init(){
    int Period;
    PWM1_Init();                /* Initialize PWM */
    Period = setPeriodTo1(50);
}

int motordoor_right(){
    int Period;
    PWM1_Init();                /* Initialize PWM */
    Period = setPeriodTo1(50);
    SetDutyCycleTo1(3.0, Period);/* 3% duty cycle */
    delay1(1000);
    SetDutyCycleTo1(7.0, Period); /* 7% duty cycle */
    delay1(1000);
}

int motordoor_left(){
    int Period;
    PWM1_Init();                /* Initialize PWM */
    Period = setPeriodTo1(50);
    SetDutyCycleTo1(7.0, Period); /* 7% duty cycle */
    delay1(1000);
    SetDutyCycleTo1(12.0, Period);/* 12% duty cycle */
    delay1(1000);
}



