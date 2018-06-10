#include "timer.h"

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
