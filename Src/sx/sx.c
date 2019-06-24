
#include "sx/sx.h"


uint8_t sxInit(void)
{
	/*
	uint8_t _b = 0;
	int8_t b_ = 0;
	b_ = sxOpMode(MODE_SLEEP);
	if(b_ != 1 && b_ != 0)
	{
		return 0;
	}
	// LoRa on
	// reg modem config 3
	// reg lna /rx
	// pa ramp /tx
	// bw - 125
	// coding rate - 5
	// header - 0
	// SF - 7
	// crc - on(1)
	// symb time - 5,8
	// max payload length - 80 len
	// sunc word - 0x34
	*/

	uint8_t _byte = 0x00;
	bool lora = false;
	/*-----RegOpMode--------------------------*/
	sxOpMode(MODE_SLEEP);

	_byte = readByteSpiSx(RegOpMode, &spiState);
	if(spiState == 0)
	{
		return 0;
	}
	_byte |= (1<<7);

	writeByteSpiSx(RegOpMode, _byte, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	lora = sxLoraOn(&spiState);
	if(spiState == 0)
	{
		return 0;
	}

	if(lora == true)
	{
		_byte = readByteSpiSx(RegOpMode, &spiState);
		if(spiState == 0)
		{
			return 0;
		}

		_byte &= ~(7<<0);
		_byte |= (1<<7);
		_byte &= ~(1<<6);
		_byte &= ~(1<<3);
		writeByteSpiSx(RegOpMode, _byte, &spiState);
		if(spiState == 0)
		{
			return 0;
		}
	} // 0b10000000
	else
	{
		return 0;
	}

	lora = sxLoraOn(&spiState);
	if(spiState == 0)
	{
		return 0;
	}

	if(lora == false)
	{
		return 0;
	}

	_byte = readByteSpiSx(RegOpMode, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegOpMode, 0, 0b10000000);
	initFunctionDefaultRegSx(RegOpMode, 1, _byte);
	#endif

	writeByteSpiSx(RegFrMsb, 0xd9, &spiState); // frf (23:16)
	if(spiState == 0)
	{
		return 0;
	}
	writeByteSpiSx(RegFrMid, 0x00, &spiState); // frf (15:8)
	if(spiState == 0)
	{
		return 0;
	}
	writeByteSpiSx(RegFrLsb, 0x00, &spiState); // frf (7:0)

	#if stateRegSx
	initFunctionDefaultRegSx(RegFrMsb, 0, 0xd9);
	initFunctionDefaultRegSx(RegFrMid, 0, 0x00);
	initFunctionDefaultRegSx(RegFrLsb, 0, 0x00);

	initFunctionDefaultRegSx(RegFrMsb, 1, readByteSpiSx(RegFrMsb, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	initFunctionDefaultRegSx(RegFrMid, 1, readByteSpiSx(RegFrMid, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	initFunctionDefaultRegSx(RegFrLsb, 1, readByteSpiSx(RegFrLsb, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegModemConfig1, 0b01111000, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig1, 0, 0b01111000);
	initFunctionDefaultRegSx(RegModemConfig1, 1, readByteSpiSx(RegModemConfig1, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif
	/*----- -----*/
	writeByteSpiSx(RegModemConfig2, 0b01110000, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig2, 0, 0b01110000);
	initFunctionDefaultRegSx(RegModemConfig2, 1, readByteSpiSx(RegModemConfig2, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegSymbTimeoutLsb, 0x08, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegSymbTimeoutLsb, 0, 0x08);
	initFunctionDefaultRegSx(RegSymbTimeoutLsb, 1, readByteSpiSx(RegSymbTimeoutLsb, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegModemConfig3, 0b00000100, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig3, 0, 0b00000100);
	initFunctionDefaultRegSx(RegModemConfig3, 1, readByteSpiSx(RegModemConfig3, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegMaxPayloadLength, 0xff, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegMaxPayloadLength, 0, 0xff);
	initFunctionDefaultRegSx(RegMaxPayloadLength, 1, readByteSpiSx(RegMaxPayloadLength, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegSyncWord, 0x34, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegSyncWord, 0, 0x34);
	initFunctionDefaultRegSx(RegSyncWord, 1, readByteSpiSx(RegSyncWord, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegDetectOptimize, 0xc3, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegDetectOptimize, 0, 0xc3);
	initFunctionDefaultRegSx(RegDetectOptimize, 1, readByteSpiSx(RegDetectOptimize, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegDetectionThreshold, 0x0a, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegDetectionThreshold, 0, 0x0a);
	initFunctionDefaultRegSx(RegDetectionThreshold, 1, readByteSpiSx(RegDetectionThreshold, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegPreambleMsb, 0x00, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegPreambleMsb, 0, 0x00);
	initFunctionDefaultRegSx(RegPreambleMsb, 1, readByteSpiSx(RegPreambleMsb, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegPreambleLsb, 0x08, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegPreambleLsb, 0, 0x08);
	initFunctionDefaultRegSx(RegPreambleLsb, 1, readByteSpiSx(RegPreambleLsb, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif


	/*----- -----*/
	writeByteSpiSx(RegLna, 0b00100011, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegLna, 0, 0b00100011);
	initFunctionDefaultRegSx(RegLna, 1, readByteSpiSx(RegLna, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	/*----- -----*/
	writeByteSpiSx(RegPaConfig, 0b11001111, &spiState);
	if(spiState == 0)
	{
		return 0;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegPaConfig, 0, 0b11001111);
	initFunctionDefaultRegSx(RegPaConfig, 1, readByteSpiSx(RegPaConfig, &spiState));
	if(spiState == 0)
	{
		return 0;
	}
	#endif

	sxFifoTxRxBaseAddr(0x00,0x00);
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/

// Р¤СѓРЅРєС†РёСЏ РґР»СЏ РїРµСЂРµРєР»СЋС‡РµРЅРёСЏ СЂРµР¶РёРјРѕРІ
int8_t sxOpMode(uint8_t MODE_DEFINE)
{
	uint8_t b = 0x00;
	b = readByteSpiSx(RegOpMode, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	b &= ~(7<<0);
	b |= MODE_DEFINE;
	writeByteSpiSx(RegOpMode, b, &spiState);
	if(spiState == 0)
	{
		return -2;
	}
	b = readByteSpiSx(RegOpMode, &spiState);
	if(spiState == 0)
	{
		return -3;
	}
	b &= ~(0xff << 3);

	if(b == MODE_DEFINE)
	{
		return 1;
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
// РџСЂРѕС‡РёС‚Р°С‚СЊ РІ РєР°РєРѕРј СЂРµР¶РёРјРµ sx
uint8_t sxOpModeReturn(void)
{
	uint8_t b = readByteSpiSx(RegOpMode, &spiState);
	if(spiState == 0)
	{
		return 255;
	}
	b &= ~(0xff << 3);
	return b;
}

/*-----------------------------------------------------------------------------------------------*/
// РќР°СЃС‚СЂРѕРёРєР° С‡Р°СЃС‚РѕС‚С‹ РєР°РЅР°Р»Р°
int8_t sxFRF(int frf_gerc/* РІ РіРµСЂС†Р°С…*/,  int frequency_gerc)
{
	/*
	Р•СЃР»Рё Rf Freq=912,00 РњР“С†, С‚РѕРіРґР°  Rf Freq / 61.03515625 Р“С† (С€Р°Рі)= 14942208 = 0xE40000

	The RF frequency is taken into account internally only when:
	- entering FSRX/FSTX modes
	- re-starting the receiver
	*/
	//sxOpMode(MODE_SLEEP);

	uint32_t frf = 0;

	{
		float f_step = (float)(frequency_gerc) / 524288;
		int res = (int)( (float)(frf_gerc) / f_step );

		frf = res;
	}

	writeByteSpiSx(RegFrMsb, (uint8_t)(frf>>16), &spiState ); // frf (23:16)
	if(spiState == 0)
	{
		return -1;
	}

	writeByteSpiSx(RegFrMid, (uint8_t)(frf>>8), &spiState ); // frf (15:8)
	if(spiState == 0)
	{
		return -2;
	}

	writeByteSpiSx(RegFrLsb, (uint8_t)(frf), &spiState ); // frf (7:0)
	if(spiState == 0)
	{
		return -3;
	}


	#if stateRegSx
	initFunctionDefaultRegSx(RegFrMsb,0,(uint8_t)(frf>>16));
	initFunctionDefaultRegSx(RegFrMid,0,(uint8_t)(frf>>8));
	initFunctionDefaultRegSx(RegFrLsb,0,(uint8_t)(frf));
	#endif

	{
		uint32_t frf_ = 0;
		uint8_t b = 0;
		b = readByteSpiSx(RegFrLsb, &spiState);
		if(spiState == 0)
		{
			return -4;
		}
		frf_ = b;
		#if stateRegSx
		initFunctionDefaultRegSx(RegFrLsb,1,b);
		#endif
		b = readByteSpiSx(RegFrMid, &spiState);
		if(spiState == 0)
		{
			return -5;
		}
		frf_ |= (uint32_t)(b << 8);
		#if stateRegSx
		initFunctionDefaultRegSx(RegFrMid,1,b);
		#endif
		b = readByteSpiSx(RegFrMsb, &spiState);
		if(spiState == 0)
		{
			return -6;
		}
		frf_ |= (uint32_t)(b << 16);
		#if stateRegSx
		initFunctionDefaultRegSx(RegFrMsb,1,b);
		#endif
		if(frf_ == frf)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
// РџСЂРѕРІРµСЂРєР° С‚Рѕ С‡С‚Рѕ РІРєР»СЋС‡РµРЅ СЂРµР¶РёРј Р›РѕСЂР°
bool sxLoraOn(uint8_t * status_spi)
{
	uint8_t b = readByteSpiSx(RegOpMode, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return false;
	}
	else
	{
		*status_spi = 1;
	}

	if( ( b & ( 1 << 7 ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*-----------------------------------------------------------------------------------------------*/
// РЈСЃС‚Р°РЅР°РІР»РёРІР°РµРј СЂРµР¶РёРј РѕР±РјРµРЅР° РґР°РЅРЅС‹С… Р»РѕСЂР°-РІР°РЅ
int8_t sxSyncWord(uint8_t sw)
{
	//sxOpMode(MODE_SLEEP);
	writeByteSpiSx(RegSyncWord, sw, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegSyncWord,0,sw);
	#endif
	{
		uint8_t b = 0;
		b = readByteSpiSx(RegSyncWord, &spiState);
		if(spiState == 0)
		{
			return -2;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegSyncWord,1,b);
		#endif
		if(b == sw)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
// РЈСЃС‚Р°РЅР°РІР»РёРІР°РµРј РґР»РёРЅСѓ РїСЂРµР°РјР±СѓР»С‹
int8_t sxPreamble(uint16_t preamble_length)
{
	//sxOpMode(MODE_SLEEP);

	/*----- -----*/
	writeByteSpiSx(RegPreambleMsb, (uint8_t)(preamble_length >> 8), &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	writeByteSpiSx(RegPreambleLsb, (uint8_t)(preamble_length), &spiState);
	if(spiState == 0)
	{
		return -2;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegPreambleMsb,0,(uint8_t)(preamble_length >> 8));
	initFunctionDefaultRegSx(RegPreambleLsb,0,(uint8_t)(preamble_length));
	#endif

	{
		uint16_t pr = 0;
		uint8_t b = 0;
		b = readByteSpiSx(RegPreambleLsb, &spiState);
		if(spiState == 0)
		{
			return -3;
		}
		pr = b;
		#if stateRegSx
		initFunctionDefaultRegSx(RegPreambleLsb,1,b);
		#endif

		b = readByteSpiSx(RegPreambleMsb, &spiState);
		if(spiState == 0)
		{
			return -4;
		}
		pr |= (uint16_t)(b << 8);
		#if stateRegSx
		initFunctionDefaultRegSx(RegPreambleMsb,1,b);
		#endif

		if(pr == preamble_length)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
// РџСЂРѕС‡РµСЃС‚СЊ РґР»РёРЅСѓ РїСЂРµР°РјР±СѓР»С‹
uint16_t sxPreamleLength(uint8_t * status_spi)
{
	uint8_t m = readByteSpiSx(RegPreambleMsb, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	uint8_t l = readByteSpiSx(RegPreambleLsb, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return (uint16_t)( l | (m << 8) );
}

/*-----------------------------------------------------------------------------------------------*/
//
int8_t sxSpreadingFactor(uint8_t sf/*6-12*/)
{
	//RegModemConfig2 7-4 (6-12 dec)
	//sxOpMode(MODE_SLEEP);

	uint8_t b = 0;
	uint8_t b_ = 0;
	b = readByteSpiSx(RegModemConfig2, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	b &= ~(15<<4);
	b |= (sf<<4);
	writeByteSpiSx(RegModemConfig2,b, &spiState);
	if(spiState == 0)
	{
		return -2;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig2,0,b);
	#endif

	b_ = readByteSpiSx(RegModemConfig2, &spiState);
	if(spiState == 0)
	{
		return -3;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig2,1,b_);
	#endif

	if(b_ != b)
	{
		return 0;
	}

	// low data rate optimize
	b = readByteSpiSx(RegModemConfig3, &spiState);
	if(spiState == 0)
	{
		return -4;
	}
	if(sf <= 10)
	{
		if( (b & (1 << 0)) )
		{
			b &= ~0x01;
		}
	}

	if(sf >= 11)
	{
		if( !(b & (1 << 0)) )
		{
			b |= 0x01;
		}
	}

	writeByteSpiSx(RegModemConfig3,b, &spiState);
	if(spiState == 0)
	{
		return -5;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig3,0,b);
	#endif

	b_ = readByteSpiSx(RegModemConfig3, &spiState);
	if(spiState == 0)
	{
		return -6;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig3,1,b_);
	#endif

	//sxSymbTimeout();

	if(b_ == b)
	{
		return 1;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------------------*/
//
int8_t sxCodingRate(uint8_t cr /*5-8*/)
{
	// RegModemConfig1 3-1 (001 010 011 100)
	//sxOpMode(MODE_SLEEP);

	uint8_t b = 0;
	b = readByteSpiSx(RegModemConfig1, &spiState);
	if(spiState == 0)
	{
		return -1;
	}

	b &= ~(7<<1);
	if(cr == 5) b |= (1<<1);
	if(cr == 6) b |= (2<<1);
	if(cr == 7) b |= (3<<1);
	if(cr == 8) b |= (4<<1);

	writeByteSpiSx(RegModemConfig1,b, &spiState);
	if(spiState == 0)
	{
		return -2;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig1,0,b);
	#endif

	{
		uint8_t b_ = 0;
		b_ = readByteSpiSx(RegModemConfig1, &spiState);
		if(spiState == 0)
		{
			return -3;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegModemConfig1,1,b_);
		#endif

		if(b == b_)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
//
int8_t sxBandwidth(uint16_t bw)
{
	//sxOpMode(MODE_SLEEP);

	uint8_t b = 0x00;
	uint8_t r = 0x00;

	if(bw == 125)
	{
		b = 0b01110000;
	}

	if(bw == 250)
	{
		b = 0b10000000;
	}
	r = readByteSpiSx(RegModemConfig1, &spiState);
	if(spiState == 0)
	{
		return -1;
	}

	r &= ~(0b11110000 << 0);
	r |= b;

	writeByteSpiSx(RegModemConfig1, r, &spiState);
	if(spiState == 0)
	{
		return -2;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig1,0,r);
	#endif

	b = readByteSpiSx(RegModemConfig1, &spiState);
	if(spiState == 0)
	{
		return -3;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig1,1,b);
	#endif

	if(b == r)
	{
		return 1;
	}
	return 0;
}

int8_t sxSymbTimeout(uint16_t timeout)
{
	/*

	в течение этого времени длина окон приема в преамбуле не обнаружена,
	 прием возвращается в режим ожидания
	SF >= 10 0x05
	SF <= 9 0x08
	 */
	// 12 bit
	uint8_t b = 0;
	writeByteSpiSx(RegSymbTimeoutLsb, timeout, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegSymbTimeoutLsb,0,timeout);
	#endif

	b = readByteSpiSx(RegSymbTimeoutLsb, &spiState);
	if(spiState == 0)
	{
		return -2;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegSymbTimeoutLsb,1,b);
	#endif


	b = readByteSpiSx(RegModemConfig2, &spiState);
	if(spiState == 0)
	{
		return -3;
	}

	uint8_t b_ = 0;
	b_ = (uint16_t)(timeout >> 8);
	b_ &= ~(0xff << 2);
	if(b_ == 0)
	{
		return 1;
	}

	//b_ = (uint16_t)(timeout >> 8);
	b &= ~0b00000011;
	b |= b_;
	writeByteSpiSx(RegModemConfig2, b, &spiState);
	if(spiState == 0)
	{
		return -3;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig2,0,b);
	#endif

	b = readByteSpiSx(RegModemConfig2, &spiState);
	if(spiState == 0)
	{
		return -4;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig2,1,b);
	#endif

	return 1;
}


/*-----------------------------------------------------------------------------------------------*/
// РЈРєР°Р·С‹РІР°РµРј РјР°РєСЃРёРјР°Р»СЊРЅСѓСЋ РґР»РёРЅСѓ РїР°РєРµС‚Р°, РїСЂРё РїСЂРёРµРјРµ Рё РїРµСЂРµРґР°С‡Рё
int8_t sxMaxPayloadLength(uint8_t pauload_length /*00-255*/)
{
	//
	writeByteSpiSx(RegMaxPayloadLength, pauload_length, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegMaxPayloadLength,0,pauload_length);
	#endif

	{
		uint8_t b = 0;
		b = readByteSpiSx(RegMaxPayloadLength, &spiState);
		if(spiState == 0)
		{
			return -2;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegMaxPayloadLength,1,b);
		#endif

		if(b == pauload_length)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/

// РЈСЃС‚Р°РЅР°РІР»РёРІР°РµРј Р±Р°Р·РѕРІС‹Рµ СѓРєР°Р·Р°С‚РµР»Рё rx/txb
int8_t sxFifoTxRxBaseAddr(uint8_t rx/*0x00*/, uint8_t tx/*0x80*/) // max 0xff
{
	uint8_t b = 0;
	uint8_t i = 0;

	writeByteSpiSx(RegFifoTxBaseAddr, tx, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegFifoTxBaseAddr,0,tx);
	#endif
	writeByteSpiSx(RegFifoRxBaseAddr, rx, &spiState);
	if(spiState == 0)
	{
		return -2;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegFifoRxBaseAddr,0,rx);
	#endif

	b = readByteSpiSx(RegFifoRxBaseAddr, &spiState);
	if(spiState == 0)
	{
		return -3;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegFifoRxBaseAddr,1,b);
	#endif

	if(b == rx)
	{
		i++;
	}

	b = readByteSpiSx(RegFifoTxBaseAddr, &spiState);
	if(spiState == 0)
	{
		return -4;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegFifoTxBaseAddr,1,b);
	#endif

	if(b == tx)
	{
		i++;
	}

	return i;
}

/*-----------------------------------------------------------------------------------------------*/
//
uint8_t sxInfoFifoRxBaseAddr(uint8_t * status_spi)
{
	uint8_t b = 0;
	b= readByteSpiSx(RegFifoRxBaseAddr, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/
//
uint8_t sxInfoFifoTxBaseAddr(uint8_t * status_spi)
{
	uint8_t b = 0;
	b = readByteSpiSx(RegFifoTxBaseAddr, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/

// Р¤СѓРЅРєС†РёСЏ РєРѕС‚РѕСЂР°СЏ СѓСЃС‚Р°РЅР°РІР»РёРІР°РµС‚ СѓРєР°Р·Р°С‚РµР»СЊ (Р°РґСЂРµСЃСЃ) FIFO
uint8_t sxFifoAddrPrt(uint8_t address, uint8_t * status_spi)
{
	writeByteSpiSx(RegFifoAddrPtr, address, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	uint8_t b = 0x00;
	b = readByteSpiSx(RegFifoAddrPtr, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/

// Р’РѕР·РІСЂР°С‰Р°РµС‚ РЅР°С‡Р°Р»СЊРЅС‹Р№ Р°РґСЂРµСЃСЃ РїРѕСЃР»РµРґРЅРµРіРѕ РїРѕР»СѓС‡РµРЅРЅРѕРіРѕ РїР°РєРµС‚Р°. РќР°С‡Р°Р»СЊРЅС‹Р№ Р°РґСЂРµСЃ (РІ Р±СѓС„РµСЂРµ РґР°РЅРЅС‹С…) РїРѕСЃР»РµРґРЅРµРіРѕ РїРѕР»СѓС‡РµРЅРЅРѕРіРѕ РїР°РєРµС‚Р°
uint8_t sxFifoRxCurrentAddr(uint8_t * status_spi)
{
	uint8_t b = 0x00;
	b = readByteSpiSx(RegFifoRxCurrentAddr, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/

// РўРµРєСѓС‰РµРµ Р·РЅР°С‡РµРЅРёРµ СѓРєР°Р·Р°С‚РµР»СЏ Р±СѓС„РµСЂР° РґР°РЅРЅС‹С… RX
uint8_t sxFifoRxByteAddr(uint8_t * status_spi)
{
	uint8_t b = 0x00;
	b = readByteSpiSx(RegFifoRxByteAddr, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/
// РљРѕР»РёС‡РµСЃС‚РІРѕ Р±Р°Р№С‚ РїРѕР»РµР·РЅРѕР№ РЅР°РіСЂСѓР·РєРё РїРѕСЃР»РµРґРЅРµРіРѕ РїРѕР»СѓС‡РµРЅРЅРѕРіРѕ РїР°РєРµС‚Р°
uint8_t sxFifoRxNbBytes(uint8_t * status_spi)
{
	uint8_t b = 0x00;
	b = readByteSpiSx(RegRxNbBytes, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/

// Р¤СѓРЅРєС†РёСЏ РґР»СЏ РїРµСЂРµРґР°С‡Рё Р±Р°Р№С‚Р° (FIFO)
int8_t sxWriteByteFifoNew(uint8_t byte_data)
{
	writeByteSpiSx(RegFifo, byte_data, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	return 1;
}

/*-----------------------------------------------------------------------------------------------*/

// Р¤СѓРЅРєС†РёСЏ РґР»СЏ С‡С‚РµРЅРёСЏ Р±Р°Р№С‚Р° СЃ (FIFO)
uint8_t sxReadByteFifoNew(uint8_t * status_spi)
{
	uint8_t b = 0x00;
	b = readByteSpiSx(RegFifo, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}
	return b;
}

/*-----------------------------------------------------------------------------------------------*/
// Р¤СѓРЅРєС†РёСЏ Р·Р°РґР°РµС‚ РєРѕР»РІРѕ Р±Р°Р№С‚ РІ РїР°РєРµС‚Рµ, РїСЂРё РѕС‚СЃСѓС‚СЃРІРёСЏ Р·Р°РіРѕР»РѕРІРєР°
int8_t sxPayloadLength(uint8_t len)
{
	writeByteSpiSx(RegPayloadLength, len, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegPayloadLength,0,len);
	#endif
	{
		uint8_t b = 0;
		b = readByteSpiSx(RegPayloadLength, &spiState);
		if(spiState == 0)
		{
			return -2;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegPayloadLength,1,b);
		#endif

		if(b == len)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
// Р¤СѓРЅРєС†РёСЏ РѕС‚РІРµС‡Р°РµС‚ Р·Р° РїСЂРµСЂС‹РІР°РЅРёРµ 1-РІРєР» 0-РІС‹РєР»
int8_t InitIrFlags(uint8_t flags)
{
	// 0 - true, 1 - false
	//sxOpMode(MODE_SLEEP);

	writeByteSpiSx(RegIrqFlagsMask, flags, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	#if stateRegSx
	initFunctionDefaultRegSx(RegIrqFlagsMask,0,flags);
	#endif

	{
		uint8_t b = 0;
		b = readByteSpiSx(RegIrqFlagsMask, &spiState);
		if(spiState == 0)
		{
			return -2;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegIrqFlagsMask,1,b);
		#endif

		if(b == flags)
		{
			return 1;
		}
	}
	return 0;
	// 7 RxTimeoutMask
	// 6 RxDoneMask
	// 5 PayloadCrcErrorMask
	// 4 ValidHeaderMask
	// 3 TxDoneMask.
	// 2 CadDoneMask
	// 1 FhssChangeChannelMask
	// 0 CadDetectedMask
}

/*-----------------------------------------------------------------------------------------------*/
// Р¤СѓРЅРєС†РёСЏ РѕС‚РІРµС‡Р°РµС‚ Р·Р° С‡С‚РµРЅРёРµ РїСЂРµСЂС‹РІР°РЅРёРµ 1-РІРєР» 0-РІС‹РєР». Рё СЃСЂР°Р·Сѓ Р¶Рµ РѕР±РЅСѓР»СЏРµРј Р·Р°РїРёСЃС‹РІР°СЏ 1
uint8_t ReadIrFlags(bool clear, uint8_t * status_spi)
{
	// РґРѕРїРёСЃР°С‚СЊ РґРѕР±Р°РІС‚СЊ РїР°СЂР°РјРµС‚СЂ РєРѕС‚РѕСЂС‹Р№ Р±СѓРґРµС‚ СЂРµРіР»Р°РјРµРЅС‚РёСЂРѕРІР°С‚СЊ РѕС‡РёС‰РµРЅРёРµ
	uint8_t b = readByteSpiSx(RegIrqFlags, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return 0;
	}
	else
	{
		*status_spi = 1;
	}

	if(clear == true)
	{
		writeByteSpiSx(RegIrqFlags, b, &spiState);
		if(spiState == 0)
		{
			*status_spi = 0;
			return 0;
		}
		else
		{
			*status_spi = 1;
		}
	}

	return b;
	// 7 RxTimeoutMask
	// 6 RxDoneMask
	// 5 PayloadCrcErrorMask
	// 4 ValidHeaderMask
	// 3 TxDoneMask.
	// 2 CadDoneMask
	// 1 FhssChangeChannelMask
	// 0 CadDetectedMask
}

/*-----------------------------------------------------------------------------------------------*/
// Р¤СѓРЅРєС†РёСЏ РґР»СЏ РІРєР» Рё РѕС‚РєР»СЋС‡РµРЅРёСЏ crc payload
int8_t sxCrc(bool crc)
{
	//sxOpMode(MODE_SLEEP);

	uint8_t b = readByteSpiSx(RegModemConfig2, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	if(crc == true)
	{

		b |= (1<<2);
	}
	else
	{
		b &= ~(1<<2);
	}
	writeByteSpiSx(RegModemConfig2,b, &spiState);
	if(spiState == 0)
	{
		return -2;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig2,0,b);
	#endif

	{
		uint8_t b_ = 0;
		b_ = readByteSpiSx(RegModemConfig2, &spiState);
		if(spiState == 0)
		{
			return -3;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegModemConfig2,1,b_);
		#endif

		if(b_ == b)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
//Р¤СѓРЅРєС†РёСЏ РґР»СЏ РІРєР» Рё РѕС‚РєР»СЋС‡РµРЅРёСЏ Р·Р°РіРѕР»РѕРІРєР°
int8_t sxHeader(bool header)
{
	//sxOpMode(MODE_SLEEP);

	uint8_t b = readByteSpiSx(RegModemConfig1, &spiState);
	if(spiState == 0)
	{
		return -1;
	}
	if(header == true)
	{
		b |= (1<<0);
	}
	else
	{
		b &= ~(1<<0);
	}

	writeByteSpiSx(RegModemConfig1,b, &spiState);
	if(spiState == 0)
	{
		return -2;
	}

	#if stateRegSx
	initFunctionDefaultRegSx(RegModemConfig1,0,b);
	#endif

	{
		uint8_t b_ = 0;
		b_ = readByteSpiSx(RegModemConfig1, &spiState);
		if(spiState == 0)
		{
			return -3;
		}
		#if stateRegSx
		initFunctionDefaultRegSx(RegModemConfig1,1,b_);
		#endif

		if(b == b_)
		{
			return 1;
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------------------------------*/
// Р¤СѓРЅРєС†РёСЏ РґР»СЏ РїСЂРѕРІРµСЂРєРё СЃС‚Р°С‚СѓСЃР° crc
bool sxStatusCrc(uint8_t * status_spi)
{
	uint8_t b = readByteSpiSx(RegModemConfig2, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return false;
	}
	else
	{
		*status_spi = 1;
	}

	if( ( b & ( 1 << 2 ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*-----------------------------------------------------------------------------------------------*/
// Р¤СѓРЅРєС†РёСЏ РґР»СЏ РїСЂРѕРІРµСЂРєРё СЃС‚Р°С‚СѓСЃР° header
bool sxStatusHeader(uint8_t * status_spi)
{
	uint8_t b = readByteSpiSx(RegModemConfig1, &spiState);
	if(spiState == 0)
	{
		*status_spi = 0;
		return false;
	}
	else
	{
		*status_spi = 1;
	}
	if( ( b & ( 1 << 0 ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*-----------------------------------------------------------------------------------------------*/
// Р РµР¶РёРј СЂР°Р±РѕС‚С‹ РїРµСЂРµРґР°С‡РёРєР°
uint8_t modeTx(uint8_t * array_bute, uint8_t length, uint8_t * return_error)
{
	/*
	1. РџСЂРѕРІРµСЂСЏРµРј С‡С‚Рѕ РґР»РёРЅР° РјР°СЃСЃРёРІР° РЅРµ РІС‹С…РѕРґРёС‚ Р·Р° РїСЂРµРґРµР»С‹
	2. РџСЂРѕРІРµСЂСЏРµРј РїСЂРёСЃСѓС‚СЃРІРёРµ Р·Р°РіРѕР»РѕРІРєР°
	3. РџСЂРѕРІРµСЂСЏРµРј РїСЂРёСЃСѓС‚СЃРІРёРµ crc
	4. РћРїСЂР°РІР»СЏРµРј РґР°РЅРЅС‹Рµ

	Р¤РѕСЂРјР°С‚С‹ РїР°РєРµС‚РѕРІ РІ С‚СЂРµС… СЂРµР¶РёРјР°С…
	0 - Explicit Header mode СЏРІРЅС‹Р№
	1 - Implicit Header mode РЅРµСЏРІРЅРѕРіРѕ

	return 0 - Р•СЃР»Рё РґР»РёРЅР° РїСЂРёРІС‹СЃРёР»Р°
	return 1 - Р’СЃРµ РѕРє
	return 2 - РќР°С…РѕРґРёС‚СЊСЃСЏ РІ СЂРµР¶РёРјРµ РїРµСЂРµРґР°С‡Рё

	// 7 RxTimeoutMask
	// 6 RxDoneMask
	// 5 PayloadCrcErrorMask
	// 4 ValidHeaderMask
	// 3 TxDoneMask.
	// 2 CadDoneMask
	// 1 FhssChangeChannelMask
	// 0 CadDetectedMask
	*/

	// TX SINGLE
	uint8_t b = 0x00;
	int8_t b_ = 0;
	b = 255;

	b = readByteSpiSx(RegMaxPayloadLength, &spiState);

	if(spiState == 0)
	{
		*return_error = 1;
		return 0;
	}

	if(b <= length)
	{
		// РїСЂРѕРІРµСЂСЏРµРј РґР»РёРЅСѓ Р±Р°Р·С‹ rx Рё tx
		*return_error = 2;
		return 0;
	}

	b_ = sxOpMode(MODE_STDBY);
	if(b_ != 0 && b_ != 1)
	{
		*return_error = 3;
		return 0;
	}
	//writeByteSpiSx(RegPayloadLength, length);

	b_ = sxPayloadLength(length);
	if(b_ == -1 || b == -2)
	{
		*return_error = 4;
		return 0;
	}

	//b_ = sxFifoTxRxBaseAddr(0,0);
	//if(b_ != 1 || b_ != 2)
	{
		//return 0;
	}
	sxFifoAddrPrt(0, &spiState);
	if(spiState == 0)
	{
		*return_error = 5;
		return 0;
	}

	// write fifo
	for(uint8_t i = 0; i < length; i++)
	{
		b_ = sxWriteByteFifoNew(array_bute[i]);
		if(b_ == -1)
		{
			*return_error = 6;
			return 0;
		}
	}

	b_ = sxOpMode(MODE_TX);
	if(b_ != 1 && b_ != 0)
	{
		*return_error = 7;
		return 0;
	}
	/*
	while(1)
	{
		b = ReadIrFlags(1);
		if( ( b & (1 << 3) ) )
		{
			return 1;
		}
	}
	*/
	*return_error = 0;
	return 1;
}

/*-----------------------------------------------------------------------------------------------*/
// Р РµР¶РёРј СЂР°Р±РѕС‚С‹ РїСЂРёРµРјРЅРёРєР°
uint8_t modeRx(uint8_t * array_byte, uint8_t * return_error)
{
	uint8_t b = 0x00;
	int8_t b_ = 0x00;

	b = sxOpModeReturn();
	if(b == 255)
	{
		//error
	}

	if(b != MODE_RXCONTINUOUS)
	{
		//b_ = sxFifoTxRxBaseAddr(0x80, 0x00);
		//if(b_ != 1 || b_ != 2)
		{
			//errors
		}

		if(spiState == 0)
		{
			*return_error = 1;
			return 0;
		}

		b_ = sxOpMode(MODE_RXCONTINUOUS);
		if(b_ != 1 && b_ != 0)
		{
			*return_error = 2;
			return 0;
		}
	}

	b = readByteSpiSx(RegIrqFlags, &spiState); // С‡РёС‚Р°Рµ С„Р»Р°РіРё
	if(spiState == 0)
	{
		*return_error = 3;
		return 0;
	}
	//RxTimeoutMask // РµСЃР»Рё СѓСЃС‚Р°РЅРѕРІР»РµРЅ СЂРµР¶РёРј "rx single"
	/*if( !( b & ( 1 << 7 ) ) )
	//{
		//return 0;
	}*/

	//RxDoneMask
	if( !(b & ( 1 << 6 )) )
	{
		*return_error = 4;
		return 0;
	}

	//ValidHeader
	if( !( b & ( 1 << 4 ) ) )
	{
		*return_error = 5;
		return 0;
	}

	//PayloadCrcErrorMask
	if( ( b & ( 1 << 5 ) ) )
	{
		*return_error = 6;
		array_byte[40] = 1;
		//return 0;
	}

	writeByteSpiSx(RegIrqFlags, b, &spiState); // РѕС‡РёС‰Р°РµРј С„Р»Р°РіРё
	if(spiState == 0)
	{
		*return_error = 7;
		return 0;
	}

	{
		uint8_t length;
		length = sxFifoRxNbBytes(&spiState); // РґР»РёРЅР° РїР°РєРµС‚Р°
		if(spiState == 0)
		{
			*return_error = 8;
			return 0;
		}
		if(length == 0)
		{
			*return_error = 9;
			return 0;
		}

		b = sxFifoRxCurrentAddr(&spiState); // Р°РґСЂРµСЃСЃ РїРµСЂРІРѕРіРѕ Р±Р°Р№С‚Р° РїР°РєРµС‚Р°
		if(spiState == 0)
		{
			*return_error = 10;
			return 0;
		}
		sxFifoAddrPrt(b, &spiState);
		if(spiState == 0)
		{
			*return_error = 11;
			return 0;
		}

		for(uint8_t i = 0; i < length; i++)
		{
			array_byte[i] = sxReadByteFifoNew(&spiState);
			if(spiState == 0)
			{
				*return_error = 12;
				return 0;
			}
		}
		*return_error = 0;
		return length;
	}

	return 0;
}

/*-----------------------------------------------------------------------------------------------*/

#if stateRegSx
uint8_t initFunctionDefaultRegSx(uint8_t reg, uint8_t con, uint8_t data)
{
	uint8_t n = 0;
	switch(reg)
	{
		// RegOpMode 	0x01
		case 0x01:
			n = 1;
			break;

		// RegFrMsb 0x06
		case 0x06:
			n = 2;
			break;

		// RegFrMid 0x07
		case 0x07:
			n = 3;
			break;

		// RegFrLsb 0x08
		case 0x08:
			n = 4;
			break;

		// RegPaConfig 0x09
		case 0x09:
			n = 5;
			break;

		// RegPaRamp 	0x0a
		case 0x0a:
			n = 6;
			break;

		// RegOcp 		0x0b
		case 0x0b:
			n = 7;
			break;

		// RegLna 		0x0c
		case 0x0c:
			n = 8;
			break;

		// RegFifoAddrPtr 			0x0d
		case 0x0d:
			n = 9;
			break;

		// RegFifoTxBaseAddr 		0x0e
		case 0x0e:
			n = 10;
			break;

		// RegFifoRxBaseAddr 		0x0F
		case 0x0F:
			n = 11;
			break;

		// RegFifoRxCurrentAddr 	0x10
		case 0x10:
			n = 12;
			break;

		// RegIrqFlagsMask			0x11
		case 0x11:
			n = 13;
			break;

		// RegIrqFlags 			0x12
		case 0x12:
			n = 14;
			break;

		// RegRxNbBytes			0x13
		case 0x13:
			n = 15;
			break;

		// RegFifoRxByteAddr 		0x25
		case 0x25:
			n = 16;
			break;

		// RegRxHeaderCntValueMsb 0x14
		case 0x14:
			n = 17;
			break;

		// RegRxHeaderCntValueLsb 0x15
		case 0x15:
			n = 18;
			break;

		// RegModemStat	0x18
		case 0x18:
			n = 19;
			break;

		// RegModemConfig1 	0x1d
		case 0x1d:
			n = 20;
			break;

		// RegModemConfig2 	0x1e
		case 0x1e:
			n = 21;
			break;

		// RegSymbTimeoutLsb 	0x1f
		case 0x1f:
			n = 22;
			break;

		// RegModemConfig3		0x26
		case 0x26:
			n = 23;
			break;

		// PpmCorrection 0x27
		case 0x27:
			n = 24;
			break;

		// RegDetectOptimize 0x31
		case 0x31:
			n = 25;
			break;

		// RegDetectionThreshold 0x37
		case 0x37:
			n = 26;
			break;

		// RegInvertIQ 0x33
		case 0x33:
			n = 27;
			break;

		// RegSyncWord 0x39
		case 0x39:
			n = 28;
			break;

		// RegPreambleMsb 	0x20
		case 0x20:
			n = 29;
			break;

		// RegPreambleLsb 	0x21
		case 0x21:
			n = 30;
			break;

		// RegPayloadLength 	0x22
		case 0x22:
			n = 31;
			break;

		// RegMaxPayloadLength 0x23
		case 0x23:
			n = 32;
			break;

		// RegHopPeriod 		0x24
		case 0x24:
			n = 33;
			break;

		// RegDioMapping1 	0x40
		case 0x40:
			n = 34;
			break;

		// RegDioMapping2 	0x41
		case 0x41:
			n = 35;
			break;

		// RegVersion 	   	0x42
		case 0x42:
			n = 36;
			break;

		// RegTcxo 		0x4b
		case 0x4b:
			n = 37;
			break;

		// RegPaDac 		0x4d
		case 0x4d:
			n = 38;
			break;

		// RegFormerTemp 	0x5b
		case 0x5b:
			n = 39;
			break;

		// RegAgcRef 		0x61
		case 0x61:
			n = 40;
			break;

		// RegAgcThresh1 0x62
		case 0x62:
			n = 41;
			break;

		// RegAgcThresh2 0x63
		case 0x63:
			n = 41;
			break;

		// RegAgcThresh3 0x64
		case 0x64:
			n = 42;
			break;

		// RegPll 0x70
		case 0x70:
			n = 43;
			break;

		//RegFifo 	0x00
		case 0x00:
			n = 44;
			break;
	}

	if(n == 0)
	{
		return 255;
	}

	if(con == 0 || con == 1)
	{
		initDefaultRegSx[n][con] = data;
		return data;
	}

	if(con == 254)
	{
		return initDefaultRegSx[n][0];
	}

	if(con == 255)
	{
		return initDefaultRegSx[n][1];
	}
	return 255;

}
#endif
