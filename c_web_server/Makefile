# Run: make
# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -std=c17 -g -Wall -I$(INCDIR)

SRCDIR = src
INCDIR = include

# Source files for this program
SRCS = $(SRCDIR)/web_scraper_main.c $(SRCDIR)/web_scraper_server.c

# Header files
HEADERS = $(INCDIR)/web_scraper_server.h

# Executable name
TARGET = web_scraper

# Build rule
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule
clean:
	rm -f $(TARGET)

# Phony target to prevent conflicts with files named 'clean' or 'all'
.PHONY: clean