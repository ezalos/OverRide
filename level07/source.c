//gcc -m32 -o source source.c
#include <stdio.h>
#include <string.h>

void			clear_stdin()
{
	char	c;

	c = 0;
	while ((c = getchar()) != -1)
		if (c == '\n')
			return ;
}

unsigned int	get_unum(void)
{
	unsigned int	unum;

	unum = 0;
	fflush(stdin);
	scanf("%u", &unum);
	clear_stdin();
	return (unum);
}


int		store_number(int *buf)
{
	unsigned int	nb; //[ebp-0x10]
	unsigned int	index; //[ebp-0xc]

	nb = 0;
	index = 0;
	printf(" Number: ");
	nb = get_unum();
	printf(" Index: ");
	index = get_unum();
	if (index % 3 == 0 || (nb >> 0x18) == 0xb7)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return (1);
	}
	buf[index] = nb;
	return (0);
}

int		read_number(int *buf)
{
	unsigned int	var1;

	var1 = 0;
	printf(" Index: ");
	var1 = get_unum();
	printf(" Number at data[%u] is %u\n", var1, buf[var1]);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	int	buf[0x64]; //  [esp+0x24]
	int	ret; //[esp+0x1b4]
	char	command[0x14]; // [esp+0x1b8]
	

	// args = av;
	ret = 0;
	((int*)(command))[0] = 0;
	((int*)(command))[1] = 0;
	((int*)(command))[2] = 0;
	((int*)(command))[3] = 0;
	((int*)(command))[4] = 0;

	memset(buf, 0, 0x190);
	while (*av)
	{
		memset(*av, 0x0, strlen(*av));
		av++;
	}
	while (*env)
	{
		memset(*env, 0x0, strlen(*env));
		env++;
	}
	puts("----------------------------------------------------\n\
  Welcome to wil's crappy number storage service!\n\
----------------------------------------------------\n\
 Commands:\n\
    store - store a number into the data storage\n\
    read  - read a number from the data storage\n\
    quit  - exit the program\n\
----------------------------------------------------\n\
   wil has reserved some storage :>\n\
----------------------------------------------------");

	while (42)
	{
		printf("Input command: ");

		ret = 1;
		fgets(command, 0x14, stdin);
		((char*)(command))[strlen(command) -1] = '\0';
		if (!strncmp(command, "store", 0x5))
			ret = store_number(buf);
		if (!strncmp(command, "read", 0x4))
			ret = read_number(buf);
		if (!strncmp(command, "quit", 0x4))
			return (0);
		if (ret != 0)
			printf(" Failed to do %s command\n", command);
		else
			printf(" Completed %s command successfully\n", command);
		((int*)(command))[0] = 0;
		((int*)(command))[1] = 0;
		((int*)(command))[2] = 0;
		((int*)(command))[3] = 0;
		((int*)(command))[4] = 0;	
	}
}