#include <time.h>
#include "server_stats.h"
#include "http_server_consts.h"
#include "logger.h"

long total_data_transferred;
int requests_handled;
long total_req_handling_time;
time_t server_start_time;
time_t req_processing_start_time;
time_t req_processing_end_time;


int init_stats(){
	total_data_transferred  = 0;
	requests_handled		= 0;
	total_req_handling_time = 0;
	req_processing_end_time = 0;
	server_start_time       = time(NULL);
	return 0;
}  

int total_uptime(){	
	return time(NULL) - server_start_time;
}

double avg_time_per_request(){
	double avg_time = 0;
	if(requests_handled != 0){
		avg_time = total_req_handling_time / requests_handled;
	}
	return avg_time;
}

void record_req_received_time(){
	struct timeval timeValue;
    if (gettimeofday( &timeValue, NULL) == 0 )
    {
		req_processing_start_time = timeValue.tv_usec;
	}
}

void record_req_end_time(){
	struct timeval timeValue;
	
    if (gettimeofday( &timeValue, NULL) == 0 )
    {
		req_processing_end_time 	= timeValue.tv_usec;
	}
	total_req_handling_time = total_req_handling_time
							+ (long)(req_processing_end_time 
							- req_processing_start_time);
}

double data_transferred(){
	return (total_data_transferred)/(1024*1024);
}

void add_data_transferred(int bytes_sent){
	logger(DEBUG,"\nbytes sent::%d",bytes_sent);
	total_data_transferred = total_data_transferred + bytes_sent;
	logger(DEBUG,"\ntotal bytes sent::%d",total_data_transferred);
}

void incr_requests_handled(){
	requests_handled++;
}

void get_server_stats(){
	logger(INFO,"--------------------------------------------------------------");
	logger(INFO,"\tEnvironmental information");
	logger(INFO,"--------------------------------------------------------------");
	
	logger(INFO,"Document root::               %s",http_server_info.root_dir);
	logger(INFO,"Port Number::                 %d",http_server_info.server_port);
	logger(INFO,"Response Strategy::           ");
	switch(http_server_info.strategy){
		case STRATEGY_FORK:
			logger(INFO,"Fork Strategy");
		break;
		case STRATEGY_SERIAL:
			logger(INFO,"Serial Strategy");
		break;
		case STRATEGY_THREAD:
			logger(INFO,"Thread Strategy");
		break;
		case STRATEGY_THREAD_POOL:
			logger(INFO,"Thread Pool Strategy");
			//logger(INFO,"Thread Pool size::%d",);
			//logger(INFO,"Queue size::%d",);
		break;
	}
	logger(INFO,"Current log level::           ");
	switch(http_server_info.log_level){
		case ERROR:
			logger(INFO,"ERROR");
		break;
		case WARNING:
			logger(INFO,"WARNING");
		break;
		case DEBUG:
			logger(INFO,"DEBUG");
		break;
		case INFO:
			logger(INFO,"INFO");
		break;
	}
	logger(INFO,"Total uptime::                %d s",total_uptime());
	logger(INFO,"Data Transferred::            %f MB",data_transferred());
	logger(INFO,"Total Requests Handled::      %d",requests_handled);
	logger(INFO,"Total request handling time:: %ld us",total_req_handling_time);
	logger(INFO,"Average time per request::    %f us",avg_time_per_request());
	logger(INFO,"--------------------------------------------------------------");
	logger(INFO,"--------------------------------------------------------------");
	logger(INFO,"");
}
