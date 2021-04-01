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

#include "theo/cell_dir.h"
#include "theo/xmalloc.h"
#include "./mocks.inc"
#include "./main.h"

#define ONE_MB 1024*1024

Ensure(test_cell_dir_init)
{
	struct cell_dir cell_dir;
	cell_dir_init(&cell_dir);
}

Ensure(test_cell_dir_add)
{
	struct cell_dir cell_dir;
	struct checksum checksum;
	struct cell_dir_entry in;
	struct cell_dir_entry out;
	char *buffer;

	cell_dir_init(&cell_dir);
	buffer = xmalloc(ONE_MB);

	checksum_init(&checksum);
	checksum.bytes[1] = 1;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);

	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);

	xfree(buffer);
}

Ensure(test_cell_dir_multi_add)
{
	struct cell_dir cell_dir;
	struct checksum checksum;
	struct cell_dir_entry in;
	struct cell_dir_entry out;
	char *buffer;

	cell_dir_init(&cell_dir);
	buffer = xmalloc(ONE_MB);

	checksum_init(&checksum);
	checksum.bytes[1] = 0;
	// Search empty cache_dir
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 1;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);
	checksum.bytes[1] = 2;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);
	checksum.bytes[1] = 3;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);
	// trigger overflow.
	checksum.bytes[1] = 4;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);

	checksum.bytes[1] = 2;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);

	checksum.bytes[1] = 0;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 4;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);

	xfree(buffer);
}

Ensure(test_cell_dir_remove)
{
	struct cell_dir cell_dir;
	struct checksum checksum;
	struct cell_dir_entry in;
	struct cell_dir_entry out;
	char *buffer;

	cell_dir_init(&cell_dir);
	buffer = xmalloc(ONE_MB);

	checksum_init(&checksum);
	checksum.bytes[1] = 0;
	// Search empty cache_dir
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 1;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);
	checksum.bytes[1] = 2;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);

	checksum.bytes[1] = 5;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     true);
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 5;
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     false);
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 3;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);
	// trigger overflow.
	checksum.bytes[1] = 4;
	cell_dir_entry_set(&in, &checksum, 26);
	cell_dir_add(&cell_dir, buffer + ONE_MB, &in);

	checksum.bytes[1] = 0;
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     false);

	checksum.bytes[1] = 1;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     true);
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 2;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     true);
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 3;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     true);
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	checksum.bytes[1] = 4;
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), true);
	assert_equal(cell_dir_remove(&cell_dir, buffer + ONE_MB, &checksum),
		     true);
	assert_equal(cell_dir_get_chunk
		     (&cell_dir, buffer + ONE_MB, &checksum, &out), false);

	xfree(buffer);
}

/**
 * Create the Test suite.
 */
TestSuite *cell_dir_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_dir_init);
	add_test(suite, test_cell_dir_add);
	add_test(suite, test_cell_dir_multi_add);
	add_test(suite, test_cell_dir_remove);

	return suite;
}

CGREEN_TEST_MAIN(cell_dir_tests);
