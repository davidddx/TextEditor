#include "systems.h"
#include "logger/log_setup.h"
#include "globals.h"
void closeLibraries(SDL_Window* window, SDL_Renderer* renderer, TTF_TextEngine* text_engine) {
        SDL_free(CWD);
        TTF_DestroyRendererTextEngine(text_engine);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_Quit();
}

bool initializeLibraries(SDL_Window* window, SDL_Renderer* renderer, TTF_TextEngine* text_engine) {
        SDL_Log("Initializing SDL Video...");
        if(!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
                SDL_Log("Could not initialize SDL Video: %s", SDL_GetError());
                return FAILED_INITIALIZING;
        }
        SDL_Log("SDL Video initialized.");
        if(!TTF_Init()) {
                SDL_Log("Could not initialize ttf library: %s", SDL_GetError());
                return FAILED_INITIALIZING;;
        }
        SDL_Log("SDL_TTF initialized.");
        
        SDL_Log("creating window and renderer..."); 
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
