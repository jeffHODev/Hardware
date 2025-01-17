/**
  ******************************************************************************
  * @file    BMP388.cpp
  * @author  Waveshare Team
  * @version V1.0
  * @date    Dec-2018
  * @brief   T
  
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 Waveshare</center></h2>
  ******************************************************************************
  */
#include "BMP3882.h"
extern I2C_HandleTypeDef hi2c1;
static BMP388_ST_CALI gsstCali;
bmp3_data	   comp_data;

#ifdef __cplusplus
extern "C" {
#endif


 void bmp388DataGet(void);
static void bmp388GetCalibrationData(void);
static int64_t bmp388CompensateTemp(uint32_t u32RegData);
static int64_t bmp388CompensatePress(uint32_t u32RegData);
int32_t * bmp388CalAvgValue(uint8_t *pu8Index, int32_t *ps32AvgBuffer, int32_t s32InVal);

/******************************************************************************
 * interface driver                                                           *
 ******************************************************************************/
void i2cInit(void)
{
  return;
}

uint8_t I2C_ReadOneByte(uint8_t DevAddr, uint8_t RegAddr)
{
  uint8_t u8Ret;
  HAL_I2C_Mem_Read(&hi2c1, DevAddr, RegAddr, I2C_MEMADD_SIZE_8BIT, &u8Ret, 1, 0x10);
  return u8Ret;
}

void I2C_WriteOneByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t value)
{
  HAL_I2C_Mem_Write(&hi2c1, DevAddr, RegAddr, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x10);
  return;
}
/******************************************************************************
 * pressure sensor module                                                     *
 ******************************************************************************/
void delay(uint32_t u32Count)
{
	uint32_t i;
	while(u32Count --) for(i = 0; i < 5000; i ++);

}
 bmp3_data  *GetPressure(void)
{
	return &comp_data; 

}




void BMP388_Get_PaT()
{
  bmp388DataGet( );
  return;
}

//BMP388��ʼ��
u8 BMP388_Init()
{

  	if(I2C_ReadOneByte(I2C_ADD_BMP388,0x00) !=0x50)
	{
		return 1;
	}
												//��ȡʧ�ܷ���1
	uint8_t u8RegData;
	uint8_t u8Ret;
	//reset 
	u8RegData = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_STATUS);
	if( u8RegData & BMP388_REG_VAL_CMD_RDY )
	{
	  I2C_WriteOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_CMD, BMP388_REG_VAL_SOFT_RESET);
	  delay(5);
	  u8RegData = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_ERR);
	  if( u8RegData & BMP388_REG_VAL_CMD_ERR)
	  {
		  u8Ret =  0;
		  return u8Ret;
	  }
	}
	//calibration data read
	bmp388GetCalibrationData();
	//seting
	I2C_WriteOneByte( I2C_ADD_BMP388, BMP388_REG_ADD_PWR_CTRL, 
					  BMP388_REG_VAL_PRESS_EN | BMP388_REG_VAL_TEMP_EN | BMP388_REG_VAL_NORMAL_MODE);
	return 0;

}

/*static uint8_t bmp388Init(void)
{
  uint8_t u8RegData;
  uint8_t u8Ret;
  //reset 
  u8RegData = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_STATUS);
  if( u8RegData & BMP388_REG_VAL_CMD_RDY )
  {
    I2C_WriteOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_CMD, BMP388_REG_VAL_SOFT_RESET);
    delay(5);
    u8RegData = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_ERR);
    if( u8RegData & BMP388_REG_VAL_CMD_ERR)
    {
        u8Ret =  0;
        return u8Ret;
    }
  }
  //calibration data read
  bmp388GetCalibrationData();
  //seting
  I2C_WriteOneByte( I2C_ADD_BMP388, BMP388_REG_ADD_PWR_CTRL, 
                    BMP388_REG_VAL_PRESS_EN | BMP388_REG_VAL_TEMP_EN | BMP388_REG_VAL_NORMAL_MODE);
  return 0;
}*/

 void bmp388DataGet()
{
  uint32_t u32TempRegData, u32PressRegData;
  uint8_t u8Xlsb, u8Lsb, u8Msb;
  int32_t s32Pressure0 = 101325;  // Mean Sea Level Pressure = 1013.25 hPA (1hPa = 100Pa = 1mbar)
  int32_t s32Temp, s32Press, s32Alti;


  u8Xlsb= I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_TEMP_XLSB);
  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_TEMP_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_TEMP_MSB);
  u32TempRegData = u8Msb;
  u32TempRegData <<= 8;
  u32TempRegData |= u8Lsb;
  u32TempRegData <<= 8;
  u32TempRegData |= u8Xlsb;

  u8Xlsb= I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_PRESS_XLSB);
  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_PRESS_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_PRESS_MSB);
  u32PressRegData = u8Msb;
  u32PressRegData <<= 8;
  u32PressRegData |= u8Lsb;
  u32PressRegData <<= 8;
  u32PressRegData |= u8Xlsb;

  s32Temp  = bmp388CompensateTemp(u32TempRegData);
  s32Press = bmp388CompensatePress(u32PressRegData);
  s32Alti  = 4433000 * (1 - pow((float)((float)((float)(s32Press)/100.0f) / (float)s32Pressure0), 0.1903)); 

  //ps32Temp = bmp388CalAvgValue( &(gsstBMP388AvgFilter[0].u8Index), gsstBMP388AvgFilter[0].s32AvgBuffer, s32Temp );
 // ps32Press = bmp388CalAvgValue( &(gsstBMP388AvgFilter[1].u8Index), gsstBMP388AvgFilter[1].s32AvgBuffer, s32Press);
  //ps32Alti = bmp388CalAvgValue( &(gsstBMP388AvgFilter[2].u8Index), gsstBMP388AvgFilter[2].s32AvgBuffer, s32Alti );
  comp_data.pressure = s32Press/10000.0;
  comp_data.temperature = s32Temp/100.0;
  comp_data.ps32Alti = s32Alti/100.0;
  return;
}

