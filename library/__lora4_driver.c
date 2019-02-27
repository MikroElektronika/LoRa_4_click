/*
    __lora4_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__lora4_driver.h"
#include "__lora4_hal.c"

/* ------------------------------------------------------------------- MACROS */

#define FRAME_HEADER        0xAA
#define RESPONSE_BIT        0x80

/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __LORA4_DRV_I2C__
static uint8_t _slaveAddress;
#endif

T_lora4_indHdl _indHanlder;

const uint8_t _LORA4_CMD_RESET = 0x30;
const uint8_t _LORA4_CMD_FACTORY_RESET = 0x31;
const uint8_t _LORA4_CMD_EEPROM_WRITE = 0x32;
const uint8_t _LORA4_CMD_EEPROM_READ = 0x33;
const uint8_t _LORA4_CMD_GET_FW_VERSION = 0x34;
const uint8_t _LORA4_CMD_GET_SERIAL_NO = 0x35;
const uint8_t _LORA4_CMD_GET_DEV_EUI = 0x36;
const uint8_t _LORA4_CMD_JOIN = 0x40;
const uint8_t _LORA4_IND_JOIN = 0x41;
const uint8_t _LORA4_CMD_GET_ACTIVATION_STATUS = 0x42;
const uint8_t _LORA4_CMD_SET_APP_KEY = 0x43;
const uint8_t _LORA4_CMD_SET_APP_SESSION_KEY = 0x44;
const uint8_t _LORA4_CMD_SET_NWK_SESSION_KEY = 0x45;
const uint8_t _LORA4_CMD_TX_MSG = 0x46;
const uint8_t _LORA4_IND_TX_MSG_CONFIRMED = 0x47;
const uint8_t _LORA4_IND_TX_MSG_UNCONFIRMED = 0x48;
const uint8_t _LORA4_IND_RX_MSG = 0x49;
const uint8_t _LORA4_CMD_GET_SESSION_STATUS = 0x4A;
const uint8_t _LORA4_CMD_SET_NEXT_DR = 0x4B;
const uint8_t _LORA4_CMD_SET_BATTERY_LVL = 0x50;
const uint8_t _LORA4_CMD_GET_BATTERY_LVL = 0x51;
const uint8_t _LORA4_CMD_SET_UPLINK_CNT = 0x52;
const uint8_t _LORA4_CMD_GET_UPLINK_CNT = 0x53;
const uint8_t _LORA4_CMD_SET_DOWNLINK_CNT = 0x54;
const uint8_t _LORA4_CMD_GET_DOWNLINK_CNT = 0x55;
const uint8_t _LORA4_CMD_SET_CH_PARAMETERS = 0x57;
const uint8_t _LORA4_CMD_GET_CH_PARAMETERS = 0x58;

const uint8_t _LORA4_JOIN_ACTIVATION_BY_PERSONALIZATION_MODE = 0x00;
const uint8_t _LORA4_JOIN_OVER_THE_AIR_ACTIVATION_MODE = 0x01;
const uint8_t _LORA4_GET_ACTIVATION_MODE = 0x00;
const uint8_t _LORA4_GET_SESSION_STATUS_MODE = 0x01;
const uint8_t _LORA4_SET_APP_KEY_MODE = 0x00;
const uint8_t _LORA4_SET_NWK_SESSION_KEY_MODE = 0x01;
const uint8_t _LORA4_UPLINK_CNT = 0x00;
const uint8_t _LORA4_DOWNLINK_CNT = 0x01;
const uint8_t _LORA4_CHANNEL_DISABLED = 0x00;
const uint8_t _LORA4_CHANNEL_ENABLED = 0x01;
const uint8_t _LORA4_UNCONFIRMED_TRANSMISSION = 0x00;
const uint8_t _LORA4_CONFIRMED_TRANSMISSION = 0x01;

const uint8_t _LORA4_OK = 0x00;
const uint8_t _LORA4_ERR_ADDR_RANGE = 0x01;
const uint8_t _LORA4_ERR_NOT_IDLE = 0x02;

const uint8_t _LORA4_ERR_PL_LEN = 0xFC;
const uint8_t _LORA4_ERR_CHKSUM = 0xFD;
const uint8_t _LORA4_ERR_RESPONSE = 0xFE;
const uint8_t _LORA4_ERR_HEADER = 0xFF;

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */

uint8_t chkSum( T_lora4_message *cmd );

void __memcpy(uint8_t *dst, uint8_t *src, uint8_t length);

void sendMessage( T_lora4_message *msg );

