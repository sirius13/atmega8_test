#include "global.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

void tim_3x_start(void); 	//������������ �������� �������� 1 ��, 10 �� � 100 �� �� ������� PCO, PC1 � PC2
void tim_pwm_start(void);	//����� ��� ����������
void timer_stop(void);		//������������� ���������

