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


void linked_list_str_printer(void* data)
{
    printf("Printer: %s\n", (char*)data);
}


int main(void)
{
    const char rif_folder[] = "C:\\Games\\GOG\\AvP Classic\\avp_rifs\\*.rif";

    size_t length = strlen(rif_folder);

    printf("%s\n", rif_folder);

    AVP_linkedlist* result_list = AVP_create_linkedlist();

    if (result_list == NULL) return EXIT_FAILURE;

    AVP_io_result_t result = AVP_io_listfiles(rif_folder, result_list);

    AVP_linkedlist_print(result_list->head, &linked_list_str_printer);

    uint32_t count = AVP_linkedlist_count(result_list);

    printf("%d\n", count);

    AVP_free_linkedlist_with_data(result_list);
    
    return EXIT_SUCCESS;
}
