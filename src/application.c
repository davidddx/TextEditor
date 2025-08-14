#include "application.h"
#include "draw.h"
#include "text.h"
#include "globals.h"
#include "systems.h"
#include "debug.h"
#include "buffer.h"
#include <stdint.h>
void textEditorApplicationLoop(SDL_Window* window, 
                SDL_Renderer* renderer, TTF_TextEngine* text_engine) 
{
        if(TESTING) 
        {
                testBufferFunctions();
        }
        SDL_PixelFormat window_pixel_fmt = SDL_GetWindowPixelFormat(window); 
        //SDL_Log("WINDOW PIXEL FORMAT: %s", SDL_GetPixelFormatName(window_pixel_fmt));
        TTF_Font* debug_text_font = generateTextTTFFont(DEBUG_FONT_SIZE, DEBUG_FONT_NAME);
        if(debug_text_font == NULL) {
                SDL_Log("Could not generate debug text font: %s", SDL_GetError());
                return;
        }
        DebugInfo application_debug_info;

        SDL_Log("Generated ttf font");
        // s per frame is 1/60 for 60 fps.
        // ms per frame is 1000 * s per frame.
        float ms_per_frame = 1000.0f/(float)MAX_FPS; // 
        float fps = MAX_FPS; 
        uint64_t start_time = 0;
        uint64_t end_time = 0;
        uint64_t delta = 0;
        int window_width = ORIGINAL_WINDOW_WIDTH;
        int window_height = ORIGINAL_WINDOW_HEIGHT;
        // debug initialization stuff begins here,
        application_debug_info.fps = fps;
        application_debug_info.window_width = window_width;
        application_debug_info.window_height = window_height;
        application_debug_info.desktop_width = DESKTOP_WIDTH;
        application_debug_info.desktop_height = DESKTOP_HEIGHT;

        SDL_Texture* debug_info_texture = SDL_CreateTexture(renderer, 
                        window_pixel_fmt, SDL_TEXTUREACCESS_TARGET, 
                        window_width, window_height);  

        ObjectDrawingInfo debug_drawing_info;
        TextDrawingInfo debug_text_drawing_info;
        debug_text_drawing_info.drawing_info = &debug_drawing_info; 
        debug_text_drawing_info.text_engine = text_engine;
        debug_text_drawing_info.font = debug_text_font;
        debug_drawing_info.renderer = renderer;
        debug_drawing_info.texture = debug_info_texture;
        debug_drawing_info.x = 0;
        debug_drawing_info.y = 0;
        debug_drawing_info.object_color = DEBUG_TEXT_COLOR; 
        // debug initializing stuff ends here
        // application text texture is where stuff is written.
        SDL_Texture* application_texture = SDL_CreateTexture(renderer, 
                        window_pixel_fmt, SDL_TEXTUREACCESS_TARGET, 
                        window_width, window_height);

        ObjectDrawingInfo text_editor_object_drawing_info;
        text_editor_object_drawing_info.renderer = renderer;
        text_editor_object_drawing_info.texture  = application_texture;
        text_editor_object_drawing_info.x = 0;
        text_editor_object_drawing_info.y = 0;
        text_editor_object_drawing_info.object_color = APPLICATION_TEXT_COLOR; 
        TextDrawingInfo text_editor_text_drawing_info;
        text_editor_text_drawing_info.drawing_info = &text_editor_object_drawing_info;
        text_editor_text_drawing_info.text_engine = text_engine;

        text_editor_text_drawing_info.font = generateTextTTFFont(TEXT_EDITOR_FONT_SIZE, 
                        TEXT_EDITOR_FONT_NAME);
        SDL_SetRenderTarget(renderer, application_texture);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderFillRect(renderer, NULL);
        SDL_SetRenderTarget(renderer, NULL);
        bool running = true;
        SDL_StartTextInput(window);
        char* curr_str = SDL_malloc(sizeof(char) * MAX_TEXT_LENGTH_PER_FRAME);
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

                SDL_Event current_event;
                while(SDL_PollEvent(&current_event)) {
                        switch(current_event.type) {
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
                                                        SDL_TEXTUREACCESS_TARGET, window_width, 
                                                        window_height);  

                                        debug_drawing_info.texture = debug_info_texture; 
                                        debug_drawing_info.renderer = renderer;
                                        debug_text_drawing_info.text_engine = text_engine;
                                        SDL_DestroyTexture(application_texture);
                                        application_texture = SDL_CreateTexture(renderer, window_pixel_fmt, 
                                                        SDL_TEXTUREACCESS_TARGET, window_width, 
                                                        window_height);
        SDL_SetRenderTarget(renderer, application_texture);
        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderFillRect(renderer, NULL);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, NULL);
                                        text_editor_object_drawing_info.texture = application_texture;
                                        text_editor_object_drawing_info.renderer = renderer;
                                        text_editor_text_drawing_info.text_engine = text_engine;
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
                                case SDL_EVENT_TEXT_INPUT:
                                        // text input event means the event has text.
                                        // refer to struct SDL_TextInputEvent.
                                        SDL_Log("Input: %s", current_event.text.text);
                                        break;
                        }
                }
                application_debug_info.fps = fps;
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                                BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
                updateBuffer(&text_editor_text_drawing_info);
                
                if(!drawDebugInfo(&debug_text_drawing_info, &application_debug_info)) {
                        SDL_Log("Could not draw debug info: %s", SDL_GetError()); 
                }
                
                // updating texture stuff.
                SDL_RenderTexture(renderer, application_texture, NULL, NULL);
                SDL_RenderTexture(renderer, debug_info_texture, NULL, NULL);
                
                SDL_RenderPresent(renderer);
                start_time = end_time;
                end_time = SDL_GetTicks();
                //SDL_free(curr_str);
                
        }
 

}

