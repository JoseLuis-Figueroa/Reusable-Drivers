/**
 * @file spi_cfg.h
 * @author Jose Luis Figueroa 
 * @brief This module contains interface definitions for the SPI
 * configuration. This is the header file for the definition of the
 * interface for retrieving the Serial Peripheral interface
 * configuration table.
 * @version 1.0
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023 Jose Luis Figueroa. MIT License.
 * 
 */
#ifndef SPI_H_
#define SPI_H_

/**********************************************************************
* Includes
**********************************************************************/

/**********************************************************************
* Preprocessor Constants
**********************************************************************/
/** 
 * Defines the number of ports on the processor.
 */
#define SPI_PORTS_NUMBER 4U

/** 
 * Set the value according with the number of Serial Peripheral 
 * interface channels to be used.
*/
#define SPI_CHANNELS_NUMBER 1

/**********************************************************************
* Typedefs
**********************************************************************/
/**
 * Define the bus mode according to clock polarity and clock phase.
 */
typedef enum
{
    SPI_MODE0,      /**< Mode 0 (CPOL=0 and CPHA=0)*/      
    SPI_MODE1,      /**< Mode 1 (CPOL=0 and CPHA=1)*/ 
    SPI_MODE2,      /**< Mode 2 (CPOL=1 and CPHA=0)*/ 
    SPI_MODE3,      /**< Mode 3 (CPOL=1 and CPHA=1)*/ 
    SPI_MAX_MODE    /**< Maximum mode */
}SpiMode_t;

/**
 * Define the hierarchy of the device. Set the device as slave or 
 * master.
 */
typedef enum
{
    /** TODO: Populate this list based on available MCU hierarchy mode*/
    SPI_SLAVE,          /**< Slave configuration*/
    SPI_MASTER,         /**< Master configuration*/
    SPI_MAX_HIERARCHY   /**< Maximum hierarchy*/
}SpiHierarchy_t;

/**
 * Define the baud rate control.
*/
typedef enum
{
    SPI_FPCLK2,     /**< Baud rate divided by 2*/
    SPI_FPCLK4,     /**< Baud rate divided by 4*/
    SPI_FPCLK8,     /**< Baud rate divided by 8*/
    SPI_FPCLK16,    /**< Baud rate divided by 16*/
    SPI_FPCLK32,    /**< Baud rate divided by 32*/
    SPI_FPCLK128,   /**< Baud rate divided by 128*/
    SPI_FPCLK256,   /**< Baud rate divided by 256*/
    SPI_MAX_FPCLK   /**< Maximum baud rate*/
}SpiBaudRate_t;

/**
 * Define the frame format. Choose the direction of the data frame 
 * to be sent.
 */
typedef enum
{
    SPI_MSB,    /**< Most significant bit transmitted first*/
    SPI_LSB,    /**< Less significant bit transmitted first*/
    SPI_MAX_FF  /**< Maximum frame format*/
}SpiFrameFormat_t;

/**
 * Define the type of data transfer.  
 */
typedef enum
{
    SPI_FULL_DUPLEX,    /**< Set full duplex communication*/
    SPI_RECEIVE_MODE,   /**< Set Receive only communication*/
    SPI_MAX_DF          /**< Set the maximum data transfer*/
}SpiTypeTransfer_t;

/**
 * Define the data frame format. Set bits number of data.
 */
typedef enum
{
    SPI_8BITS,  /**< 8 bits data is selected for communication*/
    SPI_16BITS  /**< 16 bits data is selected for communication*/
}SpiDataSize_t;

/**
 * Defines the Serial Peripheral Interface configuration table's 
 * elements that are used by Spi_Init to configure the SPI peripheral.
 */
typedef struct
{
    SpiMode_t Mode;
    SpiHierarchy_t Hierarchy;
    SpiBaudRate_t BaudRate;
    SpiFrameFormat_t FrameFormat;
    SpiTypeTransfer_t TypeTransfer;
    SpiDataSize_t DataSize;
}SpiConfig_t;


/**********************************************************************
* Function Prototypes
**********************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

const SpiConfig_t * const SPI_ConfigGet(void);

#ifdef __cplusplus
} //extern "C"
#endif

#endif  /*SPI_H_*/
