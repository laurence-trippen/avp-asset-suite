#include "avp_rif.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avp_huffman.h"
#include "avp_utils.h"

void AVP_Rif_Load(const char* path)
{
	int error_code = 0;
	bool rif_is_compressed = false;
	char* uncompressedData = NULL;


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


	// File Buffer. Don't forget to free in every path.
	char* file_buffer = (char*) malloc(file_size);
	if (file_buffer == NULL) return;

	if (rif_is_compressed)
	{
		char* pointer_after_header = file_buffer + 8;
		long file_size_without_header = file_size - 8;

		if (fread(pointer_after_header, 1, file_size_without_header, rif_file) != file_size_without_header)
		{
			error_code = AVP_CHUNK_FAILED_ON_LOAD;
			free(file_buffer);
			fclose(rif_file);
			return;
		}

		uncompressedData = HuffmanDecompress((HuffmanPackage*)file_buffer);
		file_size = ((HuffmanPackage*)file_buffer)->UncompressedDataSize;

		char* const uncompressed_size = AVP_Format_Bytes(file_size, 2);
		printf("%s\n", uncompressed_size);
		free(uncompressed_size);
	}
	else
	{

	}

	free(file_buffer);

	fclose(rif_file);
	rif_file = NULL;
}
