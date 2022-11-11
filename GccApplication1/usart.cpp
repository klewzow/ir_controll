#include "usart.h"

void USART_ini(unsigned int speed)
{
	UBRRH = (unsigned char) (speed>>8);
	UBRRL = (unsigned char) (speed);
	
	
	UCSRB |= (1<<RXEN)|(1<<TXEN);						 // Включить прием и передачу USART
	UCSRB |= (1<<RXCIE);								 // Разрешаем прерывания при приеме (по окончанию)
	//UCSRA |= (1<<U2X);									 // Удвоение скорости бод
	UCSRC |= (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0); // Обращаемся именно к регистру UCSRC = 1,
	// Асинхронный режим UNSEL = 0
	// Не используем контроль четности UPM1=0, UPM0=0
	// 2 стоп  бита USBS = 1
	// 8-ми битная  посылка UCSZ1=1, UCSZ0=1
	
}
 
 void USART_transmit(unsigned char data) // передача байта
 {
	 while (!(UCSRA&(1<<UDRE)));
	 UDR =  data; 
 }
 
 void UART_Send_Str(char str[])
{
unsigned char i = 0;

while (str[i]!='\0')
{
USART_transmit(str[i]);
i++;
}
}