static void bmp388GetCalibrationData(void)
{
  uint8_t u8Lsb, u8Msb;

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_T1_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_T1_MSB);
  gsstCali.T1 = u8Msb;
  gsstCali.T1 <<= 8;
  gsstCali.T1 |= u8Lsb;
  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_T2_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_T2_MSB);
  gsstCali.T2 = u8Msb;
  gsstCali.T2 <<= 8;
  gsstCali.T2 |= u8Lsb;
  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_T3);
  gsstCali.T3 = (int8_t)(u8Lsb);


  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P1_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P1_MSB);
  gsstCali.P1 = (int16_t)(u8Msb << 8 | u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P2_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P2_MSB);
  gsstCali.P2 = (int16_t)(u8Msb << 8 | u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P3);
  gsstCali.P3 = (int8_t)(u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P4);
  gsstCali.P4 = (int8_t)(u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P5_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P5_MSB);
  gsstCali.P5 = (u8Msb << 8 | u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P6_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P6_MSB);
  gsstCali.P6 = (u8Msb << 8 | u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P7);
  gsstCali.P7 = (int8_t)(u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P8);
  gsstCali.P8 = (int8_t)(u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P9_LSB);
  u8Msb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P9_MSB);
  gsstCali.P9 = (int16_t)(u8Msb << 8 | u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P10);
  gsstCali.P10 = (int8_t)(u8Lsb);

  u8Lsb = I2C_ReadOneByte(I2C_ADD_BMP388, BMP388_REG_ADD_P11);
  gsstCali.P11 = (int8_t)(u8Lsb);

  return;
}

static int64_t bmp388CompensateTemp(uint32_t u32RegData)
{ 
  uint64_t partial_data1;
  uint64_t partial_data2;
  uint64_t partial_data3;
  int64_t partial_data4;
  int64_t partial_data5;
  int64_t partial_data6;
  int64_t comp_temp;

  partial_data1 = (uint64_t)(u32RegData - (256 * (uint64_t)(gsstCali.T1)));
  partial_data2 = (uint64_t)(gsstCali.T2 * partial_data1);
  partial_data3 = (uint64_t)(partial_data1 * partial_data1);
  partial_data4 = (int64_t)(((int64_t)partial_data3) * ((int64_t)gsstCali.T3));
  partial_data5 = ((int64_t)(((int64_t)partial_data2) * 262144) + (int64_t)partial_data4);
  partial_data6 = (int64_t)(((int64_t)partial_data5) / 4294967296);
  /* Store t_lin in dev. structure for pressure calculation */
  gsstCali.T_fine = partial_data6;
  comp_temp = (int64_t)((partial_data6 * 25)  / 16384);
  return comp_temp;   
}

static int64_t bmp388CompensatePress(uint32_t u32RegData)
{
  int64_t partial_data1;
  int64_t partial_data2;
  int64_t partial_data3;
  int64_t partial_data4;
  int64_t partial_data5;
  int64_t partial_data6;
  int64_t offset;
  int64_t sensitivity;
  uint64_t comp_press;

  partial_data1 = gsstCali.T_fine * gsstCali.T_fine;
  partial_data2 = partial_data1 / 64;
  partial_data3 = (partial_data2 * gsstCali.T_fine) / 256;
  partial_data4 = (gsstCali.P8 * partial_data3) / 32;
  partial_data5 = (gsstCali.P7 * partial_data1) * 16;
  partial_data6 = (gsstCali.P6 * gsstCali.T_fine) * 4194304;
  offset = (int64_t)((int64_t)(gsstCali.P5) * (int64_t)140737488355328) + 
            partial_data4 + partial_data5 + partial_data6;

  partial_data2 = (((int64_t)gsstCali.P4) * partial_data3) / 32;
  partial_data4 = (gsstCali.P3 * partial_data1) * 4;
  partial_data5 = ((int64_t)(gsstCali.P2) - 16384) * ((int64_t)gsstCali.T_fine) * 2097152;
  sensitivity = (((int64_t)(gsstCali.P1) - 16384) * (int64_t)70368744177664) + 
                partial_data2 + partial_data4 + partial_data5;

  partial_data1 = (sensitivity / 16777216) * u32RegData;
  partial_data2 = (int64_t)(gsstCali.P10) * (int64_t)(gsstCali.T_fine);
  partial_data3 = partial_data2 + (65536 * (int64_t)(gsstCali.P9));
  partial_data4 = (partial_data3 * u32RegData) / 8192;
  partial_data5 = (partial_data4 * u32RegData) / 512;
  partial_data6 = (int64_t)((uint64_t)u32RegData * (uint64_t)u32RegData);
  partial_data2 = ((int64_t)(gsstCali.P11) * (int64_t)(partial_data6)) / 65536;
  partial_data3 = (partial_data2 * u32RegData) / 128;
  partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;
  comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776);

  return comp_press;
}

int32_t * bmp388CalAvgValue(uint8_t *pu8Index, int32_t *ps32AvgBuffer, int32_t s32InVal)
{ 
  uint8_t i;
int32_t *ps32OutVal;
  *(ps32AvgBuffer + ((*pu8Index) ++)) = s32InVal;
  *pu8Index &= 0x07;

  *ps32OutVal = 0;
  for(i = 0; i < 8; i ++) 
  {
    *ps32OutVal += *(ps32AvgBuffer + i);
  }
  *ps32OutVal >>= 3;

  return ps32OutVal;
}

#ifdef __cplusplus
}
#endif
