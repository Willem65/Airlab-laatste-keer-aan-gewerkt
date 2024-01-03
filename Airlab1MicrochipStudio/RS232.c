//#include "reg552.h"
//#include "Delay.h"
//#include "LCD.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "airlabbrm-willem.h"
#include "RS232.h"
//#include "I2C.h"
#include "ConsoleTimer.h"
#include "Delay.h"
#include "TWI.h"


#define USART_RX_BUFFER_SIZE 128
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )

char USART_ReceiveBuffer[USART_RX_BUFFER_SIZE];
static volatile unsigned char WritingPositionOnTheBuffer;
void USART_Transmit( unsigned char data);
uint16_t wacht;
	
char IDString[30]= "D&R Airlab v2.04 - 08/2006";

void DoSerial()
{

	
	unsigned char Event = SerialBufferIn[SerialBufferInPtrBottom++];
	unsigned char Channel = SerialBufferIn[SerialBufferInPtrBottom++];
	unsigned char Data = SerialBufferIn[SerialBufferInPtrBottom++];
	
	//unsigned char EEread[16], wr[16], Address;

	switch (Event)
	{
		case SERIAL_ON:
		{
			EventBuffer[EventBufferPtrTop++] = (unsigned int)ON | (Channel<<3);
		}
		break;
		case SERIAL_CUE:
		{
			EventBuffer[EventBufferPtrTop++] = (unsigned int)PFL | (Channel<<3);
		}
		break;
		case TIMER_START:
		{
			GlobalTimerActive = 1;
		}
		break;
		case TIMER_STOP:
		{
			GlobalTimerActive = 0;
		}
		break;
		case TIMER_RESET:
		{
			TimeAndDate.Hours = 0;
			TimeAndDate.Minutes = 0;
			TimeAndDate.Seconds = 0;
			TimeAndDate.Frames = 0;
		}
		break;
		case SERIAL_MODULESETUPDATA:
		{
			unsigned char cntByte;
			ConsoleModuleSettings[Channel][0] = Data;
			for (cntByte=1; cntByte<29; cntByte++)
			{
				ConsoleModuleSettings[Channel][cntByte] = SerialBufferIn[SerialBufferInPtrBottom++];
			}
		}
		break;
		case SERIAL_MASTERSETUPDATA:
		{
			unsigned char cntByte;
			ConsoleMasterSettings[0] = Data;
			for (cntByte=1; cntByte<5; cntByte++)
			{
				ConsoleMasterSettings[cntByte] = SerialBufferIn[SerialBufferInPtrBottom++];
			}
		}
		break;
		case GETSERIAL_IDSTRING:
		{
			unsigned char cntByte;
			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_IDSTRING;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			for (cntByte=0; cntByte<30; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = IDString[cntByte]&0x7F;
 		  }
		}
		break;
		case GETSERIAL_MODULESETUPDATA:
		{
			unsigned char cntByte;

			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
			SerialBufferOut[SerialBufferOutPtrTop++] = Channel;
			for (cntByte=0; cntByte<29; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleModuleSettings[Channel][cntByte]&0x7F;
 			}
		}
		break;
		case GETSERIAL_MASTERSETUPDATA:
		{
			unsigned char cntByte;

			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			for (cntByte=0; cntByte<5; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
			}
		}
		break;
		case GETSERIAL_CONFIG:
		{
			unsigned char cntByte;

			SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_CONFIG;
			SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			for (cntByte=0; cntByte<16; cntByte++)
			{
				SerialBufferOut[SerialBufferOutPtrTop++] = ModuleType[cntByte]&0x7F;
			}
		}
		break;
		case STORE_EEPROM:
		{			
			unsigned char cntModule;
			unsigned int Address = 0x0000;
			
			TWIM_Transmit(I2CAddressEEProm, Address, ModuleType, NROFMODULES);
			while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));

			Address += NROFMODULES;
			
			
			//--------------------------------------------------------------------
			for (int t=0; t<29; t++)
			{
				wr[t]=ConsoleModuleSettings[0][t];
			}
			//----------------------------------------------------------------------
			
				
			//----------------
			int vlag=1;
			int bufsize = sizeof(wr);
			
			for (int tm=0; tm<bufsize; tm++)
			{
				if ( (tm < 16) & (vlag==1) )
				{
					vlag=2;
					TWIM_Transmit(I2CAddressEEProm, 0x10, wr, 16);
					while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
				}
				
				if ( tm > 15 )
				{
					wr[tm-16]=wr[tm];
				}
			}
			

			TWIM_Transmit(I2CAddressEEProm, 32, wr, 13);
			while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
			//------------------------------------------------------------------------------------------
					
			Address += 29;
			
			TWIM_Transmit(I2CAddressEEProm, Address, ConsoleMasterSettings, 5);
			while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
			for (wacht=0; wacht<65000; wacht++);
	
		}
		break;
		
		
		//case STORE_SMARTCARD:
		//{
			//if (CardInserted)
			//{
				//unsigned char cntModule;
				//unsigned int Address = 0x0000;
				//WriteData(I2CAddressSmartCard, Address, ModuleType, NROFMODULES);
				//Address += NROFMODULES;
				//for (cntModule=0; cntModule<NROFMODULES; cntModule++)
				//{
					//WriteData(I2CAddressSmartCard, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
					//Address += 29;
				//}
				//WriteData(I2CAddressSmartCard, Address, ConsoleMasterSettings, 5);
			//}
		//}
		//break;
		//case GETCARD_INSERTED:
		//{		 
			//SerialBufferOut[SerialBufferOutPtrTop++] = CARD_INSERTED;
			//SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
