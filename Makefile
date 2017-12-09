CC=gcc
LDFLAGS=-F/Library/Frameworks -framework SDL2 -g

all: clean gameboy

gameboy:
	$(CC) *.c -o gameboy $(LDFLAGS)

clean:
	rm -f gameboy gameboy.exe
