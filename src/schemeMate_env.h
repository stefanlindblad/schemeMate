#ifndef ENV_HEADER
#define ENV_HEADER

struct sm_entry {
    sm_obj key;
    sm_obj value;
};

struct environment {
    unsigned used_slots;
    unsigned allocated_slots;
    struct sm_entry entries[1];
};

static struct environment* allocate_env(unsigned env_size);

#endif // ENV_HEADER
