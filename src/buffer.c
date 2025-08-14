#include "buffer.h"
#include <SDL3/SDL.h>

GapBuffer** LOADED_BUFFERS = NULL; 
GapBuffer* CURRENT_BUFFER = NULL;
Gap* CURRENT_GAP = NULL;
unsigned int FIRST_LINE_NUMBER = 1;
unsigned int MAX_GAP_SIZE = 50;
unsigned int MAX_LINE_SIZE = 1000;
unsigned int MAX_LINE_BUFFER_NUM = 1000;
unsigned int MAX_NUM_LOADED_BUFFERS = 10;
int NUM_LOADED_BUFFERS = 0;

void logBufferInfo(GapBuffer* buffer) 
{
        SDL_Log("buffer starting line number: %lld", buffer->starting_line_number);
        SDL_Log("buffer ending line number: %lld", buffer->ending_line_number);
        Line* current_line = buffer->lines[buffer->lines_index];
        char* str_to_print = SDL_malloc(sizeof(char) * (current_line->arr_size + 1));
        SDL_memcpy(str_to_print, current_line, sizeof(char) * current_line->arr_size);
        str_to_print[current_line->arr_size] = '\0';
        SDL_Log("current_line: %s(END)", str_to_print);
        SDL_free(str_to_print);
}
void logGap(Gap* gap) {
        SDL_Log("gap=(start_position: %d, end_position: %d, size: %d)", 
                        gap->start_position, gap->end_position, gap->size);
}
void logLine(Line* line) {
        char* line_str = SDL_malloc(sizeof(char) * (line->arr_size + 1));
        SDL_memcpy(line_str, line->arr, sizeof(char) * line->arr_size);
        line_str[line->arr_size] = '\0';
        SDL_Log("line info\n\tarr: (START)%s(END)\n\tsize: %d\n\tcapacity: %d", line_str, line->arr_size, line->arr_capacity);
        SDL_free(line_str);
}
Line* createLine(char* str) 
{
        Line* rv = SDL_malloc(sizeof(Line));
        char* current_line_arr = SDL_malloc(sizeof(char) * MAX_LINE_SIZE); 
        rv->arr = current_line_arr;
        rv->arr_size = 0;
        rv->arr_capacity = MAX_LINE_SIZE;
        if(str == NULL) {
                return rv;
        }
        rv->arr_size = strlen(str);
        rv->arr_capacity = MAX_LINE_SIZE;
        SDL_memcpy(current_line_arr, str, sizeof(char) * strlen(str));
        return rv;
}

Gap* createGap(int start_position, int end_position) {
        if(start_position > end_position) {
                return NULL;
        }
        int size = end_position - start_position + 1;
        Gap* gap = SDL_malloc(sizeof(Gap));
        gap->start_position = start_position;
        gap->end_position = end_position;
        gap->size = size;
        return gap;
        
}

// function assumes that there is no gap currently i.e. the former gap has been removed..
bool insertGap(Line* line, Gap* new_gap) {
        if(new_gap->start_position >= line->arr_size) {
                SDL_Log("could not insert gap: gap start position %d is greater than line arr size %d", new_gap->start_position, line->arr_size);
                return false;
        }
        if(new_gap->start_position < 0) {
                SDL_Log("could not insert gap: gap start position is %d which is less than 0.", new_gap->start_position);
                return false;
        }
        // best way to understand this code is using an example with len(arr) = 10
        // inserting gap start=1, end=3 to arr[0:9]
        // turns arr[0:9] to arr[0:12] = arr[0] + gap_space + gap_space + gap_space + arr[1:9]
        SDL_Log("line before");
        //logLine(line);
        char* new_str = SDL_malloc(sizeof(char) * line->arr_capacity); 
        SDL_memcpy(new_str, line->arr, sizeof(char) * new_gap->start_position); 
        char* new_str_at_gap = new_str + new_gap->start_position;
        SDL_memset(new_str_at_gap, ' ', sizeof(char) * new_gap->size);
        char* new_str_after_gap = new_str + new_gap->end_position + 1;
        int last_index = new_gap->size + line->arr_size - 1;
        int right_end_size =  last_index - (new_gap->end_position + 1) + 1;
        
        SDL_Log("right end size: %d", right_end_size);
        SDL_memcpy(new_str_after_gap, line->arr + new_gap->start_position, sizeof(char) * right_end_size);
        //logLine(line);
        line->arr_size += new_gap->size;
        SDL_Log("line after");
        SDL_free(line->arr);
        line->arr = new_str;
        SDL_Log("line_arr[%d]: %c", line->arr_size - 1, line->arr[line->arr_size - 1]);
        //logLine(line);
        return true;
}

