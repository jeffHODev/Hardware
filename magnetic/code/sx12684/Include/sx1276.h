
#ifndef __SX1276_H__
#define __SX1276_H__
#include "bsp.h"
#include "STM8l15x_conf.h"
#include "mytypedef.h"
#include "sx1276-LoRa.h"
#include "sx1276-Fsk.h"
/*
================================================================================
-------------------------IMPORT FUNCTIONS FOR DRIVER----------------------------
================================================================================
*/


/*
================================================================================
--------------------------------MACRO DIFINITIONS-------------------------------
================================================================================
*/
typedef enum
{
    MODE_FSK                =   0x00,       //FSK mode
//   	MODE_OOK                =   0x01,       //OOK mode
   	MODE_LORA               =   0x01,       //LoRa mode
}RANGE_MODE;
#define IS_RANGE_MODE( x )   (((x) == MODE_FSK) || ((x) == MODE_LORA))
void SX1276_Par_Error(uint8_t* file, uint32_t line);
#define AX1276_assert_param(expr) ((expr) ? (void)0 : SX1276_Par_Error((uint8_t *)__FILE__, __LINE__))



/*
================================================================================
-------------------------EXPORT FUNCTIONS OF DRIVER-----------------------------
================================================================================
*/

typedef struct
{
    INT8U               Channel;
    INT8U               PowerIndex;
    RANGE_MODE          Mode_Lora_Fsk;
    INT8U               BitrateIndex;
}T_SX1276;
extern T_SX1276 SX1276;

/*Put the device into Lora CAD mode*/
void SX1276_CADMode( void );

/*Put the device into sleep mode*/
void SX1276_SleepMode( void );

/*Receive a packet*/
INT8U SX1276_ReceivePacket( INT8U *buffer );

/*Send a packet*/
void SX1276_SendPacket( INT8U *buffer, INT8U size );

/*Set the device into RX mode*/
void SX1276_SetRxMode( void );

/*Set the datarate of the device*/
void SX1276_SetDatarate( INT8U rate_index );

/*Initialize the SX1276, Lora or FSK mode*/
void SX1276_Init( RANGE_MODE mode );

/* Write a register of the device */
void SX1276_WriteReg( INT8U addr, INT8U value );

/* Write registers of the device */
void SX1276_WriteBuffer( INT8U addr, INT8U *buffer, INT8U size );

/* Write data into fifo */
void SX1276_WriteFifo( INT8U *buffer, INT8U size );

/* Read a register of the device */
INT8U SX1276_ReadReg( INT8U addr );

/* Read a byte from FIFO */
INT8U SX1276_ReadFifo( void );

/* Put the SX1276 into another mode */
void SX1276_SetMode( RANGE_MODE range_mode );

/* Reset the SX1276 */
void SX1276_Reset( void );

/* Set SwitchTx  on */
void SX1276_SetSwitchTx( void );

/* Set SwitchRx  on */
void SX1276_SetSwitchRx( void );

/* Set the Frequency of SX1276 */
void SX1276_SetFreq( INT8U FreqIndex );

/* Set the output power */
void SX1276_SetPower(INT8U power, INT8U pin);

/* Set the PA ramp time of SX1276 */
void SX1276_SetPARampTime( INT8U rametime );

/*Set the preamble count of the device*/
void SX1276_SetPreambleSize( INT16U preamble_cnt );


#endif //__SX1276_H__