#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "http_request.h"
#include "http_response.h"
#include "http_server_consts.h"
#include "csuc_http_utilities.h"
#include "csuc_http_server.h"
#include "bounded_buffer.h"
#include "server_stats.h"

int run_serial_strategy(int s_read_fd){
	logger(DEBUG, "Serial method");
	return handle_request_response(s_read_fd);
}

int run_thread_strategy(int s_read_fd){
	pthread_t req_thread;										
	int retval = 0;
	int *client_socket = malloc(sizeof(int));
	*client_socket = s_read_fd;
	
	logger(DEBUG, "Socket is %d\n", *client_socket);
	
	retval = pthread_create(&req_thread,
			 NULL,
			 *handle_request_response_thread, 
			 (void *)client_socket);
	
	pthread_detach(req_thread);
	return retval;
}

int run_fork_strategy(int s_read_fd){
	int retval, 
		result_val = 0,
		status;
	
	retval = fork();
	if(retval == -1){
			perror(NULL);
			exit(EXIT_FAILURE);
	}
	else if(retval > 0){
			close(s_read_fd);
			if(waitpid(-1,&status,WNOHANG) == -1){
					logger(ERROR,"\nreturning from waitpid");
					exit(EXIT_FAILURE);
			}
			//while(waitpid(-1, &status, WNOHANG) > 0);                                                
	}
	else{
			sigset_t block_sig_set;
			set_blocking_signals(&block_sig_set);
			if(sigprocmask(SIG_BLOCK,&block_sig_set,NULL) == -1){
				exit(-1);
			}
			handle_request_response(s_read_fd);
			
			exit(EXIT_SUCCESS);
	}
	return result_val;
}	

void* handle_request_response_thread(void* s_read_fd){
    int client_socket = *((int *)s_read_fd);
    sigset_t block_sig_set;
    logger(DEBUG, "Socket is %d\n", client_socket);
    
    set_blocking_signals(&block_sig_set);
    pthread_sigmask(SIG_BLOCK,&block_sig_set,NULL);
    
    handle_request_response(client_socket);
    logger(DEBUG,"\n exiting thread...");
    
    free(s_read_fd);
    
    return NULL;
}

int read_opt(int argc, char **argv, server_params *params){
		int flag_thread_selection      = 0,
			flag_fork_selection 	   = 0,
			flag_thread_pool_selection = 0,
			retval;
			
        char cmd_line_option,
			 *endptr   = NULL;
        
        struct stat buf;       
                        
		strcpy(params->p_root_dir,DEFAULT_DIR);
		params->p_port			   = DEFAULT_PORT;
        params->p_strategy         = STRATEGY_SERIAL;
		params->p_thread_pool_size = THREAD_POOL_SIZE;
		params->p_queue_size       = JOB_QUEUE_SIZE;
		params->p_log_level		   = DEFAULT_LOG_LEVEL;
       
        while((cmd_line_option = getopt(argc, argv, OPTARGS))!= -1){
			//fprintf(stderr,"\n%c",cmd_line_option);
                switch(cmd_line_option){
                        case OPT_PORT:
                                params->p_port = atoi(optarg);//strtol(optarg, &endptr , 10);
                                if(optarg == endptr){
                                        logger(ERROR, "Invalid value of port.");
                                        exit(EXIT_FAILURE);
                                }
                        break;
                        case STRATEGY_FORK:
                                flag_fork_selection = 1;
                                params->p_strategy  = STRATEGY_FORK;
                        break;
                        case STRATEGY_THREAD:
                                flag_thread_selection = 1;
                                params->p_strategy    = STRATEGY_THREAD;
                        break;
                        case OPT_ROOT_DIR:
                                strcpy(params->p_root_dir,optarg);
                                retval    = stat(params->p_root_dir,&buf);
                                if(retval == EACCES || retval == ENOENT || retval == ENOTDIR){
                                        perror(NULL);
                                        exit(EXIT_FAILURE);
                                }
                        break;
                        case STRATEGY_THREAD_POOL:
								flag_thread_pool_selection = 1;
								params->p_strategy         = STRATEGY_THREAD_POOL;
								if( atoi(optarg) != 0)
									params->p_thread_pool_size = atoi(optarg);								
                        break;
                        case OPT_QUEUE_SIZE:
                        if( atoi(optarg) != 0)
								params->p_queue_size = atoi(optarg);								
                        break;
                        case OPT_LOG_LEVEL:								
								params->p_log_level = atoi(optarg);							
                        break;
                        case DEFAULT_ARG_CASE:
                                if(optopt == OPT_PORT || optopt == OPT_ROOT_DIR){
                                        logger(ERROR, "Option -%c requires an argument.\n", optopt);
                                        exit(EXIT_FAILURE);
                                }
                                print_usage();
                                exit(EXIT_FAILURE);
						}
                if(flag_thread_pool_selection == 1 && 
				  (flag_thread_selection == 1 || flag_fork_selection == 1)){
						print_usage();
                        exit(EXIT_FAILURE);
				}
                if(flag_thread_selection == 1 && flag_fork_selection == 1){
                        print_usage();
                        exit(EXIT_FAILURE);
                }
        }
        return 0;
}

