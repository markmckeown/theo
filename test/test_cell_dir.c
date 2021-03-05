#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_dir.h"
#include "./main.h"



Ensure(test_cell_dir_init)
{
	struct cell_dir cell_dir;
	cell_dir_init(&cell_dir);
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
