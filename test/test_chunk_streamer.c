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

#include "theo/chunk_streamer.h"
#include "theo/cache_manager.h"
#include "theo/msys.h"

#include "./mocks.inc"
#include "./main.h"

#define FOUR_MB 4*1024*1024ull

Ensure(test_chunk_streamer_init)
{
	int ret_t = 0;
	char *filename = "/tmp/foo";
	struct chunk_streamer chunk_streamer;
	struct cache_manager cache_manager;

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, FOUR_MB);
	assert_equal(ret_t, 0);

	chunk_streamer_init(&chunk_streamer, &cache_manager);

	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

Ensure(test_chunk_streamer_random_buffer)
{
	int ret_t = 0;
	int i;
	char *filename = "/tmp/foo";
	struct chunk_streamer chunk_streamer;
	struct cache_manager cache_manager;
	char buffer[30000];
	char retained[3000];

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, FOUR_MB);
	assert_equal(ret_t, 0);
	chunk_streamer_init(&chunk_streamer, &cache_manager);

	memset(buffer, 0, CHUNKER_MAX_CHUNK_SIZE);
	srand(0);
	for (i = 0; i < 30000; i++) {
		buffer[i] = rand();
	}

	chunk_streamer_process_buffer(&chunk_streamer, buffer, 3000);
	assert_equal(chunk_streamer.chunk_added, 1);
	assert_equal(chunk_streamer.chunk_hit, 0);
	chunk_streamer_flush_retained(&chunk_streamer, retained, 30000);
	chunk_streamer_flush_retained(&chunk_streamer, retained, 30000);

	chunk_streamer_process_buffer(&chunk_streamer, buffer, 3000);
	assert_equal(chunk_streamer.chunk_added, 1);
	assert_equal(chunk_streamer.chunk_hit, 1);

	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

Ensure(test_chunk_streamer_random_full_buffer)
{
	int ret_t = 0;
	int i;
	char *filename = "/tmp/foo";
	struct chunk_streamer chunk_streamer;
	struct cache_manager cache_manager;
	char buffer[128000];
	char retained[30000];

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, FOUR_MB);
	assert_equal(ret_t, 0);
	chunk_streamer_init(&chunk_streamer, &cache_manager);

	memset(buffer, 0, CHUNKER_MAX_CHUNK_SIZE);
	srand(0);
	for (i = 0; i < 128000; i++) {
		buffer[i] = rand();
	}

	chunk_streamer_process_buffer(&chunk_streamer, buffer, 128000);
	assert_equal(chunk_streamer.chunk_added, 13);
	assert_equal(chunk_streamer.chunk_hit, 0);
	chunk_streamer_flush_retained(&chunk_streamer, retained, 30000);
	// Do the same buffer again.
	chunk_streamer_process_buffer(&chunk_streamer, buffer, 128000);
	assert_equal(chunk_streamer.chunk_added, 13);
	assert_equal(chunk_streamer.chunk_hit, 13);

	cache_manager_release(&cache_manager);
	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, FOUR_MB);
	assert_equal(ret_t, 0);
	chunk_streamer_init(&chunk_streamer, &cache_manager);

	chunk_streamer_process_buffer(&chunk_streamer, buffer, 128000);
	assert_equal(chunk_streamer.chunk_added, 0);
	assert_equal(chunk_streamer.chunk_hit, 13);

	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

/**
 * Create the Test suite.
 */
TestSuite *chunk_streamer_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_chunk_streamer_init);
	add_test(suite, test_chunk_streamer_random_buffer);
	add_test(suite, test_chunk_streamer_random_full_buffer);

	return suite;
}

CGREEN_TEST_MAIN(chunk_streamer_tests);
