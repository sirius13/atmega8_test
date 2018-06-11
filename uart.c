#include "uart.h"

void usart_init(void)
{
	UCSRB= (1<<RXCIE)|(1<<RXEN)|(1<<TXEN); ; 									//разрешить прием, передачу и прерывание по приему байта
	UBRRH=0;
	UBRRL=51;																	//Baud rate = 9600
}

void send_string(uint8_t* bptr) 												//функция отправки данных через терминал
{
		
	while(*bptr!='\0')
		{
			UDR=*bptr;
			while(!(UCSRA&(1<<UDRE))); 										//ожидаем окончание передачи байта
			bptr++;
		}
}

uint8_t command_processing(uint8_t* inbuf)
{
	uint8_t	osc1[]="oscmode1";
	uint8_t osc2[]="oscmode2";
	uint8_t oscstop[]="oscstop";
	uint8_t pb[]="portb";
	uint8_t pc[]="portc";
	uint8_t spibyte[]="spibyte";
	uint8_t spibuff[]="spibuff";
	uint8_t answer[]="SPI answer: ";
	uint8_t rn[]="\r\n";
	uint8_t spitx[]="A change is as good as a rest...";
	
	uint8_t i,pin,status=0;
		
	if(memcmp(inbuf,osc1,8)==0) 												//включить генератор 1/10/100 Гц
						{
							timer_stop();
							tim_3x_start(); 
							status=1;
						}
								
	if(memcmp(inbuf,osc2,8)==0) 												//включить ШИМ-генератор
						{
							timer_stop();
							tim_pwm_start(); 
							status=1;
						}
	
	if(memcmp(inbuf,oscstop,7)==0)												//остановить генератор
						{		
							timer_stop(); 
							status=1;
						}
								
	if(memcmp(inbuf,pb,5)==0)													//если пришла команда управления выводами порта B
						{
							pin=strtoul((char*)(inbuf+5),0,10); 				//находим номер вывода МК
							i=0;
							
							while(inbuf[i+1]!='\0') i++;
							
							if (inbuf[i]=='h')
 
											{
												PORTB|=(1<<pin);
												status=1;
											}
							
							if (inbuf[i]=='l')
											{
												PORTB&=~(1<<pin);
												status=1;
											}
						}
	
	if(memcmp(inbuf,pc,5)==0)													//если пришла команда управления выводами порта C
						{
							pin=strtoul((char*)(inbuf+5),0,10);				//находим номер вывода МК
														
							i=0;
							while(inbuf[i+1]!='\0') i++;
																					
							if (inbuf[i]=='h')
												{
													PORTC|=(1<<pin);
													status=1;
												}
							if (inbuf[i]=='l')
												{
													PORTC&=~(1<<pin);
													status=1;
												}
							
							status=1;
						}
						
	if(memcmp(inbuf,spibyte,7)==0) 											//команда посылки байта по SPI
									{										
										SPDR=strtoul((char*)(inbuf+7),0,16); 	//для получения байта преобразуем ASCII в hex
										while(!(SPSR & (1<<SPIF))); 			//ожидаем окончание передачи байта
										send_string(answer);					//возвращаем ответ от slave
										UDR=SPDR;
										while(!(UCSRA&(1<<UDRE))); 			
										send_string(rn);
										status=1;
									}
	
	if(memcmp(inbuf,spibuff,7)==0) 											//команда посылки буфера по SPI
									{
										spi_send_buffer(spitx,31); 
										status=1;
									}
	
	
	return status;
}
