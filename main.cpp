/*
 * LCD_CUSTOM_CHARACTER.cpp
 *
 * Created: 18-04-2020 13:41:18
 * Author : abhin
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#define LCD_data PORTD
#define Data_dir_LCD_data DDRD
#define LCD_Control PORTB
#define Data_dir_LCD_Control DDRB
#define LCD_Enable 2
#define LCD_RW 1
#define LCD_RS 0

void peek_a_boo(void);
void send_a_command(unsigned char command);
void send_a_character(unsigned char Character);
void Check_if_busy(void);
void send_a_string(char *name);
void LCD_custom_character(unsigned char loc,unsigned char *msg);
unsigned char character1[8]={0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00,0x00};
unsigned char character2[8]={0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
unsigned char character3[8]={0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x00};
unsigned char character4[8]={0x01,0x03,0x05,0x09,0x0B,0x1B,0x18,0x00};		
int main(void)
{
	Data_dir_LCD_Control |=(1<<LCD_Enable) |(1<<LCD_RS) |(1<<LCD_RW);
	_delay_ms(15);
	
	send_a_command(0x30);
	_delay_ms(1);
	send_a_command(0x30);
	_delay_ms(1);
	send_a_command(0x30);
	_delay_ms(1);
	send_a_command(0x01);
	_delay_ms(2);
	send_a_command(0x38);
	_delay_us(50);
	send_a_command(0x0C);
	_delay_us(50);
	send_a_command(0x06);
	_delay_us(50);
	send_a_command(0x80);
	_delay_us(50);
	

	LCD_custom_character(0,character1);
	LCD_custom_character(1,character2);
	LCD_custom_character(2,character3);
	LCD_custom_character(3,character4);
	for(char x=0;x<4;x++)
	{
		send_a_character(0x00 + x);
		send_a_string(" ");
	}
	

	//(" I LOVE U ");
	//send_a_character(0x00);
	
	
	
	
	//send_a_command(0xC0);
	//send_a_character(0x00);

	//send_a_string(" I MISS U ");
	//send_a_character(0x00);
	
	while (1)
	{
		
		
	}
}

void Check_if_busy()
{
	Data_dir_LCD_data=0x00;
	LCD_Control |=(1<< LCD_RW);
	LCD_Control &=~(1<< LCD_RS);
	
	while(LCD_data>=0x80);
	{
		peek_a_boo();
	}
	
	Data_dir_LCD_data=0xFF;
	
}
void peek_a_boo()
{
	LCD_Control |= 1<<LCD_Enable;
	asm volatile("nop");
	asm volatile("nop");
	LCD_Control &=~(1<<LCD_Enable);
	
}
void send_a_command(unsigned char command)
{
	Check_if_busy();
	LCD_data=command;
	LCD_Control &=~(1<<LCD_RW);
	LCD_Control &=~(1<<LCD_RS);
	peek_a_boo();
	LCD_data=0;
}
void send_a_character(unsigned char Character)
{
	Check_if_busy();
	LCD_data=Character;
	LCD_Control &=~(1<<LCD_RW);
	LCD_Control |=(1<<LCD_RS);
	peek_a_boo();
	//LCD_data=0;
	
}
void send_a_string(char *name)
{
	while(*name>0)
	{
		send_a_character(*name ++);
	}
}

void LCD_custom_character(unsigned char loc,unsigned char *msg)
{
	char i;
	if(loc<4)
		send_a_command(0x40+loc*8);
		for(i=0; i<8;i++)
		{
			send_a_character(msg[i]);
		}
		send_a_command(0x80);
}
	