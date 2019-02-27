/*
Example for LoRa_4 Click

    Date          : Feb 2018.
    Author        : Nemanja Medakovic

Test configuration CEC :
    
    MCU              : CEC1702
    Dev. Board       : Clicker 2 for CEC1702
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes UART bus driver, sets indication handler, sets channel parameters,
  joins network and performs the transmission of radio frame.
- Application Task - (code snippet) - Checks is indication occured.

Additional Functions :

- void indicationHandler( uint8_t *cmd, uint8_t *plSize, uint8_t *plBuffer ) - Logs results on USB UART when device
  gets indication on command.

*/

#include "Click_LoRa_4_types.h"
#include "Click_LoRa_4_config.h"

T_lora4_message  tmpMsg;
T_lora4_message  tmpRsp;

void indicationHandler( uint8_t *cmd, uint8_t *plSize, uint8_t *plBuffer )
{
    uint8_t cnt;
    char txt[ 20 ] = { 0 };

    ByteToHex( *cmd, txt );
    mikrobus_logWrite( "IND TYPE: ", _LOG_TEXT );
    Ltrim( txt );
    mikrobus_logWrite( txt, _LOG_LINE );
    mikrobus_logWrite( "PAYLOAD : ", _LOG_TEXT );

    for (cnt = 0; cnt < *plSize; cnt++)
    {
        ByteToHex( plBuffer[ cnt ], txt );
        Ltrim( txt );
        mikrobus_logWrite( txt, _LOG_TEXT );
    }
    mikrobus_logWrite( "", _LOG_LINE );
}


void systemInit()
{
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );

    mikrobus_uartInit( _MIKROBUS1, &_LORA4_UART_CFG[0] );

    mikrobus_logInit( _MIKROBUS2, 115200 );
    mikrobus_logWrite( "Initializing...", _LOG_LINE );

    Delay_ms( 100 );
}

void applicationInit()
{
    uint8_t response;

    lora4_uartDriverInit( (T_LORA4_P)&_MIKROBUS1_GPIO, (T_LORA4_P)&_MIKROBUS1_UART );
    Delay_ms( 100 );
    lora4_hardReset();

    lora4_setIndHandler( indicationHandler );
    tmpMsg.payLoad[ 0 ] = 1;
    tmpMsg.payLoad[ 1 ] = 2;
    tmpMsg.payLoad[ 2 ] = 3;
    tmpMsg.payLoad[ 3 ] = 4;
    tmpMsg.payLoad[ 4 ] = 5;

    if ( !(response = lora4_setChannel( 3, 0x33B4FFE0, 0x52, _LORA4_CHANNEL_DISABLED )) )
        mikrobus_logWrite( "Setting Channel OK", _LOG_LINE );
    else
        mikrobus_logWrite( "Setting Channel Error", _LOG_LINE );
    if ( !(response = lora4_setNextTXDR( 5 )) )
        mikrobus_logWrite( "Setting Data Rate OK", _LOG_LINE );
    else
        mikrobus_logWrite( "Setting Data Rate Error", _LOG_LINE );
    if ( !(response = lora4_joinNetwork( _LORA4_JOIN_ACTIVATION_BY_PERSONALIZATION_MODE )) )
        mikrobus_logWrite( "Joining OK", _LOG_LINE );
    else
        mikrobus_logWrite( "Joining Error", _LOG_LINE );
    if ( !(response = lora4_txMessage( &tmpMsg.payLoad[0], 1, 10, _LORA4_UNCONFIRMED_TRANSMISSION )) )
        mikrobus_logWrite( "Transmission OK", _LOG_LINE );
    else
        mikrobus_logWrite( "Transmission Error", _LOG_LINE );
}

void applicationTask()
{
    lora4_process();
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}
