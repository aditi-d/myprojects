#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <time.h>
#include "csuc_http.h"
#include "csuc_http_utilities.h"
#include "http_server_consts.h"
#include "bounded_buffer.h"
#include "csuc_http_server.h"

server_params params;

int init_web_server(int argc, char** argv){
		int retval = 0;
				
		//get options
					
		retval = register_signals();
		
		if(retval != 0 ){
			logger(ERROR,"\nError in registering signals");
			exit(EXIT_FAILURE);
		}
		
		retval = init_stats();
		if(retval != 0 ){
			logger(ERROR,"\nError in initializing stats");
			exit(EXIT_FAILURE);
		}
		
		retval = read_opt(argc, argv, &params);	
		if(retval != 0 ){
			logger(ERROR,"\nError in reading options");
			exit(EXIT_FAILURE);
		}			
		
		//init server variables
		init_server_params(params);
    
		init_log(params.p_log_level);
		
		//copy all files into current directory								
    
		//init socket	
		retval = create_server_socket();	
                
        return retval;      		
}


int run_web_server(){
	 return run_with_strategy(http_server_info.strategy);
}

int run_with_strategy(char strategy){       
        int retval;
        
        int (*strategy_fun_ptr)(int);
        
        //listen()
        logger(INFO,"\n listening...");
        retval = listen(http_server_info.server_socket_fd,15);
        if(retval == -1){
                perror(NULL);
                return -1;
        }        
  						
		switch(strategy){
		case STRATEGY_FORK:                                								
			//call strategy_fork()
			strategy_fun_ptr = &run_fork_strategy;						      
		break;
		case STRATEGY_THREAD:			     
			//call strategy_thread()								
			strategy_fun_ptr = &run_thread_strategy;                       
		break;
		case STRATEGY_SERIAL:  
			//call run_serial_strategy()
			strategy_fun_ptr = &run_serial_strategy;
		break;
		case STRATEGY_THREAD_POOL:
			//call run_thread_pool_strategy()	
			init_thread_pool_strategy(params.p_queue_size,
									  params.p_thread_pool_size);
											
			strategy_fun_ptr = &run_thread_pool_strategy;
		break;
		}
						
		while(1){
				int s_read_fd,s_write_fd,retval;
				//accept
				logger(INFO,"\nStarted server...");
				logger(INFO,"\naccepting requests...");
				s_read_fd = accept(http_server_info.server_socket_fd, 
							(struct sockaddr*) &(http_server_info.host_addr), 
							(socklen_t *) &(http_server_info.sockaddr_len));  
				if(s_read_fd == -1){
					perror(NULL);
					exit(EXIT_FAILURE);
				}				
				incr_requests_handled();
				retval = strategy_fun_ptr(s_read_fd); 
		}        
        return 0;
}
