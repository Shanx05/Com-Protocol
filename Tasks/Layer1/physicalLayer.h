#include <avr/io.h> 

//read the data
void recieve();

//writes the data
void send();

//resetting the transmit counter
void resetTxByteCounter();

//resetting the receive counter
void resetRxByteCounter();

//sending data variable
uint8_t *transmitBuffer;		

//receiving data variable
uint8_t *recieveBuffer;	

//variable to store the data when the buffer is busy in transmitting
uint8_t *temporaryTransmitBuffer;	


int totalBytesToSend;
int totalBytesToRecv;

int byteCountRx;

uint8_t sendFlag;
uint8_t forwardFlag;


