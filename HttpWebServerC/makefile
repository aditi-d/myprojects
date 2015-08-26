csuc_http: main.c logger.o server_stats.o http_request.o http_response.o c_queue.o bounded_buffer.o\
csuc_http_utilities.o csuc_http_server.o csuc_http.h http_server_consts.h
	gcc -o csuc_http main.c logger.o server_stats.o http_request.o http_response.o c_queue.o\
	 bounded_buffer.o csuc_http_utilities.o csuc_http_server.o -I . -pthread
http_request.o: http_request.c http_request.h http_server_consts.h
	gcc -o http_request.o -c http_request.c
http_response.o: http_response.c http_response.h http_server_consts.h
	gcc -o http_response.o -c http_response.c
bounded_buffer.o: bounded_buffer.c bounded_buffer.h c_queue.c c_queue.h
	gcc -o bounded_buffer.o -c bounded_buffer.c
csuc_http_server.o: csuc_http_server.c csuc_http_server.h csuc_http_utilities.h
	gcc -o csuc_http_server.o -c csuc_http_server.c
csuc_http_utilities.o: csuc_http_utilities.c csuc_http_utilities.h csuc_http_server.h
	gcc -o csuc_http_utilities.o -c csuc_http_utilities.c	
c_queue.o: c_queue.c c_queue.h
	gcc -o c_queue.o -c c_queue.c
logger.o: logger.c logger.h
	gcc -o logger.o -c logger.c
server_stats.o: server_stats.c server_stats.h
	gcc -o server_stats.o -c server_stats.c
clean:
	rm -f csuc_http http_request http_response logger csuc_http_utilities bounded_buffer
	csuc_http_server c_queue server_stats.o
