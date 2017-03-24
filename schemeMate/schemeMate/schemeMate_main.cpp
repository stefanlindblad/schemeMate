#include "schemeMate_main.h"

int main(int argc, char *argv[])
{
	SchemeMate* scheme_interpreter = new SchemeMate();
	scheme_interpreter->id = 4;

	std::cout << scheme_interpreter->id << std::endl;
	std::cin >> scheme_interpreter->id;
}