#include "globals.h"
#include <unistd.h>
#include <errno.h>

#ifdef WIN32
char* DIRECTORY_SEPARATOR = "\\";
#else
char* DIRECTORY_SEPARATOR = "/";
#endif
bool LOGGING = true;
uint16_t ORIGINAL_WINDOW_WIDTH = 1280;
uint16_t ORIGINAL_WINDOW_HEIGHT = 720;
bool FAILED_INITIALIZING = false;
bool SUCCEEDED_INITIALIZING = true;
char* APPLICATION_TITLE = "NOTES";
char* CWD = NULL;
int CWD_MAX_SIZE = 1000;
int MAX_FPS = 60;
int DESKTOP_WIDTH = 0;
int DESKTOP_HEIGHT = 0;

bool initializeDesktopSizes(SDL_Window** window_pointer) {
        SDL_DisplayID current_display_id = SDL_GetPrimaryDisplay();
        SDL_DisplayMode* current_desktop_display_mode = SDL_GetCurrentDisplayMode(current_display_id);
        if(current_desktop_display_mode == NULL) {
                SDL_Log("Error getting current desktop display mode: %s", SDL_GetError()); 
                return false;
        }
        DESKTOP_WIDTH = current_desktop_display_mode->w;
        DESKTOP_HEIGHT = current_desktop_display_mode->h;
        SDL_Log("Desktop size: %d, %d", DESKTOP_WIDTH, DESKTOP_HEIGHT);
        return true;
}

bool initializeCWD() {
        CWD = getcwd(CWD, CWD_MAX_SIZE);
        if(!CWD) {
                SDL_Log("Could not get CWD: %s", strerror(errno));
                SDL_free(CWD);
                return FAILED_INITIALIZING;
        }
        SDL_Log("Fetched cwd: %s", CWD);
        return SUCCEEDED_INITIALIZING;
}

bool initializeTextEditorGlobals(SDL_Window** window_pointer) {
        if(!initializeDesktopSizes(window_pointer)) {
                SDL_Log("Could not initialize globals: failed to get Desktop sizes");
                return false;
        }
        if (!initializeCWD()) {
                SDL_Log("Could not initialize globals: failed to get current working directory");
                return false;
        }
        return true;
}

