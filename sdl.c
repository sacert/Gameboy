#include "sdl.h"

struct timeval t1, t2;
struct buttons buttons;
struct display display;
unsigned int frames;

SDL_Surface *screen;

void sdlInit(void) {
    SDL_Init(SDL_INIT_VIDEO);
    display.screen = SDL_CreateWindow("Game Boy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, SDL_WINDOW_OPENGL); 
    screen = SDL_GetWindowSurface(display.screen);
    frames = 0;
}

void sdlSetFrame(void) {

    if (frames == 0) {
        gettimeofday(&t1, NULL);
    }
    frames++;
    if (frames % 1000 == 0) { 
        gettimeofday(&t2, NULL);
        printf("FPS: %i\n", frames/((int)t2.tv_sec - (int)t1.tv_sec));
    }
    SDL_UpdateWindowSurface(display.screen);
}

int sdlUpdate(void) {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {

        if(event.type == SDL_QUIT)
			return 1;
        
        switch (event.type) {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        buttons.left = 1;
                        break;
                    case SDLK_RIGHT:
                        buttons.right = 1;
                        break;
                    case SDLK_UP:
                        buttons.up = 1;
                        break;
                    case SDLK_DOWN:
                        buttons.down = 1;
                        break;
                    case SDLK_z:
                        buttons.a = 1;
                        break;
                    case SDLK_x:
                        buttons.b = 1;
                        break;
                    case SDLK_a:
                        buttons.start = 1;
                        break;
                    case SDLK_s:
                        buttons.select = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        buttons.left = 0;
                        break;
                    case SDLK_RIGHT:
                        buttons.right = 0;
                        break;
                    case SDLK_UP:
                        buttons.up = 0;
                        break;
                    case SDLK_DOWN:
                        buttons.down = 0;
                        break;
                    case SDLK_z:
                        buttons.a = 0;
                        break;
                    case SDLK_x:
                        buttons.b = 0;
                        break;
                    case SDLK_a:
                        buttons.start = 0;
                        break;
                    case SDLK_s:
                        buttons.select = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    return 0;
}

unsigned int *sdlFrameBuffer(void) {
    return screen->pixels;
}

unsigned int getButton(void) {
    return ((buttons.start * 8) | (buttons.select * 4) | (buttons.b * 2) | buttons.a);
}

unsigned int getDirection(void) {
    return ((buttons.down * 8) | (buttons.up * 4) | (buttons.left * 2) | buttons.right);
}