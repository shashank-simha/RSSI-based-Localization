/*
 * Zigbee.c
 *
 *  Created on: 20-June-2019
 *      Author: simha
 */

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/UART1.h"
#include "../inc/UART0.h"
#include "../inc/Clock.h"
#include "../inc/Motor.h"
#include "../inc/LED.h"

volatile int DB = 0, M = 10, N = 10, Center = 4;
volatile uint8_t positions[10][10];

void UART1_Out(uint8_t *msg, int8_t len)
{

    int i = 0;
    for (i = 0; i < len; ++i)
    {
        UART1_OutChar(msg[i]);
    }
}

void Outmessage(uint8_t *response, uint16_t *framelen)
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

void Location_Filter(uint8_t grid[10][10], float rmin, float rmax, int curr_x,
                     int curr_y)
{
    int i, j;
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            if ((Distance(i, j, curr_x, curr_y) < rmin)
                    || (Distance(i, j, curr_x, curr_y) > rmax))
                grid[i][j] = 0;
        }
    }
}

bool Locate_Beacon(int currentdb, int *x1, int *y1)
{
    int i, j;
    bool next = false;
    float rmin = 0, rmax = 0;
    int curr_x, curr_y;
    uint8_t temp_positions[10][10];
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
            temp_positions[i][j] = 1;
    }

    Get_Current_Coordinates(&curr_x, &curr_y);

//    if (currentdb > 0x20 && currentdb <= 0x24)
//    {
//        rmin = 1.5;
//        rmax = 3.0; //2
//    }
    if (currentdb > 0x24 && currentdb <= 0x28)
    {
        rmin = 1.5; //2
        rmax = 4.0; //3
    }
    else if (currentdb > 0x28 && currentdb <= 0x2C)
    {
        rmin = 2.0; //3
        rmax = 5.0; //3.5
    }
    else if (currentdb > 0x2C && currentdb <= 0x30)
    {
        rmin = 3.0; //3.5
        rmax = 6.0; //4
    }
    else if (currentdb > 0x30 && currentdb <= 0x34)
    {
        rmin = 3.5; //4
        rmax = 7.0; //5.25
    }
    else if (currentdb > 0x34 && currentdb <= 0x38)
    {
        rmin = 4.5; //5.25
        rmax = 8.5; //6.75
    }
    else if (currentdb > 0x38 && currentdb <= 0x3C)
    {
        rmin = 6.0; //6.75
        rmax = 11.0; //9
    }
    else if (currentdb > 0x3C && currentdb <= 0x40)
    {
        rmin = 8.0; //9
        rmax = 13.0; //12
    }
    else if (currentdb > 0x40)
    {
        rmin = 11.0; //12
        rmax = 50;
    }
    else
    {
        UART0_OutString("Arrived\n\r");
        Location_Filter(temp_positions, 0, 1, curr_x, curr_y);
        Locations_Print();
        Green_On();
        return 0;
    }

    Location_Filter(temp_positions, rmin, rmax, curr_x, curr_y);
    UART0_OutString("\n\n\r");
    for (i = N - 1; i >= 0; i--)
    {
        for (j = 0; j < M; j++)
        {
            UART0_OutUDec(temp_positions[i][j]);
            UART0_OutChar(' ');
        }

        UART0_OutChar('\n');
        UART0_OutChar('\r');
    }
    UART0_OutString("\n\n\r");

    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++)
            positions[i][j] = positions[i][j] && temp_positions[i][j];

    // set current coordinates to 0
    positions[curr_y][curr_x] = 0;

    int sum = 0, sum_i = 0, sum_j =
            0, count = 0;

    *x1 = 100; //some big number
    *y1 = 100;

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            sum += positions[j][i];
            if (positions[j][i])
            {
                count++;
                sum_i += i;
                sum_j += j;
                if (Distance(curr_x, curr_y, j, i)
                        < Distance(curr_x, curr_y, *x1, *y1))
                {
                    *x1 = j;
                    *y1 = i;
                }
            }

        }
    }

    if (sum == 0)
    {
        UART0_OutString("Lost\n\r");
        Locations_Print();
        Red_On();
        return 0;
    }

    next = true;
    return next;
}

void Locations_Init()
{
    int i, j;
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
            positions[i][j] = 1;
    }
}

void Locations_Print()
{
    int i, j;
    UART0_OutChar('\r');
    for (i = N - 1; i >= 0; i--)
    {
        for (j = 0; j < M; j++)
        {
            UART0_OutUDec(positions[i][j]);
            UART0_OutChar(' ');
        }
        UART0_OutChar('\n');
        UART0_OutChar('\r');
    }
}
