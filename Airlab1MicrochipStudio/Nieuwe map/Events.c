//#include "reg552.h"
#include <avr/interrupt.h>
#include "airlabbrm-willem.h"
#include "LCD.h"
#include "Delay.h"
#include "RS232.h"
//#include "I2C.h"
#include "ReadModules.h"
#include "ConsoleTimer.h"

//unsigned char  CS_CC;


void DoEvent(unsigned int Event)
{
	unsigned char EventType = (Event&0x87);
	unsigned char Module = (Event>>3)&0x0F;
	unsigned char EventOn = !(Event&0x100);
	
	if ((Event&0x287) == EXT_MUTE)
	{
		EventType = MUTE;
	}
//ConsoleFunctionData[0] = 0x20;
	switch (EventType)
	{

		case ON:
		{
			//ConsoleFunctionData[0] = 0x20;
			switch (ModuleType[Module])
			{
				case TRIPLE:
				{	// Triple
					if (ConsoleModuleSettings[Module][SWITCHSTART])
					{
						ConsoleFunctionData[Module] |= ON_LED;
						ConsoleFunctionData[Module] |= REMOTE_FUNCTION;
						ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;

						if (EventOn)
						{
							StartControl(Module,1);
							StartActive[Module] = 1;
						}
						else
						{
							if (!ConsoleModuleSettings[Module][STARTPULS])
							{
								StartControl(Module,0);
								StartActive[Module] = 0;
							}
						}
						SendSerialData = 1;
					}
					else
					{
						if (EventOn)
						{
							if (ConsoleFunctionData[Module]&ON_LED)
							{
								ConsoleFunctionData[Module] &= ON_LED^0xFF;
								ConsoleFunctionData[Module] &= REMOTE_FUNCTION^0xFF;
	
								CheckStartCondition(Module);

								ConsoleFunctionData[Module] |= MUTE_FUNCTION;
				
								if ((ModuleData[Module][FADERON]&0x80))
								{	 // !JB
									CheckStartCondition(Module);

									//DJ-Control
									DJControl();
			
									//Studio-Mic-Control
									StudioMicControl();			

									////Timer-Control
									//TimerControl(Module);
				
									//OnAir Control
									OnAirControl();
								}
								SendSerialData = 1;
							}
							else
							{
								ConsoleFunctionData[Module] |= ON_LED;

								CheckStartCondition(Module);

							}
						}
					}
				}
				break;
			}
		}
		break;
		
	}
}
