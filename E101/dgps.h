
/*==================================================================================================================================*/
#ifndef _DGPS_H_
#define _DGPS_H_
#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx_nucleo.h"

/*==================================================================================================================================*/
#define GPS_DATA_BUFFER_SIZE	500
#define CR						13
#define LF						10
#define STAR 					42
#define PSI 					36 // packet  starting identifier $
/*==================================================================================================================================*/ 
extern uint8_t operationMode; // status : 1  , config : 2

static uint8_t gps_data_buffer[GPS_DATA_BUFFER_SIZE];
static uint8_t gps_data[GPS_DATA_BUFFER_SIZE];

static _Bool nmea0183_message_ready = 0;

enum Latitude{NORTH = 1 , SOUTH = 2};
enum Longitude{EAST = 1 , WEST = 2};

typedef struct status{
	double trueHeading;
	uint32_t latitudeDecimalMunites;
	uint8_t latitudeDir;
	uint32_t longitudeDecimalMunites;
	uint8_t longitudeDir;
}Status;

typedef struct position{
	double latitude;
	double longitude;
}Position;

struct dgpsData{
	double heading;
	struct position Position;
};

 struct configuration{
	uint8_t JPORT;   // A:1 B:2 C:3 
 	uint16_t JBAUD;
	uint16_t JAGE;
	uint16_t JLIMIT;
	uint8_t JMASK;
	uint8_t JNP;
	Position JPOS;
	uint16_t JSMOOTH;
	double JHTAU;
	double JHRTAU;
	double JMSEP;
	uint8_t JGYROAID;
	uint8_t JTILTAID;
	uint8_t JFLIPBRD;
	double JHBIAS;
	double JPBIAS;
	double JPTAU;
	uint8_t JROLL;
	uint8_t JGPGGA;
	uint8_t JGPVTG;
	uint8_t JGPGSV;
	uint8_t JGPZDA;
	uint8_t JGPHDT;
	uint8_t JGPROT;
} ;

/*==================================================================================================================================*/
void nmea0183_message_parser(char data);
void check_nmea0183_message(void);
uint8_t configureDGPS(uint8_t *data);
void gpsData2Send(uint8_t * data , uint16_t * index);
void dgpsTest(void);
uint8_t checkResponse(char * param);
uint8_t checkMessageRates(void);
uint8_t isSaved(void);
/*==================================================================================================================================*/
#endif	/*	#ifndef _GTOP_GPS_H_	*/
/*==================================================================================================================================*/
