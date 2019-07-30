/*
 * Encoder.c
 *
 *  Created on: 09-Jul-2019
 *      Author: simha
 */

 /*
  *
  * Pololu #3542 Romi Encoder connected to Pololu #3543 Motor Driver and Power Distribution Board
  * This connects motor, power, encoders, and grounds.  Kit includes this hardware.  See images.
  * Sever VPU = VREG jumper on Motor Driver and Power Distribution Board and connect VPU to 3.3V.
  * This is necessary because MSP432 inputs are not 5V tolerant.
  *
  * Left Encoder A connected to P10.5
  * Left Encoder B connected to P5.2
  * Right Encoder A connected to P10.4
  * Right Encoder B connected to P5.0
  *
  * Pololu encoder has 12 counts per revolution (counting all 4 edges).
  * The motor has a gearbox with a 120:1 ratio.
  * This gives 12*120 = 1440 counts per revolution of the wheel.
  * Since we are only counting 1 edge of the encoder we need to divide by 4 for a total of 360 counts per revolution.
 */



#include <stdint.h>
#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Initialize Encoder inputs

static int left_motor_count = 0;
static int right_motor_count = 0;

void Encoder_Init(void){
    /* Configuring P5.2 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN2);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN2, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN2);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN2);
    MAP_Interrupt_enableInterrupt(INT_PORT5);

    MAP_GPIO_setAsInputPin(GPIO_PORT_P10, GPIO_PIN5);


    /* Configuring P5.0 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN0);
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN0, GPIO_LOW_TO_HIGH_TRANSITION);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN0);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN0);
    MAP_Interrupt_enableInterrupt(INT_PORT5);

    MAP_GPIO_setAsInputPin(GPIO_PORT_P10, GPIO_PIN4);


    /* Enable Master Interrupt */
    MAP_Interrupt_enableMaster();
}

int Get_Left_Motor_Count() {
    return left_motor_count;
}

int Get_Right_Motor_Count() {
    return right_motor_count;
}

/* GPIO ISR */
void PORT5_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    if (status & GPIO_PIN2)
    {
        left_motor_count++;
    }

    if (status & GPIO_PIN0)
    {
        right_motor_count++;
    }

}

void Reset_Encoder()
{
    left_motor_count = 0;
    right_motor_count = 0;
}
