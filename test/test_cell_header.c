#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_header.h"
#include "theo/xmalloc.h"
#include "theo/cell.h"
#include "./main.h"


#define ONE_MB 1024*1024

Ensure(test_cell_header_init)
{
	struct cell cell;
	char* buffer;

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
