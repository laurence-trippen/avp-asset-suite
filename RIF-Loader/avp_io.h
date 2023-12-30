#ifndef AVP_IO_H
#define AVP_IO_H

#include "avp_linked_list.h"


typedef enum {
	AVP_IO_SUCCESS,
	AVP_IO_INVALID_FILE
} AVP_io_result_t;


typedef struct {
	char name[260];
} AVP_io_file;


AVP_io_result_t AVP_io_listfiles(
	const char* dir_path, 
	const char* filter,
	AVP_linkedlist* result_list);

#endif
