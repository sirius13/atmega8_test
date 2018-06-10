#include "timer.h"

void tim_3x_start(void) //������� ������� ���������� �� 3 ������, �������� �� ������� 1
{
	TCCR1B|=(1<<CS11); //�������� ������� ������� 8���/8=1 ���
	OCR1AH = 0x01;		//������ �������� 1000 � ������� ���������
	OCR1AL = 0xF4; 
	TIMSK|= (1<<OCIE1A); //���������� ���������� �� ���������
}

void tim_pwm_start(void) //������� ������� ����
{
	TCCR1B|=(1<<CS11);
	TCCR1A|=(1<<COM1A1)|(1<<WGM10); //�� ������ OC1A (PB1) ������� �������, ���� TCNT1==OCR1A, ������������ ���
	OCR1A=0x0f;
}

void timer_stop(void) //��������� �����������
{
	TCCR1B=0x00; 
	TCCR1A=0x00;
	TIMSK=0x00; 
	OCR1AH = 0x00;
	OCR1AL = 0x00;
}
