#include "application.h"
#include "draw.h"
#include "text.h"
#include "globals.h"
#include <stdint.h>
#include "systems.h"

void textEditorApplicationLoop(SDL_Window* window, SDL_Renderer* renderer, TTF_TextEngine* text_engine) {
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
        SDL_Surface* debug_surface = SDL_CreateSurface(window_width, window_height NULL);
        TTF_TextEngine* debug_text_engine = TTF_CreateSurfaceTextEngine(debug_surface);
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
                drawFpsText(debug_renderer, debug_text_engine, text_font, fps, FPS_TEXT_COLOR);
                SDL_SetRenderScale(renderer, width_scale_factor, height_scale_factor);
                SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
                SDL_RenderPresent(renderer);
                SDL_RenderPresent(debug_renderer);
                SDL_Event e;
                while(SDL_PollEvent(&e)) {
                        switch(e.type) {
                                case SDL_EVENT_QUIT:
                                        running = false;
                                        break;
                                case SDL_EVENT_WINDOW_RESIZED:
                                        SDL_Log("Resize event detected.");
                                        SDL_DestroyRenderer(renderer);
                                        SDL_DestroyRenderer(debug_renderer);
                                        TTF_DestroyRendererTextEngine(debug_text_engine);
                                        renderer = SDL_CreateRenderer(window, NULL);
                                        text_engine = TTF_CreateRendererTextEngine(renderer);
                                        debug_renderer = SDL_CreateRenderer(window, NULL);
                                        debug_text_engine = TTF_CreateRendererTextEngine(debug_renderer);
                                        break;

                        }
                }
                start_time = end_time;
                end_time = SDL_GetTicks();
                
        }
 

}

