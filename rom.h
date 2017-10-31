#include <stdio.h> // printing
#include <stdlib.h>
#include <math.h>

#define ROM_TITLE_OFFSET 0x134
#define ROM_TYPE_OFFSET 0x147
#define ROM_SIZE_OFFSET 0x148
#define ROM_RAM_OFFSET 0x149
#define HEADER_SIZE 0x14F

struct rom {
    unsigned char *romBytes;
    char gameTitle[17];
    int romType; // use int for now - change to string
    int romSize;
    int ramSize;
} extern rom;

void romInit(char* filename);
