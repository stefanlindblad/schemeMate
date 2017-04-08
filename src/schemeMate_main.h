#pragma once
#include "schemeMate_objects.h"
#include "schemeMate_selftest.h"
#include <stdlib.h>

int main(int argc, char *argv[]);


/* macro functions to check 

static inline BOOL isCons(SCM_OBJ obj)
{
	return hasTag(obj, TAG_CONS);
}

static inline SCM_OBJ car(SCM_OBJ obj)
{
	return hasTag(obj, TAG_CONS);
}

*/