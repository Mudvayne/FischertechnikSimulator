#include "tool.h"

void startTool(Tool *tool)
{
    tool->isRunning = 1;
}
void stopTool(Tool *tool)
{
    tool->isRunning = 0;
}
