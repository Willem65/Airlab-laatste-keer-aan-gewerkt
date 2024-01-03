
#ifndef __CONSOLETIMER_H__
#define __CONSOLETIMER_H__


void SetupTimer0();


struct TimeAndDateStruct
{
	unsigned char Frames;
	unsigned char Seconds;
	unsigned char Minutes;
	unsigned char Hours;
};

extern struct TimeAndDateStruct OldTimeAndDate, TimeAndDate;



extern unsigned char GlobalTimerActive;

extern unsigned char Count;


#endif