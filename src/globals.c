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
