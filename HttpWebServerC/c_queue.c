#include "c_queue.h"
//TODO: remove stdio
#include <stdio.h>

/*typedef struct{
	int cq_head,
		cq_tail,
		cq_max_size;
	const int *cq_job_request;	    
}c_queue_t;
*/

void init_cq(c_queue_t *job_cq_obj ,int cq_size){
		job_cq_obj->cq_head = -1;
		job_cq_obj->cq_tail = -1;
		job_cq_obj->cq_max_size = cq_size;	
		job_cq_obj->request_job_cq = (int*)malloc(sizeof(int)*cq_size);	
		fprintf(stderr,"\nend::init:head::%d tail%d::",job_cq_obj->cq_head,job_cq_obj->cq_tail);
}

bool is_cq_full(c_queue_t job_cq_obj){
	fprintf(stderr,"\nenter:is_full:head::%d tail%d::",job_cq_obj.cq_head,job_cq_obj.cq_tail);
	if(job_cq_obj.cq_head == (job_cq_obj.cq_tail + 1)%job_cq_obj.cq_max_size){
		return true;
	}
	return false;
}

bool is_cq_empty(c_queue_t job_cq_obj){
	fprintf(stderr,"\nenter:is_empty:head::%d tail%d::",job_cq_obj.cq_head,job_cq_obj.cq_tail);
	if(job_cq_obj.cq_head == -1 && job_cq_obj.cq_tail == -1){
		return true;
	}
	return false;
}

void insert_next(c_queue_t *job_cq_obj,int job_info){
	fprintf(stderr,"\nenter:insert:head::%d tail%d::",job_cq_obj->cq_head,job_cq_obj->cq_tail);
	if(!is_cq_full(*job_cq_obj)){
		/*if(job_cq_obj.cq_head == job_cq_obj.cq_tail == -1){
			job_cq_obj.cq_head = 0;
			job_cq_obj.cq_tail = 0;
		}
		else{*/
		
		//if inserting the first element
		if(is_cq_empty(*job_cq_obj)){
			job_cq_obj->cq_head = 0;
		}
		job_cq_obj->cq_tail = (job_cq_obj->cq_tail + 1)%job_cq_obj->cq_max_size;
		//}
		job_cq_obj->request_job_cq[job_cq_obj->cq_tail] = job_info;
	}	
	fprintf(stderr,"\nend:insert:head::%d tail%d::",job_cq_obj->cq_head,job_cq_obj->cq_tail);
}

int remove_next(c_queue_t *job_cq_obj){
	int job_info = -1;
	fprintf(stderr,"\nenter:remove:head::%d tail%d::",job_cq_obj->cq_head,job_cq_obj->cq_tail);
	if(!is_cq_empty(*job_cq_obj)){
		job_info = job_cq_obj->request_job_cq[job_cq_obj->cq_head];
		job_cq_obj->cq_head = (job_cq_obj->cq_head + 1)%job_cq_obj->cq_max_size;
	}
	if(job_cq_obj->cq_head == (job_cq_obj->cq_tail + 1)%job_cq_obj->cq_max_size){
		job_cq_obj->cq_head = -1;
		job_cq_obj->cq_tail = -1;
	}
		
	fprintf(stderr,"\nend:remove:head::%d tail%d::",job_cq_obj->cq_head,job_cq_obj->cq_tail);
	return job_info;	
}

