//x86_64
//canary

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void	log_wrapper(FILE *fd, char *msg, char *file)
{
	//sub    rsp,0x130
	//[rbp-0x130]
	//rcx 1st scas
	
	//[rbp-0x128]
	//rsi 1st scas
	
	//[rbp-0x120]
	//rdx/rsi strcpy
	
	//[rbp-0x118]
	//fprintf rdi
	
	//[rbp-0x110]
	//rdx 1st scas
	//rdi/rdx 2nd scas
	char	res[0x110];

	strcpy(res, msg);
	strlen(res);
	snprintf(res, 0xfe - strlen(msg), file);
	res[strcspn(res, "\n")] = 0;
	fprintf(fd, "LOG: %s\n", res);
}

#define BACKUP_FILE		"./backups/.log"
#define BACKUP_DIR		"./backups/"

int		main(int ac, char **av)
{
	//	sub    rsp,0xb0
	//	[rbp-0x88]
	FILE		*fd_log;
	//	[rbp-0x80]
	FILE		*fd_user;
	//	[rbp-0x78]
	int			fd_backup;
	//	[rbp-0x71]
	char		file_octet;
	//	[rbp-0x70]
	char		buf[0x64];

	if (ac != 2)// ac == [rbp-0x94]
		printf("Usage: %s filename", av[0]);// av[0] == [rbp-0xa0]
	if (0 == (fd_log = fopen(BACKUP_FILE, "w")))
	{
		printf("ERROR: Failed to open %s\n", BACKUP_FILE);
		exit(0x1);
	}

	log_wrapper(fd_log, "Starting back up: ", av[1]);//av[1] == [rbp-0xa0+0x8]

	if (0 == (fd_user = fopen(av[1],"r")))
	{
		printf("ERROR: Failed to open %s\n", av[1]);
		exit(0x1);
	}
	
	strcpy(buf, BACKUP_DIR); //not in asm, but makes sense
	strncat(buf, av[1], 0x64 - strlen(buf));

	if (0 == (fd_backup = open(buf, 0xc1, 0x1b0)))
	{
		printf("ERROR: Failed to open %s%s\n", BACKUP_DIR, av[1]);
		exit(0x1);
	}
	do {
		file_octet = fgetc(fd_user);
		write(fd_backup, &file_octet, 1);
	} while (file_octet != 0xff);

	log_wrapper(fd_user, "Finished back up ", buf);
	fclose(fd_user);
	close(fd_backup);
	return (0);
}
