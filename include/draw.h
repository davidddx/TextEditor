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

typedef struct {
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        int x;
        int y;
        color object_color;
} ObjectDrawingInfo;

typedef struct {
        ObjectDrawingInfo* drawing_info;
        TTF_TextEngine* text_engine;
        TTF_Font* font;
} TextDrawingInfo;

extern color BACKGROUND_COLOR;
extern color DEBUG_TEXT_COLOR;

/*
bool drawDebugInfo(SDL_Renderer* renderer, SDL_Texture* texture, 
                TTF_TextEngine* engine, TTF_Font* font,
                DebugInfo* debug_info, color text_color); */ 
bool drawDebugInfo(TextDrawingInfo* text_drawing_info, 
                DebugInfo* debug_info);

#endif
