#ifndef READER_HEADER
#define READER_HEADER
#include "schemeMate_objects.h"

#define EOF_CHAR    ((sm_char)-1)
#define INIT_BUFFER_SIZE 32

sm_obj sm_read(sm_stream input);
sm_obj sm_readString(char * input);
static int skipSeparators(sm_stream inStream);
static int readCharacter(sm_stream inStream);
static void unreadCharacter(sm_stream inStream);
static sm_bool isSeparator(int aChar);

#endif // READER_HEADER