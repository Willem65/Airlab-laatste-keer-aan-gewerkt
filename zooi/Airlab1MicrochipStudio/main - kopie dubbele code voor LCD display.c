/*
 * Airlab1MicrochipStudio.c
 *
 * Created: 16-5-2022 23:40:03
 * Author : willem
 */ 

#ifndef F_CPU
#define F_CPU 2400000 // 24 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>
//#include<inttypes.h>

#define rs 5
#define rw 4
#define en 6

#define PD0 0

//#define PORTA_OUTCLR  _SFR_MEM8(0x0606)
//#define PORTA_OUTSET  _SFR_MEM8(0x0605)
//#define PORTA_OUT  _SFR_MEM8(0x0604)

//#define PORTD_OUT  _SFR_MEM8(0x0664)
//#define PORTD_OUTSET  _SFR_MEM8(0x0665)
//#define PORTD_OUTCLR  _SFR_MEM8(0x0666)
//#define PORTD_OUTTGL  _SFR_MEM8(0x0667)



#define		LCD_D3  2048

#define		LCD_D4  0x1
#define		LCD_D5  0x2
#define		LCD_D6  0x4
#define		LCD_D7  0x8
#define		LCD_RW  0x10
#define		LCD_RS  0x20
#define		LCD_E   0x40
#define		STRB1   0x80

//char Buffer[100];

void lcd_init();
void dis_cmd(unsigned char);
void dis_data(unsigned char);
void Delay(int Tijd);
void SetupLCD(void);
void SetLCD(int RS, char Buffer[]);

unsigned char Wait; 


int	main (void)
{
	PORTA_DIR=0xff;		//Initialise port
	PORTD_DIR=0xff;		//Initialise port
	
	//while(1)
	//{
		////PORTD_OUTTGL=(1<<PD0);
		//PORTA_OUT = STRB1 * 1;
		////_delay_ms(100);
		//PORTA_OUT = STRB1 * 0;
		////_delay_ms(100);
	//}
	
	unsigned int t;
	
	unsigned char data0[11]="ENGINEERS";
	unsigned char data1[10]="GARAGE";
	
	


	lcd_init();

	for(t=0; t<9; t++)
	{
		dis_data(data0[t]);
	}

	dis_cmd(0xc0);

	for(t=0; t<6; t++)
	{
		dis_data(data1[t]);
	}


	
	//----------------------------------------------------------------------------------
	
	SetupLCD();
	
	//dis_data('A');
	//SetLCD("   D&R Airlab   ");
	
	SetLCD(1,"   D&R Airlab   ");
	SetLCD(0,"\xC0");
	SetLCD(1,"  Initializing  ");
	
	while(1)
	{
		;
	}

}

void lcd_init() // fuction for intialize
{
	_delay_ms(40);
	PORTA_OUTCLR=(1<<rs);
	PORTA_OUTCLR=(1<<rw);
	PORTA_OUTCLR=(1<<en);
	
	PORTA_OUT &= 0xF0;
	PORTA_OUT |= 0x03;
	_delay_ms(1);
	
	PORTA_OUTSET=(1<<en);
	_delay_ms(1);
	PORTA_OUTCLR=(1<<en);
	_delay_ms(1);
	
	PORTA_OUTSET=(1<<en);
	_delay_ms(1);
	PORTA_OUTCLR=(1<<en);
	_delay_ms(1);
	
	PORTA_OUTSET=(1<<en);
	_delay_ms(1);
	PORTA_OUTCLR=(1<<en);
	_delay_ms(1);
	
	PORTA_OUT &= 0xF0;
	PORTA_OUT |= 0x02;  // to initialize LCD in 4-bit mode.
	_delay_ms(1);	
	
	PORTA_OUTSET=(1<<en);
	_delay_ms(2);
	PORTA_OUTCLR=(1<<en);	
	_delay_ms(2);	
	
	dis_cmd(0x28); //to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	dis_cmd(0x01);	// clear display
	dis_cmd(0x0f);
	dis_cmd(0x06);
	dis_cmd(0x80);
}


void dis_cmd(unsigned char cmd_value)
{
	unsigned char tijd=10;	
	
	PORTA_OUTCLR=(1<<rs);
	_delay_us(1000);
	PORTA_OUTSET=(1<<rw);
	_delay_us(1000);
	
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<en);
	_delay_us(tijd);
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);			
	

	PORTA_OUT &= 0xF0;
	_delay_us(tijd);
	PORTA_OUT= ((cmd_value & 0xF0) >> 4);
	_delay_us(tijd);
	
	PORTA_OUTCLR=(1<<rs);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<rw);
	_delay_us(tijd);	


	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<en);
	_delay_us(tijd);		
	
	PORTA_OUT &= 0xF0;
	_delay_us(tijd);
	PORTA_OUT |= (cmd_value & 0x0F);
	_delay_us(tijd);
	
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<en);
	_delay_us(tijd);
}


void dis_data(unsigned char data_value)
{
	//unsigned char data_value1;
	unsigned char tijd=10;
	
	
	PORTA_OUTCLR=(1<<rs);
	_delay_us(1000);
	PORTA_OUTSET=(1<<rw);
	_delay_us(1000);
	
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<en);
	_delay_us(tijd);
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);	
	
	PORTA_OUT &= 0xF0;
	_delay_us(tijd);
	PORTA_OUT= ((data_value & 0xF0) >> 4);
	_delay_us(tijd);
	
	PORTA_OUTSET=(1<<rs);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<rw);
	_delay_us(tijd);
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<en);
	_delay_us(tijd);	
	
	PORTA_OUT &= 0xF0;
	_delay_us(tijd);
	PORTA_OUT |= (data_value & 0x0F);
	_delay_us(tijd);
	
	PORTA_OUTSET=(1<<en);
	_delay_us(tijd);
	PORTA_OUTCLR=(1<<en);
	_delay_us(tijd);
	
}


