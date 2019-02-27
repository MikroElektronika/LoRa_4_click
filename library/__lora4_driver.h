/*
    __lora4_driver.h

-----------------------------------------------------------------------------

  This file is part of mikroSDK.
  
  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

/**
@file   __lora4_driver.h
@brief    LoRa_4 Driver
@mainpage LoRa_4 Click
@{

@image html libstock_fb_view.jpg

@}

@defgroup   LORA4
@brief      LoRa_4 Click Driver
@{

| Global Library Prefix | **LORA4** |
|:---------------------:|:-----------------:|
| Version               | **1.0.0**    |
| Date                  | **Feb 2018.**      |
| Developer             | **Nemanja Medakovic**     |

*/
/* -------------------------------------------------------------------------- */

#include "stdint.h"

#ifndef _LORA4_H_
#define _LORA4_H_

/** 
 * @macro T_LORA4_P
 * @brief Driver Abstract type 
 */
#define T_LORA4_P    const uint8_t*
#define LORA4_RETVAL_T      uint8_t

/** @defgroup LORA4_COMPILE Compilation Config */              /** @{ */

//  #define   __LORA4_DRV_SPI__                            /**<     @macro __LORA4_DRV_SPI__  @brief SPI driver selector */
//  #define   __LORA4_DRV_I2C__                            /**<     @macro __LORA4_DRV_I2C__  @brief I2C driver selector */                                          
  #define   __LORA4_DRV_UART__                           /**<     @macro __LORA4_DRV_UART__ @brief UART driver selector */ 

                                                                       /** @} */
/** @defgroup LORA4_VAR Variables */                           /** @{ */

extern const uint8_t _LORA4_CMD_RESET;                      /**< Module Software Reset */
extern const uint8_t _LORA4_CMD_FACTORY_RESET;              /**< Restore EEPROM to factory defaults */
extern const uint8_t _LORA4_CMD_EEPROM_WRITE;               /**< Write EEPROM */
extern const uint8_t _LORA4_CMD_EEPROM_READ;                /**< Read EEPROM */
extern const uint8_t _LORA4_CMD_GET_FW_VERSION;             /**< Get FW version */
extern const uint8_t _LORA4_CMD_GET_SERIAL_NO;              /**< Get Serial Number */
extern const uint8_t _LORA4_CMD_GET_DEV_EUI;                /**< Get Factory stored device EUI */
extern const uint8_t _LORA4_CMD_JOIN;                       /**< Network join command */
extern const uint8_t _LORA4_IND_JOIN;                       /**< Join result Indication */
extern const uint8_t _LORA4_CMD_GET_ACTIVATION_STATUS;      /**< Get activation status */
extern const uint8_t _LORA4_CMD_SET_APP_KEY;                /**< Set application key */
extern const uint8_t _LORA4_CMD_SET_APP_SESSION_KEY;        /**< Set application application session key */
extern const uint8_t _LORA4_CMD_SET_NWK_SESSION_KEY;        /**< Set application network session key */
extern const uint8_t _LORA4_CMD_TX_MSG;                     /**< Transmission of LoRa radio message */
extern const uint8_t _LORA4_IND_TX_MSG_CONFIRMED;           /**< Indication of LoRa radio confirmed message */
extern const uint8_t _LORA4_IND_TX_MSG_UNCONFIRMED;         /**< Indication of LoRa radio unconfirmed message */
extern const uint8_t _LORA4_IND_RX_MSG;                     /**< Indicate reception of LoRa radio message */
extern const uint8_t _LORA4_CMD_GET_SESSION_STATUS;         /**< Get session status */
extern const uint8_t _LORA4_CMD_SET_NEXT_DR;                /**< Set next datarate command */
extern const uint8_t _LORA4_CMD_SET_BATTERY_LVL;            /**< Set battery level */
extern const uint8_t _LORA4_CMD_GET_BATTERY_LVL;            /**< Get battery level */
extern const uint8_t _LORA4_CMD_SET_UPLINK_CNT;             /**< Set uplink counter */
extern const uint8_t _LORA4_CMD_GET_UPLINK_CNT;             /**< Get uplink counter */
extern const uint8_t _LORA4_CMD_SET_DOWNLINK_CNT;           /**< Set downlink counter */
extern const uint8_t _LORA4_CMD_GET_DOWNLINK_CNT;           /**< Get downlink counter */
extern const uint8_t _LORA4_CMD_SET_CH_PARAMETERS;          /**< Set channel parameters */
extern const uint8_t _LORA4_CMD_GET_CH_PARAMETERS;          /**< Get channel parameters */

