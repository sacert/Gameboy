#include "sdl.h"

struct timeval t1, t2;
struct buttons buttons;
struct display display;

void sdlInit(void) {
    SDL_Init(SDL_INIT_VIDEO);
    display.screen = SDL_CreateWindow("Game Boy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN); 
}

void sdlSetFrame(void) {

    if (display.frames == 0) {
        gettimeofday(&t1, NULL);
    }

    if (display.frames % 100 == 0) { // try it out with 100 frames, might give a good enough estiamte of fps
        gettimeofday(&t2, NULL);
        printf("FPS: %d\n", display.frames/(t2.tv_sec - t1.tv_sec));
    }

    SDL_RenderPresent(renderer); // push 'behind the scenes' into display
    frames++;
}

void sdlQuit(void) {
    SDL_Quit();
}