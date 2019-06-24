#include "mac/mac_level.h"


/*-----FUNCTION---------------------------------------------------------*/


uint8_t aes128_encrypt(uint8_t * key, uint8_t * in_array_data, uint8_t * out_array_data,
		uint8_t length)
{
	//aes add key
	aes_context ctx1;
	unsigned char b = 0;
	b = aes_set_key(key, 16, &ctx1);
	if(b == (unsigned char)-1)
	{
		return 0;
	}

	// count block
	uint8_t count_block = 0;
	count_block = length / 16;
	if( (uint8_t)(count_block * 16) != length )
	{
		count_block++;
	}

	// array block
	uint8_t block[16];
	uint8_t block_[16];
	for(uint8_t i=0; i < count_block; i++)
	{
		uint8_t p = 0;
		p = i * 16;
		uint8_t p_ = 0;

		for(uint8_t j = 0; j < 16; j++)
		{
			p_ = p + j;
			if(p_ < length)
			{
				block[j] = in_array_data[p_];
			}
			else
			{
				block[j] = 0x00;
			}
		}

		// encrypt
		b = aesencrypt(block, block_, &ctx1);
		if(b == (unsigned char)-1)
		{
			return 0;
		}

		uint8_t h_ = 0;
		for(uint8_t h = 0; h < 16; h++)
		{
			h_ = p + h;
			//if(h_ < length)
			{
				out_array_data[h_] = block_[h];
			}
			//else
			{
				//out_array_data[h_] = 0x00;
			}
		}
	}

	return 1;
}

uint8_t aes128_decrypt(uint8_t * key, uint8_t * in_array_data, uint8_t * out_array_data,
uint8_t length)
{
	//aes add key
	aes_context ctx1;
	unsigned char b = 0;
	b = aes_set_key(key, 16, &ctx1);
	if(b == (unsigned char)-1)
	{
		return 0;
	}

	// count block
	uint8_t count_block = 0;
	count_block = length / 16;
	if( (uint8_t)(count_block * 16) != length )
	{
		count_block++;
	}

	// array block
	uint8_t block[16];
	uint8_t block_[16];
	for(uint8_t i=0; i < count_block; i++)
	{
		uint8_t p = 0;
		p = i * 16;
		uint8_t p_ = 0;

		for(uint8_t j = 0; j < 16; j++)
		{
			p_ = p + j;
			//if(p_ < length)
			{
				block[j] = in_array_data[p_];
			}
			//else
			{
				//block[j] = 0x00;
			}
		}

		// encrypt
		b = aesdecrypt(block, block_, &ctx1);
		if(b == (unsigned char)-1)
		{
			return 0;
		}

		uint8_t h_ = 0x00;
		for(uint8_t h = 0; h < 16; h++)
		{
			h_ = p + h;
			//if(h_ < length)
			{
				out_array_data[h_] = block_[h];
			}
			//else
			{
				//out_array_data[h_] = 0x00;
			}
		}
	}

	return 1;
}

uint8_t XOR_block(uint8_t * block_one, uint8_t * block_two, uint8_t * block_out)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		block_out[i] = block_one[i] ^ block_two[i];
	}
	return 1;
}

// OTA JSIntKey привык к сообщениям типа 1 Воссоединяться-запроса MIC, и Соединение - Принимают ответы
// JSIntKey is used to MIC Rejoin-Request type 1 messages and Join-Accept answers
uint8_t MacJSIntKeyDerivation(struct LoRaWAN * loraWan)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		/*
		JSIntKey[16], // JSIntKey = aes128_encrypt(NwkKey, 0x06 | DevEUI | pad16)
		*/
		uint8_t arr[16] = {0x06, // 0x06
				loraWan->EUI.DevEUI[7],loraWan->EUI.DevEUI[6],loraWan->EUI.DevEUI[5],loraWan->EUI.DevEUI[4], // DevEUI
				loraWan->EUI.DevEUI[3],loraWan->EUI.DevEUI[2],loraWan->EUI.DevEUI[1],loraWan->EUI.DevEUI[0], // DevEUI
				0,0,0,0,0,0,0}; // pad16


		// encrypt
		uint8_t b = 0;
		b = aes128_encrypt(loraWan->Key.NwkKey, arr, loraWan->Key.JSIntKey, 16);
		if(b == 0)
		{
			return 0;
		}

		return 1;

	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

// OTA JSEncKey используется для шифрования Соединения - Принимают инициированный Воссоединяться-запросом
// JSEncKey is used to encrypt the Join-Accept triggered by a Rejoin-Request
uint8_t MacJSEncKeyDerivation(struct LoRaWAN * loraWan)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		/*
		JSEncKey[16]; // JSEncKey = aes128_encrypt(NwkKey, 0x05 | DevEUI | pad16)
		*/
		uint8_t arr[16] = {0x05, // 0x05
				loraWan->EUI.DevEUI[7],loraWan->EUI.DevEUI[6],loraWan->EUI.DevEUI[5],loraWan->EUI.DevEUI[4], // DevEUI
				loraWan->EUI.DevEUI[3],loraWan->EUI.DevEUI[2],loraWan->EUI.DevEUI[1],loraWan->EUI.DevEUI[0], // DevEUI
				0,0,0,0,0,0,0}; // pad16

		// encrypt
		int8_t b = 0;
		b = aes128_encrypt(loraWan->Key.NwkKey, arr, loraWan->Key.JSEncKey, 16);
		if(b == 0)
		{
			return 0;
		}
		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

//in join accept,  after join req
uint8_t MacSessionKeys(struct LoRaWAN * loraWan)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		uint8_t b = 0;
		uint8_t arr[16];

		// optget == 0
		if( !(loraWan->Register.DLSettings &  (1 << OptNeg)) )
		{
			// AppSKey
			arr[0] = 0x02;
			arr[1] = loraWan->Nonce.JoinNonce[2];
			arr[2] = loraWan->Nonce.JoinNonce[1];
			arr[3] = loraWan->Nonce.JoinNonce[0];
			arr[4] = loraWan->NetID[2];
			arr[5] = loraWan->NetID[1];
			arr[6] = loraWan->NetID[0];
			arr[7] = loraWan->Nonce.DevNonce[1];
			arr[8] = loraWan->Nonce.DevNonce[0];
			for(uint8_t i = 9; i < 16; i++)
			{
				arr[i] = 0x00;
			}
		}
		else
		{
			// AppSKey
			arr[0] = 0x02;
			arr[1] = loraWan->Nonce.JoinNonce[2];
			arr[2] = loraWan->Nonce.JoinNonce[1];
			arr[3] = loraWan->Nonce.JoinNonce[0];
			for(uint8_t i = 0; i < 8; i++)
			{
				arr[4+i] = loraWan->EUI.JoinEUI[7-i];
			}
			arr[12] = loraWan->Nonce.DevNonce[1];
			arr[13] = loraWan->Nonce.DevNonce[0];
			for(uint8_t i = 14; i < 16; i++)
			{
				arr[i] = 0x00;
			}
		}

		// optget  == 0
		if( !(loraWan->Register.DLSettings &  (1 << OptNeg)) )
		{
			b = aes128_encrypt(loraWan->Key.NwkKey, arr, loraWan->Key.AppSKey, 16); // AppSKey
		}
		else
		{
			b = aes128_encrypt(loraWan->Key.AppKey, arr, loraWan->Key.AppSKey, 16); //AppSKey
		}
		if(b == 0)
		{
			return 0;
		}

		//FNwkSIntKey
		arr[0] = 0x01;
		b = aes128_encrypt(loraWan->Key.NwkKey, arr, loraWan->Key.FNwkSIntKey, 16); // AppSKey
		if(b == 0)
		{
			return 0;
		}

		// optget == 0
		if( !(loraWan->Register.DLSettings &  (1 << OptNeg)) )
		{
			/*
			If the OptNeg is unset, the session keys are derived from the NwkKey as follow:
			AppSKey = aes128_encrypt(NwkKey, 0x02 | JoinNonce 3 | NetID 3 | DevNonce 2 | pad16 )
			FNwkSIntKey = aes128_encrypt(NwkKey, 0x01 | JoinNonce | NetID | DevNonce | pad16)
			SNwkSIntKey = NwkSEncKey = FNwkSIntKey.
			*/
			for(uint8_t i = 0; i < 16; i++)
			{
				loraWan->Key.NwkSEncKey[i] = loraWan->Key.FNwkSIntKey[i];
				loraWan->Key.SNwkSIntKey[i] = loraWan->Key.FNwkSIntKey[i];
			}
		}
		else // optget == 1
		{
			//SNwkSIntKey
			arr[0] = 0x03;
			b = aes128_encrypt(loraWan->Key.NwkKey, arr, loraWan->Key.SNwkSIntKey, 16); // AppSKey
			if(b == 0)
			{
				return 0;
			}

			//NwkSEncKey
			arr[0] = 0x04;
			b = aes128_encrypt(loraWan->Key.NwkKey, arr, loraWan->Key.NwkSEncKey, 16); // AppSKey
			if(b == 0)
			{
				return 0;
			}

		}
		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