void removeGap(Line* line, Gap* gap) {
        char* new_line_arr = SDL_malloc(sizeof(char) * (line->arr_size - gap->size));
        int curr_idx = 0;
        for(int i = 0; i < gap->start_position; ++i, ++curr_idx) {
                new_line_arr[curr_idx] = line->arr[i];
        }
        for(int i = gap->end_position + 1; i < line->arr_size; ++i, ++curr_idx) {
                new_line_arr[curr_idx] = line->arr[i]; 
        }
        SDL_free(line->arr);
        line->arr = new_line_arr;
        line->arr_size = line->arr_size - gap->size; 
}

void moveCursor(GapBuffer* gap_buffer, Gap* gap, unsigned long long line_number, 
                int string_position) 
{
        if(line_number != gap_buffer->lines_index + 1 || 
                        string_position > CURRENT_GAP->end_position || 
                        string_position < CURRENT_GAP->start_position) 
        {
                removeGap(gap_buffer->lines[gap_buffer->lines_index], CURRENT_GAP);
        }
        if(gap_buffer->ending_line_number > line_number) {
                // line number less than this buffer's starting line number..
                line_number = gap_buffer->ending_line_number;

        }
        if(gap_buffer->starting_line_number < line_number) {
                // line number greater than this buffer's ending line number..
                line_number = gap_buffer->starting_line_number;
        }
        Line* current_line = gap_buffer->lines[line_number - gap_buffer->starting_line_number];
        if(string_position < 0) {
                string_position = 0;
        }
        if(string_position >= current_line->arr_size) {
                // bounded above by current_line->arr_size..
                string_position = current_line->arr_size - 1;
        }
        // modify the gap in the string.
        if(string_position > gap->end_position) {


        }
        else if (string_position < gap->start_position) {

        }
        else {

        }
        //moveGap(current_line, CURRENT_GAP);
}

GapBuffer* createGapBuffer(Line** lines, int lines_size) 
{
        if(lines == NULL) {
                GapBuffer* buffer = SDL_malloc(sizeof(GapBuffer));
                // allocating memory to lines in gap buffer
                Line** buffer_lines = SDL_malloc(sizeof(Line*) * MAX_LINE_BUFFER_NUM);
                for(int i = 0; i < MAX_LINE_BUFFER_NUM; ++i) {
                        Line* line_mem_allocation = createLine(NULL);
                        buffer_lines[i] = line_mem_allocation; 
                }
                buffer->lines = buffer_lines;
                buffer->starting_line_number = 1;
                buffer->ending_line_number = 1;
                buffer->lines_index = 0;
                buffer->lines_capacity = MAX_LINE_BUFFER_NUM;
                return buffer;
        }
        return NULL;
}

bool initializeBuffer(FILE* file) 
{
        if(file == NULL) {
                // new file mode:
                // file does not currently exist so 
                // number of loaded buffers is 1.
                LOADED_BUFFERS = SDL_malloc(sizeof(GapBuffer*));
                NUM_LOADED_BUFFERS = 1;
                GapBuffer* initial_buffer = createGapBuffer(NULL, 0);
                CURRENT_BUFFER = initial_buffer;
                CURRENT_GAP = SDL_malloc(sizeof(Gap));
                CURRENT_GAP->start_position = 0;
                CURRENT_GAP->end_position = MAX_GAP_SIZE;
                CURRENT_GAP->size = MAX_GAP_SIZE;
                LOADED_BUFFERS[0] = CURRENT_BUFFER;
                return true;
        }
        return true;
}

bool destroyBuffer(GapBuffer* buffer) 
{
        if(buffer == NULL) {
                return false;
        }
        SDL_free(buffer);
        for(int i = 0; i < buffer->ending_line_number - buffer->starting_line_number; ++i) {
                SDL_free(buffer->lines[i]);
        }
        SDL_free(buffer->lines);
        return true;
}

bool destroyAllBuffers() {
        if(CURRENT_BUFFER == NULL) {
                return false;
        }
        for(int i = 0; i < NUM_LOADED_BUFFERS; ++i) {
                if(LOADED_BUFFERS[i] == NULL) {
                        return false;
                }
                SDL_free(LOADED_BUFFERS[i]);
        }
        return true;
}

void singleTestRemoveGap(Line* line, Gap* gap) {
        SDL_Log("line before removing gap: ");
        logLine(line);
        logGap(gap);
        removeGap(line, gap);
        SDL_Log("line after removing gap: ");
        logLine(line);
}

