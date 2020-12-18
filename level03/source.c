// gcc -m32 -o source source.c

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


void	decrypt(int key)
{
	char	buf[0x11];
	size_t	len;
	size_t	i;

	strcpy(buf, "Q}|u`sfg~sf{}|a3");
	len = strlen(buf);
	i = 0;
	while (i < len)
	{
		buf[i] = (char)(key ^ (int)(buf[i]));
	}
	if (!(strncmp(buf, "Congratulations!", 0x11)))
		system("/bin/sh");
	else
		puts("\nInvalid Password");	
}

void	test(int user_input, int ref)
{
	int diff;

	diff = ref - user_input;
	if (diff <= 0x15)
		decrypt(diff);	//actually diff allows to compute an offset (diff << 2)
						//that will be add to an address storing different
						//instruction locations. All the values below 0x15 won't
						//lead to a call to decrypt(diff) but it doesn't matter
						// for the solution
	else
		decrypt(rand());
}

int		main(void)
{
	int	user_input;

	srand(time(NULL));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf ("Password:");
	scanf("%d", &user_input);
	test(user_input, 0x1337d00d);
	return (0);
}

