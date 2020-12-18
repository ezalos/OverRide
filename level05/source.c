// gcc -fno-stack-protector -z execstack -m32 -o source source.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void	main(void)
{
	char 	buf[0x64];
	size_t	len;
	size_t	i;

	fgets(buf, 0x64, stdin);
	i = 0;
	while (i < strlen(buf))
	{
		if (buf[i] > 0x40 && buf[i] <= 0x5a)
			buf[i] = buf[i] ^ 0x20;
		i++;
	}
	printf(buf);
	exit(0);
}