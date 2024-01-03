

/****************************************************************
* delay(100) = ca. 1 Sec                                        *
****************************************************************/
void Delay(int tijd)
{
	int x, y;
	for (x = 1; x <= tijd; x++)
	{
		for (y = 0; y <= 10000; y++)
		{
			__asm__ volatile("" : "+g" (tijd) : :);
			// Watchdog
			//PCON |= 0x10;
			//T3 = 0;
			;
		}
	}
}

void Delay_ms(int tijd)
{
	int y;
	//for (x = 1; x <= tijd; x++)
	//{
		for (y = 0; y <= 200; y++)
		{
			__asm__ volatile("" : "+g" (tijd) : :);
			// Watchdog
			//PCON |= 0x10;
			//T3 = 0;
			;
		}
	//}
}