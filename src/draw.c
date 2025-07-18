#include "draw.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h> // included for sprintf use.
color BACKGROUND_COLOR = {255, 255, 255, 255};
color FPS_TEXT_COLOR = {0, 0, 0, 255};
void drawStringAtToprightPos(SDL_Renderer* r, TTF_TextEngine* e, TTF_Font* f, const char* str, color c, SDL_FPoint topright) {
        TTF_Text* t = TTF_CreateText(e, f, str, strlen(str));
        TTF_SetTextColor(t, c.r, c.g, c.b, c.a);
        int tW;
        int tH;
        TTF_GetTextSize(t, &tW, &tH);
        TTF_DrawRendererText(t, topright.x - tW, topright.y);
        TTF_DestroyText(t);
}

void drawStringAtMiddlePos(SDL_Renderer* r, TTF_TextEngine* e, TTF_Font* f, const char* str, color c, SDL_FPoint topright) {
        TTF_Text* t = TTF_CreateText(e, f, str, strlen(str));
        TTF_SetTextColor(t, c.r, c.g, c.b, c.a);
        int tW;
        int tH;
        TTF_GetTextSize(t, &tW, &tH);
        TTF_DrawRendererText(t, topright.x - tW/2, topright.y - tH/2);
        TTF_DestroyText(t);
}

void drawFpsText(SDL_Renderer* r, TTF_TextEngine* e, TTF_Font* f, 
                float fps, color c) {
        char* strfps = SDL_malloc(sizeof(char) * 10);
        sprintf(strfps, "FPS: %f", fps);
        TTF_Text* t = TTF_CreateText(e, f, strfps, strlen(strfps));
        TTF_SetTextColor(t, c.r, c.g, c.b, c.a);
        TTF_DrawRendererText(t, 10, 10);
        TTF_DestroyText(t);
        SDL_free(strfps);
}
