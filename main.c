#include "rom.h"
#include "cpu.h"
#include "sdl.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"

// including MMU.h causes error in register.pc...? wtf

// one day I'll create that make file 
// gcc -o gb cpu.c interrupt.c mmu.c rom.c main.c sdl.c -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2

int main(int argc, char* argv[]) {

    romInit(argv[1]); // loads rom
    cpuInit(); // initialize registers and memory
    sdlInit();

    while (1) {
        cpuCycle();
        lcdCycle();
        interruptCycle();
        timerCycle();
    }

}
