#include "main.h"

ADC_HandleTypeDef hadc;
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart2;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM6_Init(void);
uint32_t get_ads_value();
void refactor_adc_value(uint32_t adc_value);

int main(void)
{
	HAL_Init();

	sysCl = HIGH;
	SystemClock_Config();

	HAL_Delay(100);
	MX_GPIO_Init();
	MX_SPI1_Init();
	MX_USART2_UART_Init();
	MX_RTC_Init();
	MX_ADC_Init();
	//HAL_ADC_DeInit(&hadc);
	MX_TIM6_Init();
	HAL_TIM_Base_Start_IT(&htim6);

	DIOD_OFF;
	SxNSS_higt;

	Sx_On;
	HAL_Delay(100);
	sxInit();

	sxInit();
	sxCrc(true);
	sxFRF(864700000, 32000000);
	sxBandwidth(125);
	sxSpreadingFactor(7);
	sxCodingRate(5);
	sxOpMode(MODE_SLEEP);

	//sxStatusInitVoid();

	initProfileLRWAN(&LRWAN);

	HAL_Delay(100);
	time.TimeFormat = RTC_HOURFORMAT12_PM;
	HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BCD);
	//char str[9] = {0};
	uint32_t adc_result;
	impCount1=0;
	impCount2=0;
	impCount3=0;
	impCount4=0;

	while (1)
	{
		HAL_Delay(5000);
		// OTAA LoraWan version 1.1

		uint8_t msgReqest[32] = {0};


		uint8_t count_read_flags_tx;
		uint8_t pack[256], length,error,state;

		writeMacJoinReques(&LRWAN, pack, &length);
		debugHex(msgReqest >> 24);
		debugHex(msgReqest >> 16);
		debugHex(msgReqest >> 8);
		debugHex(msgReqest);
		state = modeTx(pack,length,&error);
		if(error != 0 && state == 0)
					{
						// errors
						debugRn();
						debugString(6,(uint8_t*)"Error ");
						debugHex(error);
						debugString(13,(uint8_t*)", Byte state ");
						debugHex(state);
					}
					else
					{
						count_read_flags_tx = 0;
						while(1)
						{
							count_read_flags_tx++;

							state = readByteSpiSx(RegIrqFlags, &error);
							if(error == 0)
							{
								debugRn();
								debugString(9,(uint8_t*)"Error spi");
							}

							if( (state & (1 << 3)) )
							{
								debugRn();
								debugString(16, (uint8_t*)"Flags TxDoneMask");
								break;
							}

							if(count_read_flags_tx > 300)
							{
								debugRn();
								debugString(16,(uint8_t*)"No flag transmin");
								break;
							}
						}
					}
				HAL_Delay(10000);
			continue;
		/*{





			HAL_ADC_Stop(&hadc);
			if(	HAL_ADC_Start(&hadc) == HAL_ERROR){

				debugString(2,(uint8_t*)"ok");

			}
			HAL_ADC_PollForConversion(&hadc,5000);
			if(HAL_ADC_PollForConversion(&hadc,10000) == HAL_OK){
				debugRn();
				debugString(5,(uint8_t*)"HalOk");
				debugRn();
				adc = HAL_ADC_GetValue(&hadc);
				HAL_ADC_Stop(&hadc);
				debugString(5,(uint8_t*)"adcon");
				debugRn();
				debugHex(adc >> 24);
				debugHex(adc >> 16);
				debugHex(adc >> 8);
				debugHex(adc);
				debugRn();
				debugString(5,(uint8_t*)"adcof");



			}
				if(HAL_ADC_PollForConversion(&hadc,100) == HAL_OK)
				{



				}
		}*/
		adc_result = get_ads_value();
		continue;





		HAL_Delay(10000);

		{
			uint8_t byte_state = 0,
					byte_error = 0,
					count_read_flags_tx;

			// ***** transmit ********************************************

			uint8_t array[20], data[100], data_len;
			array[0] = '2';
			array[1] = '2';
			array[2] = '2';
			array[3] = '2';

			for(uint8_t i = 0; i < 4; i++)
			{
				array[4+i] = (uint32_t)impCount1 >> (8*i);
				array[8+i] = (uint32_t)impCount2 >> (8*i);
				array[12+i] = (uint32_t)impCount3 >> (8*i);
				array[16+i] = (uint32_t)impCount4 >> (8*i);
			}


			// len 16

			uint8_t bs;
			LRWAN.FCtrl = 0;
			LRWAN.Register.FPort = 0x01;
			LRWAN.Register.MHDR = 0x00;
			bs = writeMacUnconfirmedDataUp(&LRWAN, array, 20, data, &data_len);

			debugRn();
			for(uint8_t i = 0; i < data_len; i++)
			{

				debugHex(data[i]);
				debugString(2,(uint8_t*)", ");
			}
			debugRn();

			debugHex(bs);

			//LRWAN.FCtrl = 0;
			//LRWAN.Register.FPort = 0x01;
			//LRWAN.Register.MHDR =0x00;
			struct FRMPayloadInfo a;
			uint8_t wqq[100], m;
			bs=readMacUnconfirmedDataUp(&LRWAN, data, data_len, wqq, &m, &a);

			debugRn();
			debugHex(a.biginByte);
			debugRn();
			debugHex(a.LengthFrmPayload);
			debugRn();
			for(uint8_t i = 0; i < m; i++)
			{

				debugHex(wqq[i]);
				debugString(2,(uint8_t*)", ");
			}
			debugRn();


			byte_state = modeTx(data, data_len, &byte_error);

			if(byte_error != 0 && byte_state == 0)
			{
				// errors
				debugRn();
				debugString(6,(uint8_t*)"Error ");
				debugHex(byte_error);
				debugString(13,(uint8_t*)", Byte state ");
				debugHex(byte_state);
			}
			else
			{
				count_read_flags_tx = 0;
				while(1)
				{
					count_read_flags_tx++;

					byte_state = readByteSpiSx(RegIrqFlags, &byte_error);
					if(byte_error == 0)
					{
						debugRn();
						debugString(9,(uint8_t*)"Error spi");
					}

					if( (byte_state & (1 << 3)) )
					{
						debugRn();
						debugString(16, (uint8_t*)"Flags TxDoneMask");
						break;
					}

					if(count_read_flags_tx > 300)
					{
						debugRn();
						debugString(16,(uint8_t*)"No flag transmin");
						break;
					}
				}
			}
		}
		// ***********************************************************

		 continue;
		// ***********************************************************
		// receive
		uint8_t count_error_read_rtc,
				seconds,
				count_sec;

		count_error_read_rtc = 0;
		count_sec = 0;

		while(1)
		{

			//if(HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD) == HAL_OK)
			{
				//HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
				uint8_t sec;
				uint32_t tmpreg;
				tmpreg = (uint32_t)(hrtc.Instance->TR & RTC_TR_RESERVED_MASK);
				sec = (uint8_t)(tmpreg & (RTC_TR_ST | RTC_TR_SU));
				count_error_read_rtc = 0;
				//sec = time.Seconds;
				if(seconds != sec)
				{
					seconds = sec;
					count_sec++;
					debugRn();
					debugString(17, (uint8_t*)"Timeout seconds: ");
					debugHex(count_sec);
				}
				//debugString(1,"1");
			}
			//else
			{
				//count_error_read_rtc++;
			}

			if(count_sec > 30)
			{
				debugRn();
				debugString(14, (uint8_t*)"Windiws RX end");
				break;
			}

			if(count_error_read_rtc > 50)
			{
				debugRn();
				debugString(15, (uint8_t*)"Count error RTC");
				break;
			}

			////////////////////////////////////////////////////////////////
			{
				uint8_t byte_error = 0,
						array_pack[100],
						length_pack = 0;

				length_pack = modeRx(array_pack, &byte_error);

				if(byte_error != 0 && byte_error != 4 && byte_error != 5 )
				{

					debugRn();
					debugString(6,(uint8_t*)"Error ");
					debugHex(byte_error);

					debugRn();
					debugString(14, (uint8_t*)"Windiws RX end");
					break;
				}

				if(length_pack != 0)
				{
					debugRn();
					debugString(14, (uint8_t*)"Pack bytes: 0x");
					for(uint8_t i = 0; i < length_pack; i++)
					{
						debugHex(array_pack[i]);
						debugString(1, (uint8_t*)" ");
					}

					debugRn();
					debugString(14, (uint8_t*)"Windiws RX end");
					// reform read
					break;
				}
			}

			////////////////////////////////////////////////////////////////

		}
		//

		// ***********************************************************




		continue;

		//debugString(2,(uint8_t *)"12");
		if(i1 != impCount1)
		{
			i1 = impCount1;
			debugRn();
			debugString(2,(uint8_t *)"1=");
			debugHex(impCount1);
		}

		if(i2 != impCount2)
		{
			i2 = impCount2;
			debugRn();
			debugString(2,(uint8_t *)"2=");
			debugHex(impCount2);
		}

		if(i3 != impCount3)
		{
			i3 = impCount3;
			debugRn();
			debugString(2,(uint8_t *)"3=");
			debugHex(impCount3);
		}

		if(i4 != impCount4)
		{
			i4 = impCount4;
			debugRn();
			debugString(2,(uint8_t *)"4=");
			debugHex(impCount4);
		}
		//HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
		//uint8_t f[1];
		//f[0] = time.Seconds;
		//debugHex(time.Seconds);
		//HAL_UART_Transmit(&huart2,f,1,10000);


	}

}// 0.027mA



