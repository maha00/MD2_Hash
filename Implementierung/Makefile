# to test the implementation run ./test in the terminal 
# compiler flags here
CFLAGS=-O3  -Wall -Wextra -Wpedantic -std=c11  -D_POSIX_C_SOURCE=199309L
#excecutable : all
all: test

test: main.c blocklauf-padding.c IO.c md2_hash.c md2_alternative.c

	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f test