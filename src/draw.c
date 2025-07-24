#include "draw.h"
#include "globals.h"
#include "debug.h"
#include "text.h"
#include <stdio.h> // included for sprintf use.
#include <SDL3_ttf/SDL_ttf.h>
color BACKGROUND_COLOR = {255, 255, 255, 255};
color DEBUG_TEXT_COLOR = {0, 0, 0, 255};
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
                TTF_Font* f, const char* str, color c, SDL_FPoint topright) 
{
        TTF_Text* t = TTF_CreateText(e, f, str, strlen(str));
        TTF_SetTextColor(t, c.r, c.g, c.b, c.a);
        int tW;
        int tH;
        TTF_GetTextSize(t, &tW, &tH);
        TTF_DrawRendererText(t, topright.x - tW/2, topright.y - tH/2);
        TTF_DestroyText(t);
}


bool drawFpsText(TextDrawingInfo* draw_info, 
                float fps) 
{
        
        int max_fps_str_len = 20; // rough overestimate
        char* strfps = SDL_malloc(sizeof(char) * max_fps_str_len);
        sprintf(strfps, "FPS: %f", fps);
        TTF_Text* font_text = TTF_CreateText(
                        draw_info->text_engine, draw_info->font, 
                        strfps, strlen(strfps));
        TTF_SetTextColor(font_text, draw_info->drawing_info->object_color.r, 
                        draw_info->drawing_info->object_color.g, 
                        draw_info->drawing_info->object_color.b, 
                        draw_info->drawing_info->object_color.a);
        TTF_DrawRendererText(font_text, draw_info->drawing_info->x, 
                        draw_info->drawing_info->y);

        TTF_DestroyText(font_text);

        SDL_free(strfps);
        return true;
}
bool drawResolutionText(TextDrawingInfo* draw_info, int desktop_width, 
                int desktop_height) 
{
        int max_resolution_str_len = 100; // rough overestimate
        char* str_resolution = SDL_malloc(sizeof(char) * max_resolution_str_len);
        sprintf(str_resolution, "resolution: %dx%d", desktop_width, desktop_height);
        TTF_Text* font_text = TTF_CreateText(
                        draw_info->text_engine, draw_info->font, 
                        str_resolution, strlen(str_resolution));
        TTF_SetTextColor(font_text, draw_info->drawing_info->object_color.r, 
                        draw_info->drawing_info->object_color.g, 
                        draw_info->drawing_info->object_color.b, 
                        draw_info->drawing_info->object_color.a);
        TTF_DrawRendererText(font_text, draw_info->drawing_info->x, 
                        draw_info->drawing_info->y);

        TTF_DestroyText(font_text);

        SDL_free(str_resolution);
        return true;
}

bool drawWindowSizeText(TextDrawingInfo* draw_info, int window_width, 
                int window_height)
{
        int max_window_str_len = 100; // rough overestimate
        char* str_window = SDL_malloc(sizeof(char) * max_window_str_len);
        
        sprintf(str_window, "window: (%d, %d)", window_width, window_height);
        TTF_Text* font_text = TTF_CreateText(
                        draw_info->text_engine, draw_info->font, 
                        str_window, strlen(str_window));
        TTF_SetTextColor(font_text, draw_info->drawing_info->object_color.r, 
                        draw_info->drawing_info->object_color.g, 
                        draw_info->drawing_info->object_color.b, 
                        draw_info->drawing_info->object_color.a);
        TTF_DrawRendererText(font_text, draw_info->drawing_info->x, 
                        draw_info->drawing_info->y);

        TTF_DestroyText(font_text);

        SDL_free(str_window);
        return true;
}

bool drawDebugInfo(TextDrawingInfo* text_drawing_info, DebugInfo* debug_info) 
{
        if(!SDL_SetTextureBlendMode(
                                text_drawing_info->drawing_info->texture, 
                                SDL_BLENDMODE_BLEND)) 
        {
                return false;
        }
        if(!SDL_SetTextureAlphaMod(
                                text_drawing_info->drawing_info->texture, 
                                100)) 
        {
                return false;
        }
        SDL_SetRenderTarget(text_drawing_info->drawing_info->renderer, 
                        text_drawing_info->drawing_info->texture);
        int original_x = text_drawing_info->drawing_info->x;
        int original_y = text_drawing_info->drawing_info->y;
        SDL_RenderClear(text_drawing_info->drawing_info->renderer);
        if(!drawFpsText(text_drawing_info, debug_info->fps)) 
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not draw fps text: %s", 
                                SDL_GetError());
        }
        text_drawing_info->drawing_info->y += 
                TTF_GetFontSize(text_drawing_info->font) * PIXELS_IN_ONE_PT;
        if(!drawResolutionText(text_drawing_info, debug_info->desktop_width, 
                                debug_info->desktop_height)) 
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not draw Resolution text: %s", 
                                SDL_GetError());
        }
        text_drawing_info->drawing_info->y += 
                TTF_GetFontSize(text_drawing_info->font) * PIXELS_IN_ONE_PT;
        if(!drawWindowSizeText(text_drawing_info, debug_info->window_width, debug_info->window_height)) 
        {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not draw Window Size text: %s", 
                                SDL_GetError());
        }
        text_drawing_info->drawing_info->x = original_x;
        text_drawing_info->drawing_info->y = original_y;
        SDL_SetRenderTarget(text_drawing_info->drawing_info->renderer, NULL);



}

