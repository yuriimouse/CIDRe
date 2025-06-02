VERSION = 1.2.2

# Library name
LIB_NAME = cidre

# Installation prefix
INSTALL_PATH ?= /usr/local/

CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O2 -g -std=gnu99 -DVERSION=\"$(VERSION)\" -I./include -I/usr/include
LDFLAGS = -shared

STATIC_LIB = lib$(LIB_NAME).a
TARGET_LIB = lib$(LIB_NAME).so

.PHONY: default all clean install copy

# Default target: build only
default: all

# Build both shared and static libraries
all: $(TARGET_LIB)

# Collect object files from all .c files
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build shared and static libraries
$(TARGET_LIB): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
	ar rcs $(STATIC_LIB) $(OBJECTS)

# Remove generated files
clean:
	rm -f *.o $(TARGET_LIB) $(STATIC_LIB)

# Full install process: build, copy, set permissions
install: all copy

# Install headers and libraries using 'install' command
copy:
	install -d $(INSTALL_PATH)include/$(LIB_NAME)
	install -m 644 *.h $(INSTALL_PATH)include/$(LIB_NAME)
	install -d $(INSTALL_PATH)lib
	install -m 755 $(TARGET_LIB) $(INSTALL_PATH)lib
	install -m 644 $(STATIC_LIB) $(INSTALL_PATH)lib
