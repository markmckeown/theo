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
#include <stdio.h>
#include <sys/times.h>

#include "isa-l_crypto.h"

#include "theo/chunk_streamer.h"
#include "theo/cache_manager.h"
#include "theo/cache_metrics.h"
#include "theo/msys.h"
#include "theo/xmalloc.h"

#include "./mocks.inc"
#include "./main.h"

#define LARGE_MB 128*1024*1024ull
#define BIG_BUFFER 20*1024*1024ull
#define READ_BUFFER_SIZE 65536

Ensure(test_chunk_streamer_file_perf)
{
	int ret_t = 0;
	char *filename = "/tmp/foo";
	char *dickens = "test/files/dickens";
	struct chunk_streamer chunk_streamer;
	struct cache_manager cache_manager;
	char *file_buffer;
	int fd;
	uint32_t read_size = 0;

	msys_unlink(filename);
	file_buffer = xmalloc(BIG_BUFFER);

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, LARGE_MB);
	assert_equal(ret_t, 0);
	chunk_streamer_init(&chunk_streamer, &cache_manager);

	fd = msys_open(dickens, O_RDONLY);
	assert_true(fd != -1);
	for (;;) {
		ret_t =
		    read(fd, file_buffer + read_size, BIG_BUFFER - read_size);
		if (ret_t < 0) {
			break;
		}
		if (ret_t == 0) {
			break;
		}
		read_size += ret_t;
	}
	msys_close(fd);

	struct tms start_timer, end_timer;
	clock_t start, end;
	start = times(&start_timer);
	chunk_streamer_process_buffer(&chunk_streamer, file_buffer, read_size);
	end = times(&end_timer);
	long clock_tick = sysconf(_SC_CLK_TCK);
	fprintf(stdout, "[Rate: %8.0f B/s ",
		read_size / ((end - start) / (double)clock_tick));
	fprintf(stdout, "Real: %6.2fs ", (end - start) / (double)clock_tick);
	fprintf(stdout, "User: %6.2fs ",
		(end_timer.tms_utime -
		 start_timer.tms_utime) / (double)clock_tick);
	fprintf(stdout, "Sys: %6.2fs]\n",
		(end_timer.tms_stime -
		 start_timer.tms_stime) / (double)clock_tick);

	assert_equal(chunk_streamer.chunk_added, 1021);
	assert_equal(chunk_streamer.chunk_hit, 0);
	struct cache_metrics cache_metrics;
	cache_manager_cache_metrics(&cache_manager, &cache_metrics);
	assert_equal(cache_metrics.entry_count, 1021);
	assert_equal(cache_metrics.byte_count, 10185212);

	xfree(file_buffer);
	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

/**
 * Create the Test suite.
 */
TestSuite *chunk_streamer_perf_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_chunk_streamer_file_perf);

	return suite;
}

CGREEN_TEST_MAIN(chunk_streamer_perf_tests);
