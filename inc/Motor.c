// Motor.c
// Runs on MSP432
// Provide mid-level functions that initialize ports and
// set motor speeds to move the robot. Lab 13 starter
// Daniel Valvano
// July 8, 2017

/* This example accompanies the books
 "Embedded Systems: Introduction to the MSP432 Microcontroller",
 ISBN: 978-1512185676, Jonathan Valvano, copyright (c) 2017
 "Embedded Systems: Real-Time Interfacing to the MSP432 Microcontroller",
 ISBN: 978-1514676585, Jonathan Valvano, copyright (c) 2017
 "Embedded Systems: Real-Time Operating Systems for ARM Cortex-M Microcontrollers",
 ISBN: 978-1466468863, , Jonathan Valvano, copyright (c) 2017
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

 Simplified BSD License (FreeBSD License)
 Copyright (c) 2017, Jonathan Valvano, All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 The views and conclusions contained in the software and documentation are
 those of the authors and should not be interpreted as representing official
 policies, either expressed or implied, of the FreeBSD Project.
 */

// Sever VCCMD=VREG jumper on Motor Driver and Power Distribution Board and connect VCCMD to 3.3V.
//   This makes P3.7 and P3.6 low power disables for motor drivers.  0 to sleep/stop.
// Sever nSLPL=nSLPR jumper.
//   This separates P3.7 and P3.6 allowing for independent control
// Left motor direction connected to P1.7 (J2.14)
// Left motor PWM connected to P2.7/TA0CCP4 (J4.40)
// Left motor enable connected to P3.7 (J4.31)
// Right motor direction connected to P1.6 (J2.15)
// Right motor PWM connected to P2.6/TA0CCP3 (J4.39)
// Right motor enable connected to P3.6 (J2.11)
#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/Encoder.h"
#include "../inc/Motor.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <math.h>

#define MAX_PWM 1000
#define THRESHOLD_L 0
#define THRESHOLD_R 0
#define BOT_SPEED_PWM 10

#define PWMR_CORRECTION_FACTOR_FORWARD 1.03
#define PWML_CORRECTION_FACTOR_FORWARD 1
#define PWMR_CORRECTION_FACTOR_ROTATE 1.011//1.024045
#define PWML_CORRECTION_FACTOR_ROTATE 1

static int current_angle = 0, current_x = 0, current_y = 0;

// ------------Motor_Init------------
// Initialize GPIO pins for output, which will be
// used to control the direction of the motors and
// to enable or disable the drivers.
// The motors are initially stopped, the drivers
// are initially powered down, and the PWM speed
// control is uninitialized.
// Input: none
// Output: none
void Motor_Init(void)
{
    /*  L motor  -> PWM - P2.7
     Dir - P5.4
     Slp - P3.7
     R motor  -> PWM - P2.6
     Dir - P5.5
     Slp - P3.6
     */

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN4);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN5);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4); // Initialize direction to forward
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6); // Turnoff Motors intially
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);

    PWM_Init(MAX_PWM, MAX_PWM / 2, MAX_PWM / 2); // initialize PWM on P2.6 & P2.7

}

// ------------Motor_Start------------
// Start the motors
// Input: none
// Output: none
void Motor_Start(void)
{
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7); // Switch on both the motors i.e; deactivate sleep
}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(void)
{
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN6); // Turnoff Right Motor (Sleep)
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7); // Turnoff Left Motor (Sleep)
    PWM_DutyL(0);                                           // Set left PWM to 0
    PWM_DutyR(0);                                          // Set right PWM to 0
}

// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty)
{
    PWM_DutyL(leftDuty);     // set left motor PWM duty cycle
    PWM_DutyR(rightDuty);    // set right motor PWM duty cycle

    Set_Left_Motor_Direction(true);         // set left motor direction(forward)
    Set_Right_Motor_Direction(true);       // set right motor direction(forward)

    Motor_Start();         // Switch on both the motors i.e; deactivate sleep
}

// ------------Motor_Right------------
// Turn the robot to the right by running the
// left wheel forward and the right wheel
// backward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty)
{
    PWM_DutyL(leftDuty);     // set left motor PWM duty cycle
    PWM_DutyR(rightDuty);    // set right motor PWM duty cycle

    Set_Left_Motor_Direction(true);         // set left motor direction(forward)
    Set_Right_Motor_Direction(false);     // set right motor direction(backward)

    Motor_Start();         // Switch on both the motors i.e; deactivate sleep
}

// ------------Motor_Left------------
// Turn the robot to the left by running the
// left wheel backward and the right wheel
// forward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty)
{
    PWM_DutyL(leftDuty);     // set left motor PWM duty cycle
    PWM_DutyR(rightDuty);    // set right motor PWM duty cycle

    Set_Left_Motor_Direction(false);       // set left motor direction(backward)
    Set_Right_Motor_Direction(true);       // set right motor direction(forward)

    Motor_Start();         // Switch on both the motors i.e; deactivate sleep
}

// ------------Motor_Backward------------
// Drive the robot backward by running left and
// right wheels backward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty)
{
    PWM_DutyL(leftDuty);     // set left motor PWM duty cycle
    PWM_DutyR(rightDuty);    // set right motor PWM duty cycle

    Set_Left_Motor_Direction(false);       // set left motor direction(backward)
    Set_Right_Motor_Direction(false);     // set right motor direction(backward)

    Motor_Start();         // Switch on both the motors i.e; deactivate sleep
}

