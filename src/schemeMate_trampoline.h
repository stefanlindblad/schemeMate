#ifndef TRAMPOLINE_HEADER
#define TRAMPOLINE_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_eval.h"

#define INIT_CONT_STACK_SIZE 64

void_ptr_ptr_func* continuation_stack;
void_ptr_ptr_func* continuation_stack_pointer;
void_ptr_ptr_func* continuation_stack_top;

// inline functions

static inline void SAVE_CP(void_ptr_ptr_func func)
{
    *continuation_stack_pointer++ = func;
    if (continuation_stack_pointer >= continuation_stack_top)
        grow_cp_stack();
}

static inline void_ptr_ptr_func LOAD_CP()
{
    if (continuation_stack_pointer <= continuation_stack)
        ERROR_CODE("Continuation Stack Underflow!", 48);
    return *--continuation_stack_pointer;
}

void init_trampoline();
void execute_trampoline(void_ptr_ptr_func function);
void grow_cp_stack();
void_ptr_ptr_func contparse_initial_eval();


#endif // TRAMPOLINE_HEADER
