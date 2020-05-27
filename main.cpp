/*
 * digital_Clock.cpp
 *
 * Created: 08-05-2020 21:13:32
 * Author : abhin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#define LCD_data PORTD
#define Data_dir_LCD_data DDRD
#define LCD_Control PORTB
#define Data_dir_LCD_Control DDRB
#define LCD_Enable 2
#define LCD_RW 1
#define LCD_RS 0
void gotolocation(uint8_t x,uint8_t y);
void peek_a_boo(void);
void send_a_command(unsigned char command);
void send_a_character(unsigned char Character);
void Check_if_busy(void);
void send_a_string(char *name);
char firstcolumnposition[2]={0,64};
char position_String[4];
int sec=0;
int min=0;
int hrs=0;
char minutes[4];
char seconds[4];
char hours[4];
int button_pressed=0;
int released_confidence=0;
int pressed_confidence=0;
void initialize(void);
void Blink(void);
int main(void)
{
    DDRC&=~(1<<PINC5);
	DDRC&=~(1<<PINC4);
	DDRC&=~(1<<PINC3);
	DDRC&=~(1<<PINC2);
	TCCR1B|=(1<<CS11)|(1<<CS10);
	Data_dir_LCD_Control|=(1<<LCD_Control)|(1<<LCD_Enable)|(1<<LCD_RS)|(1<<LCD_RW);
	PORTC|=(1<<PINC5);
	PORTC|=(1<<PINC4);
	PORTC|=(1<<PINC3);
	send_a_command(0x01);
	_delay_ms(2);
	send_a_command(0x38);
	_delay_us(50);
	send_a_command(0x0C);
	_delay_us(50);
	send_a_command(0x80);
	_delay_us(50);
	send_a_command(0xC2);
	_delay_us(50);
	send_a_string("DIGITAL CLOCK");
	send_a_command(0x80);
	initialize();

	
    while (1) 
    {
				
		if(TCNT1==15625)
		{
			TCNT1=0;
			
			sec++;
			itoa(sec,seconds,10);
			Blink();
			if(sec<=9)
			{
				gotolocation(9,1);
				send_a_string("0");
				gotolocation(10,1);
				send_a_string(seconds);	
			}
		    else
			{
				gotolocation(9,1);
				send_a_string(seconds);
			}
		}
		if(sec==60)
		{
			
			min++;
			if(min<=9)
			{
				sec=0;
				gotolocation(9,1);
				send_a_string("0");
				itoa(min,minutes,10);
				gotolocation(6,1);
				send_a_string("0");
				gotolocation(7,1);
				send_a_string(minutes);
			}
			else
			{
				sec=0;
				gotolocation(9,1);
				send_a_string("0");
				itoa(min,minutes,10);
				gotolocation(6,1);
				send_a_string(minutes);
			}
		}
		
		if(min==60)
			{
				
				hrs++;
				if(hrs<=9 && hrs<24)
				{
					min=0;
					gotolocation(6,1);
					send_a_string("0");
					itoa(hrs,hours,10);
					gotolocation(3,1);
					send_a_string("0");
					gotolocation(4,1);
					send_a_string(hours);
				}
				if(hrs>=10 && hrs<24)
				{
					min=0;
					gotolocation(6,1);
					send_a_string("0");
					itoa(hrs,hours,10);
					gotolocation(3,1);
					send_a_string(hours);
				}
				if(hrs>24)
				{
					hrs=0;
					gotolocation(3,1);
					send_a_string("00");
				}
			}
			if(bit_is_clear(PINC,5))
			{
				
				pressed_confidence++;
				if(pressed_confidence>10000)
				{
					if(button_pressed==0)
					{
						gotolocation(1,4);
						
						if(hrs>24)
						{
							hrs=0;
						}
						else
						{
							hrs++;
							if(hrs<=9 && hrs<24)
							{
								min=0;
								itoa(hrs,hours,10);
								gotolocation(3,1);
								send_a_string("0");
								gotolocation(4,1);
								send_a_string(hours);
							
							}
							if(hrs>=10 && hrs<24)
							{
								min=0;
								itoa(hrs,hours,10);
								gotolocation(3,1);
								send_a_string(hours);
								
							}	
													
						}
					}
					pressed_confidence=0;
				}
				else
				{
					released_confidence++;
					if(released_confidence>10000)
					{
						button_pressed=0;
						released_confidence=0;
					}
				}
				
			}	
			if(bit_is_clear(PINC,4))
			{
				
				pressed_confidence++;
				if(pressed_confidence>10000)
				{
					if(button_pressed==0)
					{
						button_pressed=1;
						gotolocation(1,4);
						
						if(min>60)
						{
							min=0;
						}
						else
						{
							min++;
							if(min<=9)
							{
								sec=0;
								itoa(min,minutes,10);
								gotolocation(6,1);
								send_a_string("0");
								gotolocation(7,1);
								send_a_string(minutes);
							
							}
							else
							{
								sec=0;
								itoa(min,minutes,10);
								gotolocation(6,1);
								send_a_string(minutes);
							
							}
							
						}
					}
					pressed_confidence=0;
				}
				else
				{
					released_confidence++;
					if(released_confidence>10000)
					{
						button_pressed=0;
						released_confidence=0;
					}
				}
				
			}
			if(bit_is_clear(PINC,3))
			{
				
				pressed_confidence++;
				if(pressed_confidence>10000)
				{
					if(button_pressed==0)
					{
						button_pressed=1;
						sec=0;
						min=0;hrs=0;
						initialize();
					}
					pressed_confidence=0;
				}
				else
				{
					released_confidence++;
					if(released_confidence>10000)
					{
						button_pressed=0;
						released_confidence=0;
					}
				}
				
			}
			
			
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
	LCD_data=0;
	
}

void send_a_string(char *name)
{
	while(*name>0)
	{
		send_a_character(*name ++);
	}
}
void gotolocation(uint8_t x,uint8_t y)
{
	send_a_command(0x80+firstcolumnposition[y-1]+(x-1));
}
void initialize()
{
	gotolocation(3,1);
	send_a_string("00");
	gotolocation(6,1);
	send_a_string("00");
	gotolocation(9,1);
	send_a_string("00");	
}
void Blink()
{
	gotolocation(5,1);
	send_a_string(":");
	gotolocation(8,1);
	send_a_string(":");
	_delay_ms(500);
	gotolocation(5,1);
	send_a_string(" ");
	gotolocation(8,1);
	send_a_string(" ");
}