// не допроверил
uint8_t MacMICUplinkFrames(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t len_msg, uint8_t messageType)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// msg = MHDR | FHDR | FPort | FRMPayload
		uint8_t length = len_msg + 16;
		uint8_t msg_[length];

		loraWan->ArrayProgramming.B0[0] = 0x49; // 0x49
		loraWan->ArrayProgramming.B0[1] = 0x00; //  0x00 3
		loraWan->ArrayProgramming.B0[2] = 0x00; //  0x00 2
		loraWan->ArrayProgramming.B0[3] = 0x00; //  0x00 1
		loraWan->ArrayProgramming.B0[4] = 0x00; //  0x00 0
		loraWan->ArrayProgramming.B0[5] = 0x00; // Dir 0x00
		loraWan->ArrayProgramming.B0[6] = loraWan->DevAddr[3]; // DevAddr 3
		loraWan->ArrayProgramming.B0[7] = loraWan->DevAddr[2]; // DevAddr 2
		loraWan->ArrayProgramming.B0[8] = loraWan->DevAddr[1]; // DevAddr 1
		loraWan->ArrayProgramming.B0[9] = loraWan->DevAddr[0]; // DevAddr 0
		if(messageType == READ)
		{
			loraWan->ArrayProgramming.B0[10] = loraWan->FCnt.FCntUp[3]; // FCntUp 3
			loraWan->ArrayProgramming.B0[11] = loraWan->FCnt.FCntUp[2]; // FCntUp 2
			loraWan->ArrayProgramming.B0[12] = msg[6];// loraWan->FCnt.FCntUp[1]; // FCntUp 1
			loraWan->ArrayProgramming.B0[13] = msg[7];// loraWan->FCnt.FCntUp[0]; // FCntUp 0
		}
		else
		{
			loraWan->ArrayProgramming.B0[10] = loraWan->FCnt.FCntUp[3]; // FCntUp 3
			loraWan->ArrayProgramming.B0[11] = loraWan->FCnt.FCntUp[2]; // FCntUp 2
			loraWan->ArrayProgramming.B0[12] = loraWan->FCnt.FCntUp[1]; // FCntUp 1
			loraWan->ArrayProgramming.B0[13] = loraWan->FCnt.FCntUp[0]; // FCntUp 0
		}

		loraWan->ArrayProgramming.B0[14] = 0x00; // 0x00
		loraWan->ArrayProgramming.B0[15] = len_msg; // len(msg)

		// If the device is connected to a LoRaWAN1.0 Network Server then:
		// aes128_cmac(SNwkSIntKey, B0 | msg, len(msg) + len(B0), cmacF);
		// MIC = cmacF[0..3];
							/*
							// НЕ ОТРАБОТАННОЕ ПОЛЕ
							if(loraWan->LoRaWAN_version != 3)
							{
								//
								{
									//msg_[len_msg+16];
									//uint8_t length = len_msg + 16;

									for(uint8_t i = 0; i < len_msg; i++)
									{
										msg_[i+16] = msg[i];
									}

									for(uint8_t i = 0; i < 16; i++)
									{
										msg_[i] = B0[i];
									}

									//cmacF
									AES_CMAC(FNwkSIntKey, msg_, length, cmac);
									for(uint8_t i = 0; i < 4; i++)
									{
										MIC[i] = cmac[i];
									}

								}
								return 1;
							}
							*/
		// If the device is connected to a LoRaWAN1.1 Network Server then:
		// aes128_cmac(SNwkSIntKey, B1 | msg, len(msg) + len(B1), cmacS);
		// aes128_cmac(SNwkSIntKey, B0 | msg, len(msg) + len(B0), cmacF);
		// MIC = cmacS[0..1] | cmacF[0..1];
		//if(version_lorawan == 1)
		{
			//
			loraWan->ArrayProgramming.B1[0] = 0x49; // 0x49

			if( (loraWan->FCtrl & (1 << ACK)) )
			{
				//ConfFCnt
				if(messageType == READ)
				{
					loraWan->ArrayProgramming.B1[1] = msg[6];// loraWan->FCnt.FCntUp[1]; // ConfFCnt 0x00 1
					loraWan->ArrayProgramming.B1[2] = msg[7];// loraWan->FCnt.FCntUp[0]; // ConfFCnt 0x00 0
				}
				else // WRITE
				{
					loraWan->ArrayProgramming.B1[1] = loraWan->FCnt.FCntUp[1]; // ConfFCnt 0x00 1
					loraWan->ArrayProgramming.B1[2] = loraWan->FCnt.FCntUp[0]; // ConfFCnt 0x00 0
				}
			}
			else
			{
				loraWan->ArrayProgramming.B1[1] = 0x00;
				loraWan->ArrayProgramming.B1[2] = 0x00;
			}
			loraWan->ArrayProgramming.B1[3] = loraWan->Register.TxDr; //  TxDr. TxDr is the data rate used for the transmission of the uplink
			loraWan->ArrayProgramming.B1[4] = loraWan->Register.TxCh; //  TxCh. TxCh is the index of the channel used for the transmission
			loraWan->ArrayProgramming.B1[5] = 0x00; // Dir 0x00
			loraWan->ArrayProgramming.B1[6] = loraWan->DevAddr[3]; // DevAddr 3
			loraWan->ArrayProgramming.B1[7] = loraWan->DevAddr[2]; // DevAddr 2
			loraWan->ArrayProgramming.B1[8] = loraWan->DevAddr[1]; // DevAddr 1
			loraWan->ArrayProgramming.B1[9] = loraWan->DevAddr[0]; // DevAddr 0

			if(messageType == READ)
			{
				loraWan->ArrayProgramming.B1[10] = loraWan->FCnt.FCntUp[3]; // FCntUp 3
				loraWan->ArrayProgramming.B1[11] = loraWan->FCnt.FCntUp[2]; // FCntUp 2
				loraWan->ArrayProgramming.B1[12] = msg[6];//loraWan->FCnt.FCntUp[1]; // FCntUp 1
				loraWan->ArrayProgramming.B1[13] = msg[7];//loraWan->FCnt.FCntUp[0]; // FCntUp 0
			}
			else // WRITE
			{
				loraWan->ArrayProgramming.B1[10] = loraWan->FCnt.FCntUp[3]; // FCntUp 3
				loraWan->ArrayProgramming.B1[11] = loraWan->FCnt.FCntUp[2]; // FCntUp 2
				loraWan->ArrayProgramming.B1[12] = loraWan->FCnt.FCntUp[1]; // FCntUp 1
				loraWan->ArrayProgramming.B1[13] = loraWan->FCnt.FCntUp[0]; // FCntUp 0
			}

			loraWan->ArrayProgramming.B1[14] = 0x00; // 0x00
			loraWan->ArrayProgramming.B1[15] = len_msg; // len(msg)


			{
				//msg_[len_msg+16];
				//uint8_t length = len_msg + 16;

				for(uint8_t i = 0; i < len_msg; i++)
				{
					msg_[i+16] = msg[i];
				}

				for(uint8_t i = 0; i < 16; i++)
				{
					msg_[i] = loraWan->ArrayProgramming.B1[i];
				}

				//cmacS
				AES_CMAC(loraWan->Key.SNwkSIntKey, msg_, length, loraWan->ArrayProgramming.cmac);
				loraWan->MIC[0] = loraWan->ArrayProgramming.cmac[0];
				loraWan->MIC[1] = loraWan->ArrayProgramming.cmac[1];


				for(uint8_t i = 0; i < 16; i++)
				{
					msg_[i] = loraWan->ArrayProgramming.B0[i];
				}
				//cmacF
				AES_CMAC(loraWan->Key.FNwkSIntKey, msg_, length, loraWan->ArrayProgramming.cmac);
				loraWan->MIC[2] = loraWan->ArrayProgramming.cmac[0];
				loraWan->MIC[3] = loraWan->ArrayProgramming.cmac[1];

			}
			return 1;
		}
		return 0;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

// не допроверил
uint8_t MacMICDownlinkFrames(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t len_msg, uint8_t messageType)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		//msg = MHDR | FHDR | FPort | FRMPayload
		uint8_t length = len_msg + 16;
		uint8_t msg_[length];

		loraWan->ArrayProgramming.B0[0] = 0x49; // 0x49

		if( (loraWan->FCtrl & (1 << ACK)) )
		{
			//ConfFCnt
			if(loraWan->Register.FPort >= 1 && loraWan->Register.FPort <= 255/*NetApp == APP*/)
			{
				if(messageType == READ)
				{
					loraWan->ArrayProgramming.B0[1] = msg[6];//loraWan->FCnt.AFCntDwn[1]; // ConfFCnt 0x00 1
					loraWan->ArrayProgramming.B0[2] = msg[7];//loraWan->FCnt.AFCntDwn[0]; // ConfFCnt 0x00 0
				}
				else // WRITE
				{
					loraWan->ArrayProgramming.B0[1] = loraWan->FCnt.AFCntDwn[1]; // ConfFCnt 0x00 1
					loraWan->ArrayProgramming.B0[2] = loraWan->FCnt.AFCntDwn[0]; // ConfFCnt 0x00 0
				}
			}
			if(loraWan->Register.FPort == 0 /*NetApp == NET*/)
			{
				if(messageType == READ)
				{
					loraWan->ArrayProgramming.B0[1] = msg[6];//loraWan->FCnt.NFCntDwn[1]; // ConfFCnt 0x00 1
					loraWan->ArrayProgramming.B0[2] = msg[7];//loraWan->FCnt.NFCntDwn[0]; // ConfFCnt 0x00 0
				}
				else // WRITE
				{
					loraWan->ArrayProgramming.B0[1] = loraWan->FCnt.NFCntDwn[1]; // ConfFCnt 0x00 1
					loraWan->ArrayProgramming.B0[2] = loraWan->FCnt.NFCntDwn[0]; // ConfFCnt 0x00 0
				}

			}

		}
		else
		{
			loraWan->ArrayProgramming.B0[1] = 0x00;
			loraWan->ArrayProgramming.B0[2] = 0x00;
		}

		loraWan->ArrayProgramming.B0[3] = 0x00; // 0x00
		loraWan->ArrayProgramming.B0[4] = 0x00; // 0x00
		loraWan->ArrayProgramming.B0[5] = 0x01; // Dir = 0x01;
		loraWan->ArrayProgramming.B0[6] = loraWan->DevAddr[3]; // DevAddr 3
		loraWan->ArrayProgramming.B0[7] = loraWan->DevAddr[2]; // DevAddr 2
		loraWan->ArrayProgramming.B0[8] = loraWan->DevAddr[1]; // DevAddr 1
		loraWan->ArrayProgramming.B0[9] = loraWan->DevAddr[0]; // DevAddr 0


		{
			if(loraWan->Register.FPort >= 1 && loraWan->Register.FPort <= 255/*NetApp == APP*/)
			{
				loraWan->ArrayProgramming.B0[10] = loraWan->FCnt.AFCntDwn[3]; // AFCntDwn 3
				loraWan->ArrayProgramming.B0[11] = loraWan->FCnt.AFCntDwn[2]; // AFCntDwn 2
				if(messageType == READ)
				{
					loraWan->ArrayProgramming.B0[12] = msg[6];//loraWan->FCnt.AFCntDwn[1]; // AFCntDwn 1
					loraWan->ArrayProgramming.B0[13] = msg[7];//loraWan->FCnt.AFCntDwn[0]; // AFCntDwn 0
				}
				else //WRITE
				{
					loraWan->ArrayProgramming.B0[12] = loraWan->FCnt.AFCntDwn[1]; // AFCntDwn 1
					loraWan->ArrayProgramming.B0[13] = loraWan->FCnt.AFCntDwn[0]; // AFCntDwn 0
				}
			}

			if(loraWan->Register.FPort == 0/*NetApp == NET*/)
			{
				loraWan->ArrayProgramming.B0[10] = loraWan->FCnt.NFCntDwn[3]; // NFCntDwn 3
				loraWan->ArrayProgramming.B0[11] = loraWan->FCnt.NFCntDwn[2]; // NFCntDwn 2
				if(messageType == READ)
				{
					loraWan->ArrayProgramming.B0[12] = msg[6];//loraWan->FCnt.NFCntDwn[1]; // NFCntDwn 1
					loraWan->ArrayProgramming.B0[13] = msg[7];//loraWan->FCnt.NFCntDwn[0]; // NFCntDwn 0
				}
				else // WRITE
				{
					loraWan->ArrayProgramming.B0[12] = loraWan->FCnt.NFCntDwn[1]; // NFCntDwn 1
					loraWan->ArrayProgramming.B0[13] = loraWan->FCnt.NFCntDwn[0]; // NFCntDwn 0
				}

			}
		}


		loraWan->ArrayProgramming.B0[14] = 0x00; // 0x00
		loraWan->ArrayProgramming.B0[15] = len_msg; // len(msg)



		// msg = MHDR | FHDR | FPort | FRMPayload
		//AES_CMAC(SNwkSIntKey, B0 | msg, len(msg) + len(B0), cmac);
		// MIC[0..3] = cmac[0..3]
		{
			for(uint8_t i = 0; i < len_msg; i++)
			{
				msg_[i+16] = msg[i];
			}

			for(uint8_t i = 0; i < 16; i++)
			{
				msg_[i] = loraWan->ArrayProgramming.B0[i];
			}

			AES_CMAC(loraWan->Key.SNwkSIntKey, msg_, length, loraWan->ArrayProgramming.cmac);
			for(uint8_t i = 0; i < 4; i++)
			{
				loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
			}
		}
		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;

}

