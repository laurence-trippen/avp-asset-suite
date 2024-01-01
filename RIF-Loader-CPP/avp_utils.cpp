#include "avp_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


char* const AVP_Format_Bytes(long bytes, int decimals)
{
	const int MAX_STRING_LENGTH = 32;
	const int BASE = (int)pow(10, decimals);

	if (decimals <= 0) return NULL;

	if (bytes <= 0)
	{
		char* const output_string = (char*)malloc(sizeof(char) * 4);
		if (output_string == NULL) return NULL;

		strncpy(output_string, "0 B", 4);
		return output_string;
	}

	double bytesFP = (double)bytes;

	int i = (int)(floor(log(bytesFP) / log(1024.0)));

	double fractional_file_size = bytesFP / pow(1024, i);

	char* const output_string = (char*)malloc(sizeof(char) * MAX_STRING_LENGTH);
	if (output_string == NULL) return NULL;

	// Set first char to \0
	*output_string = '\0';

	// Why ugly switch-case? Cause it's fasssst!
	switch (i)
	{
	case 0:
		sprintf(output_string, "%.2f B", fractional_file_size);
		break;
	case 1:
		sprintf(output_string, "%.2f KB", fractional_file_size);
		break;
	case 2:
		sprintf(output_string, "%.2f MB", fractional_file_size);
		break;
	case 3:
		sprintf(output_string, "%.2f GB", fractional_file_size);
		break;
	case 4:
		sprintf(output_string, "%.2f TB", fractional_file_size);
		break;
	case 5:
		sprintf(output_string, "%.2f PB", fractional_file_size);
		break;
	case 6:
		sprintf(output_string, "%.2f EB", fractional_file_size);
		break;
	case 7:
		sprintf(output_string, "%.2f ZB", fractional_file_size);
		break;
	case 8:
		sprintf(output_string, "%.2f YB", fractional_file_size);
		break;
	}

	return output_string;
}
