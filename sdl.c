#include "sdl.h"

struct timeval t1, t2;
struct buttons buttons;
struct display display;

void sdlInit(void) {
    SDL_Init(SDL_INIT_VIDEO);
    display.screen = SDL_CreateWindow("Game Boy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN); 
}

void sdlQuit(void) {
    SDL_Quit();
}