uint32_t get_ads_value(){
	uint32_t adc ;
	uint8_t a;
	a = HAL_ADC_PollForConversion(&hadc,100);
	a = HAL_ADC_Start(&hadc);
	debugRn();
	debugHex(a);
	adc = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);
	debugRn();
	debugHex(adc >> 24);
	debugHex(adc >> 16);
	debugHex(adc >> 8);
	debugHex(adc);

	return adc;
}

void refactor_adc_value(uint32_t adc_value){
	// �������� ����������

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  if(sysCl == LOW)
  {
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	  HAL_PWR_EnableBkUpAccess();
	  HAL_PWREx_EnableLowPowerRunMode();
  }
  else
  {
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	  HAL_PWR_EnableBkUpAccess();
  }

  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;

  RCC_OscInitStruct.MSIClockRange = sysCl == LOW? RCC_MSIRANGE_0 : RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_8B;//12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = ENABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
    debugRn();
    debugString(4,(uint8_t*)"init");
  }

  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;

  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
    debugRn();
    debugString(6,(uint8_t*)"chanel");
  }
}





static void MX_RTC_Init(void)
{
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
}

void Init_ADC(void){




}

static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

static void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 2000;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 10;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 2400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DIOD_Pin|NSS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RELE_GPIO_Port, RELE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DIOD_Pin NSS_Pin */
  GPIO_InitStruct.Pin = DIOD_Pin|NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DIO5_Pin DIO3_Pin DIO4_Pin DIO0_Pin 
                           DIO1_Pin DIO2_Pin */
  /*
  GPIO_InitStruct.Pin = DIO5_Pin|DIO3_Pin|DIO4_Pin|DIO0_Pin 
                          |DIO1_Pin|DIO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/

  /*Configure GPIO pin : RELE_Pin */
  GPIO_InitStruct.Pin = RELE_Pin | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IMP1_Pin IMP2_Pin IMP3_Pin IMP4_Pin */
  GPIO_InitStruct.Pin = IMP1_Pin|IMP2_Pin|IMP3_Pin|IMP4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}



