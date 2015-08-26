#ifndef H_C_QUEUE
#define H_C_QUEUE

#include <stdbool.h>
#include <stdlib.h>

typedef struct{
	int cq_head,
		cq_tail,
		cq_max_size;
	//make this a dynamic array
	//const int* request_job_cq;	    
	int* request_job_cq;	    
}c_queue_t;

void init_cq(c_queue_t*,int);
bool is_cq_full(c_queue_t);
bool is_cq_empty(c_queue_t);
void insert_next(c_queue_t*,int);
int remove_next(c_queue_t*);

#endif
