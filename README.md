# ALTERNATE BIT PROTOCOL IMPLEMENTED IN CADMIUM #

----------
### ORGANIZATION ###

----------

#### Carleton University ####


### AUTHORS ###

----------
#### Orginally Developed By  : ####

**Cristina Ruiz Martin**
#### Modified By  : ####
**Namitha Jacob, Dils George**


### INTRODUCTION ###

----------
ABP (Alternating Bit Protocol) is a communication protocol to ensure reliable transmission
through unreliable network. The sender sends a packet and waits for an acknowledgement. If the acknowledgement doesn't arrive within a predefined time, the sender re-sends this packet until it receives an expected acknowledgement and then sends the next packet. In order to distinguish two consecutive packets, the sender adds an additional bit on each packet (called alternating bit because the sender uses 0 and 1 alternatively). This software is a simulator to replicate the behavior of the Alternating Bit Protocol. 

The simulator consist of mainly 3 parts.

1. **Receiver** :  Receive the data and send back an acknowledgement extracted
from the received data after a time period.

2. **Sender**   : The sender changes from initial phase passive to active when an external signal is received. Once activated it start sending the packet with the alternating bit. Every time a packet is sent, it waits for an acknowledgement during an specific waiting time.

3. **Subnet**   : Subnet just pass the packets after a time delay.

### FILE ORGANIZATION ###


----------

**bin** - This folder contains all the executable(.exe) files and is created during the execution phase.

**build** - This folder contains all the object(.o) files and is also created during the execution phase.

**data** - This folder contains the input and output files for the simulator.

1. abp_output\_1.txt
2. abp_output\_0.txt
3. input_abp\_0.txt
4. input_abp\_1.txt
5. output.txt - stores the organised output file
6. limit_output.txt - stores the modiefied output based on structure
		 

**doc** - This folder contains pdf documentation other than README.txt

1. alternatebitprot.pdf
2. Cadmium_Documentation\_Ubuntu.pdf
3. Cadmium\_Windows.pdf
		
		
**include** - This folder consists of  header files used in the code.

1. receiverCadmium.hpp
2. senderCadmium.hpp
3. subnetCadmium.hpp
4. message.hpp
5. filter.hpp
6. limit.hpp
		 
				
**lib** - This folder contains third-party library files.

1. cadmium - This folder contains the cadmium library files.
2. DESTimes - This folder contains the DESTimes library files.
3. vendor   - This folder contains third party vendor files.
    + include
        * iestream.hpp 

		 
**src** - This folder contains the source files(.cpp).

1. message 
    + message.cpp
2. top_model
    + main.cpp	 	
3. text_filter.cpp [to modify the output]	
4. limit_output.hpp[to modify output from text_filter and limit the output based on structure elements]

**test** - This folder contains files for the unit tests.

1. data  - This folder contains the input and output files for the unit tests.
    + receiver  - This folder contains input and output data for receiver
        * receiver_input\_test.txt
		* receiver_test\_output.txt
		* output.txt - stores the organised output file
		* limit_output.txt - stores the modiefied output based on structure

	+ sender - This folder contains input and output data for sender.
	    * sender_input_test_ack\_In.txt
	    * sender_input_test_control\_In.txt
	    * sender_test\_output.txt
	    * output.txt - stores the organised output file
	    * limit_output.txt - stores the modiefied output based on structure

	+ subnet  - This folder contains input and output data for subnet.
	    * subnet_input\_test.txt
	    * subnet_test\_output.txt
	    * output.txt - stores the organised output file
	    * limit_output.txt - stores the modiefied output based on structure

	    
2. src - This folder contains the source code(.cpp) files for the unit tests.
    + receiver - This folder contains source code for the receiver.
        * main.cpp
    + sender  - This folder contains source code for the sender.
        * main.cpp
    + subnet  - This folder contains source code for the subnet.
        * main.cpp

### STEPS TO RUN SIMULATOR ###

----------

Each component using the “black box” testing method. Test cases are created by adding different combinations of inputs, run the simulator and check whether the outputs are what we expected.

alternatebitprotocol.doc contains the explanation of this simulator

1. Update include paths in the makefile in this folder and subfolders. You need to update the following lines:
    * INCLUDECADMIUM=-I ../../cadmium/include
    * Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process.
          
				Example: INCLUDECADMIUM=-I ../../cadmium/include
2. Run the unit tests
    1. Run subnet test
        * Open the terminal, Press in your keyboard Ctrl+Alt+t.
        * To compile the test, type in the terminal:
          
					make clean; make all;
        * To run the test go the bin folder in terminal. 
          
					cd bin
        * Inside the bin folder, type in the terminal "./NAME_OF_THE_COMPILED_FILE". For this specific test you need to type:
 

				./SUBNET.exe	  
        * To check the output of the test, open  "../test/data/subnet_test\_output.txt"
    2. To run receiver and sender tests, the steps are analogous to 2.1.
    3. To check the modified output, output file is stored under data folder.

3. Run the simulator.
    1. Open the terminal.
    2. To compile the project, type in the terminal:
 
				make clean; make all
    3. To run the simulation go the bin folder in terminal.

					cd bin
    4. To run the simulation, type in the bin folder "./NAME_OF_THE_COMPILED\_FILE       NAME_OF_THE_INPUT\_FILE". For this test you need to type:
		
				./main_pg ../data/input_abp_0.txt

    5. To check the output of the simulation, open "../data/abp_output\_0.txt".
    6. To execute the simulator with different inputs.
        * Create new .txt files with the same structure as input_abp\_0.txt or input_abp\_1.txt.
        * Run the simulator using the instructions in step 4.
        * If you want to keep the output, rename abp_output.txt. To do so, go to bin folder in the terminal then type:
        
					"mv abp_output.txt NEW_NAME"
					Example: mv abp_output.txt abp_output_0.txt

 

