#include <stdlib.h>
#include "str_util.h"

float str_to_float(char *in_str)
{
	float return_val = strtof(in_str, NULL);
	return return_val;
}

int str_to_int(char *in_str)
{
	long str_as_long = strtol(in_str, NULL, 10);
	return str_as_long; /* FIXME: no checks on whether the conversion worked or not */
}

