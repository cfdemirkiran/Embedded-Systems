
/*==================================================================================================================================*/
#include "dgps.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "peripheral_init.h"
#include "svs_communication.h"
#include "communication.h"
#include "utility.h"
#include "communication.h"
/*==================================================================================================================================*/
volatile static uint16_t gps_buffer_index=0, gps_data_index=0;

// initialize configuration parameters
struct configuration Configuration={
		.JPORT = 1,
		.JBAUD = 19200,
		.JAGE = 2700,  //second
		.JLIMIT = 10,
		.JMASK = 5,
		.JNP = 7,
		.JPOS = {51.4,52.5},
		.JSMOOTH = 900, //second
		.JHTAU = 2.0,  //second
		.JHRTAU = 2.0,	//second
		.JMSEP = 2.0, // meter
		.JGYROAID = 1,
		.JTILTAID = 1,
		.JFLIPBRD = 0,
		.JHBIAS = 0.0,
		.JPBIAS = 0.0,
		.JPTAU = 0.0,
		.JROLL = 0, // 0 : no 1 : yes
		.JGPGGA = 1,
		.JGPVTG = 0,
		.JGPGSV = 0,
		.JGPZDA = 0,
		.JGPHDT = 1,
		.JGPROT = 0
};

extern struct dgpsData DGPSData;
/*==================================================================================================================================*/
void nmea0183_message_parser(char data)
{
	if (nmea0183_message_ready)
		return;
	
	/* Buffer indexi buffer sınırına ulaşmış ise veri alma.	*/
	if (gps_buffer_index < GPS_DATA_BUFFER_SIZE) {
		if (gps_buffer_index < 1) { 
			if (data == PSI)
				gps_data_buffer[gps_buffer_index++] = data;
			else
				gps_buffer_index = 0;
		} else {
			gps_data_buffer[gps_buffer_index++] = data;
			if (data == LF) {
				//status_led_toggle();
				nmea0183_message_ready = 1;
			}
		}
	}
	else
	{
	/* Buffer indexi buffer sınırına ulaşmış fakat mesaj alınamamış ise indexi sıfırla.	*/
		gps_buffer_index = 0;
	}
}
/*==================================================================================================================================*/

