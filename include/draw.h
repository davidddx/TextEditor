#ifndef DRAW_H
#define DRAW_H
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "debug.h"
typedef struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
} color;
extern color BACKGROUND_COLOR;
extern color FPS_TEXT_COLOR;

bool drawDebugInfo(SDL_Renderer* renderer, SDL_Texture* texture, 
                TTF_TextEngine* engine, TTF_Font* font,
                Debug_Info* debug_info, color text_color); 

#endif
