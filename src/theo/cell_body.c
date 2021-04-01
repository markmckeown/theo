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
#include <string.h>

#include "theo/cell_body.h"
#include "theo/cell_header.h"
#include "theo/cell_body_entry.h"
#include "theo/cell.h"
#include "theo/msys.h"

void cell_body_set_top(struct cell_body_entry *cell_body_entry)
{
	cell_body_entry->free_space = 1;
	cell_body_entry->top_entry = 1;
	cell_body_entry->size = 0;
	return;
}

void cell_body_init(struct cell_body *cell_body, struct cell *cell)
{
	memset(cell_body, 0, sizeof(struct cell_body));
	struct cell_body_entry *cell_body_entry;

	cell_body_entry = (struct cell_body_entry *)cell->slab;
	cell_body_set_top(cell_body_entry);
	cell_body_entry->previous_size = 0;
	cell_body->top_entry_offset = 0;
	cell_body->next_entry_offset = 0;

	return;
}

void cell_body_write_chunk(struct cell_body_entry *cell_body_entry,
			   struct checksum *checksum,
			   char *buffer, uint32_t buffer_size)
{
	char *ptr;

	ptr = (char *)cell_body_entry;
	ptr += sizeof(struct cell_body_entry);

	memcpy(&cell_body_entry->checksum, checksum, sizeof(struct checksum));
	// cell_body_entry->free_space will be zero.
	cell_body_entry->size = buffer_size;
	cell_body_entry->free_space = 0;
	memcpy(ptr, buffer, buffer_size);
	return;
}

uint32_t cell_body_free_space(struct cell_body *cell_body, struct cell *cell)
{
	uint32_t end_of_body;
	end_of_body =
	    cell_body->top_entry_offset + sizeof(struct cell_body_entry);
	return cell->slab_size - (end_of_body +
				  cell_header_bytes_used(cell->cell_header));
}

bool cell_body_add_top(struct cell_body *cell_body,
		       struct cell *cell,
		       struct checksum *checksum,
		       char *buffer,
		       uint32_t buffer_size,
		       struct cell_dir_entry *cell_dir_entry)
{
	bool ret = true;
	uint32_t free_space;
	uint32_t required_space;
	struct cell_body_entry *cell_body_entry;
	char *ptr;

	// Need room for buffer, new cell_body_entry for the new top and perhaps a new
	// cell_dir_entry if the cell_dir has overflowed.
	// If we use the top we need to add another cell_body_entry.
	required_space =
	    buffer_size + sizeof(struct cell_body_entry) +
	    sizeof(struct cell_dir_entry);
	free_space = cell_body_free_space(cell_body, cell);

	// Check we have free space first.
	if (free_space < required_space) {
		ret = false;
		goto out;
	}
	// Find cell_body_entry
	ptr = cell->slab + cell_body->top_entry_offset;
	cell_body_entry = (struct cell_body_entry *)ptr;

	// No longer top or free
	cell_body_entry->free_space = 0;
	cell_body_entry->top_entry = 0;

	// Set buffer size and copy in checksum and buffer
	cell_body_entry->size = buffer_size;
	memcpy(&cell_body_entry->checksum, checksum, sizeof(struct checksum));
	ptr += sizeof(struct cell_body_entry);
	memcpy(ptr, buffer, buffer_size);

	// Add to cell_dir
	cell_dir_entry_set(cell_dir_entry, checksum,
			   cell_body->top_entry_offset);
	cell_header_add(cell->cell_header, cell_dir_entry, buffer_size);

	// Add a new cell top entry
	ptr += buffer_size;
	cell_body_entry = (struct cell_body_entry *)ptr;
	cell_body_set_top(cell_body_entry);
	cell_body_entry->previous_size = buffer_size;

	// Reset the cell_body
	cell_body->top_entry_offset +=
	    buffer_size + sizeof(struct cell_body_entry);
	cell_body->next_entry_offset = cell_body->top_entry_offset;

out:
	return ret;
}

void cell_body_push_down_top(struct cell_body *cell_body, struct cell *cell,
			     struct cell_body_entry *cell_body_entry,
			     struct checksum *checksum, char *buffer,
			     uint32_t buffer_size,
			     struct cell_dir_entry *cell_dir_entry)
{
	cell_body_set_top(cell_body_entry);
	cell_body->top_entry_offset = cell_body->next_entry_offset;
	cell_body_add_top(cell_body, cell, checksum, buffer, buffer_size,
			  cell_dir_entry);
	return;
}

bool cell_body_next_entry_is_top(struct cell_body_entry *cell_body_entry)
{
	char *ptr;
	struct cell_body_entry *cell_body_entry_ptr;

	ptr = (char *)cell_body_entry;
	ptr += cell_body_entry->size + sizeof(struct cell_body_entry);
	cell_body_entry_ptr = (struct cell_body_entry *)ptr;
	return cell_body_entry_top(cell_body_entry_ptr);
}

void cell_body_add_dir_entry(struct cell *cell,
			     struct cell_body *cell_body,
			     struct cell_dir_entry *cell_dir_entry,
			     uint32_t entry_size)
{
	uint32_t offset;
	struct cell_body_entry *cell_body_entry;
	char *ptr;

	// Some wastage as we always make sure there is space in the
	// directory overflow even though we may not need the overflow
	// to add the directory entry.
	if (cell_body_free_space(cell_body, cell) >=
	    sizeof(struct cell_dir_entry)) {
		cell_header_add(cell->cell_header, cell_dir_entry, entry_size);
		goto out;
	}
	// Need to free up some space. Delete the entry below
	// top and move top back to free up space.
	offset = cell_body->top_entry_offset;
	ptr = cell->slab + offset;
	cell_body_entry = (struct cell_body_entry *)ptr;
	offset -=
	    (cell_body_entry->previous_size + sizeof(struct cell_body_entry));
	ptr = cell->slab + offset;
	cell_body_entry = (struct cell_body_entry *)ptr;
	// Entry below top should never free.
	ALWAYS(!cell_body_entry_free(cell_body_entry));
	// Remove the entry from the cell directory.
	cell_header_remove(cell->cell_header, cell_body_entry);
	cell_body_set_top(cell_body_entry);
	cell_body->top_entry_offset = offset;
	// Removing a single entry must provide enough space to add a directory entry.
	ALWAYS(cell_body_free_space(cell_body, cell) >=
	       sizeof(struct cell_dir_entry));
	cell_header_add(cell->cell_header, cell_dir_entry, entry_size);

out:
	return;
}

