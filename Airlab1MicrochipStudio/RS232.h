#ifndef __RS232_H__
#define __RS232_H__

void DoSerial();
void SetupSIO0();

// message format:
// 0xAAAAAAAA	0x0CCCCCCC 0x0DDDDDDD
//
// AA = Action
// CC = Channel
// DD = Data

#define SERIAL_ON								0x80	// On Switch
#define SERIAL_CUE							0x81	// Cue Switch
#define SERIAL_START							0x82  // Start
#define SERIAL_STOP							0x83  // Stop	
#define SERIAL_ONAIR1						0x84	// OnAir1
#define SERIAL_ONAIR2						0x85	// OnAir2
//															0x86 reserved
//															0x87 reserved
//															0x88 reserved
//															0x89 reserved
//															0x8A reserved
//															0x8B reserved
//															0x8C reserved
//															0x8D reserved
//															0x8E reserved
//															0x8F reserved
#define TIMER_START							0x90  // Timer Start
#define TIMER_STOP							0x91	// Timer Stop
#define TIMER_RESET							0x92	// Timer Reset
#define SERIAL_MODULESETUPDATA	0x93	// ModuleSetupData
//Byte 0:				TIMERLINEA
//Byte 1:				TIMERLINEB
//Byte 2:				TIMERMIC
//Byte 3:				STARTLINEA
//Byte 4:				STARTLINEB
//Byte 5:				STARTMIC
//Byte 6:				STARTFADER
//Byte 7:				STARTON
//Byte 8:				STARTCUE
//Byte 9:				STARTPULS
//Byte 10:			REMLINEA
//Byte 11:			REMLINEB
//Byte 12:			REMMIC
//Byte 13:			DJMIC
//Byte 14:			ANNMIC
//Byte 15:			STUDIOMIC
//Byte 16:			ONAIR1LINEA
//Byte 17:			ONAIR1LINEB
//Byte 18:			ONAIR1MIC
//Byte 19:			ONAIR2LINEA
//Byte 20:			ONAIR2LINEB
//Byte 21:			ONAIR2MIC
//Byte 22:			POWERON
//Byte 23:			MICATLINEA
//Byte 24:			MICATLINEB
//Byte 25:			REMONMIC
//Byte 26:			REMONLINEA
//Byte 27:			REMONLINEB
//Byte 28:			SWITCHSTART


#define SERIAL_MASTERSETUPDATA	0x94	// MasterSetupData
//Byte 0:				AUTOCUERESET
//Byte 1:				CUETOCRM
//Byte 2:				AUTOCOM
//Byte 3:				RS232

#define SERIAL_CONFIG						0x95
// BYTE 0:			CHANNEL1
// BYTE 1:			CHANNEL2
// BYTE 2:			CHANNEL3
// BYTE 3:			CHANNEL4
// BYTE 4:			CHANNEL5
// BYTE 5:			CHANNEL6
// BYTE 6:			CHANNEL7
// BYTE 7:			CHANNEL8
// BYTE 8:			CHANNEL9
// BYTE 9:			CHANNEL10
// BYTE 10:			CHANNEL11
// BYTE 11:			CHANNEL12
// BYTE 12:			CHANNEL13
// BYTE 13:			CHANNEL14
// BYTE 14:			CHANNEL15
// BYTE 15:			CHANNEL16

#define		STORE_EEPROM					0x96
#define   STORE_SMARTCARD				0x97
#define 	CARD_INSERTED					0x98
//															0x99 reserved
#define   SERIAL_IDSTRING				0x9A
//															0x9B reserved
//															0x9C reserved
//															0x9D reserved
//															0x9E reserved
//															0x9F reserved

//															0xA0 reserved
//															0xA1 reserved
//															0xA2 reserved
//															0xA3 reserved
//															0xA4 reserved
//															0xA5 reserved
//															0xA6 reserved
//															0xA7 reserved
//															0xA8 reserved
//															0xA9 reserved
//															0xAA reserved
//															0xAB reserved
//															0xAC reserved
//															0xAD reserved
//															0xAE reserved
//															0xAF reserved

//																0xB0 reserved
//																0xB1 reserved
//																0xB2 reserved
#define GETSERIAL_MODULESETUPDATA	0xB3
#define GETSERIAL_MASTERSETUPDATA	0xB4
#define GETSERIAL_CONFIG	 				0xB5
//																0xB6 reserved
//																0xB7 reserved

#define GETCARD_INSERTED		 			0xB8
//																0xB9 reserved
#define GETSERIAL_IDSTRING				0xBA
//																0xBB reserved
//																0xBC reserved
//																0xBD reserved
//																0xBE reserved
//																0xBF reserved


//-- Addition for V2.0 (Director)


// message format:
// 0xAAAAAAAA	0x0CCCCCCC 0x0DDDDDDD
//
// AA = Action
// CC = Tolco channel
// DD = Data
#define TODIRECTOR_TELCO_CONNECT					 	0xC0
#define TODIRECTOR_TELCO_RING  							0xC1
#define TODIRECTOR_TELCO_CUE							 	0xC2
#define TODIRECTOR_TELCO_MUTE								0xC3

#define TODIRECTOR_FUNCTION								  0xD0	
	#define TODIRECTOR_CRMMUTE									0x00	// CRM Mute 						0xD0 0x00 Data
	#define TODIRECTOR_STUDIOMUTE								0x01	// STUDIO Mute					0xD1 0x01 Data
	#define TODIRECTOR_CUE_BY_MIC_REMOTE				0x02	// CUE_BY_MIC_REMOTE		0xD1 0x02 Data


// message format:
// 0xAAAAAAAA	0x0CCCCCCC 0x0DDDDDDD
//
// AA = Action
// CC = Tolco channel
// DD = Data
#define FROMDIRECTOR_TELCO_ON_CONNECT				0xE0
#define FROMDIRECTOR_TELCO_MUTE							0xE1
#define FROMDIRECTOR_TELCO_TB								0xE2

#define FROMDIRECTOR_TB											0xF0
	#define TB_CONSOLE													0x00	//TB Console 	0xF0 0x00 Data
	#define TB_ALL															0x01	//TB All 			0xF0 0x01 Data
	

//unsigned char IDString[30]= "D&R Airlab v2.04 - 08/2006";




#endif	