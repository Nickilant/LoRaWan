
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_hal.h"

#include "cmac/cmac.h"
#include "sx/sx.h"
#include "sx/sx_status.h"
#include "mac/mac_level.h"
#include "ufaga/protocol_ufaga.h"

#define DIOD_Pin GPIO_PIN_0
#define DIOD_GPIO_Port GPIOA
#define NSS_Pin GPIO_PIN_4
#define NSS_GPIO_Port GPIOA
#define DIO5_Pin GPIO_PIN_8
#define DIO5_GPIO_Port GPIOA
#define DIO3_Pin GPIO_PIN_9
#define DIO3_GPIO_Port GPIOA
#define DIO4_Pin GPIO_PIN_10
#define DIO4_GPIO_Port GPIOA
#define DIO0_Pin GPIO_PIN_11
#define DIO0_GPIO_Port GPIOA
#define DIO1_Pin GPIO_PIN_12
#define DIO1_GPIO_Port GPIOA
#define DIO2_Pin GPIO_PIN_15
#define DIO2_GPIO_Port GPIOA
#define RELE_Pin GPIO_PIN_3
#define RELE_GPIO_Port GPIOB
#define IMP1_Pin GPIO_PIN_4
#define IMP1_GPIO_Port GPIOB
#define IMP2_Pin GPIO_PIN_5
#define IMP2_GPIO_Port GPIOB
#define IMP3_Pin GPIO_PIN_6
#define IMP3_GPIO_Port GPIOB
#define IMP4_Pin GPIO_PIN_7
#define IMP4_GPIO_Port GPIOB


#define __READ		0
#define __WRITE		1

#define IT 		0

#define 	DIOD_ON 	HAL_GPIO_WritePin(DIOD_GPIO_Port, DIOD_Pin, GPIO_PIN_SET)
#define 	DIOD_OFF 	HAL_GPIO_WritePin(DIOD_GPIO_Port, DIOD_Pin, GPIO_PIN_RESET)

// PWR MODE
uint8_t sysCl;

#define LOW 	0
#define HIGH 	1

uint8_t Low(void);
uint8_t Run(void);

#define _LOW 	Low()
#define _RUN	Run()

// SX
#define Sx_On 				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET )
#define Sx_Off 				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET )
#define SxPowerState 		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)
// SX SPI
#define SxNSS_higt			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET ) // NSS1 high
#define SxNSS_low			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET ) // NSS1 low

// DEBUG
#define debugOn     	1
#define debugSPI    	0
#define debugError  	0
#define debugStatus 	0
#define debugPack   	0
#define debugMainWhile 	0

// IMPULSE
#define Impulse 				1
#define ImpulseTest				1
#define ImpulseFlashData		0
#define ImpulseEEPData			1
#define ImpulseMaxCountDrebesg 	5

#define timeOutSpi    10000
#define timeOutUart   10000

#ifdef debugOn
void charToHex(uint8_t hex, uint8_t* array_hex/*length = 2*/);
void debugRn(void);
void debugHex(uint8_t hex_byte);
void debugString(uint16_t length, uint8_t * array_byte);
#endif

#ifdef Impulse
#define IMP0 		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)
#define IMP1		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)
#define IMP2 		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)
#define IMP3 		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)
#define IMP4 		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)
// extern impulse
uint8_t impCountFlagMod0;
uint8_t impCountFlagMod1;
uint8_t impCountFlagMod2;
uint8_t impCountFlagMod3;
uint8_t impCountFlagMod4;
uint8_t impCountFlagDreb0;
uint8_t impCountFlagDreb1;
uint8_t impCountFlagDreb2;
uint8_t impCountFlagDreb3;
uint8_t impCountFlagDreb4;
uint32_t impCount0;
uint32_t impCount1;
uint32_t impCount2;
uint32_t impCount3;
uint32_t impCount4;
#if ImpulseTest
uint32_t i0;
uint32_t i1;
uint32_t i2;
uint32_t i3;
uint32_t i4;
#endif

// function
void clearDataOperatioMemoryImpulse(void);
void impulseWhile(void);
void debugImp(void);

#ifdef ImpulseFlashData
#endif

#ifdef ImpulseEEPData
uint8_t readEEPDataImp(void);
uint8_t saveEEPDataImp(void);
uint8_t clearEEPDataImp(void);
#endif

#endif

// State chip
uint8_t readStateChip(void);
uint8_t writeStateChip(uint8_t state);

// PWR mode

// RTC
RTC_DateTypeDef date;
RTC_TimeTypeDef time;
uint8_t saveRTC(RTC_TimeTypeDef * time, RTC_DateTypeDef * date);
uint8_t readRTC(RTC_TimeTypeDef * time, RTC_DateTypeDef * date);
uint8_t clearRTC(RTC_TimeTypeDef * time, RTC_DateTypeDef * date);

// LoRaWAN
struct LoRaWAN LRWAN;
void initProfileLRWAN(struct LoRaWAN * lrwan);
//uint8_t arrDataLoRaWAN[sizeof(LRWAN)];
uint8_t stateDataLoRaWAN(uint8_t * state); // 0x01 - data yes,
uint8_t saveDataLoRaWAN(struct LoRaWAN * loraWan, uint8_t * arr_data_lora);
uint8_t readDataLoRaWAN(struct LoRaWAN * loraWan, uint8_t * arr_data_lora);

// Protocol UFAGA
//struct Ufaga ufaga;
uint8_t UfagaPack(struct Ufaga * ufaga, uint8_t * array, uint8_t length_array, uint8_t * msg, uint8_t * len_msg, uint8_t type);


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
