# CSF Assignment 4 Makefile

app = imgproc
# C source files
C_SRCS = expose.c imgproc.c image.c mirrorh.c mirrorv.c pnglite.c swapbg.c tile.c
# Executables to build for the "all" target
exec = expose.o imgproc.o image.o mirrorh.o mirrorv.o pnglite.o swapbg.o tile.o
# Plugins to generate for the "all" target
lib = mirrorh.so mirrorv.so tile.so expose.so swapbg.so

# C compiler flags
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu99 -fPIC -c
# Flags to use when linking an executable
LDFLAGS = -lz -ldl

# Rule for compiling and assembling a .c file to produce a .o file
%.o: %.c 
	gcc $(CFLAGS) $*.c -o $*.o

# Rule to build a comple image-processing plugin
%.so: mirrorh.o mirrorv.o tile.o expose.o swapbg.o
	mkdir -p plugins
	gcc -o plugins/$*.so -shared $*.o

# Default target: build all executables
all: $(app) $(exec) $(lib)

# Rule for compiling and assembling imgproc
imgproc: imgproc.o image.o pnglite.o 
	gcc -export-dynamic -o imgproc imgproc.o image.o pnglite.o $(LDFLAGS)

# Clean up generated files (such as object files and executables)
clean:
	rm -f $(app) $(exec) $(lib) depend.mak
	rm -r plugins

depend.mak:
	touch $@

# This target generates C header file dependencies (so that C modules
# get recompiled whenever their source or included header files get
# modified).
depend:
	  g++ -M $(CFLAGS) $(C_SRCS) >> depend.mak

include depend.mak