// не допроверил
uint8_t MacFOpts(struct LoRaWAN * loraWan, uint8_t * fopts, uint8_t * fopts_len, uint8_t messageType)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// k = NwkSEncKey
		// a = 1, 4, 1, 4, 4 ,1 1
		// a = 0x01 | 4x 0x00 | Dir | DevAddr | FCntUp or NFCntDwn | 0x00 | 0x00
		// The direction field (Dir) is 0 for uplink frames and 1 for downlink frames.
		// S = ase128_encrypt(K, A);

		uint8_t FOpL = loraWan->FCtrl;
		FOpL &= ~(0b11110000);
		if(FOpL > 15)
		{
			return 0;
		}
		if(messageType == WRITE)
		{
			*fopts_len = FOpL;
		}

		loraWan->ArrayProgramming.A[0] = 0x01;
		loraWan->ArrayProgramming.A[1] = 0;
		loraWan->ArrayProgramming.A[2] = 0;
		loraWan->ArrayProgramming.A[3] = 0;
		loraWan->ArrayProgramming.A[4] = 0;
		loraWan->ArrayProgramming.A[5] = loraWan->Register.Dir;
		loraWan->ArrayProgramming.A[6] = loraWan->DevAddr[3];
		loraWan->ArrayProgramming.A[7] = loraWan->DevAddr[2];
		loraWan->ArrayProgramming.A[8] = loraWan->DevAddr[1];
		loraWan->ArrayProgramming.A[9] = loraWan->DevAddr[0];
		loraWan->ArrayProgramming.A[10] = 0;
		loraWan->ArrayProgramming.A[11] = 0;
		loraWan->ArrayProgramming.A[12] = 0;
		loraWan->ArrayProgramming.A[13] = 0;
		loraWan->ArrayProgramming.A[14] = 0;
		loraWan->ArrayProgramming.A[15] = 0;

		if(loraWan->Register.Dir == 0)
		{
			for(uint8_t i = 0; i < 4; i++)
			{
				loraWan->ArrayProgramming.A[10+i] = loraWan->FCnt.FCntUp[3-i];
			}
			//return 1;
		}

		if(loraWan->Register.Dir == 1)
		{
			for(uint8_t i = 0; i < 4; i++)
			{
				loraWan->ArrayProgramming.A[10+i] = loraWan->FCnt.NFCntDwn[3-i];
			}
			//return 1;
		}

		if(messageType == READ)
		{
			loraWan->ArrayProgramming.A[12] = loraWan->FCnt.FCnt[1];
			loraWan->ArrayProgramming.A[13] = loraWan->FCnt.FCnt[0];
		}

		uint8_t v = 0;

		v = aes128_encrypt(loraWan->Key.NwkSEncKey, loraWan->ArrayProgramming.A, loraWan->ArrayProgramming.S, 16);
		if(v == 0)
		{
			return 0;
		}

		for(uint8_t i = *fopts_len; i < 16; i++)
		{
			if(messageType == READ)
			{
				fopts[i] = 0x00;
			}
			else // WRITE
			{
				loraWan->FOpts[i] = 0x00;
			}
		}
		if(messageType == READ)
		{
			XOR_block(fopts, loraWan->ArrayProgramming.S, loraWan->FOpts);
		}
		else // WRITE
		{
			XOR_block(loraWan->FOpts, loraWan->ArrayProgramming.S, fopts);
		}
		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

// не допроверил
uint8_t MacFramePayloadEncryption_FRMPayload(struct LoRaWAN * loraWan,
		uint8_t * frmpayload, uint8_t frmpayload_len,
		uint8_t * pld, uint8_t messageType)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		uint8_t count_block = 0;
		count_block = frmpayload_len / 16;
		if( (uint8_t)(count_block * 16) != frmpayload_len )
		{
			count_block++;
		}

		//uint8_t A[16], S[16], K[16];

		// key if(NetApp == Net)
		if(loraWan->Register.FPort == 0)
		{
			for(uint8_t i = 0; i < 16; i++)
			{
				loraWan->ArrayProgramming.K[i] = loraWan->Key.NwkSEncKey[i];
			}
		}

		// key if(NetApp == App)
		if(loraWan->Register.FPort >= 1 && loraWan->Register.FPort <= 255 )
		{
			for(uint8_t i = 0; i < 16; i++)
			{
				loraWan->ArrayProgramming.K[i] = loraWan->Key.AppSKey[i];
			}
		}

		loraWan->ArrayProgramming.A[0] = 0x01;
		loraWan->ArrayProgramming.A[1] = 0x00;
		loraWan->ArrayProgramming.A[2] = 0x00;
		loraWan->ArrayProgramming.A[3] = 0x00; //0x01
		loraWan->ArrayProgramming.A[4] = 0x00;
		loraWan->ArrayProgramming.A[5] = loraWan->Register.Dir;
		loraWan->ArrayProgramming.A[6] = loraWan->DevAddr[3];
		loraWan->ArrayProgramming.A[7] = loraWan->DevAddr[2];
		loraWan->ArrayProgramming.A[8] = loraWan->DevAddr[1];
		loraWan->ArrayProgramming.A[9] = loraWan->DevAddr[0];

		// A[10..13]
		if(loraWan->Register.Dir == 0)//up
		{

			for(uint8_t i = 0; i < 4; i++)
			{
				loraWan->ArrayProgramming.A[10+i] = loraWan->FCnt.FCntUp[3-i];
			}

		}
		else
		{
			// net first
			if(loraWan->Register.FPort == 0/*net*/)
			{
				for(uint8_t i = 0; i < 4; i++)
				{
					loraWan->ArrayProgramming.A[10+i] = loraWan->FCnt.NFCntDwn[3-i];
				}
			}

			//app first
			if(loraWan->Register.FPort >= 1 && loraWan->Register.FPort <= 255 /*NetApp == 1*/)
			{
				for(uint8_t i = 0; i < 4; i++)
				{
					loraWan->ArrayProgramming.A[10+i] = loraWan->FCnt.AFCntDwn[3-i];
				}
			}
		}

		if(messageType == READ)
		{
			loraWan->ArrayProgramming.A[10] = 0x00;
			loraWan->ArrayProgramming.A[11] = 0x00;
			loraWan->ArrayProgramming.A[12] = loraWan->FCnt.FCnt[1];
			loraWan->ArrayProgramming.A[13] = loraWan->FCnt.FCnt[0];
		}

		loraWan->ArrayProgramming.A[14] = 0x00;
		loraWan->ArrayProgramming.A[15] = 0x01; // A[i]

		uint8_t pld_block[16], pld_block_out[16];
		uint8_t v = 0;
		for(uint8_t i = 0; i < count_block; i++)
		{
			uint8_t i_ = i * 16;
			loraWan->ArrayProgramming.A[15] = i + 1; // number block[16] payload

			v = aes128_encrypt(loraWan->ArrayProgramming.K, loraWan->ArrayProgramming.A, loraWan->ArrayProgramming.S, 16);
			if(v == 0)
			{
				return 0;
			}

			for(uint8_t j = 0; j < 16; j++)
			{
				if((i_+j) < frmpayload_len)
				{
					pld_block[j] = frmpayload[i_ + j];
				}
				else
				{
					pld_block[j] = 0x00;
				}
			}

			XOR_block(pld_block, loraWan->ArrayProgramming.S, pld_block_out);

			for(uint8_t q = 0; q < 16; q++)
			{
				pld[i_+q] = pld_block_out[q];
			}
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif

	return 0;
}

/*----------------------------------------------------------------------*/
#if transform

