#include "avp_huffman.h"

#include <stdio.h>
#include <stdlib.h>


static int DecodeTable[1 << MAX_DEPTH]; // 2048


static void MakeHuffmanDecodeTable(const int* depth, int depthmax, const unsigned char* list)
{
    int thisdepth, depthbit, repcount, repspace, lenbits, temp, count;
    int* outp;
    int o = 0;
    const unsigned char* p;
    int* outtbl = DecodeTable;

    lenbits = 0;
    repcount = 1 << depthmax;
    repspace = 1;
    thisdepth = 0;
    depthbit = 4;
    p = list + 255; // End of list pointer

    // Endless until return statement
    while (1)
    {
        // Count as long as there are zeros
        do
        {
            lenbits++;      // 0, 1, 2, 3, etc.    
            depthbit <<= 1; // 4, 8, 16 etc.
            repspace <<= 1; // 1, 2, 4 etc.
            repcount >>= 1; // 2048, 1024, 512, etc.
        } while (!(thisdepth = *depth++));

        do
        {
            if (p < list)
            {
                temp = 0xff; // 255
            }
            else
            {
                temp = lenbits | (*p-- << 8);
            }

            outp = outtbl + (o >> 2);
            count = repcount;

            do
            {
                *outp = temp;
                outp += repspace;
            } while (--count);

            temp = depthbit;

            do
            {
                temp >>= 1;
                if (temp & 3) return;
                o ^= temp;
            } while (!(o & temp));
        } while (--thisdepth);
    }
}


static int HuffmanDecode(unsigned char* dest, const int* source, const int* table, int length)
{
    unsigned char* start;
    int                    available, reserve, fill, wid;
    unsigned int           bits = 0, resbits;
    const unsigned char* p;

    start = dest;
    available = 0;
    reserve = 0;
    wid = 0;
    resbits = 0;
    do
    {
        available += wid;
        fill = 31 - available;
        bits <<= fill;
        if (fill > reserve)
        {
            fill -= reserve;
            available += reserve;
            if (reserve)
            {
                bits = (bits >> reserve) | (resbits << (32 - reserve));
            }
            resbits = *source++;
            reserve = 32;
        }
        bits = (bits >> fill) | (resbits << (32 - fill));
        resbits >>= fill;
        reserve -= fill;
        available = 31;
        goto lpent;
        do
        {
            bits >>= wid;
            *dest++ = p[1];
        lpent:      p = (const unsigned char*)(((const short*)table) + (bits & ~EDXMASK));
        } while ((available -= (wid = *p)) >= 0 && (dest - start) != length);

    } while (available > -32 && (dest - start) != length);
    return (int)(dest - start);
}


char* HuffmanDecompress(const HuffmanPackage* inpackage)
{
    printf("Max Depth: %d\n", 1 << 11);

	unsigned char* uncompressedData = NULL;

	// Step 1: Make the decoding table
	MakeHuffmanDecodeTable(inpackage->CodelengthCount, MAX_DEPTH, inpackage->ByteAssignment);

	// Step 2: Decode data
	uncompressedData = (unsigned char*) malloc(inpackage->UncompressedDataSize + 16);
	if (uncompressedData)
	{
		HuffmanDecode(uncompressedData, (int*)(inpackage + 1), DecodeTable, inpackage->UncompressedDataSize);
	}

	return (char*)uncompressedData;
}
