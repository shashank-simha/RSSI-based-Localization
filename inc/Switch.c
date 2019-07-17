/*
 * Switch.c
 *
 *  Created on: 18-Jul-2019
 *      Author: simha
 */


#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdbool.h>

void Switch_Init()
{
      P1->SEL0 &= ~0x12;
      P1->SEL1 &= ~0x12;                        // configure P1.4 and P1.1 as GPIO
      P1->DIR &= ~0x12;                         // make P1.4 and P1.1 as input
      P1->REN |= 0x12;                         // enable pull resistors on P1.4 and P1.1
      P1->OUT |= 0x12;                         // P1.4 and P1.1 are pull-up
}

bool S1_IsPressed()
{
    if (P1->IN & 0x02)                   // read P1.1 input
        return false;
    else
        return true;
}

bool S2_IsPressed()
{
    if (P1->IN & 0x10)                   // read P1.4 input
        return false;
    else
        return true;
}
