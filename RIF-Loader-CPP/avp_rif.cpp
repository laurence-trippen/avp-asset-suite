#include "avp_rif.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avp_utils.h"

void AVP_Rif_Load(const char* path)
{
	int error_code = 0;
	bool rif_is_compressed = false;


	FILE* rif_file = fopen(path, "rb");
	if (rif_file == NULL)
	{
		error_code = AVP_CHUNK_FAILED_ON_LOAD;
		return;
	}


	// Get file size
	fseek(rif_file, 0, SEEK_END);
	long file_size = ftell(rif_file);
	rewind(rif_file);

	// Print file size
	char* const formatted_file_size = AVP_Format_Bytes(file_size, 2);
	printf("%s\n", formatted_file_size);
	free(formatted_file_size);


	// Get ID Header
	char id_buffer[9] = { '\0' };
	if (fread(id_buffer, 1, 8, rif_file) != 8)
	{
		error_code = AVP_CHUNK_FAILED_ON_LOAD;
		fclose(rif_file);
		return;
	}
	

	// Check ID Header for compression
	if (strncmp(id_buffer, AVP_COMPRESSED_RIF_IDENTIFIER, 8) == 0)
	{
		rif_is_compressed = true;
	}
	else if (strncmp(id_buffer, AVP_UNRECOGNIZED_RIF_IDENTIFIER, 8) != 0)
	{
		error_code = AVP_CHUNK_FAILED_ON_LOAD_NOT_RECOGNISED;
		fclose(rif_file);
		return;
	}

	
	printf("RIF %s\n", rif_is_compressed ? "is compressed" : "isn't compressed");


	fclose(rif_file);
	rif_file = NULL;
}