// Выводит полностью блок PHYPayload и возвращает длину блока
uint8_t transformMacJoinRequesMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg)
{
	// mac header
	// format ac msg = PHYPayload = MHDR | join/rejoin request | MIC
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return LW_ERR_CURRECT_VERSION;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return LW_ERR_CURRECT_VERSION;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return LW_ERR_CURRECT_VERSION;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// mac header
		// format ac msg = join request = JoinEUI | DevEUI | DevNonce
		uint8_t length = 23;
		*len_msg = 23;

		msg[0] = loraWan->Register.MHDR; // MHDR
		msg[0] &= ~ 0b11100000;
		msg[0] |= Join_requst; // 0x00

		// JoinEUI 8 bytes
		for(uint8_t i = 0; i < 8; i++)
		{
			msg[i+1] = loraWan->EUI.JoinEUI[7-i];
		}

		// DevEUI 8 bytes
		for(uint8_t i = 0; i < 8; i++)
		{
			msg[i+9] = loraWan->EUI.DevEUI[7-i];
		}

		// DevNonce 2 bytes
		msg[17] = loraWan->Nonce.DevNonce[1];
		msg[18] = loraWan->Nonce.DevNonce[0];

		//cmac = aes128_cmac(NwkKey, MHDR | JoinEUI | DevEUI | DevNonce)
		//MIC = cmac[0..3]
		// MIC 4 bytes
		AES_CMAC(loraWan->Key.NwkKey, msg, length - 4/*MIC len*/, loraWan->ArrayProgramming.cmac);
		for(uint8_t i = 0; i < 4; i++)
		{
			loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			msg[19+i] = loraWan->MIC[i];
		}
		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

// Выводит полностью блок PHYPayload и возвращает длину блока
uint8_t transformMacJoinAcceptMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg)
{
	// format ac msg = PHYPayload = MHDR | join accept
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// msg = MHDR | join-accept
		msg[0] = loraWan->Register.MHDR; // MHDR
		msg[0] &= ~ 0b11100000;
		msg[0] |= Join_accept; //


		msg[1] = loraWan->Nonce.JoinNonce[2]; // JoinNonce 2
		msg[2] = loraWan->Nonce.JoinNonce[1]; // JoinNonce 1
		msg[3] = loraWan->Nonce.JoinNonce[0]; // JoinNonce 0
		msg[4] = loraWan->Home_NetID[2]; // Home_NetID 2
		msg[5] = loraWan->Home_NetID[1]; // Home_NetID 1
		msg[6] = loraWan->Home_NetID[0]; // Home_NetID 0
		msg[7] = loraWan->DevAddr[3]; // DevAddr 3
		msg[8] = loraWan->DevAddr[2]; // DevAddr 2
		msg[9] = loraWan->DevAddr[1]; // DevAddr 1
		msg[10] = loraWan->DevAddr[0]; // DevAddr 0
		msg[11] = loraWan->Register.DLSettings; // DLSettings
		msg[12] = loraWan->Register.RxDelay; // RxDelay

		//(16) Optional
		for(uint8_t i = 0; i < 16; i++)
		{
			msg[13+i] = loraWan->CFList.CFList[i];
		}
		//msg[13] = CFList[0]; // CFList
		//msg[14] = 0; // CFList
		//msg[15] = 0; // CFList ...

		*len_msg = 13/**/+16/*len CFList*/+4/*len MIC*/;
		uint8_t msg_[*len_msg + 11 - 4/*MIC*/];
		uint8_t msg_decrypt[32];
		// MIC

		if( !(loraWan->Register.DLSettings &  (1 << OptNeg)) )
		{
			/*
			The MIC value of the join-accept message is calculated as follows:2 1594
			1595 cmac = aes128_cmac(NwkKey, MHDR | JoinNonce | NetID | DevAddr | DLSettings |
			1596 RxDelay | CFList )
			1597 MIC = cmac[0..3]
			*/
			AES_CMAC(loraWan->Key.NwkKey, msg, *len_msg - 4 /*MIC*/, loraWan->ArrayProgramming.cmac);
			loraWan->MIC[0] = loraWan->ArrayProgramming.cmac[0];
			loraWan->MIC[1] = loraWan->ArrayProgramming.cmac[1];
			loraWan->MIC[2] = loraWan->ArrayProgramming.cmac[2];
			loraWan->MIC[3] = loraWan->ArrayProgramming.cmac[3];

			//save MIC
			msg[*len_msg - 4] = loraWan->MIC[0];
			msg[*len_msg - 3] = loraWan->MIC[1];
			msg[*len_msg - 2] = loraWan->MIC[2];
			msg[*len_msg - 1] = loraWan->MIC[3];

			// aes128_decrypt
			uint8_t v = 0;
			//aes128_decrypt(NwkKey or JSEncKey, JoinNonce | NetID | DevAddr | DLSettings |
			// RxDelay | CFList | MIC).
			for(uint8_t i = 0; i < *len_msg-1/*MHDR*/; i++)
			{
				msg_decrypt[i] = msg[i+1];
			}
			v = aes128_decrypt(loraWan->Key.NwkKey, msg_decrypt, msg_, *len_msg-1/*MHDR*/);
			if(v == 0)
			{
				return 0;
			}
		}
		else // optget == 1
		{
			/*
			In this case the MIC value is calculated as follows:3
			cmac = aes128_cmac(JSIntKey,
			JoinReqType 1bytes | JoinEUI 8 | DevNonce 2 | MHDR | JoinNonce | NetID | DevAddr |
			DLSettings | RxDelay | CFList )
			MIC = cmac[0..3]
			*/
			//ReqType
			msg_[0] = loraWan->Req.ReqType; //JoinReqType

			//JoinEUI
			for(uint8_t i = 0; i < 8; i++)
			{
				msg_[1 + i] = loraWan->EUI.JoinEUI[7 - i];
			}

			//DevNonce
			for(uint8_t i = 0; i < 2; i++)
			{
				msg_[9 + i] = loraWan->Nonce.DevNonce[1 - i];
			}

			// 13
			//accept payload
			for(uint8_t i = 0; i < 29; i++)
			{
				msg_[11 + i] = msg[i];
			}
			// length 40
			AES_CMAC(loraWan->Key.NwkKey, msg_, 40, loraWan->ArrayProgramming.cmac);
			loraWan->MIC[0] = loraWan->ArrayProgramming.cmac[0];
			loraWan->MIC[1] = loraWan->ArrayProgramming.cmac[1];
			loraWan->MIC[2] = loraWan->ArrayProgramming.cmac[2];
			loraWan->MIC[3] = loraWan->ArrayProgramming.cmac[3];

			//save MIC
			msg[*len_msg - 4] = loraWan->MIC[0];
			msg[*len_msg - 3] = loraWan->MIC[1];
			msg[*len_msg - 2] = loraWan->MIC[2];
			msg[*len_msg - 1] = loraWan->MIC[3];

			// aes128_decrypt
			uint8_t v = 0;
			//aes128_decrypt(NwkKey or JSEncKey, JoinNonce | NetID | DevAddr | DLSettings |
			// RxDelay | CFList | MIC).
			for(uint8_t i = 0; i < *len_msg-1/*MHDR*/; i++)
			{
				msg_decrypt[i] = msg[i+1];
			}

			// if join req type 0xff NwkKey, else rejoin JSEncKey
			if(loraWan->Req.ReqType == JoinReqType) // Join req
			{
				v = aes128_decrypt(loraWan->Key.NwkKey, msg_decrypt, msg_, *len_msg - 1/*MHDR*/);
			}
			else
			{
				v = aes128_decrypt(loraWan->Key.JSEncKey, msg_decrypt, msg_, *len_msg - 1/*MHDR*/);
			}

			if(v == 0)
			{
				return 0;
			}
		}

		// decrypt save msg
		for(uint8_t i = 0; i < *len_msg - 1/*MHDR*/; i++)
		{
			msg[i+1] = msg_[i];
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

// Выводит полностью блок PHYPayload и возвращает длину блока
uint8_t transformMacRejoinRequesTypeMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		*len_msg = 0;
		msg[0] = loraWan->Register.MHDR; // MHDR
		msg[0] &= ~ 0b11100000;
		msg[0] |= Rejoin_request; // 0x00

		//RJcount1_last
		if(loraWan->Req.ReqType == RejoinReqType0 || loraWan->Req.ReqType == RejoinReqType2)
		{
			// req = RejoinType | NetID | DevEUI | RJcount0
			// cmac = aes128_cmac(SNwkSIntKey, MHDR | Rejoin Type | NetID | DevEUI | RJcount0))
			// msg = MHDR | req | MIC

			*len_msg = 19;

			msg[1] = loraWan->Req.ReqType; // ReqType 0 or 2
			msg[2] = loraWan->NetID[3]; // NetID
			msg[3] = loraWan->NetID[1]; // NetID
			msg[4] = loraWan->NetID[0]; // NetID
			for(uint8_t i = 0; i < 8; i++)
			{
				msg[5 + i] = loraWan->EUI.DevEUI[8 - i]; // DevEUI
			}
			if(loraWan->Req.ReqType == RejoinReqType0)
			{
				msg[13] = loraWan->Req.RJcount0[1]; // RJcount0
				msg[14] = loraWan->Req.RJcount0[0]; // RJcount0
			}
			else
			{
				msg[13] = loraWan->Req.RJcount2[1]; // RJcount2
				msg[14] = loraWan->Req.RJcount2[0]; // RJcount2
			}

			AES_CMAC(loraWan->Key.SNwkSIntKey, msg, *len_msg - 4 /*MIC*/, loraWan->ArrayProgramming.cmac);
			for(uint8_t i = 0; i < 4; i++)
			{
				loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
			}
			msg[15] = loraWan->MIC[0];
			msg[16] = loraWan->MIC[1];
			msg[17] = loraWan->MIC[2];
			msg[18] = loraWan->MIC[3];

			return 1;
		}

		if(loraWan->Req.ReqType == RejoinReqType1)
		{
			// req = RejoinType | JoinEUI | DevEUI | RJcount1
			// cmac = aes128_cmac(JSIntKey, MHDR | req)
			// msg = MHDR | req | MIC

			*len_msg = 24;

			msg[1] = loraWan->Req.ReqType; // RJReqType 1

			for(uint8_t i = 0; i < 8; i++)
			{
				msg[2 + i] = loraWan->EUI.JoinEUI[8 - i]; // JoinEUI
			}

			for(uint8_t i = 0; i < 8; i++)
			{
				msg[10 + i] = loraWan->EUI.JoinEUI[8 - i]; // DevEUI
			}

			msg[18] = loraWan->Req.RJcount1[1]; // RJcount1
			msg[19] = loraWan->Req.RJcount1[0]; // RJcount1

			AES_CMAC(loraWan->Key.JSIntKey, msg, *len_msg - 4 /*MIC*/, loraWan->ArrayProgramming.cmac);
			for(uint8_t i = 0; i < 4; i++)
			{
				loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
			}

			msg[20] = loraWan->MIC[0];
			msg[21] = loraWan->MIC[1];
			msg[22] = loraWan->MIC[2];
			msg[23] = loraWan->MIC[3];
			return 1;
		}
		return 0;
	}
	else
	{
		return 0;
	}
	#endif


	return 0;

}

uint8_t transformMacJoinAcceprCFList(struct LoRaWAN * loraWan, uint8_t * cflist)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		/*
		bytes	3			3			3			3			3			1
		data	Freq Ch2	Freq Ch3	Freq Ch4	Freq Ch5	Freq Ch6	CFListType
		*/

		// Freq Ch2
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.CFList[i] = loraWan->CFList.Freq_Ch2[2-i];
		}

		// Freq Ch3
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.CFList[3-i] = loraWan->CFList.Freq_Ch3[2-i];
		}

		// Freq Ch4
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.CFList[6-i] = loraWan->CFList.Freq_Ch4[2-i];
		}

		// Freq Ch5
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.CFList[9-i] = loraWan->CFList.Freq_Ch5[2-i];
		}

		// Freq Ch6
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.CFList[12+i] = loraWan->CFList.Freq_Ch6[2-i];
		}

		loraWan->CFList.CFListType = 0x00;
		loraWan->CFList.CFList[15] = loraWan->CFList.CFListType;

		for(uint8_t i = 0; i < 16; i++)
		{
			cflist[i] = loraWan->CFList.CFList[i];
		}
		return 1;
	}
	else
	{
		return 0;
	}
	#endif

	return 0;
}

uint8_t transformMacDataRateAndDeviceANDPower(struct LoRaWAN * loraWan)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		//ver 1.1
		//switch (datarate)
		switch (loraWan->DataRate.DR)
		{
			case 0x00:
				loraWan->DataRate.SF = 12;
				loraWan->DataRate.kHz = 125;
				return 1;
				break;

			case 0x01:
				loraWan->DataRate.SF = 11;
				loraWan->DataRate.kHz = 125;
				return 1;
				break;

			case 0x02:
				loraWan->DataRate.SF = 10;
				loraWan->DataRate.kHz = 125;
				return 1;
				break;

			case 0x03:
				loraWan->DataRate.SF = 9;
				loraWan->DataRate.kHz = 125;
				return 1;
				break;

			case 0x04:
				loraWan->DataRate.SF = 8;
				loraWan->DataRate.kHz = 125;
				return 1;
				break;

			case 0x05:
				loraWan->DataRate.SF = 7;
				loraWan->DataRate.kHz = 125;
				return 1;
				break;

			case 0x06:
				loraWan->DataRate.SF = 7;
				loraWan->DataRate.kHz = 250;
				return 1;
				break;

			default:
				return 0;
				break;
		}

		if(loraWan->DataRate.DR == 7)
		{
			// FSK: 50 kbps
		}

		if(loraWan->DataRate.DR >= 8 || loraWan->DataRate.DR <= 14)
		{
			// RFU
		}

		if(loraWan->DataRate.DR == 15)
		{
			// defined in LoRaWAN 1 (1 DR15 and TXPower15 are defined in the LinkADRReq MAC command of the LoRaWAN1.1 specification)
		}
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t transformMacDataRateMaximumPayloadSize(struct LoRaWAN * loraWan)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		//ver 1.1
		//switch (datarate)
		switch (loraWan->DataRate.DR)
		{
			case 0x00:
				loraWan->DataRate.M = 59;
				loraWan->DataRate.N = 51;
				return 1;
				break;

			case 0x01:
				loraWan->DataRate.M = 59;
				loraWan->DataRate.N = 51;
				return 1;
				break;

			case 0x02:
				loraWan->DataRate.M = 59;
				loraWan->DataRate.N = 51;
				return 1;
				break;

			case 0x03:
				loraWan->DataRate.M = 123;
				loraWan->DataRate.N = 115;
				return 1;
				break;

			case 0x04:
				if(loraWan->MacCommand.repeater == 1)
				{
					loraWan->DataRate.M = 230;
					loraWan->DataRate.N = 222;
					return 1;
					break;
				}

				if(loraWan->MacCommand.repeater == 0)
				{
					loraWan->DataRate.M = 250;
					loraWan->DataRate.N = 242;
					return 1;
					break;
				}
				break;

			case 0x05:
				if(loraWan->MacCommand.repeater == 1)
				{
					loraWan->DataRate.M = 230;
					loraWan->DataRate.N = 222;
					return 1;
					break;
				}

				if(loraWan->MacCommand.repeater == 0)
				{
					loraWan->DataRate.M = 250;
					loraWan->DataRate.N = 242;
					return 1;
					break;
				}
				break;

			case 0x06:
				if(loraWan->MacCommand.repeater == 1)
				{
					loraWan->DataRate.M = 230;
					loraWan->DataRate.N = 222;
					return 1;
					break;
				}

				if(loraWan->MacCommand.repeater == 0)
				{
					loraWan->DataRate.M = 250;
					loraWan->DataRate.N = 242;
					return 1;
					break;
				}
				break;

			case 0x07:
				if(loraWan->MacCommand.repeater == 1)
				{
					loraWan->DataRate.M = 230;
					loraWan->DataRate.N = 222;
					return 1;
					break;
				}

				if(loraWan->MacCommand.repeater == 0)
				{
					loraWan->DataRate.M = 250;
					loraWan->DataRate.N = 242;
					return 1;
					break;
				}
				break;

			default:
				return 0;
				break;
		}
	}
	else
	{
		return 0;
	}
	#endif

	return 0;

}

