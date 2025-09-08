#ifndef DEBUG_H
#define DEBUG_H
typedef struct {
        float fps;
        int window_width;
        int window_height;
        int desktop_width;
        int desktop_height;

} DebugInfo;
extern bool TESTING_BUFFER;
#endif
