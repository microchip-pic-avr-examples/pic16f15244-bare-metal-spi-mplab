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
    
//Length of Test Pattern
#define TEST_PATTERN_LENGTH 5
    
    //Test Pattern Used to Validate Communication with host
    const uint8_t testPatternTx[] = {0x99, 0x88, 0x77, 0x66, 0x55};
    
    //Validation Test Pattern
    const uint8_t testPatternRx[] = {0x00, 0xFF, 0xAA, 0x55, 0x00};
    
    //Init IO for Testing
    void test_initIO(void);
            
    //Set the state of the test LED
    //Note: On Curiosity Nano, LED is inverted logic (corrected in function)
    inline void test_setLED(bool state);
    
    //Callback function for test pattern Tx
    uint8_t test_exampleCallbackTx(void);
    
    //Callback function for test pattern Rx
    void test_exampleCallbackRx(uint8_t rx);
    
    //Callback function for MSSP complete
    void test_exampleCallbackDone(void);
    
    //Initializes an array to sequential values
    void test_initArraySeq(uint8_t* array, uint8_t len);
    
    //Initializes an array to a static value
    void test_initArrayStatic(uint8_t* array, uint8_t val, uint8_t len);

    /* With the SPI connected to the host test pattern, 
     * This function expects to receive, and send:
     * 
     * RX: 0x00, 0xFF, 0xAA, 0x55, 0x00
     * TX: 0x99, 0x88, 0x77, 0x66, 0x55
     * 
     * Status is shown on the LED
     */
    void test_runClientTest(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* TEST_H */

