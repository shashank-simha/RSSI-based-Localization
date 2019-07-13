#include <stdint.h>
#include "msp.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/UART1.h"
#include "../inc/UART0.h"
#include "../inc/Clock.h"
#include "../inc/Motor.h"


volatile int DB = 0, M = 8, N = 14;
volatile uint8_t positions[30][30];
volatile uint8_t b_flag = 0;

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

void Location_Filter(uint8_t grid[][30], float rmin, float rmax, float curr_x, float curr_y){

    int i, j;
    N = 14;

    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            if((dist(i, j, curr_x, curr_y) < rmin) || (dist(i, j, curr_x, curr_y) > rmax))
                grid[i][j] = 0;
        }
    }
}

uint16_t Locate_Beacon(int currentdb){
    uint16_t next = 0, i, j;
    float avg_x = 0, avg_y = 0;
    float rmin = 0, rmax = 0;
    N = 14;
    uint8_t prev_positions[30][30];

    for(i = 0; i < M; ++i)
        for(j = 0; j < N; ++j)
            prev_positions[i][j] = positions[i][j];
    float curr_x = x / 0.6096;
    float curr_y = y / 0.6096;

    if(currentdb > 27 && currentdb <= 35){
        rmin = 0.7;
        rmax = 3.2;
    }
    else if(currentdb > 35 && currentdb <= 40){
        rmin = 1.5;
        rmax = 5.5;
    }
    else if(currentdb > 40 && currentdb <= 45){
        rmin = 2.5;
        rmax = 6.4;
    }
    else if(currentdb > 45 && currentdb <= 50){
        rmin = 3.5;
        rmax = 9;
    }
    else if(currentdb > 50 && currentdb <= 55){
        rmin = 4.5;
        rmax = 12;
    }
    else if(currentdb > 55 && currentdb <= 60){
        rmin = 6.9;
        rmax = 15;
    }
    else if(currentdb > 60 && currentdb <= 65){
        rmin = 9;
        rmax = 20;
    }
    else if(currentdb > 65){
        rmin = 13;
        rmax = 100;
    }
    else{
//        ROM_IntEnable(INT_TIMER0A);
//        for(i = 0; i < 2000000; ++i);
        Clock_Delay1ms(20);
        UART0_OutString("Arrived\n\r");
        Location_Filter(positions, 0, 1, curr_x, curr_y);
        Locations_Print();
        return 0;
    }

    Location_Filter(positions, rmin, rmax, curr_x, curr_y);
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            if(positions[i][j]){
                avg_x += i;
                avg_y += j;
            }
        }
    }
    avg_x /= M;
    avg_y /= N;

    if(avg_x == 0 && avg_y == 0){
        for(i = 0; i < M; ++i)
            for(j = 0; j < N; ++j)
                positions[i][j] = prev_positions[i][j];
        rmin -= 2;
        rmax += 2;
        if(rmin < 0.7)
            rmin = 0.7;

        Location_Filter(positions, rmin, rmax, curr_x, curr_y);
        for(i = 0; i < M; ++i){
            for(j = 0; j < N; ++j){
                if(positions[i][j]){
                    avg_x += i;
                    avg_y += j;
                }
            }
        }
        avg_x /= M;
        avg_y /= N;
    }

    if(avg_x == 0 && avg_y == 0){
        UART0_OutString("Lost\n\r");
        return 0;
    }

    float dist = 60;
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j){
            if(positions[i][j] && (dist(i, j, curr_x, curr_y) - 0.7 * dist(i, j, avg_x, avg_y) < dist)){
                dist = dist(i, j, curr_x, curr_y) - dist(i, j, avg_x, avg_y);
                next = (i & 0xFF);
                next |= ((j & 0xFF) << 8);
            }
        }
    }
    return next;
}

void Locations_Init(){
    int i, j;
    N = 14;
    for(i = 0; i < M; ++i){
        for(j = 0; j < N; ++j)
            positions[i][j] = 1;
    }
}

void Locations_Print(){
    int i, j;
    UART0_OutChar('\r');
    N = 14;

    for(i = N - 1; i > -1; --i){
        for(j = 0; j < M; ++j){
            UART0_OutUDec(positions[j][i]);
            UART0_OutChar(' ');
        }
        UART0_OutChar('\n');
        UART0_OutChar('\r');
    }
}