uint8_t calc_checksum_val;
uint8_t message_Checsum_val;
void check_nmea0183_message(void)
{
	/*	GPS ten gelen veriyi buffera al	*/
	if (nmea0183_message_ready) {
		memcpy(gps_data, gps_data_buffer, gps_buffer_index);
		gps_data_index = gps_buffer_index;
		gps_buffer_index = 0;
		
		com_handler_message_ready = 1;
		
		//control checksum
		/*
		calc_checksum_val = checksum(gps_data+1, gps_data_index-5);
		message_Checsum_val = hexStringToUint8(gps_data[gps_data_index-4] , gps_data[gps_data_index-3]);
		*/
		
		if(com_handler_message_ready )
		{
		//uart2_putnc(gps_data, gps_data_index);
		}
		
		char * tokens;

		tokens = strtok((char*)gps_data, ","  );
		if(strcmp(tokens , "$GPHDT") == 0 )
		{
			tokens = strtok(NULL, ","  );
			DGPSData.heading =  atof(tokens);
			char head[10];
			//sprintf(head, "%.3f",DGPSData.heading);
			//uart2_putnc((uint8_t*)head, strlen(head));
		}
		else if(strcmp(tokens , "$GPGGA") == 0 )
		{
			// pass utc time
			tokens = strtok(NULL, ","  );
			// needed to be parsed, Latitude in degrees, minutes, and decimal minutes(DDMM.MMMMM)
			tokens = strtok(NULL, ","  );
			char* latInMin;// latitude in minutes
			latInMin = strtok(tokens, "."); // degree and minnute part
			uint16_t latitudeMinunte = atoi(latInMin);
			DGPSData.Position.latitude = (double)(latitudeMinunte/60.0);
			
			tokens = strtok(NULL, ","  );
			// needed to be parsed, Latitude in degrees, minutes, and decimal minutes(DDMM.MMMMM)
			tokens = strtok(NULL, ","  );
			char* longInMin;// latitude in minutes
			longInMin = strtok(tokens, "."); // degree and minnute part
			uint16_t longitudeMinunte = atoi(longInMin);
			DGPSData.Position.longitude = (double)(longitudeMinunte/60.0);			
			
			char temp[10];
			sprintf(temp, "%.3f",DGPSData.Position.latitude);
			//uart2_putnc((uint8_t*)temp, strlen(temp));
			sprintf(temp, "%.3f",DGPSData.Position.longitude);
			//uart2_putnc((uint8_t*)temp, strlen(temp));
			
			//tokens = strtok(NULL, ","  );

		}
				nmea0183_message_ready = 0;

	}

}
/*==================================================================================================================================*/
void gpsData2Send(uint8_t * data , uint16_t *index)
{
	memcpy( data , gps_data, gps_data_index );
	*index = gps_data_index;
}
/*==================================================================================================================================*/
void sendCommand2DGPS(uint8_t* command , uint8_t size)
{
		uint8_t checksum_val , commandSize;
		commandSize = size + 6;
		uint8_t command2Send[commandSize];
		command2Send[0] = PSI;
		memcpy(command2Send+1, command , size);
		checksum_val = checksum(command2Send + 1, size);
		command2Send[commandSize - 5] = STAR;
		command2Send[commandSize - 4] = hex_to_ascii(checksum_val, 1);
		command2Send[commandSize - 3] = hex_to_ascii(checksum_val, 0);
		command2Send[commandSize - 2] = CR;
		command2Send[commandSize - 1] = LF;

		uart1_putnc(command2Send, commandSize);
	
}
/*==================================================================================================================================*/
// send command to dgps without checksum
void sendCommand2DGPSwoCRC(uint8_t* command , uint8_t size)
{
		uint8_t  commandSize;
		commandSize = size + 3;
		uint8_t command2Send[commandSize];
		command2Send[0] = PSI;
		memcpy(command2Send+1, command , size);
		command2Send[commandSize - 2] = CR;
		command2Send[commandSize - 1] = LF;

		uart1_putnc(command2Send, commandSize);
	
}
/*==================================================================================================================================*/

