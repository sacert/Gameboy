#include <string.h>
#include "rom.h"
#include "MMU.h"
#include "interrupt.h"
#include "lcd.h"

unsigned char readByte(unsigned short address) {

    if (0x0000 <= address && address <= 0x7FFF)
        return cart[address];
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
    else if (address == 0xFF40)
        getLCDC();
    else if (address == 0xFF41)
        getLCDS();
    else if (address == 0xFF42)
        getScrollY();
    else if (address == 0xFF43)
        getScrollX();
    else if (address == 0xFF44)
        getLine();
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
        vram[address - 0xE000] = value;
    else if (0xFE00 <= address && address <= 0xFEFF)
        oam[address - 0xFE00] = value;
    else if (address == 0xFF0F)
        interrupt.flags = address;
    else if (address == 0xFFFF)
        interrupt.enable = address;
    else if (address == 0xFF40)
        setLCDC(address);
    else if (address == 0xFF41)
        setLCDS(address);
    else if (address == 0xFF42)
        setScrollY(address);
    else if (address == 0xFF43)
        setScrollX(address);
    else if (address == 0xFF45)
        setLyCompare(address);
    else if (address == 0xFF47)
        setBGPalette(address);
    else if (address == 0xFF48)
        setSpritePalette1(address);
    else if (address == 0xFF49)
        setSpritePalette2(address);
    else if (address == 0xFF4A)
        setWindowY(address);
    else if (address == 0xFF4B)
        setWindowX(address);
    else if(0xFF00 <= address && address <= 0xFF7F) // maybe only up to 0xFF4F
    		io[address - 0xff00] = value;
    else if (0xFF80 <= address && address <= 0xFFFE)
        hram[address - 0xFF80] = value;
}

void writeShort(unsigned short address, unsigned short value) {
    writeByte(address,(value & 0x00FF));
    writeByte(address+1,(value & 0xFF00) >> 8);
}

void changeMemoryBank(int bankNum) {
    memcpy(&cart[0x4000],&rom.romBytes[bankNum * 0x4000],0x4000);
}

void memInit(void) {
    writeByte(0xFF10, 0x80);
    writeByte(0xFF11, 0xBF);
    writeByte(0xFF12, 0xF3);
    writeByte(0xFF14, 0xBF);
    writeByte(0xFF16, 0x3F);
    writeByte(0xFF19, 0xBF);
    writeByte(0xFF1A, 0x7F);
    writeByte(0xFF1B, 0xFF);
    writeByte(0xFF1C, 0x9F);
    writeByte(0xFF1E, 0xBF);
    writeByte(0xFF20, 0xFF);
    writeByte(0xFF23, 0xBF);
    writeByte(0xFF24, 0x77);
    writeByte(0xFF25, 0xF3);
    writeByte(0xFF26, 0xF1);
    writeByte(0xFF40, 0x91);
    writeByte(0xFF47, 0xFC);
    writeByte(0xFF48, 0xFF);
    writeByte(0xFF49, 0xFF);
}
