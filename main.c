#include <stdio.h> // printing
#include <stdlib.h>
#include <math.h>
#include "MMU.h"

#define ROM_TITLE_OFFSET 0x134
#define ROM_TYPE_OFFSET 0x147
#define ROM_SIZE_OFFSET 0x148
#define ROM_RAM_OFFSET 0x149
#define HEADER_SIZE 0x14F

int main(int argc, char* argv[]) {
    FILE *f;
    unsigned char header[HEADER_SIZE];

    char gameTitle[17];
    int romType; // use int for now - change to string
    int romSize;
    int ramSize;

    int len;
    int i;

    f = fopen(argv[1],"rb");
    fseek(f, 0, SEEK_END); // get the end of the file
    len = ftell(f);

    rom = malloc(len);
    rewind(f);
    fread(rom, len, 1, f);

    rewind(f);
    fread(header, 1, HEADER_SIZE, f);


    romType = header[ROM_TYPE_OFFSET];
    for (i = 0; i<16; i++) {
      gameTitle[i] = header[ROM_TITLE_OFFSET+i];
    }
    romSize = header[ROM_SIZE_OFFSET];
    romSize = pow(2,romSize+1) * 16;

    ramSize = header[ROM_RAM_OFFSET];
    ramSize = pow(4, ramSize)/2;

    printf("Game Title: %s\n", gameTitle);
    printf("Rom Type: %i\n", romType);
    printf("Rom Size: %i\n", romSize);
    printf("Ram Size: %i\n", ramSize);

    unsigned short hmm = 0xC4C5;
    printf("%u\n", (unsigned char)(hmm & 0xFF00));
    printf("%x\n", hmm & 0x00FF);

    fclose(f);

}
