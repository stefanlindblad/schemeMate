#ifndef ENV_HEADER
#define ENV_HEADER

#include "schemeMate_objects.h"

static inline unsigned object_hash(sm_obj o)
{
    return (unsigned) o;
}

sm_env allocate_env(unsigned env_size);
void grow_env(sm_env env);

#endif // ENV_HEADER