uint8_t readMessage( T_lora4_message *msg );

/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */

void __memcpy(uint8_t *dst, uint8_t *src, uint8_t length)
{
    uint8_t *tmpSrc = src;

    while (length--)
    {
        *dst++ = *src++;
    }
}

uint8_t chkSum( T_lora4_message *cmd )
{
    uint8_t cnt = 0;
    uint8_t res = FRAME_HEADER;

    res += cmd->command_;
    res += cmd->payLoadSize;

    for (cnt = 0; cnt < cmd->payLoadSize; cnt++)
    {
        res += cmd->payLoad[cnt];
    }

    res = ~res;
    res += 1;

    return res;
}

void sendMessage( T_lora4_message *msg )
{
    uint8_t cnt = 0;
    uint8_t crc = chkSum( msg );

    hal_gpio_csSet( 0 );  // NWAKE CTL
    Delay_100ms();

    hal_uartWrite( FRAME_HEADER );
    hal_uartWrite( msg->command_ );
    hal_uartWrite( msg->payLoadSize );

    for (cnt = 0; cnt < msg->payLoadSize; cnt++ )
    {
        hal_uartWrite( msg->payLoad[ cnt ] );
    }

    hal_uartWrite( crc );
}

uint8_t readMessage( T_lora4_message *msg )
{
    uint8_t  crc    = 0;
    uint16_t rxCnt  = 0;
    uint8_t  ftx    = 1;
    uint8_t  plCnt  = 255;

    while (1)
    {
        /*
            Blocking calls

            TODO : Implement watchdog
        */

        /* Head *//* Response command *//* Payload size */

        while (!hal_uartReady()); if (hal_uartRead() != FRAME_HEADER) return _LORA4_ERR_HEADER;
        while (!hal_uartReady()); msg->command_ = hal_uartRead();
        while (!hal_uartReady()); plCnt = msg->payLoadSize = hal_uartRead();

        /* Payload */

        while (plCnt)
        {
            if (hal_uartReady())
            {
                msg->payLoad[rxCnt++] = hal_uartRead();
                plCnt--;
            }
        }
        break;
    }

    /* CRC */

    while (!hal_uartReady()); crc = hal_uartRead();
    hal_gpio_csSet( 1 );
    Delay_100ms();

    return ( crc == chkSum(msg) ? _LORA4_OK : _LORA4_ERR_CHKSUM );
}

/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __LORA4_DRV_SPI__

void lora4_spiDriverInit(T_LORA4_P gpioObj, T_LORA4_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __LORA4_DRV_I2C__

void lora4_i2cDriverInit(T_LORA4_P gpioObj, T_LORA4_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    // ... power ON
    // ... configure CHIP
}

#endif
#ifdef   __LORA4_DRV_UART__

void lora4_uartDriverInit(T_LORA4_P gpioObj, T_LORA4_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );

    hal_gpio_rstSet( 1 );
    hal_gpio_csSet( 1 );
    Delay_1sec();
}

#endif

/* ----------------------------------------------------------- IMPLEMENTATION */

void lora4_hardReset()
{
    hal_gpio_rstSet( 0 );
    Delay_100ms(); Delay_100ms(); Delay_100ms(); Delay_100ms(); Delay_100ms();  // 500 ms
    hal_gpio_rstSet( 1 );
    Delay_1sec();
}

void lora4_wakeUp( uint8_t state )
{
    hal_gpio_csSet( state );
    Delay_100ms();
}

void lora4_setIndHandler( T_lora4_indHdl hdl )
{
    _indHanlder = hdl;
}

uint8_t lora4_command( T_lora4_message *msg, T_lora4_message *rsp )
{
    T_lora4_message *tmpMsg = msg;
    T_lora4_message *tmpRsp = rsp;

    sendMessage( tmpMsg );
    return readMessage( tmpRsp );
}

void lora4_process()
{
    if (0 == hal_gpio_intGet())
    {
        uint8_t res;
        T_lora4_message tmp;

        if (0 == (res = readMessage( &tmp )))
            _indHanlder( &tmp.command_, &tmp.payLoadSize, &tmp.payLoad[0] );
    }
}

/* --------------------------------------------------------------- API CALLS */