////			SerialBufferOut[SerialBufferOutPtrTop++] = CardInserted;
		//}
		//break;
		
		case FROMDIRECTOR_TELCO_ON_CONNECT:
		{
			if (Data)
			{
				char ModuleNr;
			
				ModuleNr = GetModuleNrFromTelcoNr(Channel);

				if (ModuleNr != -1)
				{
					EventBuffer[EventBufferPtrTop++] = (ModuleNr<<3) | ON;
				}
			}
		}
		break;
		case FROMDIRECTOR_TELCO_MUTE:
		{
			char ModuleNr;
			
			ModuleNr = GetModuleNrFromTelcoNr(Channel);

			if (ModuleNr != -1)
			{
				if (Data)
				{
					EventBuffer[EventBufferPtrTop++] = (unsigned int)(ModuleNr<<3) | EXT_MUTE;
				}
				else
				{
					EventBuffer[EventBufferPtrTop++] = (unsigned int)(ModuleNr<<3) | EXT_MUTE | 0x100;
				}
			}
		}
		break;
		case FROMDIRECTOR_TELCO_TB:
		{
			if (Data)
			{
				char ModuleNr;
			
				ModuleNr = GetModuleNrFromTelcoNr(Channel);
	
				if (ModuleNr != -1)
				{
					EventBuffer[EventBufferPtrTop++] = (ModuleNr<<3) | PFL;
				}
			}
		}
		break;
		//case FROMDIRECTOR_TB:
		//{
			//if (Channel == TB_CONSOLE)
			//{
				//if (Data)
				//{
					//DirectorTBCUEBussActive = 1;
				//}
				//else
				//{
					//DirectorTBCUEBussActive = 0;
				//}
			//}
			//else if (Channel == TB_ALL)
			//{
				//if (Data)
				//{
					//DirectorTBCUEBussActive = 1;
				//}
				//else
				//{
					//DirectorTBCUEBussActive = 0;
				//}
			//}
			//CueControl();
		//}
		//break;
	}
}


