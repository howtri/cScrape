# Run: make
# Test: make test
# Compiler to use
CC = gcc

# Compiler flags
# POSIX source needed to be defined for strtok_r
CFLAGS = -std=c17 -D_POSIX_C_SOURCE=200809L -g -Wall -I$(INCDIR)

# Flags for linking CMocka
LDFLAGS = -lcmocka

# Use --wrap linker flag for mocking system calls
LDWRAP = -Wl,--wrap=socket

SRCDIR = src
INCDIR = include
TESTDIR = tests
BINDIR = bin

# Automatically find source files
SRCS := $(wildcard $(SRCDIR)/*.c)

# Automatically find test source files
TEST_SRCS := $(wildcard $(TESTDIR)/*.c)

# Header files
HEADERS := $(wildcard $(INCDIR)/*.h)

# Executable name
TARGET = $(BINDIR)/web_scraper

# Object files for sources (excluding main)
OBJS := $(filter-out $(SRCDIR)/web_scraper_main.o, $(SRCS:.c=.o))

# Test executable names (derived from test source filenames)
TEST_TARGETS := $(TEST_SRCS:$(TESTDIR)/%.c=$(BINDIR)/%)

# Default build target
all: $(TARGET)

# Build rule for main target
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

# Rule to compile and run tests
test: $(TEST_TARGETS)

$(TEST_TARGETS): $(BINDIR)/%: $(TESTDIR)/%.c $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDWRAP) -o $@ $< $(OBJS) $(LDFLAGS)

# Run tests
	@for test in $@; do \
		echo Running $$test; \
		./$$test; \
	done

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(BINDIR)/* $(SRCDIR)/*.o $(TESTDIR)/*.o

# Phony targets
.PHONY: clean all test
