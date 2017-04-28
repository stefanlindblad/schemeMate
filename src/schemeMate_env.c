#include "schemeMate_env.h"

struct environment* environment = NULL;

static struct environment* allocate_env(unsigned env_size)
{
	unsigned bytes;
	struct environment* env;

	bytes = (unsigned) sizeof(struct environment) + (env_size-1) * (unsigned) sizeof(struct entry);

	env = (struct environment*) malloc(bytes);
	memset(env, 0, bytes); // What is this doing again?

	env->used_slots = 0;
	env->allocated_slots = env_size;

	return env;
}

