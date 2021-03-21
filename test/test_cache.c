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

#include "theo/xmalloc.h"
#include "theo/cache.h"
#include "theo/chunk.h"
#include "theo/checksum.h"
#include "theo/cache_header.h"
#include "theo/cache_metrics.h"
#include "./mocks.inc"
#include "./main.h"


#define FIVE_MB 5*1024*1024



Ensure(test_cache_init)
{
	struct cache cache;
	char* buffer;

	buffer = xmalloc(FIVE_MB);
	memset(buffer, 0, FIVE_MB);
	cache_init(&cache, buffer, FIVE_MB);

	assert_equal(cache.cache_header->cache_magic, 340865607434ull);
	assert_equal(cache.cache_header->cell_count, 4);

	cache_release(&cache);

	// reload cache
	cache_init(&cache, buffer, FIVE_MB);
	cache_release(&cache);
	cache_release(&cache);

	xfree(buffer);
}


Ensure(test_cache_add)
{
	struct cache cache;
	char *buffer;
	char *string = "a string";
	struct chunk chunk;
	struct checksum checksum;
	struct cache_metrics cache_metrics;

	chunk_init(&chunk);
	checksum_init(&checksum);

	buffer = xmalloc(FIVE_MB);
	memset(buffer, 0, FIVE_MB);
	cache_init(&cache, buffer, FIVE_MB);

	assert_equal(cache.cache_header->cache_magic, 340865607434ull);
	assert_equal(cache.cache_header->cell_count, 4);

	checksum.bytes[0] = 2;
	cache_add_chunk(&cache, &checksum, string, strlen(string));

	cache_metrics_init(&cache_metrics);
	cache_get_metrics(&cache, &cache_metrics);
	assert_equal(cache_metrics.byte_count, strlen(string));
	assert_equal(cache_metrics.entry_count, 1);

	assert_equal(cache_has_chunk(&cache, &checksum), 1);
	chunk_init(&chunk);
	assert_equal(cache_get_chunk(&cache, &checksum, &chunk), 1);
	assert_equal(memcmp(string, chunk.buffer, strlen(string)), 0);
	checksum.bytes[0] = 22;
	assert_equal(cache_has_chunk(&cache, &checksum), 0);

	cache_release(&cache);

	// reload cache
	cache_init(&cache, buffer, FIVE_MB);
	checksum.bytes[0] = 2;
	assert_equal(cache_has_chunk(&cache, &checksum), 1);

	cache_release(&cache);
	xfree(buffer);
}

/**
 * Create the Test suite.
 */
TestSuite *cache_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cache_init);
	add_test(suite, test_cache_add);

	return suite;
}

CGREEN_TEST_MAIN(cache_tests);
