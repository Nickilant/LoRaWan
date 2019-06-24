#include "mac/mac_conf.h"


// APP - данные приложения, NET - данные сети.
#define APP 1 // FPort 1-223
#define NET 0 // FPort = 0 and FOoptLen = 0 // FPort = 224 f

// MHDR MType
#define Join_requst 			0b00000000 // 7-5 bit MHDR. Запрос процедуры активации по воздуху
#define Join_accept 			0b00100000 // 7-5 bit MHDR. Подтверждение процедуры активности по воздоху
#define Unconfirmed_Data_Up 	0b01000000 // 7-5 bit MHDR. Передача данных без подтверждения на линии вверх
#define Unconfirmed_Data_Down 	0b01100000 // 7-5 bit MHDR. Передача данных без подтверждения на линии вниз
#define Confirmed_Data_Up 		0b10000000 // 7-5 bit MHDR. Передача данных с подтверждением на линии вверх
#define Confirmed_Data_Down 	0b10100000 // 7-5 bit MHDR. Передача данных с подтверждением на линии вниз
#define Rejoin_request 			0b11000000 // 7-5 bit MHDR. RFU
#define Proprietary 			0b11100000 // 7-5 bit MHDR. Для частных решений

// message type
#define READ	0
#define WRITE	1

//FCtrl register,		 	bit , 	count
#define ADR					7		//1
#define ACK					5		//1
#define FOptsLen			0		//4
#if upLink
#define ADRACKReq			6		//1
#define ClassB				4		//1
#endif
#if downLink
#define RFU					6		//1
#define FPending			4		//1
#endif

// DLSettings defines bits	count bit
#define OptNeg 			7		//1
#define RX1DRoffset 	4		//3 RX1DRoffset
#define RX2 			0		//4 RX2 Data rate

// REQYEST
#define JoinReqType		0xff
#define RejoinReqType0 	0x00
#define RejoinReqType1 	0x01
#define RejoinReqType2	0x02

// COMMAND MAC
#if EndDevice
#define ResetInd 				0x01
#define LinkCheckReq 			0x02
#define LinkADRAns 				0x03
#define DutyCycleAns 			0x04
#define RXParamSetupAns 		0x05
#define DevStatusAns 			0x06
#define NewChannelAns 			0x07
#define RXTimingSetupAns 		0x08
#define TxParamSetupAns	 		0x09
#define DlChannelAns 			0x0a
#define RekeyInd 				0x0b
#define ADRParamSetupAns 		0x0c
#define DeviceTimeReq 			0x0d
#define RejoinParamSetupAns 	0x0f
#endif

#if Gateway
#define ResetConf 				0x01
#define LinkCheckAns 			0x02
#define LinkADRReq 				0x03
#define DutyCycleReq 			0x04
#define RXParamSetupReq 		0x05
#define DevStatusReq 			0x06
#define NewChannelReq 			0x07
#define RXTimingSetupReq 		0x08
#define TxParamSetupReq 		0x09
#define DlChannelReq 			0x0a
#define RekeyConf 				0x0b
#define ADRParamSetupReq 		0x0c
#define DeviceTimeAns 			0x0d
#define ForceRejoinReq		 	0x0e
#define RejoinParamSetupReq		0x0f
#endif

/*---------MacPayload--------*/
/*
FHDR - FHDR содержит короткий
DevAddr - адрес устройства конечного устройства (4 byte);
Fctrl - управление кадров октед (1 byte);
FCnt - счетчик кадров 2-октед (2 byte) FCntUp или NFCntDwn ;
FOpts - октетов вариантов каскасных (0..15 byte).
*/

//uint32_t DevAddr = 0x01020304; // адресс устртойства

/*
7 - ADR.  адаптивной скоростью передачи данных, включена сеть
будет оптимизирована для использования самой высокой скорости передачи данных возможно.
Отключение бита произойдет когда с сервера придет пакет с таким же положением 1
кароче всегда нуль пока что. меняем на speeding factor
6 - ADRACKReq-uplink, RFU-downlink.
ADRACKReq не устанавливать если используем по умолчанию скорость данных
5 - ACK.
4 - ClassB-uplink(дает понять что перешел на класс би), FPending-downlink(будет сверхобъем данных неоходимо увелить окна приема).
3..0 - FOptsLen. длина блока fopts
*/
//uint8_t Fctrl = 0x00;
/*---------Mic---------------*/
/*
struct LoRaWAN
{
	uint8_t JoinEUI[8], // идентификатор приложения в адресном пространстве IEEE EUI64.
			DevEUI[8]; //глобальный идентификатор устройства в адресном пространстве IEEE EUI64

#if LoRaWan_1_0 // LoRaWAN_version = 0
#if LoRaWANstruct_version == 0
//
#endif
#endif

#if LoRaWan_1_0_1 // LoRaWAN_version = 1
#if LoRaWANstruct_version == 1
//
#endif
#endif

#if LoRaWan_1_0_2 // LoRaWAN_version = 2
#if LoRaWANstruct_version == 1
//
#endif
#endif

#if LoRaWan_1_1 // LoRaWAN_version = 3
#if LoRaWANstruct_version == 3

	uint8_t JoinEUI[8], // идентификатор приложения в адресном пространстве IEEE EUI64.
			DevEUI[8]; //глобальный идентификатор устройства в адресном пространстве IEEE EUI64

	uint8_t DevNonce[2]; //  случайное число, генерируемое конечным устройством (используется при расчете NwkSKey и AppSKey).

	uint8_t JoinNonce[3], //- AppNonce - случайное число, генерируемое сервером приложений (используется при расчете NwkSKey и AppSKey).
			Home_NetID[3]; //- NetID - идентификатор сети, старшие 7 бит которого (31..25) соответствуют идентификатору NwkID, младшие 17 бит могут произвольно назначаться оператором.

#endif
#endif
};*/





