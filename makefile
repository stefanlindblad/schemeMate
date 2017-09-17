# Global variables
bindir = bin/
src = src/
cStd = c99
flags = -g -c -w

# Main build target
all: checkdir compile link

# check for build dir
checkdir:
	if not exist "%cd%\bin" mkdir bin

# Compile Commands
compile:
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_memory.o $(src)schemeMate_memory.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_functions.o $(src)schemeMate_functions.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_env.o $(src)schemeMate_env.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_eval.o $(src)schemeMate_eval.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_reader.o $(src)schemeMate_reader.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_printer.o $(src)schemeMate_printer.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_selftest.o $(src)schemeMate_selftest.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_trampoline.o $(src)schemeMate_trampoline.c
	gcc $(flags) -std=$(cStd) -o $(bindir)schemeMate_main.o	$(src)schemeMate_main.c

# Link Commands
link:
	g++ -o schemeMate $(bindir)schemeMate_memory.o $(bindir)schemeMate_trampoline.o $(bindir)schemeMate_functions.o $(bindir)schemeMate_env.o $(bindir)schemeMate_eval.o $(bindir)schemeMate_reader.o $(bindir)schemeMate_printer.o $(bindir)schemeMate_selftest.o $(bindir)schemeMate_main.o

# Removing the Object files / binaries
clean:
	del /q ".\bin\*"
	del schemeMate.exe
