/*  (C) 2022 Microchip Technology Inc. and its subsidiaries.
 *  Subject to your compliance with the conditions set forth herein, redistribution and use in source and binary forms, with or without modification.
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither Microchip?s name nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS SUPPLIED "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
 *  IN NO EVENT WILL MICROCHIP OR ANY CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
 */

// PIC16F15244 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator Mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINTOSC_1MHZ// Power-up Default Value for COSC bits (HFINTOSC (1 MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O function on RA4)
#pragma config VDDAR = HI       // VDD Range Analog Calibration Selection bit (Internal analog systems are calibrated for operation between VDD = 2.3V - 5.5V)

// CONFIG2
#pragma config MCLRE = EXTMCLR  // Master Clear Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RA3 pin function is MCLR)
#pragma config PWRTS = PWRT_OFF // Power-up Timer Selection bits (PWRT is disabled)
#pragma config WDTE = OFF       // WDT Operating Mode bits (WDT disabled; SEN is ignored)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection bit (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config PPS1WAY = OFF    // PPSLOCKED One-Way Set Enable bit (The PPSLOCKED bit can be set and cleared repeatedly by software (subject to unlock sequence))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block is disabled)
#pragma config SAFEN = OFF      // SAF Enable bit (SAF is disabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block is not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block is not write-protected)
#pragma config WRTC = OFF       // Configuration Registers Write Protection bit (Configuration Registers are not write-protected)
#pragma config WRTSAF = OFF     // Storage Area Flash (SAF) Write Protection bit (SAF is not write-protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR. MCLRE Configuration bit is ignored.)

// CONFIG5
#pragma config CP = OFF         // User Program Flash Memory Code Protection bit (User Program Flash Memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "MSSP1.h"
#include "test.h"

//Global Interrupt Service Routine for the device
void __interrupt() ISR(void)
{
    if (PIR1bits.SSP1IF)
    {
        //Call MSSP Interrupt Handler
        MSSP1_ISR();
    }
}

//Enable Interrupts on the Device
void enableInterrupts(void)
{
    INTCON = 0x00;    
    INTCONbits.GIE = 1;     //Enable Interrupts
    INTCONbits.PEIE = 1;    //Enable Peripheral Interrupts
}

void main(void) {
    //Init Test IO
    test_initIO();
    
    //Init MSSP Peripheral
    MSSP1_initIO();
    MSSP1_initSPIHost();
        
    //Enable SPI 
    MSSP1_enable();
    
    //Enable Interrupts on Device
    enableInterrupts();

    //Unit Tests - sets of 4
    //Output is shown on LEDs
    
    //test_runLoopbackTests();
    //test_runNULLTests();          //Note: Only tests 1/2 will pass (3/4 must be verified ext.)
    test_runHostTest();           //Only a Single Test - Must be Connected to Client
    
    while (1)
    {
        
    }
    
    return;
}
