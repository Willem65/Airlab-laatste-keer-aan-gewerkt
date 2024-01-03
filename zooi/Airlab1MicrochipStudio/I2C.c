//#include "reg552.h"
//#include "definitions.h"
#include "Delay.h"
#include "LCD.h"
//#include "I2C.h"
//#include "serial.h"
#include "airlabbrm-willem.h"




//unsigned char CardInserted;

unsigned char I2CBufferIn[256];



unsigned char I2CStatus;
unsigned char CurrentAddress;



unsigned char I2CBuffer[256];
unsigned char I2CLength;
unsigned char I2CDataLength;
unsigned char I2CRepeat;
unsigned char I2CRepeatAddress;
unsigned char I2CByteNr;
unsigned char I2CFromAddress;
unsigned char I2CItem;
unsigned char I2CModule;

unsigned char I2CMasterReciever;
unsigned char I2CBufferOutPtrTop;
unsigned char I2CBufferInPtrTop;
unsigned char I2CBufferInPtrBottom;
unsigned char I2CBufferOutPtrBottom;
unsigned char I2CSendReady;
unsigned char I2CError;

unsigned char I2CAddressBufferOut[256];
unsigned char I2CDataBufferOut[256];





//#ifndef __I2C_H__
//#define __I2C_H__
//	I2CBufferInPtrTop = 0;
//	I2CBufferInPtrBottom = 0;
//	I2CBufferOutPtrTop = 0;
//	I2CBufferOutPtrBottom = 0;
//	I2CSendReady=1;
//	I2CMasterReciever = 0;
//	I2CError = 0;
//	CardInserted = 0;
//#endif

//void SetAddress(unsigned char I2CAddress, unsigned int StartAddress);
//void SetupSIO1();

/***************************************************************
* Setup_I2C                                                    *
***************************************************************/
void SetupSIO1()
{
	
	//I2CBufferInPtrTop = 0;
	//I2CBufferInPtrBottom = 0;
	//I2CBufferOutPtrTop = 0;
	//I2CBufferOutPtrBottom = 0;
	//I2CSendReady=1;
	//I2CMasterReciever = 0;
	//I2CError = 0;
	//CardInserted = 0;
	//
	//
	//S1ADR = 0xFF;
//
	//P16 = 1;		// set I2Cports 
	//P17 = 1;
//
	//SI = 0;		// reset Serial Interrupt Flag   
	//AA = 0;		// disable masterinterrupt       
//
	//CR2 = 1;               // set bit-rate for I2C to 200kH 
	//CR1 = 1;
	//CR0 = 0;
//
	//ENS1 = 1;		// enable SIO1                   
//
	//STA = 0;		// no START, no STOP             
	//STO = 0;
//
	//ES1 = 1;
	//PS1 = 1;
}


void SetAddress(unsigned char I2CAddress, unsigned int StartAddress)
{
	

	
	//unsigned char Address = I2CAddress | ((StartAddress>>7)&0x0E);
	//
	//
		//
	////	I2CBufferInPtrTop = 0;
	////I2CBufferInPtrBottom = 0;
	//I2CBufferOutPtrTop = 0;
	//I2CBufferOutPtrBottom = 0;
	//I2CSendReady=1;
	////I2CMasterReciever = 0;
	//I2CError = 0;
	//CardInserted = 0;
//
	//I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
	//I2CDataBufferOut[I2CBufferOutPtrTop++] = StartAddress&0xFF;
	//EA = 0;
  //if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && I2CSendReady)
	//{
		//I2CSendReady=0;
		//EA = 1;
		//STA = 1;
	//}
	//EA = 1;
	//while (!I2CSendReady);
	//Delay(1);
//
	//if (I2CError)
	//{
		//SetLCD(0,"\xC0");
		//if (I2CAddress==I2CAddressSmartCard)
		//{
			//CardInserted = 0;
			////SerialBufferOut[SerialBufferOutPtrTop++] = CARD_INSERTED;
			////SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
			////SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
		//}
		//else
			//SetLCD(1,"No EEPROM comm. ");
//
		//I2CError = 0;
	//}
}


