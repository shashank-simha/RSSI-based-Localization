/*
 * PWM.h
 *
 *  Created on: 13-Jul-2019
 *      Author: simha
 */

#ifndef PWM_H_
#define PWM_H_

//***************************PWM_Init*******************************
// Initialize PWM outputs on P2.6, P2.7
// Inputs:  dutyL, dutyR
// Outputs: none
// period of P2.6 is 2*period*666.7ns, duty cycle is dutyR/period
// period of P2.7 is 2*period*666.7ns, duty cycle is dutyL/period

/**
 * @details  Initialize PWM outputs on P2.6, P2.7
 * @remark   Counter counts up to TA0CCR0 and back down
 * @remark   Let Timerclock period T = 8/12MHz = 666.7ns
 * @remark   P2.6=1 when timer equals TA0CCR3 on way down, P2.6=0 when timer equals TA0CCR3 on way up
 * @remark   P2.7=1 when timer equals TA0CCR4 on way down, P2.7=0 when timer equals TA0CCR4 on way up
 * @remark   Period of P2.6 is period*1.333us, duty cycle is dutyR/period
 * @remark   Period of P2.7 is period*1.333us, duty cycle is dutyL/period
 * @remark   Assumes 48 MHz bus clock
 * @remark   Assumes SMCLK = 48MHz/4 = 12 MHz, 83.33ns
 * @param  period is period of wave in 1.333us units
 * @param  dutyR is initial width of high pulse on P2.6 in 1.333us units
 * @param  dutyL is initial width of high pulse om P2.7 in 1.333us units
 * @return none
 * @brief  Initialize PWMR PWML
 */
void PWM_Init(uint16_t period, uint16_t dutyL, uint16_t dutyR);


//***************************PWM_DutyL*******************************
// change duty cycle of PWM output on P2.7
// Inputs:  dutyL
// Outputs: none
// period of P2.7 is 2*period*666.7ns, duty cycle is dutyL/period

/**
 * @details  Set duty cycle on P2.7
 * @remark   Period of P2.7 is period*1.333us, duty cycle is dutyL/period
 * @param    dutyL is width of high pulse on P2.7 in 1.333us units
 * @return   none
 * @warning  dutyL must be less than period
 * @brief    set duty cycle on PWML
 */
void PWM_DutyL(uint16_t dutyL);


//***************************PWM_DutyR*******************************
// change duty cycle of PWM output on P2.6
// Inputs:  dutyR
// Outputs: none
// period of P2.6 is 2*period*666.7ns, duty cycle is dutyR/period

/**
 * @details  Set duty cycle on P2.6
 * @remark   Period of P2.6 is period*1.333us, duty cycle is dutyR/period
 * @param    dutyR is width of high pulse on P2.7 in 1.333us units
 * @return   none
 * @warning  dutyL must be less than period
 * @brief    set duty cycle on PWMR
 */
void PWM_DutyR(uint16_t dutyR);


#endif /* PWM_H_ */
