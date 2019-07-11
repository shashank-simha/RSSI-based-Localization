#include <stdint.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/UART1.h"
#include "../inc/UART0.h"

volatile int DB = 0;
volatile uint8_t positions[30][30];

extern volatile float x, y, theta;

#define dist(x1, y1, x2, y2) (pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5))

void UART1_Out(uint8_t *msg, int8_t len)
{

    int i = 0;
    for (i = 0; i < len; ++i)
    {
        UART1_OutChar(msg[i]);
    }
}

void Outmessage(uint8_t *response, int8_t *framelen)
{

    static uint8_t msg1[] = { 0x7E, 0x00, 0x16, 0x10, 0x01, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00,
                              0x00, 0x46, 0x72, 0x6F, 0x6D, 0x20, 0x42, 0x6F,
                              0x74, 0x18 };   // data = ""
    UART1_Out(msg1, 26);

    InResponce(response, framelen);
    ZigbeeHandler(response, framelen);
}

void InResponce(uint8_t *response, uint16_t *framelen)
{

    uint16_t i;

    // Read until delimeter (0x7E) is encountered
    while (UART1_InChar() != 0x7E)
        ;

    // Start delimater
    response[0] = 0x7E;

    // Read length
    response[1] = UART1_InChar();
    response[2] = UART1_InChar();

    uint16_t len = response[1];
    len << 4;
    len += response[2];

    for (i = 3; i <= len + 3; i++) // including checksum i.e; len of message(0 to len-1) + checksum
        response[i] = UART1_InChar();

    *framelen = 3 + len + 1; // delimeter + len(2 bytes) + data + checksum
}

void UART1_data(uint8_t data[], uint8_t size)
{

    static uint8_t header[] = { 0x7E, 0x00, 0x0E, 0x10, 0x01, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x00,
                                0x00 };
    static uint8_t message[100];

    uint16_t sum = 0x0E;
    uint8_t j = 0;
    for (j = 0; j < 17; ++j)
    {
        message[j] = header[j];
    }
    for (j = 0; j < size; ++j)
    {
        message[j + 17] = data[j];
        sum += data[j];
    }
    message[2] = size + 0x0E;
    message[size + 17] = 0xFF - (sum & 0xFF);
    UART1_Out(message, size + 18);
}

void ZigbeeHandler(uint8_t *buff, uint16_t *framelen)
{

    if (buff[3] == 0x8B)
    {
        if (buff[8] == 0)
        {
            UART0_OutString("message sent\n\r");
            uint8_t db[] = { 0x7E, 0x00, 0x04, 0x08, 0x01, 0x44, 0x42, 0x70 };
            UART1_Out(db, 8);
            InResponce(buff, framelen);
            ZigbeeHandler(buff, framelen);
        }
        else
            UART0_OutString("failed to send message\n\r");
    }
    else if (buff[3] == 0x88)
    {
        if (buff[7] == 0)
        {
            UART0_OutString("DB = ");
            UART0_OutUHex(buff[8]);
            DB = buff[8];
            UART0_OutString("\n\r");
        }
    }
}