uint8_t transformMacTxPower(struct LoRaWAN * loraWan)
{

	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		switch (loraWan->DataRate.TXPower)
		{
			case 0:
				loraWan->DataRate.MaxEIRP = 20; // Max EIRP
				return 1;
				break;

			case 1:
				loraWan->DataRate.MaxEIRP = 2;
				return 1;
				break;

			case 2:
				loraWan->DataRate.MaxEIRP = 4;
				return 1;
				break;

			case 3:
				loraWan->DataRate.MaxEIRP = 6;
				return 1;
				break;

			case 4:
				loraWan->DataRate.MaxEIRP = 8;
				return 1;
				break;

			case 5:
				loraWan->DataRate.MaxEIRP = 10;
				return 1;
				break;

			case 6:
				loraWan->DataRate.MaxEIRP = 12;
				return 1;
				break;

			case 7:
				loraWan->DataRate.MaxEIRP = 14;
				return 1;
				break;

			default:
				return 0;
				break;
		}

		if(loraWan->DataRate.TXPower >= 8 || loraWan->DataRate.TXPower <= 14)
		{
			// RFU
		}

		if(loraWan->DataRate.TXPower == 15)
		{
			//Defined in LoRAWAN
			// 1 DR15 and TXPower15 are defined in the LinkADRReq MAC command of the LoRaWAN1.1 specification
			// 2 ERP = EIRP пїЅ 2.15dB; it is referenced to a half-wave dipole antenna whose gain is expressed in dBd

		}
	}
	else
	{
		return 0;
	}
	#endif

	return 0;
}


#endif

/*----------------------------------------------------------------------*/
#if reform

uint8_t reformMacJoinRequesMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// mac header
		// format ac msg = join request = JoinEUI | DevEUI | DevNonce
		if(*len_msg != 23)
		{
			return 0;
		}
		/*
		uint8_t length = 23;
		*len_msg = 23;

		msg[0] = loraWan->Register.MHDR; // MHDR
		msg[0] &= ~ 0b11100000;
		msg[0] |= Join_requst; // 0x00
		*/
		uint8_t MType = msg[0];
		MType &= ~ 0b11100000;

		if(MType != Join_requst)
		{
			return 0;
		}

		//cmac = aes128_cmac(NwkKey, MHDR | JoinEUI | DevEUI | DevNonce)
		//MIC = cmac[0..3]
		// MIC 4 bytes
		AES_CMAC(loraWan->Key.NwkKey, msg, *len_msg - 4/*MIC len*/, loraWan->ArrayProgramming.cmac);
		for(uint8_t i = 0; i < 4; i++)
		{
			loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			//msg[19+i] = loraWan->MIC[i];
			if(loraWan->MIC[i] != msg[19+i])
			{
				return 0;
			}
		}

		// JoinEUI 8 bytes
		for(uint8_t i = 0; i < 8; i++)
		{
			//msg[i+1] = loraWan->EUI.JoinEUI[7-i];
			loraWan->EUI.JoinEUI[7-i] = msg[i+1];
		}

		// DevEUI 8 bytes
		for(uint8_t i = 0; i < 8; i++)
		{
			//msg[i+9] = loraWan->EUI.DevEUI[7-i];
			loraWan->EUI.DevEUI[7-i] = msg[i+9];
		}

		// DevNonce 2 bytes
		//msg[17] = loraWan->Nonce.DevNonce[1];
		//msg[18] = loraWan->Nonce.DevNonce[0];
		loraWan->Nonce.DevNonce[1] = msg[17];
		loraWan->Nonce.DevNonce[0] = msg[18];

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t reformMacJoinAcceptMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// msg = MHDR | join-accept
		//msg[0] = loraWan->Register.MHDR; // MHDR
		//msg[0] &= ~ 0b11100000;
		//msg[0] |= Join_accept; //


		uint8_t MType = msg[0];
		MType &= ~ 0b11100000;

		if(MType != Join_requst && MType != Rejoin_request)
		{
			return 0;
		}

		if(*len_msg != 13/**/+16/*len CFList*/+4/*len MIC*/)
		{
			return 0;
		}

		uint8_t a;
		uint8_t descrypt_msg[32], encrypt_msg[32];
		for(uint8_t i = 0; i < *len_msg - 1; i++)
		{
			descrypt_msg[i] = msg[i+1];
		}
		a = aes128_encrypt((uint8_t *)(MType == Join_requst ? loraWan->Key.NwkKey : loraWan->Key.JSEncKey), descrypt_msg, encrypt_msg, 32);
		if(a != 1)
		{
			return 0;
		}

		if( (encrypt_msg[10] & (1 << OptNeg)) ) // 1
		{
			AES_CMAC(loraWan->Key.JSIntKey, encrypt_msg, *len_msg-5, loraWan->ArrayProgramming.cmac);
			for(uint8_t i = 0; i < 4 ; i++)
			{
				loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
			}

			for(uint8_t i = 0; i < 4 ; i++)
			{
				if(loraWan->MIC[i] != encrypt_msg[28+i])
				{
					return 0;
				}
			}
		}
		else // 0
		{
			AES_CMAC(loraWan->Key.NwkKey, encrypt_msg, *len_msg-5, loraWan->ArrayProgramming.cmac);
			for(uint8_t i = 0; i < 4 ; i++)
			{
				loraWan->MIC[i] = loraWan->ArrayProgramming.cmac[i];
			}

			for(uint8_t i = 0; i < 4 ; i++)
			{
				if(loraWan->MIC[i] != encrypt_msg[28+i])
				{
					return 0;
				}
			}
		}

		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->Nonce.JoinNonce[2-i] = encrypt_msg[0+i];
		}

		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->NetID[2-i] = encrypt_msg[3+i];
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			loraWan->DevAddr[3-i] = encrypt_msg[6+i];
		}

		loraWan->Register.DLSettings = encrypt_msg[10];

		loraWan->Register.RxDelay = encrypt_msg[11];

		for(uint8_t i = 0; i < 3 ; i++)
		{
			loraWan->CFList.CFList[i] = encrypt_msg[12+i];
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t reformMacRejoinRequesTypeMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{

		uint8_t MType = msg[0];
		MType &= ~ 0b11100000;

		if(MType != Rejoin_request )
		{
			return 0;
		}

		uint8_t Rejoin_type = msg[1];

		if(Rejoin_type != RejoinReqType1 && Rejoin_type != RejoinReqType0 && Rejoin_type != RejoinReqType2)
		{
			return 0;
		}

		if( (Rejoin_type != RejoinReqType1 && *len_msg != 24) || ( (Rejoin_type != RejoinReqType0 || Rejoin_type != RejoinReqType2) && *len_msg != 19 ))
		{
			return 0;
		}


		if(RejoinReqType1 == Rejoin_type)
		{
			AES_CMAC(loraWan->Key.JSIntKey, msg, *len_msg - 4, loraWan->ArrayProgramming.cmac);
			loraWan->MIC[0] = loraWan->ArrayProgramming.cmac[0];
			loraWan->MIC[1] = loraWan->ArrayProgramming.cmac[1];
			loraWan->MIC[2] = loraWan->ArrayProgramming.cmac[2];
			loraWan->MIC[3] = loraWan->ArrayProgramming.cmac[3];

			for(uint8_t i = 0; i < 4; i++)
			{
				if(loraWan->MIC[i] != msg[20+i])
				{
					return 0;
				}
			}
		}

		if(RejoinReqType0 == Rejoin_type || RejoinReqType2 == Rejoin_type)
		{
			AES_CMAC(loraWan->Key.SNwkSIntKey, msg, *len_msg - 4, loraWan->ArrayProgramming.cmac);
			loraWan->MIC[0] = loraWan->ArrayProgramming.cmac[0];
			loraWan->MIC[1] = loraWan->ArrayProgramming.cmac[1];
			loraWan->MIC[2] = loraWan->ArrayProgramming.cmac[2];
			loraWan->MIC[3] = loraWan->ArrayProgramming.cmac[3];

			for(uint8_t i = 0; i < 4; i++)
			{
				if(loraWan->MIC[i] != msg[15+i])
				{
					return 0;
				}
			}
		}


		if(RejoinReqType1 == msg[1] )
		{
			// Действия по rejoin 1
			return 1;
		}

		if(RejoinReqType0 == msg[1] )
		{
			// Действия по rejoin 0
			return 1;
		}

		if(RejoinReqType2 == msg[1] )
		{
			// Действия по rejoin 2
			return 1;
		}
		return 0;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t reformMacJoinAcceprCFList(struct LoRaWAN * loraWan, uint8_t * cflist)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		/*
		bytes	3			3			3			3			3			1
		data	Freq Ch2	Freq Ch3	Freq Ch4	Freq Ch5	Freq Ch6	CFListType
		*/

		for(uint8_t i = 0; i < 16; i++)
		{
			loraWan->CFList.CFList[i] = cflist[i];
		}
		// Freq Ch2
		for(uint8_t i = 0; i < 3; i++)
		{

			loraWan->CFList.Freq_Ch2[2-i] = loraWan->CFList.CFList[i];
		}

		// Freq Ch3
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.Freq_Ch3[2-i] = loraWan->CFList.CFList[3-i];
		}

		// Freq Ch4
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.Freq_Ch4[2-i] = loraWan->CFList.CFList[6-i];
		}

		// Freq Ch5
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.Freq_Ch5[2-i] = loraWan->CFList.CFList[9-i];
		}

		// Freq Ch6
		for(uint8_t i = 0; i < 3; i++)
		{
			loraWan->CFList.Freq_Ch6[2-i] = loraWan->CFList.CFList[12+i];
		}

		loraWan->CFList.CFListType = loraWan->CFList.CFList[15];

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t reformMacDataRateAndDeviceANDPower(struct LoRaWAN * loraWan, uint8_t datarate)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		loraWan->DataRate.DR = datarate;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t reformMacDataRateMaximumPayloadSize(struct LoRaWAN * loraWan, uint8_t datarate)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		loraWan->DataRate.DR = datarate;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t reformMacTxPower(struct LoRaWAN * loraWan, uint8_t txpower)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		loraWan->DataRate.TXPower = txpower;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}


#endif
/*----------------------------------------------------------------------*/






/*----------------------------------------------------------------------*/
/*
pack = PHYPayload
PHYPayload: MHDR | MACPayload | MIC
MACPayload: FHDR | FPort | FRMPayload (data)

*/

uint8_t writeMacJoinReques(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return transformMacJoinRequesMessage(loraWan, data, len_data);
}

uint8_t readMacJoinReques(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return reformMacJoinRequesMessage(loraWan, data, len_data);
}

