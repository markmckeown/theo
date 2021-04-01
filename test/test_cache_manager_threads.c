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
#include <pthread.h>
#include <stdlib.h>

#include "theo/cache_manager.h"
#include "theo/chunk.h"
#include "theo/checksum.h"
#include "theo/msys.h"
#include "./mocks.inc"
#include "./main.h"

#define FIVE_MB 5*1024*1024ull
#define HUNDRED_MB 100*1024*1024ull
#define FOUR_MB 4*1024*1024ull
#define ONE_MB 1024*1024ull

void *add_entry(void *ptr)
{
	char string[23000];
	struct chunk chunk;
	struct checksum checksum;
	struct cache_manager *cache_manager = (struct cache_manager *)ptr;
	int i, j, passed;

	chunk_init(&chunk);
	checksum_init(&checksum);

	passed = 1;
	for (j = 0; j < 100; j++) {
		for (i = 0; i < CHECKSUM_SIZE; i++) {
			checksum.bytes[i] = rand();
		}
		cache_manager_add_chunk(cache_manager, &checksum,
					string, (rand() % 23000) + 2000);
		if (cache_manager_has_chunk(cache_manager, &checksum) != 1) {
			passed = 0;
			break;
		}
	}
	assert_equal(passed, 1);
	return NULL;
}

Ensure(test_cache_manager_thread_1)
{
	int ret_t = 0;
	char *filename = "/tmp/foo";
	struct cache_manager cache_manager;
	struct stat statbuf;
	pthread_t thread[10];
	int i;
	srand(0);
	memset(&statbuf, 0, sizeof(struct stat));

	msys_unlink(filename);
	memset(&cache_manager, 0, sizeof(struct cache_manager));
	cache_manager_init(&cache_manager);
	ret_t = cache_manager_open_cache(&cache_manager, filename, HUNDRED_MB);
	assert_equal(ret_t, 0);
	assert_equal(msys_access(filename, W_OK), 0);

	ret_t = msys_stat(filename, &statbuf);
	assert_equal(ret_t, 0);
	assert_equal(statbuf.st_size, HUNDRED_MB + ONE_MB);

	for (i = 0; i < 10; i++) {
		assert_equal(pthread_create(&thread[i], NULL, add_entry,
					    (void *)&cache_manager), 0);
	}
	for (i = 0; i < 10; i++) {
		pthread_join(thread[i], NULL);
	}

	cache_manager_release(&cache_manager);
	msys_unlink(filename);
}

/**
 * Create the Test suite.
 */
TestSuite *cache_manager_threads_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cache_manager_thread_1);

	return suite;
}

CGREEN_TEST_MAIN(cache_manager_threads_tests);
