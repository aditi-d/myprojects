#include <stdio.h>
#include "csuc_http_server.h"
#include "logger.h"

int main(int argc, char** argv){
        int retval = 0;
        retval = init_web_server(argc,argv);
        logger(INFO,"Starting server....");
        retval = run_web_server();
        return 0;
}

