/*
 * test_xmalloc.c
 *
 */

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <string.h>

#include "theo/xmalloc.h"
#include "./main.h"

Ensure(test_malloc_free_release)
{
	void *ptr = 0;
	char *hello = "Hello World";
	int ret = 0;

	ptr = xmalloc(12);
	// valgrind will catch errors
	memcpy(ptr, hello, 12);
	ret = memcmp(ptr, hello, 12);
	assert_equal(ret, 0);

	xfree(ptr);
}

TestSuite *xmalloc_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_malloc_free_release);

	return suite;
}

CGREEN_TEST_MAIN(xmalloc_tests);
