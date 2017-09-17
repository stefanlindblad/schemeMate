#include "schemeMate_trampoline.h"

void init_trampoline()
{

}

void execute_trampoline(void_ptr_ptr_func function)
{
	while (function != NULL) {
		function = (void_ptr_ptr_func)((*function)());
	}
}
