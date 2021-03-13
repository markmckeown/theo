#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "theo/cell_header.h"
#include "theo/cell_body_entry.h"
#include "theo/checksum.h"
#include "theo/xmalloc.h"
#include "theo/cell.h"
#include "./main.h"


#define ONE_MB 1024*1024

Ensure(test_cell_body_init)
{
	struct cell cell;
	char* buffer;
	struct cell_body_entry *cell_body_entry;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	cell_body_entry = (struct cell_body_entry *) cell.slab;
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 1);

	// Now load again.
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	xfree(buffer);
}

Ensure(test_cell_body_add_entry)
{
	struct cell cell;
	char* buffer;
	struct cell_body_entry *cell_body_entry;
	struct cell_dir_entry out;
	char *string = "a string";
	struct checksum checksum;


	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	cell_body_entry = (struct cell_body_entry *) cell.slab;
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 1);

	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[3] = 23;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, strlen(string), &out);
	assert_equal(out.checksum.bytes[3], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, strlen(string));
	assert_equal(cell_body_entry->free_space, 0);
	assert_equal(cell_body_entry->top_entry, 0);
	assert_equal(cell_body_entry->checksum.bytes[3], 23);
	assert_equal(cell_body_entry->size, strlen(string));

	assert_equal (cell.cell_header->cell_body.next_entry_offset, 
			sizeof(struct cell_body_entry) + strlen(string));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			sizeof(struct cell_body_entry) + strlen(string));

	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[3], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, strlen(string));
				

	xfree(buffer);
}

Ensure(test_cell_body_add_entries)
{
	struct cell cell;
	char* buffer;
	struct cell_body_entry *cell_body_entry;
	struct cell_dir_entry out;
	char *string = "a string";
	char *another_string = "another_string_to_add";
	struct checksum checksum;


	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	cell_body_entry = (struct cell_body_entry *) cell.slab;
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 1);

	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 23;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, strlen(string), &out);
	assert_equal(out.checksum.bytes[17], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, strlen(string));
	assert_equal(cell_body_entry->free_space, 0);
	assert_equal(cell_body_entry->top_entry, 0);
	assert_equal(cell_body_entry->checksum.bytes[17], 23);
	assert_equal(cell_body_entry->size, strlen(string));

	assert_equal (cell.cell_header->cell_body.next_entry_offset, 
			sizeof(struct cell_body_entry) + strlen(string));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			sizeof(struct cell_body_entry) + strlen(string));

	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[17], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, strlen(string));
				
	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 33;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum,
			another_string, strlen(another_string), &out);
	assert_equal(out.checksum.bytes[17], 33);
	assert_equal(out.offset, sizeof(struct cell_body_entry) + strlen(string));
	assert_equal(out.size, strlen(another_string));
	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[17], 33);
	assert_equal(out.offset, sizeof(struct cell_body_entry) + strlen(string));
	assert_equal(out.size, strlen(another_string));

	assert_equal (cell.cell_header->cell_body.next_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + strlen(string));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + strlen(string));
	xfree(buffer);
}


