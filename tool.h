#ifndef __TOOL
#define __TOOL
typedef struct {
	short id;
    short isRunning;
} Tool;
#endif // __TOOL

extern void startTool(Tool *tool);
extern void stopTool(Tool *tool);

