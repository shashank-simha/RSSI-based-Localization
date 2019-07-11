/**
 * @file      FIFO2.h
 * @brief     Provide two FIFO queues
 * @details   Provide functions that initialize a FIFO, put data in, get data out,
 *            and return the current size.
 * @remark    The sizes of the FIFO must be a power of two
 * @version   V1.0
 * @author    Valvano
 * @copyright Copyright 2017 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      October 11, 2017

 ******************************************************************************/

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


#ifndef __FIFO2_H__
#define __FIFO2_H__


/**
 * \brief Size of the TxFifo2, can hold 0 to TX0FIFOSIZE-1 elements, must be a power of 2
 */
#define TX2FIFOSIZE 64    // must be a power of 2

/**
 * \brief return value on success
 */
#define FIFOSUCCESS 1

/**
 * \brief return value on failure
 */
#define FIFOFAIL    0     // return value on failure


/**
 * @details   The TxFifo2 FIFO is used by the transmit channel. Outgoing data are stored into this FIFO.
 * @details   Can hold 0 to TX0FIFOSIZE-1 elements, first in first out.
 * @param  none
 * @return none
 * @brief  Initialize TxFifo2
 */
void TxFifo2_Init(void);

/**
 * @details   Add one 8-bit element to TxFifo2.
 * @details   Can hold 0 to TX0FIFOSIZE-1 elements, first in first out
 * @warning  TxFifo2_Put itself need not be reentrant, but TxFifo2_Put must be thread-safe with TxFifo2_Get
 * @param  data 8-bit value to store into TxFifo2
 * @return FIFOSUCCESS if ok, FIFOFAIL if full and could not be saved
 * @brief  Put into TxFifo2
 */
int TxFifo2_Put(char data);

/**
 * @details   Remove one 8-bit element from TxFifo2.
 * @details   Returns the oldest value, first in first out
 * @warning  TxFifo2_Get itself need not be reentrant, but TxFifo2_Get must be thread-safe with TxFifo2_Put
 * @param  datapt pointer to a 8-bit storage location, into which to store the removed data
 * @return FIFOSUCCESS if ok, FIFOFAIL if empty and data could not be removed
 * @brief  Get into TxFifo2
 */
int TxFifo2_Get(char *datapt);

/**
 * @details   Return the number of elements in TxFifo2.
 * @details   Can hold 0 to TX0FIFOSIZE-1 elements
 * @param  none
 * @return number of elements in TxFifo2
 * @brief  Current size of TxFifo2
 */
uint16_t TxFifo2_Size(void);

/**
 * \brief Size of the RxFifo2, can hold 0 to RX0FIFOSIZE-1 elements, must be a power of 2
 */
#define RX2FIFOSIZE 16 // must be a power of 2

/**
 * @details   The RxFifo2 FIFO is used by the receive channel. Incoming data are stored into this FIFO.
 * @details   Can hold 0 to RX0FIFOSIZE-1 elements, first in first out.
 * @param  none
 * @return none
 * @brief  Initialize RxFifo2
 */
void RxFifo2_Init(void);

/**
 * @details   Add one 8-bit element to RxFifo2.
 * @details   Can hold 0 to RX0FIFOSIZE-1 elements, first in first out
 * @warning  RxFifo2_Put itself need not be reentrant, but RxFifo2_Put must be thread-safe with RxFifo2_Get
 * @param  data 8-bit value to store into RxFifo2
 * @return FIFOSUCCESS if ok, FIFOFAIL if full and could not be saved
 * @brief  Put into RxFifo2
 */
int RxFifo2_Put(char data);


/**
 * @details   Remove one 8-bit element from RxFifo2.
 * @details   Returns the oldest value, first in first out
 * @warning  RxFifo2_Get itself need not be reentrant, but RxFifo2_Get must be thread-safe with RxFifo2_Put
 * @param  datapt pointer to a 8-bit storage location, into which to store the removed data
 * @return FIFOSUCCESS if ok, FIFOFAIL if empty and data could not be removed
 * @brief  Get into RxFifo2
 */
int RxFifo2_Get(char *datapt);

/**
 * @details   Return the number of elements in RxFifo2.
 * @details   Can hold 0 to RX0FIFOSIZE-1 elements
 * @param  none
 * @return number of elements in RxFifo2
 * @brief  Current size of RxFifo2
 */
uint16_t RxFifo2_Size(void);



#endif //  __FIFO2_H__