/*------------------------------------------------------------------------------------*/
#ifdef debugOn
/*------------------------------------------------------------------------------------*/
void charToHex(uint8_t hex, uint8_t* array_hex/*length = 2*/)
{
#if debugOn
	array_hex[0] = 0x00;
	array_hex[1] = 0x00;

	if ( ((hex & 0b11110000) >> 4) <= 0x09 )
	{
		array_hex[0] = (char)(((hex & 0b11110000) >> 4) + 0x30);
	}
	else
	{
		array_hex[0] = (char)(((hex & 0b11110000) >> 4) + 0x37);
	}
	if ( (hex & 0b00001111) <= 0x09 )
	{
		array_hex[1] = (char)(((hex & 0b00001111) ) + 0x30);
	}
	else
	{
		array_hex[1] = (char)(((hex & 0b00001111) ) + 0x37);
	}
#endif
}
/*------------------------------------------------------------------------------------*/

void debugRn(void)
{
#if debugOn
	uint8_t uart_array[2];
	uart_array[0]=0x0a;
	uart_array[1]=0x0d;

#if IT == 1
	if(HAL_UART_Transmit_IT(&huart2, uart_array, 2) == HAL_OK)
	{
		//
	}
#else
	if(HAL_UART_Transmit(&huart2, uart_array, 2, timeOutUart) == HAL_OK)
	{
		//
	}
#endif
	else
	{
		// return
	}
#endif
}
/*------------------------------------------------------------------------------------*/

void debugHex(uint8_t hex_byte)
{
#if debugOn
	uint8_t array_hex[2];
	charToHex(hex_byte, array_hex/*length = 2*/);
#if IT == 1
	if(HAL_UART_Transmit_IT(&huart2, array_hex, 2) == HAL_OK)
	{
		//
	}
#else
	if(HAL_UART_Transmit(&huart2, array_hex, 2, timeOutUart) == HAL_OK)
	{
		//
	}
#endif
	else
	{
		// return
	}
#endif
}
/*------------------------------------------------------------------------------------*/

void debugString(uint16_t length, uint8_t * array_byte)
{
#if debugOn
#if IT == 1
	if(HAL_UART_Transmit_IT(&huart2, array_byte, length) == HAL_OK)
	{
		//
	}
#else
	if(HAL_UART_Transmit(&huart2, array_byte, length, timeOutUart) == HAL_OK)
	{
		//
	}
#endif
	else
	{
		// return
	}
#endif
}
/*------------------------------------------------------------------------------------*/
#endif

/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/
void writeByteSpiSx(uint8_t register_byte, uint8_t data_byte, uint8_t * status_spi)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET ); // NSS1 low
	//SxNSS_low;

	register_byte |= (1<<7);
	uint8_t tx[2]={register_byte, data_byte};
#if IT == 1
	if(HAL_SPI_Transmit_IT(&hspi1, tx, 2) != HAL_OK)
#else
	if(HAL_SPI_Transmit(&hspi1, tx, 2, timeOutSpi) != HAL_OK)
#endif
	{
		*status_spi = 0;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET ); // NSS1 high
		//return;
		//SxNSS_higt;
	}
	else
	{
		*status_spi = 1;
	}

	//SxNSS_higt;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET ); // NSS1 high

	// DEBUG INFO
	#if debugOn
	#if debugSPI
	debugRn();
	debugString(9, (uint8_t*)"Write: 0x");
	debugHex(register_byte);
	debugString(10, (uint8_t*)", data: 0x");
	debugHex(data_byte);
	debugRn();
	#endif
	#endif
}
/*------------------------------------------------------------------------------------*/
//
uint8_t readByteSpiSx(uint8_t register_byte, uint8_t * status_spi)
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_4, GPIO_PIN_RESET ); // NSS1 low
	//SxNSS_low;

	register_byte &= ~(1<<7);
	uint8_t tx[2]={register_byte, 0x00};
	uint8_t rx[2]={0x00, 0x00};
#if IT == 1
	if(HAL_SPI_TransmitReceive_IT(&hspi1, tx, rx, 2) != HAL_OK)
#else
	if(HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, timeOutSpi) != HAL_OK)