LORA4_RETVAL_T
lora4_softReset( void )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    cmdMsg.command_ = _LORA4_CMD_RESET;
    cmdMsg.payLoadSize = 0x00;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) == rspMsg.command_)
        return _LORA4_OK;

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_factoryReset( void )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    cmdMsg.command_ = _LORA4_CMD_FACTORY_RESET;
    cmdMsg.payLoadSize = 0x00;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[0];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_writeEeprom( uint8_t address, uint8_t nBytes, uint8_t *dataIn )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (nBytes > 254)
        return _LORA4_ERR_PL_LEN;

    cmdMsg.command_ = _LORA4_CMD_EEPROM_WRITE;
    cmdMsg.payLoadSize = nBytes + 1;
    cmdMsg.payLoad[ 0 ] = address;
    __memcpy( &cmdMsg.payLoad[1], dataIn, nBytes );

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[0];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_readEeprom( uint8_t address, uint8_t nBytes, uint8_t *dataOut )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (nBytes > 254)
        return _LORA4_ERR_PL_LEN;

    cmdMsg.command_ = _LORA4_CMD_EEPROM_READ;
    cmdMsg.payLoadSize = 2;
    cmdMsg.payLoad[0] = address;
    cmdMsg.payLoad[1] = nBytes;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    __memcpy( dataOut, &rspMsg.payLoad[1], nBytes );

    return rspMsg.payLoad[0];
}

LORA4_RETVAL_T
lora4_getFwVersion( uint32_t *fwVersion )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;
    uint32_t temp = 0;

    cmdMsg.command_ = _LORA4_CMD_GET_FW_VERSION;
    cmdMsg.payLoadSize = 0;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 4)
    {
        temp = rspMsg.payLoad[ 3 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 2 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 1 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 0 ];
        *fwVersion = temp;
        return _LORA4_OK;
    }

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_getSerialNo( uint32_t *serialNo )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;
    uint32_t temp = 0;

    cmdMsg.command_ = _LORA4_CMD_GET_SERIAL_NO;
    cmdMsg.payLoadSize = 0;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 4)
    {
        temp = rspMsg.payLoad[ 3 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 2 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 1 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 0 ];
        *serialNo = temp;
        return _LORA4_OK;
    }

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_getDevEUI( uint8_t *devEUI )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    cmdMsg.command_ = _LORA4_CMD_GET_DEV_EUI;
    cmdMsg.payLoadSize = 0;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 8)
    {
        __memcpy( devEUI, &rspMsg.payLoad[0], 8 );
        return _LORA4_OK;
    }

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_joinNetwork( uint8_t mode )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    cmdMsg.command_ = _LORA4_CMD_JOIN;
    cmdMsg.payLoadSize = 1;
    cmdMsg.payLoad[ 0 ] = mode;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[ 0 ];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_getStatus( uint8_t statusMode )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (statusMode == 0)
        cmdMsg.command_ = _LORA4_CMD_GET_ACTIVATION_STATUS;
    else
        cmdMsg.command_ = _LORA4_CMD_GET_SESSION_STATUS;
    cmdMsg.payLoadSize = 0;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[ 0 ];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_setKey( uint8_t *keyData, uint8_t keyMode )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (keyMode == 0)
        cmdMsg.command_ = _LORA4_CMD_SET_APP_KEY;
    else if (keyMode == 1)
        cmdMsg.command_ = _LORA4_CMD_SET_APP_SESSION_KEY;
    else
        cmdMsg.command_ = _LORA4_CMD_SET_NWK_SESSION_KEY;

    cmdMsg.payLoadSize = 0x10;
    __memcpy( &cmdMsg.payLoad[0], keyData, cmdMsg.payLoadSize );

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    return _LORA4_OK;
}

LORA4_RETVAL_T
lora4_txMessage( uint8_t *dataIn, uint8_t nBytes, uint8_t port, uint8_t option )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (nBytes > 253)
        return _LORA4_ERR_PL_LEN;
    if (port < 1)
        port = 1;
    else if (port > 223)
        port = 223;

    cmdMsg.command_ = _LORA4_CMD_TX_MSG;
    cmdMsg.payLoadSize = nBytes + 2;
    cmdMsg.payLoad[ 0 ] = option;
    cmdMsg.payLoad[ 1 ] = port;
    __memcpy( &cmdMsg.payLoad[2], dataIn, nBytes );

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[ 0 ];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_setNextTXDR( uint8_t dataRate )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (dataRate > 7)
        dataRate = 7;

    cmdMsg.command_ = _LORA4_CMD_SET_NEXT_DR;
    cmdMsg.payLoadSize = 1;
    cmdMsg.payLoad[ 0 ] = dataRate;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[ 0 ];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_setBatteryLevel( uint8_t batteryLevel )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    cmdMsg.command_ = _LORA4_CMD_SET_BATTERY_LVL;
    cmdMsg.payLoadSize = 1;
    cmdMsg.payLoad[ 0 ] = batteryLevel;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    return _LORA4_OK;
}

