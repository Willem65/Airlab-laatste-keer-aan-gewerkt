/*
 * Airlab1MicrochipStudio.c
 *
 * Created: 16-5-2022 23:40:03
 * Author : willem
 */ 

#ifndef F_CPU
#define F_CPU 9000000 // 9 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
//#include <stdio.h>
//#include <math.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "Delay.h"
#include "ConsoleTimer.h"
#include "airlabbrm-willem.h"
#include "RS232.h"
#include "ReadModules.h"
#include "Events.h"






//--------------------------------------------------------------------
//// Test Eeprom x24c16
//#define RTC_ADDR 0xE0
//#define RTC_WRITE 0x00
//#define RTC_READ 0x01
//
//
//volatile unsigned char twi_no_of_bytes,twi_data_buffer[10],twi_data_count,twi_transfer_complete;
//unsigned char clock_arr[4],day_arr[7];
//
//
////void clock_init(void);
//void init_twi(void);
//void twi_write(unsigned char reg_addr,unsigned char reg_data);
//
//void twi_read(void);
//void get_data(void);

#include "TWI.h"

#define TWI_DEVICE_ADDRESS						0x70

#define TARGET_REGISTER							0x03
#define BYTES_TO_SEND							0x03

uint8_t I2C_WriteBuffer[TWI_BUFFER_SIZE];
uint8_t I2C_ReadBuffer[TWI_BUFFER_SIZE];
uint8_t I2C_SlaveBuffer[TWI_BUFFER_SIZE];

unsigned char EEread[29];
unsigned char EEread2[29];
unsigned char wr[29];




//// Test Eeprom x24c16
//--------------------------------------------------------------------









#define	PULSWIDTH						10
#define SET_SERIAL_WAIT			5
#define ONAIR1_FUNCTION								0x20
#define ONAIR2_FUNCTION						 		0x10
#define STUDIOMUTE_FUNCTION_LED				0x04
#define MASTER_CUE_ON_FUNCTION_LED		0x02
#define CRMMUTE_FUNCTION_LED					0x01


#define		CLK1  0x10
#define		DATA1  0x8
#define		STRB1  0x1

#define		CLK2  0x40
#define		DATA2  0x20
#define		STRB2  0x2

//unsigned char CLK2;
//unsigned char DATA2;
//unsigned char STRB2;

//void Delay(int Tijd);
void SetupLCD(void);
void SetLCD(int RS, char Buffer[]);

unsigned char ConsoleModuleSettings[NROFMODULES][29];

unsigned char Wait; 
unsigned char OperatingMode;
unsigned char SerialByte;
unsigned char SerialLength;
unsigned char SerialCnt;
unsigned char DirectorTBCUEBussActive;
unsigned char DummySerialBuffer[256];
unsigned char SerialBufferIn[256];
unsigned char SerialBufferInPtrTop;
unsigned char MasterData[8];
unsigned char ModuleData[NROFMODULES][8];
unsigned char EventBufferPtrTop;
unsigned char EventBufferPtrBottom;
unsigned char ModuleType[NROFMODULES];
unsigned char ConsoleFunctionData[17];
unsigned char cntStartPuls[NROFMODULES];
unsigned char SendSerialData;
unsigned char SerialBufferOut[256];
unsigned char SerialBufferOutPtrTop;
unsigned char SerialBufferOutPtrBottom;
unsigned char SerialBufferInPtrBottom;
unsigned char ShowFunctionData[17];
unsigned char ConsoleMasterSettings[5];
unsigned char SendSerialReady;
unsigned int  EventBuffer[256];

unsigned char cntModule;
unsigned char StartActive[NROFMODULES];
unsigned char TextStringPtr;
unsigned char NrOfTelcos;
unsigned char NrOfTriples;

unsigned char CardInserted;

unsigned char cntStartFunction[NROFMODULES];
unsigned char cntCueFunction[NROFMODULES];
unsigned char cntMuteFunction[NROFMODULES];

unsigned char CUEByRemoteMIC[NROFMODULES];

char GetTelcoNrFromModuleNr(char ModuleNr);

void CueControl(void);

unsigned char TextString[4][17];
unsigned char	ModuleTextString[29][17];
unsigned char	MasterTextString[4][17];
unsigned char	SystemTextString[6][17];

unsigned char TempEnableSetup;

void TimerControl(unsigned char Module);

unsigned char TimerActive[NROFMODULES];


//unsigned char ConsoleModuleSettings[NROFMODULES][29];
//unsigned char ConsoleMasterSettings[5];

unsigned char UndoConsoleModuleSettings[NROFMODULES][29];

unsigned char UndoConsoleMasterSettings[5];


//unsigned char I2CAddressBufferOut[256];
//unsigned char I2CDataBufferOut[256];
//unsigned char I2CBufferIn[256];
//
unsigned char I2CError;
unsigned char I2CMasterReciever;
//unsigned char I2CStatus;

//unsigned char CurrentAddress;
unsigned char I2CBufferInPtrTop;
unsigned char I2CBufferInPtrBottom;
unsigned char I2CBufferOutPtrTop;
unsigned char I2CBufferOutPtrBottom;
//unsigned char I2CBuffer[256];
//unsigned char I2CLength;
//unsigned char I2CDataLength;
//unsigned char I2CRepeat;
//unsigned char I2CRepeatAddress;
unsigned char I2CSendReady;
//unsigned char I2CByteNr;
//unsigned char I2CFromAddress;
//unsigned char I2CItem;
//unsigned char I2CModule;


void TimerControl(unsigned char Module)
{
	unsigned char SetTimerOn = 0;
	unsigned char cntModule;

	if (ModuleData[Module][MIC_RING]&0x80)
	{
		if (ConsoleModuleSettings[Module][TIMERMIC])
		SetTimerOn = 1;
	}
	else if (!(ModuleData[Module][LINEB]&0x80))
	{
		if (ConsoleModuleSettings[Module][TIMERLINEA])
		SetTimerOn = 1;
	}
	else if (ModuleData[Module][LINEB]&0x80)
	{
		if (ConsoleModuleSettings[Module][TIMERLINEB])
		SetTimerOn = 1;
	}

	if (SetTimerOn)
	{
		if (((ModuleData[Module][FADERON]&0x80)) && ((ConsoleFunctionData[Module]&ON_LED)))
		{// !JB
			TimeAndDate.Hours = 0;
			TimeAndDate.Minutes = 0;
			TimeAndDate.Seconds = 0;
			TimeAndDate.Frames = 0;
			TimerActive[Module] = 1;
		}
		else
		{
			TimerActive[Module] = 0;
		}
	}
	else
	{
		TimerActive[Module] = 0;
	}

	GlobalTimerActive = 0;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (TimerActive[cntModule])
		GlobalTimerActive = 1;
	}
}


void ShowFunction(unsigned char Function)
{
	unsigned char cnt;
	for (cnt=0; cnt<NROFMODULES; cnt++)
	{
		ShowFunctionData[cnt] &= CUE_LED^0xFF;
		if (ConsoleModuleSettings[cnt][Function])
		{
			ShowFunctionData[cnt] |= CUE_LED;
		}
	}
	SendSerialData = 1;
}


char GetTelcoNrFromModuleNr(char ModuleNr)
{
	char cntTelco = -1;
	char TelcoNr = -1;
	char cntModule;
	
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (TelcoNr == -1)
		{
			if (ModuleType[cntModule] == TELCO)
			{
				cntTelco++;
				
				if (cntModule == ModuleNr)
				{
					TelcoNr = cntTelco;
				}
			}
		}
	}

	return TelcoNr;
}

