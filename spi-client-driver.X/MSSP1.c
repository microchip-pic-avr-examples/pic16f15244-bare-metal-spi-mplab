#include "MSSP1.h"

#include <stdint.h>
#include <xc.h>
#include <pic16f15244.h>

static void (* callbackDone)(void) = &MSSP1_defaultCallbackDone;
static void (* callbackRx)(uint8_t) = &MSSP1_defaultCallbackRx;
static uint8_t (* callbackTx)(void) = &MSSP1_defaultCallbackTx;

//Init the SPI driver. Enables should be called after this function
void MSSP1_initSPIClient(void)
{    
    SSP1CON1 = 0x00;
    SSP1CON1bits.SSPM = 0b0100; //SPI Client w/ HW Chip Select Control

    SSP1CON3 = 0x00;
    SSP1CON3bits.BOEN = 0b1;    //Enable Buffer Overwrites
    
    //Init MSSP Buffer
    SSP1BUF = 0x00;
    
    //Enable MSSP Interrupts
    PIR1bits.SSP1IF = 0;
    PIE1bits.SSP1IE = 1;
}

//Configures the IO Pins used by the MSSP. Must be called manually by the user.
void MSSP1_initIO(void)
{
    //Configure SDI
    SDI_TRIS = 1;
    SSP1DATPPS = SDI_PPS_IN;
    SDI_ANSEL = 0;
    
    //Configure SDO
    SDO_TRIS = 0;
    SDO_PPS = 0x08;
    
    //Configure SCK
    SCK_TRIS = 1;
    SCK_PPS_OUT = 0x07;
    SCK_ANSEL = 0;
    SSP1CLKPPS = SCK_PPS_IN;
    
    //CS
    CS_TRIS = 1;
    SSP1SSPPS = CS_PPS_IN;
    
    //Enable IOC flags for CS
    CS_IOCxP = 1;
    CS_IOCxN = 1;
    
    //Clear IOC Flags, if set
    CS_IOCxF &= CS_IOCxF_MASK;
    
    //NOTE: IOC Interrupts must be GLOBALLY enabled elsewhere!
}

//Enable MSSP
inline void MSSP1_enable(void)
{
    //Enable MSSP
    SSP1CON1bits.SSPEN = 0b1;
}

//Returns true if the BF flag is set
bool MSSP1_isBufferFull(void)
{
    return SSP1STATbits.BF;
}

//Returns true if CS is Low (indicating transfer in progress)
bool MSSP1_isCSLow(void)
{
    return !(CS_PORT);
}

//Default Callback Function - Always return 0x00
void MSSP1_defaultCallbackRx(uint8_t rx)
{
    //User Callback code could be placed here
    //Or change callback target with MSSP1_setCallbackFunctionRx (Recommended)
}

//Default Callback Function - Always return 0x00
uint8_t MSSP1_defaultCallbackTx(void)
{
    //User Callback code could be placed here
    //Or change callback target with MSSP1_setCallbackFunctionTx (Recommended)
    
    return 0x00;
}

//Default Callback for Completion
void MSSP1_defaultCallbackDone(void)
{
    //User Callback code could be placed here
    //Or change callback target with MSSP1_setCallbackFunctionDone (Recommended)
}

void MSSP1_setCallbackRxFunction(void (* isr)(uint8_t))
{
    callbackRx = isr;
}

//Sets the callback function for data Tx
void MSSP1_setCallbackTxFunction(uint8_t (* isr)(void))
{
    callbackTx = isr;
}

//Sets the callback for when the MSSP finishes
void MSSP1_setCallbackDoneFunction(void (*isr)(void))
{
    callbackDone = isr;
}

void MSSP1_SSP_ISR(void)
{
    //Call Callback Function for RX
    callbackRx(SSP1BUF);
    
    //Call Callback for TX
    SSP1BUF = callbackTx();
    
    //Clear Interrupt Flag
    PIR1bits.SSP1IF = 0;
}

//Call this function from the interrupt service routine when IOC on the CS line occurs
void MSSP1_IOC_ISR(void)
{       
    //Wrong IOC
    if ((CS_IOCxF & ~(CS_IOCxF_MASK)) == 0)
    {
        return;
    }
    
    if (CS_PORT)
    {
        //CS is High
        callbackDone();
    }
    else
    {
        //CS is Low
        
        //Load initial value into buffer
        SSP1BUF = callbackTx();
    }
    
    //Clear IOC Flags
    CS_IOCxF &= CS_IOCxF_MASK;
    PIR0bits.IOCIF = 0b0;
}