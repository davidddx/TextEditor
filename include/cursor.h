#ifndef CURSOR_H
#define CURSOR_H
#include <stdbool.h>
#include <stdint.h>
#include <draw.h>
bool initializeCursor(void);
bool updateCursor(ObjectDrawingInfo* drawing_info);
typedef struct {
        // position (1-indexed)
        char* vertical_line_number;
        char* horizontal_line_number;
        // appearance
        bool is_image;
        char* image_path; // only applies when is_image is true.
        SDL_Texture* image; // only applies when is_image is true.
        uint8_t ascii_code; // only applies when is_image is false.

} Cursor;

#endif
