/**
 * @file dio.c
 * @author Jose Luis Figueroa
 * @brief The implementation for the dio
 * @version 1.0
 * @date 2023-03-19
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. All rights reserved.
 * 
 */
/*****************************************************************************
* Includes
*****************************************************************************/
#include "dio.h"        /*For this modules definitions*/
#include "stm32f4xx.h"  /*Microcontroller family header*/                                          

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
/** Defines a array of pointers to the GPIO port mode register */
static uint32_t volatile * const moderRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->MODER, (uint32_t*)&GPIOB->MODER, 
    (uint32_t*)&GPIOC->MODER, (uint32_t*)&GPIOD->MODER,
    (uint32_t*)&GPIOH->MODER
};

/** Defines a array of pointers to the GPIO port output type register. */
static uint32_t volatile * const otyperRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->OTYPER, (uint32_t*)&GPIOB->OTYPER,
    (uint32_t*)&GPIOC->OTYPER, (uint32_t*)&GPIOD->OTYPER, 
    (uint32_t*)&GPIOH->OTYPER
};

/** Define a array of pointers to the GPIO port output speed register. */
static uint32_t volatile * const ospeedrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->OSPEEDR, (uint32_t*)&GPIOB->OSPEEDR,
    (uint32_t*)&GPIOC->OSPEEDR, (uint32_t*)&GPIOD->OSPEEDR, 
    (uint32_t*)&GPIOH->OSPEEDR
};

/** Defines a array of pointers to the GPIO port pull-up/pull-down register.*/
static uint32_t volatile * const pupdrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->PUPDR, (uint32_t*)&GPIOB->PUPDR,
    (uint32_t*)&GPIOC->PUPDR, (uint32_t*)&GPIOD->PUPDR, 
    (uint32_t*)&GPIOH->PUPDR
};

/**
 * Defines a array of pointers to the GPIO port input data register.
*/
static uint32_t volatile * const idrRegister[NUMBER_OF_PORTS] =  
{
    (uint32_t*)&GPIOA->IDR, (uint32_t*)&GPIOB->IDR, (uint32_t*)&GPIOC->IDR,
    (uint32_t*)&GPIOD->IDR, (uint32_t*)&GPIOH->IDR
};

/** Defines a array of pointers to the GPIO port output data register. */
static uint32_t volatile * const odrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->ODR, (uint32_t*)&GPIOB->ODR, (uint32_t*)&GPIOC->ODR, 
    (uint32_t*)&GPIOD->ODR, (uint32_t*)&GPIOH->ODR
};

