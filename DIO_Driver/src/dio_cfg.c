/**
 * @file dio_cfg.c
 * @author Jose Luis Figueroa (you@domain.com)
 * @brief This module contains the implementation for the digital 
 * input/output peripheral configuration.
 * @version 1.0
 * @date 2023-03-17
 * \mytag compiler ARM GCC
 * \mytag target Nucleo F401RE
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. All rights reserved.
 * 
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "dio_cfg.h"

/*****************************************************************************
* Module Preprocessor Constants
*****************************************************************************/

/*****************************************************************************
* Module Preprocessor Macros
*****************************************************************************/

/*****************************************************************************
* Module Typedefs
*****************************************************************************/

/*****************************************************************************
* Module Variable Definitions
*****************************************************************************/
/**
 * The following array contains the configuration data for each digital
 * input/output peripheral channel (pin). Each row represent a single pin.
 * Each column is representing a member of the DioConfig_t structure. This 
 * table is read in by Dio_Init, where each channel is then set up based on 
 * this table.
*/
const DioConfig_t DioConfig[] = 
{
/*              Resistor                                        Initial
* Channel       Enabled             Direction       Pin         Function
*/
   {PA0,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA1,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA2,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA3,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA4,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA5,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA6,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
   {PA7,    DIO_PULLUP_DISABLED,    DIO_OUTPUT,    DIO_HIGH,     /*TBD*/},
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: DIO_ConfigGet()
*/
/**
*\b Description:
 * This function is used to initialize the DIO based on the configuration
 * table defined in dio_cfg module.
 * 
 * PRE-CONDITION: configuration table needs to be populated (sizeof > 0)
 * POST-CONDITION: A constant pointer to the first member of the  
 * configuration table will be returned.
 * @return A pointer to the configuration table.
 * 
 * \b Example: 
 * @code
 * const Dio_ConfigType_t * const DioConfig = DIO_GetConfig();
 * 
 * DIO_Init(DioConfig);
 * @endcode
 * 
 * @see DIO_Init
 * @see DIO_ChannelRead
 * @see DIO_ChannelWrite
 * @see DIO_ChannelToggle
 * @see DIO_RegisterWrite
 * @see DIO_RegisterRead
 * 
*****************************************************************************/
const DioConfig_t * const DIO_ConfigGet(void)
{
   /* The cast is performed to ensure that the address of the first element 
    * of configuration table is returned as a constant pointer and not a
    * pointer that can be modified
   */
  return (const *)&DioConfig[0];

}