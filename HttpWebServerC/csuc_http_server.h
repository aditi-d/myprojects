#ifndef H_HTTP_SERVER
#define H_HTTP_SERVER

#include "csuc_http.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "logger.h"

typedef struct{
	char root_dir[URI_MAX];
	int server_port;	
	int server_socket_fd;
	int sockaddr_len;
	char strategy;
	log_level_t log_level;
	struct sockaddr_in host_addr; 	     
}server_info;

server_info http_server_info;

//server functions
int run_web_server();
int run_with_strategy(char strategy);
int init_web_server(int argc, char** argv);
#endif
