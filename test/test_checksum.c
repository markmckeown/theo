#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/checksum.h"
#include "./main.h"

Ensure(test_checksum_init) {
	struct checksum checksum1;
	struct checksum checksum2;
	checksum_init(&checksum1);
	checksum_init(&checksum2);
	assert_equal(checksum_compare(&checksum1, &checksum2), 0);
	checksum1.bytes[0] = 1;
	assert_equal(checksum_compare(&checksum1, &checksum2), 1);
}


/**
 * Create the Test suite.
 */
TestSuite *checksum_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_checksum_init);

	return suite;
}

CGREEN_TEST_MAIN(checksum_tests);
