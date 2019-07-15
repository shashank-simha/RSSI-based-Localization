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
#include <math.h>

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
    Locations_Init();   // initialize locations
    Locations_Print();  // Print Current location


    uint8_t response[100];
    uint16_t framelen;

    uint8_t count, nzcount = 0;
    uint16_t currentdb = 0;

    while (1)           // Loop forever
    {
        int current_x, current_y;
        Get_Current_Coordinates(&current_x, &current_y);
        float x1 = 0, y1 = 0;

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

            uint16_t next = Locate_Beacon(currentdb);
            if (next == 0)
                break;
            currentdb = 0;
            nzcount = 0;
            Locations_Print();

            x1 = (next & 0xFF);
            y1 = ((next >> 8) & 0xFF);
            UART0_OutString("x1 = ");
            UART0_OutUDec((int) (x1));
            UART0_OutString("\t\t");
            UART0_OutString("y1 = ");
            UART0_OutUDec((int) (y1));
            UART0_OutString("\n\r");

            Clock_Delay1ms(15000);
            if (x1 == 0 && y1 == 0)
            {
                UART0_OutString("Beacon not found\n\r");
                break;
            }

//            Navigate(x1, y1, x, y);
        }
    }
}
