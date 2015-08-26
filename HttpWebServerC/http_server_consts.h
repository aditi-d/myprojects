#ifndef H_HTTP_CONSTS
#define H_HTTP_CONSTS

#define MAX_BUFFER_SIZE 2048
#define TEMP_BUFFER_SIZE 16
#define JOB_QUEUE_SIZE 16
#define THREAD_POOL_SIZE 10
#define DEFAULT_LOG_LEVEL 1
#define FILE_NOT_IMPL "/not_implemented.html"
#define FILE_FORBID "/forbidden.html"
#define FILE_NOT_FOUND "/404.html"
#define FILE_NOT_FOUND_400 "/400.html"
#define FILE_NOT_FOUND_ERROR "./errors/404.html"
#define FILE_INT_SERVER_ERR "/int_server_err.html"
#define FILE_INDEX "index.html"
#define DEFAULT_DIR "."
#define OPTARGS "p:ftd:w:q:v:"
#define DEFAULT_PORT 9000
#define STRATEGY_THREAD 't'
#define STRATEGY_THREAD_POOL 'w'
#define STRATEGY_FORK 'f'
#define STRATEGY_SERIAL 's'
#define OPT_PORT 'p'
#define OPT_ROOT_DIR 'd'
#define OPT_QUEUE_SIZE 'q'
#define OPT_LOG_LEVEL 'v'
#define DEFAULT_ARG_CASE '?'

#endif
