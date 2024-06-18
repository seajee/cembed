CC=cc
CFLAGS=-Wall -Wextra

cembed: main.c
	$(CC) $(CFLAGS) -o cembed main.c utils.c

clean:
	rm -rf cembed
