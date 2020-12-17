// gcc source.c -fno-stack-protector -z execstack
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>


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


	memset(buff_user, 0, 0xc);
	memset(buff_read, 0, 0x5);
	memset(buff_pass, 0, 0xc);
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
		buff_user[strcspn(buff_user, "\n")] = '\0';
		printf("--[ Password: ");
		fgets(buff_pass, 0x64, stdin);
		buff_pass[strcspn(buff_pass, "\n")] = '\0';
		puts("*****************************************");
		if (0 == strncmp(buff_read, buff_pass, 0x29))
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
