#pragma once
#include <stdlib.h>
#include "schemeMate_objects.h"

SCM_OBJ new_integer(int iVal);
SCM_OBJ new_EOF();
SCM_OBJ new_cons(SCM_OBJ car, SCM_OBJ cdr);
SCM_OBJ new_nil();

void init_system();
void deinit_system();
