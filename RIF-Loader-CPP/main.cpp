#include <stdio.h>
#include <stdlib.h>

#include "avp_rif.h"

int main()
{
    const char* rif_path = "C:\\Games\\GOG\\AvP Classic\\avp_rifs\\derelict.rif";

    printf("%s\n", rif_path);

    AVP_Rif_Load(rif_path);

    return EXIT_SUCCESS;
}
