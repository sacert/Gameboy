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
        printf("FPS: %ld\n", display.frames/(t2.tv_sec - t1.tv_sec));
    }

    /* may need the following */
    // display.surface = get it up
    // display.texture = SDL_CreateTextureFromSurface(display.renderer, display.surface);
    // SDL_FreeSurface(display.surface);
    // SDL_RenderClear(display.renderer);
    // SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
    SDL_RenderPresent(display.renderer); // push 'behind the scenes' into display
    display.frames++;
}

int sdlUpdate(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
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
    return display.surface->pixels;
}

void sdlQuit(void) {
    SDL_Quit();
}