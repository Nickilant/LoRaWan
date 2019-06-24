#include "sx/sx_extern.h"

#define stateRegSx 	1
#define onCrc 		0

uint8_t spiState;

#if stateRegSx
uint8_t initDefaultRegSx[45][2];
#endif

#if onCrc
// CRC types
#define CRC_TYPE_CCITT	0
#define CRC_TYPE_IBM	1

// Polynomial =X^16 + X^12 + X^5 + 1
#define POLYNOMIAL_CCITT	0x1021
// Polynomial =X^16 + X^12 + X^2 + 1
#define POLYNOMIAL_IBM		0x8005

// Seeds
#define CRC_IBM_SEED 	0xffff
#define CRC_CCITT_SEED 	0x1d0f
#endif
