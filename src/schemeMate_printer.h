#ifndef PRINTER_HEADER
#define PRINTER_HEADER

#include "schemeMate_objects.h"
#include "schemeMate_memory.h"

void sm_print(FILE* file, sm_obj o);
void sm_printList(FILE* file, sm_obj obj);
void sm_printRest(FILE* outFile, sm_obj cons);
void error(char *file, int line, char *message);

#endif // PRINTER_HEADER