#include "draw.h"
#include "globals.h"
#include "debug.h"
#include <stdio.h> // included for sprintf use.
#include <SDL3_ttf/SDL_ttf.h>
color BACKGROUND_COLOR = {255, 255, 255, 255};
color FPS_TEXT_COLOR = {0, 0, 0, 255};
void drawStringAtToprightPos(SDL_Renderer* r, TTF_TextEngine* e, 
                TTF_Font* f, const char* str, color c, SDL_FPoint topright) {
        TTF_Text* t = TTF_CreateText(e, f, str, strlen(str));
        TTF_SetTextColor(t, c.r, c.g, c.b, c.a);
        int tW;
        int tH;
        TTF_GetTextSize(t, &tW, &tH);
        TTF_DrawRendererText(t, topright.x - tW, topright.y);
        TTF_DestroyText(t);
}

void drawStringAtMiddlePos(SDL_Renderer* r, TTF_TextEngine* e, 
                TTF_Font* f, const char* str, color c, SDL_FPoint topright) {
        TTF_Text* t = TTF_CreateText(e, f, str, strlen(str));
        TTF_SetTextColor(t, c.r, c.g, c.b, c.a);
        int tW;
        int tH;
        TTF_GetTextSize(t, &tW, &tH);
        TTF_DrawRendererText(t, topright.x - tW/2, topright.y - tH/2);
        TTF_DestroyText(t);
}


bool drawFpsText(SDL_Renderer* renderer, SDL_Texture* texture, 
                TTF_TextEngine* engine, TTF_Font* font, 
                float fps, color text_color) {
        
        int maxFpsStrlen = 20; // rough overestimate
        char* strfps = SDL_malloc(sizeof(char) * maxFpsStrlen);
        sprintf(strfps, "FPS: %f", fps);

        TTF_Text* font_text = TTF_CreateText(engine, font, strfps, strlen(strfps));
        TTF_SetTextColor(font_text, text_color.r, text_color.g, text_color.b, text_color.a);
        TTF_DrawRendererText(font_text, 0, 0);

        TTF_DestroyText(font_text);

        SDL_free(strfps);
        return true;
}

bool drawResolutionText(SDL_Renderer* renderer, SDL_Texture* texture,
                TTF_TextEngine* engine, TTF_Font* font,
                int desktop_width, int desktop_height, color text_color) {

        

        int max_resolution_str_len = 100; // rough overestimate
        char* str_resolution = SDL_malloc(sizeof(char) * max_resolution_str_len);
        sprintf(str_resolution, "Desktop Resolution: %dx%d", desktop_width, desktop_height);
        TTF_Text* font_text = TTF_CreateText(engine, font, str_resolution, strlen(str_resolution));
        TTF_SetTextColor(font_text, text_color.r, text_color.g, text_color.b, text_color.a);
        TTF_DrawRendererText(font_text, 0, 0);
        TTF_DestroyText(font_text);
        SDL_free(str_resolution);
        return true;
}


bool drawWindowSizeText(SDL_Renderer* renderer, SDL_Texture* texture, 
                TTF_TextEngine* engine, TTF_Font* font,
                int window_width, int window_height, color text_color) 
{
        int max_window_size_str_len = 100; // rough overestimate
        char* str_window_size = SDL_malloc(sizeof(char) * max_window_size_str_len);
        sprintf(str_window_size, "window size: (%d, %d)", window_width, window_height);
        TTF_Text* font_text = TTF_CreateText(engine, font, str_window_size, strlen(str_window_size));
        TTF_SetTextColor(font_text, text_color.r, text_color.g, text_color.b, text_color.a);
        TTF_DrawRendererText(font_text, 0, 0);
        TTF_DestroyText(font_text);
        SDL_free(str_window_size);
        return true;

}


bool drawDebugInfo(SDL_Renderer* renderer, SDL_Texture* texture, 
                TTF_TextEngine* engine, TTF_Font* font,
                Debug_Info* debug_info, color text_color) 
{
        if(!SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND)) {
                return false;
        }
        if(!SDL_SetTextureAlphaMod(texture, 100)) {
                return false;
        }
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderClear(renderer);
        if(!drawFpsText(renderer, texture, engine, font, debug_info->fps, text_color)) 
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not draw fps text: %s", 
                                SDL_GetError());
        }
        if(!drawResolutionText(renderer, texture, engine, font, debug_info->desktop_width, 
                                debug_info->desktop_height, text_color)) 
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not draw desktop resolution: %s",
                                SDL_GetError());
        }
        if(!drawWindowSizeText(renderer, texture, engine, font, debug_info->window_width,
                                debug_info->window_height, text_color)) 
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not draw window size: %s",
                                SDL_GetError());

        }

        SDL_SetRenderTarget(renderer, NULL);

}