void CheckTBMic()
{
	char TBByTelco = 0;
	char cntModule;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (ModuleType[cntModule] == TELCO)
		{
			if (ConsoleFunctionData[cntModule]&CUE_FUNCTION)
			{
				TBByTelco = 1;
			}
		}
	}

	if ((ConsoleFunctionData[16]&ANNOUNCER_COMM_FUNCTION_LED) || (TBByTelco))
	{
		//TBL_MUTE = 0x00;   // willem
	}
	else
	{
		//TBL_MUTE = 0xFF;   // willem
	}
}

void StudioMicControl()
{
	unsigned char cntModule;
	unsigned char StudioMicActive=0;

	ConsoleFunctionData[16] &= STUDIOMUTE_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (((ConsoleModuleSettings[cntModule][STUDIOMIC]) || (ConsoleModuleSettings[cntModule][ANNMIC])) && ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO)))
		{
//v1.11
/*			if (!MIC_AND_LINE_MUTE)
			{
				if (ModuleData[cntModule][MIC_RING]&0x80)
				{
					if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED)))
						StudioMicActive = 1; // !JB
				}
			}
			else
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED)))
					StudioMicActive = 1; // !JB
			}*/
			if ((ModuleData[cntModule][MIC_RING]&0x80) ||
				 ((!(ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEA])) ||
				 (((ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEB])))
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED)))
					StudioMicActive = 1; // !JB
			}
		}
	}

	if (StudioMicActive)
		ConsoleFunctionData[16] |= STUDIOMUTE_FUNCTION_LED;		

	if (ConsoleFunctionData[16]&STUDIOMUTE_FUNCTION_LED)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_STUDIOMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_STUDIOMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}

	SendSerialData = 1;
}







void DJControl()
{
	unsigned char cntModule;
	unsigned char DJMicActive=0;

	ConsoleFunctionData[16] &= CRMMUTE_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if ((ConsoleModuleSettings[cntModule][DJMIC]) && ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO)))
		{
//v1.11
/*			if (!MIC_AND_LINE_MUTE)
			{
				if (ModuleData[cntModule][MIC_RING]&0x80)
				{
					if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED) || (ConsoleFunctionData[cntModule]&CUE_FUNCTION))
						DJMicActive = 1; // !JB
				}
			}
			else
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED) || (ConsoleFunctionData[cntModule]&CUE_FUNCTION))
					DJMicActive = 1; // !JB
			}
		}*/
//v1.15
			if ((ModuleData[cntModule][MIC_RING]&0x80) ||
				 ((!(ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEA])) || (((ModuleData[cntModule][LINEB]&0x80)) && (ConsoleModuleSettings[cntModule][MICATLINEB])))
			{
				if (((ModuleData[cntModule][FADERON]&0x80)) && ((ConsoleFunctionData[cntModule]&ON_LED) || (ConsoleFunctionData[cntModule]&CUE_FUNCTION)))
					DJMicActive = 1; // !JB
			}
		}
	}

	if (DJMicActive)
		ConsoleFunctionData[16] |= CRMMUTE_FUNCTION_LED;		

	if (ConsoleFunctionData[16]&CRMMUTE_FUNCTION_LED)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_CRMMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
		SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_CRMMUTE;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}
	SendSerialData = 1;
}

void OnAirControl()
{
	unsigned char cntModule;
	unsigned char OnAir1=0;
	unsigned char OnAir2=0;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if  ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO))
		{
			if (ModuleData[cntModule][MIC_RING]&0x80)
			{
				if (ConsoleModuleSettings[cntModule][ONAIR1MIC])
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
				OnAir1 = 1;	 // !JB
				if (ConsoleModuleSettings[cntModule][ONAIR2MIC])
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
				OnAir2 = 1;	 // !JB
			}
			else if (!(ModuleData[cntModule][LINEB]&0x80))
			{
				if (ConsoleModuleSettings[cntModule][ONAIR1LINEA])
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
				OnAir1 = 1;	// !JB
				if (ConsoleModuleSettings[cntModule][ONAIR2LINEA])
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
				OnAir2 = 1;	// !JB
			}
			else if (ModuleData[cntModule][LINEB]&0x80)
			{
				if (ConsoleModuleSettings[cntModule][ONAIR1LINEB])
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
				OnAir1 = 1;	 // !JB
				if (ConsoleModuleSettings[cntModule][ONAIR2LINEB])
				if (((ModuleData[cntModule][FADERON]&0x80)) && (ConsoleFunctionData[cntModule]&ON_LED))
				OnAir2 = 1;	// !JB
			}
		}
	}
	
	ConsoleFunctionData[16] &= ONAIR1_FUNCTION^0xFF;
	ConsoleFunctionData[16] &= ONAIR2_FUNCTION^0xFF;
	if (OnAir1)
	ConsoleFunctionData[16] |= ONAIR1_FUNCTION;
	if (OnAir2)
	ConsoleFunctionData[16] |= ONAIR2_FUNCTION;

	if (ConsoleFunctionData[16]&ONAIR1_FUNCTION)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR1;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR1;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}

	if (ConsoleFunctionData[16]&ONAIR2_FUNCTION)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR2;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_ONAIR2;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
	}
	SendSerialData = 1;
}

void CheckStartCondition(unsigned char Module)
{
	unsigned char StartOn = 0;

	if (!ConsoleModuleSettings[Module][SWITCHSTART])
	{
		if  ((ConsoleModuleSettings[Module][STARTON]) && (ConsoleModuleSettings[Module][STARTFADER]))
		{
			if (((ModuleData[Module][FADERON]&0x80)) && (ConsoleFunctionData[Module]&ON_LED))
			{	 //!JB
				if (!StartActive[Module])
				{
					StartControl(Module,1);
					StartActive[Module] = 1;
				}
				StartOn = 1;
			}
			else
			{
				StartControl(Module,0);
				StartActive[Module] = 0;
			}
		}
		else if  ((ConsoleModuleSettings[Module][STARTON]) && (!(ConsoleModuleSettings[Module][STARTFADER])))
		{
			if (ConsoleFunctionData[Module]&ON_LED)
			{
				if (!StartActive[Module])
				{
					StartControl(Module,1);
					StartActive[Module] = 1;
				}
				StartOn = 1;
			}
			else
			{
				StartControl(Module,0);
				StartActive[Module] = 0;
			}
		}
		else if  ((!ConsoleModuleSettings[Module][STARTON]) && (ConsoleModuleSettings[Module][STARTFADER]))
		{
			if ((ModuleData[Module][FADERON]&0x80))
			{	 // !JB
				if (!StartActive[Module])
				{
					StartControl(Module,1);
					StartActive[Module] = 1;
				}
				StartOn = 1;
			}
			else
			{
				StartControl(Module,0);
				StartActive[Module] = 0;
			}
		}
	}
	if (ConsoleModuleSettings[Module][STARTCUE])
	{
		if (ConsoleFunctionData[Module]&CUE_FUNCTION)
		{
			StartControl(Module,1);
			StartActive[Module] = 1;
		}
		else if (!StartOn)
		{
			StartControl(Module,0);
			StartActive[Module] = 0;
		}
	}
}




