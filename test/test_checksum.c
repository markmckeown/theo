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

#include "theo/checksum.h"
#include "./mocks.inc"
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
