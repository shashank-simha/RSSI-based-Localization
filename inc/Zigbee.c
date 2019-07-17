#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/UART1.h"
#include "../inc/UART0.h"
#include "../inc/Clock.h"
#include "../inc/Motor.h"

volatile int DB = 0, M = 29, N = 29, Center = 14;
volatile uint8_t positions[29][29];
volatile uint8_t b_flag = 0;
static bool min_flag = true;

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

void Location_Filter(uint8_t grid[29][29], float rmin, float rmax, int curr_x,
                     int curr_y)
{
    int i, j;
    for (i = -Center; i <= Center; i++)
    {
        for (j = -Center; j <= Center; j++)
        {
            if ((Distance(i, j, curr_x, curr_y) < rmin)
                    || (Distance(i, j, curr_x, curr_y) > rmax))
                grid[j + Center][i + Center] = 0;
        }
    }
}

bool Locate_Beacon(int currentdb, int *x1, int *y1)
{
    int i, j;
    bool next = false;
    float rmin = 0, rmax = 0;
    int curr_x, curr_y;
    uint8_t temp_positions[29][29];
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
            temp_positions[i][j] = 1;
    }

    Get_Current_Coordinates(&curr_x, &curr_y);

    if (currentdb > 27 && currentdb <= 35)
    {
        rmin = 0.7;
        rmax = 3.2;
    }
    else if (currentdb > 35 && currentdb <= 40)
    {
        rmin = 1.5;
        rmax = 5.5;
    }
    else if (currentdb > 40 && currentdb <= 45)
    {
        rmin = 2.5;
        rmax = 6.4;
    }
    else if (currentdb > 45 && currentdb <= 50)
    {
        rmin = 3.5;
        rmax = 9;
    }
    else if (currentdb > 50 && currentdb <= 55)
    {
        rmin = 4.5;
        rmax = 12;
    }
    else if (currentdb > 55 && currentdb <= 60)
    {
        rmin = 6.9;
        rmax = 15;
    }
    else if (currentdb > 60 && currentdb <= 65)
    {
        rmin = 9;
        rmax = 20;
    }
    else if (currentdb > 65)
    {
        rmin = 13;
        rmax = 100;
    }
    else
    {
        UART0_OutString("Arrived\n\r");
        Location_Filter(temp_positions, 0, 1, curr_x, curr_y);
        Locations_Print();
        return 0;
    }

    Location_Filter(temp_positions, rmin, rmax, curr_x, curr_y);
    UART0_OutString("\n\n\r");
    for (i = N-1; i >= 0; i--)
    {
        if (i == Center)
            UART0_OutString("\n\r");
        for (j = 0; j < M; j++)
        {
            if (j == Center)
                UART0_OutChar(' ');
            UART0_OutUDec(temp_positions[i][j]);
            UART0_OutChar(' ');
            if (j == Center)
                UART0_OutChar(' ');
        }
        if (i == Center)
            UART0_OutString("\n\r");
        UART0_OutChar('\n');
        UART0_OutChar('\r');
    }
    UART0_OutString("\n\n\r");
    Clock_Delay1ms(3000);

    for (i = 0; i < M; i++)
        for (j = 0; j < N; j++)
            positions[i][j] = positions[i][j] && temp_positions[i][j];

    // set current coordinates to 0
    positions[curr_y + Center][curr_x + Center] = 0;

    // next position = min(i+j) where position[i][j] == 1, if min_flag == 1
    //                 max(i+j) where position[i][j] == 1, if min_flag == 0
    // if all grid elements are zero, print "Lost"
    int min_i = curr_x, min_j = curr_y, max_i = curr_x, max_j = curr_y, sum = 0;
    for (i = -Center; i <= Center; i++)
    {
        for (j = -Center; j <= Center; j++)
        {
            sum += positions[j + Center][i + Center];

            if (positions[j + Center][i + Center]
                    && ((i + j) < (min_i + min_j)))
            {
                min_i = i;
                min_j = j;
            }

            if (positions[j + Center][i + Center]
                    && ((i + j) > (max_i + max_j)))
            {
                max_i = i;
                max_j = j;
            }
        }
    }

    if (sum == 0)
    {
        UART0_OutString("Lost\n\r");
        Locations_Print();
        return 0;
    }

    if (min_flag)
    {
        *x1 = min_i;
        *y1 = min_j;
    }

    else
    {
        *x1 = max_i;
        *y1 = max_j;
    }

    // toggle min_flag
    min_flag = !min_flag;
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
        if (i == Center)
            UART0_OutString("\n\r");
        for (j = 0; j < M; j++)
        {
            if (j == Center)
                UART0_OutChar(' ');
            UART0_OutUDec(positions[i][j]);
            UART0_OutChar(' ');
            if (j == Center)
                UART0_OutChar(' ');
        }
        if (i == Center)
            UART0_OutString("\n\r");
        UART0_OutChar('\n');
        UART0_OutChar('\r');
    }
}
