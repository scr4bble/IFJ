ifneq ($(wildcard /usr/local/bin/gcc-4.9),)
	CC=gcc-4.9
else
	CC=clang
endif

RM=rm -f
CFLAGS=-g -ffast-math -Ofast -std=c11 -pedantic -Wall -Wextra -ftrapv -fstack-protector-all -funsigned-char
LDFLAGS=
#LDLIBS=-lpthread

BIN=ifj
SRCS=$(wildcard *.c)
OBJS=$(subst .c,.o,$(SRCS))

all: $(BIN)

$(BIN): $(OBJS) lib.a
	$(CC) $(LDFLAGS) -o $(BIN) $(BIN).o lib.a $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) -MM $^>>./.depend;

.PHONY: test

lib.a: $(filter-out $(BIN).o, $(OBJS))
	ar -rcs $@ $(filter-out $(BIN).o, $(OBJS))

playground: all
	$(MAKE) -C playground

test: all
	$(MAKE) -C unit_tests

clean:
	$(RM) *.o *.a $(BIN) core*
	$(MAKE) -C unit_tests clean
	$(MAKE) -C playground clean

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
