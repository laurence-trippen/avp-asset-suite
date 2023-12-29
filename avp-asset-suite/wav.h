#pragma once

#include <Windows.h>

typedef struct pwavchunkheader
{
	char chunkName[4];
	int chunkLength;
} PWAVCHUNKHEADER;

typedef struct pwavriffheader
{
	char type[4];
} PWAVRIFFHEADER;

typedef struct tWAVEFORMATEX
{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
    /* extra information (after cbSize) */
} WAVEFORMATEX, * PWAVEFORMATEX, NEAR* NPWAVEFORMATEX, FAR* LPWAVEFORMATEX;
