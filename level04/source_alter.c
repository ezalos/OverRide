//gcc -fno-stack-protector -z execstack -m32 -o source source.c
#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <string.h>


int	main(void)
{
	int		stat_loc; // [esp+0x1c]
	char	buf[0x80]; //[esp+0x20]
	pid_t	pid; // [esp+0xac]
	long	ptrace_ret; //[esp+0xa8]



	pid = fork();
	memset(buf, 0, 0x80);
	ptrace_ret = 0;
	stat_loc = 0;
	if (pid == 0)
	{
		prctl(0x1, 0x1);  // PR_SET_PDEATHSIG
		ptrace(PT_TRACE_ME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(buf);
	}
	else
	{
		while (1)
		{
			wait(&stat_loc);
			if (stat_loc & 0x7f != 0)
			{
				if ((((stat_loc & 0x7f) + 0x1) >> 1) > 0)
				{
					puts("child is exiting...");
					return (0);
				}
				ptrace_ret = ptrace(PT_READ_U, pid, 0x2c, 0x0);
				if (ptrace_ret != 0xb)
					continue ;
				puts("no exec() for you");
				kill(pid, 0x9);
			}
			else
			{
				puts("child is exiting...");
				return (0);
			}
		}
	}	
}