void StartControl(unsigned char Module, unsigned char On)
{
	unsigned char Status = 0;

	ConsoleFunctionData[Module] &= START_FUNCTION^0xFF;
	if (On)
	{
		if (ModuleData[Module][MIC_RING]&0x80)
		{
			if (ConsoleModuleSettings[Module][STARTMIC])
			ConsoleFunctionData[Module] |= START_FUNCTION;
		}
		else if (!(ModuleData[Module][LINEB]&0x80))
		{
			if (ConsoleModuleSettings[Module][STARTLINEA])
			ConsoleFunctionData[Module] |= START_FUNCTION;
		}
		else if (ModuleData[Module][LINEB]&0x80)
		{
			if (ConsoleModuleSettings[Module][STARTLINEB])
			ConsoleFunctionData[Module] |= START_FUNCTION;
		}
	}
	
	cntStartPuls[Module] = 0;
	if ((ConsoleFunctionData[Module]&START_FUNCTION) && (ConsoleModuleSettings[Module][STARTPULS]))
	{
		cntStartPuls[Module] = PULSWIDTH;
	}

	if (ConsoleFunctionData[Module]&START_FUNCTION)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_START;
		SerialBufferOut[SerialBufferOutPtrTop++] = Module;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	else
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_STOP;
		SerialBufferOut[SerialBufferOutPtrTop++] = Module;
		SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
	}
	SendSerialData = 1;
}


/*****************************************************************************************************
* SetConsoleData aansturen van de LEDS en module instellingen audio routing  en REMOTE en START                                             *
******************************************************************************************************/
void SetConsoleData()
{
	unsigned char cntByte, cntBit;  // byte   bits
	
	// Volgens mij is dit hieronder voor de MASTER MODULE
	for (cntBit=0; cntBit<8; cntBit++)	// 8 bits
	{
		//DATA1 = ConsoleFunctionData[16]&(0x80>>cntBit);   // AND de data met 1000 0000 . Data staat in :   ConsoleFunctionData[16]
		if (ConsoleFunctionData[16]&(0x80>>cntBit))
			PORTC_OUTSET = DATA1;
		else
			PORTC_OUTCLR = DATA1;
		
		PORTC_OUTSET = CLK1;
		//CLK2 = 1;
		for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);		// Wacht
		//CLK1 = 0;
		PORTC_OUTCLR = CLK1;
	
	}

	// Overige CONSOLE MODULE 1-16
	for (cntByte=0; cntByte<8; cntByte++)
	{
		for (cntBit=0; cntBit<6; cntBit++)								// 8 * 6 = 48
		{
			if ((OperatingMode==MODULEMODE) && ((0x01<<cntBit)&CUE_LED))   // Als betreffende module AND CUE_LED=1
			{
				//DATA1 = ShowFunctionData[7-cntByte]&(0x01<<cntBit);						// ShowFunctionData
				if ( ShowFunctionData[7-cntByte]&(0x01<<cntBit) )
					PORTC_OUTSET = DATA1;
				else
					PORTC_OUTCLR = DATA1;
				
				//DATA2 = ShowFunctionData[15-cntByte]&(0x01<<cntBit);
				if ( ShowFunctionData[15-cntByte]&(0x01<<cntBit) )
					PORTC_OUTSET = DATA2;
				else
					PORTC_OUTCLR = DATA2;
			}
			else
			{
				//DATA1 = ConsoleFunctionData[7-cntByte]&(0x01<<cntBit);        // ConsoleFunctionData
				if ( ConsoleFunctionData[7-cntByte]&(0x01<<cntBit) )
					PORTC_OUTSET = DATA1;
				else
					PORTC_OUTCLR = DATA1;
				
				//DATA2 = ConsoleFunctionData[15-cntByte]&(0x01<<cntBit);
				if ( ConsoleFunctionData[15-cntByte]&(0x01<<cntBit) )
					PORTC_OUTSET = DATA2;
				else
					PORTC_OUTCLR = DATA2;				
			}
			//CLK1 = 1;
			PORTC_OUTSET = CLK1;
			//CLK2 = 1;
			PORTC_OUTSET = CLK2;
			for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);		// Wacht
			//CLK1 = 0;
			PORTC_OUTCLR = CLK1;
			//CLK2 = 0;
			PORTC_OUTCLR = CLK2;
		}
	}
	PORTB_OUTCLR = STRB1;
	for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);
	PORTB_OUTSET = STRB1;
	//STRB2 = 0;
	//for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);
	//STRB2 = 1;
	PORTC_OUTCLR = STRB2;
	for (Wait=0; Wait<SET_SERIAL_WAIT; Wait++);
	PORTC_OUTSET = STRB2;	
}






void CheckCUEByRemoteMIC(void)
{
	char Status = 0;
	int cntModule;

	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if  (ModuleType[cntModule]==TRIPLE)
		{
			if (ModuleData[cntModule][MIC_RING]&0x80)
			{
				Status |= CUEByRemoteMIC[cntModule];
			}
		}
	}

	SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_FUNCTION;
	SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_CUE_BY_MIC_REMOTE;
	SerialBufferOut[SerialBufferOutPtrTop++] = Status;
}


char GetModuleNrFromTelcoNr(char TelcoNr)
{
	char cntTelco = -1;
	char ModuleNr = -1;
	char cntModule;
	
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (ModuleNr == -1)
		{
			if (ModuleType[cntModule] == TELCO)
			{
				cntTelco++;
			}

			if (cntTelco == TelcoNr)
			{
				ModuleNr = cntModule;
			}
		}
	}

	return ModuleNr;
}



void CueControl()
{
	unsigned char cntModule;
	unsigned char CueActive=0;

	ConsoleFunctionData[16] &= MASTER_CUE_ON_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if ((ConsoleFunctionData[cntModule]&CUE_FUNCTION) && ((ModuleType[cntModule]==TRIPLE) || (ModuleType[cntModule]==TELCO)))
		CueActive = 1;
	}

	if (DirectorTBCUEBussActive != 0)
	{
		CueActive = 1;
	}

	if ((CueActive) && (ConsoleMasterSettings[CUETOCRM]))
	ConsoleFunctionData[16] |= MASTER_CUE_ON_FUNCTION_LED;

	//Check auto communication
	ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		if (ConsoleFunctionData[cntModule]&CUE_LED)
		{
			if ((ModuleData[cntModule][MIC_RING]&0x80) && (ConsoleModuleSettings[cntModule][ANNMIC]) && (ConsoleFunctionData[cntModule]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
			{
				ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
			}
		}
	}
	CheckTBMic();

	SendSerialData = 1;
}





//########################################################################################################################################
//########################################################################################################################################
//########################################################################################################################################
//########################################################################################################################################
//########################################################################################################################################

