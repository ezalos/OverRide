//x86_64
//canary

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char var_from_fs[100];
char somewhere[100];

void	log_wrapper(FILE *fd, char *msg, char *file)
{
	//sub    rsp,0x130
	char	*res;
	
	strcpy(var_from_fs, msg);
	
	strlen(msg);
	res = snprintf(somewhere, 0xfe - strlen(msg), file);
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
	FILE		*fd_user;
	int			fd_backup;
	char		buf[0x64];
	int			r_int;

	if (ac != 2)
		printf("Usage: %s filename", av[0]);
	if (0 == (fd_log = fopen(BACKUP_FILE,"w")))
	{
		printf("ERROR: Failed to open %s\n", BACKUP_FILE);
		exit(0x1);
	}
	log_wrapper(fd_log, "Starting back up: ", av[2]);

	if (0 == (fd_user = fopen(av[1],"r")))
	{
		printf("ERROR: Failed to open %s\n", av[1]);
		exit(0x1);
	}
	
	buf = BACKUP_DIR;
	strncat(buf, av[1], 0x64 - strlen(BACKUP_DIR));

	if (0 == (fd_backup = open(buf, 0xc1, 0x1b0)))
	{
		printf("ERROR: Failed to open %s%s\n", BACKUP_DIR, av[1]);
		exit(0x1);
	}
	do {
		r_int = fgetc(fd_user);
		//write(-1, ?, 1);
	} while (r_int != 0xff);
	log_wrapper(fd_user, "???", buf);
	fclose(fd_user);
	close(fd_backup);
	return (0);
}
