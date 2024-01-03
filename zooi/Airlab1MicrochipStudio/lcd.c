
#ifndef F_CPU
#define F_CPU 32000000 // 24 MHz clock speed
#endif


#include <avr/io.h>
//#include <util/delay.h>
//#include "lcd.h"


//#define		LCD_D4  0x1
//#define		LCD_D5  0x2
//#define		LCD_D6  0x4
//#define		LCD_D7  0x8
//#define		LCD_RW  0x10
//#define		LCD_RS  0x20
//#define		LCD_E   0x40

#define		LCD_D4  0x1
#define		LCD_D5  0x2
#define		LCD_D6  0x4
#define		LCD_D7  0x10
#define		LCD_RW  0x8
#define		LCD_RS  0x40
#define		LCD_E   0x20
//#define		STRB1   0x80
//static  char Wait=40;  // Maximal 262 for _delay_us();

void SetupLCD()
{
	unsigned char TimeOut = 0x0f;
	unsigned char Wait=1;  // Maximal 262 for _delay_us();
	


	// Wait minimal 15 ms
	Delay(Wait);

	PORTA_OUTCLR = LCD_E;
	// Setup/Reset Display
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTSET = LCD_D4;
	PORTA_OUTSET = LCD_E;
	PORTA_OUTCLR = LCD_E;
	// Wait minimal 4.1 ms
	Delay(Wait);

	// Setup/Reset Display
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTSET = LCD_D4;
	PORTA_OUTSET = LCD_E * 1;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;
	// Wait minimal 100 us

	// Setup/Reset Display
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTSET = LCD_D4;
	PORTA_OUTSET = LCD_E ;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	// Busy cannot be checked
	Delay_ms(Wait);

	// Setup/Reset Display to 4 bit mode
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTCLR = LCD_D4;
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	// Set number of lines etc...
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		// Function set
	PORTA_OUTCLR = LCD_D6;		// Function set
	PORTA_OUTSET = LCD_D5;		// Function set
	PORTA_OUTCLR = LCD_D4;		// 4 bit
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTSET = LCD_D7;		// 2 lines
	PORTA_OUTCLR = LCD_D6;		// 5x7 dots chars
	PORTA_OUTCLR = LCD_D5;		// *
	PORTA_OUTCLR = LCD_D4;		// *
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		Delay_ms(Wait);
		PORTA_OUTSET = LCD_E;
	}
	PORTA_OUTCLR = LCD_E;

	// Display Off
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		// Function set
	PORTA_OUTCLR = LCD_D6;		// Function set
	PORTA_OUTCLR = LCD_D5;		// Function set
	PORTA_OUTCLR = LCD_D4;		
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTSET = LCD_D7;		// Display off
	PORTA_OUTSET = LCD_D6;		
	PORTA_OUTCLR = LCD_D5;	 	// Cursor on
	PORTA_OUTCLR = LCD_D4;	 	// Cursor Blink
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		Delay_ms(Wait);
		PORTA_OUTSET = LCD_E;
	}
	PORTA_OUTCLR = LCD_E;

	// Display Clear
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		// Function set
	PORTA_OUTCLR = LCD_D6;		// Function set
	PORTA_OUTCLR = LCD_D5;		// Function set
	PORTA_OUTCLR = LCD_D4;		
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		
	PORTA_OUTCLR = LCD_D6;		
	PORTA_OUTCLR = LCD_D5;	 
	PORTA_OUTSET = LCD_D4;		
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		Delay_ms(Wait);
		PORTA_OUTSET = LCD_E;
	}
	PORTA_OUTCLR = LCD_E;

	// Entry mode
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTCLR = LCD_D5;
	PORTA_OUTCLR = LCD_D4;		
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		
	PORTA_OUTSET = LCD_D6;
	PORTA_OUTSET = LCD_D5;		// Position increment after char
	PORTA_OUTCLR = LCD_D4;		// No shift
	PORTA_OUTSET = LCD_E;
	Delay_ms(Wait);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		Delay_ms(Wait);
		PORTA_OUTSET = LCD_E;
	}
	PORTA_OUTCLR = LCD_E;
}