void init_server_params(server_params params){
	logger(DEBUG,"\n %s",http_server_info.root_dir);
	strcpy(http_server_info.root_dir,params.p_root_dir);
	http_server_info.server_port 	= params.p_port;	
	http_server_info.strategy 		= params.p_strategy;
	http_server_info.log_level 		= params.p_log_level;
}

int create_server_socket(){
	int retval = 0;
	memset(&(http_server_info.host_addr),0,sizeof(http_server_info.host_addr));
	http_server_info.host_addr.sin_family = AF_INET;
	http_server_info.host_addr.sin_port = htons(http_server_info.server_port);
	http_server_info.host_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	http_server_info.sockaddr_len = sizeof(http_server_info.host_addr);

	//socket setup
	logger(DEBUG,"\n socket created");
	http_server_info.server_socket_fd = socket(AF_INET,SOCK_STREAM,0);        
	if(http_server_info.server_socket_fd == -1){
			logger(ERROR,"\n Error in socket creation");
			exit(EXIT_FAILURE);
	}        

	//bind
	logger(DEBUG,"\n binding");
	retval = bind(http_server_info.server_socket_fd,
				 (struct sockaddr*) &(http_server_info.host_addr),
				 sizeof(http_server_info.host_addr));
	if(retval == -1){
			logger(ERROR,"\n Error in binding");
			exit(EXIT_FAILURE);              
	}  
	return retval;
}

int handle_request_response(int s_read_fd){
        int retval = 0, s_write_fd;

        s_write_fd = dup(s_read_fd);
        logger(DEBUG,"\n accepted....");
        
        if(s_write_fd == -1){
                logger(ERROR,"write fd error");
                exit(-1);
        }

        http_request_t *request = malloc(sizeof(http_request_t));
        if(request == NULL){
                logger(ERROR,"request error");
        }
        http_response_t *response = malloc(sizeof(http_response_t));        
        if(response == NULL){
                logger(ERROR,"request error");
        }
        logger(DEBUG,"next_req");
        record_req_received_time();
        retval = next_request(s_read_fd,request);
        logger(DEBUG,"retval_req %d",retval);        
        response->status.code = retval;        
        logger(DEBUG,"build_resp");
        retval = build_response(request,response);
        logger(DEBUG,"send_resp %d",retval);
        retval = send_response(s_write_fd,response);
        record_req_end_time();
        logger(DEBUG,"exiting...");
        free(request);
        free(response);
        logger(DEBUG,"freed req resp...");
        return 0;
}

