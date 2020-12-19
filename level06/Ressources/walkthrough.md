# Level06

## Solution

* There is one file in our home, ```level05```, belonging to ```level06``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the disassembled code, but we will describe its behaviour:

	* ```main()``` prompts the user for his login and gets it with ```fgets()```
	* it then asks him for a number and gets it with ```scanf```
	* it then calls ```auth()``` passing the login and the number in parameter
	* if ```auth()``` returns ```0```, ```system("/bin/sh"``` is launched ; else the program returns

	* ```auth``` first makes checks: 
		* if ```login``` length is less or equal to ```0x5```, returns ```1```
		* if ```ptrace```, with ```TRACEME``` parameter, returns ```-1```, returns ```1```
	* ```auth``` then iterates on ```login``` and hashes it. It also checks that each character has a value greater than ```0x1f```.
	* finally returns ```0``` if the hash is equal to the user number input, ```1``` else

* What we did with all of that:
	* our goal was to see what was the hash value of login ```aaaaaa```
	* we used to set breakpoints at the right places :
	```
	(gdb) b *0x080487ba
	Breakpoint 1 at 0x80487ba
	(gdb) b *0x08048866
	Breakpoint 2 at 0x8048866
	(gdb) run
	Starting program: /home/users/level06/level06
	***********************************
	*		level06		  *
	***********************************
	-> Enter Login: aaaaaa
	***********************************
	***** NEW ACCOUNT DETECTED ********
	***********************************
	-> Enter Serial: 42

	Breakpoint 1, 0x080487ba in auth ()
	(gdb) print $eax=0
	$1 = 0
	(gdb) c
	Continuing.

	Breakpoint 2, 0x08048866 in auth ()
	(gdb) x/dx $ebp-0x10
	0xffffd678:	0x005f160a
	```
	* first breakpoint is right after call to ```ptrace()```: we are in gdb so it returns ```-1``` : we change the value of ```eax``` to ```0```
	* second breakpoint is at the end of the program: we can get the hash value (located at ```ebp-0x10```). For ```aaaaaa```, hash value is ```0x005f160a``` that is to say ```6231562```
	* finally:
	```
	level06@OverRide:~$ ./level06
	***********************************
	*		level06		  *
	***********************************
	-> Enter Login: aaaaaa
	***********************************
	***** NEW ACCOUNT DETECTED ********
	***********************************
	-> Enter Serial: 6231562
	Authenticated!
	$ cd ../level07
	$ cat .pass
	GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
	```
