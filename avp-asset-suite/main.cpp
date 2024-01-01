// avp-asset-suite.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "ffl_sound.h"
#include "ffl_stdio.h"

#pragma comment (lib, "winmm.lib")

int main(void)
{
    int result = FFL_info_init("C:\\Games\\GOG\\AvP Classic\\fastfile\\ffinfo.txt", "fastfile\\");

    // FFL_LoadSounds();

    // MessageBox(NULL, L"Hello", L"World", MB_OKCANCEL);
    // PlaySound(L"C:\\Users\\LT-Laptop\\Music\\CantinaBand60.wav", NULL, SND_SYNC);

    return EXIT_SUCCESS;
}