void cell_body_add_offset(struct cell_body *cell_body,
			  struct cell *cell,
			  struct checksum *checksum,
			  char *buffer,
			  uint32_t buffer_size,
			  struct cell_dir_entry *cell_dir_entry)
{
	uint32_t free_space;
	uint32_t required_space;
	struct cell_body_entry *cell_body_entry, *cell_body_entry_ptr;
	char *ptr;

	ptr = cell->slab + cell_body->next_entry_offset;
	cell_body_entry = (struct cell_body_entry *)ptr;

	if (!cell_body_entry_free(cell_body_entry)) {
		cell_header_remove(cell->cell_header, cell_body_entry);
	}
	cell_body_entry->free_space = 0;

	if (cell_body_entry->size == buffer_size) {
		// entry matches required size, just need to update the entry and
		// reset next_entry_offset
		cell_body_write_chunk(cell_body_entry, checksum, buffer,
				      buffer_size);
		cell_dir_entry_set(cell_dir_entry, checksum,
				   cell_body->next_entry_offset);
		cell_body_add_dir_entry(cell, cell_body, cell_dir_entry,
					buffer_size);
		cell_body->next_entry_offset +=
		    (sizeof(struct cell_body_entry) + buffer_size);
		goto out;
	}
	// Do we have enough space to add entry.
	cell_body_entry_ptr = cell_body_entry;
	free_space = cell_body_entry->size;
	required_space = buffer_size + sizeof(struct cell_body_entry);
	while (free_space < required_space) {
		ptr +=
		    cell_body_entry_ptr->size + sizeof(struct cell_body_entry);
		cell_body_entry_ptr = (struct cell_body_entry *)ptr;
		if (cell_body_entry_top(cell_body_entry_ptr)) {
			// Hit the top --- reset the cell_body_entry so that it is now the top
			// and pass over to be handled by cell_body_add_top.
			cell_body_push_down_top(cell_body, cell,
						cell_body_entry, checksum,
						buffer, buffer_size,
						cell_dir_entry);
			goto out;
		}

		if (!cell_body_entry_free(cell_body_entry_ptr)) {
			cell_header_remove(cell->cell_header,
					   cell_body_entry_ptr);
		}
		free_space +=
		    cell_body_entry_ptr->size + sizeof(struct cell_body_entry);
	}

	// Check we have not reached the top
	if (cell_body_next_entry_is_top(cell_body_entry_ptr)) {

		// next entry is actually the top.
		cell_body_push_down_top(cell_body, cell, cell_body_entry,
					checksum, buffer, buffer_size,
					cell_dir_entry);
		goto out;
	}

	cell_body_write_chunk(cell_body_entry, checksum, buffer, buffer_size);
	memcpy(&cell_dir_entry->checksum, checksum, sizeof(struct checksum));
	cell_dir_entry_set(cell_dir_entry, checksum,
			   cell_body->next_entry_offset);
	cell_body_add_dir_entry(cell, cell_body, cell_dir_entry, buffer_size);

	// Need fix the any free space up with a cell_body_entry
	free_space -= required_space;
	cell_body->next_entry_offset +=
	    sizeof(struct cell_body_entry) + buffer_size;
	ptr = cell->slab + cell_body->next_entry_offset;
	cell_body_entry_ptr = (struct cell_body_entry *)ptr;
	cell_body_entry_init(cell_body_entry_ptr);
	cell_body_entry_ptr->previous_size = buffer_size;
	cell_body_entry_ptr->size = free_space;
	cell_body_entry_ptr->free_space = 1;
	cell_body_entry_ptr->top_entry = 0;
	// Fix back pointer of next entry
	ptr += sizeof(struct cell_body_entry) + free_space;
	cell_body_entry_ptr = (struct cell_body_entry *)ptr;
	cell_body_entry_ptr->previous_size = free_space;

out:
	return;
}

void cell_body_add(struct cell_body *cell_body,
		   struct cell *cell,
		   struct checksum *checksum,
		   char *buffer,
		   uint32_t buffer_size, struct cell_dir_entry *cell_dir_entry)
{
	char *ptr;
	struct cell_body_entry *next_cell_body_entry;

	ptr = cell->slab + cell_body->next_entry_offset;
	next_cell_body_entry = (struct cell_body_entry *)ptr;

	if (!cell_body_entry_top(next_cell_body_entry)) {
		// Not at top - add at offset
		cell_body_add_offset(cell_body, cell, checksum, buffer,
				     buffer_size, cell_dir_entry);
		goto out;
	}
	// Try to add to top.
	if (!cell_body_add_top
	    (cell_body, cell, checksum, buffer, buffer_size, cell_dir_entry)) {
		// Adding to top failed, reset to start of cell and insert.
		cell->cell_header->cell_body.next_entry_offset = 0;
		cell_body_add_offset(cell_body, cell, checksum, buffer,
				     buffer_size, cell_dir_entry);
	}
out:
	return;
}
