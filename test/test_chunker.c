/**
    Copyright 2021 WANdisco
    Author Mark Mc Keown

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
    along with Libtheo.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "isa-l_crypto.h"

#include "theo/chunker.h"
#include "theo/chunk_ref.h"

#include "./mocks.inc"
#include "./main.h"



Ensure(test_chunker_mean)
{
	uint32_t mask = rolling_hashx_mask_gen(CHUNKER_AVG_CHUNK_SIZE, 0);
	assert_equal(mask, CHUNKER_MASK);	
}

Ensure(test_chunker_init)
{
	int ret = 0;
	struct chunker chunker;
	ret  = chunker_init(&chunker);
	assert_equal(ret, 0);	
}

Ensure(test_chunker_small_buffer)
{
	int ret = 0;
	struct chunker chunker;
	struct chunk_ref chunk_ref;
	char *string = "a string";

	ret  = chunker_init(&chunker);
	assert_equal(ret, 0);	

	ret = chunker_chunk_buffer(&chunker, &chunk_ref, string, strlen(string));
	assert_equal(ret, CHUNKER_NOT_FOUND);
	assert_equal(chunk_ref.buffer_size, 0);
	assert_equal(chunk_ref.buffer, 0);
	assert_equal(chunker.retained_size, strlen(string));
	assert_equal(memcmp(chunker.retained, string, strlen(string)), 0);

}

Ensure(test_chunker_large_empty_buffer)
{
	int ret = 0;
	struct chunker chunker;
	struct chunk_ref chunk_ref;
	char buffer[30000];

	ret  = chunker_init(&chunker);
	assert_equal(ret, 0);	
	memset(buffer, 0, 30000);

	ret = chunker_chunk_buffer(&chunker, &chunk_ref, buffer, 30000);
	assert_equal(ret, CHUNKER_FOUND);
	assert_equal(chunk_ref.buffer_size, CHUNKER_MAX_CHUNK_SIZE);
	assert_equal(chunker.retained_size, 0);
}


Ensure(test_chunker_medium_empty_buffer)
{
	int ret = 0;
	struct chunker chunker;
	struct chunk_ref chunk_ref;
	char buffer[20000];

	ret  = chunker_init(&chunker);
	assert_equal(ret, 0);	
	memset(buffer, 0, 20000);

	ret = chunker_chunk_buffer(&chunker, &chunk_ref, buffer, 20000);
	assert_equal(ret, CHUNKER_NOT_FOUND);
	assert_equal(chunk_ref.buffer_size, 0);
	assert_equal(chunk_ref.buffer, 0);
	assert_equal(chunker.retained_size, 20000);
}

Ensure(test_chunker_max_chunk_size_empty_buffer)
{
	int ret = 0;
	struct chunker chunker;
	struct chunk_ref chunk_ref;
	char buffer[CHUNKER_MAX_CHUNK_SIZE];

	ret  = chunker_init(&chunker);
	assert_equal(ret, 0);	
	memset(buffer, 0, CHUNKER_MAX_CHUNK_SIZE);

	ret = chunker_chunk_buffer(&chunker, &chunk_ref, buffer, CHUNKER_MAX_CHUNK_SIZE);
	assert_equal(ret, CHUNKER_FOUND);
	assert_equal(chunk_ref.buffer_size, CHUNKER_MAX_CHUNK_SIZE);
	assert_equal(chunker.retained_size, 0);
}

Ensure(test_chunker_random_buffer)
{
	int ret = 0;
	int i;
	struct chunker chunker;
	struct chunk_ref chunk_ref;
	char buffer[30000];

	ret  = chunker_init(&chunker);
	assert_equal(ret, 0);	
	memset(buffer, 0, CHUNKER_MAX_CHUNK_SIZE);

	srand(0);
	for (i = 0; i < 30000; i++) {
		buffer[i] = rand();
	}

	ret = chunker_chunk_buffer(&chunker, &chunk_ref, buffer, CHUNKER_MAX_CHUNK_SIZE);
	assert_equal(ret, CHUNKER_FOUND);
	assert_equal(chunk_ref.buffer_size, 2808);
	assert_equal(chunker.retained_size, 0);
	ret = chunker_chunk_buffer(&chunker, &chunk_ref, buffer, CHUNKER_MAX_CHUNK_SIZE);
	assert_equal(ret, CHUNKER_FOUND);
	assert_equal(chunk_ref.buffer_size, 2808);
	assert_equal(chunker.retained_size, 0);
}

/**
 * Create the Test suite.
 */
TestSuite *chunker_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_chunker_mean);
	add_test(suite, test_chunker_init);
	add_test(suite, test_chunker_small_buffer);
	add_test(suite, test_chunker_large_empty_buffer);
	add_test(suite, test_chunker_medium_empty_buffer);
	add_test(suite, test_chunker_max_chunk_size_empty_buffer);
	add_test(suite, test_chunker_random_buffer);

	return suite;
}

CGREEN_TEST_MAIN(chunker_tests);
