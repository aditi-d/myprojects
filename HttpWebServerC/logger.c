#include <string.h>
#include "logger.h"
#define MAX_LOG_NAME_SIZE 10

log_level_t current_log_level;
log_level_t max_level;

void init_log(int log_level){
	current_log_level  = log_level;
	max_level		   = DEBUG;
}

void logger(int log_level,char *msg,...){              
   va_list msg_list;
   va_start(msg_list,msg); 
   char log_level_str[MAX_LOG_NAME_SIZE];
   
   //add current date,time,log level
   //changing log level without 
   if(log_level <= current_log_level){
	   switch(log_level){
		   case ERROR:
				strcpy(log_level_str,"ERROR");
		   break;
		   case WARNING:
				strcpy(log_level_str,"WARNING");
		   break;
		   case INFO:
				strcpy(log_level_str,"INFO");
		   break;
		   case DEBUG:
				strcpy(log_level_str,"DEBUG");
		   break;
	   }
	    fprintf(stderr,"\n%s::",log_level_str);
		vfprintf(stderr,msg,msg_list);
   }   
   va_end(msg_list);      
}

int change_log_level(){
	current_log_level = (current_log_level + 1) % (max_level + 1);
}
