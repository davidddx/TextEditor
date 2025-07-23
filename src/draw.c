#include "draw.h"
#include "globals.h"
#include <stdio.h> // included for sprintf use.
#include <SDL3_ttf/SDL_ttf.h>
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


void drawFpsText(SDL_Renderer* renderer, SDL_Texture* texture, TTF_TextEngine* engine, TTF_Font* font, 
                float fps, color text_color) {
        
        int maxFpsStrlen = 20; // rough overestimate
        char* strfps = SDL_malloc(sizeof(char) * maxFpsStrlen);
        sprintf(strfps, "FPS: %f", fps);
        //SDL_Log("color: {%d %d %d %d}", text_color.r, text_color.g, text_color.b, text_color.a);
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
        TTF_Text* font_text = TTF_CreateText(engine, font, strfps, strlen(strfps));
        TTF_SetTextColor(font_text, text_color.r, text_color.g, text_color.b, text_color.a);
        /*
        float texture_width;
        float texture_height;
        SDL_GetTextureSize(texture, texture_width, texture_height);
        */
        TTF_DrawRendererText(font_text, 0, 0);
        SDL_SetRenderTarget(renderer, NULL);
        TTF_DestroyText(font_text);
        SDL_free(strfps);
}
