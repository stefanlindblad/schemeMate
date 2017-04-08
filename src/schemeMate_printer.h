#pragma once
#include "schemeMate_objects.h"
#include "schemeMate_memory.h"

SCM_OBJ scm_read(SCM_STREAM input);
SCM_OBJ scm_readString(char * input);
static int skipSeparators(SCM_STREAM inStream);
static int readCharacter(SCM_STREAM inStream);
static void unreadCharacter(SCM_STREAM inStream);
static BOOL isSeparator(int aChar);