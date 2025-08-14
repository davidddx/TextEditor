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

bool updateBuffer(TextDrawingInfo* text_drawing_info);

void testBufferFunctions();
#endif
