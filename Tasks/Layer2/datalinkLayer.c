#include <stdlib.h>

#include ".././Layer1/Layer1Physical.h"
#include "Layer2Datalink.h"
#include ".././Layer3/networkLayer.h"
#include ".././CRC32/CRC32.h"

int indexPreamble = 0;
int indexCRC = 1;
int indexPayloadSize = 5;
int indexPayloadData = 6;

int HeaderSize = 6;
uint8_t preambleData = 0b01111110;


/** implementation : sending in the datalink layer
 *
 * the physical layer accesses the send buffer from the function
 *
 */
void sendDatalink(uint8_t* data, uint8_t dataSize){
	
	int i = 0;
	if(sendFlag == 0){																		//send buffer is empty
	
		free(transmitBuffer);
		transmitBuffer = (uint8_t*)malloc((HeaderSize+dataSize)*sizeof(uint8_t));
	
		*(transmitBuffer+0)			=preambleData;	//1 byte
	       	*(transmitBuffer+indexCRC+0)		=0x00;		//1 byte
		*(transmitBuffer+indexCRC+1)		=0x00;		//1 byte
		*(transmitBuffer+indexCRC+2)		=0x00;		//1 byte
		*(transmitBuffer+indexCRC+3)		=0x00;		//1 byte
	
		*(transmitBuffer+indexPayloadSize)	=dataSize;	//1 byte				
	
		for(i=0; i< dataSize; i++)
			*(transmitBuffer+indexPayloadData+i)	=*(data+i);	
	
		struct structCRC CRCToTx;
		CRCToTx.CRCData[0] = 0x00;
		CRCToTx.CRCData[1] = 0x00;
		CRCToTx.CRCData[2] = 0x00;
		CRCToTx.CRCData[3] = 0x00;
	
		//Calculate the CRC using and pass all zero in CRC struct
		CRCToTx=CalculateCrc32((transmitBuffer+indexPayloadData), *(transmitBuffer+indexPayloadSize), CRCToTx); 
	
		//Copy the calculated CRC to send buffer
		*(transmitBuffer+indexCRC+0)		=CRCToTx.CRCData[0];		//1 byte
		*(transmitBuffer+indexCRC+1)		=CRCToTx.CRCData[1];		//1 byte
		*(transmitBuffer+indexCRC+2)		=CRCToTx.CRCData[2];		//1 byte
		*(transmitBuffer+indexCRC+3)		=CRCToTx.CRCData[3];		//1 byte
		
		sendFlag = 1; 		// setting flag 1, indicates Send buffer has data
		ResetTxByteCounter();	// Resets transmit byte counter in order to start transmission
		totalBytesToSend = (HeaderSize+*(transmitBuffer+indexPayloadSize)); //Calculate the size(in bytes) to be sent
	}
	else if(forwardFlag == 0)	//Indicates the Send buffer is busy and keeps the data in the temporary variable
	{
		free(temporaryTransmitBuffer);
		temporaryTransmitBuffer = (uint8_t*)malloc((HeaderSize+dataSize)*sizeof(uint8_t));
	
		*(temporaryTransmitBuffer+0)			=preambleData;	//1 byte
	       	*(temporaryTransmitBuffer+indexCRC+0)		=0x00;		//1 byte
		*(temporaryTransmitBuffer+indexCRC+1)		=0x00;		//1 byte
		*(temporaryTransmitBuffer+indexCRC+2)		=0x00;		//1 byte
		*(temporaryTransmitBuffer+indexCRC+3)		=0x00;		//1 byte
	
		*(temporaryTransmitBuffer+indexPayloadSize)	=dataSize;	//1 byte				
	
		for(i=0; i< dataSize; i++)
			*(temporaryTransmitBuffer+indexPayloadData+i)	=*(data+i);	
	
		struct structCRC CRCToTx;
		CRCToTx.CRCData[0] = 0x00;
		CRCToTx.CRCData[1] = 0x00;
		CRCToTx.CRCData[2] = 0x00;
		CRCToTx.CRCData[3] = 0x00;
	
		//Calculate the CRC using and pass all zero in CRC struct
		CRCToTx=CalculateCrc32((temporaryTransmitBuffer+indexPayloadData), *(temporaryTransmitBuffer+indexPayloadSize), CRCToTx);
	
		//Copy the calculated CRC to temporary transmit buffer
		*(temporaryTransmitBuffer+indexCRC+0)		=CRCToTx.CRCData[0];		//1 byte
		*(temporaryTransmitBuffer+indexCRC+1)		=CRCToTx.CRCData[1];		//1 byte
		*(temporaryTransmitBuffer+indexCRC+2)		=CRCToTx.CRCData[2];		//1 byte
		*(temporaryTransmitBuffer+indexCRC+3)		=CRCToTx.CRCData[3];		//1 byte

		// Setting the flag causes transmission when the send buffer becomes free
		forwardFlag = 1;
	}
}


/** Function : DataLinkRecieve
 \n Description : This function is used to construct the recieve buffer, the Physical layers recieves bit by bit and after preeabmle is matched, the byte is sent to datalink layers using this function
 \n
 \n Input -byteData
 \n will contain data in byte recieved
 \n
 \n
 \n Output - None
 */
