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

void init_trampoline(int running_mode);
void shutdown_trampoline();
void execute_trampoline(void_ptr_ptr_func function);
void grow_cp_stack();
void_ptr_ptr_func contparse_initial_eval();
void_ptr_ptr_func contparse_func_eval();
void_ptr_ptr_func contparse_args_eval();
void_ptr_ptr_func contparse_user_func_eval();
void_ptr_ptr_func contparse_body_start_eval();
void_ptr_ptr_func contparse_body_cont_eval();

// Built in cp based syntax
void_ptr_ptr_func contparse_define_front();
void_ptr_ptr_func contparse_define_back();
void_ptr_ptr_func contparse_set_front();
void_ptr_ptr_func contparse_set_back();
void_ptr_ptr_func contparse_lambda();
void_ptr_ptr_func contparse_display();
void_ptr_ptr_func contparse_quote();
void_ptr_ptr_func contparse_if_front();
void_ptr_ptr_func contparse_if_back();

#endif // TRAMPOLINE_HEADER
