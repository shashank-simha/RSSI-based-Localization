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
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/Encoder.h"
#include "../inc/Motor.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define MAX_PWM 65000
#define THRESHOLD 30

volatile float x = 0, y = 0, r = 0, theta = 0;

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
    /* L motor -> PWM - P2.4
     Dir - P2.6
     Slp - P5.6
     R motor -> PWM - P2.5
     Dir - P3.0
     Slp - P5.7 */

    P2->SEL0 &= ~0x70;
    P2->SEL1 &= ~0x70;    //  P2.4, P2.5 and P2.6 as GPIO
    P2->DIR |= 0x70;      //  make P2.4, P2.5 and P2.6 out

    P3->SEL0 &= ~0x01;
    P3->SEL1 &= ~0x01;    //  P3.0 as GPIO
    P3->DIR |= 0x01;      //  make P3.0 out

    P5->SEL0 &= ~0xC0;
    P5->SEL1 &= ~0xC0;    //  P5.6 and P5.7 as GPIO
    P5->DIR |= 0xC0;      //  make P5.6 and P5.7 out

    P5->OUT &= ~0xC0;     // Turnoff Motors intially

    PWM_Init12(MAX_PWM, MAX_PWM / 2, MAX_PWM / 2); // initialize PWM on P2.4 & P2.5

}

// ------------Motor_Start------------
// Start the motors
// Input: none
// Output: none
void Motor_Start(void)
{
    P5->OUT |= 0xC0;         // Switch on both the motors i.e; deactivate sleep
}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(void)
{
    P5->OUT &= ~0xC0;          // Turnoff Motors (Sleep)
    PWM_Duty1(0);              // Set left PWM to 0
    PWM_Duty2(2);              // Set right PWM to 0
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
    PWM_Duty1(leftDuty);     // set left motor PWM duty cycle
    PWM_Duty2(rightDuty);    // set right motor PWM duty cycle

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
    PWM_Duty1(leftDuty);     // set left motor PWM duty cycle
    PWM_Duty2(rightDuty);    // set right motor PWM duty cycle

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
    PWM_Duty1(leftDuty);     // set left motor PWM duty cycle
    PWM_Duty2(rightDuty);    // set right motor PWM duty cycle

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
    PWM_Duty1(leftDuty);     // set left motor PWM duty cycle
    PWM_Duty2(rightDuty);    // set right motor PWM duty cycle

    Set_Left_Motor_Direction(false);       // set left motor direction(backward)
    Set_Right_Motor_Direction(false);     // set right motor direction(backward)

    Motor_Start();         // Switch on both the motors i.e; deactivate sleep
}

// ------------Turn_Left------------
// Turn the robot to the left by an angle of theta
// Input: angle t
// Output: none
// Assumes: Motor_Init() has been called
void Turn_Left(uint8_t t)
{
    Motor_Left(32500, 32500);    // call Motor_Left function with PWM values
    Clock_Delay1ms((1000 / 360) * t);       // 1 sec delay => 360deg rotation
    Motor_Stop();
}

// ------------Turn_Right------------
// Turn the robot to the right by an angle of theta
// Input: angle t
// Output: none
// Assumes: Motor_Init() has been called
void Turn_Right(uint8_t t)
{
    Motor_Right(32500, 32500);    // call Motor_Left function with PWM values
    Clock_Delay1ms((1000 / 360) * t);       // 1 sec delay => 360deg rotation
    Motor_Stop();
}

// ------------Set_Left_Motor_PWM------------
// Set Left Motor PWM to the given percent
// Input: percent pwm_normal (range of 0 to 100)
// Output: none
// Assumes: Motor_Init() has been called
void Set_Left_Motor_PWM(uint8_t pwm_normal)
{
    int pwm;

    pwm = (MAX_PWM * pwm_normal) / 100;
    if (pwm > MAX_PWM)
        pwm = MAX_PWM;
    if (pwm < 0)
        pwm = 0;
    PWM_Duty1(pwm);
}

// ------------Set_Right_Motor_PWM------------
// Set Right Motor PWM to the given percent
// Input: percent pwm_normal (range of 0 to 100)
// Output: none
// Assumes: Motor_Init() has been called
void Set_Right_Motor_PWM(uint8_t pwm_normal)
{
    int pwm;

    pwm = (MAX_PWM * pwm_normal) / 100;

    if (pwm > MAX_PWM)
        pwm = MAX_PWM;
    if (pwm < 0)
        pwm = 0;
    PWM_Duty2(pwm);
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
        P2->OUT |= 0x40;         // set left motor direction(forward)
    else
        P2->OUT &= ~0x40;         // set left motor direction(backward)
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
        P3->OUT |= 0x01;         // set right motor direction(forward)
    else
        P3->OUT &= ~0x01;         // set right motor direction(backward)
}

/*
 *  Rotate both left and right motors by a given encoder count.
 *
 *  This routine is called in two parts, first call is with mode=INITIAL to set up some initial static variables,
 *  second call is with mode=CONTINUOUS.  This call will turn on the motors in the proper direction and check if the target count threshold
 *  has been reached.  It should be called continuously until a value of TRUE is returned.
 */
int Rotate_Motors_By_Counts(motor_mode_t mode, uint8_t speed_factor,
                            int left_count, int right_count)
{
    static int left_target;
    static int right_target;
    int left_error;
    int right_error;
    bool r = false;

    int temp, temp1;

    switch (mode)
    {
    case INITIAL:
// save the target degrees in a static variable for use later
//       left_target = get_left_motor_count() + left_count;
//       right_target = get_right_motor_count() + right_count;
        temp = Get_Left_Motor_Count();
        temp1 = Get_Right_Motor_Count();
        left_target = temp + left_count;
        right_target = temp1 + right_count;

        // set motor direction based on if degrees is positive or negative
        Set_Left_Motor_Direction(left_count >= 0);
        Set_Right_Motor_Direction(right_count >= 0);

        r = false;
        break;

    case CONTINUOUS:
        temp = Get_Left_Motor_Count();
        temp1 = Get_Right_Motor_Count();
        left_error = left_target - temp;
        right_error = right_target - temp1;

//       left_error = left_target - get_left_motor_count();
//       right_error = right_target - get_right_motor_count();

        // set motor direction based on if speed is positive or negative
        Set_Left_Motor_Direction(left_error >= 0);
        Set_Right_Motor_Direction(right_error >= 0);

        // Stop individual motor if we are within the threshold
        if (abs(left_error) > THRESHOLD)
            Set_Left_Motor_PWM(speed_factor);
        else
            Set_Left_Motor_PWM(0);

        if (abs(right_error) > THRESHOLD)
            Set_Right_Motor_PWM(speed_factor);
        else
            Set_Right_Motor_PWM(0);

        // if both motors are within the threshold then return true to signal "all done"
        if ((abs(left_error) <= THRESHOLD) && (abs(right_error) <= THRESHOLD))
            r = true;
        Motor_Start();
        break;
    }

    return r;

}


void Navigate (float x1,float y1,float x2,float y2)
{

}
