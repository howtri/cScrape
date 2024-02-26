# Run: make -f Makefile
# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -std=c17 -g -Wall

# Source files for this program
SRCS = program_main.c program_extra.c

# Header files
HEADERS = program_main.h program_extra.h

# Executable name
TARGET = program

# Build rule
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule
clean:
	rm -f $(TARGET)

# Phony target to prevent conflicts with files named 'clean' or 'all'
.PHONY: clean