uint8_t configureDGPS(uint8_t *data)
{
		uint8_t err =0;
	  // stop all ports firstly , go into silent mode :)
		const char closeAllPorts[] = "JOFF,ALL";
		sendCommand2DGPS( (uint8_t *)closeAllPorts, sizeof(closeAllPorts) );
	
		// get configuration data into struct
		if( operationMode == 2)	
			memcpy( &Configuration , data , sizeof(Configuration) );
	
		// start to configuration
		
		char command[80];
		char parameter[10];

		strcpy(command , "$JAGE,");
		sprintf(parameter , "%d" , Configuration.JAGE );
		strcat(command ,(const char *)parameter );
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JLIMIT,");
		sprintf(parameter , "%.1f" , (float)Configuration.JLIMIT );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JMASK,");
		sprintf(parameter , "%d" , Configuration.JMASK );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "$JNP,");
		sprintf(parameter , "%d" , Configuration.JNP );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		const char commandWAAS[] = "JWAASPRN,AUTO";
		sendCommand2DGPSwoCRC( (uint8_t *)commandWAAS, strlen(commandWAAS) );
		if( (err=checkResponse("$>")) != 0)
			return err;

		const char commandDIFF[] = "JDIFF,WAAS";
		sendCommand2DGPSwoCRC( (uint8_t *)commandDIFF , strlen(commandDIFF));
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "$JPOS,");
		sprintf(parameter , "%.1f,%.1f" , Configuration.JPOS.latitude , Configuration.JPOS.longitude );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JSMOOTH,");
		sprintf(parameter , "%d" , Configuration.JSMOOTH );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		const char commandAIR[] = "JAIR,AUTO";
		sendCommand2DGPSwoCRC( (uint8_t *)commandAIR , strlen(commandAIR));
		if( (err=checkResponse("JAIR,AUTO,NORM")) != 0)
			return err;
		
		const char commandALT[] = "JALT,NEVER";
		sendCommand2DGPSwoCRC( (uint8_t *)commandALT , strlen(commandALT));
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		/**********************************************/
		//send parameter to set
		const char commandTAUCOG[] = "JTAU,COG,0.00";
		sendCommand2DGPSwoCRC( (uint8_t *)commandTAUCOG , strlen(commandTAUCOG));

		if( (err=checkResponse("$>")) != 0)
			return err;

		const char commandTAUSPEED[] = "JTAU,SPEED,0.00";
		sendCommand2DGPSwoCRC( (uint8_t *)commandTAUSPEED , strlen(commandTAUSPEED));
		
		if( (err=checkResponse("$>")) != 0)
			return err;

		strcpy(command , "JATT,HTAU,");
		sprintf(parameter , "%.1f" , Configuration.JHTAU );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,HRTAU,");
		sprintf(parameter , "%.1f" , Configuration.JHRTAU );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		const char commandCOGTAU[] = "JATT,COGTAU,0.0";
		sendCommand2DGPS( (uint8_t *)commandCOGTAU , strlen(commandCOGTAU));
			
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,MSEP,");
		sprintf(parameter , "%.3f" , Configuration.JHRTAU );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,GYROAID,");
		if(Configuration.JGYROAID == 0 )
			sprintf(parameter , "NO" );
		else
			sprintf(parameter , "YES" );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,TILTAID,");
		if(Configuration.JTILTAID == 0 )
			sprintf(parameter , "NO" );
		else
			sprintf(parameter , "YES" );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		const char commandLEVEL[] = "JATT,LEVEL,NO";
		sendCommand2DGPS( (uint8_t *)commandLEVEL , strlen(commandLEVEL));
			
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		const char commandEXACT[] = "JATT,EXACT,NO";
		sendCommand2DGPS( (uint8_t *)commandEXACT , strlen(commandEXACT));
		
		if( (err=checkResponse("$>")) != 0)
			return err;

		const char commandHIGHMP[] = "JATT,HIGHMP,YES";
		sendCommand2DGPS( (uint8_t *)commandHIGHMP , strlen(commandHIGHMP));
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,FLIPBRD,");
		if(Configuration.JFLIPBRD == 0 )
			sprintf(parameter , "NO" );
		else
			sprintf(parameter , "YES" );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,HBIAS,");
		sprintf(parameter , "%.1f" , Configuration.JHBIAS );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		const char commandNEGTILT[] = "JATT,NEGTILT,NO";
		sendCommand2DGPS( (uint8_t *)commandNEGTILT , strlen(commandNEGTILT));
		
		if( (err=checkResponse("$>")) != 0)
			return err;

		const char commandNMEAHE[] = "JATT,NMEAHE,0";
		sendCommand2DGPS( (uint8_t *)commandNMEAHE , strlen(commandNMEAHE));
			
		if( (err=checkResponse("JATT,NMEAHE,OK")) != 0)
			return err;
		
		strcpy(command , "JATT,PBIAS,");
		sprintf(parameter , "%.1f" , Configuration.JPBIAS );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );		
			
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JATT,PTAU,");
		sprintf(parameter , "%.1f" , Configuration.JPTAU );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		
		strcpy(command , "JATT,ROLL,");
		if(Configuration.JTILTAID == 0 )
			sprintf(parameter , "NO" );
		else
			sprintf(parameter , "YES" );
		strcat(command ,(const char *)parameter );	
		sendCommand2DGPS( (uint8_t *)command, strlen(command) );
		
		if( (err=checkResponse("$>")) != 0)
			return err;

		
		const char commandSPDTAU[] = "JATT,SPDTAU,0.000";
		sendCommand2DGPS( (uint8_t *)commandSPDTAU , strlen(commandSPDTAU));
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
	
		const char commandSave[] = "JSAVE";
		sendCommand2DGPSwoCRC( (uint8_t *)commandSave, sizeof(commandSave) );
	  if( (err=isSaved() ) != 0)
			return err;
		
		
		char portSel[6];
		
		if(Configuration.JPORT == 2)
			sprintf(portSel , "PORTB" );
		else if (Configuration.JPORT == 3)
			sprintf(portSel , "PORTC" );
		else
			sprintf(portSel , "PORTA" );
		
		strcpy(command , "JASC,GPGGA,");
		sprintf(parameter , "%d" , Configuration.JGPGGA );
		strcat(command ,(const char *)parameter );	
		strcat(command ,(const char *)portSel );	
		//uart2_putnc((uint8_t*)command , strlen(command));
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JASC,GPVTG,");
		sprintf(parameter , "%d" , Configuration.JGPVTG );
		strcat(command ,(const char *)parameter );	
		strcat(command ,(const char *)portSel );	
		//uart2_putnc((uint8_t*)command , strlen(command));
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JASC,GPGSV,");
		sprintf(parameter , "%d" , Configuration.JGPGSV );
		strcat(command ,(const char *)parameter );	
		strcat(command ,(const char *)portSel );	
		//uart2_putnc((uint8_t*)command , strlen(command));
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JASC,GPZDA,");
		sprintf(parameter , "%d" , Configuration.JGPZDA );
		strcat(command ,(const char *)parameter );	
		strcat(command ,(const char *)portSel );	
		//uart2_putnc((uint8_t*)command , strlen(command));
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JASC,GPHDT,");
		sprintf(parameter , "%d" , Configuration.JGPHDT );
		strcat(command ,(const char *)parameter );	
		strcat(command ,(const char *)portSel );	
		//uart2_putnc((uint8_t*)command , strlen(command));
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		strcpy(command , "JASC,GPROT,");
		sprintf(parameter , "%d" , Configuration.JGPROT );
		strcat(command ,(const char *)parameter );	
		strcat(command ,(const char *)portSel );	
		//uart2_putnc((uint8_t*)command , strlen(command));
		sendCommand2DGPSwoCRC( (uint8_t *)command, strlen(command) );		
 		
		if( (err=checkResponse("$>")) != 0)
			return err;
		
		return err;
}

