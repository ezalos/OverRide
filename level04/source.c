#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>

int		main(int ac, char **av)
{
	//0x080486d0 <+8>:	sub    esp,0xb0
	
	//[esp+0x1c]
	int		*wstatus;
	//[esp+0x20]
	char	buf[0x20 * 2];
	//[esp+0xa0]
	void	*thing_1;
	//[esp+0xa4]
	void	*thing_2;
	//[esp+0xa8]
	void	*thing_3;
	//[esp+0xac]
	pid_t	fork_val;

	fork_val = fork();
	memset(buf, 0, 0x20 * 2);
	if (fork_val == 0)//0 is child
	{
		wait(wstatus);
		thing_1 = wstatus;
		if (*thing_1 == 0x7f)
		{

		}
		else
		{
			*thing_2 = *wstatus;
			if ((((*thing_2 & 0x7f) + 1) >> 1) <= 0)//jle after test al al
			{
				thing_3 = ptrace(3, 0x2c, *fork_val, 0x3);
			}
			puts("child is exiting...");
			return (0);
		}
	}
	else
	{
		prctl(0x1 /*PR_SET_PDEATHSIG ?*/, 0x1 /*SIGHUP ?*/);
		ptrace(0x0, 0x0, 0x0, 0x0);
		puts("Give me some shellcode, k");
		gets(buf);
		return (0);
	}

}
