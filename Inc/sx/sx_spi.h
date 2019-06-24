#include "stdio.h"

//SPI SX1276
void initSpi(void); // main.c
void writeByteSpiSx(uint8_t register_byte, uint8_t data_byte, uint8_t * status_spi); // Перезапись регистра SX1276. main.c
uint8_t readByteSpiSx(uint8_t register_byte, uint8_t * status_spi); // Чтение регистра SX1276. main.c

