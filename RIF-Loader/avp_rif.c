#include "avp_rif.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
Reads first 8 bytes of .rif file (header).

@returns const char* of 9 bytes. Must be freed manually!
*/
const char* AVP_rif_read_header(const char* rif_path)
{
	FILE* rif = fopen(rif_path, "rb");
	if (rif == NULL) return NULL;

	const char* header = (char*)malloc(sizeof(char) * 9);
	if (header == NULL) return NULL;
	memset(header, '\0', 9);

	if (!fread(header, sizeof(char), 8, rif))
	{
		// Error
		return;
	}

	fclose(rif);
	rif = NULL;

	return header;
}
