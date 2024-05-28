# Compiler
CC = cc

# Compiler flags
CFLAGS = $(shell pkg-config --cflags raylib)

# Linker flags
LDFLAGS = $(shell pkg-config --libs raylib)

# Source files
SRCS = main.c game.c fileio.c loopDetection.c superFastHash.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = main

# Allow overriding the executable name from the command line
ifdef EXE
	EXEC = $(EXE)
endif

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compile main.c with raylib cflags
main.o: main.c
	$(CC) -c main.c $(RAYLIB_CFLAGS) -o main.o

# Compile other source files without raylib cflags
%.o: %.c
	@ # Prevent main.c from being compiled by this rule
	if [ "$<" != "main.c" ]; then \
		$(CC) -c $< -o $@; \
	fi

# Clean up object files and executable
clean:
	rm -f $(OBJS)

.PHONY: all clean