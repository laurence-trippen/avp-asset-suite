#include "ffl.h"

#include "wav.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define VOLUME_DEFAULT	(127)


static void* LoadRebSndFile(char* filename)
{
	void* bufferPtr;
	long int save_pos, size_of_file;
	FILE* fp;
	fp = fopen(filename, "rb");

	if (!fp) goto error;

	save_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	fseek(fp, save_pos, SEEK_SET);

	bufferPtr = malloc(size_of_file);

	if (!fread(bufferPtr, size_of_file, 1, fp))
	{
		fclose(fp);
		free(bufferPtr);
		goto error;
	}

	fclose(fp);
	return bufferPtr;

error:
	{
		return 0;
	}
}


unsigned char* ExtractWavFile(int soundIndex, unsigned char* bufferPtr)
{
	PWAVCHUNKHEADER myChunkHeader;
	PWAVRIFFHEADER myRiffHeader;
	WAVEFORMATEX myWaveFormat;
	unsigned char* endOfBufferPtr;
	int lengthInSeconds;

	{
		int length = strlen(bufferPtr) + 1;
		GameSounds[soundIndex].wavName = (char*)AllocateMem(length);
		strcpy(GameSounds[soundIndex].wavName, bufferPtr);
		bufferPtr += length;
	}

	/* Read the WAV RIFF header */
	RebSndRead(&myChunkHeader, sizeof(PWAVCHUNKHEADER), 1, bufferPtr);
	endOfBufferPtr = bufferPtr + myChunkHeader.chunkLength;

	RebSndRead(&myRiffHeader, sizeof(PWAVRIFFHEADER), 1, bufferPtr);

	/* Read the WAV format chunk */
	RebSndRead(&myChunkHeader, sizeof(PWAVCHUNKHEADER), 1, bufferPtr);
	if (myChunkHeader.chunkLength == 16)
	{
		/* a standard PCM wave format chunk */
		PCMWAVEFORMAT tmpWaveFormat;
		RebSndRead(&tmpWaveFormat, sizeof(PCMWAVEFORMAT), 1, bufferPtr);
		myWaveFormat.wFormatTag = tmpWaveFormat.wf.wFormatTag;
		myWaveFormat.nChannels = tmpWaveFormat.wf.nChannels;
		myWaveFormat.nSamplesPerSec = tmpWaveFormat.wf.nSamplesPerSec;;
		myWaveFormat.nAvgBytesPerSec = tmpWaveFormat.wf.nAvgBytesPerSec;
		myWaveFormat.nBlockAlign = tmpWaveFormat.wf.nBlockAlign;
		myWaveFormat.wBitsPerSample = tmpWaveFormat.wBitsPerSample;
		myWaveFormat.cbSize = 0;
	}
	else if (myChunkHeader.chunkLength == 18)
	{
		/* an extended PCM wave format chunk */
		RebSndRead(&myWaveFormat, sizeof(WAVEFORMATEX), 1, bufferPtr);
		myWaveFormat.cbSize = 0;
	}
	else
	{
		/* uh oh: a different chunk type */
		LOCALASSERT(1 == 0);
		return 0;
	}

	/* Read	the data chunk header */
	//skip chunks until we reach the 'data' chunk
	do
	{
		/* Read	the data chunk header */
		RebSndRead(&myChunkHeader, sizeof(PWAVCHUNKHEADER), 1, bufferPtr);
		if ((myChunkHeader.chunkName[0] == 'd') && (myChunkHeader.chunkName[1] == 'a') &&
			(myChunkHeader.chunkName[2] == 't') && (myChunkHeader.chunkName[3] == 'a'))
		{
			break;
		}
		//skip to next chunk
		bufferPtr += myChunkHeader.chunkLength;
	} while (TRUE);

	/* Now do a few checks */
	if ((myChunkHeader.chunkName[0] != 'd') || (myChunkHeader.chunkName[1] != 'a') ||
		(myChunkHeader.chunkName[2] != 't') || (myChunkHeader.chunkName[3] != 'a'))
	{
		/* chunk alignment disaster */
		LOCALASSERT(1 == 0);
		return 0;
	}

	//calculate length of sample
	lengthInSeconds = DIV_FIXED(myChunkHeader.chunkLength, myWaveFormat.nAvgBytesPerSec);

	if ((myChunkHeader.chunkLength < 0) || (myChunkHeader.chunkLength > SOUND_MAXSIZE))
	{
		LOCALASSERT(1 == 0);
		return 0;
	}
	if (myWaveFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		LOCALASSERT(1 == 0);
		return 0;
	}
	if ((myWaveFormat.nChannels != 1) && (myWaveFormat.nChannels != 2))
	{
		LOCALASSERT(1 == 0);
		return 0;
	}
	if ((myWaveFormat.wBitsPerSample != 8) && (myWaveFormat.wBitsPerSample != 16))
	{
		LOCALASSERT(1 == 0);
		return 0;
	}

	{
		/* Now set the buffer description and make a sound object */
		DSBUFFERDESC dsBuffDesc;
		LPDIRECTSOUNDBUFFER sndBuffer;
		HRESULT hres;
		LPVOID audioPtr1;
		DWORD audioBytes1;
		LPVOID audioPtr2;
		DWORD audioBytes2;

		memset(&dsBuffDesc, 0, sizeof(DSBUFFERDESC));
		dsBuffDesc.dwSize = sizeof(DSBUFFERDESC);
		dsBuffDesc.dwFlags = (DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC);
		dsBuffDesc.dwBufferBytes = myChunkHeader.chunkLength;
		dsBuffDesc.lpwfxFormat = &myWaveFormat;

		/* Do we need to specify 3D. */
		if (SoundConfig.flags & SOUND_3DHW)
		{
			dsBuffDesc.dwFlags |= DSBCAPS_CTRL3D;
		}

		/* If we have no Voice Manager support we must leave some hardware buffers free. */
		if (SoundConfig.flags & SOUND_VOICE_MGER)
		{
			// Do nothing for the moment.
		}
		else
		{
			/* No Voice Manager. */
			DSCAPS caps;
			ZeroMemory(&caps, sizeof(DSCAPS));
			caps.dwSize = sizeof(DSCAPS);

			IDirectSound_GetCaps(DSObject, &caps);
			db_logf5(("caps.dwFreeHwMixingStaticBuffers %i SoundMinBufferFree %i", caps.dwFreeHwMixingStaticBuffers, SoundMinBufferFree));
			if (caps.dwFreeHwMixingStaticBuffers < SoundMinBufferFree)
			{
				/* Force to software. */
				db_log3("Forcing buffer to software.");
				dsBuffDesc.dwFlags |= DSBCAPS_LOCSOFTWARE;
			}
		}

		/* Create the Direct Sound buffer for this sound */
		hres = IDirectSound_CreateSoundBuffer(DSObject, &dsBuffDesc, &sndBuffer, NULL);
		if (hres != DS_OK)
		{
			LOCALASSERT(1 == 0);
			return 0;
		}

		/* Lock the buffer to allow the write */
		hres = IDirectSoundBuffer_Lock(sndBuffer, 0, myChunkHeader.chunkLength,
			&audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, 0);
		if ((hres != DS_OK) || (audioPtr2 != NULL))
		{
			LOCALASSERT(1 == 0);
			IDirectSoundBuffer_Release(sndBuffer);
			return 0;
		}

		/* Read data from file to buffer */
		RebSndRead(audioPtr1, 1, myChunkHeader.chunkLength, bufferPtr);
#if 0
		if (res != (size_t)myChunkHeader.chunkLength)
		{
			LOCALASSERT(1 == 0);
			IDirectSoundBuffer_Release(sndBuffer);
			return 0;
		}
#endif
		/* then unlock it and close the file */
		hres = IDirectSoundBuffer_Unlock(sndBuffer, audioPtr1, audioBytes1, audioPtr2, audioBytes2);
		if (hres != DS_OK)
		{
			LOCALASSERT(1 == 0);
			IDirectSoundBuffer_Release(sndBuffer);
			return 0;
		}
		/* Finally, put a pointer the the buffer into the sound data */
		GameSounds[soundIndex].dsBufferP = sndBuffer;

		/* Log it's position. */
		{
			DSBCAPS caps;
			ZeroMemory(&caps, sizeof(DSBCAPS));
			caps.dwSize = sizeof(DSBCAPS);
			IDirectSoundBuffer_GetCaps(sndBuffer, &caps);

			if (caps.dwFlags & DSBCAPS_LOCHARDWARE)
			{
				GameSounds[soundIndex].flags = SAMPLE_IN_HW;
				db_logf3(("Sound %s loaded into hardware slot %i by ExtractWavFile.", GameSounds[soundIndex].wavName, soundIndex));
			}
			else
			{
				GameSounds[soundIndex].flags = SAMPLE_IN_SW;
				db_logf3(("Sound %s loaded into software slot %i by ExtractWavFile.", GameSounds[soundIndex].wavName, soundIndex));
			}
		}
		GameSounds[soundIndex].length = lengthInSeconds;
	}

	return endOfBufferPtr;
}


