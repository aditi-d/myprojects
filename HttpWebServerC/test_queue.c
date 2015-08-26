#include <stdio.h>
#include "c_queue.h"

int main(){
	c_queue_t cq;
	init_cq(&cq,3);
	printf("\n\n----init::%d----\n",is_cq_empty(cq));
	insert_next(&cq,1);
	insert_next(&cq,2);
	insert_next(&cq,3);
	printf("\n\n----full::%d----\n",is_cq_full(cq));
	remove_next(&cq);
	remove_next(&cq);
	remove_next(&cq);
	printf("\n\n----empty::%d-----\n",is_cq_empty(cq));
	return 0;
}

