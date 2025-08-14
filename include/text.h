#ifndef TEXT_H
#define TEXT_H
#include <SDL3_ttf/SDL_ttf.h>



TTF_Font* generateTextTTFFont(int size, char* font_name); //generate text font
bool initializeTextEditorFont(void);
extern char* DEBUG_FONT_NAME;
extern char* TEXT_EDITOR_FONT_NAME;
extern float DEBUG_FONT_SIZE;
extern float TEXT_EDITOR_FONT_SIZE;
extern float FONT_SIZE;
extern float PIXELS_IN_ONE_PT;
extern unsigned int MAX_TEXT_LENGTH_PER_FRAME;
#endif
