#include "application.h"
#include "draw.h"
#include "text.h"
#include "globals.h"
#include <stdint.h>
#include "systems.h"
#include "debug.h"

void textEditorApplicationLoop(SDL_Window* window, 
                SDL_Renderer* renderer, TTF_TextEngine* text_engine) {
        SDL_PixelFormat window_pixel_fmt = SDL_GetWindowPixelFormat(window); 
        SDL_Log("WINDOW PIXEL FORMAT: %s", SDL_GetPixelFormatName(window_pixel_fmt));
        TTF_Font* debug_text_font = generateTextFont(DEBUG_FONT_SIZE, DEBUG_FONT_NAME);
        if(debug_text_font == NULL) {
                SDL_Log("Could not generate debug text font: %s", SDL_GetError());
                return;
        }
        Debug_Info application_debug_info;
        SDL_Log("Generated ttf font");
        bool running = true;
        // s per frame is 1/60 for 60 fps.
        // ms per frame is 1000 * s per frame.
        float ms_per_frame = 1000.0f/(float)MAX_FPS; // 
        float fps = MAX_FPS; 
        uint64_t start_time = 0;
        uint64_t end_time = 0;
        uint64_t delta = 0;
        application_debug_info.fps = fps;
        int window_width = ORIGINAL_WINDOW_WIDTH;
        int window_height = ORIGINAL_WINDOW_HEIGHT;
        application_debug_info.window_width = window_width;
        application_debug_info.window_height = window_height;
        application_debug_info.desktop_width = DESKTOP_WIDTH;
        application_debug_info.desktop_height = DESKTOP_HEIGHT;
        SDL_Texture* debug_info_texture = SDL_CreateTexture(renderer, 
                        window_pixel_fmt, SDL_TEXTUREACCESS_TARGET, 
                        window_width, window_height);  
        SDL_Log("debug info: original window size: %d, %d", window_width, window_height);
        while(running) {
                if(start_time == 0) {
                        start_time = SDL_GetTicks();
                }
                else {
                        delta = end_time - start_time;
                }
                if(delta < ms_per_frame) {
                        SDL_Delay(ms_per_frame - delta);
                }
                if(delta > ms_per_frame) {
                        fps = 1000.0f / (float)delta;  
                }
                application_debug_info.fps = fps;
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                                BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
                
                if(!drawDebugInfo(renderer, debug_info_texture, text_engine,
                                debug_text_font, &application_debug_info, 
                                FPS_TEXT_COLOR)) {
                        SDL_Log("Could not draw debug info: %s", SDL_GetError()); 
                }
                SDL_RenderTexture(renderer, debug_info_texture, NULL, NULL);
                SDL_RenderPresent(renderer);
                SDL_Event e;
                while(SDL_PollEvent(&e)) {
                        switch(e.type) {
                                case SDL_EVENT_QUIT:
                                        running = false;
                                        break;
                                case SDL_EVENT_WINDOW_RESIZED:
                                        SDL_Log("Resize event detected.");
                                        SDL_GetWindowSizeInPixels(window, &window_width, 
                                                                        &window_height);
                                        application_debug_info.window_width = window_width;
                                        application_debug_info.window_height = window_height;
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyTexture(debug_info_texture);
                                        renderer = SDL_CreateRenderer(window, NULL);
                                        text_engine = TTF_CreateRendererTextEngine(renderer);
                                        debug_info_texture = SDL_CreateTexture(renderer, window_pixel_fmt, 
                                                        SDL_TEXTUREACCESS_TARGET, window_width, window_height);  

                                        break;
                                case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
                                        SDL_DisplayMode* current_display_mode = SDL_GetCurrentDisplayMode(
                                                        SDL_GetPrimaryDisplay());
                                        DESKTOP_WIDTH = current_display_mode->w;
                                        DESKTOP_HEIGHT = current_display_mode->h;
                                        application_debug_info.desktop_width = DESKTOP_WIDTH;
                                        application_debug_info.desktop_height = DESKTOP_HEIGHT;
                                        SDL_Log("Display moved");
                                        break;

                        }
                }
                start_time = end_time;
                end_time = SDL_GetTicks();
                
        }
 

}

