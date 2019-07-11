#include "msp.h"
#include "inc/UART0.h"
#include "inc/UART1.h"
#include "inc/EUSCIA0.h"
#include "inc/Clock.h"
#include "inc/Motor.h"
#include "inc/PWM.h"
#include "inc/Encoder.h"
#include "inc/Zigbee.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    UART0_Init();       // initialize UART0 (Serial Monitor)
    UART1_Init();       // initialize UART1 (Zigbee module)
    Clock_Init48MHz();  // makes SMCLK=12 MHz
    Motor_Init();       // initialize motor(inturn initialize PWM)
    Encoder_Init();     // initialize encoder (inturn Port interrupts)

    uint8_t response[100];
    uint16_t framelen;

    while (1)           // Loop forever
    {
        uint8_t count, nzcount = 0;
        uint16_t currentdb = 0;
        for (count = 0; count < 10; count++)
        {
            Outmessage(response, &framelen);
            Clock_Delay1ms(10);
            if (DB != 0)
            {
                currentdb += DB;
                ++nzcount;
                DB = 0;
            }
        }

        if (currentdb != 0)
        {
            currentdb /= nzcount;
            UART0_OutString("**** Average DB = ");
            UART0_OutUHex(currentdb);
            UART0_OutString(" ****\n\r");
        }
    }
}