Ensure(test_cell_body_add_entries_offset)
{
	struct cell cell;
	char* buffer;
	struct cell_body_entry *cell_body_entry;
	struct cell_dir_entry out;
	char string[124];
	char *another_string = "another_string_to_add";
	char *third_string = "string";
	struct checksum checksum;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	cell_body_entry = (struct cell_body_entry *) cell.slab;
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 1);

	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 23;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 124, &out);
	assert_equal(out.checksum.bytes[17], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, 124);
	assert_equal(cell_body_entry->free_space, 0);
	assert_equal(cell_body_entry->top_entry, 0);
	assert_equal(cell_body_entry->checksum.bytes[17], 23);
	assert_equal(cell_body_entry->size, 124);

	assert_equal (cell.cell_header->cell_body.next_entry_offset, 
			sizeof(struct cell_body_entry) + 124);
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			sizeof(struct cell_body_entry) + 124);

	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[17], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, 124);
				
	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 33;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum,
			another_string, strlen(another_string), &out);
	assert_equal(out.checksum.bytes[17], 33);
	assert_equal(out.offset, sizeof(struct cell_body_entry) + 124);
	assert_equal(out.size, strlen(another_string));
	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[17], 33);
	assert_equal(out.offset, sizeof(struct cell_body_entry) + 124);
	assert_equal(out.size, strlen(another_string));

	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + 124);
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + 124);
	
	// Put the cell back to the start.
	cell.cell_header->cell_body.next_entry_offset = 0;

	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 73;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum,
			third_string, strlen(third_string), &out);
	assert_equal(out.checksum.bytes[17], 73);
	assert_equal(out.offset, 0);
	assert_equal(out.size, strlen(third_string));
	
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			sizeof(struct cell_body_entry) + strlen(third_string));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + 124);

	cell_body_entry = (struct cell_body_entry *) (cell.slab + sizeof(struct cell_body_entry) + strlen(third_string));
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 0);
	assert_equal(cell_body_entry->size, (124 + sizeof(struct cell_body_entry)) - (2 * sizeof(struct cell_body_entry) + strlen(third_string)));

	xfree(buffer);
}

Ensure(test_cell_body_add_entries_offset_overflow)
{
	struct cell cell;
	char* buffer;
	struct cell_dir_entry out;
	struct cell_body_entry *body_entry;
	char *ptr;
	char string[10];
	char larger_string[15];
	char small_string[5];
	struct checksum checksum;
	int i = 0;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	checksum_init(&checksum);
	for (i = 0; i < 5; i++) {
		cell_dir_entry_init(&out);
		checksum.bytes[17] = i;
		cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 10, &out);
		assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
		assert_equal(out.size, 10);
		assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			(i + 1) * (sizeof(struct cell_body_entry) + 10));
		assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			cell.cell_header->cell_body.next_entry_offset);
	}
	i++;

	// Reset the next_entry
	cell.cell_header->cell_body.next_entry_offset = sizeof(struct cell_body_entry) + 10;
	checksum.bytes[17] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			larger_string, 15, &out);
	assert_equal(out.offset, (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 15);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			 (sizeof(struct cell_body_entry) + 10) + 
			 (sizeof(struct cell_body_entry) + 15));
	ptr = cell.slab + cell.cell_header->cell_body.next_entry_offset;
	body_entry = (struct cell_body_entry *) ptr;
	assert_equal(body_entry->size, 5);
	assert_equal(body_entry->free_space, 1);
	assert_equal(body_entry->top_entry, 0);
	assert_equal(body_entry->previous_size, 15);
	

	++i;
	checksum.bytes[17] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			small_string, 5, &out);
	assert_equal(out.size, 5);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			 (sizeof(struct cell_body_entry) + 10) + 
			 (sizeof(struct cell_body_entry) + 15) + 
			 (sizeof(struct cell_body_entry) + 5));
	ptr = cell.slab + cell.cell_header->cell_body.next_entry_offset;
	body_entry = (struct cell_body_entry *) ptr;
	assert_equal(body_entry->size, 10);
	assert_equal(body_entry->free_space, 0);
	assert_equal(body_entry->top_entry, 0);
	assert_equal(body_entry->previous_size, 5);


	xfree(buffer);
}


