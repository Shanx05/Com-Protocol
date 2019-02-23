#include <avr/io.h> 
#include <stdlib.h> 
#include "networkLayer.h"


uint8_t myPiAddr = 17;


/** implementation: sending in the network layer
 * the network layer packet contains source and destination address along with data to be sent
 *
 * parameters:
 * payload: Contains the data to be sent
 * dataSize: Contains the size of data to be sent (in bytes)
 * destination: Specifies the destination address
 * source: Specifies the source address
 *
 */
void sendNetworkLayer(uint8_t* payload, uint8_t dataSize, uint8_t destination, uint8_t source){

	int i = 0;
	uint8_t initialStuffing = 2;
	uint8_t* payloadToDatalinkLayer = (uint8_t*)malloc((initialStuffing+dataSize)*sizeof(uint8_t));
	*(payloadToDatalinkLayer+0) = destination;
	*(payloadToDatalinkLayer+1) = source;

	for(i = 0; i < dataSize; i++){
		*(payloadToDatalinkLayer+initialStuffing+i)	= *(payload+i);
	}

	outputString("The data in the Network Layer: "); 
	for(i = 0; i < dataSize; i++)
		outputCharacter(*(payload+i));
	outputString(" is transmitted\r\n"); 

	sendDatalink(payloadToDatalinkLayer, dataSize+initialStuffing); 
}


/** implementation: receiving in network layer
 * receive the network layer packet and either send to the transport layer or forward to next node
 * 
 * if the packet is from a malicious source, the packet must be discarded
 * 
 * parameters:
 * payload: Contains the data being recieved
 * dataSize: Contains the size of data recieved in bytes
 *
 */
void receiveNetworkLayer(uint8_t* payload, uint8_t dataSize)
{
	int i = 0;
	uint8_t destinaton 	= *(payload+0);
	uint8_t source 		= *(payload+1);
	if(dataSize >= 2){
		if(destinaton == 0){ 														//to broadcast
			if(source != myPiAddr){ 												//I'm not the sender and receiver, then forward
				outputString("The data in the Network Layer: ");
				
				for(i = 2; i < dataSize; i++)
					outputCharacter(*(payload+i));
					
				outputString("\t recieved and \t broadcasted\r\n");   				
				forwardToNext();
			}
			else if(source == myPiAddr){ 											//broadcasted message and I'm the sender
				outputString("The data in the Network Layer: ");  

				for(i = 2; i < dataSize; i++)
					outputCharacter(*(payload+i));
				
				outputString(". The broadcast is complete\r\n"); 
				clearReceiveBuffer(); 
			}
		}
		else if(destinaton == myPiAddr){ 											//I'm the destination
			if(source != myPiAddr){													//I'm not the sender
				outputString("The packet is sent to myPiAddr: ");					//packet is sent to me
				
				for(i = 2; i < dataSize; i++)
					outputCharacter(*(payload+i));
 
				outputString("\r\n");
				clearReceiveBuffer(); 
			}
		}
		else if(destinaton != myPiAddr){ 											//I'm not the destination
			if(source != myPiAddr){													//I'm not the sender
				outputString("The data in the Network Layer: ");

				for(i = 2; i < dataSize; i++)
					outputCharacter(*(payload+i));

				outputString(" is forwarded\r\n"); 
				forwardToNext();
			}
		else if(source == myPiAddr){ 												//I'm the sender
				outputString("The data in the Network Layer: ");

				for(i = 2; i < dataSize; i++)
					outputCharacter(*(payload+i));

				outputString(" is sent by a malicious node\r\n"); 
			}
		}
	}
}
