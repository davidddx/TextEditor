#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <SDL3/SDL.h>
#include "logger/log_setup.h"
#include "globals.h"
FILE** log_files;
void logFilesInit() {
        log_files = (FILE**)SDL_malloc(sizeof(FILE*) * (sizeof(SDL_LogCategory) + 1)); 
        for(int i = 0; i < sizeof(SDL_LogCategory) + 1; ++i) {
                log_files[i] = NULL;
        }
}

char* getLogCategoryStr(int category) {
        char* category_str;
        switch(category) {
                case SDL_LOG_CATEGORY_APPLICATION:
                        category_str = "APPLICATION";
                        break;
                case SDL_LOG_CATEGORY_ERROR:
                        category_str = "ERROR";
                        break;
                case SDL_LOG_CATEGORY_ASSERT:
                        category_str = "ASSERT";
                        break;
                case SDL_LOG_CATEGORY_SYSTEM:
                        category_str = "SYSTEM";
                        break;
                case SDL_LOG_CATEGORY_AUDIO:
                        category_str = "AUDIO";
                        break;
                case SDL_LOG_CATEGORY_VIDEO:
                        category_str = "VIDEO";
                        break;
                case SDL_LOG_CATEGORY_RENDER:
                        category_str = "RENDER";
                        break;
                case SDL_LOG_CATEGORY_INPUT:
                        category_str = "INPUT";
                        break;
                case SDL_LOG_CATEGORY_TEST:
                        category_str = "TEST";
                        break;
                case SDL_LOG_CATEGORY_GPU:
                        category_str = "GPU";
                        break;
                default:
                        category_str = "FULL";
                        break;
        }
        return category_str;
}
char* getLogPriorityStr(SDL_LogPriority priority) {
        char* priority_str;
        switch(priority) {
                case SDL_LOG_PRIORITY_INVALID:
                        priority_str = "INVALID";
                        break;
                case SDL_LOG_PRIORITY_TRACE:
                        priority_str = "TRACE";
                        break;
                case SDL_LOG_PRIORITY_VERBOSE:
                        priority_str = "VERBOSE";
                        break;
                case SDL_LOG_PRIORITY_DEBUG:
                        priority_str = "DEBUG";
                        break;
                case SDL_LOG_PRIORITY_INFO:
                        priority_str = "INFO";
                        break;
                case SDL_LOG_PRIORITY_WARN:
                        priority_str = "WARN";
                        break;
                case SDL_LOG_PRIORITY_ERROR:
                        priority_str = "ERROR";
                        break;
                case SDL_LOG_PRIORITY_CRITICAL:
                        priority_str = "CRITICAL";
                        break;
                case SDL_LOG_PRIORITY_COUNT:
                        priority_str = "COUNT";
                        break;
        }
        return priority_str;
}

SDL_LogOutputFunction doNoLogging(void* a, int category, SDL_LogPriority priority, const char* message) {
        return NULL;
}
SDL_LogOutputFunction logToFiles(void* a, int category, SDL_LogPriority priority, const char* message) {
        time_t current_time = time(NULL);
        if(current_time == (time_t) -1) {
                printf("logger function logToFiles is not working: could not fetch current time.\n");
                return NULL;
        }
        const unsigned int MAX_BUF_SIZE = 400; // just gonna use this for char* sizes because its a pretty reasonable overestimate 
        char* buf = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE);
        if(strftime(buf, MAX_BUF_SIZE, "%D %T", localtime(&current_time)) == 0) {
                printf("logger function logToFiles is not working: time strptime function not working\n"); 
                return NULL;
        }
        char* category_str = getLogCategoryStr(category);
        char* priority_str = getLogPriorityStr(priority);
        char* log_message_prefix_category = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE); 
        char* log_message_prefix_full = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE); 
        sprintf(log_message_prefix_category, "[%s] %s:", buf, priority_str);
        sprintf(log_message_prefix_full, "[%s %s] %s: ", buf, category_str, priority_str);
        char* file_name = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE);
        char* full_log_file_name = (char*) SDL_malloc(sizeof(char) * MAX_BUF_SIZE);
        sprintf(file_name, "%s%s%s.log", "logs", DIRECTORY_SEPARATOR, category_str);
        sprintf(full_log_file_name, "%s%s%s.log", "logs", DIRECTORY_SEPARATOR, "full");
        FILE* f;
        FILE* full_log_f;
        if(log_files[category] == NULL) {
                f = fopen(file_name, "w");
                log_files[category] = f;
        }
        else {
                f = log_files[category];
        }
        // location of fullLogFile
        if(log_files[sizeof(SDL_LogCategory)] == NULL) {
                full_log_f = fopen(full_log_file_name, "w");
                log_files[sizeof(SDL_LogCategory)] = full_log_f;
        }
        else {
                full_log_f = log_files[sizeof(SDL_LogCategory)];
        }
        fprintf(full_log_f, "%s %s\n", log_message_prefix_full, message);
        fprintf(f, "%s %s\n", log_message_prefix_category, message);
        SDL_free(log_message_prefix_category);
        SDL_free(log_message_prefix_full);
        SDL_free(file_name);
        SDL_free(full_log_file_name);
        SDL_free(buf);
        return NULL;
}
SDL_LogOutputFunction logToFilesAndShell (void* a, int category, SDL_LogPriority priority, const char* message) {
        logToFiles(a, category, priority, message);
        time_t current_time = time(NULL);
        if(current_time == (time_t) -1) {
                printf("logger function logToFilesAndShell is not working: could not fetch current time.\n");
                return NULL;
        }
        const unsigned int MAX_BUF_SIZE = 400; // just gonna use this for char* sizes because its a pretty reasonable overestimate 
        char* buf = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE);
        if(strftime(buf, MAX_BUF_SIZE, "%D %T", localtime(&current_time)) == 0) {
                printf("logger function logToFilesAndShell is not working: time strptime function not working\n"); 
                return NULL;
        }
        char* category_str = getLogCategoryStr(category);
        char* priority_str = getLogPriorityStr(priority);
        char* log_message_prefix_category = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE); 
        char* log_message_prefix_full = (char*)SDL_malloc(sizeof(char) * MAX_BUF_SIZE); 
        sprintf(log_message_prefix_category, "[%s] %s:", buf, priority_str);
        sprintf(log_message_prefix_full, "[%s %s] %s: ", buf, category_str, priority_str);
        printf("%s %s\n", log_message_prefix_full, message);
        SDL_free(buf);
        SDL_free(log_message_prefix_category);
        SDL_free(log_message_prefix_full);
        return NULL; 
}

void applicationLoggingInit() {
        void* a;
        if(!LOGGING) {
                SDL_SetLogOutputFunction((SDL_LogOutputFunction)doNoLogging, a);
                return;
        }
        logFilesInit();
        SDL_SetLogOutputFunction((SDL_LogOutputFunction)logToFilesAndShell, a);
        SDL_Log("logging initialized.");
}
void applicationLoggingClose() {
        if(!LOGGING) {
                return;
        }
        for(int i = 0; i < sizeof(SDL_LogCategory) + 1; ++i) {
                if(log_files[i] == NULL) {
                        continue;
                }
                if(fclose(log_files[i]) != 0) {
                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error closing %s log file: %s", getLogCategoryStr(i), strerror(errno));
                }
        }
        SDL_free(log_files);
}