void print_usage(){
		logger(ERROR,"Invalid use of parameters\n");
        //logger(ERROR,"Usage: ./csuc_http -p port -f|-t|-w -q -d root_dir\n");
}

b_buffer *job_request_buffer = NULL;
void init_thread_pool_strategy(int queue_size,int thread_pool_size){
		int thread_num,
			retval = 0;
		thread_pool *job_request_thread_pool = (thread_pool*)malloc(sizeof(thread_pool));
		
		job_request_buffer = (b_buffer*)malloc(sizeof(b_buffer));
		
		if(job_request_buffer == NULL){
			logger(DEBUG,"\nMalloc error for b_buffer in init_thread_pool_strategy");
			exit(EXIT_FAILURE);
		}		
		
		retval = init_b_buffer(job_request_buffer,queue_size);
		if(retval != 0){
			logger(DEBUG,"\n error in init_b_buffer");
			//return -1;
		}
		init_thread_pool(job_request_thread_pool,thread_pool_size);
	
		logger(DEBUG,"\ninitialized pool and buffer");
		logger(DEBUG,"\nthread_pool_size::%d",thread_pool_size);
		//create threads in threads pool
		for(thread_num = 0;thread_num < thread_pool_size; thread_num++){
			retval = pthread_create(&job_request_thread_pool->thread_pool_pointer[thread_num],
								    NULL,
									*job_request_consumer_thread, 
									(void *)job_request_buffer);				
			pthread_detach(job_request_thread_pool->thread_pool_pointer[thread_num]);
		}	
		//free(job_request_thread_pool);			
}

void* job_request_consumer_thread(void *job_request_buffer){
	sigset_t block_sig_set;
	set_blocking_signals(&block_sig_set);
	pthread_sigmask(SIG_BLOCK,&block_sig_set,NULL);
	logger(DEBUG,"\ngoing to consumer....");
	job_request_consumer((b_buffer*)job_request_buffer);
}


int run_thread_pool_strategy(int s_read_fd){
	int retval = 0;
	logger(DEBUG,"\ngoing to producer....");
	job_request_producer(job_request_buffer,s_read_fd);
	return retval;
}

void init_thread_pool(thread_pool *job_request_thread_pool,int thread_pool_size){
	job_request_thread_pool->thread_pool_size    = thread_pool_size;
	job_request_thread_pool->thread_pool_pointer = (pthread_t *)malloc(sizeof(pthread_t) 
													* thread_pool_size);
}

int register_signals(){
	if(signal(SIGINT,signal_handler) == SIG_ERR){
		logger(ERROR,"\n error in sigint");
		exit(-1);
	}
	if(signal(SIGTERM,signal_handler) == SIG_ERR){
		logger(ERROR,"\n error in sigterm");
		exit(-1);
	}
	if(signal(SIGUSR1,signal_handler) == SIG_ERR){
		logger(ERROR,"\n error in sigusr1");
		exit(-1);
	}
	if(signal(SIGUSR2,signal_handler) == SIG_ERR){
		logger(ERROR,"\n error in sigusr2");
		exit(-1);
	}
}

void signal_handler(int signal){
	int new_log_level;
	switch(signal){
		case SIGINT:
			exit_web_server();
		break;
		case SIGTERM:
			exit_web_server();
		break;
		case SIGUSR1:			
			get_server_stats();
		break;
		case SIGUSR2:			
			new_log_level = change_log_level();
			http_server_info.log_level = new_log_level;
		break;
	}
}

void exit_web_server(){
	logger(DEBUG,"\nexiting web server");
	if(job_request_buffer != NULL){
		free(job_request_buffer);
	}
	close(http_server_info.server_socket_fd);
	exit(EXIT_SUCCESS);
}

void set_blocking_signals(sigset_t *block_sig_set){
	sigemptyset(block_sig_set);
	sigaddset(block_sig_set,SIGINT);
	sigaddset(block_sig_set,SIGTERM);
}
