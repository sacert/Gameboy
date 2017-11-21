#include <stdio.h>
#include <string.h>
#include "rom.h"
#include "MMU.h"

struct rom rom;

void romInit(char* filename) {
  FILE *f;
  unsigned char header[HEADER_SIZE];

  int len;
  int i;

  f = fopen(filename,"rb");
  fseek(f, 0, SEEK_END); // get the end of the file
  len = ftell(f);

  rom.romBytes = malloc(len);
  rewind(f);
  fread(rom.romBytes, len, 1, f);

  rewind(f);
  fread(header, 1, HEADER_SIZE, f);


  rom.romType = header[ROM_TYPE_OFFSET];
  for (i = 0; i<16; i++) {
    rom.gameTitle[i] = header[ROM_TITLE_OFFSET+i];
  }
  rom.romSize = header[ROM_SIZE_OFFSET];
  rom.romSize = pow(2,rom.romSize+1) * 16;

  rom.ramSize = header[ROM_RAM_OFFSET];
  rom.ramSize = pow(4, rom.ramSize)/2;

  memcpy(&cart[0x0000], &rom.romBytes[0x0000], 0x8000);

  printf("Game title: %s\n", rom.gameTitle);
  printf("Rom Size: %i\n", rom.romSize);
  printf("Ram Size: %i\n", rom.ramSize);
  printf("Rom Type: %i\n", rom.romType);

  fclose(f);
}
