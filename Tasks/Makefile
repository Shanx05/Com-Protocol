#Steps:
#1. Compile the code file
#2. Link the code file to .elf which will be used to generate the .hex file
#3. Copy the .elf file data and generate the .hex file which will be used for flashing on the board
#4. Flash the .hex file on board


FileMain 		= Main/main
FileCRC			= CRC32/CRC32
FileUART		= UART/UART
FileIRQ			= IRQ/IRQ
FileLayer1		= Layer1/physicalLayer
FileLayer2		= Layer2/datalinkLayer
FileLayer3		= Layer3/networkLayer

FileConfigDoxygen		= DoxyfileConfigFile



#build : compiles the code
build : 
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000L -c $(FileName).c -o $(FileName).o



#link : links the .o to .elf which is used to generate .hex file to flash
link:
	avr-gcc -mmcu=atmega328p -DF_CPU=12000000L -o $(FileName).elf $(FileName).o



#copy: copys the data of elf into hex file which is used to flash
copy :
	avr-objcopy -O ihex -R .eeprom $(FileName).elf $(FileName).hex




#flash: flashes the hex file data on the atmega via SPI
flash : 
	avrdude -c gpio -p atmega328p -U flash:w:$(FileName).hex



#Clean: cleans .hex .o .elf files
clean : 
	rm $(FileMain).elf $(FileMain).hex $(FileMain).o $(FileLayer1).o $(FileLayer2).o $(FileLayer3).o $(FileCRC).o $(FileUART).o $(FileIRQ).
	rm -rf ../Documentation/html ../Documentation/latex ../Documentation/man ../Documentation/rtf
		

#all: does all steps 1-4
all:
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileMain).c -o $(FileMain).o
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileLayer1).c -o $(FileLayer1).o
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileLayer2).c -o $(FileLayer2).o
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileLayer3).c -o $(FileLayer3).o
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileCRC).c -o $(FileCRC).o
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileUART).c -o $(FileUART).o
	avr-gcc -g -Os -mmcu=atmega328p -DF_CPU=12000000 -c $(FileIRQ).c -o $(FileIRQ).o
	avr-gcc -mmcu=atmega328p -DF_CPU=12000000 -o $(FileMain).elf $(FileMain).o $(FileLayer1).o $(FileLayer2).o $(FileLayer3).o $(FileCRC).o $(FileUART).o $(FileIRQ).o

	avr-objcopy -O ihex -R .eeprom $(FileMain).elf $(FileMain).hex
	avrdude -c gpio -p atmega328p -U flash:w:$(FileMain).hex

	rm $(FileMain).elf $(FileMain).hex $(FileMain).o $(FileLayer1).o $(FileLayer2).o $(FileLayer3).o $(FileCRC).o $(FileUART).o $(FileIRQ).o


#docs : Generates web documentation using Doxygen
docs: 
	doxygen $(FileConfigDoxygen)

	