Ensure(test_cell_body_add_entries_offset_overwrite_hole)
{
	struct cell cell;
	char* buffer;
	struct cell_dir_entry out;
	struct cell_body_entry *body_entry;
	char *ptr;
	char string[10];
	char larger_string[15];
	char very_large_string[25];
	struct checksum checksum;
	int i = 0;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	checksum_init(&checksum);
	for (i = 0; i < 6; i++) {
		cell_dir_entry_init(&out);
		checksum.bytes[17] = i;
		cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 10, &out);
		assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
		assert_equal(out.size, 10);
		assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			(i + 1) * (sizeof(struct cell_body_entry) + 10));
		assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			cell.cell_header->cell_body.next_entry_offset);
	}
	i++;

	// Reset the next_entry
	cell.cell_header->cell_body.next_entry_offset = sizeof(struct cell_body_entry) + 10;
	checksum.bytes[17] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			larger_string, 15, &out);
	assert_equal(out.offset, (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 15);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			 (sizeof(struct cell_body_entry) + 10) + 
			 (sizeof(struct cell_body_entry) + 15));
	ptr = cell.slab + cell.cell_header->cell_body.next_entry_offset;
	body_entry = (struct cell_body_entry *) ptr;
	assert_equal(body_entry->size, 5);
	assert_equal(body_entry->free_space, 1);
	assert_equal(body_entry->top_entry, 0);
	assert_equal(body_entry->previous_size, 15);
	

	// Now overwite the hole
	cell.cell_header->cell_body.next_entry_offset = sizeof(struct cell_body_entry) + 10;
	++i;
	checksum.bytes[17] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			very_large_string, 25, &out);
	assert_equal(out.size, 25);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			 (sizeof(struct cell_body_entry) + 10) + 
			 (sizeof(struct cell_body_entry) + 25));
	ptr = cell.slab + cell.cell_header->cell_body.next_entry_offset;
	body_entry = (struct cell_body_entry *) ptr;
	assert_equal(body_entry->size, 41);
	assert_equal(body_entry->free_space, 1);
	assert_equal(body_entry->top_entry, 0);
	assert_equal(body_entry->previous_size, 25);


	xfree(buffer);
}

Ensure(test_cell_body_add_entries_offset_over_top)
{
	struct cell cell;
	char* buffer;
	struct cell_dir_entry out;
	struct cell_body_entry *body_entry;
	char *ptr;
	char string[10];
	char very_large_string[25];
	struct checksum checksum;
	int i = 0;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	checksum_init(&checksum);
	for (i = 0; i < 6; i++) {
		cell_dir_entry_init(&out);
		checksum.bytes[17] = i;
		cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 10, &out);
		assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
		assert_equal(out.size, 10);
		assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			(i + 1) * (sizeof(struct cell_body_entry) + 10));
		assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			cell.cell_header->cell_body.next_entry_offset);
	}
	i++;

	// Reset the next_entry
	cell.cell_header->cell_body.next_entry_offset = 4 * (sizeof(struct cell_body_entry) + 10);
	checksum.bytes[17] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			very_large_string, 25, &out);
	assert_equal(out.offset, 4 * (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 25);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			 4 * (sizeof(struct cell_body_entry) + 10) + 
			 (sizeof(struct cell_body_entry) + 25));
	ptr = cell.slab + cell.cell_header->cell_body.next_entry_offset;
	body_entry = (struct cell_body_entry *) ptr;
	assert_equal(body_entry->size, 0);
	assert_equal(body_entry->free_space, 1);
	assert_equal(body_entry->top_entry, 1);
	assert_equal(body_entry->previous_size, 25);
	

	xfree(buffer);
}

Ensure(test_cell_body_add_entries_offset_small_buffer)
{
	struct cell cell;
	char* buffer;
	struct cell_dir_entry out;
	char string[10];
	struct checksum checksum;
	int i = 0;
	int slab_size;

	slab_size = sizeof(struct cell_header) + 5 * (sizeof(struct cell_body_entry) + 10) + 5 +
		sizeof(struct cell_dir_entry) + sizeof(struct cell_body_entry);
	buffer = xmalloc(slab_size);
	memset(buffer, 0, slab_size);
	cell_init(&cell, buffer, slab_size);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	checksum_init(&checksum);
	for (i = 0; i < 5; i++) {
		cell_dir_entry_init(&out);
		checksum.bytes[17] = i;
		cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 10, &out);
		assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
		assert_equal(out.size, 10);
		assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			(i + 1) * (sizeof(struct cell_body_entry) + 10));
		assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			cell.cell_header->cell_body.next_entry_offset);
	}
	i++;

	cell_dir_entry_init(&out);
	checksum.bytes[17] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
		string, 10, &out);
	assert_equal(out.offset, 0);
	assert_equal(out.size, 10);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
		 (sizeof(struct cell_body_entry) + 10));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
		5 * (sizeof(struct cell_body_entry) + 10));


	xfree(buffer);
}

