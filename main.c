#include "main.h"

void usart_init(void)
{
	UCSRB= (1<<RXCIE)|(1<<RXEN)|(1<<TXEN); ; //разрешить прием, передачу и прерывание по приему байта
	UBRRH=0;
	UBRRL=51;	//Baud rate = 9600
}


void command_processing(char* b1ptr) //функция обработки команд, поступающих через терминал
{

	char ch=0,ch8=0,ch9=0;
	
	
	
	if ((b1ptr[0]=='p')&&(b1ptr[1]=='o')&&(b1ptr[2]=='r')&&(b1ptr[3]=='t')) //обработка команд работы с выводами портов
	
	{
		
		if (b1ptr[5]=='b')
							{
								if (b1ptr[9]&0x0F)
									{
										PORTB|=(1<<(b1ptr[7]&0x0F));
										send_string("OK\r\n",4);
									}
									
								else 
									{
										PORTB&=~(1<<(b1ptr[7]&0x0F));
										send_string("OK\r\n",4);
									}
							}
		
		
		if (b1ptr[5]=='c')
							{
								if (b1ptr[9]&0x0F)
									{
										PORTC|=(1<<(b1ptr[7]&0x0F));
										send_string("OK\r\n",4);
									}
									
								else 
									{
										PORTC&=~(1<<(b1ptr[7]&0x0F));
										send_string("OK\r\n",4);
									}
							}
	}
	
	if ((b1ptr[0]=='t')&&(b1ptr[1]=='i')&&(b1ptr[2]=='m')&&(b1ptr[3]=='e')&&(b1ptr[4]=='r')) //обработка команд включения генераторов
	
	{
		if ((b1ptr[6]=='m')&&(b1ptr[7]=='o')&&(b1ptr[8]=='d')&&(b1ptr[9]=='e')&&(b1ptr[10]=='1'))
			{
				
				send_string("3-out generator started, pins PC0-1Hz, PC1-10Hz, PC2-100Hz\r\n", 59);
				timer_stop();
				tim_3x_start();
			}
		
		if ((b1ptr[6]=='m')&&(b1ptr[7]=='o')&&(b1ptr[8]=='d')&&(b1ptr[9]=='e')&&(b1ptr[10]=='2'))
			
			{
				
				send_string("PWM started, pin PB1, use + or - for duty ratio adjust\r\n",56);
				timer_stop();
				tim_pwm_start();				
			}
		
		if ((b1ptr[6]=='s')&&(b1ptr[7]=='t')&&(b1ptr[8]=='o')&&(b1ptr[9]=='p'))
			{
				send_string("Generator stopped\r\n",19);
				timer_stop();
			}
	}
	
	if ((b1ptr[0]=='s')&&(b1ptr[1]=='p')&&(b1ptr[2]=='i')) //обработка команд отправки данных по SPI
	
	{
		if (b1ptr[4]=='s') 
				
				if ((b1ptr[6]=='0')&&(b1ptr[7]=='x')) //если требуется отправка байта
				{
					
					if ((b1ptr[8]=='a')||(b1ptr[8]=='b')||(b1ptr[8]=='c')||(b1ptr[8]=='d')
					    ||(b1ptr[8]=='e')||(b1ptr[8]=='f'))
					ch8=(b1ptr[8]-0x57);
					
					if ((b1ptr[8]=='0')||(b1ptr[8]=='1')||(b1ptr[8]=='2')||(b1ptr[8]=='3')||(b1ptr[8]=='4')
					    ||(b1ptr[8]=='5')||(b1ptr[8]=='6')||(b1ptr[8]=='7')||(b1ptr[8]=='8')||(b1ptr[8]=='9'))
					ch8=(b1ptr[8]-0x30);
					
					
					if ((b1ptr[9]=='a')||(b1ptr[9]=='b')||(b1ptr[9]=='c')||(b1ptr[9]=='d')
					    ||(b1ptr[9]=='e')||(b1ptr[9]=='f'))
					ch9=(b1ptr[9]-0x57);
					
					if ((b1ptr[9]=='0')||(b1ptr[9]=='1')||(b1ptr[9]=='2')||(b1ptr[9]=='3')||(b1ptr[9]=='4')
					    ||(b1ptr[9]=='5')||(b1ptr[9]=='6')||(b1ptr[9]=='7')||(b1ptr[9]=='8')||(b1ptr[9]=='9'))
					ch9=(b1ptr[9]-0x30);
					
					ch=spi_send_byte((ch8<<4)|ch9);
					send_string("SPI: byte sended\r\n",18);
					send_string("SPI: device answer ",19);
					UDR=ch;
					_delay_ms(5);
					send_string("\r\n",2);
				}
				
				if ((b1ptr[6]=='b')&&(b1ptr[7]=='u')&&(b1ptr[8]=='f')) //если требуется отправка буфера
					{
						spi_send_buffer();
					}
		
		
	}
	
	
}

