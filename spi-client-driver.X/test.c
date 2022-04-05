#include "test.h"
#include "MSSP1.h"

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

//Byte Counter from ISR
static uint8_t isrCountTx = 0, isrCountRx = 0;
static uint8_t rxData[TEST_PATTERN_LENGTH];

void test_initIO(void)
{    
    //Setup LED Outputs
    
    //All LEDs are OFF by default
    LATA2 = 1;
    
    //Set pin as outputs
    TRISA2 = 0;
}

inline void test_setLED(bool state)
{
    LATA2 = !state;
}

//Callback function for test pattern
uint8_t test_exampleCallbackTx(void)
{    
    uint8_t retValue = 0x00;
    if (isrCountTx < TEST_PATTERN_LENGTH)
    {
        retValue = testPatternTx[isrCountTx];
        
        //Prevents Overflows
        isrCountTx++;
    }

    return retValue;
}

//Callback function for test pattern Rx
void test_exampleCallbackRx(uint8_t rx)
{
    if (isrCountRx < TEST_PATTERN_LENGTH)
    {
        rxData[isrCountRx] = rx;
        
        isrCountRx++;
    }
}

//Callback function for MSSP complete
void test_exampleCallbackDone(void)
{
    //Reset Tx Counter
    isrCountTx = 0;
}

//Initializes an array to sequential values
void test_initArraySeq(uint8_t* array, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        array[i] = i;
    }
}

//Initializes an array to a static value
void test_initArrayStatic(uint8_t* array, uint8_t val, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        array[i] = val;
    }

}

/* With the SPI connected to the host test pattern, 
 * This function expects to receive, and send:
 * 
 * RX: 0x00, 0xFF, 0xAA, 0x55, 0x00
 * TX: 0x99, 0x88, 0x77, 0x66, 0x55
 * 
 * Status is shown on the LED
 */
void test_runClientTest(void)
{       
    //If data was received, and CS is High...
    if ((!MSSP1_isCSLow()) && (isrCountRx > 1))
    {
        //Reset the SSP1BUF...
        SSP1BUF = testPatternTx[0];
        
        //Wrong number of bytes received
        if (isrCountRx != TEST_PATTERN_LENGTH)
        {
            isrCountRx = 0;
            test_setLED(false);
            return;
        }
        
        bool pass = true;
        
        //Validate Data
        for (uint8_t i = 0; i < TEST_PATTERN_LENGTH; i++)
        {
            if (rxData[i] != testPatternRx[i])
            {
                pass = false;
                i = TEST_PATTERN_LENGTH;
            }
        }
        
        //Set LED state
        test_setLED(pass);
        isrCountRx = 0;
    }
}
