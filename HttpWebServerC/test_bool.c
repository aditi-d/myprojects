#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
     
     void
     logger(int log_level,char *msg,...)
     {       
       FILE *log_file = fopen("log_file.txt","w");
       va_list ap;
       va_start(ap,msg);       
       vfprintf(log_file,msg,ap);
	   va_end(ap);      
     }
     
     int
     main (void)
     {
		int a = 5;
	   logger(1,"\nHi there %d",a);
       return 0;
     }

/*
int log(log_level_t log_level, const char *message, ...);

int main(){
	bool some_flag;
	time_t mytime1,mytime2;
	int uptime;
	printf("\ninit: %d",some_flag);
	some_flag = false;
	printf("\nfalse:: %d",some_flag);
	some_flag = true;
	printf("\ntrue:: %d",some_flag);
	
	
	mytime1 = time(NULL);
    printf("\n%d %d",mytime1,ctime(&mytime1));
	sleep(6);
    mytime2 = time(NULL);
    printf("\n%d %d",mytime1,ctime(&mytime2));
	uptime = mytime2 - mytime1;
	
	printf("\n%d",uptime);
	
	return 0;
}
*/
