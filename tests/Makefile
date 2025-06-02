VERSION = 1.2.2

INSTALL_PATH ?= /usr/local/

CC = gcc
CFLAGS = -fPIC -Wall -Wextra -O2 -g -std=gnu99 -DVERSION=\"$(VERSION)\" -I$(INSTALL_PATH)include -I/usr/include 
LDFLAGS = -fpie -L/usr/local/lib/

S_LIBS = -Wl,-Bstatic -lcunit -L$(INSTALL_PATH)lib -lcidre
D_LIBS = -Wl,-Bdynamic -lpthread -lresolv
TARGET_BIN = unitTest

.PHONY: default clean
default: $(TARGET_BIN) clean

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_BIN): $(OBJECTS)
	$(CC) ${LDFLAGS} -o $@ $(OBJECTS) $(S_LIBS) $(D_LIBS)

clean:
	rm -f *.o
