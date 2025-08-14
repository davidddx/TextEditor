#include "application.h"
#include "systems.h"
#include "logger/log_setup.h"
#include "globals.h"
#include "text.h"
#include <stdbool.h>
bool initializeTextEditorApp(SDL_Window** w, SDL_Renderer** r, TTF_TextEngine** t) {
        applicationLoggingInit();

        if(!initializeTextEditorLibraries(w, r, t)) {
                SDL_Log("Could not initialize libraries. Exiting app...");
                return false;
        }
        SDL_Log("libraries initialized");
        if(!initializeTextEditorGlobals(w)) {
                SDL_Log("Could not initialize global variables. Exiting app...");
                return false;
        }
        SDL_Log("globals initialized");
        if(!initializeTextEditorFont()) {
                SDL_Log("Could not initialize font. Exiting app...");
                return false;
        }
        SDL_Log("font initialized");
        return true;
}

void closeTextEditorApp(SDL_Window* w, SDL_Renderer* r, TTF_TextEngine* t) {
        applicationLoggingClose();
        textEditorCloseLibraries(w, r, t);
}

int main(void) {
        SDL_Window* w;
        SDL_Renderer* r;
        TTF_TextEngine* t;
        if(!initializeTextEditorApp(&w, &r, &t)) {
                return 1; // SDL application failed to INITIALIZE 
        }
        SDL_Log("initialization done, going to application loop");
        textEditorApplicationLoop(w, r, t);
        closeTextEditorApp(w, r, t); 
        return 0;
}
