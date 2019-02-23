#include <stdlib.h>
#include <avr/io.h> 

#include <util/delay.h>

#include <string.h>

#include ".././Layer1/physicalLayer.h"
#include ".././Layer2/datalinkLayer.h"
#include ".././Layer3/networkLayer.h"
#include ".././UART/UART.h"
#include ".././IRQ/IRQ.h"

void initLed();

/** extern variable : myPiAddr references the variable from the network layer
 *
 * the address of the RaspberryPi
 *
 */
extern uint8_t myPiAddr;

/** implementation : main
 *
 * execution of main
 *
 */
int main(void)
{
	int i = 0;
	char dataToSend[] =  "Eden Hazard"; 							//message to be sent
	uint8_t destination_address = 0;								//address of the destination node

	initLed(); 	
	initializeUART();	
	initTimer(); 

	clearReceiveBuffer();
    
	//send data to the specified node
	sendNetworkLayer(dataToSend, strlen(dataToSend), destination_address,MyAddress);
	
	while(1)
	{
		
	}
}

/** implementation : to initialize the LED
 */
void initLed()
{
    DDRB |= _BV(1)|_BV(2);
	DDRC |= 0xFF;
	PORTC = 0b00000010;
}
