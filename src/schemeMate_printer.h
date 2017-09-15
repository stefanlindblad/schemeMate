#ifndef PRINTER_HEADER
#define PRINTER_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_memory.h"
#include "schemeMate_main.h"
#include "schemeMate_reader.h"

void sm_print(sm_obj obj, bool oneLineMode);
void sm_printList(sm_obj obj, bool oneLineMode);
void sm_printRest(sm_obj cons, bool oneLineMode);
void info(char *message, char *file, int line);
void error(char *message, char *file, int line, int exit_code);

#endif // PRINTER_HEADER