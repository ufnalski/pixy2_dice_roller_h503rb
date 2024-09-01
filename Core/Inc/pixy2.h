/*
 * pixy2.h
 *
 *  Created on: Aug 23, 2024
 *      Author: user
 */

#ifndef INC_PIXY2_H_
#define INC_PIXY2_H_

#include "i2c.h"

#define PIXY2_I2C &hi2c2
#define PIXY2_ADDR (0x54 << 1)
#define PIXY2_I2C_TX_TIMEOUT 50
#define PIXY2_I2C_RX_TIMEOUT 100

#define PIXY2_ENDIANNES 0 // 0 -> little-endian, 1 -> big-endian

#define PIXY2_SIGNATURE_1 0x01
#define PIXY2_SIGNATURE_2 0x02
#define PIXY2_SIGNATURE_3 0x04
#define PIXY2_SIGNATURE_4 0x08
#define PIXY2_SIGNATURE_5 0x10
#define PIXY2_SIGNATURE_6 0x20

#define PIXY2_LABEL_1 "red"
#define PIXY2_LABEL_2 "green"
#define PIXY2_LABEL_3 "blue"
#define PIXY2_LABEL_4 "orange"
#define PIXY2_LABEL_5 "yellow"
#define PIXY2_LABEL_6 "violet"  // "purple"? I have to ask my wife :)

uint16_t Camera2Host(uint8_t *dataReceivedFirst, uint8_t *dataReceivedSecond,
		uint8_t big_endian);
uint16_t Pixy2Checksum(uint8_t *pixy2_message);
void Pixy2SetLamp(uint8_t upper, uint8_t lower);
uint8_t Pixy2SignatureDetect(uint8_t signatures_to_detect);

#endif /* INC_PIXY2_H_ */
