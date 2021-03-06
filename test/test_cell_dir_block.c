#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_dir_block.h"
#include "./main.h"

Ensure(test_cell_dir_block_init) {
	struct cell_dir_block cell_dir_block;
	cell_dir_block_init(&cell_dir_block);
	assert_equal(cell_dir_block_full(&cell_dir_block), false);
}

Ensure(test_cell_dir_block_add) {
	struct cell_dir_block cell_dir_block;
	struct checksum checksum;

	cell_dir_block_init(&cell_dir_block);
	checksum_init(&checksum);
	checksum.bytes[1] = 1;

	cell_dir_block_add(&cell_dir_block, &checksum, 34, 26);
	assert_equal(cell_dir_block_full(&cell_dir_block), false);
}


Ensure(test_cell_dir_block_add2) {
        struct cell_dir_block cell_dir_block;
        struct checksum checksum;

        cell_dir_block_init(&cell_dir_block);
        checksum_init(&checksum);
        checksum.bytes[1] = 1;

        cell_dir_block_add(&cell_dir_block, &checksum, 34, 26);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	cell_dir_block_init(&cell_dir_block);
        checksum_init(&checksum);
        checksum.bytes[1] = 2;

	cell_dir_block_add(&cell_dir_block, &checksum, 74, 56);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);
}

Ensure(test_cell_dir_block_full) {
        struct cell_dir_block cell_dir_block;
        struct checksum checksum;

        cell_dir_block_init(&cell_dir_block);
        
	checksum_init(&checksum);
        checksum.bytes[1] = 1;
        cell_dir_block_add(&cell_dir_block, &checksum, 34, 26);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum_init(&checksum);
        checksum.bytes[1] = 2;
	cell_dir_block_add(&cell_dir_block, &checksum, 74, 56);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum_init(&checksum);
        checksum.bytes[1] = 3;
	cell_dir_block_add(&cell_dir_block, &checksum, 74, 56);
        assert_equal(cell_dir_block_full(&cell_dir_block), true);

}

Ensure(test_cell_dir_block_remove) {
        struct cell_dir_block cell_dir_block;
        struct checksum checksum;

        cell_dir_block_init(&cell_dir_block);
        checksum_init(&checksum);
        checksum.bytes[1] = 1;

        cell_dir_block_add(&cell_dir_block, &checksum, 34, 26);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

        checksum.bytes[1] = 2;
        
	cell_dir_block_add(&cell_dir_block, &checksum, 74, 56);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	checksum.bytes[1] = 3;
	assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), false);
	assert_equal(cell_dir_block_full(&cell_dir_block), false);

	struct cell_dir_entry cell_dir_entry;
	cell_dir_entry_init(&cell_dir_entry);

	checksum.bytes[1] = 1;
	assert_equal(cell_dir_block_has_chunk(&cell_dir_block, &checksum, &cell_dir_entry), true);
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), true);
	assert_equal(cell_dir_block_has_chunk(&cell_dir_block, &checksum, &cell_dir_entry), false);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);

	checksum.bytes[1] = 2;
        assert_equal(cell_dir_block_remove(&cell_dir_block, &checksum), true);
        assert_equal(cell_dir_block_full(&cell_dir_block), false);
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

	return suite;
}

CGREEN_TEST_MAIN(cell_dir_block_tests);
