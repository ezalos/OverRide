# Level03

## Solution

* There is one file in our home, ```level03```, belonging to ```level04``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the very long disassembled code, but we will describe its behaviour:

	* ```main()``` prompts for a password, gets it using ```scanf()``` and stores the value in an ```int``` variable
	* it then calls ```test()```, passing in parameter the user input and another ```int``` with value ```0x1337d00d```
	*  ```test()``` computes the difference between the ref number and the user input nb :
		* if the difference is above ```0x15```, ```test()``` calls ```decrypt()``` passing it a random number
		* if not, it will jump to different parts of the code, in function of the value of the difference. Those differents parts call ```decrypt()``` passing it either the difference or a random value
	* ```decrypt``` decrypts ```Q}|u`sfg~sf{}|a3``` applying a ```xor``` on each character with the key passed in parameter
	* then it applies ```strncmp()``` to compare the decoded string with ```Congratulations!```. If ```strncmp()``` returns ```0```, ```system("/bin/sh")``` is launched ; otherwise ```Invalid Password``` is outputed

* What we did with all of that:

	* the goal was to find the key allowing to decode ```Q}|u`sfg~sf{}|a3``` to ```Congratulations!```, knowing that there was a maximum of ```0x15``` potential keys
	* we went to ```dcode.fr``` and found out that ```0x12 = 18``` was the right key
	* as the key is the difference between ```0x1337d00d``` and our input, we computed it by doing: ```0x1337d00d - 0x12 = 322424845 - 18 = 322424827```
	* Resolution:
	```
	level03@OverRide:~$ ./level03
	***********************************
	*		level03		**
	***********************************
	Password:322424827
	$ cd ../level04
	$ cat .pass
	kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
	```