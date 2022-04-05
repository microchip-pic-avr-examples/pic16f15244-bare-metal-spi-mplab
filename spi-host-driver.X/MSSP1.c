#include "MSSP1.h"

#include <stdint.h>
#include <xc.h>

static uint8_t bLim = 0;
static uint8_t bCount = 0;
static uint8_t* txMem = 0;
static uint8_t* rxMem = 0;

//Init the SPI driver. Enables should be called after this function
void MSSP1_initSPIHost(void)
{    
    SSP1CON1 = 0x00;
#ifdef BAUD_DIV
    SSP1CON1bits.SSPM = 0b1010; //SPI Host Mode at Fosc/(4*SSP1xADD + 1)
    
    //Set the Clock Frequency
    SSP1ADD = BAUD_DIV;
#endif
    SSP1CON3 = 0x00;
    SSP1CON3bits.BOEN = 0b1;    //Enable Buffer Overwrites
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
    SCK_TRIS = 0;
    SCK_PPS_OUT = 0x07;
    SCK_ANSEL = 0;
    SSP1CLKPPS = SCK_PPS_IN;
    
    //CS (General I/O) - Start IO High
    CS_LAT = 1;
    CS_TRIS = 0;
}

//Enable MSSP
inline void MSSP1_enable(void)
{
    //Enable MSSP
    SSP1CON1bits.SSPEN = 0b1;
}

//Asserts the CS (sets the line to 0)
inline void MSSP1_assertCS(void)
{
    CS_LAT = 0;
}

//Deasserts the CS (returns the line to 1)
inline void MSSP1_releaseCS(void)
{
    CS_LAT = 1;
}

//Returns true if the BF flag is set
bool MSSP1_isBufferFull(void)
{
    return SSP1STATbits.BF;
}

//Sends 1 byte and receives 1 byte
uint8_t MSSP1_transferBytePolling(uint8_t sent)
{
    MSSP1_assertCS();
    
    //Clear BF Flag
    uint8_t read = SSP1BUF;
    
    //Write the byte to send
    SSP1BUF = sent;
    
    //Wait...
    while (!MSSP1_isBufferFull()) { ; }
    
    MSSP1_releaseCS();
    
    //Return Byte
    return SSP1BUF;
}

//Sends LEN bytes and receives LEN bytes
void MSSP1_transferBytesPolling(uint8_t* bytesToSend, uint8_t* bytesToReceive, uint8_t len)
{
    MSSP1_assertCS();
    
    //Clear BF Flag
    uint8_t read = SSP1BUF;
    
    for (uint8_t i = 0; i < len; i++)
    {
        //Send Byte
        if (bytesToSend == NULL)
        {
            SSP1BUF = 0x00;
        }
        else
        {
            SSP1BUF = bytesToSend[i];
        }
        
        
        //Wait...
        while (!MSSP1_isBufferFull()) { ; }
        
        //Receive Byte
        uint8_t tempRX = SSP1BUF;
        if (bytesToReceive != NULL)
        {
            bytesToReceive[i] = tempRX;
        }
        
    }
    
    MSSP1_releaseCS();
}

//Starts a non-blocking SPI transfer
void MSSP1_transferBytesInterrupt(uint8_t* bytesToSend, uint8_t* bytesToReceive, uint8_t len)
{
    bCount = 0;
    bLim = len;
    txMem = bytesToSend;
    rxMem = bytesToReceive;
    
    MSSP1_assertCS();
    
    if (txMem != NULL)
    {
        //Send Byte[0]
        SSP1BUF = txMem[0];
    }
    else
    {
        //Dummy Value
        SSP1BUF = 0x00;
    }
    
    //Clear ISR Flag and Enable Interrupts
    PIR1bits.SSP1IF = 0;
    PIE1bits.SSP1IE = 1;
    
    //Remaining Bytes will be send by interrupt
}

//Returns true if the Interrupt Transfer is Complete or Idle
bool MSSP1_isInterruptTransferDone(void)
{
    return (bCount == bLim);
}

void MSSP1_ISR(void)
{
    //Read byte
    uint8_t rx = SSP1BUF;
    
    //Store value if function is defined
    if (rxMem != NULL)
    {
        rxMem[bCount] = rx;
    }
    
    //Increment Counter
    bCount++;
    
    if (bCount == bLim)
    {
        //All Bytes Sent!
        
        //Release CS
        MSSP1_releaseCS();
        
        //Disable Interrupt
        PIE1bits.SSP1IE = 0;
    }
    else
    {
        //Load Buffer
        
        if (txMem != NULL)
        {
            SSP1BUF = txMem[bCount];
        }
        else
        {
            SSP1BUF = 0x00;
        }
    }
    
    //Clear Interrupt Flag
    PIR1bits.SSP1IF = 0;
}