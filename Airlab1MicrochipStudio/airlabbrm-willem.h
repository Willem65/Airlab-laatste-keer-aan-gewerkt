
#ifndef __AIRLABBRM_H__
#define __AIRLABBRM_H__


#define	I2CAddressSmartCard			0x50   // 0xA0	//X24C16
#define	I2CAddressEEProm			0x70  // 0xE0	//X24164

#define MODULE1_8					0
#define MODULE9_16					1
#define	NORMALMODE					0
#define SET_SWITCH_WAIT				50
#define NROFMODULES					16
#define ATTACK 						1
#define TELCO						2

#define	MODULEMODE			 		1
#define TRIPLE						1

#define	CUE_LED					    0x08
#define CUE_FUNCTION				0x04
#define ANNOUNCER_COMM_FUNCTION_LED	0x08
#define REMOTE_FUNCTION				0x01
#define MUTE_FUNCTION				0x10

#define		AUTOCOM					2

#define	ON_LED					    0x20
#define MUTE_FUNCTION				0x10
#define	CUE_LED					    0x08
#define CUE_FUNCTION				0x04
#define START_FUNCTION				0x02
#define REMOTE_FUNCTION				0x01
#define CS_CC						0x80 // PWM1 willem
#define TBL_MUTE					0x40   // PWM0 willem

extern unsigned char cntStartFunction[NROFMODULES];
extern unsigned char cntCueFunction[NROFMODULES];
extern unsigned char cntMuteFunction[NROFMODULES];

extern unsigned char CUEByRemoteMIC[NROFMODULES];

extern char GetTelcoNrFromModuleNr(char ModuleNr);

extern void CueControl(void);

// defines for ConsoleSettings
#define		TIMERLINEA	0
#define		TIMERLINEB	1
#define		TIMERMIC		2
#define		STARTLINEA	3
#define		STARTLINEB	4
#define		STARTMIC		5
#define		STARTFADER	6
#define		STARTON			7
#define		STARTCUE		8
#define		STARTPULS		9
#define		REMLINEA		10
#define		REMLINEB		11
#define		REMMIC			12
#define		DJMIC				13
#define		ANNMIC			14
#define		STUDIOMIC		15
#define		ONAIR1LINEA	16
#define		ONAIR1LINEB	17
#define		ONAIR1MIC		18
#define		ONAIR2LINEA	19
#define		ONAIR2LINEB	20
#define		ONAIR2MIC		21
#define		POWERON			22
#define		MICATLINEA	23
#define		MICATLINEB	24
#define		REMONMIC		25
#define		REMONLINEA 	26
#define		REMONLINEB	27
#define		SWITCHSTART	28

// Defines for Events
#define EXTERNREMOTEIN			0x00
#define FADERON							0x01
#define PFL									0x02
#define ON									0x03
#define MODULETYPEA					0x04
#define	MODULETYPEB					0x05
#define LINEB								0x06
#define MIC_RING						0x07
#define ESCAPE							0x80
#define ENTER								0x81
#define RIGHT 							0x82
#define DOWN								0x83
#define UP									0x84
#define LEFT								0x85
#define CUERESET						0x86
#define SW_CC								0x87
#define MUTE								0x88
#define EXT_MUTE						0x287
#define		AUTOCUERESET	0

#define DELAY								10

#define	MASTERMODE			 		2

#define	SYSTEMMODE			 		3

#define		CUETOCRM			1

#define NOT_POWER_ON_CC								0x40

//#define CS_CC	PWM1

extern unsigned char CardInserted;

extern unsigned char cntStartFunction[NROFMODULES];
extern unsigned char cntCueFunction[NROFMODULES];
extern unsigned char cntMuteFunction[NROFMODULES];

extern unsigned char StartActive[NROFMODULES];

extern void ShowFunction(unsigned char Function);

extern void StartControl(unsigned char Module, unsigned char On);

extern unsigned char TempEnableSetup;

//OnAir1/2 Control
extern void OnAirControl();

extern void TimerControl(unsigned char Module);
extern void StudioMicControl(void);

extern void CheckStartCondition(unsigned char Module);
extern char GetTelcoNrFromModuleNr(char ModuleNr);

extern void CueControl(void);

extern void CheckTBMic(void);
extern void CheckCUEByRemoteMIC(void);

extern unsigned char CUEByRemoteMIC[NROFMODULES];

extern void DJControl(void);

extern unsigned char ConsoleFunctionData[17];

extern char IDString[30]; //= "D&R Airlab v2.04 - 08/2006";

extern unsigned char OperatingMode;

extern unsigned char TextStringPtr;

extern unsigned char GlobalTimerActive;
extern unsigned char ConsoleModuleSettings[NROFMODULES][29];
extern unsigned char ConsoleMasterSettings[5];
extern unsigned char ModuleType[NROFMODULES];

extern unsigned char NrOfTelcos;
extern unsigned char NrOfTriples;

extern unsigned int  EventBuffer[256];
extern unsigned char EventBufferPtrTop;

extern unsigned char ModuleData[NROFMODULES][8];
extern unsigned char SendSerialData;

extern unsigned char MasterData[8];

extern unsigned char Wait;

extern unsigned char SerialBufferOut[256];
extern unsigned char SerialBufferIn[256];
extern unsigned char SerialBufferInPtrTop;
extern unsigned char SerialBufferInPtrBottom;
extern unsigned char SerialBufferOutPtrTop;
extern unsigned char SerialBufferOutPtrBottom;
extern unsigned char SerialByte;
extern unsigned char SerialLength;
extern unsigned char SerialCnt;
extern unsigned char SendSerialReady;
extern unsigned char DummySerialBuffer[256];
extern unsigned char CardInserted;
extern unsigned int  EventBuffer[256];

//extern unsigned char I2CAddressBufferOut[256];
//extern unsigned char I2CDataBufferOut[256];
//extern unsigned char I2CBufferIn[256];
//
extern unsigned char I2CError;
extern unsigned char I2CMasterReciever;
//extern unsigned char I2CStatus;
//
extern unsigned char I2CBufferInPtrTop;
extern unsigned char I2CBufferInPtrBottom;
extern unsigned char I2CBufferOutPtrTop;
extern unsigned char I2CBufferOutPtrBottom;
//extern unsigned char I2CBuffer[256];
//extern unsigned char I2CLength;
//extern unsigned char I2CDataLength;
//extern unsigned char I2CRepeat;
//extern unsigned char I2CRepeatAddress;
extern unsigned char I2CSendReady;
//extern unsigned char I2CByteNr;
//extern unsigned char I2CFromAddress;
//extern unsigned char I2CItem;
//extern unsigned char I2CModule;


char GetModuleNrFromTelcoNr(char TelcoNr);

extern unsigned char UndoConsoleModuleSettings[NROFMODULES][29];

extern unsigned char UndoConsoleMasterSettings[5];

void SetConsoleData(void);

extern unsigned char TextString[4][17];
extern unsigned char	ModuleTextString[29][17];
extern unsigned char	MasterTextString[4][17];
extern unsigned char	SystemTextString[6][17];

extern unsigned char TempEnableSetup;

extern void TimerControl(unsigned char Module);

//extern unsigned char ConsoleModuleSettings[NROFMODULES][29];
//extern unsigned char ConsoleMasterSettings[5];

extern unsigned char UndoConsoleModuleSettings[NROFMODULES][29];

extern unsigned char UndoConsoleMasterSettings[5];

extern unsigned char EEread[29];
extern unsigned char wr[29];

#endif