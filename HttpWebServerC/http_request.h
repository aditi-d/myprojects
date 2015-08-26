#ifndef H_REQUEST
#define H_REQUEST

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "csuc_http.h"
#include "http_request.h"
#include "http_server_consts.h"
#include "logger.h"

/*
 * HTTP METHODS
 */
typedef enum  {
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_GET,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_CONNECT
} http_method_t;

typedef struct {
    int            method;
    char           uri[URI_MAX];
    int            major_version;
    int            minor_version;
    http_header_t  headers[MAX_HEADERS];
    int            header_count;
} http_request_t;


int next_request(int fd, http_request_t *request);
int assign_method(const char *);
#endif
