# Level00

## Solution
 * There is one file in our home, ```level00```, belonging to ```level01``` user, with suid.
 * Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

 * We run the program with gdb and we observe that:
	* `main()` uses `scanf()` to get an user input
	*  if this number is equal to `0x149c`, or `5276` in decimal, the program launch `system("/bin/sh")`

```
level00@OverRide:~$ ./level00 
***********************************
* 	     -Level00 -		  *
***********************************
Password:5276

Authenticated!
$ cd ../level01
$ cat .pass	
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
$ 
```
