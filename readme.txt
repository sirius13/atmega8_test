IDE: WinAVR
Команды для работы с терминалом

Символы вводятся в режиме ACSII, завершается нажатием клавиши Enter

1. Управление выводами порта. Доступны следующие выводы МК:

PORTB - пин 0, 1, 2, 6, 7
PORTC - пин 0-7

формат команды управления состоянием выводов: port x y z
x: b или c
y: номер вывода
z: l или h

пример: port c 6 l - выставить низкий уровень на выводе PC6
	port b 2 h - выставить высокий уровень на выводе PB2

2. Управление генераторами

osc mode1 - включение генератора 1, 10, 100 Гц, задействованы выводы PC0, PC1, PC2 соответственно
osc mode2 - включение ШИМ-генератора, управление скважностью символами + или -
osc stop - остановка генератора

3. Отправка байта данных по SPI 

spi byte 0x**

пример: spi byte 0xAF
	spi byte 0x56

4. Неблокирующая отправка буфера данных по SPI

spi buff
