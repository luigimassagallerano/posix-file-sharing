#ifndef __MSG_ERR_H__
#define __MSG_ERR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

void sys_err(string s);
void sys_warn(string s);
void user_err(string s);

#endif
