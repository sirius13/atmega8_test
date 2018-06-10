#include "global.h"
#include <stdint.h>
#include <avr/io.h>

void spi_init(void);  //настройка SPI
void spi_send_buffer(uint8_t *p, uint16_t size); //неблокирующа€ отправка данных, отправл€ем первый байт, остальные отправл€ютс€ обработчиком прерывани€ по завершению передачи 