extern const uint8_t _LORA4_JOIN_ACTIVATION_BY_PERSONALIZATION_MODE;
extern const uint8_t _LORA4_JOIN_OVER_THE_AIR_ACTIVATION_MODE;
extern const uint8_t _LORA4_GET_ACTIVATION_MODE;
extern const uint8_t _LORA4_GET_SESSION_STATUS_MODE;
extern const uint8_t _LORA4_SET_APP_KEY_MODE;
extern const uint8_t _LORA4_SET_NWK_SESSION_KEY_MODE;
extern const uint8_t _LORA4_UPLINK_CNT;
extern const uint8_t _LORA4_DOWNLINK_CNT;
extern const uint8_t _LORA4_CHANNEL_DISABLED;
extern const uint8_t _LORA4_CHANNEL_ENABLED;
extern const uint8_t _LORA4_UNCONFIRMED_TRANSMISSION;
extern const uint8_t _LORA4_CONFIRMED_TRANSMISSION;

extern const uint8_t _LORA4_OK;
extern const uint8_t _LORA4_ERR_ADDR_RANGE;
extern const uint8_t _LORA4_ERR_NOT_IDLE;

extern const uint8_t _LORA4_ERR_PL_LEN;
extern const uint8_t _LORA4_ERR_CHKSUM;
extern const uint8_t _LORA4_ERR_RESPONSE;
extern const uint8_t _LORA4_ERR_HEADER;

                                                                       /** @} */
/** @defgroup LORA4_TYPES Types */                             /** @{ */

typedef void ( *T_lora4_indHdl )( uint8_t *cmd, uint8_t* pLoadSize, uint8_t *pLoad );

typedef struct {

    uint8_t command_;
    uint8_t payLoadSize;
    uint8_t payLoad[ 255 ];

}T_lora4_message;

                                                                       /** @} */
#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup LORA4_INIT Driver Initialization */              /** @{ */

#ifdef   __LORA4_DRV_SPI__
void lora4_spiDriverInit(T_LORA4_P gpioObj, T_LORA4_P spiObj);
#endif
#ifdef   __LORA4_DRV_I2C__
void lora4_i2cDriverInit(T_LORA4_P gpioObj, T_LORA4_P i2cObj, uint8_t slave);
#endif
#ifdef   __LORA4_DRV_UART__
void lora4_uartDriverInit(T_LORA4_P gpioObj, T_LORA4_P uartObj);
#endif

                                                                       /** @} */
/** @defgroup LORA4_FUNC Driver Functions */                   /** @{ */

/**
 * @brief Hardware reset function
 *
 * Resets device by setting pin and after that puts device back in normal operation mode.
 */
void lora4_hardReset();

/**
 * @brief Wake Up function
 *
 * @param[in] state       0 - wake up, 1 - shutdown
 *
 * Puts device in shutdown mode or wake up device depending on state.
 */
void lora4_wakeUp( uint8_t state );

/**
 * @brief Set indication handler function
 *
 * @param[in] hdl        Pointer to void handler function
 *
 * Function reads indications (response) to determined command.
 */
void lora4_setIndHandler( T_lora4_indHdl hdl );

/**
 * @brief Command function
 *
 * @param[in] msg         Pointer to command witch be sent
 * @param[out] rsp        Pointer to output where response be placed
 *
 * @return 0 - OK, 1 - Check Sum Error
 *
 * Function executes commands and reads response on executed commands. Also checks CRC error report.
 */
uint8_t lora4_command( T_lora4_message *msg, T_lora4_message *rsp );

/**
 * @brief Process function
 *
 * Function executes reading indications and forwards indications to handler.
 */
void lora4_process();

/**
 * @brief Software reset function
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function resets module by sending command.
 */
LORA4_RETVAL_T lora4_softReset( void );

/**
 * @brief Factory reset function
 *
 * @return 0 - OK, 2 - LoRa Mac not in idle state, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function performs the recovery of EEPROM default values.
 */
LORA4_RETVAL_T lora4_factoryReset( void );

/**
 * @brief EEPROM write function
 *
 * @param[in] address  EEPROM Address where data be written
 * @param[in] nBytes   Number of bytes witch be written (max 254)
 * @param[in] dataIn   Pointer to buffer witch from data be written
 *
 * @return 0 - OK, 2 - LoRa Mac not in idle state, 252 - Payload Length Error, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function writes data to EEPROM.
 */
