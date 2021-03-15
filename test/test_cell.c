#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_header.h"
#include "theo/cell_body_entry.h"
#include "theo/checksum.h"
#include "theo/chunk.h"
#include "theo/xmalloc.h"
#include "theo/cell.h"
#include "./main.h"


#define ONE_MB 1024*1024



Ensure(test_cell_init)
{
	struct cell cell;
	char* buffer;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	// Now load again.
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	xfree(buffer);
}

Ensure(test_cell_add)
{
	struct cell cell;
	char* buffer;
	char *string = "a string";
	struct checksum checksum;
	struct chunk chunk;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);


	checksum_init(&checksum);
	checksum.bytes[3] = 23;
	cell_add_chunk(&cell, &checksum, string, strlen(string));

	assert_equal(cell_has_chunk(&cell, &checksum), 1);
	chunk_init(&chunk);
	assert_equal(cell_get_chunk(&cell, &checksum, &chunk), 1);
	assert_equal(chunk.buffer_size, strlen(string));
	assert_equal(memcmp(&chunk.buffer, string, strlen(string)), 0);

	checksum.bytes[3] = 24;
	assert_equal(cell_has_chunk(&cell, &checksum), 0);
	assert_equal(cell_get_chunk(&cell, &checksum, &chunk), 0);

	// Now load again.
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	checksum.bytes[3] = 23;
	assert_equal(cell_has_chunk(&cell, &checksum), 1);


	xfree(buffer);
}


/**
 * Create the Test suite.
 */
TestSuite *cell_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_init);
	add_test(suite, test_cell_add);

	return suite;
}

CGREEN_TEST_MAIN(cell_tests);
