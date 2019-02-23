
#1.    Execution of code using Make File: (Please also check hardware related connection mentioned below)

    Steps:
        1. Compile the code file
        2. Link the code file to .elf which will be used to generate the .hex file
        3. Copy the .elf file data and generate the .hex file which will be used for flashing on the board
        4. Flash the .hex file on board
        5. Delete unnecessary files

        All these are steps will be performed by a single call "make all"
        
        In command prompt, navigate to where "make" file is available and execute command "make all"

#2.     Generates web documentation using Doxygen

        -Doxygen is used to generate the document.
        -The command is "make docs"
            
#3.     Cleaning files generated from doxygen or execution

        -Command is  "make clean"
        
#4.     Hardware-Physical wire connections requirement

        -Pin Connections
        1. PB1-PD3  : Clock Send-Recieve.
        2. PB2-PD3  : Data Send-Recieve.
        3. Any of Gnd pins connected for the clock reference.



