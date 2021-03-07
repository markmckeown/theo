#include <string.h>

#include "theo/cell_dir_overflow.h"
#include "theo/msys.h"

void cell_dir_overflow_init(struct cell_dir_overflow *cell_dir_overflow) {
	memset(cell_dir_overflow, 0, sizeof(struct cell_dir_overflow));
	return;
}

uint32_t cell_dir_overflow_add(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
                struct cell_dir_entry *cell_dir_entry) {
	char* entry;
	struct cell_dir_entry *ptr;
	       
	cell_dir_overflow->overflow_count++;
	entry = buffer_top - (cell_dir_overflow->overflow_count * sizeof(struct cell_dir_entry));
	ptr = (struct cell_dir_entry*) entry;
	cell_dir_entry_copy(ptr, cell_dir_entry);
	return cell_dir_overflow->overflow_count;
}


bool cell_dir_overflow_remove(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum){
	bool ret = false;
	char* entry;
	struct cell_dir_entry *cell_dir_entry, *ptr;
	int i = 0;
	
	entry = buffer_top - (cell_dir_overflow->overflow_count * sizeof(struct cell_dir_entry));
	cell_dir_entry = (struct cell_dir_entry*) entry;

	if (cell_dir_overflow->overflow_count == 1) {
		if (checksum_compare(&cell_dir_entry->checksum, checksum) == 0) {
			memset(cell_dir_entry, 0, sizeof(struct cell_dir_entry));
			cell_dir_overflow->overflow_count = 0;
			ret = true;
		}
		goto out;
	}

	ptr = cell_dir_entry;
	for (i = 0; i < cell_dir_overflow->overflow_count; i++, ptr++) {
		if (checksum_compare(&ptr->checksum, checksum) == 0) {
			cell_dir_entry_copy(ptr, cell_dir_entry);
			memset(cell_dir_entry, 0, sizeof(struct cell_dir_entry));
			cell_dir_overflow->overflow_count--;
			ret = true;
			break;
		}
	}

out:
	return ret; 
}


bool cell_dir_overflow_get_chunk(struct cell_dir_overflow *cell_dir_overflow, char *buffer_top,
		struct checksum *checksum, struct cell_dir_entry *cell_dir_entry) {
	bool ret = false;
	char* entry;
	struct cell_dir_entry *ptr;
	int i = 0;
	
	if (cell_dir_overflow->overflow_count == 0) {
		goto out;
	}

	entry = buffer_top - (cell_dir_overflow->overflow_count * sizeof(struct cell_dir_entry));
	ptr = (struct cell_dir_entry*) entry;
	for (i = 0; i < cell_dir_overflow->overflow_count; i++, ptr++) {
		if (checksum_compare(&ptr->checksum, checksum) == 0) {
			cell_dir_entry_copy(cell_dir_entry, ptr);
			ret = true;
			break;
		}
	}

out:
	return ret;	
}

