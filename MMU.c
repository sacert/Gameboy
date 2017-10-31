#include "MMU.h"
#include "interrupt.h"

// 0xFFFF = Interrupt Enable Register

unsigned char readByte(unsigned short address) {

    if (0x0000 <= address && address <= 0x7FFF)
        return rom[address];
    else if (0x8000 <= address && address <= 0x9FFF)
        return vram[address - 0x8000];
    else if (0xA000 <= address && address <= 0xBFFF)
        return sram[address - 0xA000];
    else if (0xC000 <= address && address <= 0xDFFF)
        return wram[address - 0xC000];
    else if (0xE000 <= address && address <= 0xFDFF) // echo of wram
        return wram[address - 0xE000];
    else if (0xFE00 <= address && address <= 0xFEFF)
        return oam[address - 0xFE00];
    else if (address == 0xFF0F)
        return interrupt.flags;
    else if (address == 0xFFFF)
        return interrupt.enable;
    else if(0xFF00 <= address && address <= 0xFF7F) // maybe only up to 0xFF4F
    		return io[address - 0xff00];
    else if (0xFF80 <= address && address <= 0xFFFE)
        return hram[address - 0xFF80];

    return 0;
}

unsigned short readShort(unsigned short address) {
    return (readByte(address) | (readByte(address+1) << 8));
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
    else if (address == 0xFF0F)
        interrupt.flags = address;
    else if (address == 0xFFFF)
        interrupt.enable = address;
    else if(0xFF00 <= address && address <= 0xFF7F) // maybe only up to 0xFF4F
    		io[address - 0xff00] = value;
    else if (0xFF80 <= address && address <= 0xFFFE)
        hram[address - 0xFF80] = value;
}

void writeShort(unsigned short address, unsigned short value) {
    writeByte(address,(value & 0x00FF));
    writeByte(address+1,(value & 0xFF00) >> 8);
}
