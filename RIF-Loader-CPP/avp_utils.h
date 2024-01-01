#ifndef AVP_UTILS_H
#define AVP_UTILS_H

/*
Formats bytes to pretty format.

@returns Formatted String. Returned pointer must be freed!
*/
char* const AVP_Format_Bytes(long bytes, int decimals);

#endif
