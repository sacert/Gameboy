#include "rom.h"
#include "cpu.h"
#include "sdl.h"
#include "lcd.h"
#include "interrupt.h"
#include "timer.h"

int main(int argc, char* argv[]) {

    if(argc != 2) {
		fprintf(stderr, "%s [rom]\n", argv[0]);
		return 0;
	}

    romInit(argv[1]); 
    cpuInit(); 
    sdlInit();
    
    while (1) {

        unsigned int timeStart = SDL_GetTicks();
        
        cpuCycle();
        interruptCycle();
        timerCycle();

        if (!lcdCycle(timeStart))
            break;
    }

    SDL_Quit();

}