Ensure(test_cell_body_add_entries_small_buffer_header_pushdown)
{
	struct cell cell;
	char* buffer;
	struct cell_dir_entry out;
	struct cell_body_entry *body_entry;
	char *ptr;
	char string[10];
	struct checksum checksum;
	int i = 0;
	int slab_size;

	slab_size = sizeof(struct cell_header) + 5 * (sizeof(struct cell_body_entry) + 10) + 5 +
		sizeof(struct cell_dir_entry) + sizeof(struct cell_body_entry);
	buffer = xmalloc(slab_size);
	memset(buffer, 0, slab_size);
	cell_init(&cell, buffer, slab_size);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	// Buffer will take five entries, note will add to different cache_dir buckets
	checksum_init(&checksum);
	for (i = 0; i < 5; i++) {
		cell_dir_entry_init(&out);
		checksum.bytes[17] = i;
		cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 10, &out);
		assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
		assert_equal(out.size, 10);
		assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			(i + 1) * (sizeof(struct cell_body_entry) + 10));
		assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			cell.cell_header->cell_body.next_entry_offset);
	}

	uint32_t top = cell.cell_header->cell_body.top_entry_offset;
	
	// Buffer will not take five entries, note will to same cache_dir bucket, cache_header will overflow.
	i = 0;
	cell_dir_entry_init(&out);
	checksum.bytes[18] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
		string, 10, &out);
	assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 10);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
		(i + 1) * (sizeof(struct cell_body_entry) + 10));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
		top);
	i++;

	cell_dir_entry_init(&out);
	checksum.bytes[18] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
		string, 10, &out);
	assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 10);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
		(i + 1) * (sizeof(struct cell_body_entry) + 10));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
		top);
	i++;

	cell_dir_entry_init(&out);
	checksum.bytes[18] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
		string, 10, &out);
	assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 10);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
		(i + 1) * (sizeof(struct cell_body_entry) + 10));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
		top);
	i++;
	
	cell_dir_entry_init(&out);
	checksum.bytes[18] = i;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
		string, 10, &out);
	assert_equal(out.offset, i * (sizeof(struct cell_body_entry) + 10));
	assert_equal(out.size, 10);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
		(i + 1) * (sizeof(struct cell_body_entry) + 10));
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
		4 * (sizeof(struct cell_body_entry) + 10));
	assert_equal(cell.cell_header->cell_dir.overflow.overflow_count, 1);
	ptr = cell.slab + cell.cell_header->cell_body.next_entry_offset;
	body_entry = (struct cell_body_entry *) ptr;
	assert_equal(body_entry->top_entry, 1);
	assert_equal(body_entry->size, 0);
	assert_equal(body_entry->previous_size, 10);
	assert_equal(body_entry->free_space, 1);

	i++;

	xfree(buffer);
}


