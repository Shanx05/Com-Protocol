#include <stdlib.h>
#include <avr/io.h> 
#define BAUD 9600
#include <util/setbaud.h>
#include "UART.h"

/** implementation: initializing the UART
 *
 * initializing the UART registers
 *
 */
void initializeUART()
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

    UCSR0C |= 	(1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B |=  	(1<<TXEN0);
}


/** implementation: to output a character on the minicom
 *
 * to write data on serial communication (minicom)
 *
 */
void outputCharacter(char data)
{
        while(!(UCSR0A & (1<<UDRE0))){};
        UDR0=data;
}


/** implementation: to output a string on the minicom
 *
 * to write data on serial communication (minicom)
 *
 */
void outputString(char *data)
{
	if(*data == '\0')
	{
		return;
	}
        while(!(UCSR0A & (1<<UDRE0))){};
        UDR0=*data;
	outputString(++data);
}