void testRemoveGap() {
        SDL_Log("testing remove gap.");
        SDL_Log("test #1.");
        Gap* gap;
        /*
        int test_arr_capacity = 500;
        char* line_arr = SDL_malloc(sizeof(char) * test_arr_capacity);
        Line* line = SDL_malloc(sizeof(Line));
        line->arr_capacity = test_arr_capacity;
        line_arr[0] = 'a';
        line_arr[1] = 'a';
        line_arr[2] = 'a';
        line_arr[3] = 'a';
        line_arr[4] = 'a';
        line_arr[5] = 'a';
        line_arr[6] = ' ';
        line->arr_size = 7;
        line->arr = line_arr;
        */
        Line* line = createLine("aaaaaa ");
        gap = createGap(6, 6);
        /*
        gap->start_position = 6;
        gap->end_position = 6;
        gap->size = 1;
        */
        singleTestRemoveGap(line, gap);
        SDL_free(line->arr);
        SDL_free(gap);
        SDL_free(line);
        SDL_Log("test #2.");
        /*
        char* test_str = "bbbb    AAAA"; 
        line_arr = SDL_malloc(sizeof(char) * test_arr_capacity);
        SDL_memcpy(line_arr, test_str, sizeof(char) * SDL_strlen(test_str));
        line = SDL_malloc(sizeof(Line));
        line->arr = line_arr;
        line->arr_size = SDL_strlen(test_str);
        line->arr_capacity = test_arr_capacity;
        */
        line = createLine("bbbb    AAAA");
        //gap = SDL_malloc(sizeof(Gap));
        gap = createGap(4,7);
        /*
        gap->start_position = 4; 
        gap->end_position = 7;
        gap->size = 4;
        */
        singleTestRemoveGap(line, gap);
        SDL_free(line->arr);
        SDL_free(gap);
        SDL_free(line);
        SDL_Log("Remove Gap test finished");
}

void testMoveCursor() {
        SDL_Log("testing moveCursor");
        SDL_Log("move cursor test finished");
}

void testInitializeBuffer() {
        // first testing with file=NULL
        SDL_Log("testing initialize buffer");
        if(initializeBuffer(NULL)) {
                logBufferInfo(CURRENT_BUFFER);
                destroyAllBuffers();
                SDL_Log("buffer initialized, test finished");
        }
}

void testInsertGap() {
        SDL_Log("==testing insert gap==");
        SDL_Log("===test 1===");
        Line* line1 = createLine("line1");
        Gap* gap1 = createGap(4, 4);
        SDL_Log("gap: ");
        logGap(gap1);
        SDL_Log("line before inserting gap");
        logLine(line1);
        insertGap(line1, gap1);
        SDL_Log("line after inserting gap");
        logLine(line1);
        SDL_Log("===test 1 done===");
        SDL_Log("===test 2===");
        Line* line2 = createLine("line2");
        Gap* gap2 = createGap(2, 4);
        SDL_Log("gap: ");
        logGap(gap2);
        SDL_Log("line before inserting gap");
        logLine(line2);
        insertGap(line2, gap2);
        SDL_Log("line after inserting gap");
        logLine(line2);
        SDL_Log("===test 2 done===");
        SDL_Log("===test 3===");
        Line* line3 = createLine("line3");
        SDL_Log("line before");
        logLine(line3);
        Gap* gap3 = createGap(0, 5);
        insertGap(line3, gap3);
        SDL_Log("line after");
        logLine(line3);
        SDL_Log("===test 3 finished===");
        SDL_Log("===test 4===");
        Line* line4 = createLine("line4");
        Gap* gap4 = createGap(line4->arr_size, line4->arr_size + 10);
        insertGap(line4, gap4);
        SDL_Log("==insert gap test finished==");
}

void testCreateLine() {
        SDL_Log("==testing create line==");
        char* str1 = "aaaaa";
        SDL_Log("attempting to ccreate line of contents %s", str1);
        Line* l = createLine(str1);
        logLine(l);
        char* str2 = "bbbbb";
        SDL_Log("attempting to ccreate line of contents %s", str2);
        Line* l2 = createLine(str2);
        logLine(l2);
        SDL_free(l->arr);
        SDL_free(l);
        SDL_free(l2->arr);
        SDL_free(l2);
        SDL_Log("==testing create line DONE==");
}

void testBufferFunctions() {
        testRemoveGap();
        testInitializeBuffer();
        testCreateLine();
        testInsertGap();
        testMoveCursor();
        SDL_Log("buffer functions test finished");
}




bool updateBuffer(TextDrawingInfo* text_drawing_info) 
{
        //logBufferInfo(CURRENT_BUFFER);
        //testAllFunctions();
        return true;
}


