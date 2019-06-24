#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define length_flash_data	256

#define type_pack	7 // possition bit
#ifdef type_pack
#define reception 	0
#define answer		1
#endif

#define Coercion	7 // possition bit
#define YES 		1
#define NO			0

#define device 	LSI // number device
#ifdef device
#define LSI		0
#define LBS		1
#define USD		2
#define MSD		3
#endif

#define version_device		version_1
#ifdef version_device
#define version_1 			1
#define version_2 			2
#define version_3 			3
#define version_4 			4
#endif

// message type
#define READ	0
#define WRITE	1

#define ReturnUfaga
#ifdef ReturnUfaga
//
#endif

#if device == LSI
struct UfagaImpulse
{
	uint8_t count_impulse;
	uint8_t impulse1[4];
	uint8_t impulse2[4];
	uint8_t impulse3[4];
	uint8_t impulse4[4];
	uint8_t impulse5[4];
};

struct Pack
{
	uint8_t length;
	uint8_t n;
	uint8_t count_block;
	uint8_t number_block;
	uint8_t length_block;
	uint8_t command_gateway;
	uint8_t flash_data[length_flash_data];
};

struct DateTime
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};

struct Battery
{
	uint8_t state;
};

struct Ufaga
{
	uint8_t version;
	uint8_t device_type;
	uint8_t command[2];
	uint8_t Gcnt[2];
	uint8_t Ecnt[2];
	struct UfagaImpulse ufaga_impulse;
	struct DateTime datetime;
	struct Battery baterry;
	struct Pack pack;
};
#endif