uint8_t writeMacJoinAccept(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return transformMacJoinAcceptMessage(loraWan, data, len_data);
}

uint8_t readMacJoinAccept(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return reformMacJoinAcceptMessage(loraWan, data, len_data);
}

uint8_t writeMacREJoinReques(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return transformMacRejoinRequesTypeMessage(loraWan, data, len_data);
}

uint8_t readMacREJoinReques(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	uint8_t s;
	s = reformMacRejoinRequesTypeMessage(loraWan, data, len_data);
	if(s == 0)
	{
		return 0;
	}

	// rejoin обработчик действий 0, 1, 2.

	return s;
}

// WRITE
#if upLink

#if LoRaDevice == LoRaDevice_EndDevice || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
//Без потверждения / Не дописано
uint8_t writeMacUnconfirmedDataUp(struct LoRaWAN * loraWan, uint8_t * FrmPayload_data_in, uint8_t len_FrmPayload_data_in,
		uint8_t * data_out, uint8_t * len_data_out)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		uint8_t s, len_opt;

		// struct loraWan config
		loraWan->Register.Dir = 0x00;
		// ...

		len_opt = loraWan->FCtrl;
		len_opt &= ~0b11110000;

		data_out[0] = loraWan->Register.MHDR;
		data_out[0] &= ~0b11100000;
		data_out[0] |= Unconfirmed_Data_Up;

		data_out[1] = loraWan->DevAddr[3];
		data_out[2] = loraWan->DevAddr[2];
		data_out[3] = loraWan->DevAddr[1];
		data_out[4] = loraWan->DevAddr[0];

		data_out[5] = loraWan->FCtrl;

		data_out[6] = loraWan->FCnt.FCntUp[1];
		data_out[7] = loraWan->FCnt.FCntUp[0];
		//data_out[8] = loraWan->FCnt.FCntUp[1];
		//data_out[9] = loraWan->FCnt.FCntUp[0];

		if(loraWan->Register.FPort == 0x00)
		{
			len_opt = 0x00;
			data_out[5] &= ~0b00001111;
		}
		else
		{
			// len opt
			if(len_opt != 0)
			{
				uint8_t fopts[16];
				s = MacFOpts(loraWan,fopts, &len_opt, WRITE);
				if(s == 0)
				{
					return 0;
				}
				for(uint8_t i = 0; i < len_opt; i++)
				{
					data_out[8+i] = fopts[i];
				}
			}
		}
		data_out[8+len_opt] = loraWan->Register.FPort;

		//data_out[9+len_opt] = loraWan->Register.FPort;

		uint8_t pld[len_FrmPayload_data_in];
		s = MacFramePayloadEncryption_FRMPayload(loraWan, FrmPayload_data_in, len_FrmPayload_data_in, pld, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < len_FrmPayload_data_in; i++)
		{
			data_out[9+len_opt+i] = pld[i];
		}

		*len_data_out = 9 + len_FrmPayload_data_in + len_opt + 4;

		s = MacMICUplinkFrames(loraWan, data_out, *len_data_out - 4, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[*len_data_out - 4 + i] = loraWan->MIC[i];
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
}

//с потвердждением
uint8_t writeMacConfirmedDataUp(struct LoRaWAN * loraWan, uint8_t * FrmPayload_data_in, uint8_t  len_FrmPayload_data_in,
		uint8_t *data_out, uint8_t * len_data_out)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		uint8_t s, len_opt;

		// struct loraWan config
		loraWan->Register.Dir = 0x00;
		// ...

		len_opt = loraWan->FCtrl;
		len_opt &= ~0b11110000;

		data_out[0] = loraWan->Register.MHDR;
		data_out[0] &= ~0b00011111;
		data_out[0] |= Confirmed_Data_Up;

		data_out[1] = loraWan->DevAddr[3];
		data_out[2] = loraWan->DevAddr[2];
		data_out[3] = loraWan->DevAddr[1];
		data_out[4] = loraWan->DevAddr[0];

		data_out[5] = loraWan->FCtrl;

		data_out[6] = loraWan->FCnt.FCntUp[1];
		data_out[7] = loraWan->FCnt.FCntUp[0];

		if(loraWan->Register.FPort == 0x00)
		{
			len_opt = 0x00;
			data_out[5] &= ~0b00001111;
		}
		else
		{
			// len opt
			if(len_opt != 0)
			{
				uint8_t fopts[16];
				s = MacFOpts(loraWan,fopts, &len_opt, WRITE);
				if(s == 0)
				{
					return 0;
				}
				for(uint8_t i = 0; i < len_opt; i++)
				{
					data_out[8+i] = fopts[i];
				}
			}
		}


		data_out[8+len_opt] = loraWan->Register.FPort;

		uint8_t pld[len_FrmPayload_data_in];
		s = MacFramePayloadEncryption_FRMPayload(loraWan, FrmPayload_data_in, len_FrmPayload_data_in, pld, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < len_FrmPayload_data_in; i++)
		{
			data_out[9+len_opt+i] = pld[i];
		}

		*len_data_out = 9 + len_FrmPayload_data_in + len_opt + 4;

		s = MacMICUplinkFrames(loraWan, data_out, *len_data_out - 4, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[*len_data_out - 4 + i] = loraWan->MIC[i];
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
}
#endif

#if LoRaDevice == LoRaDevice_Gateway || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
uint8_t readMacUnconfirmedDataUp(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		// struct loraWan config
		loraWan->Register.Dir = 0x00;
		// ...

		uint8_t len_opt = 0;
		uint8_t s = 0;
		uint8_t MType = data_in[0];
		MType &= ~ 0b00011111;

		if(MType != Unconfirmed_Data_Up)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[1+i] != loraWan->DevAddr[3-i])
			{
				return 1;
			}
		}

		loraWan->FCtrl = data_in[5];

		loraWan->FCnt.FCnt[1] = data_in[6];
		loraWan->FCnt.FCnt[0] = data_in[7];

		len_opt = data_in[5];
		len_opt &= ~0b11110000;

		loraWan->Register.FPort = data_in[8+len_opt];

		if(loraWan->Register.FPort == 0 && len_opt != 0)
		{
			return 2;
		}

		s = MacMICUplinkFrames(loraWan, data_in, len_data_in - 4, READ);
		if(s != 1)
		{
			return 3;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[(len_data_in - 4) + i] != loraWan->MIC[i])
			{
				return 4;
			}
		}

		*len_data_out = len_data_in;

		for(uint8_t i = 0; i < 8; i++)
		{
			data_out[i] = data_in[i];
		}

		data_out[8+len_opt] = data_in[8+len_opt];

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[len_data_in-4+i] = loraWan->MIC[i];
		}

		if( (loraWan->FCtrl & (1 << ACK)) )
		{
			// message потверждения.
		}


		uint8_t len_frmpayload = 0;
		len_frmpayload = /*MHDR*/1+ /*devAddr*/4+ /*fctrl*/1+ /*fcnt*/2+ /*len_fopts*/ len_opt+ /*fport*/1+ /*MIC*/4;
		len_frmpayload = len_data_in - len_frmpayload;

		infoFrmPayload->biginByte = 9+len_opt;
		infoFrmPayload->LengthFrmPayload = len_frmpayload;

		if(len_frmpayload != 0)
		{

			uint8_t in_frmpayload[len_frmpayload], out_frmpayload[len_frmpayload];

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				// 	0		1..4	5		6..7	8..		8+FL	9+FL		FL - 0..15
				// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload
				in_frmpayload[i] = data_in[9+len_opt+i];
			}

			s = MacFramePayloadEncryption_FRMPayload(loraWan, in_frmpayload, len_frmpayload, out_frmpayload, WRITE);
			if(s == 0)
			{
				return 5;
			}

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				data_out[9+len_opt+i] = out_frmpayload[i];
			}
			// frm payload data yes
			// ...
		}

		if(len_opt != 0)
		{
			uint8_t fopts[16];
			for(uint8_t i = 0; i < len_opt; i++)
			{
				loraWan->FOpts[i] = data_in[8+i];
			}

			s = MacFOpts(loraWan, fopts, &len_opt, READ);
			if(s == 0)
			{
				return 6;
			}

			for(uint8_t i = 0; i < len_opt; i++)
			{
				data_out[i+8] = loraWan->FOpts[i];
			}
			// recoding
		}



		if(loraWan->Register.FPort == 0)
		{
			// the mac comands
		}


		return 7;

		//////////////////////////////////

	}
	else
	{
		return 8;
	}
	#endif

	return 9;
}

uint8_t readMacConfirmedDataUp(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		// struct loraWan config
		loraWan->Register.Dir = 00;
		// ...

		uint8_t len_opt = 0;
		uint8_t s = 0;
		uint8_t MType = data_in[0];
		MType &= ~0b00011111;

		if(MType != Confirmed_Data_Up)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[1+i] != loraWan->DevAddr[3-i])
			{
				return 0;
			}
		}

		loraWan->FCtrl = data_in[5];

		loraWan->FCnt.FCnt[1] = data_in[6];
		loraWan->FCnt.FCnt[0] = data_in[7];

		len_opt = data_in[5];
		len_opt &= ~0b11110000;

		loraWan->Register.FPort = data_in[8+len_opt];

		if(loraWan->Register.FPort == 0 && len_opt != 0)
		{
			return 0;
		}

		s = MacMICUplinkFrames(loraWan, data_in, len_data_in - 4, READ);
		if(s != 1)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[(len_data_in - 4) + i] != loraWan->MIC[i])
			{
				return 0;
			}
		}

		*len_data_out = len_data_in;

		for(uint8_t i = 0; i < 8; i++)
		{
			data_out[i] = data_in[i];
		}

		data_out[8+len_opt] = data_in[8+len_opt];

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[len_data_in-4+i] = loraWan->MIC[i];
		}

		if( (loraWan->FCtrl & (1 << ACK)) )
		{
			// message потверждения.
		}


		uint8_t len_frmpayload = 0;
		len_frmpayload = /*MHDR*/1+ /*devAddr*/4+ /*fctrl*/1+ /*fcnt*/2+ /*len_fopts*/ len_opt+ /*fport*/1+ /*MIC*/4;
		len_frmpayload = len_data_in - len_frmpayload;

		infoFrmPayload->biginByte = 9+len_opt;
		infoFrmPayload->LengthFrmPayload = len_frmpayload;

		if(len_frmpayload != 0)
		{

			uint8_t in_frmpayload[len_frmpayload], out_frmpayload[len_frmpayload];

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
				// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload
				in_frmpayload[i] = data_in[9+len_opt];
			}

			s = MacFramePayloadEncryption_FRMPayload(loraWan, in_frmpayload, len_frmpayload, out_frmpayload, WRITE);
			if(s == 0)
			{
				return 0;
			}

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				data_out[9+len_opt+i] = out_frmpayload[i];
			}
			// frm payload data yes
			// ...
		}

		if(len_opt != 0)
		{
			uint8_t fopts[16];
			for(uint8_t i = 0; i < len_opt; i++)
			{
				loraWan->FOpts[i] = data_in[8+i];
			}

			s = MacFOpts(loraWan, fopts, &len_opt, READ);
			if(s == 0)
			{
				return 0;
			}

			for(uint8_t i = 0; i < len_opt; i++)
			{
				data_out[i+8] = loraWan->FOpts[i];
			}
			// recoding
		}



		if(loraWan->Register.FPort == 0)
		{
			// the mac comands
		}


		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}
#endif

#endif
#if downLink

