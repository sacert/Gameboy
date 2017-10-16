unsigned char *rom;   // ROM (Cart 1 & 2)
unsigned char vram[0x2000];  // video RAM
unsigned char sram[0x2000];  // switchable RAM
unsigned char wram[0x2000];  // working RAM
unsigned char oam[0xA0];     // Sprite Attribute Memory
unsigned char io[0x100];     // Input/Output - Not sure if I need 0x100, 0x40 may suffice
unsigned char hram[0x7F];    // High RAM

unsigned char readByte(unsigned short address);
unsigned short readShort(unsigned short address);
void writeByte(unsigned short address, unsigned char value);
void writeShort(unsigned short address, unsigned short value);
