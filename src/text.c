#include "text.h"
#include <stdio.h>
#include "globals.h"
float FONT_SIZE = 12;
float DEBUG_FONT_SIZE = 0; // set in initializeTextEditorFont.
char* DEBUG_FONT_NAME = "CaskaydiaMonoNerdFontMono-Regular.ttf";
float PIXELS_IN_ONE_PT = 1.3333333333f;

// need to free returned string after use.
char* getTextFontPath(char* cwd, char* directory_separator, char* dir_name, char* font_name) {
        const int MAX_PATH_SIZE = 1000;
        const char* path = SDL_malloc(sizeof(char) * MAX_PATH_SIZE);
        sprintf(path, "%s%s%s%s%s", cwd, directory_separator, dir_name, directory_separator, font_name);
        return path;
}


bool initializeTextEditorFont() {
        // initializing font size using fact 
        // 1 pt = 1.333333333 pixels
        float debug_text_pixels = (float)DESKTOP_HEIGHT/12.0f; // proportion for debug text im sticking to
        DEBUG_FONT_SIZE = debug_text_pixels / PIXELS_IN_ONE_PT; // relatinoship b/t pt && pixels.
        return true;
}



TTF_Font* generateTextFont(int size, char* font_name) {
        const int MAX_PATH_SIZE = 1000;
        const char* path = getTextFontPath(CWD, DIRECTORY_SEPARATOR, "fonts", font_name);
        //const char* path = SDL_malloc(sizeof(char) * MAX_PATH_SIZE);
        //sprintf(path, "%s%s%s%s%s", CWD, DIRECTORY_SEPARATOR, "fonts", DIRECTORY_SEPARATOR, font_name);
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
