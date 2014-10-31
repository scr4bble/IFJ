#include "string.h"

String initEmptyString()
{
	String str = {
		.length  = 0,
		.allocated_size = STRING_INIT_ALLOC_SIZE,
		.data    = malloc(STRING_INIT_ALLOC_SIZE * sizeof(char))
	};
	MALLOC_TEST(str.data);

	return str;
}

void destroyString(String *dst) {
	if(dst->allocated_size > 0)
		free(dst->data);
	dst->allocated_size = 0;
	dst->length = 0;
}

char atString(String *dst, uint32_t pos) {
	assert(dst);
	assert(pos <= dst->length);

	return dst->data[pos];
}

char setAtString(String *dst, uint32_t pos, char c) {
	assert(dst);
	assert(pos <= dst->length);

	return dst->data[pos] = c;
}

void printString(String *dst)
{
	assert(dst);

	if(dst->length > 0) {
		printf("%s", dst->data);
	}
}

void aboutString(String *dst) {
	printf("STR: allocated %u ,length %u\n", dst->allocated_size, dst->length);
}

void appendCharToString(String *dst, char c)
{
	assert(dst);
	assert(dst->allocated_size > 0 &&
								"Performed operation on uninitialized String");

	// Double allocated space every potencial overflow
	// !! Side-effect condition
	if(++dst->length + 1 > dst->allocated_size) {
		dst->allocated_size *= 2;
		dst->data = realloc(dst->data, dst->allocated_size);

		assert(dst->length < dst->allocated_size);
	}

	assert((int32_t) dst->length - 1 >= 0);

	dst->data[dst->length - 1] = c;
	dst->data[dst->length] = (char) 0;
	// Due to performance, not clearing all remaning bytes
}

void truncateString(String *dst) {
	assert(dst);
	if(dst->allocated_size > 0) {
		// Cutting string with zero byte
		dst->data[0] = (char) 0;
		dst->length = 0;
	}
}

// Do not use! Use streq()
bool equalsStringChars(String *str1, char *str2)
{
	assert(str1); assert(str2);
	return (strcmp(str1->data, str2) == 0);
}

// Do not use! Use streq()
bool equalsStringString(String *str1, String *str2)
{
	assert(str1); assert(str2);
	return (strcmp(str1->data, str2->data) == 0);
}
