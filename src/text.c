#include "text.h"
#include <stdio.h>
#include "globals.h"
int FONT_SIZE = 12;
char* FONT_NAME = "CaskaydiaMonoNerdFontMono-Regular.ttf";
TTF_Font* generateTextFont(int size, char* font_name) {
        const int MAX_PATH_SIZE = 1000;
        const char* path = SDL_malloc(sizeof(char) * MAX_PATH_SIZE);
        sprintf(path, "%s%s%s%s%s", CWD, DIRECTORY_SEPARATOR, "fonts", DIRECTORY_SEPARATOR, font_name);
        SDL_Log("generating text font...");
        SDL_Log("FONT PATH: %s", path);
        SDL_Log("font_size: %d", size);
        SDL_Log("font_name: %s", font_name);
        TTF_Font* rv = TTF_OpenFont(path, size);
        SDL_free(path);
        if(rv == NULL) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load font %s: %s", font_name, SDL_GetError());
        }
        return rv;
}
