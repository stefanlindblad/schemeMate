#ifndef ENV_HEADER
#define ENV_HEADER

#include "schemeMate_objects.h"

#define ENV_INIT_SIZE 10

static inline unsigned long object_hash(sm_obj o)
{
    return (unsigned long) o;
}

void init_environment(int RUNNING_MODE);
sm_obj allocate_env(unsigned env_size, sm_obj env);
sm_obj grow_env(sm_obj env);
void add_binding(sm_obj key, sm_obj value, sm_obj *env);
sm_obj get_binding(sm_obj key, sm_obj env);

#endif // ENV_HEADER
