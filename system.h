/*
 * Project name:
 * Implementace interpretu imperativního jazyka IFJ14
 *
 * Repository:
 * https://github.com/Dasio/IFJ
 *
 * Team:
 * Dávid Mikuš			(xmikus15)
 * Peter Hostačný		(xhosta03)
 * Tomáš Kello			(xkello00)
 * Adam Lučanský		(xlucan01)
 * Michaela Lukášová		(xlukas09)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#ifndef SYSTEM_H
#define SYSTEM_H

//#include "error.h"

#define MALLOC_TEST(ptr) do {if(ptr == NULL) exit(1); } while(0)


// https://gcc.gnu.org/onlinedocs/gcc-3.4.6/gcc/Typeof.html#Typeof
#define max(a, b) \
	({  typeof (a) _a = (a); \
		typeof (b) _b = (b); \
			_a > _b ? _a : _b; })

#define streq(a, b) (strcmp((a), (b)) == 0)

#ifndef NDEBUG
#define DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )
#else
#define DEBUG_PRINT(...) do{ } while ( false )
#endif

/**
 * Calculates random value from withing range <low, high>
 * @param  low  Lower bound
 * @param  high Higher bound
 * @return      Random integer within range <low, high> including both sides
 */
int RandInt(int low, int high);

#endif
