#include "test.h"
#include "MSSP1.h"

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

//Byte Counter from ISR
static uint8_t isrCount = 0;

void test_initIO(void)
{
    //Setup LED Outputs
    
    //All LEDs are OFF by default
    LATC5 = 0;
    LATA2 = 0;
    LATA1 = 0;
    LATA5 = 0;
    
    //Set pins as outputs
    TRISC5 = 0;
    TRISA2 = 0;
    TRISA1 = 0;
    TRISA5 = 0;
}

#define TEST_ISR_ITERATIONS 16

inline void test_setLED0(bool state)
{
    LATC5 = state;
}

inline void test_setLED1(bool state)
{
    LATA2 = state;
}

inline void test_setLED2(bool state)
{
    LATA1 = state;
}

inline void test_setLED3(bool state)
{
    LATA5 = state;
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

/* With the SPI in loopback, this runs the following tests:
 * Bulk Exchange (Polling)
 * Bulk Exchange (Interrupt)
 * Single Byte Exchange (Polling)
 * Single Byte Exchange (Interrupt)
 * 
 * Status is shown on the LEDs
*/
void test_runLoopbackTests(void)
{
    test_setLED0(test_SPIBulkExchange());
    test_setLED1(test_SPIBulkInterruptExchange());
    test_setLED2(test_SPISingleByteExchange());
    test_setLED3(test_SPISingleInterruptExchange());
}

/* With the SPI in loopback, this runs the following tests:
 * Bulk Exchange (Polling) - NULL TX
 * Bulk Exchange (Interrupt) - NULL TX
 * Bulk Exchange (Polling) - NULL RX
 * Bulk Exchange (Interrupt) - NULL RX
 * 
 * Status is shown on the LEDs
 */
void test_runNULLTests(void)
{
    test_setLED0(test_SPINullTxPolling());
    test_setLED1(test_SPINullTxInterrupt());
    test_setLED2(test_SPINullRxPolling());
    test_setLED3(test_SPINullRxInterrupt());
}

bool test_SPIBulkExchange(void)
{
    uint8_t memTest_tx[SPI_TEST_BULK_LENGTH], memTest_rx[SPI_TEST_BULK_LENGTH];
    
    test_initArraySeq(&memTest_tx[0], SPI_TEST_BULK_LENGTH);
    test_initArrayStatic(&memTest_rx[0], 0xFF, SPI_TEST_BULK_LENGTH);
    
    MSSP1_transferBytesPolling(&memTest_tx[0], &memTest_rx[0], SPI_TEST_BULK_LENGTH);
    
    bool pass = true;
    
    for (uint8_t i = 0; i < SPI_TEST_BULK_LENGTH; i++)
    {
        if (memTest_tx[i] != memTest_rx[i])
        {
            pass = false;
            i = SPI_TEST_BULK_LENGTH;
        }
    }
    
    return pass;
}

bool test_SPISingleByteExchange(void)
{
    uint8_t tx, rx;
    tx = 0xA5;
    
    rx = MSSP1_transferBytePolling(tx);
    
    if (rx != tx)
    {
        return false;
    }
    
    return true;
}

/* With the SPI connected to a client device (running the test pattern):
 * TX: 0x00, 0xFF, 0xAA, 0x55, 0x00
 * RX: 0x99, 0x88, 0x77, 0x66, 0x55
 * 
 * Exchange bytes and verify data. 
 */
void test_runHostTest(void)
{
    const uint8_t txValue[] = {0x00, 0xFF, 0xAA, 0x55, 0x00};
    const uint8_t rxValidate[] = {0x99, 0x88, 0x77, 0x66, 0x55};
    uint8_t rxValue[5];
    bool pass = true;
    
    MSSP1_transferBytesPolling(&txValue[0], &rxValue[0], 5);
    
    //Verify
    for (uint8_t i = 0; i < 5; i++)
    {
        if (rxValue[i] != rxValidate[i])
        {
            pass = false;
            i = 5;
        }
    }
    
    //Assign LED
    test_setLED0(pass);
}

//Send / Receive multiple bytes in interrupt mode
bool test_SPIBulkInterruptExchange(void)
{
    bool pass = true;
    
    uint8_t memTest_tx[SPI_TEST_BULK_LENGTH], memTest_rx[SPI_TEST_BULK_LENGTH];
    
    test_initArraySeq(&memTest_tx[0], SPI_TEST_BULK_LENGTH);
    test_initArrayStatic(&memTest_rx[0], 0xFF, SPI_TEST_BULK_LENGTH);

    MSSP1_transferBytesInterrupt(&memTest_tx[0], &memTest_rx[0], SPI_TEST_BULK_LENGTH);
    
    //Wait for Transfer Completion
    while (!MSSP1_isInterruptTransferDone());
    
    for (uint8_t i = 0; i < SPI_TEST_BULK_LENGTH; i++)
    {
        if (memTest_tx[i] != memTest_rx[i])
        {
            pass = false;
            i = SPI_TEST_BULK_LENGTH;
        }
    }
    
    return pass;
}

//Send / Receive a single byte in interrupt mode
bool test_SPISingleInterruptExchange(void)
{
    bool pass = true;
    
    uint8_t tx = 0xF0, rx;
    
    //Run Transfer
    MSSP1_transferBytesInterrupt(&tx, &rx, 1);
    
    //Wait...
    while (!MSSP1_isInterruptTransferDone());
    
    if (tx != rx)
    {
        pass = false;
    }
    
    return pass;
}

//Sends Multiple Dummy Values (0x00) in Polling Mode
bool test_SPINullTxPolling(void)
{
    bool pass = true;
    uint8_t memTest_rx[SPI_TEST_BULK_LENGTH];

    test_initArrayStatic(&memTest_rx[0], 0xFF, SPI_TEST_BULK_LENGTH);

    MSSP1_transferBytesPolling(NULL, &memTest_rx[0], SPI_TEST_BULK_LENGTH);
    
    //Wait for Transfer Completion
    while (!MSSP1_isInterruptTransferDone());
    
    for (uint8_t i = 0; i < SPI_TEST_BULK_LENGTH; i++)
    {
        if (0x00 != memTest_rx[i])
        {
            pass = false;
            i = SPI_TEST_BULK_LENGTH;
        }
    }
    
    return pass;
}

//Sends Multiple Values in Polling Mode, with Rx discarded (verify on Logic Analyzer)
bool test_SPINullRxPolling(void)
{
    uint8_t memTest_tx[SPI_TEST_BULK_LENGTH];

    test_initArraySeq(&memTest_tx[0], SPI_TEST_BULK_LENGTH);

    MSSP1_transferBytesPolling(&memTest_tx[0], NULL, SPI_TEST_BULK_LENGTH);
    
    //Wait for Transfer Completion
    while (!MSSP1_isInterruptTransferDone());

    //Verify with Logic Analyzer
    return false;
}

//Sends Multiple Dummy Values (0x00) in Interrupt Mode
bool test_SPINullTxInterrupt(void)
{
    bool pass = true;
    uint8_t memTest_rx[SPI_TEST_BULK_LENGTH];

    test_initArrayStatic(&memTest_rx[0], 0xFF, SPI_TEST_BULK_LENGTH);

    MSSP1_transferBytesInterrupt(NULL, &memTest_rx[0], SPI_TEST_BULK_LENGTH);
    
    //Wait for Transfer Completion
    while (!MSSP1_isInterruptTransferDone());
        
    for (uint8_t i = 0; i < SPI_TEST_BULK_LENGTH; i++)
    {
        if (0x00 != memTest_rx[i])
        {
            pass = false;
            i = SPI_TEST_BULK_LENGTH;
        }
    }

    return pass;
}

//Sends Multiple Values in Interrupt Mode, with Rx discarded (verify on Logic Analyzer)
bool test_SPINullRxInterrupt(void)
{
    uint8_t memTest_tx[SPI_TEST_BULK_LENGTH];

    test_initArraySeq(&memTest_tx[0], SPI_TEST_BULK_LENGTH);

    MSSP1_transferBytesPolling(&memTest_tx[0], NULL, SPI_TEST_BULK_LENGTH);
    
    //Wait for Transfer Completion
    while (!MSSP1_isInterruptTransferDone());
    
    //Verify with Logic Analyzer
    return false;
}

