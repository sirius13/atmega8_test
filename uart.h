#include "global.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "timer.h"
#include "spi.h"
#include <stdlib.h>

uint8_t input_buffer[20];
uint8_t counter;

void usart_init(void);
void send_string(uint8_t* bptr); //функция отправки данных через терминал
uint8_t command_processing(uint8_t* input_buffer);