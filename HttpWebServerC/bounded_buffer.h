#ifndef H_BOUNDED_BUFFER
#define H_BOUNDED_BUFFER

#include <pthread.h>
#include "c_queue.h"
#include <stdio.h>

typedef struct{
	pthread_mutex_t mutex_buffer_status;
	pthread_cond_t cond_buffer_not_empty;
	pthread_cond_t cond_buffer_not_full;	
	c_queue_t job_request_queue;
}b_buffer;

//int thread_pool_size = 10;

//void run_thread_pool_strategy(b_buffer*);
void job_request_producer(b_buffer*,int job);
void job_request_consumer(b_buffer*);
int get_next_job(b_buffer*);
void add_new_job(b_buffer*,int);
bool is_buffer_empty(b_buffer);
bool is_buffer_full(b_buffer);
int init_b_buffer(b_buffer *,int size);
#endif
