#ifndef TEXT_H
#define TEXT_H
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font* generateTextFont(int size, char* font_name); //generate text font
bool initializeTextEditorFont(void);
extern char* DEBUG_FONT_NAME;
extern float DEBUG_FONT_SIZE;
extern float FONT_SIZE;
extern float PIXELS_IN_ONE_PT;
#endif
