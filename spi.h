#include "global.h"
#include <stdint.h>
#include <avr/io.h>

void spi_init(void);  //��������� SPI
void spi_send_buffer(uint8_t *p, uint16_t size); //������������� �������� ������, ���������� ������ ����, ��������� ������������ ������������ ���������� �� ���������� �������� 

