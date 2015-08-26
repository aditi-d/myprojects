#ifndef H_LOG
#define H_LOG

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

void init_log(int log_level);
void logger(int log_level,char *msg,...);
int change_log_level();

typedef enum{
	ERROR,
	WARNING,
	INFO,
	DEBUG
}log_level_t;

#endif
