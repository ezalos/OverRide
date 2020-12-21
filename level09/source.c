//gcc -o -fno-stack-protector source source.c
#include <stdio.h>
#include <string.h>

typedef struct	s_data
{
	char	message[0x8c];
	char	username[0x28];
	int		value;
}				t_data;


void	secret_backdoor(void)
{
	char	buf[0x80];

	fgets(buf, 0x80, stdin);
	system(buf);
}

void	set_username(t_data *data)
{
	//data = [rbp-0x98]

	char	username[0x80]; //[rbp-0x90]
	int		i;

	memset(username, 0, 0x80);
	puts(">: Enter your username");
	printf(">>: ");
	fgets(username, 0x80, stdin);
	i = 0;
	while (i <= 0x28 && username[i])
	{
		data->username[i] = username[i];
		i++;
	}
	printf(">: Welcome, %s", data->username);
}

void	set_msg(t_data *data)
{
	//data = [rbp-0x408]

	char message[0x400];

	memset(message, 0, 0x400);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(message, 0x400, stdin);
	strncpy(data->message, message, data->value);
}

void	handle_msg(void)
{
	t_data	data;

	bzero(data.username, 0x28);
	data.value = 0x8c;
	set_username(&data);
	set_msg(&data);
	puts(">: Msg sent!");
}

int		main(void)
{
	puts("--------------------------------------------\n\
|   ~Welcome to l33t-m$n ~    v1337        |\n\
--------------------------------------------");
	handle_msg();
	return (0);
}