LORA4_RETVAL_T lora4_writeEeprom( uint8_t address, uint8_t nBytes, uint8_t *dataIn );

/**
 * @brief EEPROM read function
 *
 * @param[in] address         EEPROM Address witch from data be read
 * @param[in] nBytes          Number of bytes witch be read (max 254)
 * @param[out] dataOut        Pointer to buffer where data be stored
 *
 * @return 0 - OK, 252 - Payload Length Error, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error or Data is empty
 *
 * Function reads data from EEPROM.
 */
LORA4_RETVAL_T lora4_readEeprom( uint8_t address, uint8_t nBytes, uint8_t *dataOut );

/**
 * @brief Get FW Version function
 *
 * @param[out] fwVersion         Pointer to buffer where FW Version be stored
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function gets 32-bit FW version.
 */
LORA4_RETVAL_T lora4_getFwVersion( uint32_t *fwVersion );

/**
 * @brief Get Serial Number function
 *
 * @param[out] serialNo     Pointer to buffer where Serial Number be stored
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function gets 32-bit Serial Number.
 */
LORA4_RETVAL_T lora4_getSerialNo( uint32_t *serialNo );

/**
 * @brief Get DevEUI function
 *
 * @param[out] devEUI        Pointer to buffer where devEUI data be stored
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function gets DevEUI provided by Mipot.
 */
LORA4_RETVAL_T lora4_getDevEUI( uint8_t *devEUI );

/**
 * @brief Join Network function
 *
 * @param[in] mode     0 - Activation by personalization, 1 - Over the air activation
 *
 * @return 0 - OK, 2 - Busy, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function performs the join network command.
 */
LORA4_RETVAL_T lora4_joinNetwork( uint8_t mode );

/**
 * @brief Get Status function
 *
 * @param[in] statusMode     0 - Get Activation Status, 1 - Get Session Status
 *
 * @return 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *         0 - Not activated, 1 - Joining, 2 - Joined, 3 - MAC ERROR; when statusMode is 0
 *         0 - Idle, 1 - Busy, 2 - Device not activated, 3 - Delayed; when statusMode is 1
 *
 * Function gets the module activation and session status.
 */
LORA4_RETVAL_T lora4_getStatus( uint8_t statusMode );

/**
 * @brief Set Key function
 *
 * @param[in] keyData     Pointer to buffer witch from key data be written
 * @param[in] keyMode     0 - Set App Key, 1 - Set App Session Key, 2 - Set NWK Session Key
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function sets 16 byte key in Little Endian Order. App key is needed for OTAA procedure, and App Session key and NWK Session key
 * is needed for APB procedure.
 */
LORA4_RETVAL_T lora4_setKey( uint8_t *keyData, uint8_t keyMode );

/**
 * @brief Radio Frame Transmission function
 *
 * @param[in] dataIn      Pointer to buffer witch from data be sent
 * @param[in] nBytes      Number of bytes witch be sent (max 253)
 * @param[in] port        From 1 to 223
 * @param[in] option      0bxxxxxxx0 - Unreliable Data Transmission, 0bxxxxxxx1 - Reliable Data Transmission
 *
 * @return 0 - OK, 252 - Payload Length Error, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error, 1 - Device busy
 *         2 - Device not activated, 3 - Channel blocked by duty-cycle, 5 - Length not supported, 6 - End Node in silent state, 7 - Error
 *
 * Function performs the trasnmission of radio frame.
 */
LORA4_RETVAL_T lora4_txMessage( uint8_t *dataIn, uint8_t nBytes, uint8_t port, uint8_t option );

/**
 * @brief Set Next Data Rate function
 *
 * @param[in] dataRate       0 - SF12/125kHz, 1 - SF11/125kHz, 2 - SF10/125kHz, 3 - SF9/125kHz
 *                           4 - SF8/125kHz, 5 - SF7/125kHz, 6 - SF7/250kHz, 7 - FSK
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *         A value different from zero means that an error has occurred
 *
 * Function sets next transmission Data Rate.
 */
LORA4_RETVAL_T lora4_setNextTXDR( uint8_t dataRate );

