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

#include "theo/cell_ic.h"
#include "./mocks.inc"
#include "./main.h"

Ensure(test_cell_ic_sanity) {
	struct cell_ic cell_ic;
	cell_ic_init(&cell_ic);
	assert_equal(cell_ic.exit_code, 0);
	assert_equal(cell_ic.entry_code, 0);
	assert_equal(cell_ic_sane(&cell_ic), true);
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
	add_test(suite, test_cell_ic_setting);

	return suite;
}

CGREEN_TEST_MAIN(cell_ic_tests);
