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
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_header.h"
#include "theo/xmalloc.h"
#include "theo/cell.h"
#include "./mocks.inc"
#include "./main.h"

#define ONE_MB 1024*1024

Ensure(test_cell_header_init)
{
	struct cell cell;
	char *buffer;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);
	xfree(buffer);
}

/**
 * Create the Test suite.
 */
TestSuite *cell_header_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_header_init);

	return suite;
}

CGREEN_TEST_MAIN(cell_header_tests);
