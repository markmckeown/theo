#include <string.h>

#include "theo/cell_body.h"
#include "theo/cell_body_entry.h"
#include "theo/cell.h"

void cell_body_init(struct cell_body *cell_body, struct cell *cell) {
	memset(cell_body, 0, sizeof(struct cell_body));
	struct cell_body_entry *cell_body_entry;

	cell_body_entry = (struct cell_body_entry *) cell->slab;
	cell_body_entry->free_space = 1;
	cell_body_entry->top_entry = 1;
	cell_body_entry->size = 0;
	cell_body_entry->previous_size = 0;
	cell_body->top_entry_offset = 0;
       	cell_body->next_entry_offset = 0;	

	return;
}


void cell_body_write_chunk(struct cell_body_entry *cell_body_entry,
		struct checksum *checksum, 
		char *buffer, 
		uint32_t buffer_size) {
	char* ptr;

	ptr  = (char *) cell_body_entry;
	ptr += sizeof(struct cell_body_entry); 

	memcpy(&cell_body_entry->checksum, checksum, sizeof(struct checksum));
	// cell_body_entry->free_space will be zero.
	cell_body_entry->size = buffer_size;
	cell_body_entry->free_space = 0;
	memcpy(ptr, buffer, buffer_size);
	return;
}


uint32_t cell_body_free_space(struct cell_body *cell_body,
		struct cell *cell) {
	uint32_t end_of_body;
	end_of_body = cell_body->top_entry_offset + sizeof(struct cell_body_entry);
	return cell->slab_size - (end_of_body + cell_header_bytes_used(cell->cell_header));
}


bool cell_body_add_top(struct cell_body *cell_body,
		struct cell *cell,
		struct checksum *checksum,
		char *buffer,
		uint32_t buffer_size,
		struct cell_dir_entry *cell_dir_entry) {
	bool ret = true;
	uint32_t free_space;
	uint32_t required_space;
	struct cell_body_entry *cell_body_entry;
	char *ptr;

	// Need room for buffer, new cell_body_entry for the new top and perhaps a new
	// cell_dir_entry if the cell_dir has overflowed.
	required_space = buffer_size + sizeof(struct cell_body_entry) + sizeof(struct cell_dir_entry);
	free_space = cell_body_free_space(cell_body, cell);

	// Check we have free space first.
	if (free_space < required_space) {
		ret = false;
		goto out;
	}
	
	// Find cell_body_entry
	cell_body_entry = (struct cell_body_entry *)  cell->slab + cell_body->top_entry_offset;
	
	// No longer top or free
	cell_body_entry->free_space = 0;
	cell_body_entry->top_entry = 0;

	// Set buffer size and copy in checksum and buffer
	cell_body_entry->size = buffer_size;
	memcpy(&cell_body_entry->checksum, checksum, sizeof(struct checksum));
	ptr = (char *) cell_body_entry;
        ptr = ptr + sizeof(struct cell_body_entry);
	memcpy(ptr, buffer, buffer_size);	
	
	// Add to cell_dir
	cell_dir_entry_set(cell_dir_entry, checksum, buffer_size, cell_body->top_entry_offset);
	cell_dir_add(&cell->cell_header->cell_dir, (char *) cell->cell_header,
		   cell_dir_entry);

	// Add a new cell top entry
	cell_body_entry = (struct cell_body_entry *) (ptr + buffer_size);
	cell_body_entry->free_space = 1;
	cell_body_entry->top_entry = 1;
	cell_body_entry->previous_size = buffer_size;

	// Reset the cell_body
	cell_body->top_entry_offset += buffer_size + sizeof(struct cell_body_entry);
	cell_body->next_entry_offset = cell_body->top_entry_offset;

out:
	return ret; 
}

