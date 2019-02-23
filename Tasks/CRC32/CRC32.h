#include <avr/io.h> 


/** struct : structCRC
 \n Description : Contains array of size 4 of type uint8_t
 \n This struct is used to access and return the CRC values
 */
struct structCRC
{
	uint8_t CRCData[4];    	// Size = 4 byte
};


struct structCRC  CalculateCrc32(uint8_t* data, uint8_t dataSize, struct structCRC CRC);
