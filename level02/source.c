<<<<<<< HEAD
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
=======
// gcc source.c -fno-stack-protector -z execstack
#include <unistd.h>
#include <string.h>
#include <stdio.h>


int	main(void)
{
	//	 [rbp-0x8]
	FILE	*fd;

	//	 [rbp-0xc]
	int32_t	bytes_read;

	//   [rbp-0x70]
	char	buff_user[0xc];

	//   [rbp-0xa0]
	char	buff_read[0x5];

	//   [rbp-0x110]
	char	buff_pass[0xc];


	memset(buf_user, 0, 0xc);
	memset(buf_read, 0, 0x5);
	memset(buf_pass, 0, 0xc);
	fd = fopen("/home/users/level03/.pass", "r");
	if (fd == 0)
	{
		fwrite("ERROR: failed to open password file\n", 0x24, 0x1, stderr);
		exit(1);
	}
	bytes_read =fread(buff_read, 0x29, 0x1, fd);
	if (buff_read + bytes_read == strcspn(buff_read, "\n"))
	{
		fclose(fd);
		puts("===== [ Secure Access System v1.0 ] =====");
		puts("/***************************************\\");
		puts("| You must login to access this system. |");
		puts("\\***************************************/");
		printf("--[ Username: ");
		fgets(buff_user, 0x64, stdin);
		*strcspn(buff_user, "\n") = '\0';
		printf("--[ Password: ");
		fgets(buff_pass, 0x64, stdin);
		*strcspn(buff_pass, "\n") = '\0';
		puts("*****************************************");
		if (0 == strncmp(buff_read, buff_pass))
		{
			printf("Greetings, %s!\n", buff_user);
			system("/bin/sh");
			return (0);
		}
		else
		{
			printf(buff_user);
			puts(" does not have access!");
			exit(1);
		}

	}
	else
	{
		fwrite("ERROR: failed to read password file\n", 0x24, 0x1, stderr);
		fwrite("ERROR: failed to read password file\n", 0x24, 0x1, stderr);
		exit(1);
	}


}
>>>>>>> ea0bfb9b4049012a224c40bb562b805b9b4df8ac
