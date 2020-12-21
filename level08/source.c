//x86_64
//canary

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void	log_wrapper(FILE *fd, char *msg, char *file)
{
	//sub    rsp,0x130
	//[rbp-0x110]
	char	buf[0xfe];

	strcpy(buf, msg);
	snprintf(buf + strlen(buf), 0xfe - strlen(buf), file);
	buf[strcspn(buf, "\n")] = 0;
	fprintf(fd, "LOG: %s\n", buf);
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

	file_octet = -1;
	fd_backup = -1;
	if (ac != 2)// ac == [rbp-0x94]
		printf("Usage: %s filename", av[0]);// av[0] == [rbp-0xa0]
	if (0 == (fd_log = fopen(BACKUP_FILE, "w")))//								./backups/.log
	{
		printf("ERROR: Failed to open %s\n", BACKUP_FILE);
		exit(0x1);
	}

	log_wrapper(fd_log, "Starting back up: ", av[1]);//av[1] == [rbp-0xa0+0x8]

	if (0 == (fd_user = fopen(av[1],"r")))//									av[1]
	{
		printf("ERROR: Failed to open %s\n", av[1]);
		exit(0x1);
	}
	
	strcpy(buf, BACKUP_DIR);
	strncat(buf, av[1], 0x64 - strlen(buf));

	if (0 > (fd_backup = open(buf, 0xc1, 0x1b0)))//								./backups + av[1]
	{
		printf("ERROR: Failed to open %s%s\n", BACKUP_DIR, av[1]);
		exit(0x1);
weird_loop:
		write(fd_backup, &file_octet, 1);
	}
		file_octet = fgetc(fd_user);
	if (file_octet != -1)
		goto weird_loop;

	log_wrapper(fd_log, "Finished back up ", av[1]);
	fclose(fd_user);
	close(fd_backup);
	return (0);
}
