/*
 * PWM.c
 *
 *  Created on: 13-Jul-2019
 *      Author: simha
 */

// Runs on MSP432
// PWM on P2.4 using TimerA0 TA0.CCR1
// PWM on P2.5 using TimerA0 TA0.CCR2
// PWM on P2.6 using TimerA0 TA0.CCR3
// PWM on P2.7 using TimerA0 TA0.CCR4
// MCLK = SMCLK = 3MHz DCO; ACLK = 32.768kHz
// TACCR0 generates a square wave of freq ACLK/1024 =32Hz
// Derived from msp432p401_portmap_01.c in MSPware

#include "msp.h"

//***************************PWM_Init*******************************
// PWM outputs on P2.6, P2.7
// Inputs:  period (1.333us)
//          dutyL
//          dutyR
// Outputs: none
// SMCLK = 48MHz/4 = 12 MHz, 83.33ns
// Counter counts up to TA0CCR0 and back down
// Let Timerclock period T = 8/12MHz = 666.7ns
// period of P7.3 squarewave is 4*period*666.7ns
// P2.6=1 when timer equals TA0CCR3 on way down, P2.6=0 when timer equals TA0CCR3 on way up
// P2.7=1 when timer equals TA0CCR4 on way down, P2.7=0 when timer equals TA0CCR4 on way up
// Period of P2.6 is period*1.333us, duty cycle is dutyR/period
// Period of P2.7 is period*1.333us, duty cycle is dutyL/period
void PWM_Init(uint16_t period, uint16_t dutyL, uint16_t dutyR)
{
    if (dutyL >= period)
        return; // bad input
    if (dutyR >= period)
        return; // bad input
    P2->DIR |= 0xC0;          // P2.6, P2.7 output
    P2->SEL0 |= 0xC0;         // P2.6, P2.7 Timer0A functions
    P2->SEL1 &= ~0xC0;        // P2.6, P2.7 Timer0A functions
    TIMER_A0->CCTL[0] = 0x0080;      // CCI0 toggle
    TIMER_A0->CCR[0] = period;   // Period is 2*period*8*83.33ns is 1.333*period
    TIMER_A0->EX0 = 0x0000;        //    divide by 1
    TIMER_A0->CCTL[3] = 0x0040;      // CCR1 toggle/reset
    TIMER_A0->CCR[3] = dutyR;        // CCR1 duty cycle is dutyR/period
    TIMER_A0->CCTL[4] = 0x0040;      // CCR2 toggle/reset
    TIMER_A0->CCR[4] = dutyL;        // CCR2 duty cycle is dutyL/period
    TIMER_A0->CTL = 0x02F0;        // SMCLK=12MHz, divide by 8, up-down mode
    // bit  mode
    // 9-8  10    TASSEL, SMCLK=12MHz
    // 7-6  11    ID, divide by 8
    // 5-4  11    MC, up-down mode
    // 2    0     TACLR, no clear
    // 1    0     TAIE, no interrupt
    // 0          TAIFG
}

//***************************PWM_DutyR*******************************
// change duty cycle of PWM output on P2.6
// Inputs:  dutyR
// Outputs: none
// period of P2.6 is 2*period*666.7ns, duty cycle is dutyR/period
void PWM_DutyR(uint16_t dutyR)
{
    if (dutyR >= TIMER_A0->CCR[0])
        return; // bad input
    TIMER_A0->CCR[3] = dutyR;        // CCR1 duty cycle is dutyR/period
}

//***************************PWM_DutyL*******************************
// change duty cycle of PWM output on P2.7
// Inputs:  dutyL
// Outputs: none// period of P2.7 is 2*period*666.7ns, duty cycle is dutyL/period
void PWM_DutyL(uint16_t dutyL)
{
    if (dutyL >= TIMER_A0->CCR[0])
        return; // bad input
    TIMER_A0->CCR[4] = dutyL;        // CCR1 duty cycle is dutyL/period
}
