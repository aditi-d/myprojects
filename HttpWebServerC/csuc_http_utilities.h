#ifndef H_CSUC_UTILITIES
#define H_CSUC_UTILITIES

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include "logger.h"

typedef struct{
	char p_strategy;
	char p_root_dir[URI_MAX];
	int p_port;
	int p_thread_pool_size;
	int p_queue_size;
	int p_log_level;
}server_params;

int handle_request_response(int fd);
void print_usage();
int read_opt(int argc, char **argv, server_params*);
void init_server_params(server_params);
int create_server_socket();
int register_signals();
void exit_web_server();
void signal_handler(int);

//TODO: put in a different file: strategy.c/.h
typedef struct{
	pthread_t *thread_pool_pointer;
	int thread_pool_size;
}thread_pool;


int run_serial_strategy(int);
int run_fork_strategy(int);

int run_thread_strategy(int);
void* handle_request_response_thread(void* s_read_fd);

void init_thread_pool_strategy();
int run_thread_pool_strategy(int);
void* job_request_consumer_thread(void *job_request_buffer);
void init_thread_pool(thread_pool*,int );
void set_blocking_signals();
#endif