int FFL_LoadSounds(void)
{
	void* rebSndBuffer;
	unsigned char* bufferPtr;
	int soundIndex;
	int pitch;

	char filename[256];

	strcpy_s(filename, "C:\\Games\\GOG\\AvP Classic\\fastfile");
	strcat_s(filename, "\\");
	strcat_s(filename, "common.ffl");

	printf("%s\n", filename);

	rebSndBuffer = LoadRebSndFile(filename);

	if (!rebSndBuffer)
	{
		// TODO: Handle Error
		return -1;
	}

	// Process the file...
	bufferPtr = (unsigned char*)rebSndBuffer;
	soundIndex = (int)(*bufferPtr++);
	pitch = (int)((signed char)(*bufferPtr++));
	while ((soundIndex != 0xff) || (pitch != -1))
	{
		if ((soundIndex < 0) || (soundIndex >= SID_MAXIMUM))
		{
			// invalid sound number
			// LOCALASSERT("Invalid Sound Index" == 0);
		}

		/*
		if (GameSounds[soundIndex].loaded)
		{
			// Duplicate game sound loaded
			LOCALASSERT("Duplicate game sound loaded" == 0);
		}
		*/

		bufferPtr = ExtractWavFile(soundIndex, bufferPtr);

		// GameSounds[soundIndex].loaded = 1;
		// GameSounds[soundIndex].activeInstances = 0;
		// GameSounds[soundIndex].volume = VOLUME_DEFAULT;

		/* pitch offset is in semitones: need to convert to 1/128ths */
		// GameSounds[soundIndex].pitch = pitch;

		// InitialiseBaseFrequency(soundIndex);
		soundIndex = (int)(*bufferPtr++);
		pitch = (int)((signed char)(*bufferPtr++));
	}

	return 0;
}
