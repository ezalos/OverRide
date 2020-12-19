//gcc -m32 -o source source.c 
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>

int		auth(char *login, unsigned int serial) //login = [ebp+0x8] [ebp + 0xc]
{
	size_t	len; //[ebp-0xc]
	int		hash; //[ebp-0x10]
	int		i; //[ebp-0x14]

	login[strcspn(login, '\n')] = 0;
	len = strnlen(login, 0x20);
	if (len <= 0x5)
		return (1);
	if (ptrace(PT_TRACE_ME, 0, 1, 0) == -1)
	{
		puts("\[32m.---------------------------.");
		puts("\[31m| !! TAMPERING DETECTED !!  |");
		puts("\[32m'---------------------------'");
		return (1);
	}
	hash = (login[3] ^ 0x1337) + 0x5eeded;
	i = 0;
	while (i < len)
	{
		if (login[i] <= 0x1f)
			return (1);
		hash += ((login[i] ^ hash) * 0x88233b2b); //calcul en realite plus complique
		i++;
	}
	if (serial == hash)
		return (0);
	return (1);
}



int		main(void)
{
	unsigned int	serial;
	char			buf_login[0x20];

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	print("-> Enter Login: ");
	fgets(buf_login, 0x20, stdin);
	puts("***********************************");
	puts("***** NEW ACCOUNT DETECTED ********");
	puts("***********************************");
	printf("-> Enter Serial: ");
	scanf("%u", &serial);
	if (auth(buf_login, serial) != 0)
		return (1);
	puts("Authenticated!");
	system("/bin/sh");
	return (0);
}