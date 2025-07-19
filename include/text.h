#ifndef TEXT_H
#define TEXT_H
#include <SDL3_ttf/SDL_ttf.h>
TTF_Font* generateTextFont(int size, char* font_name); //generate text font
extern char* FONT_NAME;
extern int FONT_SIZE;
#endif
