#ifndef GLOBALS_H
#define GLOBALS_H
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
extern char* DIRECTORY_SEPARATOR;
bool initializeTextEditorGlobals(SDL_Window** window_pointer);
extern bool LOGGING;
extern uint16_t ORIGINAL_WINDOW_WIDTH;
extern uint16_t ORIGINAL_WINDOW_HEIGHT;
extern bool FAILED_INITIALIZING;
extern bool SUCCEEDED_INITIALIZING;
extern char* APPLICATION_TITLE;
extern int CWD_MAX_SIZE;
extern char* CWD;
extern int MAX_FPS;
extern int DESKTOP_WIDTH;
extern int DESKTOP_HEIGHT;
#endif
