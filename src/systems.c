#include "systems.h"
#include "constants.h"
#include "logger/log_setup.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
void closeLibraries() {
        SDL_free(CWD);
        TTF_DestroyRendererTextEngine(text_engine);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
}

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_TextEngine* text_engine = NULL;

bool initializeLibraries() {
        // initializing libraries
        SDL_Log("Initializing SDL...");
        if(!SDL_Init(SDL_INIT_VIDEO)) {
                SDL_Log("Could not initialize video: %s", SDL_GetError());
                return FAILED_INITIALIZING;
        }
        SDL_Log("SDL initialized.");
        if(!TTF_Init()) {
                SDL_Log("Could not initialize ttf library: %s", SDL_GetError());
                return FAILED_INITIALIZING;;
        }
        SDL_Log("SDL_TTF initialized.");
        if(!SDL_CreateWindowAndRenderer(APPLICATION_TITLE, 
                                ORIGINAL_WINDOW_WIDTH, ORIGINAL_WINDOW_HEIGHT, 
                                SDL_WINDOW_RESIZABLE, &window, &renderer)) {
                SDL_Log("Could not create window and renderer: %s", SDL_GetError());
                return FAILED_INITIALIZING;;
        }
        SDL_Log("Created window and renderer");
        text_engine = TTF_CreateRendererTextEngine(renderer);
        if(!text_engine) {
                SDL_Log("Could not create text engine: %s", SDL_GetError());
                return FAILED_INITIALIZING;;
        }
        SDL_Log("Created text engine");
        return SUCCEEDED_INITIALIZING; 
}
