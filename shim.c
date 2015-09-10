#define _GNU_SOURCE

void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

struct node_t {
	void* ptr;
	int size;
	struct node_t* next;
	struct node_t* prev;
};

struct node_t* head;
struct node_t* tail;

void (*original_free)(void *ptr) = NULL;
void* (*original_malloc)(size_t size);

void init() {
	original_free = dlsym(RTLD_NEXT, "free");
	original_malloc = dlsym(RTLD_NEXT, "malloc");
	
	head = original_malloc(sizeof(struct node_t));
	tail = original_malloc(sizeof(struct node_t));
	
	head->ptr = NULL;
	head->size = 0;
	head->prev = NULL;
	head->next = tail;
	
	tail->ptr = NULL;
	tail->size = 0;
	tail->prev = head;
	tail->next = NULL;
}

void cleanup() {
printf("CLEANUP\n");
	struct node_t* nodePtr = head->next;
	int sum = 0;
	int count = 0;
	
	while (nodePtr->next != NULL) {
		sum += nodePtr->size;
		count++;
		fprintf(stderr, "LEAK\t%d\n", nodePtr->size);
		nodePtr = nodePtr->next;
	}
	
	fprintf(stderr,"TOTAL\t%d\t%d\n", count, sum);
}

void free(void *ptr) {
	struct node_t* nodePtr = head->next;
	int exit = 0;
	
	while (nodePtr->next != NULL && exit == 0) {
		if (nodePtr->ptr == ptr) {
			nodePtr->prev->next = nodePtr->next;
			nodePtr->next->prev = nodePtr->prev;
			original_free(nodePtr);
			exit = 1;
		}	
		nodePtr = nodePtr->next;
	}
	original_free(ptr);
}

void* malloc(size_t size) {
	void* ptr = original_malloc(size);
	
	struct node_t* node = original_malloc(sizeof(struct node_t));
	node->size = (int)size;
	node->ptr = ptr;
	
	node->prev = tail->prev;
	node->next = tail;
	tail->prev->next = node;
	tail->prev = node;
	return ptr;
}
