#ifndef AVP_HUFFMAN
#define AVP_HUFFMAN


#define MAX_DEPTH 11
#define EDXMASK ((((1 << (MAX_DEPTH + 1)) - 1) ^ 1) ^ -1)


typedef struct
{
    char			Identifier[8];
    int				CompressedDataSize;
    int				UncompressedDataSize;
    int				CodelengthCount[MAX_DEPTH];
    unsigned char	ByteAssignment[256];
} HuffmanPackage;


char* HuffmanDecompress(const HuffmanPackage* inpackage);


#endif
