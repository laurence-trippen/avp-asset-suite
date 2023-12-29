#include "avp_io.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdint.h>


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


AVP_io_result_t AVP_io_listfiles(const char* dir_path)
{
	return AVP_IO_SUCCESS;
}
