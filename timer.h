#include "global.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

void tim_3x_start(void); 	//формирование тактовых сигналов 1 Гц, 10 Гц и 100 Гц на выводах PCO, PC1 и PC2
void tim_pwm_start(void);	//режим ШИМ генератора
void timer_stop(void);		//останавливаем генератор

