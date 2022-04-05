<!-- Please do not change this logo with link -->

[![MCHP](images/microchip.png)](https://www.microchip.com)

# Bare Metal SPI (MSSP) Drivers for PIC16F15244

This example provides bare metal SPI drivers for the PIC16F15244 family of MCUs. These drivers are designed to be lightweight, simple implementations that can be used to create highly efficient applications in performance or power-critical environments.  

## Related Documentation

- [Bare Metal I<sup>2</sup>C drivers for PIC16F15244](https://github.com/microchip-pic-avr-examples/pic16f15244-bare-metal-i2c-mplab)
- [PIC16F15244 Family of MCUs](https://www.microchip.com/en-us/products/microcontrollers-and-microprocessors/8-bit-mcus/pic-mcus/pic16f15244?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-spi-mplab-github)

## Software Used

- [MPLAB® X IDE 6.0.0 or newer](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-spi-mplab-github)
- [MPLAB® XC8 2.36.0 or newer](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-spi-mplab-github)
- PIC16F1xxxx_DFP v1.10.174

## Hardware Used

For testing:
- SPI Host: [Curiosity Nano Development Board (DM164137)](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM164137?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-spi-mplab-github)
   - With PIC16F15244 DIP
- SPI Client: [PIC16F15244 Curiosity Nano Evaluation Kit (EV09Z19A)](https://www.microchip.com/en-us/development-tool/EV09Z19A?utm_source=GitHub&utm_medium=TextLink&utm_campaign=MCU8_MMTCha_pic152xx&utm_content=pic16f15244-bare-metal-spi-mplab-github)
- Wire

## General Information

### Pin Rerouting

To move the pin assignments around, the defines at the top of `MSSP1.h` can be modified to select a different I/O location.

### Test Application

This example isn't an application - it's a series of test vectors that were used to test the driver and verify functionality. The test vectors show how to setup, utilize, and use the driver.

### Loopback Wiring

For some of the host mode tests, loopback mode was used for testing. To setup loopback mode, connect SDO and SDI together so that the output of the SPI module is also connected to the input.

### Driver Portability

Both drivers are expected to be natively portable across the PIC16F15244 family. (I/O may need to be reassigned). Other parts with an MSSP peripheral may require minor changes for functionality, due to changed bit positions.

## Host API

Device Used: Curiosity Nano Development Board

### Initialization

The following functions must be called to initialize the driver:

- `MSSP1_initIO`
- `MSSP1_initSPIHost`

These functions configure the MSSP I/O and the peripheral settings. After initialized, call `MSSP1_enable` to enable the peripheral.

If interrupt (non-blocking) modes are used, then the global and peripheral interrupt enables (in INTCON) must be set as well. *Note: The MSSP interrupt is enabled as needed by the driver.*

#### Sample Initialization Code

```
//Init MSSP Peripheral
MSSP1_initIO();
MSSP1_initSPIHost();

//Enable SPI
MSSP1_enable();

//Enable Interrupts on Device
enableInterrupts();
```

### Function Reference

| Function Name | Description
| ------------- | -----------
| void MSSP1_initSPIHost(void) | Initializes the MSSP Peripheral as a SPI Host.
| void MSSP1_initIO(void) | Initializes the I/O used by the MSSP Peripheral.
| void MSSP1_enable(void) | Enables the MSSP Peripheral.
| void MSSP1_assertCS(void) | Asserts the Chip Select (CS) line (sets to 0).
| void MSSP1_releaseCS(void) | Releases the CS line (sets to 1).
| bool MSSP1_isBufferFull(void) | Returns true if the Buffer Full (BF) flag is set.
| uint8_t MSSP1_transferBytePolling(uint8_t sent) | Exchanges a single byte with a client. **Function is blocking.**
| void MSSP1_transferBytesPolling(uint8_t* bytesToSend, uint8_t* bytesToReceive, uint8_t len) | Exchanges multiple bytes with a client. **Function is blocking.**
| void MSSP1_transferBytesInterrupt(uint8_t* bytesToSend, uint8_t* bytesToReceive, uint8_t len) | Exchanges multiple bytes with a client. **Function is non-blocking.**
| bool MSSP1_isInterruptTransferDone(void) | Returns true if the interrupt driven (non-blocking) function has completed transmitting bytes, or is idle.
| void MSSP1_ISR(void) | Call this function from your Interrupt Service Routine (ISR) when the SSP flag is set.

### Required Interrupts
This driver can be run in either polling or interrupt driven (non-blocking) mode. If interrupt-driven mode is used, the SSP interrupt from the **User's ISR** must call the function `MSSP1_ISR`. When the MSSP has completed data transmission, the interrupt is disabled.

Interrupt and polling modes may be used in the same program, but must not be called at the same time.

### User-Interrupt Configuration
This API does not have any callbacks or other functions that can be set at runtime.

### SPI Clock Configuration

By default, the driver uses **Fosc / 4** as the SPI clock. If the macro `BAUD_DIV` is defined, then the peripheral will use this formula:

 **Fclk = Fosc / (4 * (BAUD_DIV + 1))**

 Where `BAUD_DIV` is an 8-bit value >= 3.

### Test Patterns

The LEDs on the Curiosity Development Board are used to display the test status. From left to right, they are considered (for this example) to be 0 through 3, e.g.: [0123].

#### Loopback
Tests the driver by sending data to itself. These tests confirm the module is sending and receiving data correctly.

| Test | Function | Description | Expected Output
| ---- | -------- | ------------ | -----
| Test 0 | `test_SPIBulkExchange` | Sends and receives multiple bytes in polling mode. | PASS
| Test 1 | `test_SPIBulkInterruptExchange` | Sends and receives multiple bytes in interrupt mode. | PASS
| Test 2 | `test_SPISingleByteExchange` | Sends and receives a single byte in polling mode. | PASS
| Test 3 | `test_SPISingleInterruptExchange` | Sends and receives a single byte in interrupt mode. | PASS

#### NULL
Tests the driver's ability to handle data when no user-function is set. **Connect in loopback mode.** A logic analyzer is recommended for Tx tests and required for Rx tests.

| Test | Function | Description | Expected Output
| ---- | -------- | ------------ | -----
| Test 0 | `test_SPINullTxPolling` | Leaves the Tx function as default in polling mode. This sends dummy bytes (0x00) to the receiver.  | PASS
| Test 1 | `test_SPINullTxInterrupt` | Leaves the Tx function as default in interrupt mode. This sends dummy bytes (0x00) to the receiver. | PASS
| Test 2 | `test_SPINullRxPolling` | Leaves the Rx function as default in polling mode. Data is transmitted, but data received is not processed. | FAIL
| Test 3 | `test_SPINullRxInterrupt` | Leaves the Rx function as default in interrupt mode. Data is transmitted, but data received is not processed. | FAIL

(Note: There is not a simple way to validate the Null Rx on the device.)

#### Client
Tests the driver by communicating with a client device (running the appropriate test pattern). The byte sequence is completely arbitrary and can be changed (although the host pattern must also be changed as well).

| Test | Function | Description | Expected Output
| ---- | -------- | ------------ | -----
| Test 0 | `test_runHostTest` | Sends {0x00, 0xFF, 0xAA, 0x55, 0x00} to a client and expects to receive {0x99, 0x88, 0x77, 0x66, 0x55} back.  | PASS

All other tests will FAIL.

## Client API

Device Used: PIC16F15244 Curiosity Nano Evaluation Kit

### Initialization

The following functions must be called to initialize the driver:

- `MSSP1_initIO`
- `MSSP1_initSPIClient`

These functions configure the I/O used for SPI and the MSSP peripheral settings. In most cases, the functions below are also called to setup callbacks:

- `MSSP1_setCallbackRxFunction`
- `MSSP1_setCallbackTxFunction`
- `MSSP1_setCallbackDoneFunction`

Additionally, the application must enable the Interrupt-on-Change (IOC) interrupts. Once all callbacks and peripheral interrupts are enabled, the MSSP can be enabled.

- `MSSP1_enable`

After the peripheral is enabled, the global and peripheral interrupt enables (in INTCON) must be set.

#### Example Initialization Code

```   
//Init MSSP Peripheral
MSSP1_initIO();
MSSP1_initSPIClient();
IOC_enableInterrupts();

//Set callbacks
MSSP1_setCallbackRxFunction(&test_exampleCallbackRx);
MSSP1_setCallbackTxFunction(&test_exampleCallbackTx);
MSSP1_setCallbackDoneFunction(&test_exampleCallbackDone);

//Enable SPI
MSSP1_enable();

//Enable Interrupts on Device
enableInterrupts();
```

### Function Reference

| Function Name | Description
| ------------- | -----------
| void MSSP1_initSPIClient(void) | Initializes the MSSP Peripheral as a SPI Client.
| void MSSP1_initIO(void) | Initializes the I/O used by the MSSP Peripheral, including any IOC pin settings required.
| void MSSP1_enable(void) | Enables the MSSP Module.
| bool MSSP1_isBufferFull(void) | Returns true if the Buffer Full (BF) flag is set.
| bool MSSP1_isCSLow(void) | Returns true if Chip Select (CS) is low.
| void MSSP1_defaultCallbackRx(uint8_t rx) | Default Callback function for byte received. Called automatically, see **User-Interrupt Configuration** for more.
| uint8_t MSSP1_defaultCallbackTx(void) | Default callback function for byte transmit. Called automatically, see **User-Interrupt Configuration** for more.
| void MSSP1_defaultCallbackDone(void) | Default callback function when serial is complete. Called automatically, see **User-Interrupt Configuration** for more.
| void MSSP1_setCallbackRxFunction(void (* isr)(uint8_t)) | Sets the callback function when a byte is received.
| void MSSP1_setCallbackTxFunction(uint8_t (* isr)(void)) | Sets the callback function when a byte needs to be sent.
| void MSSP1_setCallbackDoneFunction(void (*isr)(void)) | Sets the default callback function when the serial data transfer is complete.
| void MSSP1_SSP_ISR(void) | Call this function from your Interrupt Service Routine (ISR) when the SSP flag is set.
| void MSSP1_IOC_ISR(void) | Call this function from your ISR when an Interrupt-on-Change (IOC) occurs. This function automatically ignores IOC for unrelated pins.

### Required Interrupts

This driver requires 2 interrupts - the SSP flag from the peripheral, and the IOC flag for the CS. When these interrupts occur, `MSSP1_SSP_ISR` and `MSSP1_IOC_ISR` must be called for the driver to properly operate.

Note: In the case of the IOC interrupt, the function checks the IOC for the correct pin. If the IOC was for a different pin, the function returns to the ISR without performing any actions.

### User-Interrupt Configuration
The SPI Client driver generates 3 interrupts:

- Byte Tx
- Byte Rx
- Done

Each can be assigned to a user defined function without modifying the driver itself. This is done by calling the functions `MSSP1_setCallbackRxFunction`, `MSSP1_setCallbackTxFunction` or `MSSP1_setCallbackDoneFunction`. If no function is provided, the driver will run fail-safe default behavior, as shown below.

| Interrupt | Default Behavior
| --------- | ---------------
| Byte Tx | Send 0x00
| Byte Rx | Data is discarded
| Done | No action

### Operating Requirements

Since the client driver must handle data from the MSSP before the host continues transmitting data, the operating frequency of the client device is usually higher than that of the host. If the device is not fast enough to complete execution before data continues, unexpected behavior may occur. This issue can be resolved in 2 ways:

- Increase the idle time between data bytes at the host
- Decrease SPI Frequency
- Increase the operating frequency of the client device

### Test Setup

The driver can be tested by communicating with a host device (running the appropriate test pattern). The byte sequence is completely arbitrary and can be changed (although the host pattern must also be changed as well).

| Test | Function | Description | Expected Output
| ---- | -------- | ------------ | -----
| Test 0 | `test_runHostTest` | Sends {0x99, 0x88, 0x77, 0x66, 0x55} to the host and expects to receive {0x00, 0xFF, 0xAA, 0x55, 0x00}.  | PASS

Note: There is only 1 LED on the Curiosity Nano.

## Summary

This project provides a simple SPI driver for use with the PIC16F15244 family of MCUs.
