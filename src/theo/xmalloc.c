/*
 * xmalloc.c
 *
 */

#include <stdlib.h>

#include "msys.h"

void *xmalloc(size_t size)
{
	void *ptr = 0;

	ptr = malloc(size);
	ALWAYS(ptr);

	return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
	void *new_ptr = 0;

	new_ptr = realloc(ptr, size);
	ALWAYS(new_ptr);

	return new_ptr;
}

void xfree(void *ptr)
{
	ALWAYS(ptr);
	free(ptr);
	return;
}
