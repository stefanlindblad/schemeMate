#include "schemeMate_env.h"

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
