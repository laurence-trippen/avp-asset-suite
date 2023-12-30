#include "avp_checker.h"

#include "avp_io.h"
#include "avp_rif.h"
#include "avp_linked_list.h"

#include <stdio.h>

AVP_error_t AVP_check_rif_headers(const char* rif_folder_path)
{
	AVP_linkedlist* result_list = AVP_create_linkedlist();
	if (result_list == NULL) return GENERAL_ERROR;


	AVP_io_result_t result = AVP_io_listfiles(rif_folder_path, "*.rif", result_list);
	AVP_linkedlist_print(result_list->head, &AVP_linkedlist_str_printer);


	uint32_t count = AVP_linkedlist_count(result_list);
	printf("%d\n", count);


	const char* header = AVP_rif_read_header("C:\\Games\\GOG\\AvP Classic\\avp_rifs\\derelict.rif");
	free(header);


	AVP_free_linkedlist_with_data(result_list);

	return SUCCESS;
}
