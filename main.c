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
	if (gCounter==0) SPCR^=(1<<SPIE); //отключаем прерывание по завешению
	SPDR=*(gPointer);
}

ISR(TIMER1_COMPA_vect)	//формирование тактовых сигналов 1 Гц, 10 Гц и 100 Гц на выводах PCO, PC1 и PC2 
{
	
	static uint8_t div10=0;		//делитель на 10 для получения 100 Гц
	static uint8_t div100=0;	//делитель на 100 для получения 10 Гц
	static uint16_t div1000=0;	//делитель на 1000 для получения 1 Гц
	
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
		case 0x0D: 	//Enter - признак окончания ввода команды
				{ 	
					if(command_processing(input_buffer)) send_string(OK);
					else send_string(Err);
					memset(input_buffer,0,20);
					ptr=0;
					break;
				}
				
		case '+': //уменьшаем скважность ШИМ (увеличиваем заполнение)
				{
					 OCR1A+=10;
					 break;
				}
				
		case '-': //увеличиваем скважность ШИМ (уменьшаем заполнение)
				{
					 OCR1A-=10;
					 break;
				}
		
		case 0x20: break; //пропускаем пробелы
				
				
		default: //заполняем буфер входных данных
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

	SREG=(1<<7); //глобальное разрешение прерываний
	
	spi_init();

	while (1) 
		{
			
		}
}
