#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL


volatile char rxbyte;

char cnt;
char input_buffer[64]={0};

//����� ������ �� ��������				
char SPI_TXbuf[64]={0x00,0x01,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0d,0x0D,0x0E,0x0F,
					0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
					0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
					0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F}; 

volatile char spi_tx_count=0;
char spi_buf_size=64;


volatile char flag;
volatile unsigned char ptr;
volatile char flag1=0;

short div1000=0;
char div10=0,div100=0;

void usart_init(void);						//��������� UART
void send_string(char* bptr, char size);	//������� ������
void command_processing(char* b1ptr);		//���������� ��������� ���������

void spi_init(void);						//��������� SPI
void spi_send_buffer(void);					//������������� �������� ������ SPI_TXbuf[64]
char spi_send_byte(char byte);				//�������� �����

void tim_3x_start(void); 	//������������ �������� �������� 1 ��, 10 �� � 100 �� �� ������� PCO, PC1 � PC2
void tim_pwm_start(void);	//����� ��� ����������
void timer_stop(void);		//������������� ���������


