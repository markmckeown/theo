#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_header.h"
#include "./main.h"



Ensure(test_cell_header_init)
{
	char buffer[1024];
	struct cell_header cell_header;
	cell_header_init(&cell_header, buffer, 1024);
	assert_equal(cell_header_sanity(&cell_header), 0);	
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
