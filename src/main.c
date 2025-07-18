#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "application.h"
#include "systems.h"
#include "logger/log_setup.h"
#include "globals.h"
#include <stdbool.h>
bool initialize() {
        loggingInit();
        if(!initializeGlobalsAndConstants()) {
                SDL_Log("Could not initialize globals and constants. Exiting app");
                return false;
        }
        SDL_Log("Globals and constants Initialized.");
        if(!initializeLibraries()) {
                SDL_Log("Could not initialize libraries. Exiting app...");
                return false;
        }
        SDL_Log("libraries initialized");
        return true;
}

void close() {
        loggingClose();
        closeLibraries();
}

int main(void) {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
        if(!initialize()) {
                return 1; // SDL application failed to INITIALIZE 
        }
        SDL_Log("initialization done, going to application loop");
        applicationLoop();
        close(); 
        return 0;
}
