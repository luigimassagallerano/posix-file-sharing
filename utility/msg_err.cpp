#include "../include/msg_err.h"

void sys_err(string s)
{
	perror(s.c_str());
	exit(-1);
}

void sys_warn(string s)
{
	perror(s.c_str());
}

void user_err(string s)
{
	printf("%s\n", s.c_str());
	exit(-1);
}