void Init(void)
{	
	
	unsigned char cntByte;
	
	unsigned int Address;
	unsigned char cntModule;

	unsigned char ClearMem = 0;
	
	_delay_ms(2);
	//############## PORT DIRECTION REGISTERS ######################
	
	PORTA_DIRSET = 0b01111111;		// Initialise port als uitgang LCD DISPLAY
	PORTA_DIRSET = 0b10000000;      // RST_CC uitgang even als test pin
	
	
	PORTR_DIRCLR = 0b00000011;		// Initialise port als ingang KNOPJES INLEZEN  ( SW1 t/m SW2   Zie orginele schema muxboard airlb6a )
	PORTD_DIRCLR = 0b11111100;		// Initialise port als ingang KNOPJES INLEZEN  ( SW2 t/m SW7   Zie orginele schema muxboard airlb6a )


	PORTB_DIRSET = 0b00001110;		// Initialise port als uitgang KNOPJES SCANNEN  ( AS3 AS0 en AS1  Zie orginele schema muxboard airlb6a )
	PORTC_DIRSET = 0b00000101;		// Initialise port als uitgang KNOPJES SCANNEN  ( AS4 en AS2      Zie orginele schema muxboard airlb6a )

	PORTE_DIRCLR = 0b00000100;      //  Initialise port als ingang   RX RS232
	PORTE_DIRSET = 0b00001000;	    //  Initialise port als uitgang  TX RS232

	PORTC_DIRSET = 0b01111010;	    // Initialise port als uitgang LEDJES AANSTUREN  CLK2 DATA2 CLK1 DATA1 en STRB2
	PORTB_DIRSET = 0b00000001;	    // Initialise port als uitgang LEDJES AANSTUREN  STRB1 
	
		PORTE_DIRCLR = 0b00000011;      //  Initialise port als ingang   IIC
		//PORTE_DIRSET = 0b00000010;	    //  Initialise port als uitgang  IIC
	

	
	
	
	//##############################################################	
	
	SetupSIO0();  // Setup Serial port	
	SetupLCD();
	

	

	
	//-----------------------------------------------------------------------------------------------	
	//    Instellen van de interne clock osc
	//-----------------------------------------------------------------------------------------------	


	//////----- INTERNE OSCILLATOR OP 32 MHz --------- zonder pll multiplier-----------------------------
	////CCP = CCP_IOREG_gc;                // disable register security for oscillator update
	////OSC.CTRL = OSC_RC32MEN_bm;         // enable 32MHz oscillator
	////while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // wait for oscillator to be ready
	////CCP = CCP_IOREG_gc;                //disable register security for clock update
	////CLK.CTRL = CLK_SCLKSEL_RC32M_gc;   // switch to 32MHz clock
	//////-----------------------------------------------------------------------------------------------


	unsigned char n,s;

	s=SREG;


	// Internal 2 MHz RC oscillator initialization
	// Enable the internal 2 MHz RC oscillator
	OSC.CTRL|=OSC_RC2MEN_bm;
	// Disable the auto-calibration of the internal 2 MHz RC oscillator
	DFLLRC2M.CTRL&= ~DFLL_ENABLE_bm;

	// Wait for the internal 2 MHz RC oscillator to stabilize
	while ((OSC.STATUS & OSC_RC2MRDY_bm)==0);

	// PLL initialization
	// PLL clock source: 2 MHz Internal Osc.
	// PLL multiplication factor: 18
	// PLL output/2: Off
	// PLL frequency: 36,000000 MHz
	// Set the PLL clock source and multiplication factor
	n=(OSC.PLLCTRL & (~(OSC_PLLSRC_gm | OSC_PLLDIV_bm | OSC_PLLFAC_gm))) |
	OSC_PLLSRC_RC2M_gc | (0<<OSC_PLLDIV_bp) | 18;
	CCP=CCP_IOREG_gc;
	OSC.PLLCTRL=n;
	// Enable the PLL
	OSC.CTRL|=OSC_PLLEN_bm;

	// System Clock prescaler A division factor: 4
	// System Clock prescalers B & C division factors: B:1, C:1
	// ClkPer4: 9000,000 kHz
	// ClkPer2: 9000,000 kHz
	// ClkPer:  9000,000 kHz
	// ClkCPU:  9000,000 kHz
	n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
	CLK_PSADIV_4_gc | CLK_PSBCDIV_1_1_gc;
	CCP=CCP_IOREG_gc;
	CLK.PSCTRL=n;

	// Wait for the PLL to stabilize
	while ((OSC.STATUS & OSC_PLLRDY_bm)==0);

	// Select the system clock source: Phase Locked Loop
	n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_PLL_gc;
	CCP=CCP_IOREG_gc;
	CLK.CTRL=n;

	// Disable the unused oscillators: 32 MHz, internal 32 kHz, external clock/crystal oscillator
	OSC.CTRL&= ~(OSC_RC32MEN_bm | OSC_RC32KEN_bm | OSC_XOSCEN_bm);

	// ClkPer output disabled
	PORTCFG.CLKEVOUT&= ~(PORTCFG_CLKOUTSEL_gm | PORTCFG_CLKOUT_gm);
	// Restore interrupts enabled/disabled state
	SREG=s;

	
	

	_delay_ms(2);	

	
	OperatingMode = NORMALMODE;
	//GlobalTimerActive = 1;   // willem
	DirectorTBCUEBussActive = 0;
	
	//ConsoleFunctionData[0] |= 0x10;  // Even voor de Test : ConsoleFunctionData[module] |= ON_LED   LED wordt op des betreffende module aan of uit gezet plus overige functies mogelijk
	
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		ConsoleFunctionData[cntModule] = MUTE_FUNCTION;
		ShowFunctionData[cntModule] = 0x00;
	}
	ConsoleFunctionData[16] = NOT_POWER_ON_CC;
	
	SetConsoleData();
	
	//TBL_MUTE = 0xFF;
	//CS_CC = 0x00;

	//CLK1 = 0;
	//CLK2 = 0;
	//STRB1 = 1;
	//STRB2 = 1;
	
		
	
	OldTimeAndDate.Hours = 0;
	OldTimeAndDate.Minutes = 0;
	OldTimeAndDate.Seconds = 0;
	OldTimeAndDate.Frames = 0;
	TimeAndDate.Hours = 0;
	TimeAndDate.Minutes = 0;
	TimeAndDate.Seconds = 0;
	TimeAndDate.Frames = 0;
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		TimerActive[cntModule] = 0;
		cntStartPuls[cntModule] = 0;
		cntStartFunction[cntModule] = 0;
		cntCueFunction[cntModule] = 0;
		cntMuteFunction[cntModule] = 0;
		StartActive[cntModule] = 0;
		CUEByRemoteMIC[cntModule] = 0;
	}
	GlobalTimerActive = 0;
	TextStringPtr = 0;

	SerialBufferInPtrTop = 0;
	SerialBufferInPtrBottom = 0;
	SerialBufferOutPtrTop = 0;
	SerialBufferOutPtrBottom = 0;
	SerialCnt = 0;
	SerialLength = 0;
	SendSerialReady = 1;
	
	

	

	I2CBufferInPtrTop = 0;
	I2CBufferInPtrBottom = 0;
	I2CBufferOutPtrTop = 0;
	I2CBufferOutPtrBottom = 0;
	I2CSendReady=1;
	I2CMasterReciever = 0;
	I2CError = 0;
	CardInserted = 0;
	
	//SetupSIO1();
			
	SetupTimer0();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// Set display 
	SetLCD(1,"   D&R Airlab   ");
	SetLCD(0,"\xC0");
	SetLCD(1,"  Initializing  ");	
	
	NrOfTelcos = 0;
	NrOfTriples = 0;
	
	//EA=1;
	//AA=1;
	
	
	

	/*
		Initialize the interrupts
			-> Enable low level interrupts
			-> Enable global interrupts
	*/	
	TWIM_InitInterrupt();
	//TWIM_Init();
	PMIC_CTRL |= PMIC_LOLVLEN_bm;
	sei();
	
	uint16_t wacht;
	
	
	
	
	
	
	//**********************  Hieronder allemaal code om te testen of het goed werkt *************************************
	
	//for (wacht=0; wacht<50000; wacht++);
	//PORTA_OUTTGL |= 0x80;  // om als trigger puls te gebruiken voor de oscilloscoop
	//for (wacht=0; wacht<50000; wacht++);
	//for (wacht=0; wacht<50000; wacht++);
	//unsigned char EEread[16], wr[16];
		//

	//
	//for (int tm=0; tm<16; tm++)
	//{
		//wr[tm]=1;
	//}
	//
	//TWIM_Transmit(I2CAddressEEProm, 0x0, wr, 16);
	//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));	// Hier wordt luk raak gedaan als of er allemaal tripel kanalen in zitten
	
	//TWIM_Transmit(I2CAddressEEProm, 0x10, wr, 16);
	//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));			
	//
	//TWIM_Transmit(I2CAddressEEProm, 0x20, wr, 16);
	//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));	
				
	
	////------------------
	//
			//wr[0]=1;
			//wr[1]=0;
			//wr[2]=0;
			//wr[3]=0;
			//wr[4]=0;
			//wr[5]=0;
			//wr[6]=0;
			//wr[7]=1;
			//
			//wr[8]=1;
			//wr[9]=0;
			//wr[10]=0;
			//wr[11]=0;
			//wr[12]=0;
			//wr[13]=0;
			//wr[14]=0;
			//wr[15]=1;
			//
			////TWIM_Transmit(I2CAddressEEProm, 0x10, wr, 16);
			////while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
			//
			//wr[16]=1;
			//wr[17]=1;
			//wr[18]=1;
			//wr[19]=1;
			//wr[20]=1;
			//wr[21]=1;
			//wr[22]=1;
			//wr[23]=1;
			//
			//wr[24]=1;
			//wr[25]=1;
			//wr[26]=1;
			//wr[27]=1;
			//wr[28]=1;
			//
			////wr[0]=wr[16];
			////wr[1]=wr[17];
			////wr[2]=wr[18];
			////wr[3]=wr[19];
			////wr[4]=wr[20];
			////wr[5]=wr[21];
			////wr[6]=wr[22];
			////wr[7]=wr[23];
			////
			////wr[8]=wr[24];
			////wr[9]=wr[25];
			////wr[10]=wr[26];
			////wr[11]=wr[27];
			////wr[12]=wr[28];			
			//
			//
			////TWIM_Transmit(I2CAddressEEProm, 0x20, wr, 13);
			////while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
			//
			//
			//
			//
			//
			//
			//
		////-------------------
			//int vlag=1;
			//int bufsize = sizeof(wr);
			//
			//for (int tm=0; tm<bufsize; tm++)
			//{
				//if ( tm < 16 & vlag==1 )
				//{
					//vlag=2;
					//TWIM_Transmit(I2CAddressEEProm, 0x10, wr, 16);
					//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
				//}
				//
				//if ( tm > 15 )
				//{
					//wr[tm-16]=wr[tm];
				//}
			//}
			//
