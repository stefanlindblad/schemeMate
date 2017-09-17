#ifndef ENV_HEADER
#define ENV_HEADER

#include "schemeMate_objects.h"

#define ENV_INIT_SIZE 10

static inline unsigned long object_hash(sm_obj o)
{
    return (unsigned long) o;
}

void init_environment();
sm_env allocate_env(unsigned env_size, sm_env env);
sm_env grow_env(sm_env env);
void add_binding(sm_obj key, sm_obj value, sm_env *env);
sm_obj get_binding(sm_obj key, sm_env env);

#endif // ENV_HEADER
