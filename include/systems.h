#ifndef SYSTEMS_H
#define SYSTEMS_H 
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>

bool initializeTextEditorLibraries(SDL_Window** w, SDL_Renderer** renderer, TTF_TextEngine** text_engine); // initializes window, renderer, sdl library, etc. 
void textEditorCloseLibraries(SDL_Window* w, SDL_Renderer* renderer, TTF_TextEngine* text_engine);

#endif
