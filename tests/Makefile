# tests/Makefile

VERSION = 1.2.2

# Installation prefix (used for includes and linking)
INSTALL_PATH ?= /usr/local/

CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O2 -g -std=gnu99 -DVERSION=\"$(VERSION)\" -I$(INSTALL_PATH)/include -I/usr/include
LDFLAGS = -fpie -L$(INSTALL_PATH)/lib

# Static and dynamic libraries
S_LIBS = -Wl,-Bstatic -lcunit -lcidre
D_LIBS = -Wl,-Bdynamic -lpthread -lresolv

# Output binary name
TARGET_BIN = unitTest

.PHONY: default clean run

# Default target: build only
default: $(TARGET_BIN)

# Collect object files
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

# Compile .c into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the test binary
$(TARGET_BIN): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(S_LIBS) $(D_LIBS)

# Remove intermediate files
clean:
	rm -f *.o $(TARGET_BIN)

run: $(TARGET_BIN)
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(TARGET_BIN)