void receiveDatalink(uint8_t byteData)
{
	int i;
	//Recieve untill the expected no of bytes are recieved
	if(byteCountRx < totalBytesToRecv)
	{
		if(byteCountRx == 0) // Check for preeamble 
		{
			*(recieveBuffer+byteCountRx) = byteData;
			if(*(recieveBuffer+byteCountRx) != preambleData)
			{
				InitializeRecvBuffer(); 
				byteCountRx=-1;
				PORTC = ~PORTC;
			}
		}
		else if(byteCountRx == 5) // Check for the size of the data to be recieved
		{
			*(recieveBuffer+byteCountRx) = byteData;
			totalBytesToRecv = (totalBytesToRecv + *(recieveBuffer+byteCountRx)); // Calculate total bytes of data to be recieved
			recieveBuffer = (uint8_t*)realloc(recieveBuffer, totalBytesToRecv*sizeof(uint8_t)); // Re allocate memory to the size it should recieve
			for(i=0;i<(totalBytesToRecv-HeaderSize);i++)
			{
				*(recieveBuffer+indexPayloadData+i)=0b00000000;
			}
		}
		else
		{
			*(recieveBuffer+byteCountRx) = byteData;
		}
	}	
	if(byteCountRx == totalBytesToRecv-1 && *(recieveBuffer+indexPayloadSize)>0)
	{
		struct structCRC CRCToRx;
		CRCToRx.CRCData[0] = *(recieveBuffer+indexCRC+0);
		CRCToRx.CRCData[1] = *(recieveBuffer+indexCRC+1);
		CRCToRx.CRCData[2] = *(recieveBuffer+indexCRC+2);
		CRCToRx.CRCData[3] = *(recieveBuffer+indexCRC+3);

		//Pass the recieved CRC to the CalculateCRC method
		//The output should the all Zero CRC, if no error has occured
		CRCToRx=CalculateCrc32((recieveBuffer+indexPayloadData), *(recieveBuffer+indexPayloadSize), CRCToRx);

		int isAllCRCZero=0;
		//Check for all CRC
		for(i=0; i<4; i++)
		{	
			
			if(CRCToRx.CRCData[i] == 0x00)
				isAllCRCZero=0;
			else
			{ 	
				isAllCRCZero=1;
				break;
			}
		}	 
		if(isAllCRCZero==0) //If no error in transmission then pass the data to network layer
		{
			NetworkRecieve((recieveBuffer+indexPayloadData),*(recieveBuffer+indexPayloadSize));
		}
		ResetRxByteCounter(); //Reset recieve counters in order to be ready to recieve future messages
		totalBytesToRecv=6;
		byteCountRx=-1;
	}
	byteCountRx++;
}

/** Function : InitializeRecvBuffer
 \n Description : This function is used to initialize the recieve buffer and is called during the initialization of the main program
 \n
 \n Input - None
 \n
 \n Output - None
 */
void clearReceiveBuffer()
{
	free(recieveBuffer);
	recieveBuffer = (uint8_t*)malloc((HeaderSize)*sizeof(uint8_t)); // Initially allocate the size of recieve buffer to the size of Payload overhead
	*(recieveBuffer+indexPreamble) = 0x00;		//1 byte
    *(recieveBuffer+indexCRC+0)	= 0x00;		//1 byte
	*(recieveBuffer+indexCRC+1)	= 0x00;		//1 byte
	*(recieveBuffer+indexCRC+2)	= 0x00;		//1 byte
	*(recieveBuffer+indexCRC+3)	= 0x00;		//1 byte
	*(recieveBuffer+indexPayloadSize) = 0x00;		//1 byte				
	totalBytesToRecv = (HeaderSize);
	byteCountRx = 0;
	
	resetRxByteCounter();
}

/** Function : ForwardToNextNode
 \n Description : This function is used to forward the complete recieved packet to next node.
 \n
 \n Input - None
 \n
 \n Output - None
 */
void forwardToNext()
{
	int i=0;
	if(sendFlag == 0)	//Indicates the Send buffer is empty	
	{
		free(transmitBuffer);
		transmitBuffer = (uint8_t*)malloc((HeaderSize+*(recieveBuffer+indexPayloadSize))*sizeof(uint8_t));
		for(i=0; i< (HeaderSize+*(recieveBuffer+indexPayloadSize)); i++)
		{
			uint8_t a= *(recieveBuffer+i);
			*(transmitBuffer+i)=a;
		}
		sendFlag = 1;
		ResetTxByteCounter();
		totalBytesToSend = (HeaderSize+*(recieveBuffer+indexPayloadSize));
	}
	else	//if send buffer is not free, copy data to temporary buffer
	{
		free(temporaryTransmitBuffer);
		temporaryTransmitBuffer = (uint8_t*)malloc((HeaderSize+*(recieveBuffer+indexPayloadSize))*sizeof(uint8_t));
		for(i=0; i< (HeaderSize+*(recieveBuffer+indexPayloadSize)); i++)
		{
			uint8_t a= *(recieveBuffer+i);
			*(temporaryTransmitBuffer+i)=a;
		}
		// Setting the flag causes transmission when the send buffer becomes free
		forwardFlag = 1; 
	}
	//Initialize recieve buffer to be able to recieve the future coming messages
	InitializeRecvBuffer(); 
	
}
