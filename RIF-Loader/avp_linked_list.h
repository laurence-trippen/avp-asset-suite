#ifndef AVP_LINKEDLIST_H
#define AVP_LINKEDLIST_H


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


#endif