//
			//TWIM_Transmit(I2CAddressEEProm, 32, wr, 13);
			//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
		////-----------------------

		
		
	//**********************  Einde test code  *************************************
		
//---------------------------------------------------------------------------------------			
//            First Load all data from EEPROM (or Card?)
//---------------------------------------------------------------------------------------
	
	Address = 0x0000;
	
	// ModuleType
	TWIM_Receive(I2CAddressEEProm, Address, ModuleType, NROFMODULES);
	while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR)));
	
	Address += NROFMODULES;
	
	int t=0;
	
	// ConsoleModuleSettings
	TWIM_Receive(I2CAddressEEProm, Address, EEread, 29);
	while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR)));
	
	for (t=0; t<30; t++)
	{
		ConsoleModuleSettings[0][t]=EEread[t];
	}
		
	Address += 29;
	
	// ConsoleMasterSettings	
	TWIM_Receive(I2CAddressEEProm, Address, ConsoleMasterSettings, 5);
	while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR)));
	
	
//------------------------------------------------------------------------------------------------------------------------------------------------	


	Delay(10);	

	// Clear readbuffers
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		for (cntByte=0; cntByte<8; cntByte++)
		{
			ModuleData[cntModule][cntByte]=ATTACK;
		}
	}
	for (cntByte=0; cntByte<8; cntByte++)
	{
		MasterData[cntByte]=ATTACK;
	}
	
	Delay(10);

	for (cntModule=0; cntModule<100; cntModule++)
	{
		ReadModuleSwitches(MODULE1_8);
		ReadModuleSwitches(MODULE9_16);
	}
	EventBufferPtrTop = 0;
	EventBufferPtrBottom = 0;
	
	

	
	
	// Determine Nr Of Modules and Telco's
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		unsigned char Type = 0;

		if (ModuleData[cntModule][4]&0x80)
		Type = 0x02;
		if (ModuleData[cntModule][5]&0x80)
		Type |= 0x01;

		if (ModuleType[cntModule] != Type)
		{
			ModuleType[cntModule] = Type;
			SetLCD(0,"\xC0");
			SetLCD(1,"Config changed");
			Delay(20);

			ConsoleModuleSettings[cntModule][TIMERLINEA] = 0x01;
			ConsoleModuleSettings[cntModule][TIMERLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][TIMERMIC] = 0x00;
			ConsoleModuleSettings[cntModule][STARTLINEA] = 0x01;			
			ConsoleModuleSettings[cntModule][STARTLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][STARTMIC] = 0x00;
			ConsoleModuleSettings[cntModule][STARTFADER] = 0x01;
			ConsoleModuleSettings[cntModule][STARTON] = 0x00;     //7
			
			
			ConsoleModuleSettings[cntModule][STARTCUE] = 0x00;
			ConsoleModuleSettings[cntModule][STARTPULS] = 0x00;
			ConsoleModuleSettings[cntModule][REMLINEA] = 0x00;
			ConsoleModuleSettings[cntModule][REMLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][REMMIC] = 0x00;
			ConsoleModuleSettings[cntModule][DJMIC] = 0x00;
			ConsoleModuleSettings[cntModule][ANNMIC] = 0x00;
			ConsoleModuleSettings[cntModule][STUDIOMIC] = 0x00;   //15
			
			ConsoleModuleSettings[cntModule][ONAIR1LINEA] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR1LINEB] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR1MIC] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR2LINEA] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR2LINEB] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR2MIC] = 0x00;
			ConsoleModuleSettings[cntModule][POWERON] = 0x00;
			ConsoleModuleSettings[cntModule][MICATLINEA] = 0x00;   //23
			
			ConsoleModuleSettings[cntModule][MICATLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][REMONMIC] = 0x00;
			ConsoleModuleSettings[cntModule][REMONLINEA] = 0x00;
			ConsoleModuleSettings[cntModule][REMONLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][SWITCHSTART] = 0x00;  //28
		}

		if (ModuleType[cntModule] == TELCO)
		{
			ConsoleModuleSettings[cntModule][TIMERMIC] = 0x00;
			ConsoleModuleSettings[cntModule][STARTLINEA] = 0x01;
			ConsoleModuleSettings[cntModule][STARTLINEB] = 0x01;
			ConsoleModuleSettings[cntModule][STARTMIC] = 0x00;
			ConsoleModuleSettings[cntModule][STARTFADER] = 0x00;
			ConsoleModuleSettings[cntModule][STARTON] = 0x01;
			ConsoleModuleSettings[cntModule][STARTPULS] = 0x00;
			ConsoleModuleSettings[cntModule][DJMIC] = 0x00;
			ConsoleModuleSettings[cntModule][ANNMIC] = 0x00;
			ConsoleModuleSettings[cntModule][STUDIOMIC] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR1MIC] = 0x00;
			ConsoleModuleSettings[cntModule][ONAIR2MIC] = 0x00;
			ConsoleModuleSettings[cntModule][MICATLINEA] = 0x00;
			ConsoleModuleSettings[cntModule][MICATLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][REMONMIC] = 0x00;
			ConsoleModuleSettings[cntModule][REMONLINEA] = 0x00;
			ConsoleModuleSettings[cntModule][REMONLINEB] = 0x00;
			ConsoleModuleSettings[cntModule][SWITCHSTART] = 0x00;
		}
	
	

	}
	
	//ModuleType[0] = 1;
	//ModuleType[1] = 1;
	
		 
	
	
	// Determine Startup setting
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		ConsoleFunctionData[cntModule] = 0x00;
		if (ConsoleModuleSettings[cntModule][POWERON])
		{
			ConsoleFunctionData[cntModule] |= ON_LED;
			if ((ConsoleModuleSettings[cntModule][STARTON]) && (!ConsoleModuleSettings[cntModule][STARTFADER]))
			StartControl(cntModule,1);

			if ((ModuleData[cntModule][FADERON]&0x80))
			{ // !JB
				if  ((ConsoleModuleSettings[cntModule][STARTON]) && (ConsoleModuleSettings[cntModule][STARTFADER]))
				StartControl(cntModule,1);

				ConsoleFunctionData[cntModule] |= REMOTE_FUNCTION;
			}
			else
			{
				ConsoleFunctionData[cntModule] |= MUTE_FUNCTION;
			}
		}
		else
		{
			ConsoleFunctionData[cntModule] |= MUTE_FUNCTION;
		}
		// Watchdog
		//PCON |= 0x10;
		//T3 = 0;
	}	
	
	ConsoleFunctionData[16] = NOT_POWER_ON_CC;

	for (cntModule; cntModule<NROFMODULES; cntModule++)
	{
		TimerControl(cntModule);
		// Watchdog
		//PCON |= 0x10;
		//T3 = 0;
	}

	//OnAir1/2 Control
	OnAirControl();

	//DJ-Control
	DJControl();

	//StudioMic Control
	StudioMicControl();

	//Cue Control
	CueControl();

	SetConsoleData();


	// Determine Nr Of Modules and Telco's
	for (cntModule=0; cntModule<NROFMODULES; cntModule++)
	{
		switch (ModuleType[cntModule])
		{
			case TRIPLE:
			{
				// Triple Module
				NrOfTriples++;
			}
			break;
			case TELCO:
			{
				// Telco Module
				NrOfTelcos++;
			}
			break;
		}
		// Watchdog
		//PCON |= 0x10;
		//T3 = 0;
	}
		
	/* Set display */
	SetLCD(0,"\x80");
	SetLCD(1,"   D&R Airlab   ");
	ShowConfig();
	
	Delay(1000);
	
	
	PORTD_OUTCLR = 0x01;
	PORTD_OUTCLR = 0x02;
	PORTD_OUTCLR = 0x04;
	PORTD_OUTCLR = 0x08;
	PORTD_OUTSET = 0x10;

	if ((PORTC_IN&0x43)==0x00)
	{
		ConsoleMasterSettings[4] = 0x01;
		SetLCD(0,"\xC0");
		SetLCD(1,"Security off    ");
	}
	
	Delay(50);
	SetLCD(0,"\x80");
	SetLCD(1,"                ");
	SetLCD(0,"\xC0");
	SetLCD(1,TextString[TextStringPtr]);
	
 	//// Watchdog
 	//PCON |= 0x10;
 	//T3 = 0;

 	//EA=1;
 	//AA = 1;
	 
 	//SetAddress(I2CAddressEEProm, 0x00);
	 
	 
	//###############################################################################################################################################################

	//uint16_t wacht;	
