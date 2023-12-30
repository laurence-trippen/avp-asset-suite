#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "avp_error.h"
#include "avp_checker.h"


int main(void)
{
    const char rif_folder[] = "C:\\Games\\GOG\\AvP Classic\\avp_rifs\\";

    AVP_error_t result = AVP_check_rif_headers(rif_folder);
    if (result == FATAL_ERROR) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