/*==================================================================================================================================*/
// return 0:ok 1:could not conf  2:no response
uint8_t checkResponse(char * param)
{
	for(int i = 0 ; i < 1000 ; i++)
	{
		if(nmea0183_message_ready)
			break;
		delay_ms(5);
	}
	if( !nmea0183_message_ready )
	{
		return 2;
	}
	else
	{

		memcpy(gps_data, gps_data_buffer, gps_buffer_index);
		gps_data_index = gps_buffer_index;
		gps_buffer_index = 0;
		//uart2_putnc(gps_data , gps_data_index );

		if ( strstr((char*)gps_data , param) != NULL )
		{
			nmea0183_message_ready = 0;
			return 0;
		}
		else
		{
			nmea0183_message_ready = 0;
			return 1;
		}
	}
	
}

/*==================================================================================================================================*/
// return 0:ok 1:could not conf  2:no response
uint8_t isSaved(void)
{
		char saveMessage[40];
start:
	for(int i = 0 ; i < 10000 ; i++)
	{
		if(nmea0183_message_ready)
			break;
		delay_ms(1);
	}
	if(nmea0183_message_ready ==  1 )
	{
		memcpy( (uint8_t*)saveMessage , gps_data_buffer, gps_buffer_index);
		gps_data_index = gps_buffer_index;
		gps_buffer_index = 0;
		//uart2_putnc((uint8_t*)saveMessage , gps_data_index );
		nmea0183_message_ready = 0;
					gps_buffer_index = 0;
		
		if(  strstr(saveMessage , "Save" ) == NULL)
			goto start;
		
		if(strstr(saveMessage , "Complete") == NULL)
			return 1;
		else
			return 0;
			
	}
	else
	{
		goto start;
	}
	
}

/*==================================================================================================================================*/
