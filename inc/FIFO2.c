// FIFO2.c
// Runs on any microcontroller
// Two first in first out queues
// These will be implemented as part of Lab 18
// Daniel and Jonathan Valvano
// October 29, 2017

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

#include <stdint.h>
#include "../inc/FIFO2.h"

// Implementation of the transmit FIFO, TxFifo2
// can hold 0 to TXFIFOSIZE-1 elements
// you are allowed to restrict TXFIFOSIZE to a power of 2

// add static, volatile, global variables here this as part of Lab 18

uint32_t TxHistogram[TX2FIFOSIZE];
// probability mass function of the number of times TxFifo2 as this size
// as a function of FIFO size at the beginning of call to TxFifo2_Put
// initialize index TxFifo2
void TxFifo2_Init(void){int i;

// write this as part of Lab 18

  for(i=0;i<TX2FIFOSIZE;i++){
      TxHistogram[i] = 0;
  }
}
// add element to end of index TxFifo2
// return TXFIFOSUCCESS if successful
int TxFifo2_Put(char data){
  TxHistogram[TxFifo2_Size()]++;  // probability mass function

// write this as part of Lab 18

  return(FIFOSUCCESS);
}
// remove element from front of TxFifo2
// return TXFIFOSUCCESS if successful
int TxFifo2_Get(char *datapt){

// write this as part of Lab 18


  return(FIFOSUCCESS);
}
// number of elements in TxFifo2
// 0 to TXFIFOSIZE-1
uint16_t TxFifo2_Size(void){

// write this as part of Lab 18

 return 0; // replace this line
}

// Implementation of the receive FIFO, RxFifo2
// can hold 0 to RXFIFOSIZE-1 elements
// you are allowed to restrict RXFIFOSIZE to a power of 2

// add static, volatile, global variables here this as part of Lab 18

// initialize RxFifo2
void RxFifo2_Init(void){

// write this as part of Lab 18

}
// add element to end of RxFifo2
// return FIFOSUCCESS if successful
int RxFifo2_Put(char data){

  // write this as part of Lab 18

  return(FIFOSUCCESS);
}
// remove element from front of RxFifo2
// return FIFOSUCCESS if successful
int RxFifo2_Get(char *datapt){

  // write this as part of Lab 18

  return(FIFOSUCCESS);
}
// number of elements in RxFifo2
// 0 to RXFIFOSIZE-1
uint16_t RxFifo2_Size(void){
// write this as part of Lab 18

 return 0; // replace this line
}


