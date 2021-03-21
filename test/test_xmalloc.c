/**
    Copyright 2021 WANdisco

    This file is part of Libtheo.

    Libtheo is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
*/
/*
 * test_xmalloc.c
 *
 */

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <string.h>

#include "theo/xmalloc.h"
#include "./mocks.inc"
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