/*		
	TWIE_CTRL = 0x00;
	TWIE_MASTER_BAUD = 0x28;
	TWIE_MASTER_CTRLA = TWI_MASTER_ENABLE_bm;   // wordt in  master mode gezet
	//TWIE_MASTER_CTRLA = TWI_MASTER_INTLVL_LO_gc | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;  // Voor als je interrupt wilt gebruiken
	TWIE_MASTER_STATUS |= TWI_MASTER_BUSSTATE_IDLE_gc;
	TWIE_MASTER_CTRLB = 0x00;
	

		
		PORTA_OUTTGL |= 0x80;
			
		TWIE_MASTER_ADDR = (0x70 << 0x01) | 0x00;             // Address + writebit
		while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm));
		for (wacht=0; wacht<100; wacht++);
	
		TWIE_MASTER_DATA = 0xAB;                           // Reg Data write
		while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm));
		for (wacht=0; wacht<100; wacht++);
			
		TWIE_MASTER_DATA = 0xEB;                               // Data byte		
		while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm));
		for (wacht=0; wacht<100; wacht++);		
		
				TWIE_MASTER_DATA = 0x12;                               // Data byte
				while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm));
				for (wacht=0; wacht<1000; wacht++);	
			
		TWIE_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;            // STOP
	
		for (wacht=0; wacht<5000; wacht++);  // Het is echt nodig om hier een delay te zetten !!!! 10000
	

	
	uint8_t Data=0x00;
		
	
	//while(1)
	{
		
			TWIE_MASTER_ADDR = (0x70 << 0x01) | 0x00;             // Address + writebit
			while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm) );
			
			TWIE_MASTER_DATA = 0xAB;                           // Reg Data write
			while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm));
					
		
					TWIE_MASTER_ADDR = (0x70 << 0x01) | 0x01;             // Address + readbit
					while(!(TWIE_MASTER_STATUS & TWI_MASTER_RIF_bm));
					TWIE_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
					//Data = TWIE_MASTER_DATA;
	
		
		while(!(TWIE_MASTER_STATUS & TWI_MASTER_RIF_bm));				
		//Data = TWIE_MASTER_DATA;
		
		
		
		TWIE_MASTER_CTRLC = TWI_MASTER_ACKACT_bm;
		TWIE_MASTER_CTRLC |= TWI_MASTER_CMD_STOP_gc;
		
		PORTA_OUTTGL |= 0x80;  // om als trigger puls te gebruiken voor de oscilloscoop
		for (wacht=0; wacht<5000; wacht++);
	}
	
	
*/		



//// Fill the write buffer
		//for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		//{
			//I2C_WriteBuffer[i] = i + 0x05;
		//}
//
		///*
			//Initialize master mode TWI
				//-> Normal TWI
				//-> Master mode
				//-> No Timeout
				//-> No Interrupts
				//-> 100 kHz @ 2 MHz
		//*/
		//TWIM_Init();
	//
		//// Write the data into the target register
		//// Send the device address
		//TWIM_SendAddress(TWI_WRITE(TWI_DEVICE_ADDRESS));
		//
		//// Send the register address
		//TWIM_SendData(TARGET_REGISTER);
//
		//// Send the data bytes
		//for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		//{
			//TWIM_SendData(I2C_WriteBuffer[i]);
		//}
	//
		//// Send stop condition
		//TWIM_SendStop(TWI_ACK);
//
		//for (wacht=0; wacht<5000; wacht++);
//
////	while(1)
	//{
		//PORTA_OUTTGL |= 0x80;  // om als trigger puls te gebruiken voor de oscilloscoop
		//
		//// Read data from the target register
		//for(uint8_t i = 0x00; i < 0x03; i++)
		//{
			//// Send the device address
			//TWIM_SendAddress(TWI_WRITE(TWI_DEVICE_ADDRESS));
		//
			//// Send the register address
			//TWIM_SendData(TARGET_REGISTER);
		//
			//// Send the device address
			//TWIM_SendAddress(TWI_READ(TWI_DEVICE_ADDRESS));
			//
			//// Read the data bytes
			//for(uint8_t i = 0x00; i < (BYTES_TO_SEND - 1); i++)
			//{
				//I2C_ReadBuffer[i] = TWIM_ReadData(TWI_ACK);
			//}
