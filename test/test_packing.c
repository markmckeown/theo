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

#include "theo/cell_header.h"
#include "theo/cell_ic.h"
#include "theo/checksum.h"
#include "theo/cell_body_entry.h"
#include "theo/checksum.h"
#include "theo/chunk.h"
#include "theo/xmalloc.h"
#include "theo/cell.h"
#include "./mocks.inc"
#include "./main.h"

Ensure(test_structs_packed)
{
	assert_equal(sizeof(struct cell_ic), 2 * sizeof(uint32_t));
	assert_equal(sizeof(struct checksum), 32);
	assert_equal(sizeof(struct cell_dir_entry), 36);
	assert_equal(sizeof(struct cell_body_entry), 36);
	assert_equal(sizeof(struct cell_dir_block),
		     3 * sizeof(struct cell_dir_entry) + sizeof(uint32_t));
	assert_equal(sizeof(struct cell_dir_overflow), sizeof(uint32_t));
	assert_equal(sizeof(struct cell_dir),
		     256 * sizeof(struct cell_dir_block) +
		     sizeof(struct cell_dir_overflow));
	assert_equal(sizeof(struct cell_body), 2 * sizeof(uint32_t));
	assert_equal(sizeof(struct cell_header),
		     3 * sizeof(uint32_t) + sizeof(struct cell_ic) +
		     sizeof(struct cell_body) + sizeof(struct cell_dir));

}

/**
 * Create the Test suite.
 */
TestSuite *packing_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_structs_packed);

	return suite;
}

CGREEN_TEST_MAIN(packing_tests);
