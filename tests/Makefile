# tests/Makefile
# Makefile for CIDRe unit tests using CUnit
# Version: 1.3.1

VERSION = 1.3.1

# Installation prefix for include and library paths
INSTALL_PATH ?= /usr/local/

# Compiler and flags
CC = gcc
CFLAGS  = -fPIC -Wall -Wextra -O2 -g -std=gnu99 \
          -DVERSION=\"$(VERSION)\" \
          -I$(INSTALL_PATH)/include -I/usr/include
LDFLAGS = -fpie -L$(INSTALL_PATH)/lib

# Static and dynamic libraries
S_LIBS = -Wl,-Bstatic -lcunit -lcidre
D_LIBS = -Wl,-Bdynamic -lpthread -lresolv

# Output binary name
TARGET_BIN = unitTest

# List of source files and corresponding object files
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

.PHONY: default clean run

# Default target: build the test binary
default: $(TARGET_BIN)

# Compile each .c file to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link all object files into the final test binary
$(TARGET_BIN): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(S_LIBS) $(D_LIBS)

# Clean up compiled files
clean:
	rm -f *.o $(TARGET_BIN)

# Run the test binary with Valgrind memory checking
run: $(TARGET_BIN)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET_BIN)
