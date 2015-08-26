#ifndef H_SERVER_STATUS
#define H_SERVER_STATUS

//might change
#include "csuc_http_server.h"

void get_server_stats();
int init_stats();
void add_data_transferred();
void record_req_received_time();
void record_req_end_time();
#endif
