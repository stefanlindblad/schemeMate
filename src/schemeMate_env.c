#include "schemeMate_env.h"

sm_env MAIN_ENV = NULL;

void init_environment(int RUNNING_MODE)
{
	MAIN_ENV = allocate_env(ENV_INIT_SIZE, NULL);
}

sm_env allocate_env(unsigned env_size, sm_env parent)
{
	unsigned bytes;
	sm_env env;
	sm_entry entries;

	// allocate memory for the entries
	bytes = env_size * (unsigned) sizeof(struct sm_entry_struct);
	entries = (sm_entry) malloc(bytes);
	memset(entries, 0, bytes);

	// allocate the environment
	env = (sm_env) malloc(sizeof(struct sm_env_struct));
	env->parent = parent;
	env->used_slots = 0;
	env->allocated_slots = env_size;
	env->entries = entries;
	return env;
}

sm_env grow_env(sm_env oldEnv)
{
    int oldSize, newSize;
    sm_env newEnv;

    oldSize = oldEnv->allocated_slots;
    newSize = oldSize * 2 + 1;
    newEnv = allocate_env(newSize, oldEnv->parent);

    // TODO check if it isnt enough to iterate used_slots
    for (int i = 0; i < oldSize; i++) {
		sm_obj oldKey = oldEnv->entries[i].key;

		if (oldKey != NULL) {
	    	sm_obj oldValue;
	    	int startId, newId;

	    	oldValue = oldEnv->entries[i].value;
	    	startId = newId = (int) ( object_hash(oldKey) % newSize );
	    	while (true) {
				if (newEnv->entries[newId].key == NULL) {
		    		newEnv->entries[newId].key = oldKey;
		    		newEnv->entries[newId].value = oldValue;
		    		break;
				}
				newId = (newId + 1) % newSize;

				if (startId == newId)
					ERROR_CODE("Happened to do round trip in growing environment", 43);
	    		}
			}
    	}
    newEnv->used_slots = oldEnv->used_slots;
    return newEnv;
}

void add_binding(sm_obj key, sm_obj value, sm_env *env)
{
    int hash_id = (int) (object_hash(key) % (*env)->allocated_slots);
	int original_id = hash_id;
	sm_entry entry;

    while (true) {
		entry = &((*env)->entries[hash_id]);
		if (entry->key == NULL) {
	    	entry->key = key;
			entry->value = value;
			(*env)->used_slots++;
			if ((*env)->used_slots > ((*env)->allocated_slots * 3 / 4))
				*env = grow_env(*env);
	    	return;
		}
		if (entry->key == key) {
	    	entry->value = value;
	    	return;
		}

		hash_id = (hash_id + 1) % (*env)->allocated_slots;

		if (hash_id == original_id)
			ERROR_CODE("Happened to do round trip in add binding", 43);
    }
}

sm_obj get_binding(sm_obj key, sm_env env)
{
	ASSERT_SYMBOL(key);

	int hash_id;
	int original_id;
	sm_entry entry;

    while (true) {
		hash_id = (int) ( object_hash(key) % env->allocated_slots );
		original_id = hash_id;
		while (true) {
	    	entry = &(env->entries[hash_id]);
	    	if (entry->key == key)
				return (entry->value);

	    	if (entry->key == NULL) {
				if (env->parent == NULL) {
		    		return NULL;
				}
				env = env->parent;
				break;
	    	}
	    	hash_id = (hash_id+1) % env->allocated_slots;
	    	if (hash_id == original_id)
				ERROR_CODE("Happened to do round trip in get binding", 43);
		}
    }
}
