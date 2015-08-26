#include "http_request.h"

int next_request(int fd, http_request_t *request){
        char version[TEMP_BUFFER_SIZE],
         method[TEMP_BUFFER_SIZE],
         *http_request 	= NULL,
         *temp 			= NULL,
         *end 			= NULL,
         *saveptr 		= NULL,
         *hash_ptr 		= NULL,
         *q_ptr 		= NULL;
         
        int header_cntr    = 0,
			char_cntr 	   = 0;
        size_t buffer_size = 0;
		ssize_t input_read = 0;

        logger(DEBUG,"\n inside next_req");

        //open the input stream
        FILE *f_ptr = fdopen(fd,"r");
        if(f_ptr == NULL){
                logger(ERROR,"Error in opening file");             
                exit(EXIT_FAILURE);
        }
        logger(DEBUG,"\n fdopen");


        //read the first line of the input
        input_read = getline(&http_request,&buffer_size,f_ptr);
    
        if(input_read == -1){
                logger(ERROR,"Error in input read");                
                exit(EXIT_FAILURE);
        }
        logger(DEBUG,"\n just read");

        //parse the first line of the input i.e. method, uri and the version
        sscanf(http_request,"%s %s %s",method,request->uri,version);        
        free(http_request);
        http_request = NULL;
        logger(DEBUG,"\n parsed first line");

        //ignore anything after # or ?
        hash_ptr = strchr(request->uri,'#');
        q_ptr = strchr(request->uri,'?');
        if(hash_ptr){
                *hash_ptr = '\0';
        }
        if(q_ptr){
                *q_ptr = '\0';
        }
        logger(DEBUG,"\n delete hash and ?");

        request->method = assign_method(method);
        
        temp = strtok_r(version,"/",&saveptr);
        temp = strtok_r(NULL,"",&saveptr);
        saveptr=NULL;
        request->minor_version = strtol(strtok_r(temp,".",&saveptr),&end,10);
        request->major_version = strtol(strtok_r(NULL,"",&saveptr),&end,10);
        temp = NULL;
        saveptr=NULL;

        //accept the headers and parse the headers
        while(header_cntr < MAX_HEADERS){
                input_read = 0;
                input_read = getline(&http_request,&buffer_size,f_ptr);
                if(strcmp("\r\n",http_request) == 0){
                        break;
                }
                sscanf(http_request,"%s %s",request->headers[header_cntr].field_name,
                        request->headers[header_cntr].field_value);

                logger(DEBUG,"\n accept headers");

                //remove the trailing : after separating header name and value
                temp=strtok_r(http_request,":",&saveptr);
                strcpy(request->headers[header_cntr].field_name,temp);                
                temp = NULL;                
                free(http_request);
                http_request = NULL;
                header_cntr++;
        }
        request->header_count = header_cntr;
        fclose(f_ptr);
        logger(DEBUG,"\n exiting next_req");
        return 0;
}

//this method converts the string format of the method to the equivalent int format and
//returns it
int assign_method(const char *req_string){
        int retval;
        if(strcmp("GET",req_string) == 0){
                retval=HTTP_METHOD_GET;
        }
        /*else if(strcmp("OPTIONS",req_string)==0){
                retval=HTTP_METHOD_OPTIONS;
        }
        else if(strcmp("HEAD",req_string)==0){
                retval=HTTP_METHOD_HEAD;
        }
        else if(strcmp("POST",req_string)==0){
                retval=HTTP_METHOD_POST;
        }
        else if(strcmp("PUT",req_string)==0){
                retval=HTTP_METHOD_PUT;
        }
        else if(strcmp("DELETE",req_string)==0){
                retval=HTTP_METHOD_DELETE;
        }
        else if(strcmp("TRACE",req_string)==0){
                retval=HTTP_METHOD_TRACE;
        }
        else if(strcmp("CONNECT",req_string)==0){
                retval=HTTP_METHOD_CONNECT;
        }*/
        else{
                retval = -1;
        }
        return retval;
}
