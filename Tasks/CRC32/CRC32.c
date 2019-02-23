#include "CRC32.h"
#include <string.h>

//! Polynomial of CRC calculation which will be predefined
char crcPolynomial[] = "111011011011100010000011001000001";
		

/** Function : CalculateCrc32
 \n Description : Calculates and Returns 32 bit CRC using a predifined polynomial.
 \n
 \n Input-
 \n data     : Value on which CRC should be calculated.
 \n dataSize : specifies byte length of data.
 \n CRC      : Contains the CRC which will be padded along with data while calculating CRC
 \n
 \n Output-
 \n \t Returns structCRC datastructure.
*/
struct structCRC CalculateCrc32(uint8_t* data, uint8_t dataSize, struct structCRC CRC)
{

	/*
		This method converts bits to char, compares, performs calculation and returns by converting char to buts
	*/


	int iPolynomialLength = strlen(crcPolynomial);
	int iBufferCRCSize= 4;
	int i=0;
	int j=0;
	int k=0;
	
	if(iBufferCRCSize*8 == iPolynomialLength-1)
	{
		int iPayloadlength = dataSize * 8;
		char payloadWithPadding[iPayloadlength + iPolynomialLength -1];
		uint8_t mask= 0b10000000;

		//Bit to char of Payload data
		k=iPayloadlength + iPolynomialLength -1-1;
		for(i=0; i<dataSize; i++)
		{
			for(j=0; j<8; j++)
			{
				uint8_t temp= (mask & *(data+i));
				if(temp == 0x00)
					payloadWithPadding[k--]= '0';
				else
					payloadWithPadding[k--]= '1';

				mask = (mask>>1);
			}
			mask= 0b10000000;
		}
		mask= 0b10000000;

		//Bit to char of CRC data
		for(i=0; i<iBufferCRCSize; i++)
		{
			for(j=0; j<8; j++)
			{
				uint8_t temp= (mask & CRC.CRCData[i]);
				if(temp == 0x00)
					payloadWithPadding[k--]= '0';
				else
					payloadWithPadding[k--]= '1';

				mask = (mask>>1);
			}
			mask= 0b10000000;
		}

		//CRC calculation 
		int iCountBeginOf1= iPayloadlength + iPolynomialLength -1 -1;
		while(iCountBeginOf1 > iPolynomialLength -1 -1)
		{
			if(payloadWithPadding[iCountBeginOf1] == '0')
			{
				iCountBeginOf1--;
			}
			else
			{
				int iPayload1StartIndex=iCountBeginOf1;
				for(i=iPolynomialLength -1; i>=0; i--)
				{
					// XOR
					if(payloadWithPadding[iPayload1StartIndex] == crcPolynomial[i])
					{
						payloadWithPadding[iPayload1StartIndex--]= '0';
					}
					else 
						payloadWithPadding[iPayload1StartIndex--]= '1';
				}
				iCountBeginOf1= iPayloadlength + iPolynomialLength -1 -1;
			}
		}
		
		//Convert char to bit
		k=iBufferCRCSize*8-1;
		for(i=0; i<iBufferCRCSize; i++)
		{	
			CRC.CRCData[i] = 0b00000000;
			for(j=7; j>=0; j--)
			{
				if(payloadWithPadding[k--] == '1')
				{
					CRC.CRCData[i]|=(1<<j);
				}
			}
		}
	}
	return CRC;
}
