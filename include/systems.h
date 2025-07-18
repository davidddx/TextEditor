#ifndef SYSTEMS_H
#define SYSTEMS_H 
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

extern bool initializeLibraries(void); // initializes window, renderer, sdl library, etc. 
extern void closeLibraries(void);
/*
extern SDL_Window* window; 
extern SDL_Renderer* renderer;
extern TTF_TextEngine* text_engine;
*/
#endif
