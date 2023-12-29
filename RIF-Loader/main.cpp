#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "avp_io.h"
#include "avp_linked_list.h"


void linked_list_int_printer(void* data)
{
    printf("Printer: %d\n", *((int*)data));
}


int main(void)
{
    const char rif_folder[] = "C:\\Games\\GOG\\AvP Classic\\avp_rifs\\*.rif";

    size_t length = strlen(rif_folder);

    printf("%s\n", rif_folder);

    AVP_io_result_t result = AVP_io_listfiles(rif_folder);

    
    AVP_linkedlist* list = AVP_create_linkedlist();

    if (list == NULL) return EXIT_FAILURE;

    int a = 117;
    int b = 223;
    int c = 99;

    AVP_linkedlist_append(list, &a);
    AVP_linkedlist_append(list, &b);
    AVP_linkedlist_append(list, &c);

    AVP_linkedlist_print(list->head, &linked_list_int_printer);

    AVP_free_linkedlist(list);
    
    return EXIT_SUCCESS;
}
