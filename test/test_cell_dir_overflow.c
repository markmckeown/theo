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

#include "theo/cell_dir_overflow.h"
#include "theo/xmalloc.h"
#include "./mocks.inc"
#include "./main.h"

#define ONE_MB 1024*1024

Ensure(test_cell_dir_overflow_init)
{
	struct cell_dir_overflow cell_dir_overflow;
	cell_dir_overflow_init(&cell_dir_overflow);
}

Ensure(test_cell_dir_overflow_add)
{
	struct cell_dir_overflow cell_dir_overflow;
	char *buffer;
	struct checksum checksum;
	struct cell_dir_entry cell_dir_entry;
	struct cell_dir_entry in;

	cell_dir_overflow_init(&cell_dir_overflow);
	checksum_init(&checksum);
	checksum.bytes[0] = 1;
	buffer = xmalloc(ONE_MB);

	cell_dir_entry_set(&in, &checksum, 11);
	assert_equal(cell_dir_overflow_add(&cell_dir_overflow, buffer + ONE_MB,
					   &in), 1);
	assert_equal(cell_dir_overflow.overflow_count, 1);

	checksum.bytes[0] = 2;
	cell_dir_entry_set(&in, &checksum, 22);
	assert_equal(cell_dir_overflow_add(&cell_dir_overflow, buffer + ONE_MB,
					   &in), 2);
	assert_equal(cell_dir_overflow.overflow_count, 2);

	checksum.bytes[0] = 3;
	cell_dir_entry_set(&in, &checksum, 33);
	assert_equal(cell_dir_overflow_add(&cell_dir_overflow, buffer + ONE_MB,
					   &in), 3);
	assert_equal(cell_dir_overflow.overflow_count, 3);

	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), true);
	assert_equal(cell_dir_entry.offset, 33);

	checksum.bytes[0] = 2;
	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), true);
	assert_equal(cell_dir_entry.offset, 22);

	checksum.bytes[0] = 1;
	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), true);
	assert_equal(cell_dir_entry.offset, 11);

	checksum.bytes[0] = 99;
	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), false);

	xfree(buffer);
}

Ensure(test_cell_dir_overflow_remove)
{
	struct cell_dir_overflow cell_dir_overflow;
	char *buffer;
	struct checksum checksum;
	struct cell_dir_entry cell_dir_entry;
	struct cell_dir_entry in;

	cell_dir_overflow_init(&cell_dir_overflow);
	checksum_init(&checksum);
	checksum.bytes[0] = 1;
	buffer = xmalloc(ONE_MB);

	cell_dir_entry_set(&in, &checksum, 11);
	assert_equal(cell_dir_overflow_add(&cell_dir_overflow, buffer + ONE_MB,
					   &in), 1);
	assert_equal(cell_dir_overflow.overflow_count, 1);

	checksum.bytes[0] = 2;
	cell_dir_entry_set(&in, &checksum, 22);
	assert_equal(cell_dir_overflow_add(&cell_dir_overflow, buffer + ONE_MB,
					   &in), 2);
	assert_equal(cell_dir_overflow.overflow_count, 2);

	checksum.bytes[0] = 3;
	cell_dir_entry_set(&in, &checksum, 33);
	assert_equal(cell_dir_overflow_add(&cell_dir_overflow, buffer + ONE_MB,
					   &in), 3);
	assert_equal(cell_dir_overflow.overflow_count, 3);

	checksum.bytes[0] = 1;
	assert_equal(cell_dir_overflow_remove
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum), true);
	assert_equal(cell_dir_overflow.overflow_count, 2);

	checksum.bytes[0] = 2;
	assert_equal(cell_dir_overflow_remove
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum), true);
	assert_equal(cell_dir_overflow.overflow_count, 1);

	// Remove entry that does not exist
	checksum.bytes[0] = 99;
	assert_equal(cell_dir_overflow_remove
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum), false);
	assert_equal(cell_dir_overflow.overflow_count, 1);

	checksum.bytes[0] = 3;
	assert_equal(cell_dir_overflow_remove
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum), true);
	assert_equal(cell_dir_overflow.overflow_count, 0);

	// Remove from empty overflow.
	checksum.bytes[0] = 99;
	assert_equal(cell_dir_overflow_remove
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum), false);
	assert_equal(cell_dir_overflow.overflow_count, 0);

	checksum.bytes[0] = 3;
	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), false);

	checksum.bytes[0] = 2;
	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), false);

	checksum.bytes[0] = 1;
	cell_dir_entry_init(&cell_dir_entry);
	assert_equal(cell_dir_overflow_get_chunk
		     (&cell_dir_overflow, buffer + ONE_MB, &checksum,
		      &cell_dir_entry), false);

	xfree(buffer);
}

/**
 * Create the Test suite.
 */
TestSuite *cell_dir_overflow_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_dir_overflow_init);
	add_test(suite, test_cell_dir_overflow_add);
	add_test(suite, test_cell_dir_overflow_remove);

	return suite;
}

CGREEN_TEST_MAIN(cell_dir_overflow_tests);
