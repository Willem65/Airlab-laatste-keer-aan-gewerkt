//#include "reg552.h"
#include <avr/io.h>
//#include <stdio.h>
//#include <math.h>
#include "ReadModules.h"
#include "airlabbrm-willem.h"
#include "LCD.h"
#include "ReadModules.h"


////#define	P10  0x1 //read master
////#define	P11  0x2
////#define	P12  0x4
////#define	P13  0x8
////#define	P14  0x10

#define	P10  0x4 //read master // portB AS0
#define	P11  0x2  // portB AS1
#define	P12  0x1  // portC AS2
#define	P13  0x8  // portB AS3
#define	P14  0x4  // portC AS4

unsigned char Bits;

void ReadConsole()
{
	ReadMasterSwitches();
	//PORTB_OUTCLR = P10;
	//PORTB_OUTCLR = P11;
	//PORTC_OUTCLR = P12;
	//PORTB_OUTCLR = P13;
	//PORTC_OUTSET = P14;
	for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);
	ReadModuleSwitches(MODULE1_8);
	ReadModuleSwitches(MODULE9_16);
}

void ReadMasterSwitches()
{
	unsigned char cntByte;

	PORTB_OUTCLR = P10; //read master
	PORTB_OUTCLR = P11; 
	PORTC_OUTCLR = P12; 
	PORTB_OUTCLR = P13; 
	PORTC_OUTSET = P14; 

	for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);

	for (cntByte=0; cntByte<8; cntByte++)
	{
		// If bit set and switch isn't ON send event is necessery, else reset counter
		unsigned char Bits;                                                                   // = P5;
		//Bits = PORTC_IN;
		Bits = PORTR_IN;  // willem
		Bits &=0x3;
		if ((!(Bits&(0x01<<cntByte))) && (!(MasterData[cntByte]&0x80)))
		{
			if ((--MasterData[cntByte])&0x80)
			{
				EventBuffer[EventBufferPtrTop++] = 0x80 | cntByte;
			}
		}
		else if ((Bits&(0x01<<cntByte))&& (MasterData[cntByte]&0x80))
		{
			MasterData[cntByte]=ATTACK;
			EventBuffer[EventBufferPtrTop++] = 0x180 | cntByte;
		}
	}
}




