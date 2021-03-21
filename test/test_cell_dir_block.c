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

#include "theo/cell_dir_block.h"
#include "./mocks.inc"
#include "./main.h"

Ensure(test_cell_dir_block_init) {
	struct cell_dir_block cell_dir_block;
	cell_dir_block_init(&cell_dir_block);
	assert_equal(cell_dir_block_full(&cell_dir_block), false);
}

Ensure(test_cell_dir_block_add) {
	struct cell_dir_block cell_dir_block;
	struct checksum checksum;
	struct cell_dir_entry in;

	cell_dir_block_init(&cell_dir_block);
	checksum_init(&checksum);
	checksum.bytes[1] = 1;

	cell_dir_entry_set(&in ,&checksum, 26);
	cell_dir_block_add(&cell_dir_block, &in);
	assert_equal(cell_dir_block_full(&cell_dir_block), false);
}


Ensure(test_cell_dir_block_add2) {
        struct cell_dir_block cell_dir_block;
        struct checksum checksum;
	struct cell_dir_entry in;

        cell_dir_block_init(&cell_dir_block);
        checksum_init(&checksum);
        checksum.bytes[1] = 1;

	cell_dir_entry_set(&in, &checksum,  26);
        cell_dir_block_add(&cell_dir_block, &in);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum.bytes[1] = 2;

	cell_dir_entry_set(&in, &checksum,  56);
	cell_dir_block_add(&cell_dir_block, &in);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);
}

Ensure(test_cell_dir_block_full) {
        struct cell_dir_block cell_dir_block;
        struct checksum checksum;
	struct cell_dir_entry in;

        cell_dir_block_init(&cell_dir_block);
        
	checksum_init(&checksum);
        checksum.bytes[1] = 1;
	cell_dir_entry_set(&in, &checksum, 26);
        assert_equal(cell_dir_block_add(&cell_dir_block, &in), true);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum_init(&checksum);
        checksum.bytes[1] = 2;
	cell_dir_entry_set(&in, &checksum, 56);
        assert_equal(cell_dir_block_add(&cell_dir_block, &in), true);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum_init(&checksum);
        checksum.bytes[1] = 3;
	cell_dir_entry_set(&in, &checksum, 56);
        assert_equal(cell_dir_block_add(&cell_dir_block, &in), true);
        assert_equal(cell_dir_block_full(&cell_dir_block), true);

	// Add to full block
        checksum_init(&checksum);
        checksum.bytes[1] = 4;
	cell_dir_entry_set(&in, &checksum, 96);
        assert_equal(cell_dir_block_add(&cell_dir_block, &in), false);
        assert_equal(cell_dir_block_full(&cell_dir_block), true);
}

Ensure(test_cell_dir_block_remove) {
        struct cell_dir_block cell_dir_block;
        struct checksum checksum;
	struct cell_dir_entry in;
	struct cell_dir_entry cell_dir_entry;
	
	cell_dir_entry_init(&cell_dir_entry);

        cell_dir_block_init(&cell_dir_block);
        checksum_init(&checksum);
        checksum.bytes[1] = 1;
	assert_equal(cell_dir_block_get_chunk(&cell_dir_block, &checksum, &cell_dir_entry), false);

	cell_dir_entry_set(&in, &checksum, 26);
        cell_dir_block_add(&cell_dir_block, &in);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum.bytes[1] = 2;
	cell_dir_entry_set(&in, &checksum, 56);
	cell_dir_block_add(&cell_dir_block, &in);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	checksum.bytes[1] = 3;
	cell_dir_entry_set(&in, &checksum, 56);
	cell_dir_block_add(&cell_dir_block, &in);
	assert_equal(cell_dir_block_full(&cell_dir_block), true);


	checksum.bytes[1] = 4;
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), false);

	checksum.bytes[1] = 1;
	assert_equal(cell_dir_block_get_chunk(&cell_dir_block, &checksum, &cell_dir_entry), true);
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), true);
	assert_equal(cell_dir_block_get_chunk(&cell_dir_block, &checksum, &cell_dir_entry), false);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	checksum.bytes[1] = 2;
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), true);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	checksum.bytes[1] = 3;
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), true);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	checksum.bytes[1] = 4;
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), false);
}

Ensure(test_cell_dir_block_overflow) {
        struct cell_dir_block cell_dir_block;
	cell_dir_block_init(&cell_dir_block);
	assert_equal(cell_dir_block_overflowed(&cell_dir_block), false);
	assert_equal(cell_dir_block_increment_overflow(&cell_dir_block), 1);
	assert_equal(cell_dir_block_overflowed(&cell_dir_block), true);
	assert_equal(cell_dir_block_decrement_overflow(&cell_dir_block), 0);
	assert_equal(cell_dir_block_overflowed(&cell_dir_block), false);
}

/**
 * Create the Test suite.
 */
TestSuite *cell_dir_block_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_dir_block_init);
	add_test(suite, test_cell_dir_block_add);
	add_test(suite, test_cell_dir_block_add2);
	add_test(suite, test_cell_dir_block_full);
	add_test(suite, test_cell_dir_block_remove);
	add_test(suite, test_cell_dir_block_overflow);

	return suite;
}

CGREEN_TEST_MAIN(cell_dir_block_tests);
