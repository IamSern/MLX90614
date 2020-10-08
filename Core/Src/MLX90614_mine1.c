
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "MLX90614_mine1.h"

extern I2C_HandleTypeDef hi2c2;


/**
 * thực hiện ghi data vào EEPROM
 * truoc khi ghi can xoa noi dung bang cach ghi 0x00 vao EEPROM truoc
 * @param devAddr: địa chỉ slave
 * @param regAddr: địa chỉ EEPROM
 * @param data: dữ liệu cần ghi vào EEPROM
 *
 */
void MLX90614_WriteReg(uint8_t devAddr, uint8_t regAddr, uint16_t data) {

	uint8_t i2cdata[4], temp[4];
	// start xoa
	temp[0] = (devAddr << 1);// chuyen dia chi slave tu 7bit sang 8bit
	temp[1] = regAddr;	//  lenh, dia chi EEPROM

	temp[2] = 0x00;	//LSB
	temp[3] = 0x00;	//MSB

	i2cdata[0] = temp[1]; //EEPROM-address
	i2cdata[1] = temp[2]; //Delete Byte, low
	i2cdata[2] = temp[3]; //Delete Byte, high

	HAL_I2C_Master_Transmit(&hi2c2, (devAddr << 1), i2cdata, 4, 0xFFFF);
	HAL_Delay(10);
	// end xoa


	// start write
	temp[2] = data & 0xFF; //LSB
	temp[3] = data >> 8;   //MSB

	i2cdata[0] = temp[1]; //lenh
	i2cdata[1] = temp[2]; //LSB
	i2cdata[2] = temp[3]; //MSB

	HAL_I2C_Master_Transmit(&hi2c2, (devAddr << 1), i2cdata, 4, 0xFFFF);
	HAL_Delay(10);
	// end write
}

/**
 * read data from register have address @regAddr
 * @param devAddr: slave address
 * @param regAddr: command
 *
 */
uint16_t MLX90614_ReadReg(uint8_t devAddr, uint8_t regAddr)
{
	uint16_t data;
	uint8_t in_buff[3];

	// read data from address @regAddr --> save in @in_buff
	HAL_I2C_Mem_Read(&hi2c2, (devAddr<<1), regAddr, 1, in_buff, 3, 100);
	// read LSB truoc --> LSB = in_buff[0] ; MSB = in_buff[1]

	data = (in_buff[1] <<8 | in_buff[0]);

	HAL_Delay(10);
	return data;
}

/**
 * tinh toan nhiet do tu data doc tu thanh ghi
 * @param devAddr: slave address
 * @param regAddr: command
 * @return temp(C)
 */
float MLX90614_ReadTemp(uint8_t devAddr, uint8_t regAddr)
{
	float temp;
	uint16_t data;

	data = MLX90614_ReadReg(devAddr, regAddr);
	temp = data*0.02 - 273.15;

	return temp;
}




