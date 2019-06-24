#include "aes/aes.h"
#include "cmac/cmac.h"
#include "mac/mac_define.h"

/*
Example

transform
reform


Radio PHY layer:
	Preamble | PHDR | PHDR_CRC | PHYPayload | CRC*

PHYPayload:
	MHDR | MACPayload | MIC
or
	MHDR | Join-Request or Rejoin-Request | MIC
or
	MHDR | Join-Accept

MACPayload:
	FHDR | FPort | FRMPayload

FHDR:
	DevAddr | FCtrl | FCnt | FOpts

*/

/*
#if LoRaWan_1_0 // LoRaWAN_version = 0
if(LoRaWAN_version == 0)
{
	//
}
else
{
	return 0;
}
#endif

#if LoRaWan_1_0_1 // LoRaWAN_version = 1
if(LoRaWAN_version == 1)
{
	//
}
else
{
	return 0;
}
#endif

#if LoRaWan_1_0_2 // LoRaWAN_version = 2
if(LoRaWAN_version == 1)
{
	//
}
else
{
	return 0;
}
#endif

#if LoRaWan_1_1 // LoRaWAN_version = 3
if(LoRaWAN_version == 3)
{
	//
}
else
{
	return 0;
}
#endif
*/

/*-----HEADER function--------------------------------------------------*/

uint8_t aes128_encrypt(uint8_t * key, uint8_t * in_array_data, uint8_t * out_array_data,
		uint8_t length);

uint8_t aes128_decrypt(uint8_t * key, uint8_t * in_array_data, uint8_t * out_array_data,
		uint8_t length);

uint8_t XOR_block(uint8_t * block_one, uint8_t * block_two, uint8_t * block_out);

uint8_t MacJSIntKeyDerivation(struct LoRaWAN * loraWan);

uint8_t MacJSEncKeyDerivation(struct LoRaWAN * loraWan);

uint8_t MacSessionKeys(struct LoRaWAN * loraWan);

uint8_t MacMICUplinkFrames(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t len_msg, uint8_t messageType);

uint8_t MacMICDownlinkFrames(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t len_msg, uint8_t messageType);

uint8_t MacFOpts(struct LoRaWAN * loraWan, uint8_t * fopts, uint8_t * fopts_len, uint8_t messageType);

uint8_t MacFramePayloadEncryption_FRMPayload(struct LoRaWAN * loraWan,
		uint8_t * frmpayload, uint8_t frmpayload_len, uint8_t * pld, uint8_t messageType);

/*----------------------------------------------------------------------*/
#if transform

uint8_t transformMacJoinRequesMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg);

uint8_t transformMacJoinAcceptMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg);

uint8_t transformMacRejoinRequesTypeMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg);

uint8_t transformMacJoinAcceprCFList(struct LoRaWAN * loraWan, uint8_t * cflist);

uint8_t transformMacDataRateAndDeviceANDPower(struct LoRaWAN * loraWan);

uint8_t transformMacDataRateMaximumPayloadSize(struct LoRaWAN * loraWan);

uint8_t transformMacTxPower(struct LoRaWAN * loraWan);

#endif

/*----------------------------------------------------------------------*/
#if reform

uint8_t reformMacJoinRequesMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg);

uint8_t reformMacJoinAcceptMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg);

uint8_t reformMacRejoinRequesTypeMessage(struct LoRaWAN * loraWan, uint8_t * msg, uint8_t * len_msg);

uint8_t reformMacJoinAcceprCFList(struct LoRaWAN * loraWan, uint8_t * cflist);

uint8_t reformMacDataRateAndDeviceANDPower(struct LoRaWAN * loraWan, uint8_t datarate);

uint8_t reformMacDataRateMaximumPayloadSize(struct LoRaWAN * loraWan, uint8_t datarate);

uint8_t reformMacTxPower(struct LoRaWAN * loraWan, uint8_t txpower);

uint8_t reformMacFramePayloadEncryption_FRMPayload(struct LoRaWAN * loraWan,
		uint8_t * frmpayload, uint8_t * frmpayload_len, uint8_t * pld);

#endif
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
uint8_t writeMacJoinReques(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
uint8_t readMacJoinReques(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
uint8_t writeMacJoinAccept(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
uint8_t readMacJoinAccept(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
uint8_t writeMacREJoinReques(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
uint8_t readMacREJoinReques(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);

#if upLink
#if LoRaDevice == LoRaDevice_EndDevice || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
//Без потверждения
uint8_t writeMacUnconfirmedDataUp(struct LoRaWAN * loraWan, uint8_t * FrmPayload_data_in, uint8_t len_FrmPayload_data_in,
		uint8_t * data_out, uint8_t * len_data_out);
//с потвердждением
uint8_t writeMacConfirmedDataUp(struct LoRaWAN * loraWan,uint8_t * FrmPayload_data_in, uint8_t  len_FrmPayload_data_in,
		uint8_t *data_out, uint8_t * len_data_out);
#endif
#if LoRaDevice == LoRaDevice_Gateway || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
uint8_t readMacUnconfirmedDataUp(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload);
uint8_t readMacConfirmedDataUp(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload);
#endif
#endif
#if downLink
#if LoRaDevice == LoRaDevice_Gateway || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
uint8_t writeMacUnconfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * FrmPayload_data_in, uint8_t  len_FrmPayload_data_in,
		uint8_t *data_out, uint8_t * len_data_out);
uint8_t writeMacConfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * FrmPayload_data_in, uint8_t  len_FrmPayload_data_in,
		uint8_t *data_out, uint8_t * len_data_out);
#endif
#if LoRaDevice == LoRaDevice_EndDevice || LoRaDevice == LoRaDevice_EndDevice_and_Gateway
uint8_t readMacUnconfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload);
uint8_t readMacConfirmedDataDown(struct LoRaWAN * loraWan,uint8_t * data_in, uint8_t  len_data_in,
		uint8_t *data_out, uint8_t * len_data_out, struct FRMPayloadInfo * infoFrmPayload);
#endif
#endif
/*----------------------------------------------------------------------*/
uint8_t writeMacProprietary(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
uint8_t readMacProprietary(struct LoRaWAN * loraWan,uint8_t * data, uint8_t * len_data);
/*----------------------------------------------------------------------*/

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

uint8_t transformMacCommand(uint8_t comm, uint8_t * arr, uint8_t * len_arr, uint8_t * init);

#if Gateway // передаваемое от Gateway
uint8_t transformMacCommResetConf(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommLinkCheckAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommLinkADRReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDutyCycleReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRXParamSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDevStatusReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommNewChannelReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRXTimingSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommTxParamSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDlChannelReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRekeyConf(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommADRParamSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDeviceTimeAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommForceRejoinReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRejoinParamSetupReq(uint8_t * arr, uint8_t * len_arr);
#endif

#if EndDevice // передаваемое от EndDevice
uint8_t transformMacCommResetInd(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommLinkCheckReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommLinkADRAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDutyCycleAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRXParamSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDevStatusAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommNewChannelAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRXTimingSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommTxParamSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDlChannelAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRekeyInd(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommADRParamSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommDeviceTimeReq(uint8_t * arr, uint8_t * len_arr);
uint8_t transformMacCommRejoinParamSetupAns(uint8_t * arr, uint8_t * len_arr);
#endif

#endif

#if reform // reformMacComm

uint8_t reformMacCommand(uint8_t comm, uint8_t * arr, uint8_t * len_arr, uint8_t * init);

#if Gateway
uint8_t reformMacCommResetConf(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommLinkCheckAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommLinkADRReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDutyCycleReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRXParamSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDevStatusReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommNewChannelReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRXTimingSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommTxParamSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDlChannelReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRekeyConf(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommADRParamSetupReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDeviceTimeAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommForceRejoinReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRejoinParamSetupReq(uint8_t * arr, uint8_t * len_arr);
#endif

#if EndDevice
uint8_t reformMacCommResetInd(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommLinkCheckReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommLinkADRAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDutyCycleAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRXParamSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDevStatusAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommNewChannelAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRXTimingSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommTxParamSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDlChannelAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRekeyInd(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommADRParamSetupAns(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommDeviceTimeReq(uint8_t * arr, uint8_t * len_arr);
uint8_t reformMacCommRejoinParamSetupAns(uint8_t * arr, uint8_t * len_arr);
#endif

#endif
#endif

#endif





















