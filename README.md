# WANOpt Byte Cache with DAX

WANOpt dedupe cache is a byte cache of network traffic.
Repeated byte patterns in network traffic can be replaced 
with a reference to the set of bytes thus offloading data 
from the wire. This is Data Deduplication. In a similar
way Data Deduplication can be used to remove duplicate
byte sequences on a storage device.

The larger the Data Deduplication cache the better the performance of
the WANOpt device, this quickly leads to the case that 
data has to be paged to and from disk. Paging to and from
disk impacts performance and care must be taken to optimise
performance. 

DAX (https://www.kernel.org/doc/Documentation/filesystems/dax.txt) 
provides direct access to Non Volatile Main
Memory, however care must be taken when writing to NVMM using
DAX to make sure data is flushed from CPU caches and to 
ensure the cache does not become corrupted.

This is an implementation of Data Deduplication that is targetted
for NVMM (Non-Volatile Main Memory). The behaviour of
NVMM is different from conventional disk block storage - 
its byte addressible, there is no paging and no guarantuee
that a complete "page" is written to disk.

Note that we need to handle system crashes - as the system is a 
cache we can lose some small amount of data but we cannot 
compromise the integrity of the cache. Care needs to taken
as the CPU will cache data before writing back to memory -
we need to flush this data in a similar way to how
fysnc is used to flush disk writes. 

# chunk_streamer


chunk_streamer is a chunker that uses an Intel Rolling
Hash for chunking. It stores new chunks in the cache_manager.

test/test_chunk_streamer_file.c shows the cache and chunking
algrothim working together in an end-to-end way.

# cell

Proposed solution is to design a cache based on a fixed size 
file. Using a fixed size file means that when used with DAX
all access will be non-blocking, there are no file meta data
operations. The file will be explicitly zero filled initially. 

The file will be split into equally sized "cells" with
each cell able to hold some number of chunks. Note chunks are 
of unequal length. Each chunk will have associated with it
a SHA-256 checksum. The checksum will be used to index to the 
cell that stores the chunk. So given a SHA-256 there is a 
linear mapping to a cell, eg we use some subset of
the SHA-256 as an index to the cell. It can be imagined there 
could be the order of millions of cells (current cell size is
1MB).

The cell will have two parts, the body and the head. The
head will be at top of the cell and will grow down, it will
contain an index into the body of the cell which will hold
the chunk data. The body will contain the chunk data and will grow 
up from the bottom of the cell. When the cell is full we
start at the bottom of the cell again overwriting/deleting
data in the cell - this is GC of the cache storage. The cell
is essentially a ring buffer for cache chunks.

Thread safety can be provided by simple mutex, as there
are so many cells there should be low conflict and read-write
locks are more expensive. Each cell will have its own mutex
and this will not be stored in NVMM. There should be no IO 
blocking behaviour when holding a lock -- the cache file will be
pre-allocated and of fixed size, under DAX this should
mean no blocking (of course the CPU can switch out the thread).

Cell integrity -- will be handled by two special uint32_t in 
the cell meta data, entry_counter and exit_counter. The cell is
corrupt if these two numbers do not match and should be
re-initialised.

High level approach to adding chunk to cell.

mutex lock on cell
check entry_counter and exit_counter match 
  if not then reset cell.
increment entry_counter --- cell is now corrupt.
use memory barrier to flush entry_counter to persistent memory.
update ring buffer
update cell meta data
increment exit_count 
use memory barrier to flush exit_counter to persistent memory -- cell is nolonger corrupt.
unlock mutex

The CPU can reorder memory write back from the cache to memory, and
in this case persistent memory. So we need to use memory barriers to
insure the counter and flip-counter are set into persistent memory.

# cache


The cache is a slab of memory that is broken into 1MB areas,
the first 1MB area holds metadata about the cache and the rest
of the 1MB areas are cells. The cache metadata has a mechanism
to allow cache integrity to be checked. 

The cache is initialised by passing it a chunk of NVMM memory,
eg mmap'd using DAX.

When a chunk is added to the cache the correct cell is identified
using mapping form the checksum to cell, eg modulus against the
first 8 bytes of the checksum against the number of cells. 
Similarly when looking up a chunk.

Project aims to have very high line and branch coverage.
Unit tests are run with Valgrind to detect memory issues.
Unit tests are run with Helgrin to detect threading and race conditions.
cpd is used for copy and paste detection.
pmccabe is used to complexity.

cgreen library is used for unit tests and mocking.

# build targets:

make - build library
make test - run unit tests
make test-debug - run unit tests in debug mode.
make test-coverage - run coverage with tests. Target is 100% line and branch
                     coverage.
make test-valgrind - run tests with valgrind - memory issues.
make test-helgrind - run tests with helgring - threading and mutex issues.
make cpd - check for copy and pasting.
make pmccabe - check code complexity.
make third-party - build any third party libraries, cgreen.
make cachegrind - run cachegrind against system.

To run a particular test with gdb/cgdb:

LD_LIBRARY_PATH=:generated/lib/x86_64-linux-gnu:generated/lib:generated/debug/lib cgdb --args generated/debug/cgreen/test_cell_dir_block.exe test_cell_dir_block_remove
