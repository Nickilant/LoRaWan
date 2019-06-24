
#include "sx/sx_crc.h"

#if onCrc
/*
CRC algoritm implementation

\param[IN] crc Previous CRC value
\param[IN] data New data to be added to the CRC
\param[IN] polynomial CRC polynomial selected [CRC_TYPE_CCITT, CRC_TYPE_IBM]

\retval crc New computed CRC
*/
uint16_t ComputeCrc(uint16_t crc, uint8_t data, uint16_t polynomial)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		if( ( ( ( crc & 0x8000 ) >> 8) ^ ( data & 0x80 ) ) != 0 )
		{
			crc <<= 1;			// shift left once
			crc ^= polynomial;	// XOR with polynomial
		}
		else
		{
			crc <<= 1; 			// shift left once
		}
		data <<= 1;				// Next data bit
	}
	return crc;
}

/*
CRC algoritm implementation

\param[IN] buffer Array containing the data
\param[IN] bufferLength Buffer Length
\param[IN] crcrType polynomial selected [CRC_TYPE_CCITT, CRC_TYPE_IBM]

\retval crc Buffer computed CRC
*/
uint16_t RadioPacketComputeCrc(uint8_t *buffer, uint8_t bufferLength, uint8_t crcType)
{
	uint8_t i;
	uint16_t crc;
	uint16_t polynomial;
	
	polynomial = ( crcType == CRC_TYPE_IBM ) ? POLYNOMIAL_IBM : POLYNOMIAL_CCITT;
	crc = ( crcType == CRC_TYPE_IBM ) ? CRC_IBM_SEED : CRC_CCITT_SEED;
	
	for(i = 0; i < bufferLength; i++)
	{
		crc = ComputeCrc(crc, buffer[i],polynomial);
	}
	
	if(crcType == CRC_TYPE_IBM)
	{
		return crc;
	}
	else
	{
		return (uint16_t)(~crc);
	}
}
#endif




