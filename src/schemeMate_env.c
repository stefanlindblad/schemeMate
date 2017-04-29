#include "schemeMate_env.h"

void init_environment()
{
	main_environment = allocate_env(ENV_INIT_SIZE);
}

sm_env allocate_env(unsigned env_size)
{
	unsigned bytes;
	sm_env env;

	bytes = (unsigned) sizeof(struct sm_environment) + (env_size-1) * (unsigned) sizeof(struct sm_entry);
	env = (sm_env) malloc(bytes);

	// fills the memory for env with zeros
	memset(env, 0, bytes); 

	env->used_slots = 0;
	env->allocated_slots = env_size;

	return env;
}

void grow_env(sm_env oldEnv)
{
    int oldSize, newSize;
    sm_env newEnv;

    oldSize = oldEnv->allocated_slots;
    newSize = oldSize * 2 + 1;
    newEnv = allocate_env(newSize);

    // TODO check if it isnt enough to iterate used_slots
    for (int i = 0; i < oldSize; i++) {
		sm_obj oldKey = oldEnv->entries[i].key;

		if (oldKey != NULL) {
	    	sm_obj oldValue;
	    	int startId, newId;

	    	oldValue = oldEnv->entries[i].value;
	    	startId = newId = (int) ( object_hash(oldKey) % newSize );
	    	for (;;) {
				if (newEnv->entries[newId].key == NULL) {
		    		newEnv->entries[newId].key = oldKey;
		    		newEnv->entries[newId].value = oldValue;
		    		break;
				}
				newId = (newId + 1) % newSize;

				if (startId == newId)
		    		ERROR("Happened to do round trip in growing environment");
	    		}
			}
    	}
    newEnv->used_slots = oldEnv->used_slots;
    oldEnv = newEnv;
}

void add_binding(sm_obj key, sm_obj value, sm_env env)
{
    int hash_id = (int) (object_hash(key) % env->allocated_slots);
	int original_id = hash_id;
	struct sm_entry *entry;

    while (true) {
		entry = &(env->entries[hash_id]);
		if (entry->key == NULL) {
	    	entry->key = key;
	    	entry->value = value;
	    	env->used_slots++;
	    	if (env->used_slots > (env->allocated_slots * 3 / 4))
				grow_env(env);
	    	return;
		}
		if (entry->key == key) {
	    	entry->value = value;
	    	return;
		}

		hash_id = (hash_id + 1) % env->allocated_slots;

		if (hash_id == original_id)
	    	ERROR("Happened to do round trip in add binding");
    }
}