// ------------Set_Left_Motor_PWM------------
// Set Left Motor PWM to the given percent
// Input: percent pwm_normal (range of 0 to 100)
// Output: none
// Assumes: Motor_Init() has been called
void Set_Left_Motor_PWM(uint8_t pwm_normal, bool rotate)
{
    int pwm;

    pwm = (MAX_PWM * pwm_normal) / 100;
    if (pwm > MAX_PWM)
        pwm = MAX_PWM;
    if (pwm < 0)
        pwm = 0;
    if (rotate)
        pwm *= PWML_CORRECTION_FACTOR_ROTATE;
    else
        pwm *= PWML_CORRECTION_FACTOR_FORWARD;
    PWM_DutyL(pwm);
}

// ------------Set_Right_Motor_PWM------------
// Set Right Motor PWM to the given percent
// Input: percent pwm_normal (range of 0 to 100)
// Output: none
// Assumes: Motor_Init() has been called
void Set_Right_Motor_PWM(uint8_t pwm_normal, bool rotate)
{
    int pwm;

    pwm = (MAX_PWM * pwm_normal) / 100;

    if (pwm > MAX_PWM)
        pwm = MAX_PWM;
    if (pwm < 0)
        pwm = 0;
    if (rotate)
        pwm *= PWMR_CORRECTION_FACTOR_ROTATE;
    else
        pwm *= PWMR_CORRECTION_FACTOR_FORWARD;
    PWM_DutyR(pwm);
}

/*
 *  Set left motor direction.
 *
 *  True: forward
 *  False: reverse
 */
void Set_Left_Motor_Direction(int dir)
{
    if (dir)
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4); // set left motor direction(forward)
    else
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4); // set left motor direction(backward)
}

/*
 *  Set right motor direction.
 *
 *  True: forward
 *  False: reverse
 */
void Set_Right_Motor_Direction(int dir)
{
    if (dir)
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5); // set right motor direction(forward)
    else
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN5); // set right motor direction(backward)
}

/*
 *  Rotate both left and right motors by a given encoder count.
 *
 *  This routine is called in two parts, first call is with mode=INITIAL to set up some initial static variables,
 *  second call is with mode=CONTINUOUS.  This call will turn on the motors in the proper direction and check if the target count threshold
 *  has been reached.  It should be called continuously until a value of TRUE is returned.
 */
void Rotate_Motors_By_Counts(uint8_t speed_factor, int left_count,
                             int right_count)
{
    Motor_Stop();
    Reset_Encoder();

    int left_error;
    int right_error;

    // set motor direction based on if degrees is positive or negative
    Set_Left_Motor_Direction(left_count >= 0);
    Set_Right_Motor_Direction(right_count >= 0);

    // set motor PWM
    if ((left_count > 0 && right_count < 0)
            || (left_count < 0 && right_count > 0))
    {
        Set_Left_Motor_PWM(speed_factor, true);
        Set_Right_Motor_PWM(speed_factor, true);
    }

    else
    {
        Set_Left_Motor_PWM(speed_factor, false);
        Set_Right_Motor_PWM(speed_factor, false);
    }

    left_error = abs(left_count) - Get_Left_Motor_Count();
    right_error = abs(right_count) - Get_Right_Motor_Count();

    if (left_error > THRESHOLD_L || right_error > THRESHOLD_R)
        Motor_Start();

    while (left_error > THRESHOLD_L || right_error > THRESHOLD_R)
    {
        // Stop individual motor if we are within the threshold
        if (left_error <= THRESHOLD_L)
            Set_Left_Motor_PWM(0, false);

        if (right_error <= THRESHOLD_R)
            Set_Right_Motor_PWM(0, false);

        left_error = abs(left_count) - Get_Left_Motor_Count();
        right_error = abs(right_count) - Get_Right_Motor_Count();
    }
    Motor_Stop();
}

void Get_Current_Coordinates(int *x, int *y)
{
    *x = current_x;
    *y = current_y;
}

void Navigate(float x1, float y1, float x2, float y2) // parameters are in feet
{
    float dist = Distance(x1, y1, x2, y2);
    float ang = Angle(x1, y1, x2, y2);
    int rotation_count = 0;

    // Initially turn the bot by the required angle
    rotation_count = Angle_To_Rotation_Count(ang - current_angle);
    Rotate_Motors_By_Counts(BOT_SPEED_PWM, rotation_count, -(rotation_count));
    current_angle = ang;

    Clock_Delay1ms(1000);

    // Move the bot by required distance
    rotation_count = Distance_To_Rotation_Count(dist);
    Rotate_Motors_By_Counts(BOT_SPEED_PWM, rotation_count, rotation_count);
    current_x = x2;
    current_y = y2;

    Clock_Delay1ms(1000);
}

float Distance(int x1, int y1, int x2, int y2)
{
    float distance = pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
    return distance;
}

float Angle(float x1, float y1, float x2, float y2)
{
    float angle = atan2f(x2 - x1, y2 - y1);
    angle *= (180 / M_PI);
    return angle;
}

int Distance_To_Rotation_Count(float distance) // distance in feet
{
    // rotation count = (Distance/wheel perimeter) * 360
    distance *= 304.8; // feet to mm conversion
    int count = (distance / (70 * M_PI)) * 360; // wheel diameter =  70mm => 1 rotation(360 degrees)
    return count;
}

int Angle_To_Rotation_Count(float degree)  // angle in degrees
{
    // rotation count = ((chassis perimeter/wheel perimeter) * angle_required) or ((chassis diameter/wheel diameter) * angle_required)
    int count = (165 / 70) * degree;
    return count;
}
