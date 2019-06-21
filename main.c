#include "msp.h"
#include "inc/UART0.h"
#include "inc/EUSCIA0.h"
#include "inc/Clock.h"
#include "inc/Motor.h"
#include "inc/PWM.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	EUSCIA0_Init();     // initialize UART
	Clock_Init48MHz();  // makes SMCLK=12 MHz
	Motor_Init();       // initialize motor(inturn initialize PWM)

}
