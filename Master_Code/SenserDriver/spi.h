#ifndef _SPI_H_
#define _SPI_H_

#include "sysconfig.h"

void spi_init(void);
uint8_t spi_single_send(uint8_t SendData);
void spi_transmit(uint8_t *data, uint16_t len);
void spi_receive(uint8_t *data, uint16_t len);


void icm20602_pin_init(void);
int8_t icm20602_read(uint8_t id, uint8_t reg, uint8_t * data, uint16_t len);
int8_t icm20602_write(uint8_t id, uint8_t reg, uint8_t * data, uint16_t len);

#endif