Ensure(test_cell_body_add_entries_top_full)
{
	struct cell cell;
	char* buffer;
	struct cell_body_entry *cell_body_entry;
	struct cell_dir_entry out;
	char string[124];
	char *another_string = "another_string_to_add";
	char third_string[86];
	struct checksum checksum;

	buffer = xmalloc(ONE_MB);
	memset(buffer, 0, ONE_MB);
	cell_init(&cell, buffer, ONE_MB);
	assert_equal(cell_header_sane(cell.cell_header), true);	

	assert_equal(cell.cell_header->cell_body.top_entry_offset, 0);
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 0);

	cell_body_entry = (struct cell_body_entry *) cell.slab;
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 1);

	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 23;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum, 
			string, 124, &out);
	assert_equal(out.checksum.bytes[17], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, 124);
	assert_equal(cell_body_entry->free_space, 0);
	assert_equal(cell_body_entry->top_entry, 0);
	assert_equal(cell_body_entry->checksum.bytes[17], 23);
	assert_equal(cell_body_entry->size, 124);

	assert_equal (cell.cell_header->cell_body.next_entry_offset, 
			sizeof(struct cell_body_entry) + 124);
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			sizeof(struct cell_body_entry) + 124);

	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[17], 23);
	assert_equal(out.offset, 0);
	assert_equal(out.size, 124);
				
	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 33;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum,
			another_string, strlen(another_string), &out);
	assert_equal(out.checksum.bytes[17], 33);
	assert_equal(out.offset, sizeof(struct cell_body_entry) + 124);
	assert_equal(out.size, strlen(another_string));
	cell_dir_entry_init(&out);
	assert_equal(cell_dir_get_chunk(&cell.cell_header->cell_dir, 
				(char *) cell.cell_header, &checksum, &out), true);
	assert_equal(out.checksum.bytes[17], 33);
	assert_equal(out.offset, sizeof(struct cell_body_entry) + 124);
	assert_equal(out.size, strlen(another_string));

	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + 124);
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			2 * sizeof(struct cell_body_entry) + strlen(another_string) + 124);
	
	cell.cell_header->cell_body.top_entry_offset = cell.slab_size - (sizeof(struct cell_header) + 
			sizeof(struct cell_body_entry) + 82);
	cell.cell_header->cell_body.next_entry_offset = cell.cell_header->cell_body.top_entry_offset;
	cell_body_entry = (struct cell_body_entry *)(cell.slab + cell.cell_header->cell_body.top_entry_offset);
	cell_body_entry->top_entry = 1;
	cell_body_entry->free_space = 0;
	cell_body_entry->size = 0;

	checksum_init(&checksum);
	cell_dir_entry_init(&out);
	checksum.bytes[17] = 73;
	cell_body_add(&cell.cell_header->cell_body, &cell, &checksum,
			third_string, 86, &out);
	assert_equal(out.checksum.bytes[17], 73);
	assert_equal(out.offset, 0);
	assert_equal(out.size, 86);
	
	assert_equal(cell.cell_header->cell_body.next_entry_offset, 
			sizeof(struct cell_body_entry) + 86);
	assert_equal(cell.cell_header->cell_body.top_entry_offset, 
			cell.slab_size - (sizeof(struct cell_header) + sizeof(struct cell_body_entry) + 82));

	cell_body_entry = (struct cell_body_entry *) (cell.slab + sizeof(struct cell_body_entry) + 86);
	assert_equal(cell_body_entry->free_space, 1);
	assert_equal(cell_body_entry->top_entry, 0);
	assert_equal(cell_body_entry->size, (124 + sizeof(struct cell_body_entry)) - (2 * sizeof(struct cell_body_entry) + 86));

	xfree(buffer);
}


/**
 * Create the Test suite.
 */
TestSuite *cell_body_tests()
{
	TestSuite *suite = create_test_suite();

	add_test(suite, test_cell_body_init);
	add_test(suite, test_cell_body_add_entry);
	add_test(suite, test_cell_body_add_entries);
	add_test(suite, test_cell_body_add_entries_offset);
	add_test(suite, test_cell_body_add_entries_offset_overflow);
	add_test(suite, test_cell_body_add_entries_offset_overwrite_hole);
	add_test(suite, test_cell_body_add_entries_offset_over_top);
	add_test(suite, test_cell_body_add_entries_offset_small_buffer);
	add_test(suite, test_cell_body_add_entries_small_buffer_header_pushdown);
	add_test(suite, test_cell_body_add_entries_top_full);

	return suite;
}

CGREEN_TEST_MAIN(cell_body_tests);