#if LoRaDevice == LoRaDevice_Gateway || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
uint8_t writeMacUnconfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * FrmPayload_data_in, uint8_t  len_FrmPayload_data_in,
		uint8_t *data_out, uint8_t * len_data_out)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		uint8_t s, len_opt;

		// struct loraWan config
		loraWan->Register.Dir = 0x01;
		// ...

		len_opt = loraWan->FCtrl;
		len_opt &= ~0b11110000;

		data_out[0] = loraWan->Register.MHDR;
		data_out[0] &= ~0b11100000;
		data_out[0] |= Unconfirmed_Data_Down;

		data_out[1] = loraWan->DevAddr[3];
		data_out[2] = loraWan->DevAddr[2];
		data_out[3] = loraWan->DevAddr[1];
		data_out[4] = loraWan->DevAddr[0];

		data_out[5] = loraWan->FCtrl;

		if(loraWan->Register.FPort == 0)
		{
			data_out[6] = loraWan->FCnt.NFCntDwn[1];
			data_out[7] = loraWan->FCnt.NFCntDwn[0];
		}
		else
		{
			data_out[6] = loraWan->FCnt.AFCntDwn[1];
			data_out[7] = loraWan->FCnt.AFCntDwn[0];
		}

		if(loraWan->Register.FPort == 0x00)
		{
			len_opt = 0x00;
			data_out[5] &= ~0b00001111;
		}
		else
		{
			// len opt
			if(len_opt != 0)
			{
				uint8_t fopts[16];
				s = MacFOpts(loraWan,fopts, &len_opt, WRITE);
				if(s == 0)
				{
					return 0;
				}
				for(uint8_t i = 0; i < len_opt; i++)
				{
					data_out[8+i] = fopts[i];
				}
			}
		}


		data_out[8+len_opt] = loraWan->Register.FPort;

		uint8_t pld[len_FrmPayload_data_in];
		s = MacFramePayloadEncryption_FRMPayload(loraWan, FrmPayload_data_in, len_FrmPayload_data_in, pld, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < len_FrmPayload_data_in; i++)
		{
			data_out[9+len_opt+i] = pld[i];
		}

		*len_data_out = 9 + len_FrmPayload_data_in + len_opt + 4;

		s = MacMICUplinkFrames(loraWan, data_out, *len_data_out - 4, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[*len_data_out - 4 + i] = loraWan->MIC[i];
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t writeMacConfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * FrmPayload_data_in, uint8_t  len_FrmPayload_data_in,
		uint8_t *data_out, uint8_t * len_data_out)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		uint8_t s, len_opt;

		// struct loraWan config
		loraWan->Register.Dir = 0x01;
		// ...

		len_opt = loraWan->FCtrl;
		len_opt &= ~0b11110000;

		data_out[0] = loraWan->Register.MHDR;
		data_out[0] &= ~0b11100000;
		data_out[0] |= Confirmed_Data_Down;

		data_out[1] = loraWan->DevAddr[3];
		data_out[2] = loraWan->DevAddr[2];
		data_out[3] = loraWan->DevAddr[1];
		data_out[4] = loraWan->DevAddr[0];

		data_out[5] = loraWan->FCtrl;

		if(loraWan->Register.FPort == 0)
		{
			data_out[6] = loraWan->FCnt.NFCntDwn[1];
			data_out[7] = loraWan->FCnt.NFCntDwn[0];
		}
		else
		{
			data_out[6] = loraWan->FCnt.AFCntDwn[1];
			data_out[7] = loraWan->FCnt.AFCntDwn[0];
		}

		// len opt
		if(len_opt != 0)
		{
			uint8_t fopts[16];
			s = MacFOpts(loraWan,fopts, &len_opt, WRITE);
			if(s == 0)
			{
				return 0;
			}
			for(uint8_t i = 0; i < len_opt; i++)
			{
				data_out[8+i] = fopts[i];
			}
		}

		if(loraWan->Register.FPort == 0x00)
		{
			len_opt = 0x00;
			data_out[5] &= ~0b00001111;
		}

		data_out[8+len_opt] = loraWan->Register.FPort;

		uint8_t pld[len_FrmPayload_data_in];
		s = MacFramePayloadEncryption_FRMPayload(loraWan, FrmPayload_data_in, len_FrmPayload_data_in, pld, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < len_FrmPayload_data_in; i++)
		{
			data_out[9+len_opt+i] = pld[i];
		}

		*len_data_out = 9 + len_FrmPayload_data_in + len_opt + 4;

		s = MacMICUplinkFrames(loraWan, data_out, *len_data_out - 4, WRITE);
		if(s == 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[*len_data_out - 4 + i] = loraWan->MIC[i];
		}

		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}
#endif

#if LoRaDevice == LoRaDevice_EndDevice || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
uint8_t readMacUnconfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		// struct loraWan config
		loraWan->Register.Dir = 0x01;
		// ...

		uint8_t len_opt = 0;
		uint8_t s = 0;
		uint8_t MType = data_in[0];
		MType &= ~ 0b00011111;

		if(MType != Unconfirmed_Data_Down)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[1+i] != loraWan->DevAddr[3-i])
			{
				return 0;
			}
		}

		loraWan->FCtrl = data_in[5];

		loraWan->FCnt.FCnt[1] = data_in[6];
		loraWan->FCnt.FCnt[0] = data_in[7];

		len_opt = data_in[5];
		len_opt &= ~0b11110000;

		loraWan->Register.FPort = data_in[8+len_opt];

		if(loraWan->Register.FPort == 0 && len_opt != 0)
		{
			return 0;
		}

		s = MacMICUplinkFrames(loraWan, data_in, len_data_in - 4, READ);
		if(s != 1)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[(len_data_in - 4) + i] != loraWan->MIC[i])
			{
				return 0;
			}
		}

		*len_data_out = len_data_in;

		for(uint8_t i = 0; i < 8; i++)
		{
			data_out[i] = data_in[i];
		}

		// Fport
		data_out[8+len_opt] = data_in[8+len_opt];

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[len_data_in-4+i] = loraWan->MIC[i];
		}

		if( (loraWan->FCtrl & (1 << ACK)) )
		{
			// message потверждения.
		}


		uint8_t len_frmpayload = 0;
		len_frmpayload = /*MHDR*/1+ /*devAddr*/4+ /*fctrl*/1+ /*fcnt*/2+ /*len_fopts*/ len_opt+ /*fport*/1+ /*MIC*/4;
		len_frmpayload = len_data_in - len_frmpayload;

		infoFrmPayload->biginByte = 9+len_opt;
		infoFrmPayload->LengthFrmPayload = len_frmpayload;

		if(len_frmpayload != 0)
		{

			uint8_t in_frmpayload[len_frmpayload], out_frmpayload[len_frmpayload];

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
				// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload
				in_frmpayload[i] = data_in[9+len_opt+i];
			}

			s = MacFramePayloadEncryption_FRMPayload(loraWan, in_frmpayload, len_frmpayload, out_frmpayload, WRITE);
			if(s == 0)
			{
				return 0;
			}

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				data_out[9+len_opt+i] = out_frmpayload[i];
			}
			// frm payload data yes
			// ...
		}

		if(len_opt != 0)
		{
			uint8_t fopts[16];
			for(uint8_t i = 0; i < len_opt; i++)
			{
				loraWan->FOpts[i] = data_in[8+i];
			}

			s = MacFOpts(loraWan, fopts, &len_opt, READ);
			if(s == 0)
			{
				return 0;
			}

			for(uint8_t i = 0; i < len_opt; i++)
			{
				data_out[i+8] = loraWan->FOpts[i];
			}
			// recoding
		}



		if(loraWan->Register.FPort == 0)
		{
			// the mac comands
		}


		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}

uint8_t readMacConfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload)
{
	#if LoRaWan_1_0 // LoRaWAN_version = 0
	if(loraWan->LoRaWAN_version == 0)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_1 // LoRaWAN_version = 1
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_0_2 // LoRaWAN_version = 2
	if(loraWan->LoRaWAN_version == 1)
	{
		//
	}
	else
	{
		return 0;
	}
	#endif

	#if LoRaWan_1_1 // LoRaWAN_version = 3
	if(loraWan->LoRaWAN_version == 3)
	{
		// MHDR 1 | MACPayload | MIC 4     						5
		// MACPayload = FHDR | FPORT 1 | FRMPayload				1
		// FHDR = DevAddr 4 | FCtrl 1 | FCnt 2 | FOpts 15		22

		// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
		// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload

		// struct loraWan config
		loraWan->Register.Dir = 0x01;
		// ...

		uint8_t len_opt = 0;
		uint8_t s = 0;
		uint8_t MType = data_in[0];
		MType &= ~ 0b00011111;

		if(MType != Confirmed_Data_Down)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[1+i] != loraWan->DevAddr[3-i])
			{
				return 0;
			}
		}

		loraWan->FCtrl = data_in[5];

		loraWan->FCnt.FCnt[1] = data_in[6];
		loraWan->FCnt.FCnt[0] = data_in[7];

		len_opt = data_in[5];
		len_opt &= ~0b11110000;

		loraWan->Register.FPort = data_in[8+len_opt];

		if(loraWan->Register.FPort == 0 && len_opt != 0)
		{
			return 0;
		}

		s = MacMICUplinkFrames(loraWan, data_in, len_data_in - 4, READ);
		if(s != 1)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(data_in[(len_data_in - 4) + i] != loraWan->MIC[i])
			{
				return 0;
			}
		}

		*len_data_out = len_data_in;

		for(uint8_t i = 0; i < 8; i++)
		{
			data_out[i] = data_in[i];
		}

		data_out[8+len_opt] = data_in[8+len_opt];

		for(uint8_t i = 0; i < 4; i++)
		{
			data_out[len_data_in-4+i] = loraWan->MIC[i];
		}

		if( (loraWan->FCtrl & (1 << ACK)) )
		{
			// message потверждения.
		}


		uint8_t len_frmpayload = 0;
		len_frmpayload = /*MHDR*/1+ /*devAddr*/4+ /*fctrl*/1+ /*fcnt*/2+ /*len_fopts*/ len_opt+ /*fport*/1+ /*MIC*/4;
		len_frmpayload = len_data_in - len_frmpayload;

		infoFrmPayload->biginByte = 9+len_opt;
		infoFrmPayload->LengthFrmPayload = len_frmpayload;

		if(len_frmpayload != 0)
		{

			uint8_t in_frmpayload[len_frmpayload], out_frmpayload[len_frmpayload];

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				// 	0		1..4	5		6..7	8+FL	9+FL	10+FL		FL - 0..15
				// MHDR | DevAddr | FCtrl | FCnt | FOpts | FPORT | FRMPayload
				in_frmpayload[i] = data_in[9+len_opt+i];
			}

			s = MacFramePayloadEncryption_FRMPayload(loraWan, in_frmpayload, len_frmpayload, out_frmpayload, WRITE);
			if(s == 0)
			{
				return 0;
			}

			for(uint8_t i = 0; i < len_frmpayload; i++)
			{
				data_out[9+len_opt+i] = out_frmpayload[i];
			}
			// frm payload data yes
			// ...
		}

		if(len_opt != 0)
		{
			uint8_t fopts[16];
			for(uint8_t i = 0; i < len_opt; i++)
			{
				loraWan->FOpts[i] = data_in[8+i];
			}

			s = MacFOpts(loraWan, fopts, &len_opt, READ);
			if(s == 0)
			{
				return 0;
			}

			for(uint8_t i = 0; i < len_opt; i++)
			{
				data_out[i+8] = loraWan->FOpts[i];
			}
			// recoding
		}



		if(loraWan->Register.FPort == 0)
		{
			// the mac comands
		}


		return 1;
	}
	else
	{
		return 0;
	}
	#endif
	return 0;
}
#endif