void ReadModuleSwitches(unsigned char High)
{
	unsigned char cntByte;
	unsigned char cntModule;


	
	//P13 = High;  // AS3
	
	if (High==1)
		PORTB_OUTSET = P13;
	else
		PORTB_OUTCLR = P13;
	
	//P14 = 0; // Not read master  AS4
	PORTC_OUTCLR = P14;

	for (cntByte=0; cntByte<8; cntByte++)
	{
		for (cntModule=0; cntModule<8; cntModule++)
		{
			//unsigned char Bits;

			// Select switch to read
			//P10 = cntByte&0x01;  // AS0
			//P11 = cntByte&0x02;  // AS1
			//P12 = cntByte&0x04;  // AS2
			
			//PORTD_OUT |= cntByte&0x01;  // AS0
			//PORTD_OUT |= cntByte&0x02;  // AS1
			//PORTD_OUT |= cntByte&0x04;  // AS2
			
			
			if(cntByte&0x01)
			{	
				PORTB_OUTSET = P10;
			}
			else
			{
				PORTB_OUTCLR = P10;
			}
			
			
			if(cntByte&0x02)
			{
				PORTB_OUTSET = P11;
			}
			else
			{
				PORTB_OUTCLR = P11;
			}
			
			
			if(cntByte&0x04)
			{
				PORTC_OUTSET = P12;
			}
			else
			{
				PORTC_OUTCLR = P12;
			}
				
				
			//PORTD_OUTSET = P10;
			//PORTD_OUTSET = P11;
			//PORTD_OUTCLR = P12;
			//PORTD_OUTCLR = P13;
			//PORTD_OUTCLR = P14;
			
			//PORTD_OUT |= 0x1;
			//PORTD_OUT |= 0x2;
			//PORTD_OUT &= ~0x4;
			//PORTD_OUT &= ~0x8;
			//PORTD_OUT &= ~0x10;					

			for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);

			// If bit set and switch isn't ON send event is necessery, else reset counter
			//Bits = P5;
			//Bits &=0x3;  // willem
			
			if ( (PORTR_IN&0x2)&0x2 ) // SW1 PR1
				Bits |= 0x1;
			else
				Bits  &= ~0x1;
			
			if ( (PORTR_IN&0x1)&0x1 ) // SW2 PR0
				Bits |= 0x2;
			else
				Bits &= ~0x2;
			
				
			if ( (PORTD_IN&0x8)&0x8 )  // SW3 PD3 
				Bits |= 0x4;
			else
				Bits &= ~0x4;
			
			if ( (PORTD_IN&0x4)&0x4 )  // SW4 PD2
				Bits |= 0x8;
			else
				Bits  &= ~0x8;		
				
				
			if ( (PORTD_IN&0x80)&0x80 )  // SW5 PD7
			Bits |= 0x10;
			else
			Bits &= ~0x10;
			
			if ( (PORTD_IN&0x40)&0x40 )  // SW6 PD6
			Bits |= 0x20;
			else
			Bits  &= ~0x20;		
			
			
			if ( (PORTD_IN&0x20)&0x20 )  // SW7 PD5
			Bits |= 0x40;
			else
			Bits &= ~0x40;
			
			if ( (PORTD_IN&0x10)&0x10 )  // SW8 PD4
			Bits |= 0x80;
			else
			Bits  &= ~0x80;							
			

			
			
			for (Wait=0; Wait<SET_SWITCH_WAIT; Wait++);
			
			if ((!(Bits&(0x01<<cntModule))) && (!(ModuleData[cntModule+(8*High)][cntByte]&0x80)))
			{
				if ((--ModuleData[cntModule+(8*High)][cntByte])&0x80)
				{
					EventBuffer[EventBufferPtrTop++] = ((cntModule+(8*High))<<3) | cntByte;
				}
			}
			else if ((Bits&(0x01<<cntModule)) && (ModuleData[cntModule+(8*High)][cntByte]&0x80))
			{
				ModuleData[cntModule+(8*High)][cntByte]=ATTACK;
				EventBuffer[EventBufferPtrTop++] = 0x100 | ((cntModule+(8*High))<<3) | cntByte;
			}
		}
	}
}


void ShowConfig()
{
	char TextBuffer[17];
	unsigned char Ptr=0;

	
	TextBuffer[Ptr++] = 'S'; //"StMod:x Telco:xx"
	TextBuffer[Ptr++] = 't'; //"StMod:xx Telco:x"
	TextBuffer[Ptr++] = 'M';
	TextBuffer[Ptr++] = 'o';
	TextBuffer[Ptr++] = 'd';
	TextBuffer[Ptr++] = ':';
	
	if ((NrOfTriples/10)!=0)
	{
		TextBuffer[Ptr++] = 0x30+(NrOfTriples/10);
	}

	TextBuffer[Ptr++] = 0x30+(NrOfTriples%10);
	
	TextBuffer[Ptr++] = ' ';
	TextBuffer[Ptr++] = 'T';
	TextBuffer[Ptr++] = 'e';
	TextBuffer[Ptr++] = 'l';
	TextBuffer[Ptr++] = 'c';
	TextBuffer[Ptr++] = 'o';
	TextBuffer[Ptr++] = ':';
	
	if ((NrOfTelcos/10)!=0)
	{
		TextBuffer[Ptr++] = 0x30+(NrOfTelcos/10);
	}
	
	////	else
	////	{
	////		TextBuffer[Ptr++] = ' ';
	////	}
	
	TextBuffer[Ptr++] = 0x30+(NrOfTelcos%10);
	TextBuffer[Ptr++] = 0;
	
	SetLCD(0,"\xC0");
	SetLCD(1,TextBuffer);

}









