#include "avp_io.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>


AVP_io_result_t AVP_io_count_files(
	const char* dir_path, 
	const char* filter,
	uint32_t* result_count)
{
	WIN32_FIND_DATAA find_data;

	HANDLE h_find = FindFirstFileA(dir_path, &find_data);

	if (h_find == INVALID_HANDLE_VALUE) return AVP_IO_INVALID_FILE;

	*result_count = 0;

	do
	{
		(*result_count)++;
	} while (FindNextFileA(h_find, &find_data) != 0);

	FindClose(h_find);

	return AVP_IO_SUCCESS;
}


AVP_io_result_t AVP_io_listfiles(const char* dir_path, AVP_linkedlist* result_list)
{
	WIN32_FIND_DATAA find_data;

	HANDLE h_find = FindFirstFileA(dir_path, &find_data);

	if (h_find == INVALID_HANDLE_VALUE) return AVP_IO_INVALID_FILE;

	do
	{
		const int PATH_LENGTH = 260;

		// Must be freed with linked list
		char* file_name = (char*)malloc(sizeof(char) * PATH_LENGTH);

		if (file_name == NULL) return AVP_IO_INVALID_FILE;

		strcpy_s(file_name, PATH_LENGTH, find_data.cFileName);

		AVP_linkedlist_append(result_list, file_name);
	} while (FindNextFileA(h_find, &find_data) != 0);

	FindClose(h_find);

	return AVP_IO_SUCCESS;
}