void send_string(char* bptr, char size) //функция отправки данных через терминал
{
	char cnt;
	
	for(cnt=0;cnt<size;cnt++)
		{
			UDR=*bptr;
			_delay_ms(5);
			bptr++;
		}
}

void tim_3x_start(void) //функция запуска генератора на 3 вывода, основана на таймере 1
{
	TCCR1B|=(1<<CS11); //тактовая частота таймера 8Мгц/8=1 Мгц
	OCR1AH = 0x01;		//запись значения 1000 в регистр сравнения
	OCR1AL = 0xF4; 
	TIMSK|= (1<<OCIE1A); //разрешение прерывания по сравнению
}

void tim_pwm_start(void) //Функция запуска ШИМа
{
	TCCR1B|=(1<<CS11);
	TCCR1A|=(1<<COM1A1)|(1<<WGM10); //на выводе OC1A (PB1) высокий уровень, если TCNT1==OCR1A, восьмибитный ШИМ
	OCR1A=0x0f;
}

void timer_stop(void) //остановка генераторов
{
	TCCR1B=0x00; 
	TCCR1A=0x00;
	TIMSK=0x00; 
	OCR1AH = 0x00;
	OCR1AL = 0x00;
}

void spi_init(void)  //настройка SPI
{
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0); //включение SPI, режим мастер, тактовая частота SPI=8000000/18=500 КГц
}


void spi_send_buffer(void) //неблокирующая отправка данных, отправляем первый байт,
			   //остальные данные отправляются обработчиком прерывания по завершению передачи 
{
	spi_tx_count=0;
	SPCR|=(1<<SPIE); //разрешение прерывания по завершению передачи байта
	SPDR = *SPI_TXbuf;
}

char spi_send_byte(char byte)
{
	char ch;
	SPDR=byte;
	while(!(SPSR & (1<<SPIF))); 	//ожидаем появление флага окончания передачи байта
	ch=SPDR;
	return ch;						//возвращаем байт ответа от slave
}

int main (void)
{
	
	DDRC=0xFF;
	DDRB|=(1<<PB7)|(1<<PB6)|(1<<PB5)|(1<<PB3)|(1<<PB2)|(1<<PB1)|(1<<PB0);
	
	usart_init();
	send_string("Program started\r\n",17);
	
	spi_init();
		
	SREG=(1<<7); //глобальное разрешение прерываний
	flag=0;
	
	ptr=0;
		
	while (1) 
		{
			
		}

}

ISR(USART_RXC_vect)		//прерывание UART по приему байта 
{
	rxbyte = UDR;
	
	switch (rxbyte)
	
	{		
		case 0x0D: 	//Enter - признак окончания ввода команды
				{ 	
					flag=1;
					ptr=0 ;
					command_processing(input_buffer); //передаем данные в функцию обработки сообщений терминала
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
				
		
		default: //заполняем буфер входных данных
				{
					input_buffer[ptr]=rxbyte;
					ptr++;
				}
	}
	
}


ISR(TIMER1_COMPA_vect)	//формирование тактовых сигналов 1 Гц, 10 Гц и 100 Гц на выводах PCO, PC1 и PC2 
{
	TCNT1H=0;
	TCNT1L=0;
		
	div1000++;	//делитель на 1000 для получения 1 Гц
	if (div1000==1000) 
		{	
			PORTC^=(1<<PC0);
			div1000=0;
		}
	
	div100++;	//делитель на 100 для получения 10 Гц
	if (div100==100) 
		{	
			PORTC^=(1<<PC1);
			div100=0;
		}
	
	div10++;
	if (div10==10) //делитель на 10 для получения 100 Гц
		{	
			PORTC^=(1<<PC2);
			div10=0;
		}
}

ISR(SPI_STC_vect)
{
	spi_tx_count++;
	SPDR=*(SPI_TXbuf+spi_tx_count);
	while(!(SPSR & (1<<SPIF))); 	    //ждем окончания передачи байта
	if (spi_tx_count==(spi_buf_size-1)) //если завершили отправку буфера
		{
			send_string("SPI: buffer sended\r\n",20);
			SPCR^=(1<<SPIE); //отключаем прерывание по завешению
		}
}
