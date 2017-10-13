unsigned char rom[0x8000];   // ROM (Cart 1 & 2)
unsigned char vram[0x2000];  // video RAM
unsigned char sram[0x2000];  // switchable RAM
unsigned char wram[0x2000];  // working RAM
unsigned char oam[0xA0];     // Sprite Attribute Memory
unsigned char io[0x100];     // Input/Output - Not sure if I need 0x100, 0x40 may suffice
unsigned char hram[0x7F];    // High RAM

// 0xFFFF = Interrupt Enable Register

void readByte(unsigned short address) {

    if (0x0000 <= address && address <= 0x7FFF)
        return rom[address];
    else if (0x8000 <= address && address <= 0x9FFF)
        return vram[address - 0x8000];
    else if (0xA000 <= address && address <= 0xBFFF)
        return sram[address - 0xA000];
    else if (0xC000 <= address && address <= 0xDFFF)
        return wram[address - 0xC000];
    else if (0xE000 <= address && address <= 0xFDFF)
        return oam[address - 0xE000];
    else if(0xFF00 <= address && address <= 0xFF7F) // maybe only up to 0xFF4F
    		return io[address - 0xff00];
    else if (0xFF80 <= address && address <= 0xFFFE)
        return hram[address - 0xFF80];
}

void writeByte(unsigned short address, unsigned char value) {

    // cant write to ROM
    if (0x8000 <= address && address <= 0x9FFF)
        vram[address - 0x8000] = value;
    else if (0xA000 <= address && address <= 0xBFFF)
        sram[address - 0xA000] = value;
    else if (0xC000 <= address && address <= 0xDFFF)
        wram[address - 0xC000] = value;
    else if (0xE000 <= address && address <= 0xFDFF)
        oam[address - 0xE000] = value;
    else if(0xFF00 <= address && address <= 0xFF7F) // maybe only up to 0xFF4F
    		io[address - 0xff00] = value;
    else if (0xFF80 <= address && address <= 0xFFFE)
        hram[address - 0xFF80] = value;
}
