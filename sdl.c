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

int sdlUpdate(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                swtich(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        button.left = 1;
                        break;
                    case SDLK_RIGHT:
                        button.right = 1;
                        break;
                    case SDLK_UP:
                        button.up = 1;
                        break;
                    case SDLK_DOWN:
                        button.down = 1;
                        break;
                    case SDLK_z:
                        button.a = 1;
                        break;
                    case SDLK_x:
                        button.b = 1;
                        break;
                    case SDLK_a:
                        button.start = 1;
                        break;
                    case SDLK_s:
                        button.select = 1;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                swtich(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        button.left = 0;
                        break;
                    case SDLK_RIGHT:
                        button.right = 0;
                        break;
                    case SDLK_UP:
                        button.up = 0;
                        break;
                    case SDLK_DOWN:
                        button.down = 0;
                        break;
                    case SDLK_z:
                        button.a = 0;
                        break;
                    case SDLK_x:
                        button.b = 0;
                        break;
                    case SDLK_a:
                        button.start = 0;
                        break;
                    case SDLK_s:
                        button.select = 0;
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

void sdlQuit(void) {
    SDL_Quit();
}