void cell_body_add_dir_entry(struct cell *cell, 
		struct cell_body *cell_body, 
		struct cell_dir_entry *cell_dir_entry) {
	uint32_t offset;
	struct cell_body_entry *cell_body_entry;
	char *ptr;

	if (cell_body_free_space(cell_body, cell) >= sizeof(struct cell_dir_entry)) {
		cell_dir_add(&cell->cell_header->cell_dir, (char *) cell->cell_header,
		   cell_dir_entry);
		goto out;
	}

	// Need to free up some space.
	offset = cell_body->top_entry_offset;
	ptr = cell->slab + offset;
	cell_body_entry = (struct cell_body_entry *) ptr;
	offset -= (cell_body_entry->previous_size + sizeof(struct cell_body_entry));
	ptr = cell->slab + offset;
	cell_body_entry = (struct cell_body_entry *) ptr;
	if (!cell_body_entry_free(cell_body_entry)) {
		cell_dir_remove(&cell->cell_header->cell_dir, cell->slab - sizeof(struct cell_header),
                                &cell_body_entry->checksum);
	}
	cell_body_entry->free_space = 1;
	cell_body_entry->top_entry = 1;
	if (cell_body->top_entry_offset == cell_body->next_entry_offset) {
		// Before we pushed top down it was the next_entry_offset so
		// need to reset next_entry_offset
		cell_body->top_entry_offset = offset;
		cell_body->next_entry_offset = offset;
	} else {
		// next_entry_offset did not point at top, so no need to change it.
		cell_body->top_entry_offset = offset;
	}
	cell_dir_add(&cell->cell_header->cell_dir, (char *) cell->cell_header,
		   cell_dir_entry);

out:	
	return;
}


void cell_body_add_offset(struct cell_body *cell_body,
		struct cell *cell,
		struct checksum *checksum,
		char *buffer,
		uint32_t buffer_size,
		struct cell_dir_entry *cell_dir_entry) {
	uint32_t free_space;
	uint32_t required_space;
	struct cell_body_entry *cell_body_entry, *cell_body_entry_ptr;
	char *ptr;

	required_space = buffer_size + sizeof(struct cell_body_entry);
	cell_body_entry = (struct cell_body_entry *) (cell->slab + cell_body->next_entry_offset);
	ptr = cell->slab + cell_body->next_entry_offset;

	if (!cell_body_entry_free(cell_body_entry)) {
		cell_dir_remove(&cell->cell_header->cell_dir, cell->slab - sizeof(struct cell_header), 
				&cell_body_entry->checksum);
		cell_body_entry->free_space = 1;
	}

	cell_body_entry_ptr = cell_body_entry;
	free_space = cell_body_entry->size;
	while (free_space < required_space) {
		ptr += cell_body_entry_ptr->size + sizeof(cell_body_entry);
		cell_body_entry_ptr = (struct cell_body_entry *) ptr;
		if (cell_body_entry_top(cell_body_entry_ptr)) {
			// Hit the top --- reset the cell_body_entry so that it is now the top
			// and pass over to be handled cell_body_add_top
			cell_body_entry->top_entry = 1;
			cell_body_add_top(cell_body, cell, checksum, buffer, buffer_size, cell_dir_entry);
			goto out;
		}

		if (!cell_body_entry_free(cell_body_entry_ptr)) {
			cell_dir_remove(&cell->cell_header->cell_dir, cell->slab - sizeof(struct cell_header),
                                &cell_body_entry_ptr->checksum);
		}
		free_space += cell_body_entry_ptr->size + sizeof(cell_body_entry);
	}

	cell_body_write_chunk(cell_body_entry, checksum, buffer, buffer_size);
	memcpy(&cell_dir_entry->checksum, checksum, sizeof(struct checksum));
	cell_dir_entry->offset = cell_body->next_entry_offset;
	cell_dir_entry->size = buffer_size;
	cell_body_add_dir_entry(cell, cell_body, cell_dir_entry);

	// Need fix the any free space up with a cell_body_entry
	free_space -= required_space;
	cell_body->next_entry_offset += required_space;
	cell_body_entry_ptr = (struct cell_body_entry *) (cell->slab + cell_body->next_entry_offset);
	cell_body_entry_init(cell_body_entry_ptr);
	cell_body_entry_ptr->previous_size = buffer_size;
	cell_body_entry_ptr->size = free_space;
	cell_body_entry_ptr->free_space = 1;
	cell_body_entry_ptr->top_entry = 0;

out:
	return;
}


void cell_body_add(struct cell_body *cell_body,
		struct cell *cell,
		struct checksum *checksum,
		char *buffer,
		uint32_t buffer_size,
		struct cell_dir_entry *cell_dir_entry) {
	struct cell_body_entry *next_cell_body_entry;

	next_cell_body_entry = (struct cell_body_entry *) (cell->slab + cell_body->next_entry_offset);
	if (cell_body_entry_top(next_cell_body_entry)) {
		if (cell_body_add_top(cell_body, cell, checksum, buffer, buffer_size, cell_dir_entry)) {
			goto out;
		} else {
			cell->cell_header->cell_body.next_entry_offset = 0;
		}
	}

	cell_body_add_offset(cell_body, cell, checksum, buffer, buffer_size, cell_dir_entry);

out:
	return;
}

