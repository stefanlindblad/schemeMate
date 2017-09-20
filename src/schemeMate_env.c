#include "schemeMate_env.h"

sm_obj MAIN_ENV = NULL;

void init_environment(int RUNNING_MODE)
{
	MAIN_ENV = allocate_env(ENV_INIT_SIZE, NULL);
}

void shutdown_environment()
{
	free(MAIN_ENV);
	MAIN_ENV = NULL;
}

sm_obj allocate_env(unsigned env_size, sm_obj parent)
{
	unsigned bytes;
	sm_obj env;
	sm_entry entries;

	// allocate memory for the entries
	bytes = env_size * (unsigned) sizeof(struct sm_entry_struct);
	entries = (sm_entry) malloc(bytes);
	memset(entries, 0, bytes);

	// allocate the environment
	env = (sm_obj) malloc(sizeof(struct sm_env_type));
	env->sm_env.tag = TAG_ENV;
	env->sm_env.parent = parent;
	env->sm_env.used_slots = 0;
	env->sm_env.allocated_slots = env_size;
	env->sm_env.entries = entries;
	return env;
}

sm_obj grow_env(sm_obj oldEnv)
{
    int oldSize, newSize;
    sm_obj newEnv;

    oldSize = oldEnv->sm_env.allocated_slots;
    newSize = oldSize * 2 + 1;
    newEnv = allocate_env(newSize, oldEnv->sm_env.parent);

    for (int i = 0; i < oldSize; i++) {
		sm_obj oldKey = oldEnv->sm_env.entries[i].key;

		if (oldKey != NULL) {
	    	sm_obj oldValue;
	    	int startId, newId;

			oldValue = oldEnv->sm_env.entries[i].value;
	    	startId = newId = (int) ( object_hash(oldKey) % newSize );
	    	while (true) {
				if (newEnv->sm_env.entries[newId].key == NULL) {
					newEnv->sm_env.entries[newId].key = oldKey;
					newEnv->sm_env.entries[newId].value = oldValue;
		    		break;
				}
				newId = (newId + 1) % newSize;

				if (startId == newId)
					ERROR_CODE("Happened to do round trip in growing environment", 43);
	    		}
			}
    	}
    newEnv->sm_env.used_slots = oldEnv->sm_env.used_slots;
    return newEnv;
}

void add_binding(sm_obj key, sm_obj value, sm_obj *env)
{
    int hash_id = (int) (object_hash(key) % (*env)->sm_env.allocated_slots);
	int original_id = hash_id;
	sm_entry entry;

    while (true) {
		entry = &((*env)->sm_env.entries[hash_id]);
		if (entry->key == NULL) {
	    	entry->key = key;
			entry->value = value;
			(*env)->sm_env.used_slots++;
			if ((*env)->sm_env.used_slots > ((*env)->sm_env.allocated_slots * 3 / 4))
				*env = grow_env(*env);
	    	return;
		}
		if (entry->key == key) {
	    	entry->value = value;
	    	return;
		}

		hash_id = (hash_id + 1) % (*env)->sm_env.allocated_slots;

		if (hash_id == original_id)
			ERROR_CODE("Happened to do round trip in add binding", 43);
    }
}

sm_obj get_binding(sm_obj key, sm_obj env)
{
	ASSERT_SYMBOL(key);

	int hash_id;
	int original_id;
	sm_entry entry;

    while (true) {
		hash_id = (int) ( object_hash(key) % env->sm_env.allocated_slots );
		original_id = hash_id;
		while (true) {
			entry = &(env->sm_env.entries[hash_id]);
	    	if (entry->key == key)
				return (entry->value);

	    	if (entry->key == NULL) {
				if (env->sm_env.parent == NULL) {
		    		return NULL;
				}
				env = env->sm_env.parent;
				break;
	    	}
			hash_id = (hash_id+1) % env->sm_env.allocated_slots;
	    	if (hash_id == original_id)
				ERROR_CODE("Happened to do round trip in get binding", 43);
		}
    }
}
