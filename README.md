![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# LoRa_4 Click

- **CIC Prefix**  : LORA4
- **Author**      : Nemanja Medakovic
- **Verison**     : 1.0.0
- **Date**        : Feb 2018.

---

### Software Support

We provide a library for the LoRa_4 Click on our [LibStock](https://libstock.mikroe.com/projects/view/2343/lora-4-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

Initializes and defines UART bus driver and driver functions witch offers a choice to writting on EEPROM, reading from EEPROM, communication with
other LoRa Mipot 32001353 click, but first we must join to network and a router is needed. Functions also offers a choice to setting and getting 
channel parameters, and getting indications on some executed commands. For more details check documentation.

Key functions :

- ``` void lora4_process() ``` - Function executes reading indications and forwards indications to handler.
- ``` LORA4_RETVAL_T lora4_writeEeprom( uint8_t address, uint8_t nBytes, uint8_t *dataIn ) ``` - Function writes data to EEPROM.
- ``` LORA4_RETVAL_T lora4_joinNetwork( uint8_t mode ) ``` - Function performs the join network command.
- ``` LORA4_RETVAL_T lora4_txMessage( uint8_t *dataIn, uint8_t nBytes, uint8_t port, uint8_t option ) ``` - Function performs the trasnmission of radio frame.
- ``` LORA4_RETVAL_T lora4_setChannel( uint8_t index, uint32_t freq, uint8_t dataRateRange, uint8_t status ) ``` - Function sets channel parameters and 
  enable/disable optional channels.

**Examples Description**

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes UART bus driver, sets indication handler, sets channel parameters,
  joins network and performs the transmission of radio frame.
- Application Task - (code snippet) - Checks is indication occured.


```.c
void applicationTask()
{
    lora4_process();
}
```

Additional Functions :

- void indicationHandler( uint8_t *cmd, uint8_t *plSize, uint8_t *plBuffer ) - Logs results on USB UART when device
  gets indication on command.

The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/2343/lora-4-click) page.

Other mikroE Libraries used in the example:

- Conversions
- UART

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