LORA4_RETVAL_T
lora4_getBatteryLevel( void )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    cmdMsg.command_ = _LORA4_CMD_GET_BATTERY_LVL;
    cmdMsg.payLoadSize = 0;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[ 0 ];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_setCounter( uint32_t cnt, uint8_t cntMode )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;

    if (cntMode == 0)
        cmdMsg.command_ = _LORA4_CMD_SET_UPLINK_CNT;
    else
        cmdMsg.command_ = _LORA4_CMD_SET_DOWNLINK_CNT;
    cmdMsg.payLoadSize = 4;
    cmdMsg.payLoad[ 0 ] = cnt & 0x000000FF;
    cmdMsg.payLoad[ 1 ] = (cnt >> 8) & 0x000000FF;
    cmdMsg.payLoad[ 2 ] = (cnt >> 16) & 0x000000FF;
    cmdMsg.payLoad[ 3 ] = cnt >> 24;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    return _LORA4_OK;
}

LORA4_RETVAL_T
lora4_getCounter( uint32_t *cnt, uint8_t cntMode )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;
    uint32_t temp = 0;

    if (cntMode == 0)
        cmdMsg.command_ = _LORA4_CMD_GET_UPLINK_CNT;
    else
        cmdMsg.command_ = _LORA4_CMD_GET_DOWNLINK_CNT;
    cmdMsg.payLoadSize = 0;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 4)
    {
        temp = rspMsg.payLoad[ 3 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 2 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 1 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 0 ];
        *cnt = temp;
        return _LORA4_OK;
    }

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_setChannel( uint8_t index, uint32_t freq, uint8_t dataRateRange, uint8_t status )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;
    uint8_t minDRR = dataRateRange & 0x0F;
    uint8_t maxDRR = dataRateRange & 0xF0;

    if (index < 3)
        index = 3;
    else if (index > 15)
        index = 15;
    if (maxDRR > 0x70)
    {
        maxDRR &= 0x00;
        dataRateRange |= 0x70;
    }
    if (minDRR > 0x07)
    {
        minDRR &= 0x00;
        dataRateRange |= 0x07;
    }
    if (minDRR > (maxDRR >> 4))
        minDRR = maxDRR >> 4;
    if (status > 1)
        status = 1;

    cmdMsg.command_ = _LORA4_CMD_SET_CH_PARAMETERS;
    cmdMsg.payLoadSize = 7;
    cmdMsg.payLoad[ 0 ] = index;
    cmdMsg.payLoad[ 1 ] = freq & 0x000000FF;
    cmdMsg.payLoad[ 2 ] = (freq >> 8) & 0x000000FF;
    cmdMsg.payLoad[ 3 ] = (freq >> 16) & 0x000000FF;
    cmdMsg.payLoad[ 4 ] = freq >> 24;
    cmdMsg.payLoad[ 5 ] = minDRR | maxDRR;
    cmdMsg.payLoad[ 6 ] = status;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 1)
        return rspMsg.payLoad[ 0 ];

    return _LORA4_ERR_RESPONSE;
}

LORA4_RETVAL_T
lora4_getChannel( uint8_t index, uint32_t *freq, uint8_t *dataRateRange, uint8_t *status )
{
    uint8_t result;
    T_lora4_message cmdMsg;
    T_lora4_message rspMsg;
    uint32_t temp = 0;

    if (index > 15)
        index = 15;

    cmdMsg.command_ = _LORA4_CMD_GET_CH_PARAMETERS;
    cmdMsg.payLoadSize = 1;
    cmdMsg.payLoad[ 0 ] = index;

    if (result = lora4_command( &cmdMsg, &rspMsg ))
        return result;

    if ((cmdMsg.command_ | RESPONSE_BIT) != rspMsg.command_)
        return _LORA4_ERR_RESPONSE;

    if (rspMsg.payLoadSize == 6)
    {
        temp = rspMsg.payLoad[ 3 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 2 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 1 ];
        temp <<= 8;
        temp |= rspMsg.payLoad[ 0 ];
        *freq = temp;
        *dataRateRange = rspMsg.payLoad[ 4 ];
        *status = rspMsg.payLoad[ 5 ];
        return _LORA4_OK;
    }

    return _LORA4_ERR_RESPONSE;
}

/* -------------------------------------------------------------------------- */
/*
  __lora4_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */