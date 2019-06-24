
#include "ufaga/define_protocol_ufaga.h"

#ifdef device
#if device == LSI
uint8_t transformSendPreparedData(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t transformSetHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t transformPrepareDataTimeAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg,
		uint8_t coercion);
uint8_t transformNullifyHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t transformPrepareDataBatteryCharge(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg,
		uint8_t coercion);
uint8_t transformPrepareImpulseLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg,
		uint8_t coercion);
uint8_t transformSmoothDataImulsnyLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t transformSmoothDataImulsnyRewriting(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t transformPrepareDeviceDetails(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg,
		uint8_t coercion);

uint8_t reformSendPreparedData(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t reformSetHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t len_msg);
uint8_t reformPrepareDataTimeAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t reformNullifyHoursAndDate(struct Ufaga * ufaga, uint8_t * msg, uint8_t len_msg);
uint8_t reformPrepareDataBatteryCharge(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t reformPrepareImpulseLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t reformSmoothDataImulsnyLines(struct Ufaga * ufaga, uint8_t * msg, uint8_t len_msg);
uint8_t reformSmoothDataImulsnyRewriting(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
uint8_t reformPrepareDeviceDetails(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
#endif
#endif

#ifdef LBS
uint8_t transformSendPreparedDataGateway(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);

uint8_t reformSendPreparedDataGateway(struct Ufaga * ufaga, uint8_t * msg, uint8_t * len_msg);
#endif

uint8_t ReadUfaga(struct Ufaga * ufaga, uint8_t * msg_in, uint8_t len_msg_in);
uint8_t WriteUfaga(struct Ufaga * ufaga, uint8_t * msg_out, uint8_t * len_msg_out);
