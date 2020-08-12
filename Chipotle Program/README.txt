Lucas Streanga 1001612558

README
-------------------------
File included: choice.cpp, and others
-------------------------

This is a program for managing chipotle delivery orders. The user will run the program with a file of names of delivery drivers.
Upon running the program, 3 options are present: customer, apply, and exit. Customer option will create a new order. User should follow
the prompts and select the desired items. If a driver is available, the order will be delivered. The apply option
allows a new driver to be entered. Once entered, the will be added to the available drivers as well as written to the proper file.
The exit option will end the program and display the chipotle revenue and delivery fees (at 20% by default). Keep in mind that an order that is
not deliverable will be counted towards chipotle revenue but will not be counted towards delivery charges. Please see the sample output:

Some notes:
The predefined values for the delivery percent as well as other prices are defined in globals.h. You can edit this and rebuild.
The resolution the program runs at is also defined in globals.h
The console is not used at all, but will output debug information, such as deletes

-------------------------
Compilation instructions:
 - This was built in a lubuntu virtual machine
 - Use the provided makefile
    make
    ./Chipotle
