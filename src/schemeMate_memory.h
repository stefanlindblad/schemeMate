#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include "schemeMate_objects.h"

sm_obj new_integer(int value);
sm_obj new_EOF();
sm_obj new_cons(sm_obj car, sm_obj cdr);
sm_obj new_nil();

void init_memory();

#endif // MEMORY_HEADER