#endif
	{
		*status_spi = 0;
		//SxNSS_higt;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET ); // NSS1 high
		//return 0;
	}
	else
	{
		*status_spi = 1;
	}

	//SxNSS_higt;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET ); // NSS1 high

	// DEBUG INFO
	#if debugOn
	#if debugSPI
	debugRn();
	debugString(8, (uint8_t*)"Read: 0x");
	debugHex(register_byte);
	debugString(10, (uint8_t*)", data: 0x");
	debugHex(rx[1]);
	debugRn();
	#endif
	#endif
	return rx[1];
}
/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
//
#ifdef Impulse // 134742016 - 134742046
/*------------------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6)
	{
		impulseWhile();
	}
}
/*------------------------------------------------------------------------------------*/
void clearDataOperatioMemoryImpulse(void)
{
	#if Impulse
	// extern impulse
	impCountFlagMod0 = 0;
	impCountFlagMod1 = 0;
	impCountFlagMod2 = 0;
	impCountFlagMod3 = 0;
	impCountFlagMod4 = 0;
	impCountFlagDreb0 = 0;
	impCountFlagDreb1 = 0;
	impCountFlagDreb2 = 0;
	impCountFlagDreb3 = 0;
	impCountFlagDreb4 = 0;
	impCount0 = 0;
	impCount1 = 0;
	impCount2 = 0;
	impCount3 = 0;
	impCount4 = 0;
	#if ImpulseTest
	i0=0;
	i1=0;
	i2=0;
	i3=0;
	i4=0;
	#endif

	#if debugOn
	debugRn();
	debugString(34, (uint8_t *)"Clear data operation MemoryImpulse");
	#endif
	#endif
}
/*------------------------------------------------------------------------------------*/
void impulseWhile(void)
{
	#if Impulse
	// IMP0
	if(impCountFlagMod0 == 0)//
	{
		if(IMP0 == GPIO_PIN_RESET)//GPIO_PIN_RESET
		{
			impCountFlagDreb0++; //
		}
		else
		{
			impCountFlagDreb0 = 0;//
		}

		if(impCountFlagDreb0 >= ImpulseMaxCountDrebesg)//
		{
			impCount0++;
			impCountFlagMod0 = 1;
		}
	}

	if(impCountFlagMod0 == 1)//
	{
		if(IMP0 == GPIO_PIN_SET)// GPIO_PIN_SET
		{
			impCountFlagDreb0++; //
		}
		else
		{
			impCountFlagDreb0 = 0;//
		}

		if(impCountFlagDreb0 >= ImpulseMaxCountDrebesg)//
		{
			impCountFlagMod0 = 0;
		}
	}


	// IMP1
	if(impCountFlagMod1 == 0)//
	{
		if(IMP1 == GPIO_PIN_RESET)//
		{
			impCountFlagDreb1++; //
		}
		else
		{
			impCountFlagDreb1 = 0;//
		}

		if(impCountFlagDreb1 >= ImpulseMaxCountDrebesg)//
		{
			impCount1++;
			impCountFlagMod1 = 1;
		}
	}

	if(impCountFlagMod1 == 1)//
	{
		if(IMP1 == GPIO_PIN_SET)//
		{
			impCountFlagDreb1++; //
		}
		else
		{
			impCountFlagDreb1 = 0;//
		}

		if(impCountFlagDreb1 >= ImpulseMaxCountDrebesg)//
		{
			impCountFlagMod1 = 0;
		}
	}

	// IMP2
	if(impCountFlagMod2 == 0)//
	{
		if(IMP2 == GPIO_PIN_RESET)
		{
			impCountFlagDreb2++; //
		}
		else
		{
			impCountFlagDreb2 = 0;//
		}

		if(impCountFlagDreb2 >= ImpulseMaxCountDrebesg)//
		{
			impCount2++;
			impCountFlagMod2 = 1;
		}
	}

	if(impCountFlagMod2 == 1)//
	{
		if(IMP2 == GPIO_PIN_SET)//
		{
			impCountFlagDreb2++; //
		}
		else
		{
			impCountFlagDreb2 = 0;//
		}

		if(impCountFlagDreb2 >= ImpulseMaxCountDrebesg)//
		{
			impCountFlagMod2 = 0;
		}
	}

	// IMP3
	if(impCountFlagMod3 == 0)//
	{
		if(IMP3 == GPIO_PIN_RESET)//
		{
			impCountFlagDreb3++; //
		}
		else
		{
			impCountFlagDreb3 = 0;//
		}

		if(impCountFlagDreb3 >= ImpulseMaxCountDrebesg)//
		{
			impCount3++;
			impCountFlagMod3 = 1;
		}
	}

	if(impCountFlagMod3 == 1)//
	{

		if(IMP3 == GPIO_PIN_SET)//
		{
			impCountFlagDreb3++; //
		}
		else
		{
			impCountFlagDreb3 = 0;//
		}

		if(impCountFlagDreb3 >= ImpulseMaxCountDrebesg)//
		{
			impCountFlagMod3 = 0;
		}
	}

	// IMP4
	if(impCountFlagMod4 == 0)//
	{
		if(IMP4 == GPIO_PIN_RESET)//
		{
			impCountFlagDreb4++; //
		}
		else
		{
			impCountFlagDreb4 = 0;//
		}

		if(impCountFlagDreb4 >= ImpulseMaxCountDrebesg)//
		{
			impCount4++;
			impCountFlagMod4 = 1;
		}
	}

	if(impCountFlagMod4 == 1)//
	{

		if(IMP4 == GPIO_PIN_SET)//
		{
			impCountFlagDreb4++; //
		}
		else
		{
			impCountFlagDreb4 = 0;//
		}

		if(impCountFlagDreb4 >= ImpulseMaxCountDrebesg)//
		{
			impCountFlagMod4 = 0;
		}
	}
	#endif
}
/*------------------------------------------------------------------------------------*/
void debugImp(void)
{
	#if debugOn
	debugRn();
	debugString(20,(uint8_t *)"Count impulse 0 = 0x");
	debugHex((uint8_t)(impCount0 >> 24));
	debugHex((uint8_t)(impCount0 >> 16));
	debugHex((uint8_t)(impCount0 >> 8));
	debugHex((uint8_t)(impCount0));

	debugRn();
	debugString(20,(uint8_t *)"Count impulse 1 = 0x");
	debugHex((uint8_t)(impCount1 >> 24));
	debugHex((uint8_t)(impCount1 >> 16));
	debugHex((uint8_t)(impCount1 >> 8));
	debugHex((uint8_t)(impCount1));

	debugRn();
	debugString(20,(uint8_t *)"Count impulse 2 = 0x");
	debugHex((uint8_t)(impCount2 >> 24));
	debugHex((uint8_t)(impCount2 >> 16));
	debugHex((uint8_t)(impCount2 >> 8));
	debugHex((uint8_t)(impCount2));

	debugRn();
	debugString(20,(uint8_t *)"Count impulse 3 = 0x");
	debugHex((uint8_t)(impCount3 >> 24));
	debugHex((uint8_t)(impCount3 >> 16));
	debugHex((uint8_t)(impCount3 >> 8));
	debugHex((uint8_t)(impCount3));

	debugRn();
	debugString(20,(uint8_t *)"Count impulse 4 = 0x");
	debugHex((uint8_t)(impCount4 >> 24));
	debugHex((uint8_t)(impCount4 >> 16));
	debugHex((uint8_t)(impCount4 >> 8));
	debugHex((uint8_t)(impCount4));
	#endif
}
/*------------------------------------------------------------------------------------*/
#ifdef ImpulseEEPData
/*------------------------------------------------------------------------------------*/
uint8_t readEEPDataImp(void)
{
	/*
	EXAMPLE
	address 0x0808 0000 - 0x0808 07FF (134742016 - 134744063)
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742020, 0x54); // Запись
	}
	else
	{
		// ERROR write byte EEPROM
	}
	uint32_t Address = 134742020;
	*(__IO uint8_t *)Address = (uint8_t) Data; // Запись
	uint8_t data = *(__IO uint8_t *)Address; // Чтение
	uint16_t data = *(__IO uint16_t *)Address; // Чтение
	uint32_t data = *(__IO uint32_t *)Address; // Чтение
	*/
	// 8
	impCountFlagMod0 = (*(__IO uint8_t *)134742016);
	impCountFlagMod1 = (*(__IO uint8_t *)134742017);
	impCountFlagMod2 = (*(__IO uint8_t *)134742018);
	impCountFlagMod3 = (*(__IO uint8_t *)134742019);
	impCountFlagMod4 = (*(__IO uint8_t *)134742020);
	impCountFlagDreb0 = (*(__IO uint8_t *)134742021);
	impCountFlagDreb1 = (*(__IO uint8_t *)134742022);
	impCountFlagDreb2 = (*(__IO uint8_t *)134742023);
	impCountFlagDreb3 = (*(__IO uint8_t *)134742024);
	impCountFlagDreb4 = (*(__IO uint8_t *)134742025);

	// 32
	impCount0 = 0;
	for(uint8_t i = 0; i < 4; i++)
	{
		impCount0 |= (uint32_t) ( (*(__IO uint8_t *)134742026+i) << (i*8) );
	} // 26, 27, 28, 29

	impCount0 = 1;
	for(uint8_t i = 0; i < 4; i++)
	{
		impCount1 |= (uint32_t) ( (*(__IO uint8_t *)134742030+i) << (i*8) );
	} // 30, 31, 32, 33

	impCount0 = 2;
	for(uint8_t i = 0; i < 4; i++)
	{
		impCount2 |= (uint32_t) ( (*(__IO uint8_t *)134742034+i) << (i*8) );
	} // 34, 35, 36, 37

	impCount0 = 3;
	for(uint8_t i = 0; i < 4; i++)
	{
		impCount3 |= (uint32_t) ( (*(__IO uint8_t *)134742038+i) << (i*8) );
	} // 38, 39, 40, 41

	impCount0 = 4;
	for(uint8_t i = 0; i < 4; i++)
	{
		impCount4 |= (uint32_t) ( (*(__IO uint8_t *)134742042+i) << (i*8) );
	} // 42, 43, 44, 44

	#if debugOn
	debugRn();
	debugString(24,(uint8_t *)"Read data impulse EEPROM");
	#endif
	return 1;
}
/*------------------------------------------------------------------------------------*/
uint8_t saveEEPDataImp(void)
{
	/*
	EXAMPLE
	address 0x0808 0000 - 0x0808 07FF (134742016 - 134744063)
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742020, 0x54); // Запись
	}
	else
	{
		// ERROR write byte EEPROM
	}
	uint32_t Address = 134742020;
	*(__IO uint8_t *)Address = (uint8_t) Data; // Запись
	uint8_t data = *(__IO uint8_t *)Address; // Чтение
	uint16_t data = *(__IO uint16_t *)Address; // Чтение
	uint32_t data = *(__IO uint32_t *)Address; // Чтение
	*/
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		// 8
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742016, impCountFlagMod0) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742017, impCountFlagMod1) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742018, impCountFlagMod2) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742019, impCountFlagMod3) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742020, impCountFlagMod4) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742021, impCountFlagDreb0) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742022, impCountFlagDreb1) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742023, impCountFlagDreb2) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742024, impCountFlagDreb3) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742025, impCountFlagDreb4) != HAL_OK) return 0;

		// 32
		for(uint8_t i = 0; i < 4; i++)
		{
			if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742026+i,(uint8_t)(impCount0 >> (8*i)) ) != HAL_OK) return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742030+i,(uint8_t)(impCount1 >> (8*i)) ) != HAL_OK) return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742034+i,(uint8_t)(impCount2 >> (8*i)) ) != HAL_OK) return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742038+i,(uint8_t)(impCount3 >> (8*i)) ) != HAL_OK) return 0;
		}

		for(uint8_t i = 0; i < 4; i++)
		{
			if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742042+i,(uint8_t)(impCount4 >> (8*i)) ) != HAL_OK) return 0;
		}// 42 43 44 46

		#if debugOn
		debugRn();
		debugString(24,(uint8_t *)"Save data impulse EEPROM");
		#endif
		return 1;
	}
	else
	{
		#if debugOn
		debugRn();
		debugString(30,(uint8_t *)"Error save data impulse EEPROM");
		#endif
		return 0;// ERROR write byte EEPROM
	}
	return 0;
}
/*------------------------------------------------------------------------------------*/
uint8_t clearEEPDataImp(void)
{
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		return 1;//clear//HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742020, 0x54); // Запись
	}
	else
	{
		// ERROR write byte EEPROM
		return 0;
	}
	return 0;
}
/*------------------------------------------------------------------------------------*/
#endif
/*------------------------------------------------------------------------------------*/
#endif
//
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/
void sxStatusInitVoid(void)
{
	// DEBUG INFO
#if debugOn
#if stateRegSx

	debugRn();
	debugString(11, (uint8_t*)"Init SX1276"); debugRn();

	// FRF
	debugString(15, (uint8_t*)"FRF: default 0x");
	debugHex(initFunctionDefaultRegSx(RegFrMsb,254,0));
	debugHex(initFunctionDefaultRegSx(RegFrMid,254,0));
	debugHex(initFunctionDefaultRegSx(RegFrLsb,254,0));
	debugString(9, (uint8_t*)", fact 0x");
	debugHex(initFunctionDefaultRegSx(RegFrMsb,255,0));
	debugHex(initFunctionDefaultRegSx(RegFrMid,255,0));
	debugHex(initFunctionDefaultRegSx(RegFrLsb,255,0));
	debugRn(); debugRn();

	// CONFIG
	debugString(7,(uint8_t*)"Config:"); debugRn();
	debugString(19,(uint8_t*)"RegModemConfig1: 0x");
	debugHex(initFunctionDefaultRegSx(RegModemConfig1,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegModemConfig1,255,0));
	debugRn();

	debugString(19,(uint8_t*)"RegModemConfig2: 0x");
	debugHex(initFunctionDefaultRegSx(RegModemConfig2,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegModemConfig2,255,0));
	debugRn();

	debugString(21,(uint8_t*)"RegSymbTimeoutLsb: 0x");
	debugHex(initFunctionDefaultRegSx(RegSymbTimeoutLsb,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegSymbTimeoutLsb,255,0));
	debugRn();

	debugString(19,(uint8_t*)"RegModemConfig3: 0x");
	debugHex(initFunctionDefaultRegSx(RegModemConfig3,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegModemConfig3,255,0));
	debugRn();

	debugString(23,(uint8_t*)"RegMaxPayloadLength: 0x");
	debugHex(initFunctionDefaultRegSx(RegMaxPayloadLength,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegMaxPayloadLength,255,0));
	debugRn();

	debugString(15,(uint8_t*)"RegSyncWord: 0x");
	debugHex(initFunctionDefaultRegSx(RegSyncWord,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegSyncWord,255,0));
	debugRn();

	debugString(21,(uint8_t*)"RegDetectOptimize: 0x");
	debugHex(initFunctionDefaultRegSx(RegDetectOptimize,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegDetectOptimize,255,0));
	debugRn();

	debugString(25,(uint8_t*)"RegDetectionThreshold: 0x");
	debugHex(initFunctionDefaultRegSx(RegDetectionThreshold,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegDetectionThreshold,255,0));
	debugRn();

	debugString(18,(uint8_t*)"RegPreambleMsb: 0x");
	debugHex(initFunctionDefaultRegSx(RegPreambleMsb,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegPreambleMsb,255,0));
	debugRn();

	debugString(18,(uint8_t*)"RegPreambleLsb: 0x");
	debugHex(initFunctionDefaultRegSx(RegPreambleLsb,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegPreambleLsb,255,0));
	debugRn();

	debugRn();

	debugString(5,(uint8_t*)"Read:");
	debugString(10,(uint8_t*)"RegLna: 0x");
	debugHex(initFunctionDefaultRegSx(RegLna,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegLna,255,0));
	debugRn();

	debugRn();
	debugString(6,(uint8_t*)"Write:");
	debugString(15,(uint8_t*)"RegPaConfig: 0x");
	debugHex(initFunctionDefaultRegSx(RegPaConfig,254,0));
	debugString(4,(uint8_t*)", 0x"); debugHex(initFunctionDefaultRegSx(RegPaConfig,255,0));
	debugRn();
#endif
#endif
}
/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/
// 134742047 - 134742052
/*------------------------------------------------------------------------------------*/
uint8_t saveRTC(RTC_TimeTypeDef * time, RTC_DateTypeDef * date)
{
	/*
	EXAMPLE
	address 0x0808 0000 - 0x0808 07FF (134742016 - 134744063)
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742020, 0x54); // Запись
	}
	else
	{
		// ERROR write byte EEPROM
	}
	uint32_t Address = 134742020;
	*(__IO uint8_t *)Address = (uint8_t) Data; // Запись
	uint8_t data = *(__IO uint8_t *)Address; // Чтение
	uint16_t data = *(__IO uint16_t *)Address; // Чтение
	uint32_t data = *(__IO uint32_t *)Address; // Чтение
	*/
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742047, time->Hours) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742048, time->Minutes) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742049, time->Seconds) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742050, date->Date) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742051, date->Month) != HAL_OK) return 0;
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742052, date->Year) != HAL_OK) return 0;

		#if debugOn
		debugRn();
		debugString(20,(uint8_t *)"Save data RTC EEPROM");
		#endif
		return 1;
	}
	#if debugOn
	debugRn();
	debugString(21,(uint8_t *)"Error save RTC EEPROM");
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------*/
uint8_t readRTC(RTC_TimeTypeDef * time, RTC_DateTypeDef * date)
{
	/*
	EXAMPLE
	address 0x0808 0000 - 0x0808 07FF (134742016 - 134744063)
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK) // Открываем
	{
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742020, 0x54); // Запись
	}
	else
	{
		// ERROR write byte EEPROM
	}
	uint32_t Address = 134742020;
	*(__IO uint8_t *)Address = (uint8_t) Data; // Запись
	uint8_t data = *(__IO uint8_t *)Address; // Чтение
	uint16_t data = *(__IO uint16_t *)Address; // Чтение
	uint32_t data = *(__IO uint32_t *)Address; // Чтение
	*/
	time->Hours = *(__IO uint8_t *)134742047;
	time->Minutes = *(__IO uint8_t *)134742048;
	time->Seconds = *(__IO uint8_t *)134742049;
	date->Date = *(__IO uint8_t *)134742050;
	date->Month = *(__IO uint8_t *)134742051;
	date->Year = *(__IO uint8_t *)134742052;
	#if debugOn
	debugRn();
	debugString(20,(uint8_t *)"Read data RTC EEPROM");
	#endif
	return 1;
}
/*------------------------------------------------------------------------------------*/
uint8_t clearRTC(RTC_TimeTypeDef * time, RTC_DateTypeDef * date)
{
	return 0;
}
/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/
void initProfileLRWAN(struct LoRaWAN * lrwan)
{
	// Ручная настройка профиля лораван сети
	lrwan->LoRaWAN_version = 3;
	lrwan->LoRaDEVICE = LoRaDevice;

	// id
	for(uint8_t i = 0; i < 3; i++)
	{
		lrwan->Home_NetID[i] = 2;
		lrwan->NetID[i] = 2;
	}

	for(uint8_t i = 0; i < 4; i++)
	{
		lrwan->DevAddr[i] = 1;
	}

	// Key
	for(uint8_t i = 0; i < 16; i++)
	{
		lrwan->Key.AppKey[i] = i;
		lrwan->Key.NwkKey[i] = i;
	}

	// Nonce
	lrwan->Nonce.DevNonce[0] = 0x01;
	lrwan->Nonce.DevNonce[1] = 0x01;

	lrwan->Nonce.JoinNonce[0] = 0x02;
	lrwan->Nonce.JoinNonce[1] = 0x02;
	lrwan->Nonce.JoinNonce[2] = 0x02;

	// EUI
	for(uint8_t i = 0; i < 8; i++)
	{
		lrwan->EUI.DevEUI[i] = i;
		lrwan->EUI.JoinEUI[i] = i;
	}

	// Count S
	for(uint8_t i = 0; i < 4; i++)
	{
		lrwan->FCnt.AFCntDwn[i] = 0;
		lrwan->FCnt.FCntUp[i] = 0;
		lrwan->FCnt.NFCntDwn[i] = 0;
	}

	for(uint8_t i = 0; i < 2; i++)
	{
		lrwan->FCnt.ConfFCnt[i] = 0;
		lrwan->FCnt.FCnt[i] = 0;
	}

	// Register
	lrwan->Register.DLSettings = 0;
	lrwan->Register.Dir = 0;

	// Key join
	MacJSIntKeyDerivation(lrwan);
	MacJSEncKeyDerivation(lrwan);

	// Key S
	MacSessionKeys(lrwan);


	// data write/read
}
//134742053 - 134742052 + 256
/*------------------------------------------------------------------------------------*/
uint8_t stateDataLoRaWAN(uint8_t * state) // 0x01 - data save, 0x02 - data read
{
	*state = *(__IO uint8_t *)134742053;
	#if debugOn
	debugRn();
	debugString(30,(uint8_t *)"State data LoRaWAN EEPROM = 0x");
	debugHex(*state);
	#endif
	return 1;
}
/*------------------------------------------------------------------------------------*/
uint8_t saveDataLoRaWAN(struct LoRaWAN * loraWan, uint8_t * arr_data_lora)
{
	uint8_t length =  sizeof(loraWan);
	//134742054
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK)
	{
		for(uint8_t i = 0; i < length; i++)
		{
			if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742054+i, *((uint8_t *)&loraWan+i)) != HAL_OK) return 0;
		}

		// state save
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742053, 0x01) != HAL_OK) return 0;

		#if debugOn
		debugRn();
		debugString(24,(uint8_t *)"Save data LoRaWAN EEPROM");
		#endif
		return 1;
	}
	#if debugOn
	debugRn();
	debugString(30,(uint8_t *)"Error save data LoRaWAN EEPROM");
	#endif
	return 0;
}
/*------------------------------------------------------------------------------------*/
uint8_t readDataLoRaWAN(struct LoRaWAN * loraWan, uint8_t * arr_data_lora)
{
	uint8_t length =  sizeof(loraWan);
	for(uint8_t i = 0; i < length; i++)
	{
		//arr_data_lora[i] = *(__IO uint8_t *)134742054 + i;
		//*((uint8_t *)&loraWan+i) = arr_data_lora[i];
		*((uint8_t *)&loraWan+i) = *(__IO uint8_t *)134742054 + i;

	}

	// state save
	if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK)
	{
		if(HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE,134742053, 0x02) != HAL_OK) return 0;
	}
	else
	{
		return 0;
	}

	#if debugOn
	debugRn();
	debugString(24,(uint8_t *)"Read data LoRaWAN EEPROM");
	#endif
	return 1;
}
/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/
uint8_t UfagaPack(struct Ufaga * ufaga, uint8_t * array, uint8_t length_array,
uint8_t * msg, uint8_t * len_msg, uint8_t type)
{
	// 		2				2			1				1				1			n
	// 0..1 count | 2..3 command | 4 count block | 5 number block | 6 len block | 7..N data
	// if(fopt_len == 0) n = M-7 (при M=59); else n = M-(7+fopt_len) (при M=59);

	uint8_t s = 0;
	*len_msg = 0;

	if(__WRITE == type)
	{
		ufaga->ufaga_impulse.count_impulse = 4;
		for(uint8_t i = 0; i < 4; i++)
		{
			ufaga->ufaga_impulse.impulse1[i] = (uint8_t)( (uint32_t)(impCount1 >> 8*i) );
			ufaga->ufaga_impulse.impulse2[i] = (uint8_t)( (uint32_t)(impCount2 >> 8*i) );
			ufaga->ufaga_impulse.impulse3[i] = (uint8_t)( (uint32_t)(impCount3 >> 8*i) );
			ufaga->ufaga_impulse.impulse4[i] = (uint8_t)( (uint32_t)(impCount4 >> 8*i) );
		}

		ufaga->datetime.sec = time.Seconds;
		ufaga->datetime.min = time.Minutes;
		ufaga->datetime.hour = time.Hours;
		ufaga->datetime.day = date.Date;
		ufaga->datetime.month = date.Month;
		ufaga->datetime.year = date.Year;

		s = WriteUfaga(ufaga, msg, len_msg);
		if(s == 0)
		{
			msg[0] = ufaga->Gcnt[1];
			msg[1] = ufaga->Gcnt[0];
			msg[2] = ufaga->command[1];
			msg[2] = ufaga->command[0];
			msg[2] = 0;
			msg[3] = 0;
			msg[4] = 1;
			msg[3] = 255;
			*len_msg = 8;

			return 0;
		}
		/*------------------------------------------------------------------*/

	}

	if(__READ == type)
	{
		// 		2				2			1				1				1			n
		// 0..1 count | 2..3 command | 4 count block | 5 number block | 6 len block | 7..N data
		// if(fopt_len == 0) n = M-7 (при M=59); else n = M-(7+fopt_len) (при M=59);
		/*------------------------------------------------------------------*/
		if(array[2] == 0x01 && array[3] == 0x01)
		{
			if(array[4] != 0)
			{
				//
				return 0;
			}
			else
			{
				if(array[6] != 6 && length_array != 13)
				{
					return 0;
				}

				time.Seconds = array[6];
				time.Minutes = array[7];
				time.Hours = array[8];
				date.Date = array[9];
				date.Month = array[10];
				date.Year = array[11];


				// save eeprom

				// write pack

				*len_msg = 8;
				msg[0] = ufaga->Ecnt[1];
				msg[1] = ufaga->Ecnt[0];
				msg[2] = 0x81;
				msg[3] = 0x01;
				msg[4] = 0;
				msg[5] = 0;
				msg[6] = 1;
				msg[7] = 1;
			}

			return 0;
		}

		/*------------------------------------------------------------------*/
		if(array[2] == 0x01 && (array[3] == 0x02 || array[3] == 0x82))
		{
			if(array[4] != 0)
			{
				//
				return 0;
			}
			else
			{
				if(array[6] != 0 && length_array != 7)
				{
					return 0;
				}


				//if()
				time.Seconds = array[6];
				time.Minutes = array[7];
				time.Hours = array[8];
				date.Date = array[9];
				date.Month = array[10];
				date.Year = array[11];


				// save eeprom

				// write pack

				*len_msg = 8;
				msg[0] = ufaga->Ecnt[1];
				msg[1] = ufaga->Ecnt[0];
				msg[2] = 0x81;
				msg[3] = 0x01;
				msg[4] = 0;
				msg[5] = 0;
				msg[6] = 1;
				msg[7] = 1;
			}

			return 0;
		}
	}

	return 0;
}
/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/
uint8_t Low(void)
{
	sysCl = LOW;
	HAL_SPI_DeInit(&hspi1);
	//HAL_ADC_DeInit(&hadc);
	HAL_UART_DeInit(&huart2);
	HAL_TIM_Base_Stop_IT(&htim6);
	HAL_TIM_Base_DeInit(&htim6);

	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	SystemClock_Config();
	MX_TIM6_Init();
	MX_USART2_UART_Init();
	MX_SPI1_Init();


	MX_GPIO_Init();
	HAL_TIM_Base_Start_IT(&htim6);

	return 1;
}
/*------------------------------------------------------------------------------------*/
uint8_t Run(void)
{
	sysCl = HIGH;

	HAL_SPI_DeInit(&hspi1);
	//HAL_ADC_DeInit(&hadc);
	HAL_UART_DeInit(&huart2);
	HAL_TIM_Base_Stop_IT(&htim6);
	HAL_TIM_Base_DeInit(&htim6);

	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_4);
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);


	SystemClock_Config();
	MX_TIM6_Init();
	MX_USART2_UART_Init();
	MX_SPI1_Init();

	//MX_ADC_Init();
	MX_GPIO_Init();
	HAL_TIM_Base_Start_IT(&htim6);

	return 1;
}
/*------------------------------------------------------------------------------------*/
//
//
//
//
//
//
//
//
/*------------------------------------------------------------------------------------*/

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
