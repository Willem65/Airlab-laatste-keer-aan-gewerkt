/*
 * TWIM.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Driver for XMega master mode TWI.

  GNU GENERAL PUBLIC LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

#include "TWI.h"

static TWI_Message_t Message;
volatile uint16_t wacht;

static void TWIM_ErrorHandler(void)
{
	Message.Status = TWI_MASTER_ERROR;

	Message.Device->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
}

static void TWIM_ArbitrationLostHandler(void)
{
	TWIM_ErrorHandler();
}

static void TWIM_WriteHandler(void)
{
	// Abort transmission if slave has send an NACK
	if(Message.Device->MASTER.STATUS & TWI_MASTER_RXACK_bm)
	{
		// Slave send NACK instead of ACK - abort transmission
		TWIM_ErrorHandler();
	}
	else
	{
		if(Message.Status == TWI_MASTER_REGISTER)
		{
			Message.Device->MASTER.DATA = Message.Register;

			if(Message.BytesRead > 0x00)
			{
				Message.Status = TWI_MASTER_ADDRESS;
			}
			else
			{
				Message.Status = TWI_MASTER_WRITE;
			}
		}
		else if(Message.Status == TWI_MASTER_ADDRESS)
		{
			Message.Device->MASTER.ADDR = TWI_READ(Message.DeviceAddress);
			Message.Status = TWI_MASTER_READ;
		}
		else if(Message.Status == TWI_MASTER_WRITE)
		{
			if ( Message.IndexWrite  < Message.BytesWrite  )
			{
				Message.Device->MASTER.DATA = Message.BufferWrite[ Message.IndexWrite++];
				
			}
			else
			{
				Message.Device->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
				Message.Status = TWI_MASTER_SEND;
			}
		}
	}
}

static void TWIM_ReadHandler(void)
{
	if(Message.Status == TWI_MASTER_READ)
	{
		// Check for buffer overflow
		if(Message.IndexRead < TWI_BUFFER_SIZE)
		{
			Message.BufferRead[Message.IndexRead++] = Message.Device->MASTER.DATA;
		}
		else
		{
			Message.Device->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
			Message.Status = TWI_MASTER_BUFFEROVERFLOW;
		}
		
		if(Message.IndexRead < Message.BytesRead)
		{
			Message.Device->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
		}
		else
		{
			Message.Device->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
			Message.Status = TWI_MASTER_RECEIVED;
		}
	}
	else
	{
		TWIM_ErrorHandler();		
	}
}

void TWIM_Init(void)
{
	TWIE_CTRL = 0x00;
	TWIE_MASTER_BAUD = 0x28;
	TWIE_MASTER_CTRLA = TWI_MASTER_ENABLE_bm;
	TWIE_MASTER_CTRLB = 0x00;

	// Set the state machine into idle state
	TWIE_MASTER_STATUS |= TWI_MASTER_BUSSTATE_IDLE_gc;
}

void TWIM_InitInterrupt(void)
{
	TWIE_CTRL = 0x00;
	TWIE_MASTER_BAUD = 0x28;
	TWIE_MASTER_CTRLA = TWI_MASTER_INTLVL_LO_gc | TWI_MASTER_RIEN_bm | TWI_MASTER_WIEN_bm | TWI_MASTER_ENABLE_bm;
	TWIE_MASTER_CTRLB = 0x00;

	// Set the state machine into idle state
	TWIE_MASTER_STATUS |= TWI_MASTER_BUSSTATE_IDLE_gc;
}

void TWIM_SendAddress(uint8_t Address)
{
	// You can use this function for read and write, because the WIF and RIF flag is cleared when you write
	// an address into the ADDR register. So after each address transfer (read or write) the right interrupt flag
	// is set.
	TWIE_MASTER_ADDR = Address;
	while(!((TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm) || (TWIE_MASTER_STATUS & TWI_MASTER_RIF_bm)));
}

void TWIM_SendData(uint8_t Data)
{
	TWIE_MASTER_DATA = Data;
	while(!(TWIE_MASTER_STATUS & TWI_MASTER_WIF_bm));
	for (wacht=0; wacht<20000; wacht++);
}

uint8_t TWIM_ReadData(uint8_t NACK)
{
	uint8_t Data = 0x00;

	while(!(TWIE_MASTER_STATUS & TWI_MASTER_RIF_bm));
	Data = TWIE_MASTER_DATA;

	if(!NACK)
	{
		TWIE_MASTER_CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}

	return Data;
}

void TWIM_SendStop(uint8_t NACK)
{
	if(NACK)
	{
		TWIE_MASTER_CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_STOP_gc;
	}
	else
	{
		TWIE_MASTER_CTRLC = TWI_MASTER_CMD_STOP_gc;
	}
}



//void TWIM_TransmitBytes(uint8_t DeviceAddress, uint8_t Bytes, uint8_t* Data)
void TWIM_TransmitBytes(uint8_t DeviceAddress, uint8_t* Data, uint8_t Bytes)
{
	Message.BufferRead = 0x00;
	Message.BytesRead = 0x00;

	Message.IndexWrite = 0x00;
	Message.BufferWrite = Data;
	Message.BytesWrite = Bytes;
	Message.Device = &TWIE;
	Message.DeviceAddress = DeviceAddress;
	Message.Register = 0x00;
	Message.Status = TWI_MASTER_WRITE;

	// Start the transmission by writing the address
	Message.Device->MASTER.ADDR = TWI_WRITE(Message.DeviceAddress);
	
	for (wacht=0; wacht<15000; wacht++);
}



//void TWIM_Transmit(uint8_t DeviceAddress, uint8_t Register, uint8_t Bytes, uint8_t* Data)
void TWIM_Transmit(uint8_t DeviceAddress, uint8_t Register, unsigned char* Data, uint8_t Bytes)
{
	Message.BufferRead = 0x00;
	Message.BytesRead = 0x00;	

	Message.IndexWrite = 0x00;
	Message.BufferWrite = Data;
	Message.BytesWrite = Bytes;
	Message.Device = &TWIE;
	Message.DeviceAddress = DeviceAddress;
	Message.Register = Register;
	Message.Status = TWI_MASTER_REGISTER;

	// Start the transmission by writing the address
	Message.Device->MASTER.ADDR = TWI_WRITE(Message.DeviceAddress);

	for (wacht=0; wacht<15000; wacht++);

}

//void TWIM_ReceiveBytes(uint8_t DeviceAddress, uint8_t Bytes, uint8_t* Data)
void TWIM_ReceiveBytes(uint8_t DeviceAddress, uint8_t* Data, uint8_t Bytes)
{
	Message.BufferWrite = 0x00;
	Message.BytesWrite = 0x00;

	Message.IndexRead = 0x00;
	Message.BufferRead = Data;
	Message.BytesRead = Bytes;
	Message.Device = &TWIE;
	Message.DeviceAddress = DeviceAddress;
	Message.Register = 0x00;
	Message.Status = TWI_MASTER_READ;

	// Start the transmission by writing the address
	Message.Device->MASTER.ADDR = TWI_READ(Message.DeviceAddress);
	
	for (wacht=0; wacht<20000; wacht++);
}

//void TWIM_Receive(uint8_t DeviceAddress, uint8_t Register, uint8_t Bytes, uint8_t* Data)
void TWIM_Receive(uint8_t DeviceAddress, uint8_t Register, unsigned char* Data, uint8_t Bytes)
{
	Message.BufferWrite = 0x00;
	Message.BytesWrite = 0x00;

	Message.IndexRead = 0x00;
	Message.BufferRead = Data;
	Message.BytesRead = Bytes;
	Message.Device = &TWIE;
	Message.DeviceAddress = DeviceAddress;
	Message.Register = Register;
	Message.Status = TWI_MASTER_REGISTER;

	// Start the transmission by writing the address
	Message.Device->MASTER.ADDR = TWI_WRITE(Message.DeviceAddress);
	
	for (wacht=0; wacht<20000; wacht++);
}

TWI_MasterStatus_t TWIM_Status(void)
{
	return Message.Status;
}

ISR(TWIE_TWIM_vect)
{
	uint8_t Status = Message.Device->MASTER.STATUS;

	/*
		Arbitration lost
	*/
	if(Status & TWI_MASTER_ARBLOST_bm)
	{
		TWIM_ArbitrationLostHandler();
	}
	/*
		Write interrupt
	*/
	else if(Status & TWI_MASTER_WIF_bm)
	{	
		TWIM_WriteHandler();
	}
	/*
		Read interrupt
	*/
	else if(Status & TWI_MASTER_RIF_bm)
	{
		TWIM_ReadHandler();
	}
	/*
		Error
	*/
	else
	{
		TWIM_ErrorHandler();
	}
}