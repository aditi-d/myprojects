#include "bounded_buffer.h"
#include "logger.h"

void job_request_producer(b_buffer *job_request_buffer, int request_fd){
	
	logger(DEBUG,"\nin the producer::");	
	pthread_mutex_lock(&job_request_buffer->mutex_buffer_status);
	logger(DEBUG,"\nin the producer has lock::");
	while(is_buffer_full(*job_request_buffer)){		
		pthread_cond_wait(&job_request_buffer->cond_buffer_not_full,
		&job_request_buffer->mutex_buffer_status);
	}
	logger(DEBUG,"\nin the producer adding::");	
	//add to buffer
	add_new_job(job_request_buffer,request_fd);
	logger(DEBUG,"\nadded new job::");	
	pthread_cond_broadcast(&job_request_buffer->cond_buffer_not_empty);
	//broadcast to say that something is in the buffer
	pthread_mutex_unlock(&job_request_buffer->mutex_buffer_status);	
}

void job_request_consumer(b_buffer *job_request_buffer){
	int request_fd;
	while(1){		
		pthread_mutex_lock(&job_request_buffer->mutex_buffer_status);
		logger(DEBUG,"\nbuffer_status::%d",is_buffer_empty(*job_request_buffer));	
		while(is_buffer_empty(*job_request_buffer)){
			logger(DEBUG,"\nReleasing consumer lock::");		
			logger(DEBUG,"\nReleased consumer lock::");
			pthread_cond_wait(&job_request_buffer->cond_buffer_not_empty,
							  &job_request_buffer->mutex_buffer_status);
		}
		request_fd = get_next_job(job_request_buffer);
		pthread_cond_signal(&job_request_buffer->cond_buffer_not_full);
		pthread_mutex_unlock(&job_request_buffer->mutex_buffer_status);
		handle_request_response(request_fd);
	}
}
 
int init_b_buffer(b_buffer *job_request_buffer,int queue_size){
	int retval = 0;
		retval = pthread_mutex_init(&job_request_buffer->mutex_buffer_status,NULL);
	if(retval != 0)
		return retval;
		retval = pthread_cond_init(&job_request_buffer->cond_buffer_not_empty,NULL);
	if(retval != 0)
		return retval;
		retval = pthread_cond_init(&job_request_buffer->cond_buffer_not_full,NULL);
	if(retval != 0)
		return retval;		
		init_cq(&(job_request_buffer->job_request_queue),queue_size);
	return 0;
}

bool is_buffer_empty(b_buffer job_request_buffer){
	return is_cq_empty(job_request_buffer.job_request_queue);
}

bool is_buffer_full(b_buffer job_request_buffer){
	return is_cq_full(job_request_buffer.job_request_queue);
}

int get_next_job(b_buffer *job_request_buffer){
	return remove_next(&(job_request_buffer->job_request_queue));
}

void add_new_job(b_buffer *job_request_buffer,int request_fd){
	insert_next(&(job_request_buffer->job_request_queue),request_fd);
}
