#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_dir_overflow.h"
#include "./main.h"



Ensure(test_cell_dir_overflow_init)
{
	struct cell_dir_overflow cell_dir_overflow;
	cell_dir_overflow_init(&cell_dir_overflow);
}


/**
 * Create the Test suite.
 */
TestSuite *cell_dir_overflow_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_dir_overflow_init);

	return suite;
}

CGREEN_TEST_MAIN(cell_dir_overflow_tests);
