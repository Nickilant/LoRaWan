#include "ufaga/protocol_ufaga.h"
#ifdef device
#if device == LSI
/*------------------------------------------------------------------------------------------------*/
//0x81 0x00
uint8_t transformSendPreparedData(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1

	if(ufaga->pack.length > ufaga->pack.n)
	{
		ufaga->pack.count_block = ufaga->pack.length / ufaga->pack.n;
		if((ufaga->pack.count_block * ufaga->pack.n) < ufaga->pack.length)
		{
			ufaga->pack.count_block++;
		}

		uint8_t nb_bl = 0x00;
		if(ufaga->pack.number_block != 0)
		{
			nb_bl = ufaga->pack.number_block - 1;
		}
		else
		{
			nb_bl = ufaga->pack.number_block;
		}
		ufaga->pack.length_block = ufaga->pack.number_block != ufaga->pack.count_block ? ufaga->pack.n : \
				( ufaga->pack.length-(ufaga->pack.n*(nb_bl)) );

		msg[0] = ufaga->Ecnt[1];
		msg[1] = ufaga->Ecnt[0];

		msg[2] = 0x81;
		msg[3] = 0x00;

		msg[4] = ufaga->pack.count_block;

		msg[5] = ufaga->pack.number_block;

		msg[6] = ufaga->pack.length_block;


		uint8_t j = ufaga->pack.n*nb_bl;
		for(uint8_t i = 0; i < ufaga->pack.length_block; i++)
		{
			msg[7+i] = ufaga->pack.flash_data[i+j];
		}

		*len_msg = 7+ufaga->pack.length_block;

		return 1;
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformSetHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformPrepareDataTimeAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg, uint8_t coercion)
{
	#if version_device == version_1

	msg[0] = ufaga->Ecnt[1];
	msg[1] = ufaga->Ecnt[0];

	msg[2] = 0x81;
	msg[3] = 0x02;

	msg[7] = ufaga->datetime.sec;
	msg[8] = ufaga->datetime.min;
	msg[9] = ufaga->datetime.hour;
	msg[10] = ufaga->datetime.day;
	msg[11] = ufaga->datetime.month;
	msg[12] = ufaga->datetime.year;

	if(coercion == YES)
	{
		if(13 > ufaga->pack.n)
		{
			// не доделано
		}
		else
		{
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x06;
			*len_msg = 13;
		}

		return 1;
	}
	else
	{
		msg[6] = 0x01;
		msg[7] = 1;
		*len_msg = 8;
		ufaga->pack.length = 6;
		ufaga->pack.flash_data[0] = ufaga->datetime.sec;
		ufaga->pack.flash_data[1] = ufaga->datetime.min;
		ufaga->pack.flash_data[2] = ufaga->datetime.hour;
		ufaga->pack.flash_data[3] = ufaga->datetime.day;
		ufaga->pack.flash_data[4] = ufaga->datetime.month;
		ufaga->pack.flash_data[5] = ufaga->datetime.year;

		return 1;
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformNullifyHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformPrepareDataBatteryCharge(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg, uint8_t coercion)
{
	#if version_device == version_1

	msg[0] = ufaga->Ecnt[1];
	msg[1] = ufaga->Ecnt[0];

	msg[2] = 0x81;
	msg[3] = 0x04;

	if(coercion == YES)
	{

		if(8 > ufaga->pack.n)
		{
			// не доделано
		}
		else
		{
			msg[4] = 0x00;
			msg[5] = 0x00;
			msg[6] = 0x01;
			*len_msg = 1+7;
		}

		msg[7] = ufaga->baterry.state;

		return 1;
	}
	else
	{
		msg[6] = 0x01;
		*len_msg = 8;
		ufaga->pack.length = 1;
		ufaga->pack.flash_data[0] = ufaga->baterry.state;

		return 1;
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformPrepareImpulseLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg, uint8_t coercion)
{
	#if version_device == version_1

	msg[0] = ufaga->Ecnt[1];
	msg[1] = ufaga->Ecnt[0];

	msg[2] = 0x81;
	msg[3] = 0x05;

	msg[7] = ufaga->ufaga_impulse.count_impulse;
	for(uint8_t i = 0; i < 4; i++)
	{
		msg[8+i] = ufaga->ufaga_impulse.impulse1[3-i];
		msg[12+i] = ufaga->ufaga_impulse.impulse2[3-i];
		msg[16+i] = ufaga->ufaga_impulse.impulse3[3-i];
		msg[20+i] = ufaga->ufaga_impulse.impulse4[3-i];
	}

	if(24 > ufaga->pack.n && coercion == YES)
	{
		// не доделано
		return 2;
	}
	else
	{
		msg[4] = 0x00;
		msg[5] = 0x00;
		*len_msg = 23;
	}

	if(coercion == YES)
	{
		msg[6] = 17;
		return 1;
	}
	else
	{
		msg[6] = 0x01;
		msg[7] = 1;
		*len_msg = 8;

		ufaga->pack.length = 17;
		ufaga->pack.flash_data[0] = ufaga->ufaga_impulse.count_impulse;
		for(uint8_t i = 0; i < 4; i++)
		{
			ufaga->pack.flash_data[1+i] = ufaga->ufaga_impulse.impulse1[3-i];
			ufaga->pack.flash_data[5+i] = ufaga->ufaga_impulse.impulse2[3-i];
			ufaga->pack.flash_data[9+i] = ufaga->ufaga_impulse.impulse3[3-i];
			ufaga->pack.flash_data[13+i] = ufaga->ufaga_impulse.impulse4[3-i];
		}

		return 1;
	}


	#endif
	return 0;
}

/*------------------------------------------------------------------------------------------------*/
uint8_t transformSmoothDataImulsnyLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformSmoothDataImulsnyRewriting(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t transformPrepareDeviceDetails(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg, uint8_t coercion)
{
	#if version_device == version_1

	msg[0] = ufaga->Ecnt[1];
	msg[1] = ufaga->Ecnt[0];

	msg[2] = 0x81;
	msg[3] = 0x08;

	msg[7] = ufaga->baterry.state;
	msg[8] = ufaga->ufaga_impulse.count_impulse;
	for(uint8_t i = 0; i < 4; i++)
	{
		msg[9+i] = ufaga->ufaga_impulse.impulse1[3-i];
		msg[13+i] = ufaga->ufaga_impulse.impulse2[3-i];
		msg[17+i] = ufaga->ufaga_impulse.impulse3[3-i];
		msg[21+i] = ufaga->ufaga_impulse.impulse4[3-i];
	}

	if(25 > ufaga->pack.n && coercion == YES)
	{
		// не доделано
		return 2;
	}
	else
	{
		msg[4] = 0x00;
		msg[5] = 0x00;
		*len_msg = 17+7;
	}

	if(coercion == YES)
	{
		msg[6] = 0x11; // 17

		return 1;
	}
	else
	{
		msg[6] = 0x01;
		msg[7] = 1;
		*len_msg = 8;

		ufaga->pack.length = 18;
		ufaga->pack.flash_data[0] = ufaga->baterry.state;
		ufaga->pack.flash_data[1] = ufaga->ufaga_impulse.count_impulse;
		for(uint8_t i = 0; i < 4; i++)
		{
			ufaga->pack.flash_data[2+i] = ufaga->ufaga_impulse.impulse1[3-i];
			ufaga->pack.flash_data[6+i] = ufaga->ufaga_impulse.impulse2[3-i];
			ufaga->pack.flash_data[10+i] = ufaga->ufaga_impulse.impulse3[3-i];
			ufaga->pack.flash_data[14+i] = ufaga->ufaga_impulse.impulse4[3-i];
		}

		return 1;
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
uint8_t reformSendPreparedData(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformSetHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t len_msg)
{
	#if version_device == version_1
	// 0..1 count | 2..3 command | 4 count block | 5 number block | 6 len block | 7..N data

	if(len_msg != 13)
	{
		return 0;
	}

	if( (msg[2] != 0x81 || msg[2] != 0x01) && msg[3] != 0x01)
	{
		return 0;
	}

	if(msg[4] == 0)
	{
		if(msg[6] != 6)
		{
			return 0;
		}

		ufaga->datetime.sec = msg[7];
		ufaga->datetime.min = msg[8];
		ufaga->datetime.hour = msg[9];
		ufaga->datetime.day = msg[10];
		ufaga->datetime.month = msg[11];
		ufaga->datetime.year = msg[12];

		return 1;
	}
	else
	{
		// n block, save
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformPrepareDataTimeAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformNullifyHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t len_msg)
{
	#if version_device == version_1

	if(len_msg != 7)
	{
		return 0;
	}

	if( (msg[2] != 0x81 || msg[2] != 0x01) && msg[3] != 0x03)
	{
		return 0;
	}

	if(msg[4] == 0)
	{
		if(msg[6] != 0)
		{
			return 0;
		}

		ufaga->datetime.sec = 0;
		ufaga->datetime.min = 0;
		ufaga->datetime.hour = 0;
		ufaga->datetime.day = 0;
		ufaga->datetime.month = 0;
		ufaga->datetime.year = 0;
		return 1;
	}
	else
	{
		// n block, save
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformPrepareDataBatteryCharge(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformPrepareImpulseLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformSmoothDataImulsnyLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t len_msg)
{
	#if version_device == version_1

	if(len_msg != 7)
	{
		return 0;
	}

	if( (msg[2] != 0x81 || msg[2] != 0x01) && msg[3] != 0x03)
	{
		return 0;
	}

	if(msg[4] == 0)
	{
		if(msg[6] != 0)
		{
			return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			ufaga->ufaga_impulse.impulse1[i] = 0x00;
			ufaga->ufaga_impulse.impulse2[i] = 0x00;
			ufaga->ufaga_impulse.impulse3[i] = 0x00;
			ufaga->ufaga_impulse.impulse4[i] = 0x00;
		}

		return 1;
	}
	else
	{
		// n block, save
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformSmoothDataImulsnyRewriting(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformPrepareDeviceDetails(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
#endif
#endif

#ifdef LBS
/*------------------------------------------------------------------------------------------------*/
uint8_t transformSendPreparedDataGateway(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t reformSendPreparedDataGateway(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg)
{
	#if version_device == version_1
	//
	#endif
	return 0;
}
#endif
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
uint8_t ReadUfaga(struct Ufaga * ufaga, uint8_t * msg_in, uint8_t len_msg_in)
{
	#if version_device == version_1

	uint8_t s = 0x00;
	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x00 || ufaga->command[1] == 0x80))
	{
		//
		return 1;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && ufaga->command[1] == 0x01)
	{

		s = reformSetHoursAndDate(ufaga, msg_in, len_msg_in);
		if(s == 0)
		{
			return 0;
		}
		return 1;

		return 0;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x02 || ufaga->command[1] == 0x82))
	{
		if(ufaga->command[1] == 0x02)
		{
			s = transformPrepareDataTimeAndDate(ufaga, NULL, NULL, NO);
			if(s == 0)
			{
				return 0;
			}

			return 1;
		}

		if(ufaga->command[1] == 0x82)
		{
			return 1;
		}
		return 0;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && ufaga->command[1] == 0x03)
	{
		s = reformNullifyHoursAndDate(ufaga, msg_in, len_msg_in);
		if(s == 0)
		{
			return 0;
		}
		return 1;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x04 || ufaga->command[1] == 0x84))
	{
		if(ufaga->command[1] == 0x04)
		{
			s = transformPrepareDataBatteryCharge(ufaga, NULL, NULL, NO);
			if(s == 0)
			{
				return 0;
			}

			return 1;
		}

		if(ufaga->command[1] == 0x84)
		{
			return 1;
		}
		return 0;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x05 || ufaga->command[1] == 0x85))
	{
		if(ufaga->command[1] == 0x05)
		{
			s = transformPrepareImpulseLines(ufaga, NULL, NULL, NO);
			if(s == 0)
			{
				return 0;
			}

			return 1;
		}

		if(ufaga->command[1] == 0x85)
		{
			return 1;
		}
		return 0;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x06 || ufaga->command[1] == 0x86))
	{
		s = reformSmoothDataImulsnyLines(ufaga, msg_in, len_msg_in);
		if(s == 0)
		{
			return 0;
		}
		return 1;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x07 || ufaga->command[1] == 0x87))
	{
		//
		return 0;
	}

	/*----------------------------------------------------------------------*/
	if(ufaga->command[0] == 0x01 && (ufaga->command[1] == 0x08 || ufaga->command[1] == 0x88))
	{
		if(ufaga->command[1] == 0x08)
		{
			s = transformPrepareDeviceDetails(ufaga, NULL, NULL, NO);
			if(s == 0)
			{
				return 0;
			}
			return 1;
		}
		if(ufaga->command[1] == 0x88)
		{
			return 1;
		}
		return 0;
	}

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/
uint8_t WriteUfaga(struct Ufaga * ufaga, uint8_t * msg_out, uint8_t * len_msg_out)
{
	#if version_device == version_1

	uint8_t s = 0x00;

	/*----------------------------------------------------------------------*/
	if((ufaga->command[0] == 0x01 || ufaga->command[0] == 0x81) && ufaga->command[1] == 0x00)
	{

		s = transformSendPreparedData(ufaga, msg_out, len_msg_out);
		if(s == 0)
		{
			return 0;
		}
		return 1;
	}

	/*----------------------------------------------------------------------*/
	if((ufaga->command[0] == 0x01 || ufaga->command[0] == 0x81) && (ufaga->command[1] == 0x82 || ufaga->command[1] == 0x02))
	{

		// count pack++
		if(ufaga->command[1] == 0x82)
		{
			s = transformPrepareDataTimeAndDate(ufaga, msg_out, len_msg_out, YES);
		}

		if(ufaga->command[1] == 0x02)
		{
			s = transformPrepareDataTimeAndDate(ufaga, msg_out, len_msg_out, NO);
		}
		if(s == 0)
		{
			// count pack--
			return 0;
		}

		return 1;
	}

	/*----------------------------------------------------------------------*/
	if((ufaga->command[0] == 0x01 || ufaga->command[0] == 0x81) && (ufaga->command[1] == 0x84 || ufaga->command[1] == 0x04))
	{

		if(ufaga->command[1] == 0x84)
		{
			s = transformPrepareDataBatteryCharge(ufaga, msg_out, len_msg_out, YES);
		}

		if(ufaga->command[1] == 0x04)
		{
			s = transformPrepareDataBatteryCharge(ufaga, msg_out, len_msg_out, NO);
		}
		if(s == 0)
		{
			return 0;
		}

		return 1;
	}

	/*----------------------------------------------------------------------*/
	if((ufaga->command[0] == 0x01 || ufaga->command[0] == 0x81) && (ufaga->command[1] == 0x85 || ufaga->command[1] == 0x05))
	{

		if(ufaga->command[1] == 0x85)
		{
			s = transformPrepareImpulseLines(ufaga, msg_out, len_msg_out, YES);
		}

		if(ufaga->command[1] == 0x05)
		{
			s = transformPrepareImpulseLines(ufaga, msg_out, len_msg_out, NO);
		}

		if(s == 0)
		{
			return 0;
		}

		return 1;
	}

	/*----------------------------------------------------------------------*/
	if((ufaga->command[0] == 0x01 || ufaga->command[0] == 0x81) && (ufaga->command[1] == 0x88 || ufaga->command[1] == 0x08))
	{

		if(ufaga->command[1] == 0x88)
		{
			s = transformPrepareDeviceDetails(ufaga, msg_out, len_msg_out, YES);
		}

		if(ufaga->command[1] == 0x08 )
		{
			s = transformPrepareDeviceDetails(ufaga, msg_out, len_msg_out, NO);
		}

		if(s == 0)
		{
			return 0;
		}
		// if(fopt_len == 0) n = M-7 (при M=59); else n = M-(7+fopt_len) (при M=59);
		return 1;
	}

	/*----------------------------------------------------------------------*/
	if((ufaga->command[0] == 0x00 || ufaga->command[0] == 0x80) && ufaga->command[1] == 0x00)
	{
		//
	}
	/*----------------------------------------------------------------------*/

	#endif
	return 0;
}
/*------------------------------------------------------------------------------------------------*/








