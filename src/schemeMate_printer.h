#ifndef PRINTER_HEADER
#define PRINTER_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_memory.h"

void sm_print(FILE* file, sm_obj o);
void sm_printList(FILE* file, sm_obj obj);
void sm_printRest(FILE* outFile, sm_obj cons);
void info(char *message, char *file, int line);
void error(char *message, char *file, int line, int exit_code);

#endif // PRINTER_HEADER