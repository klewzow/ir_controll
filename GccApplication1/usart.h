#include "mine.h" 
#ifndef USART_H_
#define USART_H_
void USART_ini(unsigned int speed);
void USART_transmit(unsigned char data);
void UART_Send_Str(char str[]);
#endif  