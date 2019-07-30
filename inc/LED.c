/*
 * LED.c
 *
 *  Created on: 22-Jul-2019
 *      Author: simha
 */

#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdbool.h>

void LED_Init()
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);

    MAP_GPIO_setDriveStrengthHigh(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setDriveStrengthHigh(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setDriveStrengthHigh(GPIO_PORT_P2, GPIO_PIN2);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void LED_Off()
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void Red_On()
{
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
}

void Red_Off()
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
}

void Green_On()
{
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void Green_Off()
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void Blue_On()
{
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);
}

void Blue_Off()
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
}
