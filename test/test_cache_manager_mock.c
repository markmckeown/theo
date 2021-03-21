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
#include <errno.h>
#include <sys/mman.h>


#include "theo/cache_manager.h"
#include "theo/chunk.h"
#include "theo/checksum.h"
#include "theo/msys.h"
#include "./main.h"

#define MOCK_MSYS_OPEN
#define MOCK_MSYS_WRITE
#define MOCK_MSYS_FSYNC
#define MOCK_MSYS_CLOSE
#define MOCK_MSYS_STAT
#define MOCK_MSYS_UNLINK
#define MOCK_MSYS_MMAP
#define MOCK_MSYS_MUNMAP
#define MOCK_CACHE_INIT
#include "./mocks.inc"
#ifdef MOCK_MSYS_OPEN
#error "mock not set"
#endif
#ifdef MOCK_MSYS_WRITE
#error "mock not set"
#endif
#ifdef MOCK_MSYS_FSYNC
#error "mock not set"
#endif
#ifdef MOCK_MSYS_CLOSE
#error "mock not set"
#endif
#ifdef MOCK_MSYS_STAT
#error "mock not set"
#endif
#ifdef MOCK_MSYS_UNLINK
#error "mock not set"
#endif
#ifdef MOCK_MSYS_MMAP
#error "mock not set"
#endif
#ifdef MOCK_MSYS_MUNMAP
#error "mock not set"
#endif
#ifdef MOCK_CACHE_INIT
#error "mock not set"
#endif

#define FIVE_MB 5*1024*1024ull
#define FOUR_MB 4*1024*1024ull
#define ONE_MB  1*1024*1024ull
#define QUARTER_MB  256*1024ull


Ensure(test_cache_manager_create_file_1)
{
	char *filename = "/tmp/foo";
	expect(msys_open, will_return(-1));
	assert_equal(cache_manager_create_file(filename, FIVE_MB), -1);
}

Ensure(test_cache_manager_create_file_2)
{
	char *filename = "/tmp/foo";
	expect(msys_open, will_return(0));
	expect(msys_write, will_return(-1));
	expect(msys_close, will_return(0));
	assert_equal(cache_manager_create_file(filename, FIVE_MB), -1);
}

Ensure(test_cache_manager_create_file_3)
{
	char *filename = "/tmp/foo";
	expect(msys_open, will_return(0));
	expect(msys_write, will_return(ONE_MB));
	expect(msys_fsync, will_return(0));
	expect(msys_close, will_return(0));
	assert_equal(cache_manager_create_file(filename, ONE_MB), 0);
}

Ensure(test_cache_manager_create_file_4)
{
	char *filename = "/tmp/foo";
	expect(msys_open, will_return(0));
	expect(msys_write, will_return(QUARTER_MB));
	expect(msys_write, will_return(QUARTER_MB));
	expect(msys_write, will_return(QUARTER_MB));
	expect(msys_write, will_return(QUARTER_MB));
	expect(msys_fsync, will_return(0));
	expect(msys_close, will_return(0));
	assert_equal(cache_manager_create_file(filename, ONE_MB), 0);
}

Ensure(test_cache_manager_check_file_1)
{
	char *filename = "/tmp/foo";
	// Won't find file
	expect(msys_stat, will_return(-1));
	// Cannot create file
	expect(msys_open, will_return(-1));
	assert_equal(cache_manager_check_file(filename, ONE_MB), -1);
}

Ensure(test_cache_manager_check_file_2)
{
	char *filename = "/tmp/foo";
	// Won't find file
	stat_buf_st_size = 0;
	expect(msys_stat, will_return(0));
	// Cannot unlink file
	expect(msys_unlink, will_return(-1));
	assert_equal(cache_manager_check_file(filename, ONE_MB), -1);
}

Ensure(test_cache_manager_check_file_3)
{
	char *filename = "/tmp/foo";
	// Won't find file
	stat_buf_st_size = 0;
	expect(msys_stat, will_return(0));
	// Cannot unlink file
	expect(msys_unlink, will_return(0));
	expect(msys_open, will_return(-1));
	assert_equal(cache_manager_check_file(filename, ONE_MB), -1);
}