#if LoRaWANstruct_version == 0
#if LoRaWan_1_0
//
#endif
#endif

#if LoRaWANstruct_version == 1
#if LoRaWan_1_0_1
//
#endif
#endif

#if LoRaWANstruct_version == 2
#if LoRaWan_1_0_2
//
#endif
#endif

#if LoRaWANstruct_version == 3
#if LoRaWan_1_1
struct _EUI // global		16
{
    uint8_t JoinEUI[8];
	uint8_t DevEUI[8];
};
/*
struct _FCtrl // registre
{
	uint8_t ADR; // 7
	uint8_t RFU; // 6
	uint8_t ADRACKReq; // 6
	uint8_t ACK; // 5
	uint8_t FPending; // 4
	uint8_t ClassB; // 4
	uint8_t FOptsLen; // 3..0
};
*/
struct _FCnt // global		22
{
	uint8_t AFCntDwn[4]; // LW 1.1
	//uint8_t res_AFCntDwn[4]; // LW 1.1
	uint8_t	NFCntDwn[4]; // LW 1.1
	//uint8_t	res_NFCntDwn[4]; // LW 1.1
	//uint8_t	FCntDwn[4]; // LW 1.0
	//uint8_t	FCntDown[4]; // LW 1.0
	uint8_t	FCntUp[4];
	//uint8_t	res_FCntUp[4];
	uint8_t ConfFCnt[2];
	uint8_t FCnt[2];


};

struct _Key // global		160
{
	uint8_t AppKey[16];
	uint8_t	NwkKey[16];
	uint8_t SAppSIntKey[16];
	uint8_t	SNwkSIntKey[16];
	uint8_t FAppSIntKey[16];
	uint8_t	FNwkSIntKey[16];
	uint8_t AppSKey[16];
	uint8_t	NwkSEncKey[16];
	uint8_t JSIntKey[16]; // JSIntKey = aes128_encrypt(NwkKey, 0x06 | DevEUI | pad16)
	uint8_t	JSEncKey[16];
};

struct _Nonce // global		5
{
	uint8_t DevNonce[2];
	uint8_t JoinNonce[3];
};
					//		8
struct _DataRate // global // реализуеться по красоте когда МАС команды все изучу
{
	uint8_t DR; // DR0..15 DataRate
	uint8_t M;// register
	uint8_t	N;// register
	uint8_t SF;// register
	uint8_t	kHz;// register
	uint8_t	CR;// register
	uint8_t TXPower;
	uint8_t	MaxEIRP; /*dB*/;
};
				//		12
struct _Req // global // Request
{
	uint8_t ReqType; // register
	uint8_t RJcount0[2];
	uint8_t	RJcount1[2];
	uint8_t	RJcount2[2];
	uint8_t RJcount0_last[2];
	uint8_t	RJcount1_last[2];
	uint8_t	RJcount2_last[2];
};

struct _CFList // global 	16
{
	uint8_t CFList[16];
	uint8_t CFListType;// register
	uint8_t Freq_Ch2[3];//?
	uint8_t	Freq_Ch3[3];//?
	uint8_t	Freq_Ch4[3];//?
	uint8_t	Freq_Ch5[3];//?
	uint8_t	Freq_Ch6[3];//?
};

struct _Register  //		4
{
	uint8_t FPort;// registre
	uint8_t MHDR; // registre
	uint8_t DLSettings;// register // global
	uint8_t RxDelay;// register // global
	uint8_t Dir;
	uint8_t TxDr; //?	// global
	uint8_t TxCh; //?	// global
};

struct _ArrayProgramming
{
	uint8_t B0[16];
	uint8_t B1[16];
	uint8_t cmac[16];
	uint8_t A[16];
	uint8_t S[16];
	uint8_t K[16];
};


struct _MacCommand
{
	uint8_t repeater;
};

// 243
struct LoRaWAN
{
	uint8_t OTA; // global
	uint8_t ABP; // global
	uint8_t LoRaWAN_version; // global
	uint8_t LoRaDEVICE;

	uint8_t FCtrl;
	uint8_t FOpts[16];
	uint8_t DevAddr[4]; // global
	uint8_t Home_NetID[3]; // global
	uint8_t NetID[3]; // global
	uint8_t MIC[4];

	struct _EUI EUI;
	//struct _FCtrl FCtrl;
	struct _FCnt FCnt;
	struct _Key Key;
	struct _Nonce Nonce;
	struct _DataRate DataRate;
	struct _Req Req;
	struct _CFList CFList;
	struct _Register Register;
	struct _ArrayProgramming ArrayProgramming;
	struct _MacCommand MacCommand;
}; // 253


struct FRMPayloadInfo
{
	uint8_t typeDate;
	uint8_t biginByte;
	uint8_t LengthFrmPayload;
};

#endif
#endif

// error
#define LW_ERR						0
#define LW_OK 						1
#define LW_ERR_CURRECT_VERSION		2
#define LW_ERR_MTYPE				3
#define LW_ERR_MIC					4

// transform state
// reform state
// join req
// rejoin req
// join accept
// Unconfirmed Data Up
// Unconfirmed Data Down
// Confirmed Data Up
// Confirmed Data Dpwn
