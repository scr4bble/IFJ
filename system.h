#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#ifndef SYSTEM_H
#define SYSTEM_H

#include "error.h"

#define MALLOC_TEST(ptr) do {if(ptr == NULL) setError(ERR_Allocation); } while(0)

// https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Typeof.html#Typeof
#define max(a, b) \
	({  typeof (a) _a = (a); \
		typeof (b) _b = (b); \
			_a > _b ? _a : _b; })

#endif
