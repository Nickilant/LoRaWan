/*
Radio PHY layer:
	Preamble | PHDR | PHDR_CRC | PHYPayload | CRC*

PHYPayload:
	MHDR | MACPayload | MIC
or
	MHDR | Join-Request or Rejoin-Request | MIC
or
	MHDR | Join-Accept

MHDR (1byte):
	MType (3bit) | RFU (3bit) | Major (2bit)

MIC:
	cmac[0..3] - везде по разному

MACPayload:
	FHDR | FPort | FRMPayload
	FHDR - DevAddr (4byte) | FCtrl (1byte) | FCnt (2byte) | FOpts (0-15byte)

FHDR:
	c	4		1		2		15
	DevAddr | FCtrl | FCnt | FOpts
	FCtrl - ADR (1bit) | ADRACKReq or RFU (1bit) | ACK (1bit) | ClassB or ClassB (1bit) | FOptsLen (4bit)
*/

/*
Standart RU:
LORA (RU864-870 МГц)
sync word - 0x34
preamble len - 8
bandwidth kHz - 125
Channel feq - 868.9 869.1
Lora DR - 0..5
Nb channels - 2
*/

/*
CFList
bytes	3			3			3			3			3			1
data	Freq Ch2	Freq Ch3	Freq Ch4	Freq Ch5	Freq Ch6	CFListType
CFListType ДОЛЖЕН быть равен нулю (0) до
 указать, что CFList содержит список частот.
*/

/*
The Network Session consists of the following state:

- F/SNwkSIntKey
- NwkSEncKey
- FCntUp
- FCntDwn (LW 1.0) or NFCntDwn (LW 1.1)
- DevAddr

The Application Session consists of the following state:

- AppSKey
- FCntUp
- FCntDown (LW 1.0) or AFCntDwn (LW 1.1)

*/

/*
0 - 1.0,
1 - 1.0.1,
2 - 1.0.2,
3 - 1.1
example:
#if LoRaWan_1.0
//
#endif
*/
/*
Dir - при чтение или отравки на uplink(gateway) ставим 0,
при чтение и отправки на downlink(end device) ставим 1.
*/
#include "mac/mac_extern.h"

/*
0. Выберите режим работы.
- Если у вас "EndDevice" то вы должны установить "0" и
выбрать устройсво LoRaDEVICE = LoRaDevice_EndDevice,
LoRaWAN_SERVER = 0,
следует установить 1 в upLink, а в downLink 0
- Если у вас "Gateway", то установите "1" и выберите
LoRaDEVICE = LoRaDevice_Gateway,
LoRaWAN_SERVER = 1,
следует установить 1 в downLink, а в upLink 0
*/
#define LoRaWAN_SERVER 0

// 1. Выберите версию протокола МАС. Включить "1", выключить "0"
uint8_t LoRaWAN_version;
#define LoRaWan_1_0 	0 // LoRaWAN_version = 0
#define LoRaWan_1_0_1 	0 // LoRaWAN_version = 1
#define LoRaWan_1_0_2 	0 // LoRaWAN_version = 2
#define LoRaWan_1_1 	1 // LoRaWAN_version = 3

// 1.1 Настройка структуры под версию. 0 = 1.0, 1 = 1.0.1, 2 = 1.0.2, 3 = 1.1
#define LoRaWANstruct_version 3

// 2. Выберите устройства

#define LoRaDevice 					LoRaDevice_EndDevice_and_Gateway
#define LoRaDevice_EndDevice 				0
#define LoRaDevice_Gateway					1
#define LoRaDevice_EndDevice_and_Gateway 	2


// Макросы открывающие поля ПО, для реализаций какого либо функционала или констант
#define EndDevice 	1 // Включить - 1, выключить 0.
#define Gateway 	1 // Включить - 1, выключить 0.

#define upLink		1 // if(LoRaDEVICE == LoRaDevice_EndDevice) 1 else 0. Uplink Message - a message from a Device to an Application.
#define downLink	1 //if(LoRaDEVICE == LoRaDevice_Gateway) 1 else 0. Downlink Message - a message from an Application to a Device.

#define macCommand	1 // МАС команды Адаптация скорости. Включить - 1, выключить 0.

#define transform 	1 // Преобразовать. Включить - 1, выключить 0.
#define reform		1 // Разпреобразова :D .Включить - 1, выключить 0.

#define LoRaWAN_ReturnDebug			1