/***************************************************************
* READ_DATA_I2C                                                    *
***************************************************************/
void ReadData(unsigned char I2CAddress, unsigned int StartAddress, unsigned char *Buffer, unsigned char NrOfBytes)
{
	//unsigned char cnt;
	//unsigned char Address = I2CAddress | ((StartAddress>>7)&0x0E) |0x01;
	//
	//
		//
		//I2CBufferInPtrTop = 0;
	//I2CBufferInPtrBottom = 0;
	//I2CBufferOutPtrTop = 0;
	////I2CBufferOutPtrBottom = 0;
	//I2CSendReady=1;
	//I2CMasterReciever = 0;
	////I2CError = 0;
	////CardInserted = 0;
//
	//SetAddress(I2CAddress, StartAddress);
//
	//I2CMasterReciever = 1;
	//I2CDataLength = NrOfBytes;
	//I2CAddressBufferOut[I2CBufferOutPtrTop++] = Address;
	//EA = 0;
//
  //if (I2CSendReady)
	//{
		//I2CSendReady=0;
		//EA = 1;
		//STA = 1;  // ????
	//}
	//EA = 1;
//
	//while(!I2CSendReady);
	//I2CMasterReciever = 0;
//
	//cnt = 0;
	//while ((I2CBufferInPtrTop != I2CBufferInPtrBottom))
	//{
		//unsigned char Dummy = I2CBufferIn[I2CBufferInPtrBottom++];
		//if (cnt<NrOfBytes)
			//Buffer[cnt++] = Dummy;
	//}
}


/***************************************************************
* WRITE_DATA_I2C                                                    *
***************************************************************/
void WriteData(unsigned char I2CAddress, unsigned int StartAddress, unsigned char *Buffer, unsigned char NrOfBytes)
{
	//unsigned char cntByte = 0;
	//unsigned char Address = I2CAddress | ((StartAddress>>7)&0x0E);
//
	//while (cntByte<NrOfBytes)
	//{
		//// Watchdog
		//PCON |= 0x10;
		//T3 = 0;
//
		//I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
		//I2CDataBufferOut[I2CBufferOutPtrTop++] = StartAddress&0xFF;
	 	//I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
	  //I2CDataBufferOut[I2CBufferOutPtrTop++] = Buffer[cntByte];
		//StartAddress++;
		//cntByte++;
//
		//while ((cntByte<NrOfBytes) && (StartAddress&0x000F))
		//{
	  	//I2CAddressBufferOut[I2CBufferOutPtrTop] = Address;
		  //I2CDataBufferOut[I2CBufferOutPtrTop++] = Buffer[cntByte];
			//StartAddress++;
			//cntByte++;
		//}
//
		//EA = 0;
	  //if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && I2CSendReady)
		//{
			//I2CSendReady=0;
			//EA = 1;
			//STA = 1;
		//}
		//EA = 1;
//
		//while (!I2CSendReady);
//
		//Address = I2CAddress | ((StartAddress>>7)&0x0E);
//
		//Delay(1);
	//}
}



