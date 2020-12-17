// gcc -fno-stack-protector -z execstack -m32 -o source source.c

#include <stdio.h>
#include <string.h>

char	a_user_name[0x100];

int	verify_user_name(void)
{
	puts("verifying username....\n");
	return (strncmp(a_user_name, "dat_wil", 7));
}

int	verify_user_pass(char *pass)
{
	return (strncmp(pass, "admin", 5));
}

int	main(void)
{
	char	buf[0x40];
	int		ret;

	bzero(buf, 0x40);
	ret = 0;
	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 0x100, stdin);
	if ((ret = verify_user_name()))
	{
		puts("nope, incorrect username...\n");
		return (1);
	}
	puts("Enter Password: ");
	fgets(buf, 0x64, stdin);
	ret = verify_user_pass(buf);
	if (ret == 0)
		goto end;
	if (ret == 0)
		return (0);
	end:
	puts("nope, incorrect password...\n");
	return (1);
}