CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O0 -g

playlist: playlist.c
	$(CC) $(CFLAGS) -o playlist playlist.c

.PHONY: clean run
clean:
	rm -f playlist playlist.exe

run: playlist
	./playlist --empty
