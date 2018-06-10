#include "main.h"
#include "global.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "spi.h"
#include "uart.h"
#include "timer.h"
#include "string.h"

#define F_CPU 8000000UL

uint8_t start[]="Started...\r\n";

uint8_t input_buffer[20]={0};

ISR(SPI_STC_vect)
{
	gPointer++;
	gCounter--;	
	if (gCounter==0) SPCR^=(1<<SPIE); //��������� ���������� �� ���������
	SPDR=*(gPointer);
}

ISR(TIMER1_COMPA_vect)	//������������ �������� �������� 1 ��, 10 �� � 100 �� �� ������� PCO, PC1 � PC2 
{
	
	static uint8_t div10=0;		//�������� �� 10 ��� ��������� 100 ��
	static uint8_t div100=0;	//�������� �� 100 ��� ��������� 10 ��
	static uint16_t div1000=0;	//�������� �� 1000 ��� ��������� 1 ��
	
	TCNT1H=0;
	TCNT1L=0;
		
	div1000++;	
	if (div1000==1000) 
		{	
			PORTC^=(1<<PC0);
			div1000=0;
		}
	
	div100++;	
	if (div100==100) 
		{	
			PORTC^=(1<<PC1);
			div100=0;
		}
	
	div10++;
	if (div10==10) 
		{	
			PORTC^=(1<<PC2);
			div10=0;
		}
}

ISR(USART_RXC_vect)
{
	uint8_t rxbyte;
	static uint8_t ptr=0;
	
	uint8_t OK[]="OK\r\n";
	uint8_t Err[]="Error data\r\n";
	
	rxbyte = UDR;
	
	switch (rxbyte)
	
	{		
		case 0x0D: 	//Enter - ������� ��������� ����� �������
				{ 	
					if(command_processing(input_buffer)) send_string(OK);
					else send_string(Err);
					memset(input_buffer,0,20);
					ptr=0;
					break;
				}
				
		case '+': //��������� ���������� ��� (����������� ����������)
				{
					 OCR1A+=10;
					 break;
				}
				
		case '-': //����������� ���������� ��� (��������� ����������)
				{
					 OCR1A-=10;
					 break;
				}
		
		case 0x20: break; //���������� �������
				
				
		default: //��������� ����� ������� ������
				{
					input_buffer[ptr]=rxbyte;
					ptr++;
				}
	}
	
}


int main (void)
{
	
	DDRC=0xFF;
	DDRB|=(1<<PB7)|(1<<PB6)|(1<<PB5)|(1<<PB3)|(1<<PB2)|(1<<PB1)|(1<<PB0);
	
	usart_init();
	send_string(start);

	SREG=(1<<7); //���������� ���������� ����������
	
	spi_init();

	while (1) 
		{
			
		}
}