Ensure(test_cache_manager_open_cache_1)
{
	struct cache_manager cache_manager;
	char *filename = "/tmp/foo";

	cache_manager_init(&cache_manager);

	// No such file
	expect(msys_stat, will_return(-1));
	// Cannot open file
	expect(msys_open, will_return(-1));
	assert_equal(
		cache_manager_open_cache(&cache_manager, filename, ONE_MB), -1);

	cache_manager_release(&cache_manager);
}

Ensure(test_cache_manager_open_cache_2)
{
	struct cache_manager cache_manager;
	char *filename = "/tmp/foo";

	cache_manager_init(&cache_manager);

	// File exists and is right size
	stat_buf_st_size = 2 * ONE_MB;
	expect(msys_stat, will_return(0));
	// cache_manager_open_cache cannot open file
	expect(msys_open, will_return(-1));
	assert_equal(
		cache_manager_open_cache(&cache_manager, filename, ONE_MB), -1);

	cache_manager_release(&cache_manager);
}

Ensure(test_cache_manager_open_cache_3)
{
	struct cache_manager cache_manager;
	char *filename = "/tmp/foo";

	cache_manager_init(&cache_manager);

	// File exists and is right size
	stat_buf_st_size = 2 * ONE_MB;
	expect(msys_stat, will_return(0));
	// cache_manager_open_cache cannot open file
	expect(msys_open, will_return(5));
	errno = EOPNOTSUPP; // No dax, but second mmap fails
	expect(msys_mmap, will_return((void *) -1));
	expect(msys_mmap, will_return((void *) -1));
	expect(msys_close, will_return(0));
	assert_equal(
		cache_manager_open_cache(&cache_manager, filename, ONE_MB), -1);

	cache_manager_release(&cache_manager);
}

Ensure(test_cache_manager_open_cache_4)
{
	struct cache_manager cache_manager;
	char *filename = "/tmp/foo";

	cache_manager_init(&cache_manager);

	// File exists and is right size
	stat_buf_st_size = 2 * ONE_MB;
	expect(msys_stat, will_return(0));
	// cache_manager_open_cache cannot open file
	expect(msys_open, will_return(5));
	// errno not set, dax supported but some other failure.
	expect(msys_mmap, will_return((void *) -1));
	expect(msys_close, will_return(0));
	assert_equal(
		cache_manager_open_cache(&cache_manager, filename, ONE_MB), -1);

	cache_manager_release(&cache_manager);
}

Ensure(test_cache_manager_open_cache_5)
{
	struct cache_manager cache_manager;
	char *filename = "/tmp/foo";

	cache_manager_init(&cache_manager);

	// File exists and is right size
	stat_buf_st_size = 2 * ONE_MB;
	expect(msys_stat, will_return(0));
	// cache_manager_open_cache cannot open file
	expect(msys_open, will_return(5));
	// mmap with DAX OK.
	expect(msys_mmap, will_return((void *)0xDEADBEEF));
	expect(cache_init, will_return(0));
	expect(msys_munmap, will_return(0));
	expect(msys_close, will_return(0));
	assert_equal(
		cache_manager_open_cache(&cache_manager, filename, ONE_MB), 0);
	assert_equal(cache_manager.dax, 1);
	cache_manager_release(&cache_manager);
}

/**
 * Create the Test suite.
 */
TestSuite *cache_manager_mock_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cache_manager_create_file_1);
	add_test(suite, test_cache_manager_create_file_2);
	add_test(suite, test_cache_manager_create_file_3);
	add_test(suite, test_cache_manager_create_file_4);

	add_test(suite, test_cache_manager_check_file_1);
	add_test(suite, test_cache_manager_check_file_2);
	add_test(suite, test_cache_manager_check_file_3);

	add_test(suite, test_cache_manager_open_cache_1);
	add_test(suite, test_cache_manager_open_cache_2);
	add_test(suite, test_cache_manager_open_cache_3);
	add_test(suite, test_cache_manager_open_cache_4);
	add_test(suite, test_cache_manager_open_cache_5);

	return suite;
}

CGREEN_TEST_MAIN(cache_manager_mock_tests);