/************************************************************************
*       Interrupt on recieving I2C                                                                                                                      *
************************************************************************/
void Int_I2C(void) //interrupt 5 using 3 
{
	//unsigned char Status = S1STA;
	//unsigned char Data = S1DAT;
	//
	//
		//
		//I2CBufferInPtrTop = 0;
	////I2CBufferInPtrBottom = 0;
	//I2CBufferOutPtrTop = 0;
	//I2CBufferOutPtrBottom = 0;
	//I2CSendReady=1;
	//I2CMasterReciever = 0;
	//I2CError = 0;
	////CardInserted = 0;
//
	//I2CStatus = Status;
//
	//switch (Status)
	//{
		//case 0x08:
		//{	/*	A START Condition has been transmitted */
			//if (I2CBufferOutPtrTop != I2CBufferOutPtrBottom)
			//{
				//S1DAT = CurrentAddress = I2CAddressBufferOut[I2CBufferOutPtrBottom];
				//if (I2CMasterReciever)
					//I2CBufferOutPtrBottom++;
			//}
			//STO = 0;
//
			//if (I2CRepeatAddress != CurrentAddress)
			//{
				//I2CRepeat = 0;
				//I2CRepeatAddress = CurrentAddress;
			//}
		//}
		//break;
		//case 0x10:
		//{	/*	A repeated START Condition has been transmitted */
			//STO = 0;
			//S1DAT = CurrentAddress = I2CAddressBufferOut[I2CBufferOutPtrBottom];
			//if (I2CRepeatAddress != CurrentAddress)
			//{
				//I2CRepeat = 0;
				//I2CRepeatAddress = CurrentAddress;
			//}
		//}
		//break;
		//case 0x18:
		//{	/*	SLA+W has been transmitted; ACK has been recieved */
			//if (I2CBufferOutPtrTop != I2CBufferOutPtrBottom)
			//{
				//STA = 0;
				//STO = 0;
				//S1DAT = I2CDataBufferOut[I2CBufferOutPtrBottom];
			//}
			//else
			//{
				//STA = 0;
				//STO = 1;
				//AA = 1;
				//I2CSendReady = 1;
			//}
			//I2CRepeat = 0;
		//}
		//break;
		//case 0x20:
		//{	/*	SLA+W has been transmitted; NOT ACK has been recieved */
			//I2CRepeat++;
			//if (I2CRepeat > 10)
			//{
				//STA = 0;
				//STO = 1;
				//AA = 1;
 				//I2CSendReady = 1;
				//I2CError = 1;
				//while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
				//{
					//I2CBufferOutPtrBottom++;
				//}
			//}
			//else
			//{
				//STA = 1;
				//STO = 0;
				//AA = 1;
			//}
		//}
		//break;
		//case 0x28:
		//{	/*	Data byte in S1DAT has been transmitted; ACK has been recieved */
			//I2CBufferOutPtrBottom++;
			//if ((I2CBufferOutPtrTop != I2CBufferOutPtrBottom) && (I2CAddressBufferOut[I2CBufferOutPtrBottom] == CurrentAddress))
			//{
				//STA = 0;
				//STO = 0;
				//S1DAT = I2CDataBufferOut[I2CBufferOutPtrBottom];
			//}
			//else
			//{
				//STA = 0;
				//STO = 1;
				//AA = 1;
				//I2CSendReady = 1;
			//}
			//I2CRepeat = 0;
		//}
		//break;
		//case 0x30:
		//{	/*	Data byte in S1DAT has been transmitted; NOT ACK has been recieved */
			//STA = 1;
			//STO = 0;
			//I2CRepeat++;
			//if (I2CRepeat > 10)
			//{
				//while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
				//{
					//I2CBufferOutPtrBottom++;
				//}
			//}
		//}
		//break;
		//case 0x38:
		//{	/* Arbitration lost in SLA+R/W or databytes */
			//STA = 1;
			//STO = 0;
			//I2CRepeat++;
			//if (I2CRepeat > 10)
			//{
				//while ((I2CBufferOutPtrBottom!=I2CBufferOutPtrTop) && (I2CRepeatAddress == I2CAddressBufferOut[I2CBufferOutPtrBottom]))
				//{
					//I2CBufferOutPtrBottom++;
				//}
  		//}
		//}
		//break;
		//case 0x40:
		//{	/* SLA+R has been transmitted; ACK has been recieved */
			//STA = 0;
			//STO = 0;
//
			//I2CByteNr = 0;
			//if (I2CDataLength<2)
				//AA = 0;
			//else
				//AA = 1;
		//}
		//break;
		//case 0x48:
		//{	/* SLA+R has been transmitted; NOT ACK has been recieved */
			//STA = 0;
			//STO = 1;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0x50:
		//{	/* Data byte has been recieved; ACK has been returned */
			//I2CBuffer[I2CByteNr++] = Data;
//
			//STA = 0;
			//STO = 0;
			//if (I2CByteNr>(I2CDataLength-1))
				//AA = 0;
			//else
				//AA = 1;
		//}
		//break;
		//case 0x58:
		//{	/* Data byte has been recieved; NOT ACK has been returned */
			//unsigned char cnt = 0;
//
			//while (cnt < I2CByteNr)
			//{
				//I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
				//cnt++;
			//}
			//I2CBufferIn[I2CBufferInPtrTop++] = Data;
//
			//I2CByteNr = 0;
			//STA = 0;
			//STO = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0x60:
		//{	/* Own SLA+W has been revieved; ACK has been returned */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
			//I2CByteNr = 0;
		//}
		//break;
		//case 0x68:
		//{	/* Arbitration lost in SLA+R/W as master; Own SLA+W has been recieved, ACK returned */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
			//I2CByteNr = 0;
		//}
		//break;
		//case 0x70:
		//{	/* General call address (00H) has been recieved; ACK has been returned */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
			//I2CByteNr = 0;
		//}
		//break;
		//case 0x78:
		//{	/*	Arbitration lost in SLA+R/W as master; Generall call has been recieved; ACK has been returned */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
			//I2CByteNr = 0;
		//}
		//break;
		//case 0x80:
		//{	/* Previously addressed with own SLV adress; Data has been received; ACK has been returned */
			//switch (I2CByteNr)
			//{
				//case 0:
				//{
					//I2CFromAddress = Data;
					//I2CByteNr++;
				//}
				//break;
				//case 1:
				//{
					//I2CItem = Data;
					//switch (I2CItem)
					//{
						//default:
						//{
							//I2CLength = 3;
						//}
					//}
					//I2CByteNr++;
				//}
				//break;
				//case 2:
				//{
					//I2CModule = Data;
					//I2CByteNr++;
				//}
				//break;
				//default:
				//{
					//I2CBuffer[I2CByteNr++] = Data;
					//if (I2CByteNr > I2CLength)
					//{
						//unsigned char cnt = 3;
						//I2CBufferIn[I2CBufferInPtrTop++] = I2CFromAddress;
						//I2CBufferIn[I2CBufferInPtrTop++] = I2CItem;
						//I2CBufferIn[I2CBufferInPtrTop++] = I2CModule;
						//while (cnt < I2CByteNr)
						//{
							//I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
							//cnt++;
						//}
						//I2CByteNr = 0;
					//}
				//}
				//break;
			//}
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0x88:
		//{	/* Previously addressed with own SLA; Data has been received; NOT ACK has been returned */
			//STA = 0;
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
//
		//}
		//break;
		//case 0x90:
		//{	/*	Previously addressed with General Call;Data byte has been recieved; ACK has been recieved */
			//switch (I2CByteNr)
			//{
				//case 0:
				//{
					//I2CFromAddress = Data;
					//I2CByteNr++;
				//}
				//break;
				//case 1:
				//{
					//I2CItem = Data;
					//switch (I2CItem)
					//{
						//default:
						//{
							//I2CLength = 3;
						//}
					//}
					//I2CByteNr++;
				//}
				//break;
				//case 2:
				//{
					//I2CModule = Data;
					//I2CByteNr++;
				//}
				//break;
				//default:
				//{
					//I2CBuffer[I2CByteNr++] = Data;
					//if (I2CByteNr > I2CLength) 
					//{
						//unsigned char cnt = 3;
						//I2CBufferIn[I2CBufferInPtrTop++] = I2CFromAddress;
						//I2CBufferIn[I2CBufferInPtrTop++] = I2CItem;
						//I2CBufferIn[I2CBufferInPtrTop++] = I2CModule;
						//while (cnt < I2CByteNr)
						//{
							//I2CBufferIn[I2CBufferInPtrTop++] = I2CBuffer[cnt];
							//cnt++;
						//}
					//}
					//I2CByteNr = 0;
				//}
				//break;
			//}
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0x98:
		//{	/* Previously addressed with General Call;Data byte has been recieved; NOT ACK has been returned */
			//STA = 0;
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xA0:
		//{	/*	A STOP condition or repeated START condition has been recieved while still addressed as SLV/REC or SLV/TRX */
			//STA = 0;
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xA8:
		//{	/*	Own SLA+R has been recieved; ACK has been returned */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xB0:
		//{	/*	Arbitration lost in SLA+R/W as master; own SLA+R has been recieved; ACK has been returned */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xB8:
		//{	/*	Data byte in S1DAT has been transmitted; ACK has been recieved */
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xC0:
		//{	/*	Data byte in S1DAT has been transmitted; NOT ACK has been recieved */
			//STA = 0;
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xC8:
		//{	/* Last data byte in S1DAT has been transmitted (AA=0); ACK has been recieved */
			//STA = 0;
			//STO = 0;
			//AA = 1;
			//I2CSendReady = 1;
		//}
		//break;
		//case 0xF8:
		//{	/* No relevant state information available; S1 = 0 */
		//}
		//break;
		//case 0x00:
		//{	/* Bus error */
			//STA = 0;
			//STO = 1;
			//I2CSendReady = 1;
		//}
		//break;
	//}
	//SI = 0;
}