#define LCD_E_Wait		100


void SetupLCD()
{
	unsigned char TimeOut = 0x0f;

	// Wait minimal 15 ms
	Delay(15);

	PORTA_OUTCLR = LCD_E;
	// Setup/Reset Display
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTSET = LCD_D4;
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;
	// Wait minimal 4.1 ms
	Delay(1);

	// Setup/Reset Display
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTSET = LCD_D4;
	PORTA_OUTSET = LCD_E * 1;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;
	// Wait minimal 100 us
	Delay(1);

	// Setup/Reset Display
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTSET = LCD_D4;
	PORTA_OUTSET = LCD_E ;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	// Busy cannot be checked
	Delay(1);

	// Setup/Reset Display to 4 bit mode
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;
	PORTA_OUTCLR = LCD_D6;
	PORTA_OUTSET = LCD_D5;
	PORTA_OUTCLR = LCD_D4;
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	Delay(1);

	// Set number of lines etc...
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		// Function set
	PORTA_OUTCLR = LCD_D6;		// Function set
	PORTA_OUTSET = LCD_D5;		// Function set
	PORTA_OUTCLR = LCD_D4;		// 4 bit
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTSET = LCD_D7;		// 2 lines
	PORTA_OUTCLR = LCD_D6;		// 5x7 dots chars
	PORTA_OUTCLR = LCD_D5;		// *
	PORTA_OUTCLR = LCD_D4;		// *
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		for (Wait=0; Wait<LCD_E_Wait; Wait++);
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
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTSET = LCD_D7;		// Display off
	PORTA_OUTSET = LCD_D6;		
	PORTA_OUTCLR = LCD_D5;	 	// Cursor on
	PORTA_OUTSET = LCD_D4;	 	// Cursor Blink
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		for (Wait=0; Wait<LCD_E_Wait; Wait++);
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
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		
	PORTA_OUTCLR = LCD_D6;		
	PORTA_OUTCLR = LCD_D5;	 
	PORTA_OUTSET = LCD_D4;		
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		for (Wait=0; Wait<LCD_E_Wait; Wait++);
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
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTCLR = LCD_RW;
	PORTA_OUTCLR = LCD_D7;		
	PORTA_OUTSET = LCD_D6;
	PORTA_OUTSET = LCD_D5;		// Position increment after char
	PORTA_OUTCLR = LCD_D4;		// No shift
	PORTA_OUTSET = LCD_E;
	for (Wait=0; Wait<LCD_E_Wait; Wait++);
	PORTA_OUTCLR = LCD_E;

	TimeOut = 0x0F;
	PORTA_OUTSET = LCD_D7;
	PORTA_OUTCLR = LCD_RS;
	PORTA_OUTSET = LCD_RW;
	PORTA_OUTSET = LCD_E;
	while ((LCD_D7) && (TimeOut--))
	{
		PORTA_OUTCLR = LCD_E;
		for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTSET = LCD_E;
	}
	PORTA_OUTCLR = LCD_E;


	// Display on
/*	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 0;
	LCD_D6 = 0;
	LCD_D5 = 0;
	LCD_D4 = 0;		
	LCD_E = 1;
	LCD_E = 0;

	LCD_RS = 0;
	LCD_RW = 0;
	LCD_D7 = 1;		
	LCD_D6 = 1;
	LCD_D5 = 1;
	LCD_D4 = 1;
	LCD_E = 1;
	LCD_E = 0;

	TimeOut = 0x0F;
	LCD_D7 = 1;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_E = 1;
	LCD_E = 0;
	while ((LCD_D7) && (TimeOut--))
	{
		LCD_E = 1;
		LCD_E = 0;
	}*/
}

/****************************************
* place data on LCD	RS = 0:	Instruction *
*					RS = 1:	Data        *
****************************************/
void SetLCD( int RS, char Buffer[] )
{
	unsigned char cntBuffer = 0;
	
	while (Buffer[cntBuffer] != 0)
	{

		PORTA_OUTCLR = LCD_RS;   //PORTA_OUT |= LCD_RS * RS;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTSET = LCD_RW;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
			
		PORTA_OUTSET = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTCLR = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTSET = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);		
		
		PORTA_OUT &= 0xF0;
		PORTA_OUT= ((( char)Buffer[cntBuffer] & 0xF0) >> 4);
		for (Wait=0; Wait<LCD_E_Wait; Wait++);		
		
		if(RS==0)
			PORTA_OUTCLR = LCD_RS;   //PORTA_OUT |= LCD_RS * RS;
		else if(RS==1)
			PORTA_OUTSET = LCD_RS;		
		
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTCLR = LCD_RW;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);		
		
		PORTA_OUTSET = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTCLR = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		
		PORTA_OUT &= 0xF0;
		PORTA_OUT |= ( ( char)Buffer[cntBuffer] & 0x0F );
		for (Wait=0; Wait<LCD_E_Wait; Wait++);
		
		PORTA_OUTSET = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);
		PORTA_OUTCLR = LCD_E;
		//for (Wait=0; Wait<LCD_E_Wait; Wait++);		

		cntBuffer++;
	}
}


/****************************************************************
* delay(100) = ca. 1 Sec                                        *
****************************************************************/
void Delay(int tijd)
{
	int x, y;
	for (x = 1; x <= tijd; x++)
	{
		for (y = 0; y <= 1000; y++)
		{
			// Watchdog
			//PCON |= 0x10;
			//T3 = 0;
			;
		}
	}
}


