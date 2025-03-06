include common.makefile

CFLAGS = -g
GDB_PATH = gdb

all: div-by-zero
	# Create two different core files so that loading multiple ones
	# can be tested.
	$(GDB_PATH) -x gdb-dump-corefile.cmd --args ./div-by-zero 1
	mv div-by-zero.core div-by-zero-1.core
	$(GDB_PATH) -x gdb-dump-corefile.cmd --args ./div-by-zero 2
	mv div-by-zero.core div-by-zero-2.core

div-by-zero : div-by-zero.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) div-by-zero div-by-zero-1.core div-by-zero-2.core
