#ifndef TEST_H
#define TEST_H

#include <assert.h>

#define TEST_ASSERT(condition) \
  do { \
    test_count++; \
    assert(condition); \
  } while(0)

#endif