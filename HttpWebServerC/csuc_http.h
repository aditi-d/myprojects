/*
  HTTP 1.1 Spec - http://www.w3.org/Protocols/rfc2616/rfc2616.html
*/

#ifndef _CSUC_HTTP_H
#define _CSUC_HTTP_H

#include <limits.h>

/*
 * Useful Constants
 */
#define URI_MAX                 PATH_MAX
#define MAX_HEADER_NAME_LENGTH  256
#define MAX_HEADER_VALUE_LENGTH 768
#define MAX_HEADER_LINE_LENGTH  1024
#define MAX_HEADERS             64

typedef struct {
    char field_name[MAX_HEADER_NAME_LENGTH];
    char field_value[MAX_HEADER_VALUE_LENGTH];
} http_header_t;


#endif
