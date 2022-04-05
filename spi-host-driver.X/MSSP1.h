/*  (C) 2022 Microchip Technology Inc. and its subsidiaries.
 *  Subject to your compliance with the conditions set forth herein, redistribution and use in source and binary forms, with or without modification.
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither Microchip?s name nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS SUPPLIED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
 *  IN NO EVENT WILL MICROCHIP OR ANY CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
 */

#ifndef MSSP1_H
#define	MSSP1_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

//RC7 - SDO
#define SDO_TRIS    TRISC7
#define SDO_PPS     RC7PPS

//RB6 - SCK
#define SCK_TRIS    TRISB6
#define SCK_PPS_OUT RB6PPS
#define SCK_ANSEL   ANSELBbits.ANSB6
#define SCK_PPS_IN  0b001110    //(Set to RB6)  

//RB4 - SDI
#define SDI_TRIS    TRISB4
#define SDI_ANSEL   ANSELBbits.ANSB4
#define SDI_PPS_IN  0b001100    //(Set to RB4)
    
//RC6 - CS
#define CS_TRIS     TRISC6
#define CS_LAT      LATC6
   
/*
 * Baud Rate Divider for SCK
 * Fclk = Fosc/(4*(BAUD_DIV + 1)), BAUD_DIV >= 3
 * 
 * If this MACRO is not defined, Fosc/4 is used instead.
*/
//#define BAUD_DIV    3
        
    //Init the SPI driver. Enable should be called after this function
    void MSSP1_initSPIHost(void);
    
    //Configures the IO Pins used by the MSSP. Must be called manually by the user.
    void MSSP1_initIO(void);
    
    //Enables the SPI Peripheral (does not start communication)
    inline void MSSP1_enable(void);
        
    //Asserts the CS (sets the line to 0)
    inline void MSSP1_assertCS(void);
    
    //Deasserts the CS (returns the line to 1)
    inline void MSSP1_releaseCS(void);
    
    //Returns true if the BF flag is set
    bool MSSP1_isBufferFull(void);
    
    //Sends 1 byte and receives 1 byte
    uint8_t MSSP1_transferBytePolling(uint8_t sent);

    //Sends LEN bytes and receives LEN bytes
    void MSSP1_transferBytesPolling(uint8_t* bytesToSend, uint8_t* bytesToReceive, uint8_t len);
    
    //Starts a non-blocking SPI transfer
    //Interrupts for the MSSP are enabled when this function is called.
    void MSSP1_transferBytesInterrupt(uint8_t* bytesToSend, uint8_t* bytesToReceive, uint8_t len);
    
    //Returns true if the interrupt transfer is still active
    bool MSSP1_isInterruptTransferDone(void);
    
    //Call this function from the interrupt service routine
    void MSSP1_ISR(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MSSP1_H */

