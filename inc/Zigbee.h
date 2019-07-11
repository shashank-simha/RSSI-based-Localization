#ifndef ZIGBEE_H_
#define ZIGBEE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void UART1_Out(uint8_t *, int8_t);
void Outmessage(uint8_t *response, int8_t *framelen);
void InResponce(uint8_t *response, uint16_t *framelen);
void UART1_data(uint8_t data[], uint8_t size);
void ZigbeeHandler(uint8_t *buff, int8_t *framelen);


extern volatile int DB;
#endif /* ZIGBEE_H_ */
