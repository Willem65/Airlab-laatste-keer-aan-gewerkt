
#ifndef __READCONSOLE_H__
#define __READCONSOLE_H__

extern unsigned char Bits;

void ReadConsole();
void ReadMasterSwitches();
void ReadModuleSwitches(unsigned char High);
void ShowConfig(void);


#endif