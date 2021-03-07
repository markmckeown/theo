#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_ic.h"
#include "./main.h"

Ensure(test_cell_ic_sanity) {
	struct cell_ic cell_ic;
	cell_ic_init(&cell_ic);
	assert_equal(cell_ic.exit_code, 0);
	assert_equal(cell_ic.entry_code, 0);
	assert_equal(cell_ic_sane(&cell_ic), true);
}


Ensure(test_cell_ic_padding) {
	assert_equal(cell_ic_padding(), 0);
}

Ensure(test_cell_ic_setting) {
	struct cell_ic cell_ic;
	cell_ic_init(&cell_ic);
	assert_equal(cell_ic.exit_code, 0);
	assert_equal(cell_ic.entry_code, 0);
	assert_equal(cell_ic_sane(&cell_ic), true);

	assert_equal(cell_ic_on_entry(&cell_ic), 1);
	assert_equal(cell_ic_sane(&cell_ic), false);
	assert_equal(cell_ic_on_exit(&cell_ic), 1);
	assert_equal(cell_ic_sane(&cell_ic), true);

}

/**
 * Create the Test suite.
 */
TestSuite *cell_ic_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_ic_sanity);
	add_test(suite, test_cell_ic_padding);
	add_test(suite, test_cell_ic_setting);

	return suite;
}

CGREEN_TEST_MAIN(cell_ic_tests);
