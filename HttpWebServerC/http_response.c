#include "http_response.h"
#include "server_stats.h"

const http_status_t HTTP_STATUS_LOOKUP[] = {
    // Informational Status Codes
    // Request received, continuing process
    {100, "Continue"},
    {101, "Switching Protocols"},

    // Success Status Codes
    // The action was successfully received, understood, and accepted
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},

    // Redirection Status Codes
    // Further action must be taken in order to complete the request
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {307, "Temporary Redirect"},

    // Client Error Status Codes
    // The request contains bad syntax or cannot be fulfilled
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Time-out"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Request Entity Too Large"},
    {414, "Request-URI Too Large"},
    {415, "Unsupported Media Type"},
    {416, "Requested range not satisfiable"},
    {417, "Expectation Failed"},

    // Server Error Status Codes
    // The server failed to fulfill an apparently valid request
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Time-out"},
    {505, "HTTP Version not supported"}
};

//accepts the request and creates the response
int build_response(const http_request_t *request, http_response_t *response){
        int ret_val,status_code,header_cntr;        
        struct stat stat_data;

        logger(DEBUG,"\n inside build_response");
        response->major_version = request->major_version;
        response->minor_version = request->minor_version;
        response->header_count = request->header_count;

        //prepend the root path to the uri
        strcpy(response->resource_path,http_server_info.root_dir);
        logger(DEBUG,"root_dir %s\n",http_server_info.root_dir);
        logger(DEBUG,"request uri %s\n",request->uri);
        strcat(response->resource_path,request->uri);        
        logger(DEBUG,"req->res_path %s",response->resource_path);
        
        //separate document root from request uri
                                  
        ret_val = stat(response->resource_path,&stat_data);
		logger(DEBUG,"retval ",stat_data.st_mode & S_IFMT,S_IFDIR);				
        logger(DEBUG,"stat::%d %d ",stat_data.st_mode & S_IFMT,S_IFDIR);

        //set response status
        if(response->status.code == HTTP_STATUS_INTERNAL_SERVER_ERROR){
                status_code = HTTP_STATUS_INTERNAL_SERVER_ERROR;
                strcpy(response->resource_path,http_server_info.root_dir);
                strcat(response->resource_path,FILE_INT_SERVER_ERR);
        }
        else if(request->method == -1){
                status_code = HTTP_STATUS_NOT_IMPLEMENTED;
                strcpy(response->resource_path,http_server_info.root_dir);
                strcat(response->resource_path,FILE_NOT_IMPL);
        }
        else if((ret_val == -1 && errno == EACCES) || ((S_IRUSR & stat_data.st_mode) == 1)){        
                status_code = HTTP_STATUS_FORBIDDEN;
                strcpy(response->resource_path,http_server_info.root_dir);
                strcat(response->resource_path,FILE_FORBID);                        
        }
        else if((stat_data.st_mode & S_IFMT) == S_IFDIR){
                status_code = HTTP_STATUS_OK;
                strcpy(response->resource_path,http_server_info.root_dir);
                strcat(response->resource_path,request->uri);                
                strcat(response->resource_path,FILE_INDEX);
                logger(DEBUG,"\nresponse_path::%s %s %s \n",response->resource_path,http_server_info.root_dir,FILE_INDEX);
                ret_val = 0;
                
                //stat to check if index is present
                 ret_val = stat(response->resource_path,&stat_data);
                if(ret_val == -1 && errno == ENOENT){
					status_code = HTTP_STATUS_NOT_FOUND;
					strcpy(response->resource_path,http_server_info.root_dir);
					strcat(response->resource_path,FILE_NOT_FOUND);
					ret_val = 0;
					ret_val = stat(response->resource_path,&stat_data);        
					if(ret_val == -1 && errno == ENOENT){
							strcpy(response->resource_path,http_server_info.root_dir);
							strcat(response->resource_path,FILE_NOT_FOUND_400);
							ret_val = 0;
							ret_val = stat(response->resource_path,&stat_data);        
							if(ret_val == -1 && errno == ENOENT){
								strcpy(response->resource_path,FILE_NOT_FOUND_ERROR);
								//strcat(response->resource_path,FILE_NOT_FOUND_ERROR);
							}
					}
				}
        }
        else if(ret_val == -1 && errno == ENOENT){
                status_code = HTTP_STATUS_NOT_FOUND;
                strcpy(response->resource_path,http_server_info.root_dir);
                strcat(response->resource_path,FILE_NOT_FOUND);
                ret_val = 0;
                ret_val = stat(response->resource_path,&stat_data);        
                if(ret_val == -1 && errno == ENOENT){
                        strcpy(response->resource_path,http_server_info.root_dir);
                        strcat(response->resource_path,FILE_NOT_FOUND_400);
                        ret_val = 0;
                        ret_val = stat(response->resource_path,&stat_data);        
						if(ret_val == -1 && errno == ENOENT){
							strcpy(response->resource_path,FILE_NOT_FOUND_ERROR);
						}
                }
        }
        else{
                status_code = HTTP_STATUS_OK;
                logger(DEBUG,"\nstatus ok");
        }
        
        response->status.code = HTTP_STATUS_LOOKUP[status_code].code;
        response->status.reason = HTTP_STATUS_LOOKUP[status_code].reason;
        logger(DEBUG,"\nresponse_path::%s %s %s\n",response->resource_path,http_server_info.root_dir,FILE_INDEX);
        create_response_header(response);
        return 0;
}