#endif
/*----------------------------------------------------------------------*/
uint8_t writeMacProprietary(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return 0;
}

uint8_t readMacProprietary(struct LoRaWAN * loraWan, uint8_t * data, uint8_t * len_data)
{
	return 0;
}
/*----------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------*/


#if macCommand

#if LoRaWan_1_0
//
#endif

#if LoRaWan_1_0_1
//
#endif

#if LoRaWan_1_0_2
//
#endif

#if LoRaWan_1_1

#if transform // transformMacComm

uint8_t transformMacCommand(uint8_t comm, uint8_t * arr, uint8_t * len_arr, uint8_t * init)
{
	#if LoRaWAN_SERVER == 0
	switch (comm)
	{
	// EndDevice
		case ResetInd:
			*init = transformMacCommResetInd(arr, len_arr);
			return 1;
			break;

		case LinkCheckReq:
			*init = transformMacCommLinkCheckReq(arr, len_arr);
			return 1;
			break;

		case LinkADRAns:
			*init = transformMacCommLinkADRAns(arr, len_arr);
			return 1;
			break;

		case DutyCycleAns:
			*init = transformMacCommDutyCycleAns(arr, len_arr);
			return 1;
			break;

		case RXParamSetupAns:
			*init = transformMacCommRXParamSetupAns(arr, len_arr);
			return 1;
			break;

		case DevStatusAns:
			*init = transformMacCommDevStatusAns(arr, len_arr);
			return 1;
			break;

		case NewChannelAns:
			*init = transformMacCommNewChannelAns(arr, len_arr);
			return 1;
			break;

		case RXTimingSetupAns:
			*init = transformMacCommRXTimingSetupAns(arr, len_arr);
			return 1;
			break;

		case TxParamSetupAns:
			*init = transformMacCommTxParamSetupAns(arr, len_arr);
			return 1;
			break;

		case DlChannelAns:
			*init = transformMacCommDlChannelAns(arr, len_arr);
			return 1;
			break;

		case RekeyInd:
			*init = transformMacCommRekeyInd(arr, len_arr);
			return 1;
			break;

		case ADRParamSetupAns:
			*init = transformMacCommADRParamSetupAns(arr, len_arr);
			return 1;
			break;

		case DeviceTimeReq:
			*init = transformMacCommDeviceTimeReq(arr, len_arr);
			return 1;
			break;

		case RejoinParamSetupAns:
			*init = transformMacCommRejoinParamSetupAns(arr, len_arr);
			return 1;
			break;

		default:
			return 0;
			break;
	}
	#endif

	#if LoRaWAN_SERVER == 1
	// Gateway
	switch(comm)
	{
		// Gateway
		case ResetConf:
			*init = transformMacCommResetConf(arr, len_arr);
			return 1;
			break;

		case LinkCheckAns:
			*init = transformMacCommLinkCheckAns(arr, len_arr);
			return 1;
			break;

		case LinkADRReq:
			*init = transformMacCommLinkADRReq(arr, len_arr);
			return 1;
			break;

		case DutyCycleReq:
			*init = transformMacCommDutyCycleReq(arr, len_arr);
			return 1;
			break;

		case RXParamSetupReq:
			*init = transformMacCommRXParamSetupReq(arr, len_arr);
			return 1;
			break;

		case DevStatusReq:
			*init = transformMacCommDevStatusReq(arr, len_arr);
			return 1;
			break;

		case NewChannelReq:
			*init = transformMacCommNewChannelReq(arr, len_arr);
			return 1;
			break;

		case RXTimingSetupReq:
			*init = transformMacCommRXTimingSetupReq(arr, len_arr);
			return 1;
			break;

		case TxParamSetupReq:
			*init = transformMacCommTxParamSetupReq(arr, len_arr);
			return 1;
			break;

		case DlChannelReq:
			*init = transformMacCommDlChannelReq(arr, len_arr);
			return 1;
			break;

		case RekeyConf:
			*init = transformMacCommRekeyConf(arr, len_arr);
			return 1;
			break;

		case ADRParamSetupReq:
			*init = transformMacCommADRParamSetupReq(arr, len_arr);
			return 1;
			break;

		case DeviceTimeAns:
			*init = transformMacCommDeviceTimeAns(arr, len_arr);
			return 1;
			break;

		case ForceRejoinReq:
			*init = transformMacCommForceRejoinReq(arr, len_arr);
			return 1;
			break;

		case RejoinParamSetupReq:
			*init = transformMacCommRejoinParamSetupReq(arr, len_arr);
			return 1;
			break;

		default:
			return 0;
			break;
	}
	#endif
	return 0;
}




#if Gateway
uint8_t transformMacCommResetConf(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommLinkCheckAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommLinkADRReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDutyCycleReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRXParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDevStatusReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommNewChannelReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRXTimingSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommTxParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDlChannelReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRekeyConf(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommADRParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDeviceTimeAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommForceRejoinReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRejoinParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
#endif



#if EndDevice
uint8_t transformMacCommResetInd(uint8_t * arr, uint8_t * len_arr)
{
	// Dev LoRaWAN version

	return 0;
}
uint8_t transformMacCommLinkCheckReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommLinkADRAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDutyCycleAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRXParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDevStatusAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommNewChannelAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRXTimingSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommTxParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDlChannelAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRekeyInd(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommADRParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommDeviceTimeReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t transformMacCommRejoinParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
#endif

#endif





#if reform // reformMacComm

uint8_t reformMacCommand(uint8_t comm, uint8_t * arr, uint8_t * len_arr, uint8_t * init)
{
	#if LoRaWAN_SERVER == 1
	switch (comm)
	{
	// EndDevice
		case ResetInd:
			*init = reformMacCommResetInd(arr, len_arr);
			return 1;
			break;

		case LinkCheckReq:
			*init = reformMacCommLinkCheckReq(arr, len_arr);
			return 1;
			break;

		case LinkADRAns:
			*init = reformMacCommLinkADRAns(arr, len_arr);
			return 1;
			break;

		case DutyCycleAns:
			*init = reformMacCommDutyCycleAns(arr, len_arr);
			return 1;
			break;

		case RXParamSetupAns:
			*init = reformMacCommRXParamSetupAns(arr, len_arr);
			return 1;
			break;

		case DevStatusAns:
			*init = reformMacCommDevStatusAns(arr, len_arr);
			return 1;
			break;

		case NewChannelAns:
			*init = reformMacCommNewChannelAns(arr, len_arr);
			return 1;
			break;

		case RXTimingSetupAns:
			*init = reformMacCommRXTimingSetupAns(arr, len_arr);
			return 1;
			break;

		case TxParamSetupAns:
			*init = reformMacCommTxParamSetupAns(arr, len_arr);
			return 1;
			break;

		case DlChannelAns:
			*init = reformMacCommDlChannelAns(arr, len_arr);
			return 1;
			break;

		case RekeyInd:
			*init = reformMacCommRekeyInd(arr, len_arr);
			return 1;
			break;

		case ADRParamSetupAns:
			*init = reformMacCommADRParamSetupAns(arr, len_arr);
			return 1;
			break;

		case DeviceTimeReq:
			*init = reformMacCommDeviceTimeReq(arr, len_arr);
			return 1;
			break;

		case RejoinParamSetupAns:
			*init = reformMacCommRejoinParamSetupAns(arr, len_arr);
			return 1;
			break;

		default:
			return 0;
			break;
	}
	#endif

	#if LoRaWAN_SERVER == 0
	// Gateway
	switch(comm)
	{
		// Gateway
		case ResetConf:
			*init = reformMacCommResetConf(arr, len_arr);
			return 1;
			break;

		case LinkCheckAns:
			*init = reformMacCommLinkCheckAns(arr, len_arr);
			return 1;
			break;

		case LinkADRReq:
			*init = reformMacCommLinkADRReq(arr, len_arr);
			return 1;
			break;

		case DutyCycleReq:
			*init = reformMacCommDutyCycleReq(arr, len_arr);
			return 1;
			break;

		case RXParamSetupReq:
			*init = reformMacCommRXParamSetupReq(arr, len_arr);
			return 1;
			break;

		case DevStatusReq:
			*init = reformMacCommDevStatusReq(arr, len_arr);
			return 1;
			break;

		case NewChannelReq:
			*init = reformMacCommNewChannelReq(arr, len_arr);
			return 1;
			break;

		case RXTimingSetupReq:
			*init = reformMacCommRXTimingSetupReq(arr, len_arr);
			return 1;
			break;

		case TxParamSetupReq:
			*init = reformMacCommTxParamSetupReq(arr, len_arr);
			return 1;
			break;

		case DlChannelReq:
			*init = reformMacCommDlChannelReq(arr, len_arr);
			return 1;
			break;

		case RekeyConf:
			*init = reformMacCommRekeyConf(arr, len_arr);
			return 1;
			break;

		case ADRParamSetupReq:
			*init = reformMacCommADRParamSetupReq(arr, len_arr);
			return 1;
			break;

		case DeviceTimeAns:
			*init = reformMacCommDeviceTimeAns(arr, len_arr);
			return 1;
			break;

		case ForceRejoinReq:
			*init = reformMacCommForceRejoinReq(arr, len_arr);
			return 1;
			break;

		case RejoinParamSetupReq:
			*init = reformMacCommRejoinParamSetupReq(arr, len_arr);
			return 1;
			break;

		default:
			return 0;
			break;
	}
	#endif
	return 0;
}





#if Gateway
uint8_t reformMacCommResetConf(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommLinkCheckAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommLinkADRReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDutyCycleReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRXParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDevStatusReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommNewChannelReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRXTimingSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommTxParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDlChannelReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRekeyConf(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommADRParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDeviceTimeAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommForceRejoinReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRejoinParamSetupReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
#endif




#if EndDevice
uint8_t reformMacCommResetInd(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommLinkCheckReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommLinkADRAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDutyCycleAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRXParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDevStatusAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommNewChannelAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRXTimingSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommTxParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDlChannelAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRekeyInd(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommADRParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommDeviceTimeReq(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
uint8_t reformMacCommRejoinParamSetupAns(uint8_t * arr, uint8_t * len_arr)
{
	return 0;
}
#endif

#endif
#endif

#endif

/*
uint8_t MacPackRead(uint8_t front, uint8_t * array, uint8_t length);
uint8_t MacPackWrite(uint8_t front, uint8_t Mtype, uint8_t * array, uint8_t length);
uint8_t PackCommandStm(uint8_t * array);
*/
uint8_t MacPackRead(uint8_t front, uint8_t * array, uint8_t length)
{
	return 0;
}


uint8_t MacPackWrite(uint8_t front, uint8_t Mtype, uint8_t * array, uint8_t length)
{
	return 0;
}

uint8_t PackCommandStm(uint8_t * array)
{
	uint8_t length = 0x00;
	length |= array[1];
	length |= (uint16_t)(array[0] << 8);

	if(array[2] == 0x00 && array[3] == 0x00)
	{
		//
	}
	return 0;
}

