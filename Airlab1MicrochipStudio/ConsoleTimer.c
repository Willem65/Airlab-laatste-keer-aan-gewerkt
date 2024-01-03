
//#include "reg552.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ConsoleTimer.h"


unsigned char GlobalTimerActive;
unsigned char Count;

struct TimeAndDateStruct OldTimeAndDate, TimeAndDate;


void SetupTimer0()
{	
	cli();

	// Clock source: ClkPer/4
	TCC0_CTRLA=TC_CLKSEL_DIV4_gc;
	//TCC0_CTRLA = 0x02;


	// Overflow interrupt: Low Level
	// Error interrupt: Disabled
	TCC0_INTCTRLA=TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_MED_gc;
	//TCC0_INTCTRLA = 0x02;
	
	
	// Set Period register
	TCC0_PER=0x5A5D;

	PMIC_CTRL |= 0x02;
	
	sei();	
}

ISR(TCC0_OVF_vect) 
{

	//GlobalTimerActive=1;     // willem
	 Count++;
	 if (Count>9) //Was before 10.
		 Count = 0;

	 if ((Count==0x00) && (GlobalTimerActive))
	 {
		 	//PORTA_OUTTGL |= (1<<0);    // ----------------------------------------------------------------
		 	//PORTA_OUTTGL |= 0x80;
			TimeAndDate.Frames+=10;
			
			if (TimeAndDate.Frames>90)
			{


				TimeAndDate.Frames = 0;
				TimeAndDate.Seconds++;
				if (TimeAndDate.Seconds>59)
				{
					
					TimeAndDate.Seconds = 0;
					TimeAndDate.Minutes++;
					if (TimeAndDate.Minutes>59)
					{
						TimeAndDate.Minutes = 0;
						TimeAndDate.Hours++;
						if (TimeAndDate.Hours>23)
						{
							TimeAndDate.Hours = 0;
						}
					}
				}
		 }
	}

}