/** Defines a array of pointers to the GPIO alternate function low register.
 * This is compound for two 32 bits registers.
*/
static uint32_t volatile * const afrRegister[NUMBER_OF_PORTS] =
{
    (uint32_t*)&GPIOA->AFR[0], (uint32_t*)&GPIOB->AFR[0], 
    (uint32_t*)&GPIOC->AFR[0], (uint32_t*)&GPIOD->AFR[0], 
    (uint32_t*)&GPIOH->AFR[0]
};

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/*****************************************************************************
 * Function: DIO_init()
*//**
*\b Description:
 * This function is used to initialize the Dio based on the configuration  
 * table defined in dio_cfg module.
 * 
 * PRE-CONDITION: Configuration table needs to be populated (sizeof > 0) <br>
 * PRE-CONDITION: NUMBER_OF_PORTS > 0 <br>
 * PRE-CONDITION: The MCU clocks must be configured and enabled.
 * 
 * POST-CONDITION: The DIO peripheral is set up with the configuration 
 * settings.
 * 
 * @param   Config is a pointer to the configuration table that contains the 
 *                 initialization for the peripheral.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  const DioConfig_t * const DioConfig = DIO_configGet();
 *  DIO_init(DioConfig);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 * 
*****************************************************************************/
void DIO_init(const DioConfig_t * Config)
{
    /** Loop through all the elements of the configuration table. */
    for(uint8_t i=0; i<=sizeof(Config); i++)
    {
        /** 
         * Set the mode of the Dio pin on the GPIO port mode register. 
         * Multiply the pin number (Config[i].Pin) by two as MODER uses two 
         * bits to configure the one pin.
        */
        if(Config[i].Mode == DIO_INPUT)
        {
            *moderRegister[Config[i].Port] &=~ (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] &=~ (2UL<<(Config[i].Pin*2));

        }
        else if (Config[i].Mode == DIO_OUTPUT)
        {
            *moderRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] &=~ (2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Mode == DIO_FUNCTION)
        {
            *moderRegister[Config[i].Port] &=~ (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Mode == DIO_ANALOG)
        {
            *moderRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *moderRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else
        {
            printf("This Mode does not exist\n");
        }

        /**
         * Set the output type of the Dio pin on the GPIO port output type 
         * register.
         */
        if(Config[i].Type == DIO_PUSH_PULL)
        {
            *otyperRegister[Config[i].Port] &= ~(1UL<<Config[i].Pin);
        }
        else if (Config[i].Type == DIO_OPEN_DRAIN)
        {
            *otyperRegister[Config[i].Port] |= (1UL<<Config[i].Pin);
        }
        else
        {
            printf("This output type does not exist\n");
        }

        /**
         * Set the speed of the Dio pin on the GPIO port output speed register. 
         */
        if(Config[i].Speed == DIO_LOW_SPEED)
        {
            *ospeedrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Speed == DIO_MEDIUM_SPEED)
        {
            *ospeedrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
        }
        else if (Config[i].Speed == DIO_HIGH_SPEED)
        {
            *ospeedrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else if(Config[i].Speed == DIO_VERY_SPEED)
        {
            *ospeedrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *ospeedrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
        }
        else
        {
            printf("The output speed does not exist\n");
        }

        /** 
         * Set the internal resistor of the Dio pin on the GPIO port 
         * pull-up/pull-down register
        */
       if(Config[i].Resistor == DIO_NO_RESISTOR)
       {
            *pupdrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *pupdrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
       }
       else if (Config[i].Resistor == DIO_PULLUP)
       {
            *pupdrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*2));
            *pupdrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*2));
       }
       else if (Config[i].Resistor == DIO_PULLDOWN)
       {
            *pupdrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*2));
            *pupdrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*2));
       }
       else
       {
            printf("The port register does not exist");
       }

        /** Set the alternate function of the Dio pin on the GPIO alternate 
         * function
        */
       if(Config[i].Function == DIO_AF0)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF1)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF2)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF3)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF4)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF5)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF6)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF7)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF8)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF9)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF10)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF11)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF12)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF13)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] &= ~(2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF14)
       {
            *afrRegister[Config[i].Port] &= ~(1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       }
       else if(Config[i].Function == DIO_AF15)
       {
            *afrRegister[Config[i].Port] |= (1UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (2UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (4UL<<(Config[i].Pin*4));
            *afrRegister[Config[i].Port] |= (8UL<<(Config[i].Pin*4));
       } 

    }
}

/*****************************************************************************
 * Function: DIO_pinRead()
*//**
 *\b Description:
 * This function is used to read the state of a dio pin.
 * 
 * PRE-CONDITION: The pin is configured as INPUT <br>
 * PRE-CONDITION: The pin is configured as GPIO <br>
 * PRE-CONDITION: The Port is within the maximum DioPort_t.
 * PRE-CONDITION: The Pin is within the maximum DioPin_t. 
 * definition.
 * 
 * POST-CONDITION: The channel state is returned.
 * 
 * @param   Port is the DioPort_t that represents a port.
 * @param   Pin is the DioPin_t that represents a pin.
 * @return  The state of the channel as HIGH or LOW.
 * 
 * \b Example:
 * @code
 *  uint8_t pin = DIO_pinRead(DIO_PC, DIO_PC5);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 * 
**********************************************************************/
DioPinState_t DIO_pinRead(DioPort_t Port, DioPin_t Pin)
{
    /** Read the port associated with the desired pin */
    DioPinState_t portState = (DioPinState_t)*idrRegister[Port];
    /** Determinate the Port bit associated with this pin*/
    DioPinState_t pinMask = (DioPinState_t)(1UL<<(Pin));

    return ((portState & pinMask) ? DIO_HIGH : DIO_LOW); 
}

/**********************************************************************
 * Function: DIO_pinWrite()
*//**
 *\b Description:
 * This function is used to write the state of a pin as either logic 
 * high or low through the use of the DioChannel_t enum to select the 
 * channel and the DioPinState_t to define the desired state.
 * 
 * PRE-CONDITION: The pin is configured as OUTPUT <br>
 * PRE-CONDITION: The pin is configured as GPIO <br>
 * PRE-CONDITION: The pin is within the maximum DioChannel_t .
 * definition.
 * 
 * POST-CONDITION: The channel state will be Stated.
 * 
 * @param   Port is the GPIO to write using the DioPort_t enum.
 * @param   Pin is the bit to write using the DioPin_t enum definition.
 * @param   State is HIGH or LOW as defined in the DioPinState_t enum. 
 *          
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  DIO_pinWrite(DIO_PA, DIO_PA1, LOW);  //Set the PORT pin low
 *  DIO_pinWrite(DIO_PB, DIO_PB3, HIGH); //Set the PORT pin high
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 * 
 **********************************************************************/
void DIO_pinWrite(DioPort_t Port, DioPin_t Pin, DioPinState_t State)
{
    if(State == DIO_HIGH)
    {
        *odrRegister[Port] |= (1UL<<(Pin));
    }
    else if (State == DIO_LOW)
    {
        *odrRegister[Port] &= ~(1UL<<Pin);
    }
    else
    {
        printf("This option does not exist");
    }
}

/**********************************************************************
 * Function: DIO_pinToggle()
*//**
 *\b Description:
 * This function is used to toggle the current state of a pin.
 * 
 * PRE-CONDITION: The channel is configured as output <br>
 * PRE-CONDITION: The channel is configured as GPIO <br>
 * PRE-CONDITION: The channel is within the maximum DioChannel_t 
 * definition.
 *
 * POST-CONDITION:
 * 
 * @param   Port is the GPIO to write using the DioPort_t enum.
 * @param   Pin is the bit from the DioPin_t that is to be modified.
 * 
 * @return  void
 * 
 * \b Example:
 * @code
 *  DIO_pinToggle(DIO_PA, DIO_PA3);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_pinRead
 * @see DIO_pinWrite
 * @see DIO_pinToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 * 
 * <br><b> - HISTORY OF CHANGES - </b>
 * 
 **********************************************************************/
void DIO_pinToggle(DioPort_t Port, DioPin_t Pin)
{
    *odrRegister[Port] ^= (1UL<<Pin);
}

/**********************************************************************
 * Function: DIO_registerWrite()
*//**
 *\b Description:
 * This function is used to directly address and modify a Dio register.
 * The function should be used to access specialized functionality in 
 * the Dio peripheral that is not exposed by any other function of the
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the Dio register
 * address space.
 * 
 * POST-CONDITION: The register located at address with be updated with
 * value.
 * 
 * @param   address is a register address within the Dio peripheral map.
 * @param   value is the value to set the Dio register. 
 * 
 * @return void
 * 
 * \b Example
 * @code
 *  DIO_registerWrite(0x1000, 0x15);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_channelRead
 * @see DIO_channelWrite
 * @see DIO_channelToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 * 
**********************************************************************/ 
void DIO_registerWrite(uint32_t address, uint32_t value)
{
    volatile uint32_t * const registerPointer = (uint32_t*)address;
    *registerPointer = value;
}

/**********************************************************************
 * Function: DIO_registerRead()
*//**
 *\b Description:
 * This function is used to directly address a Dio register. The 
 * function should be used to access specialized functionality in the 
 * Dio peripheral that is not exposed by any other function of the 
 * interface.
 * 
 * PRE-CONDITION: Address is within the boundaries of the Dio register 
 * address space.
 * 
 * POST-CONDITION: The value stored in the register is returned to the 
 * caller.
 * 
 * @param   address is the address of the Dio register to read.
 * 
 * @return  The current value of the Dio register.
 * 
 * \b Example:
 * @code
 *  dioValue = DIO_registerRead(0x1000);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_channelRead
 * @see DIO_channelWrite
 * @see DIO_channelToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 *
 **********************************************************************/ 
uint32_t DIO_registerRead(uint32_t address)
{
    volatile uint32_t * const registerPointer = (uint32_t*)address;

    return *registerPointer;
}

/**********************************************************************
 * Function: DIO_callbackRegister()
*//**
 *\b Description:
 * This function is used to set the callback function of the dio driver.
 * By default, the callbacks are initialized to a NULL pointer. The 
 * driver may contain more than one possible callback, so the function
 * will take a parameter to configure the specified callback.
 * 
 * PRE-CONDITION: The DioCallback_t has been populated.
 * PRE-CONDITION: The callback function exists within memory.
 * 
 * POST-CONDITION: The specified callback function will be registered 
 * with the driver.
 * 
 * @param   function is the callback function that will be registered.
 * @param   callbackFunction is a function pointer to the desired 
 * function.
 * 
 * @return None
 * 
 * \b Example:
 * @code
 *  DioCallback_t Dio_Function = DIO_SAMPLE_COMPLETE;
 *  DIO_callbackRegister(Dio_Function, Dio_SampleAverage);
 * @endcode
 * 
 * @see DIO_init
 * @see DIO_channelRead
 * @see DIO_channelWrite
 * @see DIO_channelToggle
 * @see DIO_registerWrite
 * @see DIO_registerRead
 * @see DIO_callbackRegister
 * 
 **********************************************************************/ 
/*void DIO_CallbackRegister(DioCallback_t Function, TYPE (*CallbackFunction)(type))
{
    
}*/