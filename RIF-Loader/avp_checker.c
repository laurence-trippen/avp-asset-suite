#include "avp_checker.h"

#include "avp_io.h"
#include "avp_rif.h"
#include "avp_linked_list.h"

#include <stdio.h>
#include <string.h>

AVP_error_t AVP_check_rif_headers(const char* rif_folder_path)
{
	AVP_linkedlist* result_list = AVP_create_linkedlist();
	if (result_list == NULL) return GENERAL_ERROR;


	AVP_io_result_t result = AVP_io_listfiles(rif_folder_path, "*.rif", result_list);
	// AVP_linkedlist_print(result_list->head, &AVP_linkedlist_str_printer);


	uint32_t count = AVP_linkedlist_count(result_list);
	printf("RIF File Count: %d\n", count);


	AVP_linkedlist_node* cursor = result_list->head;
	while (cursor != NULL)
	{
		if (cursor->data != NULL)
		{
			char* rif_file_name = (char*)cursor->data;
			// printf("LoL: %s\n", rif_file_name);

			// TODO: Is this efficient?
			// TODO: Path seperator check
			char rif_file_path[260] = { '\0' };
			strcat_s(rif_file_path, sizeof(rif_file_path), rif_folder_path);
			strcat_s(rif_file_path, sizeof(rif_file_path), rif_file_name);

			const char* header = AVP_rif_read_header(rif_file_path);

			int isCompressed = strcmp(header, AVP_COMPRESSED_RIF_IDENTIFIER) == 0;
			char* compressedText = isCompressed ? "Compressed" : "Not Compressed";

			printf("[%s] [%s] %s\n", header, compressedText, rif_file_path);

			free(header);
		}

		cursor = cursor->next;
	}


	AVP_free_linkedlist_with_data(result_list);

	return SUCCESS;
}
