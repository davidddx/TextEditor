#ifndef DRAW_H
#define DRAW_H
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
typedef struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
} color;
extern color BACKGROUND_COLOR;
extern color FPS_TEXT_COLOR;

void drawFpsText(SDL_Renderer* renderer, SDL_Texture* texture, TTF_TextEngine* e, TTF_Font* f, float fps, color c);
#endif
