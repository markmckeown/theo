#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/hello.h"
#include "./main.h"

Ensure(test_hello)
{
	assert_equal(hello(), 1);
}

/**
 * Create the Test suite.
 */
TestSuite *hello_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_hello);

	return suite;
}

CGREEN_TEST_MAIN(hello_tests);
