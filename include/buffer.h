#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H
#include "draw.h"
#include <stdbool.h>
// using indices for gap buffer. 

// start position and end position is inclusive.
// i.e. str looks as follows
// {c0 c1 c2 c3 [gap_start_position, ... , gap_end_position] c4, ... cn-1)
// for characters ci where i is a natural number.
typedef struct {
        unsigned int start_position;
        unsigned int end_position;
        unsigned int size;
} Gap;
typedef struct {
        char* arr; // gaps go in here.
        unsigned int arr_size; // current size
        unsigned int arr_capacity; // capacity
} Line;
typedef struct {
        Line** lines; // array of lines
        unsigned long long starting_line_number; // starting line number.
        unsigned long long ending_line_number; // ending line number.
        int lines_capacity; // initialized capacity of the lines arr
        int lines_index; // current index in the lines arr.
} GapBuffer;

typedef enum {
        NO_ARROW_KEY,
        ARROW_KEY_UP,
        ARROW_KEY_DOWN,
        ARROW_KEY_LEFT,
        ARROW_KEY_RIGHT
} ARROW_KEY;

typedef enum {
        NO_MODIFIER_KEY,
        MODIFIER_KEY_SHIFT,
        MODIFIER_KEY_ALT,
        MODIFIER_KEY_CTRL,
        MODIFIER_KEY_BACKSPACE
} MODIFIER_KEY;

typedef struct {
        bool is_ascii;
        char ascii;
        MODIFIER_KEY modifier_key;
        ARROW_KEY arrow_key;
} ModifiedChar;

typedef struct {
        TextDrawingInfo* text_draw_info;
        ModifiedChar* characters_pressed;
        int characters_pressed_size;
} BufferUpdateInfo;

extern const char INVALID_ASCII;

bool updateBuffer(BufferUpdateInfo* buffer_update_info);
ModifiedChar initializeModifiedChar(void);
void testBufferFunctions(void);
bool initializeBuffer(FILE*);
#endif
