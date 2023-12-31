#include "avp_linked_list.h"

#include <stdlib.h>

/*
* @returns New linkedlist object or NULL in case of failure.
*/
AVP_linkedlist* AVP_create_linkedlist(void)
{
	AVP_linkedlist* list = (AVP_linkedlist*)malloc(sizeof(AVP_linkedlist));

	if (list == NULL) return NULL;

	list->head = NULL;

	return list;
}


/*
* Frees linked list (nodes) NOT DATA from start to finish.
*/
void AVP_free_linkedlist(AVP_linkedlist* list)
{
	AVP_linkedlist_node* cursor = list->head;

	while (cursor != NULL)
	{
		// Cache next pointer before wiping current cursor...
		AVP_linkedlist_node* next = cursor->next;

		free(cursor);

		cursor = next;
	}

	// Don't forget to free list struct
	free(list);

	list = NULL;
}


/*
* Frees linked list nodes & data.
*
* Caution: If data is on stack or not modifiable then it can cause errors!
*/
void AVP_free_linkedlist_with_data(AVP_linkedlist* list)
{
	AVP_linkedlist_node* cursor = list->head;

	while (cursor != NULL)
	{
		// Cache next pointer before wiping current cursor...
		AVP_linkedlist_node* next = cursor->next;

		free(cursor->data);
		free(cursor);

		cursor = next;
	}

	// Don't forget to free list struct
	free(list);

	list = NULL;
}


AVP_linkedlist_node* AVP_linkedlist_append(AVP_linkedlist* list, void* data)
{
	if (list == NULL) return NULL;

	if (list->head == NULL)
	{
		AVP_linkedlist_node* new_node = (AVP_linkedlist_node*)malloc(sizeof(AVP_linkedlist_node));

		if (new_node == NULL) return NULL;

		new_node->data = data;
		new_node->next = NULL;

		list->head = new_node;

		return new_node;
	}
	else
	{
		AVP_linkedlist_node* cursor_node = list->head;
		AVP_linkedlist_node* last_node = NULL;

		while (cursor_node != NULL)
		{
			last_node = cursor_node;

			cursor_node = cursor_node->next;
		}

		AVP_linkedlist_node* new_node = (AVP_linkedlist_node*)malloc(sizeof(AVP_linkedlist_node));

		if (new_node == NULL) return NULL;

		new_node->data = data;
		new_node->next = NULL;

		last_node->next = new_node;

		return new_node;
	}

	return NULL;
}

void AVP_linkedlist_print(
	AVP_linkedlist_node* const node,
	void (*AVP_linkedlist_printer)(void* data)
) {
	if (node == NULL) return;

	AVP_linkedlist_node* cursor_node = node;

	while (cursor_node != NULL)
	{
		(*AVP_linkedlist_printer)(cursor_node->data);

		cursor_node = cursor_node->next;
	}
}


uint32_t AVP_linkedlist_count(AVP_linkedlist* const list)
{
	if (list == NULL) return 0;
	if (list->head == NULL) return 0;

	uint32_t count = 0;

	AVP_linkedlist_node* cursor_node = list->head;

	while (cursor_node != NULL)
	{
		count++;

		cursor_node = cursor_node->next;
	}

	return count;
}


void AVP_linkedlist_int_printer(void* data)
{
	printf("Printer: %d\n", *((int*)data));
}


void AVP_linkedlist_str_printer(void* data)
{
	printf("Printer: %s\n", (char*)data);
}
