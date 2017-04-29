# Global variables
bindir = bin/
src = src/
cStd = c99
cppStd = c++11
debugFlag = -g

# Main build target
all: compile link

# Compile Commands
compile:
	gcc $(debugFlag) -c -w -std=$(cStd) -o $(bindir)schemeMate_memory.o $(src)schemeMate_memory.c
	gcc $(debugFlag) -c -w -std=$(cStd) -o $(bindir)schemeMate_env.o $(src)schemeMate_env.c
	gcc $(debugFlag) -c -w -std=$(cStd) -o $(bindir)schemeMate_reader.o $(src)schemeMate_reader.c
	gcc $(debugFlag) -c -w -std=$(cStd) -o $(bindir)schemeMate_printer.o $(src)schemeMate_printer.c
	gcc $(debugFlag) -c -w -std=$(cStd) -o $(bindir)schemeMate_selftest.o $(src)schemeMate_selftest.c
	gcc $(debugFlag) -c -w -std=$(cStd) -o $(bindir)schemeMate_main.o	$(src)schemeMate_main.c

# Link Commands
link:
	g++ -o schemeMate $(bindir)schemeMate_memory.o $(bindir)schemeMate_env.o $(bindir)schemeMate_reader.o $(bindir)schemeMate_printer.o $(bindir)schemeMate_selftest.o $(bindir)schemeMate_main.o

# Removing the Binary
clean:
	del /q ".\bin\*"
	del schemeMate.exe