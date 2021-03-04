#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_dir.h"
#include "./main.h"



Ensure(test_cell_dir_init)
{
	char buffer[1024];
	struct cell_dir cell_dir;
	cell_dir_init(&cell_dir, buffer, 1024);
	assert_equal(cell_dir.buffer_start, buffer);
	assert_equal(cell_dir.hole_start, buffer);
	assert_equal(cell_dir.hole_size, 1024);
	assert_equal(cell_dir.buffer_size, 1024);
	assert_equal(cell_dir.next_slot, 0);
	assert_equal(cell_dir.cell_entries_count, 0);
}


/**
 * Create the Test suite.
 */
TestSuite *cell_dir_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_dir_init);

	return suite;
}

CGREEN_TEST_MAIN(cell_dir_tests);
