#include "application.h"
#include "draw.h"
#include "text.h"
#include "globals.h"
#include <stdint.h>
#include "systems.h"

void textEditorApplicationLoop(SDL_Window* window, 
                SDL_Renderer* renderer, TTF_TextEngine* text_engine) {
        SDL_PixelFormat window_pixel_fmt = SDL_GetWindowPixelFormat(window); 
        SDL_Log("WINDOW PIXEL FORMAT: %s", SDL_GetPixelFormatName(window_pixel_fmt));
        int formats_idx = 0;
        TTF_Font* debug_text_font = generateTextFont(DEBUG_FONT_SIZE, DEBUG_FONT_NAME);
        if(debug_text_font == NULL) {
                return;
        }
        SDL_Log("Generated ttf font");
        bool running = true;

        // s per frame is 1/60 for 60 fps.
        // ms per frame is 1000 * s per frame.
        float ms_per_frame = 1000.0f/(float)MAX_FPS; // 
        float fps = MAX_FPS; 
        uint64_t start_time = 0;
        uint64_t end_time = 0;
        uint64_t delta = 0;
        int window_width = ORIGINAL_WINDOW_WIDTH;
        int window_height = ORIGINAL_WINDOW_HEIGHT;
        SDL_Texture* texture_to_render = SDL_CreateTexture(renderer, 
                        window_pixel_fmt, SDL_TEXTUREACCESS_TARGET, 
                        window_width, window_height);  
        SDL_Log("original window size: %d, %d", window_width, window_height);
        bool do_resizing = false;
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
                float width_scale_factor = (float)window_width/(float)ORIGINAL_WINDOW_WIDTH;
                float height_scale_factor = (float)window_height/(float)ORIGINAL_WINDOW_HEIGHT;
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, 
                                BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
                drawFpsText(renderer, texture_to_render, text_engine, debug_text_font, fps, FPS_TEXT_COLOR);
                SDL_RenderTexture(renderer, texture_to_render, NULL, NULL);
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

                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyTexture(texture_to_render);
                                        renderer = SDL_CreateRenderer(window, NULL);
                                        text_engine = TTF_CreateRendererTextEngine(renderer);
                                        texture_to_render = SDL_CreateTexture(renderer, window_pixel_fmt, 
                                                        SDL_TEXTUREACCESS_TARGET, window_width, window_height);  

                                        break;
                                case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
                                        SDL_DisplayMode* current_display_mode = SDL_GetCurrentDisplayMode(
                                                        SDL_GetPrimaryDisplay());
                                        DESKTOP_WIDTH = current_display_mode->w;
                                        DESKTOP_HEIGHT = current_display_mode->h;
                                        SDL_Log("Display moved");
                                        break;

                        }
                }
                start_time = end_time;
                end_time = SDL_GetTicks();
                
        }
 

}