//
			//I2C_ReadBuffer[BYTES_TO_SEND - 1] = TWIM_ReadData(TWI_NACK);
		//
			//// Send stop condition
			//TWIM_SendStop(TWI_NACK);
		//}
	//}
	
	
	
	
	
	

		// Fill the write buffer--------------------------------------------------

		/*
			Interrupt driven TWI example
		*/

		//// Fill the write buffer -------------------------------------------------
		//for(uint8_t i = 0x00; i < BYTES_TO_SEND; i++)
		//{
			//I2C_WriteBuffer[i] = i + 0x03;
		//}
	
		/*
			Initialize the TWI
				-> Normal TWI
				-> Master mode
				-> No Timeout
				-> Master interrupts (low level)
				-> 100 kHz @ 2 MHz
		*/
		
		//TWIM_InitInterrupt();
//
		///*
			//Initialize the interrupts
				//-> Enable low level interrupts
				//-> Enable global interrupts
		//*/
		//PMIC_CTRL = PMIC_LOLVLEN_bm;
		//sei();
	
		////Transmit the message
		//TWIM_Transmit(I2CAddressEEProm, TARGET_REGISTER,  I2C_WriteBuffer, BYTES_TO_SEND);
		
		//// Wait until data got transmitted
		//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
		
		//for (wacht=0; wacht<5000; wacht++);

		//// Read the device
		//for(uint8_t i = 0x00; i < 0x03; i++)
		//{
			//TWIM_Receive(I2CAddressEEProm, TARGET_REGISTER, I2C_ReadBuffer, BYTES_TO_SEND);
//
			//// Wait until data are received
			//while(!((TWIM_Status() == TWI_MASTER_RECEIVED) || (TWIM_Status() == TWI_MASTER_ERROR)));
		//}


		
		//for (wacht=0; wacht<500; wacht++);
		//PORTA_OUTTGL |= 0x80;  // om als trigger puls te gebruiken voor de oscilloscoop
		


		//while(1)
		  //;







}


//*****************************************************************************************************************************
//*****************************************************************************************************************************
//*****************************************************************************************************************************
//*****************************************************************************************************************************
//*****************************************************************************************************************************
int	main (void)
{	
	char TextBuffer[9];
	
	Init();

	//P10 = 0; //read master
	//P11 = 0;
	//P12 = 0;
	//P13 = 0;
	//P14 = 1;
	//if ((P5&0x3C)==0x00)
	//{
		//unsigned char Stop=0;
		//SetLCD(0,"\x80");
		//SetLCD(1,"Test mode       ");
		//SetLCD(0,"\xC0");
		//SetLCD(1,"                ");
//
		//while (!Stop)
		//{
			//unsigned char cnt;
//
			//// Watchdog
			//PCON |= 0x10;
			//T3 = 0;
			//
//
			//for (cnt=0; cnt<NROFMODULES; cnt++)
			//{
				//if (cntStartPuls[cnt])
				//{
					//cntStartPuls[cnt]--;
					//if (cntStartPuls[cnt]==0)
					//{
						//ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;
						//SendSerialData = 1;
					//}
				//}
			//}
			//
			//if (SendSerialData)
			//{
				//SendSerialData = 0;
				//SetConsoleData();
			//}
//
			//ReadConsole();
			//if (EventBufferPtrTop != EventBufferPtrBottom)
			//{
				//unsigned int Event = EventBuffer[EventBufferPtrBottom++];
				//unsigned char EventType = (Event&0x87) | ((Event&0x200)>>6);
				//unsigned char Module = (Event>>3)&0x0F;
				//unsigned char EventOn = !(Event&0x100);
				//char TestString2[17] = "Off at Module xx";
				//if (EventOn)
				//{
					//TestString2[0]=' ';
					//TestString2[1]='O';
					//TestString2[2]='n';
				//}
				//TestString2[14] = ((Module+1)/10)+0x30;
				//TestString2[15] = ((Module+1)%10)+0x30;
//
				//switch (EventType)
				//{
					//case EXTERNREMOTEIN:
					//{
						//char TestString1[17] = "'Ext. remote In'";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//ConsoleFunctionData[Module] |= START_FUNCTION;
						//cntStartPuls[Module] = PULSWIDTH;
						//ConsoleFunctionData[Module] ^= CUE_LED;
						//ConsoleFunctionData[Module] ^= REMOTE_FUNCTION;
						//SetConsoleData();
					//}
					//break;
					//case FADERON:
					//{
						//char TestString1[17] = "'Fader On'      ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
					//}
					//break;
					//case PFL:
					//{
						//char TestString1[17] = "'CUE'           ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
					//}
					//break;
					//case ON:
					//{
						//char TestString1[17] = "'ON'            ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
					//}
					//break;
					//case LINEB:
					//{
						//char TestString1[17] = "'LINE B'        ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
					//}
					//break;
					//case MIC_RING:
					//{
						//char TestString1[17] = "'MIC/RING'      ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
					//}
					//break;
					//case ESCAPE:
					//{
						//char TestString1[17] = "'ESCAPE'        ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case ENTER:
					//{
						//char TestString1[17] = "'ENTER'         ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case RIGHT:
					//{
						//char TestString1[17] = "'RIGHT'         ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case UP:
					//{
						//char TestString1[17] = "'UP'            ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case DOWN:
					//{
						//char TestString1[17] = "'DOWN'          ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case LEFT:
					//{
						//char TestString1[17] = "'LEFT'          ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case CUERESET:
					//{
						//char TestString1[17] = "'CUE RESET'     ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
					//case SW_CC:
					//{
						//char TestString1[17] = "'Card Switch'   ";
						//SetLCD(0,"\x80");
						//SetLCD(1,TestString1);
						//TestString2[3]=0;
					//}
					//break;
				//}
				//SetLCD(0,"\xC0");
				//SetLCD(1,TestString2);
			//}
//
			//P10 = 0; //read master
			//P11 = 0;
			//P12 = 0;
			//P13 = 0;
			//P14 = 1;
			//if ((P5&0x43)==0x00)
			//{
				//Stop = 1;
//
				//SetLCD(0,"\x80");
				//SetLCD(1,"                ");
				//SetLCD(0,"\xC0");
				//SetLCD(1,"                ");
//
				//for (cnt=0; cnt<NROFMODULES; cnt++)
				//{
					//ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;
					//cntStartPuls[cnt] = 0;
					//ConsoleFunctionData[cnt] &= CUE_LED^0xFF;
					//ConsoleFunctionData[cnt] &= CUE_FUNCTION^0xFF;
				//}
			//}
		//}
	//}

	TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
	TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
	TextBuffer[2] = ':';
	TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
	TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
	TextBuffer[5] = ':';
	TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
	TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);

	SetLCD(0,"\x84");
	TextBuffer[8] = 0;
	SetLCD(1,TextBuffer);
	


					
	unsigned char cntByte, cntChannel;				
										
	// MASTERSETUPDATA
	SerialBufferInPtrTop = SerialBufferInPtrBottom = SerialBufferOutPtrTop = SerialBufferOutPtrBottom = 0;
														
	SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
	SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
										
	for (cntByte=0; cntByte<5; cntByte++)
	{
		SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
	}

	if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom) && SendSerialReady)
	{
		USARTE0_DATA = SerialBufferOut[SerialBufferOutPtrBottom++];
		SendSerialReady = 0;
	}


					
	Delay(100);
	//cntChannel=0;
		
	// MODULESETDATA
	for (cntChannel=0; cntChannel<16; cntChannel++)
	{
		//if ((ModuleType[cntChannel]==0x01) || (ModuleType[cntChannel]==0x02))
		//{
		SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
		SerialBufferOut[SerialBufferOutPtrTop++] = cntChannel;
						
		for (cntByte=0; cntByte<29; cntByte++)
		{
			ConsoleModuleSettings[cntChannel][cntByte]&=0x01;
			SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleModuleSettings[cntChannel][cntByte]&0x7F;
		}
		//}
		if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom) && SendSerialReady)
		{
			USARTE0_DATA = SerialBufferOut[SerialBufferOutPtrBottom++];
			SendSerialReady = 0;
		}
		while(SendSerialReady);
	}
									
	Delay(10);
		

	

	
