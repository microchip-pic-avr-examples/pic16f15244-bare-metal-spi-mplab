/*  (C) 2022 Microchip Technology Inc. and its subsidiaries.
 *  Subject to your compliance with the conditions set forth herein, redistribution and use in source and binary forms, with or without modification.
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither Microchip?s name nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS SUPPLIED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
 *  IN NO EVENT WILL MICROCHIP OR ANY CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
 */

#ifndef TEST_H
#define	TEST_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdbool.h>
#include <stdint.h>
    
//How many bytes to test during bulk transfers
#define SPI_TEST_BULK_LENGTH 16
    
    //Init IO for Testing
    void test_initIO(void);
            
    //Set the state of the test LEDs
    inline void test_setLED0(bool state);
    inline void test_setLED1(bool state);
    inline void test_setLED2(bool state);
    inline void test_setLED3(bool state);
    
    //Initializes an array to sequential values
    void test_initArraySeq(uint8_t* array, uint8_t len);
    
    //Initializes an array to a static value
    void test_initArrayStatic(uint8_t* array, uint8_t val, uint8_t len);

    /* With the SPI in loopback, this runs the following tests:
     * Bulk Exchange (Polling)
     * Bulk Exchange (Interrupt)
     * Single Byte Exchange (Polling)
     * Single Byte Exchange (Interrupt)
     * 
     * Status is shown on the LEDs
     */
    void test_runLoopbackTests(void);
    
    /* With the SPI in loopback, this runs the following tests:
     * Bulk Exchange (Polling) - NULL TX
     * Bulk Exchange (Interrupt) - NULL TX
     * Bulk Exchange (Polling) - NULL RX    (Always Fails) 
     * Bulk Exchange (Interrupt) - NULL RX  (Always Fails)
     * 
     * Status is shown on the LEDs
     */
    void test_runNULLTests(void);
    
    /* With the SPI connected to a client device (running the test pattern):
     * TX: 0x00, 0xFF, 0xAA, 0x55, 0x00
     * RX: 0x99, 0x88, 0x77, 0x66, 0x55
     * 
     * Exchange bytes and verify data. 
     */
    void test_runHostTest(void);
    
    //Send / Receive a lot of data in polling mode
    bool test_SPIBulkExchange(void);
    
    //Send / Receive a single byte in polling mode
    bool test_SPISingleByteExchange(void);
    
    //Send / Receive multiple bytes in interrupt mode
    bool test_SPIBulkInterruptExchange(void);
    
    //Send / Receive a single byte in interrupt mode
    bool test_SPISingleInterruptExchange(void);
    
    //Sends Multiple Dummy Values (0x00) in Polling Mode
    bool test_SPINullTxPolling(void);
    
    //Sends Multiple Values in Polling Mode, with Rx discarded (verify on Logic Analyzer)
    bool test_SPINullRxPolling(void);
    
    //Sends Multiple Dummy Values (0x00) in Interrupt Mode
    bool test_SPINullTxInterrupt(void);
    
    //Sends Multiple Values in Interrupt Mode, with Rx discarded (verify on Logic Analyzer)
    bool test_SPINullRxInterrupt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TEST_H */

