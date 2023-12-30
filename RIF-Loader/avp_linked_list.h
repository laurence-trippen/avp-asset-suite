#ifndef AVP_LINKEDLIST_H
#define AVP_LINKEDLIST_H

#include <stdint.h>


#define AVP_TRAVERSE_LIST(code, head) AVP_linkedlist_node* cursor = head; \
while (cursor != NULL) { \
code \
cursor = cursor->next; \
}



typedef struct AVP_linkedlist_node_s {
	struct AVP_linkedlist_node_s* next;
	void* data;
} AVP_linkedlist_node;


typedef struct {
	AVP_linkedlist_node* head;
} AVP_linkedlist;


AVP_linkedlist* AVP_create_linkedlist(void);

void AVP_free_linkedlist(AVP_linkedlist* list);
void AVP_free_linkedlist_with_data(AVP_linkedlist* list);

AVP_linkedlist_node* AVP_linkedlist_append(AVP_linkedlist* list, void* data);

void AVP_linkedlist_print(
	AVP_linkedlist_node* const node,
	void (*AVP_linkedlist_printer)(void* data));

uint32_t AVP_linkedlist_count(AVP_linkedlist* const list);

void AVP_linkedlist_int_printer(void* data);
void AVP_linkedlist_str_printer(void* data);


#endif