/**
 * @brief Set Battery Level function
 *
 * @param[in] batteryLevel  0 - The end-device is connected to an external power source
 *                          The battery level - 1 being at minimum and 254 being at maximum
 *                          255 - The end-device was not able to measure battery level
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function sets Battery Level.
 */
LORA4_RETVAL_T lora4_setBatteryLevel( uint8_t batteryLevel );

/**
 * @brief Get Battery Level function
 *
 * @return 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *         0 - The end-device is connected to an external power source
 *         The battery level - 1 being at minimum and 254 being at maximum
 *         255 - The end-device was not able to measure battery level
 *
 * Function gets Battery Level.
 */
LORA4_RETVAL_T lora4_getBatteryLevel( void );

/**
 * @brief Set Counter function
 *
 * @param[in] cnt             32-bit counter value
 * @param[in] cntMode         0 - Set Uplink counter, 1 - Set Downlink counter
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function sets Uplink or Downlink counter.
 */
LORA4_RETVAL_T lora4_setCounter( uint32_t cnt, uint8_t cntMode );

/**
 * @brief Get Counter function
 *
 * @param[out] cnt             Pointer to buffer where counter value be stored
 * @param[in] cntMode          0 - Get Uplink counter, 1 - Get Downlink counter
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function gets Uplink or Downlink counter.
 */
LORA4_RETVAL_T lora4_getCounter( uint32_t *cnt, uint8_t cntMode );

/**
 * @brief Set Channel function
 *
 * @param[in] index           From 3 to 15
 * @param[in] freq            32-bit frequency value
 * @param[in] dataRateRange   Data Rate Max is most significant nibble (from 0 to 7)
 *                            Data Rate Min is less significant nibble (from 0 to 7)
 * @param[in] status          0 - Disabled, 1 - Enabled
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *         242 - Frequency out of range, 244 - MAC busy
 *
 * Function sets channel parameters and enable/disable optional channels.
 */
LORA4_RETVAL_T lora4_setChannel( uint8_t index, uint32_t freq, uint8_t dataRateRange, uint8_t status );

/**
 * @brief Get Channel function
 *
 * @param[in] index           From 0 to 15
 * @param[out] freq           Pointer to buffer where 32-bit frequency value be placed
 * @param[out] dataRateRange  Data Rate Max is most significant nibble (from 0 to 7)
 *                            Data Rate Min is less significant nibble (from 0 to 7)
 * @param[out] status         0 - Disabled, 1 - Enabled
 *
 * @return 0 - OK, 253 - Check Sum Error, 254 - Response Error, 255 - Header Error
 *
 * Function gets channel parameters.
 */
LORA4_RETVAL_T lora4_getChannel( uint8_t index, uint32_t *freq, uint8_t *dataRateRange, uint8_t *status );

                                                                       /** @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif

/**
    @example Click_LoRa_4_STM.c
    @example Click_LoRa_4_TIVA.c
    @example Click_LoRa_4_CEC.c
    @example Click_LoRa_4_KINETIS.c
    @example Click_LoRa_4_MSP.c
    @example Click_LoRa_4_PIC.c
    @example Click_LoRa_4_PIC32.c
    @example Click_LoRa_4_DSPIC.c
    @example Click_LoRa_4_AVR.c
    @example Click_LoRa_4_FT90x.c
    @example Click_LoRa_4_STM.mbas
    @example Click_LoRa_4_TIVA.mbas
    @example Click_LoRa_4_CEC.mbas
    @example Click_LoRa_4_KINETIS.mbas
    @example Click_LoRa_4_MSP.mbas
    @example Click_LoRa_4_PIC.mbas
    @example Click_LoRa_4_PIC32.mbas
    @example Click_LoRa_4_DSPIC.mbas
    @example Click_LoRa_4_AVR.mbas
    @example Click_LoRa_4_FT90x.mbas
    @example Click_LoRa_4_STM.mpas
    @example Click_LoRa_4_TIVA.mpas
    @example Click_LoRa_4_CEC.mpas
    @example Click_LoRa_4_KINETIS.mpas
    @example Click_LoRa_4_MSP.mpas
    @example Click_LoRa_4_PIC.mpas
    @example Click_LoRa_4_PIC32.mpas
    @example Click_LoRa_4_DSPIC.mpas
    @example Click_LoRa_4_AVR.mpas
    @example Click_LoRa_4_FT90x.mpas
*/                                                                     /** @} */
/* -------------------------------------------------------------------------- */
/*
  __lora4_driver.h

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