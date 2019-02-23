#include <stdlib.h>
#include "physicalLayer.h"
#include ".././Layer2/datalinkLayer.h"

// counter to keep no. of bits transmitted
int bitCountTx=0;

// counter to keep no. of bits recieved
int bitCountRx=0;

// counter to keep no. of bytes transmitted
int byteCountTx=0;

// counter to keep no. of bytes recieved
int byteCountRx=0;

int IsPreeambleMatched=0;

uint8_t tempByteBuffer=0x00;

uint8_t sendFlag=0;
uint8_t forwardFlag=0;

/** implementation : sending in the physical layer
 *
 * used to write data bit by bit on PORTB, available on "transmitBuffer" variable.
 *
 */

void send()
{
	uint8_t tempDataToSend  = 0b10000000;
	uint8_t tempDataToSendLow = 0b11111011;
	uint8_t tempDataToSendHigh = 0b00000100;
	int i;
	
	
	if(byteCountTx < totalBytesToSend)
	{
		if(bitCountTx < 8)
		{	
			bitCountTx++;
			tempDataToSend &= *(transmitBuffer+byteCountTx);

			//Send bit high or low based on value
			if(tempDataToSend == 0)
			{
				PORTB &= tempDataToSendLow;
			}
			else
			{
				PORTB |= tempDataToSendHigh;
			}

			*(transmitBuffer+byteCountTx)=(*(transmitBuffer+byteCountTx)<<1);
		}
		if(bitCountTx >= 8)
		{
			bitCountTx=0;
			byteCountTx++;
		}
	}
	//After all data is sent reset counter
	else if(byteCountTx >= totalBytesToSend)
	{
		ResetTxByteCounter();
		sendFlag = 0;
	}
}


/** implementation : receuving in the physical layer
 *
 * used to call the datalink layer as soon as the preamble is detected
 *
 */
void recieve()
{
	uint8_t tempDataToRecvMask = 0b00000001;
	
	if(bitCountRx < 8 || IsPreeambleMatched == 0)	
	{ 
		tempDataToRecvMask &= (PIND>>2);

		if(IsPreeambleMatched == 1) // Set flag once preemble is matched
		{
			if(bitCountRx <= 7)
				tempByteBuffer = (tempByteBuffer<<1);
		}
		else //Ignore all bits untill the preeamble is matched 
		{
			tempByteBuffer = (tempByteBuffer<<1);
		}

		tempByteBuffer |= (tempDataToRecvMask);

		if((IsPreeambleMatched == 0) &&(tempByteBuffer == preambleData))
		{
			IsPreeambleMatched = 1;
		}
		bitCountRx++;
	}
	if(bitCountRx >= 8 && IsPreeambleMatched == 1)
	{
		//Pass recieved byte to Datalink Layer once 8 bits are recieved after preemable matched
		DataLinkRecieve(tempByteBuffer);
		tempByteBuffer = 0x00;			
		bitCountRx=0;
	}
}

/** implementation : to reset the transmitting byte counter
 *
 * used to reset transmit counters and copy data from temporary buffer to send buffer
 *
 */
void resetTxByteCounter()
{
	int i=0;
	totalBytesToSend=0;
	if(forwardFlag == 1)
	{
		free(transmitBuffer);
		transmitBuffer = (uint8_t*)malloc((HeaderSize+*(temporaryTransmitBuffer+indexPayloadSize))*sizeof(uint8_t));
		for(i=0; i< (HeaderSize+*(temporaryTransmitBuffer+indexPayloadSize)); i++)
		{
			uint8_t a= *(temporaryTransmitBuffer+i);
			*(temporaryTransmitBuffer+i)=a;
		}
		totalBytesToSend = (HeaderSize+*(temporaryTransmitBuffer+indexPayloadSize));
		sendFlag = 1;
		forwardFlag = 0;
	}
	bitCountTx=0;
	byteCountTx=0;
}


/** implementation : to reset the receiving byte counter
 *
 * used to reset receive counters
 *
 */
void resetRxByteCounter()
{
	bitCountRx=0;
	IsPreeambleMatched = 0;
}