int create_response_header(http_response_t *response){
        logger(DEBUG, "Create Response Header\n");
        char *content_type = NULL,
         *buffer = NULL,
         *saveptr=NULL,
         *extension = NULL;
        ssize_t file_size = 0;
        int count;
        FILE* f_resource = NULL;
        time_t t_now=time(0);
        struct tm *gmt_tm = gmtime(&t_now);
        char *dt = asctime(gmt_tm);        
        
        response->header_count=0;
        
        //get the extension from the resource path
        char *temp_ext = malloc(sizeof(char)*strlen(response->resource_path) + 1);
        strcpy(temp_ext,response->resource_path);
        strtok_r(temp_ext,".",&saveptr);
        extension = strtok_r(NULL,"",&saveptr);
        saveptr=NULL;
             
        strcpy(response->headers[0].field_name,"Date");
        strtok_r(dt,"\n",&saveptr);
        strcat(dt," GMT");
        strcpy(response->headers[0].field_value,dt);
        response->header_count++;
        saveptr=NULL;
        
        strcpy(response->headers[1].field_name,"Server");
        strcpy(response->headers[1].field_value,"DZ server");
        response->header_count++;
                //fprintf(stderr, "RH 4\n");
        
        
        if(strcmp("html",extension) == 0 || strcmp("css",extension) == 0){        
                content_type=malloc(strlen(extension)+strlen("text/") + 1);
				logger(DEBUG, "Extension is: |%s|\n", extension);
                strcpy(content_type,"text/");
                strcat(content_type,extension);
        }
        else if(strcmp("jpeg",extension) == 0 || strcmp("png",extension) == 0 || strcmp("jpg",extension)==0){
                content_type = malloc(strlen(extension)+strlen("image/") + 1);
                strcpy(content_type,"image/");
                strcat(content_type,extension);
        }
        else if(strcmp("js",extension) == 0 || strcmp("xml",extension) == 0){
                content_type = malloc(strlen(extension)+strlen("application/") + 1);
                strcpy(content_type,"application/");
                strcat(content_type,extension);
        }
        else if(strcmp("mp3",extension) == 0){
                content_type = malloc(strlen(extension)+strlen("audio/") + 1);
                strcpy(content_type,"audio/");
                strcat(content_type,extension);
        }
        else if(strcmp("mpeg",extension) == 0 || strcmp("mpg",extension) == 0
                        || strcmp("mp4",extension) == 0 || strcmp("mov",extension) == 0){
                content_type = malloc(strlen(extension)+strlen("video/") + 1);
                strcpy(content_type,"video/");
                strcat(content_type,extension);
        }

        strcpy(response->headers[2].field_name,"Content-Type");
        strcpy(response->headers[2].field_value,content_type);        
        response->header_count++;
        logger(DEBUG, "Before fopen\n");
        f_resource = fopen(response->resource_path,"r");
        if(f_resource == NULL){
                file_size=0;        
                return -1;        
        }
        fseek(f_resource,0,SEEK_END);
        file_size = ftell(f_resource);
        logger(DEBUG, "After file size\n");
    
        strcpy(response->headers[3].field_name,"Content-Length");
        snprintf(response->headers[3].field_value,MAX_HEADER_VALUE_LENGTH,"%d",(int)file_size);
        response->header_count++;		
        fclose(f_resource);		
		free(content_type);
        return 0;        
}

//send the response
int send_response(int fd, const http_response_t *response){
        
        char buff_to_send[MAX_BUFFER_SIZE];
        ssize_t bytes_read = 0;
        int bytes_sent = 0;
        int count=0,cntr = 0,content_length=0;
        FILE *f_ptr = NULL,*f_resource=NULL;
        
        //open the output stream
        f_ptr = fdopen(fd,"w");
        if(f_ptr == NULL){
                perror(NULL);
        }

        bytes_read = sprintf(buff_to_send,"HTTP/%d.%d %d %s",
                         response->major_version,response->minor_version,
                         response->status.code,response ->status.reason);
        fprintf(f_ptr,"%s",buff_to_send);
        
        for(cntr=0;cntr<response->header_count;cntr++){
                bytes_read = sprintf(buff_to_send,"\n%s:%s",
                                                response->headers[cntr].field_name,
                                                response->headers[cntr].field_value);
                fprintf(f_ptr,"%s",buff_to_send);
        }        

        bytes_read = bytes_read + sprintf(buff_to_send + bytes_read, "\n\n");
        
        fwrite(&buff_to_send,sizeof(char),bytes_read,f_ptr);

		add_data_transferred((int)bytes_read);

        //send the contents of the file.
        f_resource = fopen(response->resource_path,"r");
        if(f_resource == NULL){        
                return -1;        
        }
        
        while((count = fread(&buff_to_send,sizeof(char),MAX_BUFFER_SIZE,f_resource)) != 0){                
                bytes_sent = bytes_sent + (int)count;
                fwrite(&buff_to_send,sizeof(char),count,f_ptr);
        }
        add_data_transferred(bytes_sent);
        fclose(f_resource);
        fclose(f_ptr);        
        return 0;        
}
