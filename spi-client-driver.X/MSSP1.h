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
#define SDO_TRIS        TRISC7
#define SDO_PPS         RC7PPS

//RB6 - SCK
#define SCK_TRIS        TRISB6
#define SCK_PPS_OUT     RB6PPS
#define SCK_ANSEL       ANSELBbits.ANSB6
#define SCK_PPS_IN      0b001110    //(Set to RB6)  

//RB4 - SDI
#define SDI_TRIS        TRISB4
#define SDI_ANSEL       ANSELBbits.ANSB4
#define SDI_PPS_IN      0b001100    //(Set to RB4)
    
//RC6 - CS
#define CS_TRIS         TRISC6
#define CS_LAT          LATC6
#define CS_PORT         PORTCbits.RC6
#define CS_PPS_IN       0b010110    //(Set to RC6)
#define CS_IOCxP        IOCCPbits.IOCCP6
#define CS_IOCxN        IOCCNbits.IOCCN6
#define CS_IOCxF        IOCCF       //To prevent loss of data, use AND mask for this
#define CS_IOCxF_MASK   ~(0x40)     //Clear Mask for IOCxF (AND)
    
    //Init the SPI driver. Enable should be called after this function
    void MSSP1_initSPIClient(void);
    
    //Configures the IO Pins used by the MSSP. Must be called manually by the user.
    void MSSP1_initIO(void);
        
    //Enables the SPI Peripheral (does not start communication)
    inline void MSSP1_enable(void);
    
    //Returns true if the BF flag is set
    bool MSSP1_isBufferFull(void);
    
    //Returns true if CS is Low (indicating transfer in progress)
    bool MSSP1_isCSLow(void);
    
    //Default Callback Function 
    void MSSP1_defaultCallbackRx(uint8_t rx);
    
    //Default Callback Function - Always return 0x00
    uint8_t MSSP1_defaultCallbackTx(void);
    
    //Default Callback for Completion
    void MSSP1_defaultCallbackDone(void);
    
    //Sets the callback function for data Rx
    void MSSP1_setCallbackRxFunction(void (* isr)(uint8_t));
    
    //Sets the callback function for data Rx
    void MSSP1_setCallbackTxFunction(uint8_t (* isr)(void));
    
    //Sets the callback for when the MSSP finishes
    void MSSP1_setCallbackDoneFunction(void (*isr)(void));
    
    //Call this function from the interrupt service routine when the SSP flag is set
    void MSSP1_SSP_ISR(void);
    
    //Call this function from the interrupt service routine when IOC on the CS line occurs
    //Enable both Rising and Falling Edges
    void MSSP1_IOC_ISR(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* MSSP1_H */

