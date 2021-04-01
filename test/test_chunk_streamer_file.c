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
#include <fcntl.h>

#include "isa-l_crypto.h"

#include "theo/chunk_streamer.h"
#include "theo/cache_manager.h"
#include "theo/cache_metrics.h"
#include "theo/msys.h"

#include "./mocks.inc"
#include "./main.h"

#define LARGE_MB 128*1024*1024ull
#define READ_BUFFER_SIZE 65536

Ensure(test_chunk_streamer_file)
{
	int ret_t = 0;
	char *filename = "/tmp/foo";
	char *dickens = "test/files/dickens";
	struct chunk_streamer chunk_streamer;
	struct cache_manager cache_manager;
	char read_buffer[READ_BUFFER_SIZE];
	int fd;

	msys_unlink(filename);

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, LARGE_MB);
	assert_equal(ret_t, 0);
	chunk_streamer_init(&chunk_streamer, &cache_manager);

	fd = msys_open(dickens, O_RDONLY);
	assert_true(fd != -1);
	uint32_t base_size = 0;
	for (;;) {
		ret_t =
		    read(fd, read_buffer + base_size,
			 READ_BUFFER_SIZE - base_size);
		if (ret_t < 0) {
			break;
		}
		if (ret_t == 0) {
			break;
		}
		chunk_streamer_process_buffer(&chunk_streamer, read_buffer,
					      ret_t + base_size);
		base_size =
		    chunk_streamer_flush_retained(&chunk_streamer, read_buffer,
						  READ_BUFFER_SIZE);
	}
	msys_close(fd);

	assert_equal(chunk_streamer.chunk_added, 1021);
	assert_equal(chunk_streamer.chunk_hit, 0);
	struct cache_metrics cache_metrics;
	cache_manager_cache_metrics(&cache_manager, &cache_metrics);
	assert_equal(cache_metrics.entry_count, 1021);
	assert_equal(cache_metrics.byte_count, 10185212);

	// Close and re-open cache_manager.
	cache_manager_release(&cache_manager);
	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, LARGE_MB);
	assert_equal(ret_t, 0);

	// Now re-read file but add some bytes to the start
	chunk_streamer_init(&chunk_streamer, &cache_manager);
	fd = msys_open(dickens, O_RDONLY);
	assert_true(fd != -1);
	base_size = strlen(dickens);
	memcpy(read_buffer, dickens, strlen(dickens));
	for (;;) {
		ret_t =
		    read(fd, read_buffer + base_size,
			 READ_BUFFER_SIZE - base_size);
		if (ret_t < 0) {
			break;
		}
		if (ret_t == 0) {
			break;
		}
		chunk_streamer_process_buffer(&chunk_streamer, read_buffer,
					      ret_t + base_size);
		base_size =
		    chunk_streamer_flush_retained(&chunk_streamer, read_buffer,
						  READ_BUFFER_SIZE);
	}
	msys_close(fd);

	// We lose one block.
	assert_equal(chunk_streamer.chunk_added, 1);
	assert_equal(chunk_streamer.chunk_hit, 1020);

	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

/**
 * Create the Test suite.
 */
TestSuite *chunk_streamer_file_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_chunk_streamer_file);

	return suite;
}

CGREEN_TEST_MAIN(chunk_streamer_file_tests);