/****************************************
* place data on LCD	RS = 0:	Instruction *
*					RS = 1:	Data        *
****************************************/
void SetLCD( int RS, char Buffer[] )
{
	unsigned char cntBuffer = 0;
	unsigned char Wait=1;  // Maximal 262 for Delay_ms();
	unsigned char outb;
	//uint8_t outc;

	
//#define		LCD_D4  0x1
//#define		LCD_D5  0x2
//#define		LCD_D6  0x4
//#define		LCD_D7  0x10
//#define		LCD_RW  0x8
//#define		LCD_RS  0x40
//#define		LCD_E   0x20	
	
	while (Buffer[cntBuffer] != 0)
	{
		
		//Buffer[cntBuffer]=0xff;
		

		PORTA_OUTCLR = LCD_RS;   //PORTA_OUT |= LCD_RS * RS;
		PORTA_OUTSET = LCD_RW;
			
		PORTA_OUTSET = LCD_E;
		PORTA_OUTCLR = LCD_E;			// 20
		PORTA_OUTSET = LCD_E;
			
//		PORTA_OUT = PORTA_OUT & 0xf0;
//		PORTA_OUT = PORTA_OUT | ((( char)Buffer[cntBuffer] & 0xF0) >> 4);					
		
	//	PORTA_OUT &= 0b11101000;		
		outb = ( char)Buffer[cntBuffer];
		
		if(outb & 0x10)
			PORTA_OUTSET = LCD_D4;
		else
			PORTA_OUTCLR = LCD_D4;
			
		if(outb & 0x20)
			PORTA_OUTSET = LCD_D5;
		else
			PORTA_OUTCLR = LCD_D5;		
			
		if(outb & 0x40)
			PORTA_OUTSET = LCD_D6;
		else
			PORTA_OUTCLR = LCD_D6;
					
		if(outb & 0x80)
			PORTA_OUTSET = LCD_D7;
		else
			PORTA_OUTCLR = LCD_D7;
			
		//outb &= 0b01110000;
		//outb >>= 4;						
		//outc = ( char)Buffer[cntBuffer];
		//outc &= 0b10000000;
		//outc >>= 3;
		//PORTA_OUT |= outb | outc;



		Delay_ms(Wait);		
		
		if(RS==0)
			PORTA_OUTCLR = LCD_RS;   //PORTA_OUT |= LCD_RS * RS;
		else if(RS==1)
			PORTA_OUTSET = LCD_RS;		
		
		PORTA_OUTCLR = LCD_RW;		
		
		PORTA_OUTSET = LCD_E;
		Delay_ms(Wait);
		PORTA_OUTCLR = LCD_E;
		
//		PORTA_OUT &= 0xf0;
//		PORTA_OUT |= ( ( char)Buffer[cntBuffer] & 0x0F );

		//PORTA_OUT &= 0b11101000;
		outb = ( char)Buffer[cntBuffer];

		if(outb & 0x1)
			PORTA_OUTSET = LCD_D4;
		else
			PORTA_OUTCLR = LCD_D4;
		
		if(outb & 0x2)
			PORTA_OUTSET = LCD_D5;
		else
			PORTA_OUTCLR = LCD_D5;
		
		if(outb & 0x4)
			PORTA_OUTSET = LCD_D6;
		else
			PORTA_OUTCLR = LCD_D6;
		
		if(outb & 0x8)
			PORTA_OUTSET = LCD_D7;
		else
			PORTA_OUTCLR = LCD_D7;


		//outb &= 0b00000111;						
		//outc = ( char)Buffer[cntBuffer];
		//outc &= 0b00001000;
		//outc <<= 1;
		//PORTA_OUT |= outb | outc;
		
		
		Delay_ms(Wait);
		
		PORTA_OUTSET = LCD_E;
		Delay_ms(Wait);
		PORTA_OUTCLR = LCD_E;		

		cntBuffer++;
	}
}



