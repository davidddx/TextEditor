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
        TTF_Font* text_font = generateTextFont(FONT_SIZE, FONT_NAME);
        if(text_font == NULL) {
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
        int window_width;
        int window_height;
        SDL_GetWindowSizeInPixels(window, &window_width, 
                        &window_height);
        SDL_Texture* texture_to_render = SDL_CreateTexture(renderer, window_pixel_fmt, SDL_TEXTUREACCESS_TARGET, window_width, window_height);  
        SDL_Log("original window size: %f, %f", window_width, window_height);
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
                SDL_GetWindowSizeInPixels(window, &window_width, 
                                &window_height);
                float width_scale_factor = (float)window_width/(float)ORIGINAL_WINDOW_WIDTH;
                float height_scale_factor = (float)window_height/(float)ORIGINAL_WINDOW_HEIGHT;
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 
                                BACKGROUND_COLOR.a);
                drawFpsText(renderer, texture_to_render, text_engine, text_font, fps, FPS_TEXT_COLOR);
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
                                        float texture_width;
                                        float texture_height;
                                        SDL_GetTextureSize(texture_to_render, &texture_width, &texture_height);
                                        SDL_Log("texture size: %f, %f", texture_width, texture_height);
                                        SDL_Log("window width: %f, %f", window_width, window_height);
                                        break;

                        }
                }
                start_time = end_time;
                end_time = SDL_GetTicks();
                
        }
 

}

