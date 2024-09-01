/*
 * pixy2.c
 *
 *  Created on: Aug 23, 2024
 *      Author: user
 */

#include "pixy2.h"
#include "i2c.h"

uint16_t Camera2Host(uint8_t *dataReceivedFirst, uint8_t *dataReceivedSecond,
		uint8_t big_endian)
{
	if (big_endian)
	{
		return ((((uint16_t) (*dataReceivedFirst) << 8)) & 0xFF00)
				| (((uint16_t) (*dataReceivedSecond)) & 0x00FF);
	}
	else // little-endian
	{
		return (((uint16_t) (*dataReceivedFirst)) & 0x00FF)
				| ((((uint16_t) (*dataReceivedSecond) << 8)) & 0xFF00);
	}
}

uint16_t Pixy2Checksum(uint8_t *pixy2_message)
{
	uint16_t checksum = 0;
	for (uint8_t i = 0; i < pixy2_message[3]; i++)
	{
		checksum += pixy2_message[6 + i];
	}
	return checksum;
}

void Pixy2SetLamp(uint8_t upper, uint8_t lower)
{
	uint8_t pixy2_tx_buff[6];
	uint8_t pixy2_rx_buff[10];

	pixy2_tx_buff[0] = 174;
	pixy2_tx_buff[1] = 193;
	pixy2_tx_buff[2] = 22;
	pixy2_tx_buff[3] = 2;
	pixy2_tx_buff[4] = upper;
	pixy2_tx_buff[5] = lower;

	HAL_I2C_Master_Transmit(PIXY2_I2C, PIXY2_ADDR, pixy2_tx_buff,
			sizeof(pixy2_tx_buff), PIXY2_I2C_TX_TIMEOUT);
	HAL_I2C_Master_Receive(PIXY2_I2C, PIXY2_ADDR, pixy2_rx_buff,
			sizeof(pixy2_rx_buff), PIXY2_I2C_RX_TIMEOUT);
}

uint8_t Pixy2SignatureDetect(uint8_t signatures_to_detect)
{
	uint8_t pixy2_tx_buff[6];
	uint8_t pixy2_rx_buff[20];

	pixy2_rx_buff[8] = 0x0D; // just to make checksum invalid if no message is received
	pixy2_rx_buff[6] = 0; // signature detected

	pixy2_tx_buff[0] = 174;
	pixy2_tx_buff[1] = 193;
	pixy2_tx_buff[2] = 32;
	pixy2_tx_buff[3] = 2;
	pixy2_tx_buff[4] = signatures_to_detect;
	pixy2_tx_buff[5] = 1;  // return just the dominant signature

	HAL_I2C_Master_Transmit(PIXY2_I2C, PIXY2_ADDR, pixy2_tx_buff,
			sizeof(pixy2_tx_buff), PIXY2_I2C_TX_TIMEOUT);
	HAL_I2C_Master_Receive(PIXY2_I2C, PIXY2_ADDR, pixy2_rx_buff,
			sizeof(pixy2_rx_buff), PIXY2_I2C_RX_TIMEOUT);

	if (Pixy2Checksum(pixy2_rx_buff)
			== Camera2Host(pixy2_rx_buff + 4, pixy2_rx_buff + 5,
			PIXY2_ENDIANNES))
	{
		return pixy2_rx_buff[6];
	}
	else
	{
		return 0xFF; // communication error (not sure if this value is free to use)
	}
}
