#include "sx/sx_conf.h"

#if onCrc
uint16_t ComputeCrc(uint16_t crc, uint8_t data, uint16_t polynomial);
uint16_t RadioPacketComputeCrc(uint8_t *buffer, uint8_t bufferLength, uint8_t crcType);
#endif




