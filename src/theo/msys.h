/*
 * msys.h
 *
 */

#ifndef MSYS_H_
#define MSYS_H_

#include <assert.h>

#if defined(COVERAGE)
# define ALWAYS(X)
# define NEVER(X)
#else
# define ALWAYS(X)      assert(X)
# define NEVER(X)       assert(!(X))
#endif

#endif /* MSYS_H_ */
