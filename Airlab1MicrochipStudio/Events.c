//#include "reg552.h"
#include <avr/interrupt.h>
#include "airlabbrm-willem.h"
#include "LCD.h"
#include "Delay.h"
#include "RS232.h"
#include "I2C.h"
#include "ReadModules.h"
#include "ConsoleTimer.h"
#include "TWI.h"


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

	switch (EventType)
	{
		case EXTERNREMOTEIN:
		{
			if (OperatingMode != MODULEMODE)
			{
				switch (ModuleType[Module])
				{
					case TRIPLE:
					{ // TRIPLE
						if (EventOn)
						{
							if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][REMMIC]))
							{
								ConsoleFunctionData[Module] |= CUE_LED;
								ConsoleFunctionData[Module] |= CUE_FUNCTION;
	
								//DJ-Control
								DJControl();
	
								ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
								if (ConsoleFunctionData[Module]&REMOTE_FUNCTION)
									ConsoleFunctionData[Module]|=MUTE_FUNCTION;
								else if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][ANNMIC]) && (ConsoleFunctionData[Module]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
								{
									ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
								}

								CUEByRemoteMIC[Module] = 1;
								CheckCUEByRemoteMIC();
								CheckTBMic();
								SendSerialData = 1;
							}
							else if ((!(ModuleData[Module][LINEB]&0x80)) && (ConsoleModuleSettings[Module][REMLINEA]))
							{
								ConsoleFunctionData[Module] |= CUE_LED;
								ConsoleFunctionData[Module] |= CUE_FUNCTION;

								//DJ-Control
								DJControl();

								if (ConsoleModuleSettings[Module][MICATLINEA])
								{
									ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
									if (ConsoleFunctionData[Module]&REMOTE_FUNCTION)
										ConsoleFunctionData[Module]|=MUTE_FUNCTION;
									if ((!(ModuleData[Module][LINEB]&0x80)) && (ConsoleModuleSettings[Module][ANNMIC]) && (ConsoleFunctionData[Module]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
									{
											ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
									}
									CheckTBMic();
									SendSerialData = 1;
								}
							}
							else if ((ModuleData[Module][LINEB]&0x80) && (ConsoleModuleSettings[Module][REMLINEB]))
							{
								ConsoleFunctionData[Module] |= CUE_LED;
								ConsoleFunctionData[Module] |= CUE_FUNCTION;

								//DJ-Control
								DJControl();

								if (ConsoleModuleSettings[Module][MICATLINEB])
								{
									ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
									if (ConsoleFunctionData[Module]&REMOTE_FUNCTION)
										ConsoleFunctionData[Module]|=MUTE_FUNCTION;
									if ((ModuleData[Module][LINEB]&0x80) && (ConsoleModuleSettings[Module][ANNMIC]) && (ConsoleFunctionData[Module]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
									{
											ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
									}
  								CheckTBMic();
									SendSerialData = 1;
								}
							}

							// REMOTE ON
							if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][REMONMIC]))
							{
								EventBuffer[EventBufferPtrTop++] = (Module<<3) | ON;
							}
							else if ((!(ModuleData[Module][LINEB]&0x80)) && (ConsoleModuleSettings[Module][REMONLINEA]) && (!(ModuleData[Module][MIC_RING]&0x80)))
							{
								EventBuffer[EventBufferPtrTop++] = (Module<<3) | ON;
							}
							else if ((ModuleData[Module][LINEB]&0x80) && (ConsoleModuleSettings[Module][REMONLINEB]) && (!(ModuleData[Module][MIC_RING]&0x80)))
							{
								EventBuffer[EventBufferPtrTop++] = (Module<<3) | ON;
							}

							//Cue-Control
							CueControl();
						}
						else
						{
							if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][REMMIC]))
							{
								ConsoleFunctionData[Module] &= CUE_LED^0xFF;
								ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
								if ((ConsoleFunctionData[Module]&ON_LED) && ((ModuleData[Module][FADERON]&0x80)))
									ConsoleFunctionData[Module]&=MUTE_FUNCTION^0xFF;	// !JB

								CUEByRemoteMIC[Module] = 0;
								CheckCUEByRemoteMIC();

								SendSerialData = 1;

							}
							else if ((!(ModuleData[Module][LINEB]&0x80)) && (ConsoleModuleSettings[Module][REMLINEA]))
							{
								ConsoleFunctionData[Module] &= CUE_LED^0xFF;
								ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
								if ((ConsoleFunctionData[Module]&ON_LED) && ((ModuleData[Module][FADERON]&0x80)))
									ConsoleFunctionData[Module]&=MUTE_FUNCTION^0xFF;	// !JB

								SendSerialData = 1;
							}
							else if ((ModuleData[Module][LINEB]&0x80) && (ConsoleModuleSettings[Module][REMLINEB]))
							{
								ConsoleFunctionData[Module] &= CUE_LED^0xFF;
								ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
								if ((ConsoleFunctionData[Module]&ON_LED) && ((ModuleData[Module][FADERON]&0x80)))
									ConsoleFunctionData[Module]&=MUTE_FUNCTION^0xFF;	// !JB

								SendSerialData = 1;
							}

							//DJ-Control
							DJControl();

							//Cue-Control
							CueControl();
						}
					}
					break;
					case TELCO:
					{ // TELCO
						if (EventOn)
						{
							if ((!(ModuleData[Module][LINEB]&0x80)) && (ConsoleModuleSettings[Module][REMLINEA]))
							{
								EventBuffer[EventBufferPtrTop++] = (Module<<3) | PFL;
							}
							else if ((ModuleData[Module][LINEB]&0x80) && (ConsoleModuleSettings[Module][REMLINEB]))
							{
								EventBuffer[EventBufferPtrTop++] = (Module<<3) | PFL;
							}
							else if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][REMMIC]))
							{
								ConsoleFunctionData[Module] |= CUE_LED;
								ConsoleFunctionData[Module] |= CUE_FUNCTION;
								if (!(ConsoleFunctionData[Module]&REMOTE_FUNCTION))
									ConsoleFunctionData[Module]|=MUTE_FUNCTION;
								SendSerialData = 1;
							}
							//Cue-Control
							CueControl();
						}
						else
						{
							if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][REMMIC]) && (ConsoleFunctionData[Module]&REMOTE_FUNCTION))
							{
								ConsoleFunctionData[Module] &= CUE_LED^0xFF;
								ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
								ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;
								SendSerialData = 1;
							}
							//Cue-Control
							CueControl();
						}
					}
					break;
				}
			}
		}
		break;
		case FADERON:
		{
			switch (ModuleType[Module])
			{
				case TRIPLE:
				{ //Triple
					if (EventOn)
					{		// Was eerst het OfEvent JB
							// Inverse reaction bij op P-print by Jan Betten!! 
							// dus onevent is fader uit bij P-print!!!!

						if (ConsoleFunctionData[Module]&ON_LED)
						{
							ConsoleFunctionData[Module] |= REMOTE_FUNCTION;

							//OnAir1/2 Control
							OnAirControl();

							TimerControl(Module);

							ConsoleFunctionData[Module] &= CUE_LED^0xFF;
							ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;

							//Cue-Control
							if (ConsoleMasterSettings[AUTOCUERESET])
							{
								if (((ModuleData[Module][FADERON]&0x80)) && ((ConsoleFunctionData[Module]&ON_LED)))
								{	 // !JB
									unsigned char cntModule;
									for (cntModule=0; cntModule<NROFMODULES; cntModule++)
									{
										ConsoleFunctionData[cntModule] &= CUE_LED^0xFF;
										ConsoleFunctionData[cntModule] &= CUE_FUNCTION^0xFF;
									}
								}
							}
							CueControl();

							//DJ-Control
							DJControl();
							//StudioMic Control
							StudioMicControl();

							CheckStartCondition(Module);

							ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;
							SendSerialData = 1;
						}
						else
						{
							if ((ConsoleModuleSettings[Module][STARTFADER]) && (!ConsoleModuleSettings[Module][STARTON]))
							{
								CheckStartCondition(Module);
							}
							TimerControl(Module);
						}
					}
					else
					{
						ConsoleFunctionData[Module] |= MUTE_FUNCTION;
						if (ConsoleFunctionData[Module]&ON_LED)
						{
							if (ConsoleModuleSettings[Module][STARTFADER]) 
							{
									CheckStartCondition(Module);
							}
							//OnAir1/2 Control
							OnAirControl();

							TimerControl(Module);

							//DJ-Control
							DJControl();

							//StudioMic Control
							StudioMicControl();
						}
						else
						{
							if ((ConsoleModuleSettings[Module][STARTFADER]) && (!ConsoleModuleSettings[Module][STARTON]))
							{
								CheckStartCondition(Module);
							}
						}
						ConsoleFunctionData[Module] &= REMOTE_FUNCTION^0xFF;
						SendSerialData = 1;
					}
// was eerst het OnEvent JB
				}
				break;
				case TELCO:
				{ // Telco
					if (EventOn)
					{// JB was else
						if (ConsoleFunctionData[Module]&ON_LED)
						{
							char TelcoNr = GetTelcoNrFromModuleNr(Module);
							if ((TelcoNr>=0) && (TelcoNr<4))
							{
								SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
								SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
								SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
							}

							//OnAir1/2 Control
							OnAirControl();
							TimerControl(Module);
							
							ConsoleFunctionData[Module] &= CUE_LED^0xFF;
							ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;

							//Cue-Control
							if (ConsoleMasterSettings[AUTOCUERESET])
							{
								if (((ModuleData[Module][FADERON]&0x80)) && ((ConsoleFunctionData[Module]&ON_LED)))
								{	 // !JB
									unsigned char cntModule;
									for (cntModule=0; cntModule<NROFMODULES; cntModule++)
									{
										char TelcoNr = GetTelcoNrFromModuleNr(cntModule);
										if ((TelcoNr>=0) && (TelcoNr<4))
										{
											SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
											SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
											SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
										}

										ConsoleFunctionData[cntModule] &= CUE_LED^0xFF;
										ConsoleFunctionData[cntModule] &= CUE_FUNCTION^0xFF;

									}
								}
							}
							CueControl();

							ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;
							SendSerialData = 1;
						}
					}
					else
					{			// JB was if
 						//OnAir1/2 Control
						OnAirControl();
						TimerControl(Module);
						ConsoleFunctionData[Module] |= MUTE_FUNCTION;
						SendSerialData = 1;
					}
 				}
				break;
			}
		}
		break;
		case PFL:
		{
			switch (OperatingMode)
			{
				case MODULEMODE:
				{
					if ((EventOn) && (ConsoleMasterSettings[4]))
					{
						unsigned char cntByte;

						char Buffer[] ="Module xx: xxx  ";
						
						if (ConsoleModuleSettings[Module][TextStringPtr])
							ConsoleModuleSettings[Module][TextStringPtr] = 0;
						else
							ConsoleModuleSettings[Module][TextStringPtr] = 1;

						if (ModuleType[Module]==TELCO)
						{
							switch (TextStringPtr)
							{
								case TIMERMIC:
								case STARTMIC:
								case STARTFADER:
								case STARTPULS:
								case DJMIC:
								case ANNMIC:
								case STUDIOMIC:
								case ONAIR1MIC:
								case ONAIR2MIC:
								case REMMIC:
								case REMONMIC:
								case REMONLINEA:
								case REMONLINEB:
								case MICATLINEA:
								case MICATLINEB:
								{
									ConsoleModuleSettings[Module][TextStringPtr]=0x00;
								}
								break;
								case STARTON:
								case STARTLINEA:
								case STARTLINEB:
								case STARTCUE:
								{
									ConsoleModuleSettings[Module][TextStringPtr]=0x01;
								}
								break;
							}
						}

						switch (TextStringPtr)
						{
							case STARTLINEA:
							case STARTLINEB:
							case STARTMIC:
							case STARTFADER:
							case STARTON:
							case STARTCUE:
							case STARTPULS:
							{
								unsigned char cntModule;
								for (cntModule=0; cntModule<NROFMODULES; cntModule++)
								{
									StartControl(cntModule, 0);
									TimerControl(cntModule);
								}
							}
							break;
							case DJMIC:
							{
								DJControl();
							}
							break;
							case MICATLINEA:
							case MICATLINEB:
							{
								DJControl();
								StudioMicControl();
							}
							break;
							case ANNMIC:
							{
								if (ModuleType[Module]==TRIPLE)
								{
									unsigned char cntModule;
									for (cntModule=0; cntModule<NROFMODULES; cntModule++)
									{
										if (cntModule!=Module)
											ConsoleModuleSettings[cntModule][TextStringPtr] = 0x00;
									}
								}
								StudioMicControl();
							}
							break;
							case STUDIOMIC:
							{
								StudioMicControl();
							}
							break;
							case ONAIR1LINEA:
							case ONAIR1LINEB:
							case ONAIR1MIC:
							case ONAIR2LINEA:
							case ONAIR2LINEB:
							case ONAIR2MIC:
							{
								//OnAir1/2 Control
								OnAirControl();
							}
							break;
						}

						Buffer[7] = 0x30+((Module+1)/10);
						Buffer[8] = 0x30+((Module+1)%10);
						if (TextStringPtr==STARTPULS)
						{
							if (ConsoleModuleSettings[Module][TextStringPtr])
							{
								Buffer[11] = 'P';
								Buffer[12] = 'u';
								Buffer[13] = 'l';
								Buffer[14] = 's';
							}
							else
							{
								Buffer[11] = 'C';
								Buffer[12] = 'o';
								Buffer[13] = 'n';
								Buffer[14] = 't';
							}
						}
						else
						{
							if (ConsoleModuleSettings[Module][TextStringPtr])
							{
								Buffer[11] = 'Y';
								Buffer[12] = 'e';
								Buffer[13] = 's';
							}
							else
							{
								Buffer[11] = ' ';
								Buffer[12] = 'N';
								Buffer[13] = 'o';
							}
						}
						SetLCD(0,"\xC0");
						SetLCD(1,Buffer);
						ShowFunction(TextStringPtr);

			
						SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
						SerialBufferOut[SerialBufferOutPtrTop++] = Module;
						for (cntByte=0; cntByte<29; cntByte++)
						{
							SerialBufferOut[SerialBufferOutPtrTop++] =
							ConsoleModuleSettings[Module][cntByte]&0x7F;
					  }
					}
				}
				break;
				default:
				{
					switch (ModuleType[Module])
					{
						case TRIPLE:
						{	// Triple
							if (EventOn)
							{
								if (ConsoleFunctionData[Module]&CUE_FUNCTION)
								{
									unsigned char cntModule;

									ConsoleFunctionData[Module] &= CUE_LED^0xFF;
									ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;

									CheckStartCondition(Module);
									//DJ-Control
									DJControl();

									//Check auto communication
									ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
									for (cntModule=0; cntModule<NROFMODULES; cntModule++)
									{
										if (ConsoleFunctionData[cntModule]&CUE_LED)
										{
											if ((ModuleData[cntModule][MIC_RING]&0x80) && (ConsoleModuleSettings[cntModule][ANNMIC]) && (ConsoleFunctionData[cntModule]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
												ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
										}
									}
  								CheckTBMic();
									SendSerialData = 1;
								}
								else
								{
									unsigned char cntModule;

									ConsoleFunctionData[Module] |= CUE_LED;
									ConsoleFunctionData[Module] |= CUE_FUNCTION;
									CheckStartCondition(Module);									

									//DJ-Control
									DJControl();

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
								// Cue-Control
								CueControl();
							}
						}
						break;
						case TELCO:
						{	// Telco
							if ((EventOn) && (!(ModuleData[Module][FADERON]&0x80)))
							{	 // JB
								char TelcoNr = GetTelcoNrFromModuleNr(Module);

								if (ConsoleFunctionData[Module]&CUE_FUNCTION)
								{
									ConsoleFunctionData[Module] &= CUE_LED^0xFF;
									ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;

									if ((TelcoNr>=0) && (TelcoNr<4))
									{
										SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
										SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
										SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
									}
									CheckTBMic();
									SendSerialData = 1;
								}
								else
								{
									ConsoleFunctionData[Module] |= START_FUNCTION;
									ConsoleFunctionData[Module] |= ON_LED;
									ConsoleFunctionData[Module] |= CUE_LED;
									ConsoleFunctionData[Module] |= REMOTE_FUNCTION;
//									ConsoleFunctionData[Module] |= CUE_FUNCTION;
									cntCueFunction[Module] = DELAY;

									SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_START;
									SerialBufferOut[SerialBufferOutPtrTop++] = Module;
									SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;

									if ((TelcoNr>=0) && (TelcoNr<4))
									{
										SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CONNECT;
										SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
										SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
									}

									if ((TelcoNr>=0) && (TelcoNr<4))
									{
										SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
										SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
										SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
									}
									CheckTBMic();
									SendSerialData = 1;
								}
								// Cue-Control
								CueControl();
							}
						}
						break;
					}
				}
				break;
			}
		}
		break;
		case ON:
		{
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

									//Timer-Control
									TimerControl(Module);
				
									//OnAir Control
									OnAirControl();
								}
								SendSerialData = 1;
							}
							else
							{
								ConsoleFunctionData[Module] |= ON_LED;

								CheckStartCondition(Module);

								if ((ModuleData[Module][FADERON]&0x80))
								{	 // !JB
									ConsoleFunctionData[Module] |= REMOTE_FUNCTION;

									CheckStartCondition(Module);

									ConsoleFunctionData[Module] &= CUE_LED^0xFF;
									ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
								
									//Cue-Control
									if (ConsoleMasterSettings[AUTOCUERESET])
									{
										if (((ModuleData[Module][FADERON]&0x80)) && ((ConsoleFunctionData[Module]&ON_LED)))
										{	 // !JB
											unsigned char cntModule;
											for (cntModule=0; cntModule<NROFMODULES; cntModule++)
											{
												ConsoleFunctionData[cntModule] &= CUE_LED^0xFF;
												ConsoleFunctionData[cntModule] &= CUE_FUNCTION^0xFF;
											}
										}
									}
									CueControl();
			
									//Timer-Control
									TimerControl(Module);
											
									//OnAirControl
									OnAirControl();

									//DJ-Control
									DJControl();
			
									//Studio-Mic-Control
									StudioMicControl();

									CheckStartCondition(Module);
														
									ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;					
								}
								SendSerialData = 1;
							}
						}
					}
				}
				break;
				case TELCO:
				{ //Telco
					if (EventOn)
					{
						char TelcoNr = GetTelcoNrFromModuleNr(Module);

						if (ConsoleFunctionData[Module]&ON_LED)
						{
							char TelcoNr = GetTelcoNrFromModuleNr(Module);
							if ((TelcoNr>=0) && (TelcoNr<4))
							{
								SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
								SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
								SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;

								SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_MUTE;
								SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
								SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
							}

							ConsoleFunctionData[Module] &= ON_LED^0xFF;
							ConsoleFunctionData[Module] |= MUTE_FUNCTION;
							ConsoleFunctionData[Module] &= REMOTE_FUNCTION^0xFF;
							ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
							ConsoleFunctionData[Module] &= CUE_LED^0xFF;
//							ConsoleFunctionData[Module] &= START_FUNCTION^0xFF;			
							cntStartFunction[Module] = DELAY;
				
							SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_STOP;
							SerialBufferOut[SerialBufferOutPtrTop++] = Module;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;

							CueControl();

							if ((ModuleData[Module][FADERON]&0x80))
							{	 // !JB
								//Timer-Control
								TimerControl(Module);
				
								//OnAir Control
								OnAirControl();
							}

							if ((TelcoNr>=0) && (TelcoNr<4))
							{
								SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CONNECT;
								SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
								SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
							}
							SendSerialData = 1;
						}
						else
						{
							ConsoleFunctionData[Module] |= ON_LED;
							ConsoleFunctionData[Module] |= START_FUNCTION;			
							ConsoleFunctionData[Module] |= REMOTE_FUNCTION;
				
							SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_START;
							SerialBufferOut[SerialBufferOutPtrTop++] = Module;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
							
							if ((ModuleData[Module][FADERON]&0x80))
							{	// !JB
								char TelcoNr = GetTelcoNrFromModuleNr(Module);
								if ((TelcoNr>=0) && (TelcoNr<4))
								{
									SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
									SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
									SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
								}

								ConsoleFunctionData[Module] &= CUE_LED^0xFF;
								ConsoleFunctionData[Module] &= CUE_FUNCTION^0xFF;
//								ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;
								cntMuteFunction[Module] = DELAY;

								//Cue-Control
								if (ConsoleMasterSettings[AUTOCUERESET])
								{
									if (((ModuleData[Module][FADERON]&0x80)) && ((ConsoleFunctionData[Module]&ON_LED)))
									{		// !JB
										unsigned char cntModule;
										for (cntModule=0; cntModule<NROFMODULES; cntModule++)
										{
											char TelcoNr = GetTelcoNrFromModuleNr(cntModule);
											if ((TelcoNr>=0) && (TelcoNr<4))
											{
												SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
												SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
												SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
											}

											ConsoleFunctionData[cntModule] &= CUE_LED^0xFF;
											ConsoleFunctionData[cntModule] &= CUE_FUNCTION^0xFF;
										}
									}
								}
								CueControl();
			
								//Timer-Control
								TimerControl(Module);
			
								//OnAirControl
								OnAirControl();
							}

							if ((TelcoNr>=0) && (TelcoNr<4))
							{
								SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CONNECT;
								SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
								SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
							}
							SendSerialData = 1;
						}
					}
				}
				break;
			}
		}
		break;
		case LINEB:
		{
			CheckStartCondition(Module);
			TimeAndDate.Hours = 0;
			TimeAndDate.Minutes = 0;
			TimeAndDate.Seconds = 0;
			TimeAndDate.Frames = 0;
			TimerControl(Module);
			DJControl();
			StudioMicControl();
			OnAirControl();
		}
		break;
		case MIC_RING:
		{
			switch (ModuleType[Module])
			{
				case TRIPLE:
				{
					if (!(ModuleData[Module][MIC_RING]&0x80))
					{
						ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
						CheckTBMic();
						SendSerialData = 1;
					}
		
					CheckStartCondition(Module);
					TimeAndDate.Hours = 0;
					TimeAndDate.Minutes = 0;
					TimeAndDate.Seconds = 0;
					TimeAndDate.Frames = 0;
					TimerControl(Module);
					DJControl();
					StudioMicControl();
					OnAirControl();
				}
				break;
				case TELCO:
				{
					char TelcoNr = GetTelcoNrFromModuleNr(Module);
					
					if (EventOn)
					{
						ConsoleFunctionData[Module]|=CUE_LED;
						ConsoleFunctionData[Module]|=REMOTE_FUNCTION;

						if ((TelcoNr>=0) && (TelcoNr<4))
						{
							SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_RING;
							SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
						}
					}
					else
					{
						if (!(ConsoleFunctionData[Module]&START_FUNCTION))
						{
							ConsoleFunctionData[Module]&=CUE_LED^0xFF;
							ConsoleFunctionData[Module]&=REMOTE_FUNCTION^0xFF;
						}

						if ((TelcoNr>=0) && (TelcoNr<4))
						{
							SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_RING;
							SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
						}
					}
					SendSerialData = 1;
				}
				break;
			}
		}
		break;
		case ESCAPE:
		{
			SetLCD(0,"\xC0");
			if (EventOn)
 			{
				if (OperatingMode != NORMALMODE)
				{
					char TextBuffer[9];

					if (OperatingMode==MODULEMODE)
					{
						unsigned char cntModule, cntByte;
						for (cntModule=0; cntModule<NROFMODULES; cntModule++)
						{
							for (cntByte=0; cntByte<29; cntByte++)
							{
								ConsoleModuleSettings[cntModule][cntByte] = UndoConsoleModuleSettings[cntModule][cntByte];
							}
						}
					}
					else if (OperatingMode==MASTERMODE)
					{
						unsigned char cntByte;
						for (cntByte=0; cntByte<5; cntByte++)
						{
								ConsoleMasterSettings[cntByte] = UndoConsoleMasterSettings[cntByte];
						}
					}

					OperatingMode = NORMALMODE;
					TextStringPtr = 0;

					SetLCD(0,"\x80");
					SetLCD(1,"                ");
					SetLCD(0,"\xC0");
					SetLCD(1, TextString[TextStringPtr]);

					TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
					TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
					TextBuffer[2] = ':';
				  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
					TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
					TextBuffer[5] = ':';
					TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
					TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*				TextBuffer[8] = ':';
					TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
					TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
					TextBuffer[11] = 0;*/
					TextBuffer[8] = 0;
					SetLCD(0,"\x84");
					SetLCD(1,TextBuffer);
					SetConsoleData();
				}
				else if (TextStringPtr==0)
				{
					TimeAndDate.Hours = 0;
					TimeAndDate.Minutes = 0;
					TimeAndDate.Seconds = 0;
					TimeAndDate.Frames = 0;
				}
				else
				{
					char TextBuffer[9];
					//New in v1.12
					SetupLCD();
	 				SetLCD(0,"\xC0");

					SetLCD(0,"\x80");
					SetLCD(1,"                ");
					SetLCD(0,"\xC0");
					SetLCD(1, TextString[TextStringPtr]);

					TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
					TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
					TextBuffer[2] = ':';
				  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
					TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
					TextBuffer[5] = ':';
					TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
					TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*				TextBuffer[8] = ':';
					TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
					TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
					TextBuffer[11] = 0;*/
					TextBuffer[8] = 0;
					SetLCD(0,"\x84");
					SetLCD(1,TextBuffer);
					SetConsoleData();
				}
/* 			ReadData(I2CAddressEEProm, 0x0000, Buffer1, 16);
				Buffer1[16] = 0;
				SetLCD(1,Buffer1);*/
			}
		}
		break;
		case ENTER:
		{
			if (EventOn)
			{
				switch (OperatingMode)
				{
					case NORMALMODE:
					{
						switch (TextStringPtr)
						{
							case 0:
							{
								TimeAndDate.Hours = 0;
								TimeAndDate.Minutes = 0;
								TimeAndDate.Seconds = 0;
								TimeAndDate.Frames = 0;
								GlobalTimerActive = 1;
							}
							break;
							case 1:
							{
								unsigned char cntModule, cntByte;
								for (cntModule=0; cntModule<NROFMODULES; cntModule++)
								{
									for (cntByte=0; cntByte<29; cntByte++)
									{
										UndoConsoleModuleSettings[cntModule][cntByte] = ConsoleModuleSettings[cntModule][cntByte];
									}
								}
								OperatingMode = MODULEMODE;
								TextStringPtr = 0;
								SetLCD(0,"\x80");
								SetLCD(1,ModuleTextString[TextStringPtr]);
								SetLCD(0,"\xC0");
//								SetLCD(1,"Select Module   ");
								SetLCD(1,"Toggle by SELECT");
								ShowFunction(TextStringPtr);
							}
							break;
							case 2:
							{
								unsigned char cntByte;
								for (cntByte=0; cntByte<5; cntByte++)
								{
										UndoConsoleMasterSettings[cntByte] = ConsoleMasterSettings[cntByte];
								}

								OperatingMode = MASTERMODE;
								TextStringPtr = 0;
								SetLCD(0,"\x80");
								SetLCD(1,MasterTextString[TextStringPtr]);

								SetLCD(0,"\xC0");
								if (ConsoleMasterSettings[TextStringPtr])
								{
									SetLCD(1,"Yes             ");
								}
								else
								{
									SetLCD(1," No             ");
								}
							}
							break;
							case 3:
							{
								OperatingMode = SYSTEMMODE;
								TextStringPtr = 0;
								SetLCD(0,"\x80");
								SetLCD(1,SystemTextString[TextStringPtr]);
								ShowConfig();
							}
							break;
						}
					}
					break;
					case SYSTEMMODE:
					{
						char TextBuffer[9];
						switch (TextStringPtr)
						{
							case 1:
							{	// EEPROM Load
								unsigned char cntModule;
								unsigned int Address = 0x0000;

								Address = 0x0000;
							  ReadData(I2CAddressEEProm, Address, &(ModuleType[0]), NROFMODULES);
							  Address += NROFMODULES;
								for (cntModule=0; cntModule<NROFMODULES; cntModule++)
								{
									ReadData(I2CAddressEEProm, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
								  Address += 29;
								}
								ReadData(I2CAddressEEProm, Address, &(ConsoleMasterSettings[0]), 5);


								{//Reload
									unsigned char cntByte, cntChannel;

									for (cntChannel=0; cntChannel<16; cntChannel++)
									{
										if ((ModuleType[cntChannel]==0x01) || (ModuleType[cntChannel]==0x02))
										{
											SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
											SerialBufferOut[SerialBufferOutPtrTop++] = cntChannel;
											for (cntByte=0; cntByte<29; cntByte++)
											{
												SerialBufferOut[SerialBufferOutPtrTop++] =
												ConsoleModuleSettings[cntChannel][cntByte]&0x7F;
								 		  }
										}
										if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom) && SendSerialReady)
										{
											USARTE0_DATA = SerialBufferOut[SerialBufferOutPtrBottom++];
											SendSerialReady = 0;
										}
										while(!SendSerialReady);
									}

									SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
									SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
									for (cntByte=0; cntByte<5; cntByte++)
									{
										SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
									}
								}
							}
							break;
							case 2:
							{ // EEPROM Write
								//unsigned char cntModule;
								//unsigned int Address = 0x0000;
//
								//SetLCD(0,"\x80");
								//SetLCD(1,"Saving...");
								//
								////WriteData(I2CAddressEEProm, Address, ModuleType, NROFMODULES);
								////Transmit the message
								//TWIM_Transmit(I2CAddressEEProm, Address, ModuleType, NROFMODULES);
								//// Wait until data got transmitted
								//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
								//
								//Address += NROFMODULES;
								//for (cntModule=0; cntModule<NROFMODULES; cntModule++)
								//{
									////WriteData(I2CAddressEEProm, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
									////Transmit the message
									//TWIM_Transmit(I2CAddressEEProm, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
									//// Wait until data got transmitted
									//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
									//Address += 29;
								//}
								////WriteData(I2CAddressEEProm, Address, ConsoleMasterSettings, 5);
								////Transmit the message
								//TWIM_Transmit(I2CAddressEEProm, Address, ConsoleMasterSettings, 5);
								//// Wait until data got transmitted
								//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
							}
							break;
							case 3:
							{ // SmartCard Load
								unsigned char ReadedModuleType[NROFMODULES];
								unsigned char cntModule;
								unsigned int Address = 0x0000;
								unsigned char DifferentConfiguration=0;

							  ReadData(I2CAddressSmartCard, Address, ReadedModuleType, NROFMODULES);

								for (cntModule=0; cntModule<NROFMODULES; cntModule++)
								{
									if (ModuleType[cntModule] != ReadedModuleType[cntModule])
									{
										DifferentConfiguration = 1;
									}
								}

								if (!DifferentConfiguration)
								{
									Address += NROFMODULES;
									for (cntModule=0; cntModule<NROFMODULES; cntModule++)
									{
										ReadData(I2CAddressSmartCard, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
										Address += 29;
									}
									ReadData(I2CAddressSmartCard, Address, ConsoleMasterSettings, 5);

									{//Reload
										unsigned char cntByte, cntChannel;

										for (cntChannel=0; cntChannel<16; cntChannel++)
										{
  										if ((ModuleType[cntChannel]==0x01) || (ModuleType[cntChannel]==0x02))
											{
												SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MODULESETUPDATA;
												SerialBufferOut[SerialBufferOutPtrTop++] = cntChannel;
												for (cntByte=0; cntByte<29; cntByte++)
												{
													ConsoleModuleSettings[cntChannel][cntByte]&=0x01;
													SerialBufferOut[SerialBufferOutPtrTop++] =
													ConsoleModuleSettings[cntChannel][cntByte]&0x7F;
									 		  }
											}
											if ((SerialBufferOutPtrTop != SerialBufferOutPtrBottom) && SendSerialReady)
											{
												USARTE0_DATA = SerialBufferOut[SerialBufferOutPtrBottom++];
												SendSerialReady = 0;
											}
											while(!SendSerialReady);
										}
	
										SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
										SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
										for (cntByte=0; cntByte<5; cntByte++)
										{
											SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
										}
									}
 							  }
								else
								{
									SetLCD(0,"\xC0");
									SetLCD(1,"Other Config.   ");
									Delay(100);
								}
							}
							break;
							case 4:
							{ // SmartCard Write
			 					unsigned char cntModule;
								unsigned int Address = 0x0000;

								SetLCD(0,"\x80");
								SetLCD(1,"Saving...");

								////WriteData(I2CAddressSmartCard, Address, ModuleType, NROFMODULES);
																////Transmit the message
																//TWIM_Transmit(I2CAddressSmartCard, Address, ModuleType, NROFMODULES);
																//// Wait until data got transmitted
																//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
								//
								//Address += NROFMODULES;
								//for (cntModule=0; cntModule<NROFMODULES; cntModule++)
								//{
									////WriteData(I2CAddressSmartCard, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
																	////Transmit the message
																	//TWIM_Transmit(I2CAddressSmartCard, Address, &(ConsoleModuleSettings[cntModule][0]), 29);
																	//// Wait until data got transmitted
																	//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
									//Address += 29;
								//}
								////WriteData(I2CAddressSmartCard, Address, ConsoleMasterSettings, 5);
																////Transmit the message
																//TWIM_Transmit(I2CAddressSmartCard, Address, ConsoleMasterSettings, 5);
																//// Wait until data got transmitted
																//while(!((TWIM_Status() == TWI_MASTER_SEND) || (TWIM_Status() == TWI_MASTER_ERROR)));
							}
							break;
						}
						OperatingMode = NORMALMODE;
						TextStringPtr = 0;

						SetLCD(0,"\x80");
						SetLCD(1,"                ");
						SetLCD(0,"\xC0");
						SetLCD(1, TextString[TextStringPtr]);

						TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
						TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
						TextBuffer[2] = ':';
					  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
						TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
						TextBuffer[5] = ':';
						TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
						TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*						TextBuffer[8] = ':';
						TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
						TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
						TextBuffer[11] = 0;*/
						TextBuffer[8] = 0;
						SetLCD(0,"\x84");
						SetLCD(1,TextBuffer);
					}
					break;
					default:
					{
						char TextBuffer[9];

						OperatingMode = NORMALMODE;
						TextStringPtr = 0;

						SetLCD(0,"\x80");
						SetLCD(1,"                ");
						SetLCD(0,"\xC0");
						SetLCD(1, TextString[TextStringPtr]);

						TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
						TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
						TextBuffer[2] = ':';
					  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
						TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
						TextBuffer[5] = ':';
						TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
						TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*						TextBuffer[8] = ':';
						TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
						TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
						TextBuffer[11] = 0;*/
						TextBuffer[8] = 0;
						SetLCD(0,"\x84");
						SetLCD(1,TextBuffer);
						SetConsoleData();
					}
					break;
				}
			}
		}
		break;
		case RIGHT:
		{
			if (EventOn)
			{
				switch (OperatingMode)
				{
					case NORMALMODE:
					{
					}
					break;
					case MASTERMODE:
					{
						if (ConsoleMasterSettings[4])
						{
							unsigned char cntByte;

							ConsoleMasterSettings[TextStringPtr] = 1;
							SetLCD(0,"\xC0");
							SetLCD(1,"Yes             ");

							SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
							for (cntByte=0; cntByte<5; cntByte++)
							{
								SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
							}
						}
					}
					break;
				}
			}
		}
		break;
		case UP:
		{
			if (EventOn)
			{		
				TextStringPtr--;
				switch (OperatingMode)
				{
					case NORMALMODE:
					{
						if (TextStringPtr>3)
							TextStringPtr=3;
						SetLCD(0,"\xC0");					
		 				SetLCD(1,TextString[TextStringPtr]);
					}
					break;
					case MODULEMODE:
					{
						if (TextStringPtr>28)
							TextStringPtr=28;

						SetLCD(0,"\x80");					
						SetLCD(1,ModuleTextString[TextStringPtr]);
						SetLCD(0,"\xC0");							
//						SetLCD(1,"Select Module   ");
						SetLCD(1,"Toggle by SELECT");
						ShowFunction(TextStringPtr);
					}
					break;
					case MASTERMODE:
					{
						if (TextStringPtr>3)
							TextStringPtr=3;
						SetLCD(0,"\x80");										
						SetLCD(1,MasterTextString[TextStringPtr]);
						SetLCD(0,"\xC0");
						if (ConsoleMasterSettings[TextStringPtr])
						{
							SetLCD(1,"Yes             ");
						}
						else
						{
							SetLCD(1," No             ");
						}
					}
					break;
					case SYSTEMMODE:
					{
						if (TextStringPtr>5)
							TextStringPtr=5;

						if (!CardInserted)
						{
							if (TextStringPtr==4)
								TextStringPtr=2;
						}
						SetLCD(0,"\x80");										
						SetLCD(1,SystemTextString[TextStringPtr]);

						switch (TextStringPtr)
						{
							case 0:
							{
								ShowConfig();
							}
							break;
							case 5:
							{
								SetLCD(0,"\xC0");
								SetLCD(1,IDString);
							}
							break;
							default:
							{
								SetLCD(0,"\xC0");
								SetLCD(1,"Press Enter/Esc ");
							}
							break;
						}
					}
					break;
				}
			}
		}
		break;
		case DOWN:
		{
			if (EventOn)
			{
				TextStringPtr++;
				switch (OperatingMode)
				{
					case NORMALMODE:
					{
						if (TextStringPtr>3)
							TextStringPtr=0;
						SetLCD(0,"\xC0");			
		 				SetLCD(1,TextString[TextStringPtr]);
					}
					break;
					case MODULEMODE:
					{
						if (TextStringPtr>28)
							TextStringPtr=0;

						SetLCD(0,"\x80");							
						SetLCD(1,ModuleTextString[TextStringPtr]);
						SetLCD(0,"\xC0");							
//						SetLCD(1,"Select Module   ");
						SetLCD(1,"Toggle by SELECT");
						ShowFunction(TextStringPtr);
					}
					break;
					case MASTERMODE:
					{
						if (TextStringPtr>3)
							TextStringPtr=0;
						SetLCD(0,"\x80");					
						SetLCD(1,MasterTextString[TextStringPtr]);
						SetLCD(0,"\xC0");
						if (ConsoleMasterSettings[TextStringPtr])
						{
							SetLCD(1,"Yes             ");
						}
						else
						{
							SetLCD(1," No             ");
						}
					}
					break;
					case SYSTEMMODE:
					{
						if (TextStringPtr>5)
							TextStringPtr=0;

						if (!CardInserted)
						{
							if (TextStringPtr==3)
								TextStringPtr=5;
						}
						SetLCD(0,"\x80");										
						SetLCD(1,SystemTextString[TextStringPtr]);
						switch (TextStringPtr)
						{
							case 0:
							{
								ShowConfig();
							}
							break;
							case 5:
							{
								SetLCD(0,"\xC0");
								SetLCD(1,IDString);
							}
							break;
							default:
							{
								SetLCD(0,"\xC0");
								SetLCD(1,"Press Enter/Esc ");
							}
							break;
						}
					}
					break;
				}
			}
		}
		break;
		case LEFT:
		{
			if (EventOn)
			{
				switch (OperatingMode)
				{
					case NORMALMODE:
					{
					}
					break;
					case MASTERMODE:
					{
						if (ConsoleMasterSettings[4])
						{
							unsigned char cntByte;

							ConsoleMasterSettings[TextStringPtr] = 0;
							SetLCD(0,"\xC0");
							SetLCD(1," No             ");
							switch (TextStringPtr)
							{
								case AUTOCUERESET:
								case CUETOCRM:
								{
									CueControl();
								}
								break;
								case AUTOCOM:
								{
									ConsoleFunctionData[16] &= ANNOUNCER_COMM_FUNCTION_LED^0xFF;
									if ((ModuleData[Module][MIC_RING]&0x80) && (ConsoleModuleSettings[Module][ANNMIC]) && (ConsoleFunctionData[Module]&MUTE_FUNCTION) && (ConsoleMasterSettings[AUTOCOM]))
									{
										ConsoleFunctionData[16] |= ANNOUNCER_COMM_FUNCTION_LED;
									}
   								CheckTBMic();
									SendSerialData = 1;
								}
								break;
							}
	
							SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
							for (cntByte=0; cntByte<5; cntByte++)
							{
								SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
							}
						}
 					}
					break;

				}
			}
		}
		break;
		case CUERESET:
		{
			unsigned char cntModule;

			for (cntModule=0; cntModule<NROFMODULES; cntModule++)
			{
				ConsoleFunctionData[cntModule] &= CUE_LED^0xFF;
				ConsoleFunctionData[cntModule] &= CUE_FUNCTION^0xFF;

				if (ModuleType[cntModule] == TELCO)
				{
					char TelcoNr = GetTelcoNrFromModuleNr(cntModule);
					if ((TelcoNr>=0) && (TelcoNr<4))
					{
						SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_CUE;
						SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
						SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
					}
				}
			}
			CueControl();
			DJControl();
			StudioMicControl();
			CheckStartCondition(cntModule);

			SendSerialData = 1;
 		}
		break;
		case SW_CC:
		{
			if (EventOn)
			{
				ConsoleFunctionData[16] &= NOT_POWER_ON_CC^0xFF;
				SetConsoleData();
				Delay(5);
				// CS_CC = 0xFF;   // willem
				Delay(5);
				CardInserted = 1;
				SetAddress(I2CAddressSmartCard, 0x00);
				if (CardInserted)
				{
					unsigned int Address;
					unsigned char cntByte;

					OperatingMode = SYSTEMMODE;
					TextStringPtr = 3;
					SetLCD(0,"\x80");
					SetLCD(1,SystemTextString[TextStringPtr]);
					SetLCD(0,"\xC0");
					SetLCD(1,"Press Enter/Esc ");

					SerialBufferOut[SerialBufferOutPtrTop++] = CARD_INSERTED;
					SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
					SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
				
					TempEnableSetup = ConsoleMasterSettings[4];
					Address = NROFMODULES + (NROFMODULES*29) + 4;
					ReadData(I2CAddressSmartCard, Address, &(ConsoleMasterSettings[4]), 1);

					SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
					SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
					for (cntByte=0; cntByte<5; cntByte++)
					{
						SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
					}
				}
				SendSerialData = 1;
			}
			else
			{
				unsigned char cntByte;

				CardInserted = 0;

				SerialBufferOut[SerialBufferOutPtrTop++] = CARD_INSERTED;
				SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
				SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;

				ConsoleMasterSettings[4] = TempEnableSetup; 

				SerialBufferOut[SerialBufferOutPtrTop++] = SERIAL_MASTERSETUPDATA;
				SerialBufferOut[SerialBufferOutPtrTop++] = 0x00;
				for (cntByte=0; cntByte<5; cntByte++)
				{
					SerialBufferOut[SerialBufferOutPtrTop++] = ConsoleMasterSettings[cntByte]&0x7F;
				}

				ConsoleFunctionData[16] |= NOT_POWER_ON_CC;
				SendSerialData = 1;
				// CS_CC = 0x00;   willem
				if ((OperatingMode == SYSTEMMODE) && ((TextStringPtr==3) || ((TextStringPtr==4))))
				{
					char TextBuffer[9];

					OperatingMode = NORMALMODE;
					TextStringPtr = 0;

					SetLCD(0,"\x80");
					SetLCD(1,"                ");
					SetLCD(0,"\xC0");
					SetLCD(1, TextString[TextStringPtr]);

					TextBuffer[0] = 0x30+(TimeAndDate.Hours/10);
					TextBuffer[1] = 0x30+(TimeAndDate.Hours%10);
					TextBuffer[2] = ':';
				  TextBuffer[3] = 0x30+(TimeAndDate.Minutes/10);
					TextBuffer[4] = 0x30+(TimeAndDate.Minutes%10);
					TextBuffer[5] = ':';
					TextBuffer[6] = 0x30+(TimeAndDate.Seconds/10);
					TextBuffer[7] = 0x30+(TimeAndDate.Seconds%10);
/*					TextBuffer[8] = ':';
					TextBuffer[9] = 0x30+(TimeAndDate.Frames/10);
					TextBuffer[10] = 0x30+(TimeAndDate.Frames%10);
					TextBuffer[11] = 0;*/
					TextBuffer[8] = 0;
					SetLCD(0,"\x84");
					SetLCD(1,TextBuffer);
				}
			}
		}
		break;
		case MUTE:
		{
			char TelcoNr = GetTelcoNrFromModuleNr(Module);
			
			if (ConsoleFunctionData[Module]&ON_LED)
			{
				if (EventOn)
				{
						ConsoleFunctionData[Module] |= MUTE_FUNCTION;
						cntMuteFunction[Module] = 0;
	
						if ((TelcoNr>=0) && (TelcoNr<4))
						{
							SerialBufferOut[SerialBufferOutPtrTop++] = TODIRECTOR_TELCO_MUTE;
							SerialBufferOut[SerialBufferOutPtrTop++] = TelcoNr;
							SerialBufferOut[SerialBufferOutPtrTop++] = 0x01;
						}
	
						SendSerialData = 1;
				}
				else
				{
					if (cntMuteFunction[Module] == 0)
					{
							ConsoleFunctionData[Module] &= MUTE_FUNCTION^0xFF;
	
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
		}
		break;
	}
}