// ************** WHILE(1) ************* ************** WHILE(1) ************* ************** WHILE(1) *************
// ************** WHILE(1) ************* ************** WHILE(1) ************* ************** WHILE(1) *************
// ************** WHILE(1) ************* ************** WHILE(1) ************* ************** WHILE(1) *************

	//SerialBufferOutPtrTop = 1;  // Dit schijnt belangrijk te zijn
	//SerialBufferOutPtrBottom = 2;  // buffers moeten opeen volgend zijn !!!  0-1  1-2  2-3 enz.
	
	SerialBufferInPtrTop = SerialBufferInPtrBottom = SerialBufferOutPtrTop = SerialBufferOutPtrBottom = 0;

	
	while(1)
	{
		unsigned char cnt;

		for (cnt=0; cnt<NROFMODULES; cnt++)
		{
			if (cntStartPuls[cnt])
			{
				cntStartPuls[cnt]--;
				if (cntStartPuls[cnt]==0)
				{
					ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;
					SendSerialData = 1;
				}
			}

			if (cntStartFunction[cnt])
			{
				cntStartFunction[cnt]--;
				if (cntStartFunction[cnt]==0)
				{
					ConsoleFunctionData[cnt] &= START_FUNCTION^0xFF;
					SendSerialData = 1;
				}
			}
			if (cntCueFunction[cnt])
			{
				cntCueFunction[cnt]--;
				if (cntCueFunction[cnt]==0)
				{
					ConsoleFunctionData[cnt] |= CUE_FUNCTION;
					CueControl();
					SendSerialData = 1;
				}
			}
			if (cntMuteFunction[cnt])
			{
				cntMuteFunction[cnt]--;
				if (cntMuteFunction[cnt]==0)
				{
					char TelcoNr = GetTelcoNrFromModuleNr(cnt);

					ConsoleFunctionData[cnt] &= MUTE_FUNCTION^0xFF;

					if ((TelcoNr>=0) && (TelcoNr<4))
					{
						SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_MUTE;
						SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
						SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
					}

					SendSerialData = 1;
				}
			}
		}
				
		if (SendSerialData)
		{
			SendSerialData = 0;
			SetConsoleData();
		}
		
		if ((TimeAndDate.Seconds != OldTimeAndDate.Seconds) ) //&& (OperatingMode==NORMALMODE))
		{		
			TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
			TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
			TextBuffer[2] = ':';
			TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
			TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
			TextBuffer[5] = ':';
			TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
			TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
			TextBuffer[8] = 0;
			
			SetLCD(0,"\x84");
			SetLCD(1,TextBuffer);
			OldTimeAndDate.Seconds = TimeAndDate.Seconds;		
		}
		
		ReadConsole();
		
		if (EventBufferPtrTop != EventBufferPtrBottom)
		{
			//USARTE0_DATA = EventBuffer[EventBufferPtrBottom];    // Aha hier weet je nu welke schakelaar er is ingedrukt
			DoEvent(EventBuffer[EventBufferPtrBottom++]);			
			//USARTE0_DATA =EventBufferPtrTop;
			//USARTE0_DATA =EventBufferPtrBottom;
			//EventBufferPtrBottom++;			
		}

		
		ConsoleMasterSettings[3]=1;  // is nodig zodat RS232 communicatie altijd mogelijk is

		if (ConsoleMasterSettings[3])
		{
			if (SerialBufferInPtrTop != SerialBufferInPtrBottom)
						
			DoSerial();

			if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom)  && SendSerialReady)
			//if(SendSerialReady)
			{
				
				USARTE0_DATA = SerialBufferOut[SerialBufferOutPtrBottom++];				
				SendSerialReady = 0;
				while ( !( USARTE0.STATUS & USART_DREIF_bm) );
			}
		}
		else
		{
			SerialBufferInPtrTop = SerialBufferInPtrBottom = SerialBufferOutPtrTop = SerialBufferOutPtrBottom = 0;
		}
	}
}







//ISR(TWIE_TWIM_vect)
//{
	////If TWI arbitration is lost send STOP
	//
	//if(TWIE_MASTER_STATUS & (1<<TWI_MASTER_ARBLOST_bp))
	//{
		//TWIE_MASTER_CTRLC=(1<<TWI_MASTER_CMD1_bp)|(1<<TWI_MASTER_CMD0_bp); //send stop condition
	//}
	//
	////If TWI bus error flag is set or NACK received then send STOP
	//
	//if((TWIE_MASTER_STATUS & (1<<TWI_MASTER_BUSERR_bp))||(TWIE_MASTER_STATUS & (1<<TWI_MASTER_RXACK_bp)))
	//{
		//TWIE_MASTER_CTRLC=(1<<TWI_MASTER_CMD1_bp)|(1<<TWI_MASTER_CMD0_bp); //send stop condition
	//}
	//
	//
	//
	//
	//
	////If TWI write interrupt flag is set
	//
	//if(TWIE_MASTER_STATUS & (1<<TWI_MASTER_WIF_bp))
	//{
		//if(!(TWIE_MASTER_STATUS & (1<<TWI_MASTER_RXACK_bp)))      //check whether acknowledge is received or not
		//{
			//TWIE_MASTER_DATA=twi_data_buffer[twi_data_count++];
			//
			//if(twi_no_of_bytes)
			//{
				//twi_no_of_bytes--;
			//}
			//else
			//{
				//TWIE_MASTER_CTRLC=(1<<TWI_MASTER_CMD1_bp)|(1<<TWI_MASTER_CMD0_bp); //send stop condition if all bytes are transferred
				//
				//twi_transfer_complete=1;
			//}
		//}
	//}
	//
	//
	//
	//
	////If RTC read interrupt flag is set
	//
	//if(TWIE_MASTER_STATUS & (1<<TWI_MASTER_RIF_bp))          //check whether read interrupt flag is set or not
	//{
		//twi_data_buffer[twi_data_count++]=TWIE_MASTER_DATA;
		//
		//if(twi_no_of_bytes==0)
		//{
			//TWIE_MASTER_CTRLC=(1<<TWI_MASTER_ACKACT_bp)|(1<<TWI_MASTER_CMD1_bp)|(1<<TWI_MASTER_CMD0_bp); //send NACK and stop condition if all bytes are transferred
			//
			//twi_transfer_complete=1;
		//}
		//else
		//{
			//twi_no_of_bytes--;
			//
			//TWIE_MASTER_CTRLC=(1<<TWI_MASTER_CMD1_bp)|(0<<TWI_MASTER_CMD0_bp); //send acknowledge
		//}
	//}
//}