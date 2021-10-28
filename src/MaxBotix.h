#ifndef __MAXBOTIX_H
#define __MAXBOTIX_H

/*
 * Datasheet: https://www.maxbotix.com/documents/LV-MaxSonar-EZ_Datasheet.pdf
 */

#include <Arduino.h>
#include <SPI.h>

#define ECHO_RECD   0x02
#define UART_RECD   0x04
#define ADC_READ    0x08

#define USE_CTRL_PIN    0x01
#define USE_ECHO        0x02
#define USE_UART        0x04
#define USE_ADC         0x08

const float Slope = 58.249;
const float YInt = -115.7;

class MaxBotix 
{
private:
    uint8_t state = 0;

    uint32_t lastPing = 0;          // for keeping track of intervals
    uint32_t pingInterval = 200;    // default to 200 ms

    uint32_t lastADCread = 0;       // can't read the ADC too fast

    uint32_t pulseStart = 0;
    uint32_t pulseEnd = 0;

    String serialString;
public:
    MaxBotix(void);  //ideally, this would take pins as parameters, but just hard-coded for now since we only have one
    void init(void);
    void init(uint8_t interfaces);

    //checks to see if it's time for a ping
    uint8_t checkPingTimer(void);

    //Checks to see if a pulse echo has been registered
    uint16_t checkEcho(void);

    //Reads the MCP3002 ADC; returns ADC result
    uint16_t readMCP3002(bool force = false);

    //Checks/reads on the RS-232 interface
    uint16_t readASCII(void);

    /**
     * TODO: Write a getDistance() function for the distance method of your choice.
     * 
     * See the .cpp file.
     */
    bool getDistance(float& distance);

    //ISR for the MaxBotix sensor
    void MB_ISR(void);
};

extern MaxBotix mb_ez1; 

#endif