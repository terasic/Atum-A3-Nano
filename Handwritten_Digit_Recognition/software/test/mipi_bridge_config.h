/*
 * mipi_bridge_config.h
 *
 *  Created on: 2015��4��22��
 *      Author: Administrator
 */

#ifndef MIPI_BRIDGE_CONFIG_H_
#define MIPI_BRIDGE_CONFIG_H_

#define MIPI_BRIDGE_I2C_ADDR   0x1C  // 8'b0001_1100 - 7'b0E + 1'b0 (write bit)


extern I2C i2c_mipi_bridge;

#ifdef __cplusplus
extern "C" {
#endif

void MipiBridgeInit(void);
void MipiBridgeRegWrite(alt_u16 Addr, alt_u16 Value);
alt_u16 MipiBridgeRegRead(alt_u16 Addr);

#ifdef __cplusplus
}
#endif

#endif /* MIPI_BRIDGE_CONFIG_H_ */
