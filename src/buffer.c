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

void destroyLine(Line* line) {
        SDL_free(line->arr);
        SDL_free(line);
}

void destroyGapBuffer(GapBuffer* buffer) {
        for(int i = 0; i < buffer->lines_capacity; ++i) {
                destroyLine(buffer->lines[i]);
        }
        SDL_free(buffer);
}



void logGap(Gap* gap) {
        if(gap == NULL) {
                SDL_Log("gap=NULL");
                return;
        }
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

void logBufferInfo(GapBuffer* buffer) 
{
        SDL_Log("\tlogging buffer info...");
        SDL_Log("buffer starting line number: %lld", buffer->starting_line_number);
        SDL_Log("buffer ending line number: %lld", buffer->ending_line_number);
        Line* current_line = buffer->lines[buffer->lines_index];
        SDL_Log("lines index: %d", buffer->lines_index);
        SDL_Log("current_line");
        logLine(current_line);
        SDL_Log("\tlogging buffer info done");
}

void logBufferText(GapBuffer* buffer) 
{
        for(int i = buffer->starting_line_number; i <= buffer->ending_line_number; ++i) {
                SDL_Log("line %d", i);
                logLine(buffer->lines[i - buffer->starting_line_number]);
        }
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
        if(new_gap->start_position > line->arr_size) {
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
        char* new_str = SDL_malloc(sizeof(char) * line->arr_capacity); 
        SDL_memcpy(new_str, line->arr, sizeof(char) * new_gap->start_position); 
        char* new_str_at_gap = new_str + new_gap->start_position;
        SDL_memset(new_str_at_gap, ' ', sizeof(char) * new_gap->size);
        char* new_str_after_gap = new_str + new_gap->end_position + 1;
        int last_index = new_gap->size + line->arr_size - 1;
        int right_end_size =  last_index - (new_gap->end_position + 1) + 1;
        
        SDL_memcpy(new_str_after_gap, line->arr + new_gap->start_position, sizeof(char) * right_end_size);
        //logLine(line);
        line->arr_size += new_gap->size;
        SDL_free(line->arr);
        line->arr = new_str;
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

int getCurrentLineNumber(GapBuffer* gap_buffer) {
        return gap_buffer->starting_line_number + gap_buffer->lines_index; 
}

// moves gap within stuff.
void moveCursor(GapBuffer* gap_buffer, Gap** prev_gap, 
                unsigned long long line_number, 
                int line_position, Gap** new_gap) 
{
        // line number is the line number, line position is the position within the Line struct.
        if(gap_buffer->starting_line_number > line_number 
                        || gap_buffer->ending_line_number < line_number) {
                *new_gap = NULL;
                return;
        }
        if(line_number == getCurrentLineNumber(gap_buffer) && 
                        line_position == (*prev_gap)->start_position) 
        {
                *new_gap = createGap((*prev_gap)->start_position, (*prev_gap)->end_position);
                return;
        }
        removeGap(gap_buffer->lines[gap_buffer->lines_index], *prev_gap);       
        Line* current_line = gap_buffer->lines[line_number - gap_buffer->starting_line_number];
        if(line_position < 0) { 
                // keep line position >= 0
                line_position = 0;
        }
        if(line_position > current_line->arr_size) {
                // keep line position <= current_line->arr_size 
                line_position = current_line->arr_size;
        }
        Gap* gap_to_insert = createGap(line_position, line_position + MAX_GAP_SIZE - 1);
        insertGap(current_line, gap_to_insert); 
        *new_gap = gap_to_insert;

}

GapBuffer* createGapBuffer(Line** lines, int lines_size, 
                unsigned long long starting_line_number) 
{
        if(lines_size > MAX_LINE_BUFFER_NUM) 
        {
                SDL_Log("cannot fit all these lines into one buffer: lines_size = %d but MAX_LINE_BUFFER_NUM = %d", lines_size, MAX_LINE_BUFFER_NUM);
        }
        if(lines == NULL) 
        {
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
        GapBuffer* buffer = SDL_malloc(sizeof(GapBuffer));
        buffer->starting_line_number = starting_line_number;
        buffer->ending_line_number = starting_line_number + lines_size - 1;
        buffer->lines_index = 0;
        buffer->lines_capacity = MAX_LINE_BUFFER_NUM;
        Line** buffer_lines = SDL_malloc(sizeof(Line*) * MAX_LINE_BUFFER_NUM);
        buffer->lines = buffer_lines;
        for(int i = 0; i < lines_size; ++i) {
                buffer_lines[i] = lines[i]; 
        }
        for(int i = lines_size; i < MAX_LINE_BUFFER_NUM; ++i) 
        {
                Line* line = createLine(NULL);
                buffer_lines[i] = line;
        }
        return buffer;
}

bool initializeBuffer(FILE* file) 
{
        if(file == NULL) {
                // new file mode:
                // file does not currently exist so 
                // number of loaded buffers is 1.
                LOADED_BUFFERS = SDL_malloc(sizeof(GapBuffer*));
                NUM_LOADED_BUFFERS = 1;
                GapBuffer* initial_buffer = createGapBuffer(NULL, 0, 0);
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

bool destroyAllBuffers() {
        if(CURRENT_BUFFER == NULL) {
                return false;
        }
        for(int i = 0; i < NUM_LOADED_BUFFERS; ++i) {
                if(LOADED_BUFFERS[i] == NULL) {
                        return false;
                }
                destroyGapBuffer(LOADED_BUFFERS[i]);
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
        Line* line = createLine("aaaaaa ");
        gap = createGap(6, 6);
        singleTestRemoveGap(line, gap);
        SDL_free(line->arr);
        SDL_free(gap);
        SDL_free(line);
        SDL_Log("test #2.");
        line = createLine("bbbb    AAAA");
        gap = createGap(4,7);
        singleTestRemoveGap(line, gap);
        SDL_free(line->arr);
        SDL_free(gap);
        SDL_free(line);
        SDL_Log("Remove Gap test finished");
}


void testInitializeBuffer() {
        // first testing with file=NULL
        SDL_Log("==testing initialize buffer==");
        if(initializeBuffer(NULL)) {
                logBufferInfo(CURRENT_BUFFER);
                destroyAllBuffers();
                SDL_Log("==buffer initialized, test finished==");
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
        SDL_Log("gap");
        Gap* gap3 = createGap(0, 5);
        insertGap(line3, gap3);
        logGap(gap3);
        SDL_Log("line after");
        logLine(line3);
        SDL_Log("===test 3 finished===");
        SDL_Log("===test 4===");
        Line* line4 = createLine("line4");
        SDL_Log("line before");
        logLine(line4);
        Gap* gap4 = createGap(line4->arr_size, line4->arr_size + 9);
        SDL_Log("gap");
        logGap(gap4);
        insertGap(line4, gap4);
        SDL_Log("line after");
        logLine(line4);
        SDL_Log("===test 4 finished===");
        SDL_Log("===test 5===");
        Line* line5 = createLine("");
        Gap* gap5 = createGap(0, 10);
        SDL_Log("line before");
        logLine(line5);
        SDL_Log("gap");
        logGap(gap5);
        insertGap(line5, gap5);
        SDL_Log("line after");
        logLine(line5);
        SDL_Log("===test 5 finished===");
        SDL_Log("==insert gap test finished==");
        SDL_free(line1->arr);
        SDL_free(line2->arr);
        SDL_free(line3->arr);
        SDL_free(line4->arr);
        SDL_free(line5->arr);
        SDL_free(line1);
        SDL_free(line2);
        SDL_free(line3);
        SDL_free(line4);
        SDL_free(line5);
        SDL_free(gap1);
        SDL_free(gap2);
        SDL_free(gap3);
        SDL_free(gap4);
        SDL_free(gap5);

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

void testCreateGapBuffer() {
        SDL_Log("==testing create gap buffer==");
        SDL_Log("===test 1===");
        Line* line1 = createLine("");
        Line** lines1 = SDL_malloc(sizeof(Line*)); 
        lines1[0] = line1;
        GapBuffer* gap_buffer_1 = createGapBuffer(lines1, 1, 0);
        logBufferText(gap_buffer_1);
        SDL_Log("===test 1 finished===");

        SDL_Log("===test 2===");
        Line** lines2 = SDL_malloc(sizeof(Line*) * 10);
        for(int i = 0; i < 10; ++i) {
                char* curr_str;
                SDL_asprintf(&curr_str, "line%d", i);
                Line* line = createLine(curr_str);
                lines2[i] = line;
                SDL_free(curr_str);
        }
        GapBuffer* gap_buffer_2 = createGapBuffer(lines2, 10, 0);
        logBufferText(gap_buffer_2);
        destroyGapBuffer(gap_buffer_1);
        destroyGapBuffer(gap_buffer_2);
        SDL_Log("===test 2 finished===");
        SDL_Log("==testing create gap buffer done==");
}

void singleTestMoveCursor(int test_num, 
                GapBuffer* buff, Gap* gap, 
                int line_position, 
                unsigned long long line_number) 
{
        SDL_Log("===test %d===", test_num);
        insertGap(buff->lines[buff->lines_index], gap);
        Gap* new_gap = createGap(0, 0);
        SDL_Log(" before ");
        logGap(gap);
        logBufferText(buff);
        moveCursor(buff, &gap, line_number, line_position, &new_gap);
        SDL_Log(" after ");
        logGap(new_gap);
        logBufferText(buff);
        logBufferInfo(buff);
        if(new_gap != NULL) {
                SDL_free(new_gap);
        }
        SDL_Log("===test %d finished===", test_num);
}

void testMoveCursor() {
        SDL_Log("==testing move cursor==");
        int lines_size = 10;
        Line** lines = SDL_malloc(sizeof(Line*) * lines_size);
        for(int i = 0; i < lines_size; ++i) {
                char* s = SDL_malloc(sizeof(char) * 20);
                SDL_asprintf(&s, "line%d", i);
                lines[i] = createLine(s); 
                SDL_free(s);
        }
        GapBuffer* buff1 = createGapBuffer(lines, lines_size, 0);
        int beginning_of_gap = 1;
        Gap* gap1 = createGap(beginning_of_gap, beginning_of_gap + MAX_GAP_SIZE - 1);
        singleTestMoveCursor(1, buff1, gap1, 0, 0);
        destroyGapBuffer(buff1);
        SDL_free(gap1);
        lines = SDL_malloc(sizeof(Line*) * lines_size);
        for(int i = 0; i < lines_size; ++i) {
                char* s = SDL_malloc(sizeof(char) * 20);
                SDL_asprintf(&s, "line%d", i);
                lines[i] = createLine(s); 
                SDL_free(s);
        }
        GapBuffer* buff2 = createGapBuffer(lines, lines_size, 0);
        beginning_of_gap = 4;
        Gap* gap2 = createGap(beginning_of_gap, beginning_of_gap + MAX_GAP_SIZE - 1);
        buff2->lines_index = 3;
        singleTestMoveCursor(2, buff2, gap2, 1, 2);
        destroyGapBuffer(buff2);
        SDL_free(gap2);
        lines = SDL_malloc(sizeof(Line*) * lines_size);
        for(int i = 0; i < lines_size; ++i) {
                char* s = SDL_malloc(sizeof(char) * 20);
                SDL_asprintf(&s, "line%d", i);
                lines[i] = createLine(s); 
                SDL_free(s);
        }
        GapBuffer* buff3 = createGapBuffer(lines, lines_size, 10);
        Gap* gap3 = createGap(0, 10);
        singleTestMoveCursor(3, buff3, gap3, gap3->start_position, 0); 
        singleTestMoveCursor(3, buff3, gap3, gap3->start_position, 11); 
        SDL_Log("==finished testing move cursor==");
}

void testBufferFunctions() {
        testRemoveGap();
        testCreateLine();
        testInsertGap();
        testCreateGapBuffer();
        testInitializeBuffer();
        testMoveCursor();
        SDL_Log("buffer functions test finished");
}

bool updateBuffer(TextDrawingInfo* text_drawing_info) 
{
        //logBufferInfo(CURRENT_BUFFER);
        //testAllFunctions();
        return true;
}