void SetupSIO0()
{
			

	cli();

		PORTE_OUTSET = 0x8;
					
		// Communication mode: Asynchronous USART on PORTE
		// Data bits: 8
		// Stop bits: 1
		// Parity: Disabled
		USARTE0_CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

		// Receive complete interrupt: Medium Level
		// Transmit complete interrupt: Medium Level
		// Data register empty interrupt: Disabled
		USARTE0.CTRLA=(USARTE0.CTRLA & (~(USART_RXCINTLVL_gm | USART_TXCINTLVL_gm | USART_DREINTLVL_gm))) |
		USART_RXCINTLVL_MED_gc | USART_TXCINTLVL_MED_gc | USART_DREINTLVL_OFF_gc;
		
		// Bij een interne OSC van 32 MHz
		// Required Baud rate: 57600
		// Real Baud Rate: 57605.8 (x1 Mode), Error: 0.0 %
		//USARTE0.BAUDCTRLA=0x6E;
		//USARTE0.BAUDCTRLB=((0x0A << USART_BSCALE_gp) & USART_BSCALE_gm) | 0x08;
		
		// Bij een interne OSC van 16 MHz
		// Required Baud rate: 57600
		// Real Baud Rate: 57605,8 (x1 Mode), Error: 0,0 %
		//USARTE0.BAUDCTRLA=0x2E;
		//USARTE0.BAUDCTRLB=((0x09 << USART_BSCALE_gp) & USART_BSCALE_gm) | 0x08;
		
		// Bij een interne OSC van 10 MHz
		// Required Baud rate: 57600
		// Real Baud Rate: 57595,4 (x1 Mode), Error: 0,0 %
		//USARTE0.BAUDCTRLA=0xED;
		//USARTE0.BAUDCTRLB=((0x09 << USART_BSCALE_gp) & USART_BSCALE_gm) | 0x04;
		
		// Bij een interne OSC van 9 MHz
		// Required Baud rate: 57600
		// Real Baud Rate: 57600,0 (x1 Mode), Error: 0,0 %
		USARTE0.BAUDCTRLA=0x62;
		USARTE0.BAUDCTRLB=((0x09 << USART_BSCALE_gp) & USART_BSCALE_gm) | 0x04;		
			

		// Receiver: On
		// Transmitter: On
		// Double transmission speed mode: Off
		// Multi-processor communication mode: Off
		USARTE0.CTRLB=(USARTE0.CTRLB & (~(USART_RXEN_bm | USART_TXEN_bm | USART_CLK2X_bm | USART_MPCM_bm | USART_TXB8_bm))) |
		USART_RXEN_bm | USART_TXEN_bm;

				
		sei();
}


ISR(USARTE0_RXC_vect)
{
	SerialByte = USARTE0.DATA;

	if (SerialByte&0x80)
	{

		SerialCnt = 0;
		switch (SerialByte)
		{
			case SERIAL_MODULESETUPDATA:
			{

				SerialLength = 31;
				
			}
			break;
			case SERIAL_MASTERSETUPDATA:
			{

				SerialLength = 7;
			}
			break;
			default:
			{

				SerialLength = 3;
			}
			break;
		}	
	}
		
	DummySerialBuffer[SerialCnt++] = SerialByte;
	if (SerialCnt >= SerialLength)
	{
		unsigned char cnt;
		for (cnt=0; cnt<SerialLength; cnt++)
		{
							//PORTB_OUTSET = 1;  // TEST LED
							//Delay(1000);
							//PORTB_OUTCLR = 1;
							//Delay(1000);
			SerialBufferIn[SerialBufferInPtrTop++] = DummySerialBuffer[cnt];
		}
	}
}

ISR(USARTE0_TXC_vect)
{
	//SerialBufferOutPtrBottom=0x2B;
	//USARTE0_DATA = (!(SerialBufferOut[SerialBufferOutPtrBottom]&0x80));
	//USARTE0_DATA = SerialBufferOutPtrTop;
	//while ( !( USARTE0.STATUS & USART_DREIF_bm) );
	//USARTE0_DATA = SerialBufferOutPtrBottom;
	//while ( !( USARTE0.STATUS & USART_DREIF_bm) );
	if ((SerialBufferOutPtrBottom != SerialBufferOutPtrTop) &&  (!(SerialBufferOut[SerialBufferOutPtrBottom]&0x80)))
	//if (SerialBufferOutPtrBottom != SerialBufferOutPtrTop)
	//if (!(SerialBufferOut[SerialBufferOutPtrBottom]&0x80))
	{
		USARTE0_DATA = SerialBufferOut[SerialBufferOutPtrBottom++];
	}
	else
	{
		SendSerialReady = 1;
	}
}
