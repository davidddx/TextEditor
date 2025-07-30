#include "cursor.h"

Cursor* TEXT_EDITOR_CURSOR; // file scope variable initialized in 
                            // function initializeCursor.

char* MIN_LINE_NUMBER = "1";

void doStringIncrement(char** string_ptr) {
        char* string = *string_ptr;
        SDL_Log("incrementing string. original string: %s", string);
        int len = SDL_strlen(string); 
        int carry = 0;
        int initial_num = string[len - 1] - '0';
        SDL_Log("initial_num: %d", initial_num);
        if(initial_num + 1 < 10) {
                string[len - 1] = (initial_num + '0') + 1;
                return;
        }
        SDL_Log("len: %d", len);
        string[len - 1] = '0';
        carry = 1;
        for(int i = len - 2; i >= 0; --i) {
                int num = string[i] - '0'; // turns char digit to int val.
                int updated_num = (num + carry) % 10; // base 10
                if(updated_num < num) {
                        carry = 1;
                }
                else {
                        carry = 0;
                }
                string[i] = updated_num + '0'; 
        }
        SDL_Log("string after carry computation: %s", string);
        //addition results in another digit.
        if(carry == 1) {
                SDL_Log("addind a digit!");
                char* new_str = SDL_malloc(sizeof(char) * (len + 1 + 1));
                SDL_memcpy(new_str + 1, string, len + 1); 
                new_str[len + 1] = '\0';
                new_str[0] = '1';
                SDL_free(string);
                *string_ptr = new_str;
                SDL_Log("new_str: %s", new_str);
        }
        SDL_Log("resulting string: %s", *string_ptr);
}

void doStringDecrement(char** string_ptr) {
        char* string = *string_ptr;
        // case cannot decrement string, positive values only.
        if(strcmp(string, MIN_LINE_NUMBER) == 0) {
                return;
        }
        SDL_Log("decrementing string. original string: %s", string);

        int len = SDL_strlen(string);
        int last_digit = string[len - 1] - '0';
        SDL_Log("last digit: %d", last_digit);
        if(last_digit - 1 >= 0) {
                --last_digit;
                string[len - 1] = (char)(last_digit + '0');
                return;
        }
        string[len - 1] = '9';
        int next_digit_decrement = 1;
        for(int i = len - 2; i >= 0; --i) {
                int curr_digit = (int)(string[i] - '0') - next_digit_decrement;
                SDL_Log("curr digit: %d", curr_digit);
                if(curr_digit >= 0) {
                        string[i] = (char)((curr_digit) + '0');
                        break;
                }
                string[i] = '9';
                next_digit_decrement = 1;
        }
        if(string[0] == '0') {
                char* new_str = SDL_malloc(sizeof(char) * (len - 1));
                memcpy(new_str, string + 1, sizeof(char) * (len - 1)); 
                new_str[len - 1] = '\0';
                SDL_free(string);
                *string_ptr = new_str;
        }
        SDL_Log("resulting string: %d", *string_ptr);
        
}

bool initializeCursor() {
        // test values for now, will replace later.
        Cursor* app_cursor = SDL_malloc(sizeof(Cursor));
        char* initial_line_number = "1";
        app_cursor->vertical_line_number = SDL_strdup(initial_line_number);
        app_cursor->horizontal_line_number = SDL_strdup(initial_line_number);
        app_cursor->is_image = false;
        app_cursor->image_path = NULL;
        app_cursor->image = NULL;
        app_cursor->ascii_code = 73;
        TEXT_EDITOR_CURSOR = app_cursor;
        return true;
}

bool updateCursor(ObjectDrawingInfo* drawing_info) {
        bool* key_states = SDL_GetKeyboardState(NULL);


        if(key_states[SDL_SCANCODE_UP] == true) {
                doStringIncrement(&(TEXT_EDITOR_CURSOR->vertical_line_number));

        }
        if(key_states[SDL_SCANCODE_DOWN] == true) {
                doStringDecrement(&(TEXT_EDITOR_CURSOR->vertical_line_number));

        }
        if(key_states[SDL_SCANCODE_LEFT] == true) {
                doStringDecrement(&(TEXT_EDITOR_CURSOR->horizontal_line_number));

        }
        if(key_states[SDL_SCANCODE_RIGHT] == true) {
                doStringIncrement(&(TEXT_EDITOR_CURSOR->horizontal_line_number));

        }
        SDL_Log("horizontal line number: %s", TEXT_EDITOR_CURSOR->horizontal_line_number);
        SDL_Log("vertical line number: %s", TEXT_EDITOR_CURSOR->vertical_line_number);

        return true;
}
