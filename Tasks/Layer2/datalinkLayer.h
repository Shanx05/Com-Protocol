#include <avr/io.h> 

//forwards the recieved data to next node
void forwardToNext();

//clears the recieve buffer
void clearReceiveBuffer();

//to receive data from physical layer
void receiveDatalink(uint8_t byteData);

//to receive data from network layer
void sendDatalink(uint8_t* data, uint8_t dataSize);

uint8_t preambleData;

int initialFrame;

int indexPayloadSize;

