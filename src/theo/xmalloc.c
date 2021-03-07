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

void xfree(void *ptr)
{
	ALWAYS(ptr);
	free(ptr);
	return;
}
