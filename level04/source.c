//gcc -fno-stack-protector -z execstack -m32 -o source source.c
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <string.h>
#include <sys/wait.h>


int	main(void)
{
	int		stat_loc; // [esp+0x1c]
	char	buf[0x80]; //[esp+0x20]
	int		status1; //[esp+0xa0]
	int		status2; //[esp+0xa4]
	int		ptrace_ret; //[esp+0xa8]
	pid_t	pid; // [esp+0xac]

	pid = fork();
	memset(buf, 0, 0x80);
	ptrace_ret = 0;
	stat_loc = 0;
	if (pid == 0)
	{
		prctl(0x1, 0x1); // PR_SET_PDEATHSIG with signal SIGHUP
		ptrace(PT_TRACE_ME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(buf);
	}
	else
	{
		while (1)
		{
			wait(&stat_loc);
			status1 = WIFEXITED(stat_loc);
			status2 = WIFSIGNALED(stat_loc);
			if (status1 || status2)
			{
				puts("child is exiting...");
				return (0);
			}
			ptrace_ret = ptrace(PT_READ_U, pid, 0x2c, 0x0);
			if (ptrace_ret != 0xb)
				continue ;
			puts("no exec() for you");
			kill(pid, 0x9);
			return (0);
		}
	}
}
