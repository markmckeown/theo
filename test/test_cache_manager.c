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

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>


#include "theo/cache_manager.h"
#include "theo/cache_metrics.h"
#include "theo/chunk.h"
#include "theo/checksum.h"
#include "theo/msys.h"
#include "./mocks.inc"
#include "./main.h"


#define FIVE_MB 5*1024*1024ull
#define FOUR_MB 4*1024*1024ull



Ensure(test_cache_manager_init)
{
	int ret_t = 0;
	char *filename = "/tmp/foo";
	struct cache_manager cache_manager;
	struct stat statbuf;
	char *string = "a string";
	struct chunk chunk;
	struct checksum checksum;

	chunk_init(&chunk);
	checksum_init(&checksum);
	memset(&statbuf, 0, sizeof(struct stat));

	msys_unlink(filename);
	memset(&cache_manager, 0, sizeof(struct cache_manager));
	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager,
			filename, FOUR_MB); 
	assert_equal(ret_t, 0);
	assert_equal(msys_access(filename, W_OK), 0);

	ret_t = msys_stat(filename, &statbuf);
	assert_equal(ret_t, 0);
	assert_equal(statbuf.st_size, FIVE_MB);

	checksum.bytes[0] = 2;
	assert_equal(cache_manager_add_chunk(&cache_manager, &checksum, 
			string, strlen(string)), 1);
	// second add fails
	assert_equal(cache_manager_add_chunk(&cache_manager, &checksum, 
			string, strlen(string)), 0);
	assert_equal(cache_manager_has_chunk(&cache_manager, &checksum), 1);
	chunk_init(&chunk);
	assert_equal(cache_manager_get_chunk(&cache_manager, 
				&checksum, &chunk), 1);
	assert_equal(memcmp(string, chunk.buffer, strlen(string)), 0);
	checksum.bytes[0] = 22;
	assert_equal(cache_manager_has_chunk(&cache_manager, &checksum), 0);

	cache_manager_release(&cache_manager);

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager,
			filename, FOUR_MB); 
	assert_equal(ret_t, 0);
	assert_equal(msys_access(filename, W_OK), 0);

	checksum.bytes[0] = 2;
	assert_equal(cache_manager_get_chunk(&cache_manager, 
				&checksum, &chunk), 1);
	assert_equal(memcmp(string, chunk.buffer, strlen(string)), 0);

	cache_manager_release(&cache_manager);
	// Double release should be safe
	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

Ensure(test_cache_manager_reset_file)
{
	int ret_t = 0;
	int fd = 0;
	char *filename = "/tmp/foo";
	struct cache_manager cache_manager;
	struct stat statbuf;
	char *string = "a string";
	struct chunk chunk;
	struct checksum checksum;

	chunk_init(&chunk);
	checksum_init(&checksum);
	memset(&statbuf, 0, sizeof(struct stat));

	msys_unlink(filename);
	fd = msys_open(filename, O_RDWR|O_CREAT);
	assert_true(fd > 0);
	ret_t = msys_close(fd);
	assert_equal(ret_t, 0);

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager,
			filename, FOUR_MB); 
	assert_equal(ret_t, 0);
	assert_equal(msys_access(filename, W_OK), 0);

	ret_t = msys_stat(filename, &statbuf);
	assert_equal(ret_t, 0);
	assert_equal(statbuf.st_size, FIVE_MB);

	checksum.bytes[0] = 2;
	cache_manager_add_chunk(&cache_manager, &checksum, 
			string, strlen(string));
	assert_equal(cache_manager_has_chunk(&cache_manager, &checksum), 1);
	chunk_init(&chunk);
	assert_equal(cache_manager_get_chunk(&cache_manager, 
				&checksum, &chunk), 1);
	assert_equal(memcmp(string, chunk.buffer, strlen(string)), 0);
	checksum.bytes[0] = 22;
	assert_equal(cache_manager_has_chunk(&cache_manager, &checksum), 0);

	cache_manager_release(&cache_manager);

	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager,
			filename, FOUR_MB); 
	assert_equal(ret_t, 0);
	assert_equal(msys_access(filename, W_OK), 0);

	checksum.bytes[0] = 2;
	assert_equal(cache_manager_get_chunk(&cache_manager, 
				&checksum, &chunk), 1);
	assert_equal(memcmp(string, chunk.buffer, strlen(string)), 0);

	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

Ensure(test_cache_manager_check_metrics)
{
	int ret_t = 0;
	char *filename = "/tmp/foo";
	struct cache_manager cache_manager;
	char *string = "a string";
	struct chunk chunk;
	struct checksum checksum;
	struct cache_metrics cache_metrics;

	chunk_init(&chunk);
	checksum_init(&checksum);

	msys_unlink(filename);
	memset(&cache_manager, 0, sizeof(struct cache_manager));
	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager,
			filename, FOUR_MB); 
	assert_equal(ret_t, 0);

	checksum.bytes[0] = 2;
	assert_equal(cache_manager_add_chunk(&cache_manager, &checksum, 
			string, strlen(string)), 1);
	// second add fails
	assert_equal(cache_manager_add_chunk(&cache_manager, &checksum, 
			string, strlen(string)), 0);
	assert_equal(cache_manager_has_chunk(&cache_manager, &checksum), 1);

	cache_metrics_init(&cache_metrics);
	cache_manager_cache_metrics(&cache_manager, &cache_metrics);
	assert_equal(cache_metrics.byte_count, strlen(string));
	assert_equal(cache_metrics.entry_count, 1);


	chunk_init(&chunk);
	assert_equal(cache_manager_get_chunk(&cache_manager, 
				&checksum, &chunk), 1);
	assert_equal(memcmp(string, chunk.buffer, strlen(string)), 0);


	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}
/**
 * Create the Test suite.
 */
TestSuite *cache_manager_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cache_manager_init);
	add_test(suite, test_cache_manager_reset_file);
	add_test(suite, test_cache_manager_check_metrics);

	return suite;
}

CGREEN_TEST_MAIN(cache_manager_tests);
