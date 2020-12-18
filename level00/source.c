// gcc source.c -fno-stack-protector -z execstack
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define STARS "***********************************"

int	main(void)
{
	int32_t		nb;

	puts(STARS);
	puts("* \t     -Level00 -\t\t  *");
	puts(STARS);
	printf("Password:");
	scanf("%d", &nb);
	if (nb == 0x1a7)//5276
	{
		puts("\nAuthenticated!");
		system("/bin/sh");
		return (0);
	}
	else
	{
		puts("\nInvalid Password!");
		return (1);
	}
}
