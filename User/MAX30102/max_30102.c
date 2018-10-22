#include "bsp_i2c_gpio.h"
#include "max_30102.h"

#define max30102_WR_address 0xAE

u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{

	i2c_Start();

	i2c_SendByte(max30102_WR_address | I2C_WR);

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}

	i2c_SendByte(Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}
	
	i2c_SendByte(Word_Data);


	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	
	}


	i2c_Stop();
	return 1;

cmd_fail:

	i2c_Stop();
	return 0;
}



u8 max30102_Bus_Read(u8 Register_Address)
{
	u8  data;

	i2c_Start();

	i2c_SendByte(max30102_WR_address | I2C_WR);

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}

	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}

	i2c_Start();

	i2c_SendByte(max30102_WR_address | I2C_RD);

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}

	{
		data = i2c_ReadByte();

		i2c_NAck();
	}

	i2c_Stop();
	return data;

cmd_fail:

	i2c_Stop();
	return 0;
}


void max30102_FIFO_Read(u8 Register_Address,u16  Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;

	i2c_Start();

	i2c_SendByte(max30102_WR_address | I2C_WR);

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}

	i2c_SendByte((uint8_t)Register_Address);
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}
	
	i2c_Start();

	i2c_SendByte(max30102_WR_address | I2C_RD);

	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;
	}

	while (no)
	{
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		i2c_Ack();
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //

		
		data1 = i2c_ReadByte();	
		i2c_Ack();
		data2 = i2c_ReadByte();
		if(1==no)
			i2c_NAck();
		else
			i2c_Ack();
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 

		no--;	
		i++;
	}
	
	i2c_Stop();

cmd_fail:
	
	i2c_Stop();
}

#define INTERRUPT_REG  					0X00


void max30102_init()
{
	
	max30102_Bus_Write(0x09, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
	max30102_Bus_Write(0x01, 0xF0); //open all of interrupt
	max30102_Bus_Write(INTERRUPT_REG, 0x00); //all interrupt clear
	max30102_Bus_Write(0x03, 0x02); //DIE_TEMP_RDY_EN
	max30102_Bus_Write(0x21, 0x01); //SET   TEMP_EN

	max30102_Bus_Write(0x0a, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

	max30102_Bus_Write(0x0c, 0x47); 
	max30102_Bus_Write(0x0d, 0x47); 
											
}

