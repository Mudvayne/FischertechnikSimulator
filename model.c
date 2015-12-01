#include "model.h"

Threadmill firstThreadmill = {.isRunning=0};
Threadmill secondThreadmill = {.isRunning=0};
Threadmill thirdThreadmill = {.isRunning=0};
Threadmill fourthThreadmill = {.isRunning=0};

Pusher firstPusher = {.runningDirection=INACTIVE};
Pusher secondPusher = {.runningDirection=INACTIVE};

LightBarrier firstLightBarrier = {.isBlocked = 0};
LightBarrier secondLightBarrier = {.isBlocked = 0};
LightBarrier thirdLightBarrier = {.isBlocked = 0};
LightBarrier fourthLightBarrier = {.isBlocked = 0};
LightBarrier fifthLightBarrier = {.isBlocked = 0};

Tool firstTool = {.isRunning=0};
Tool secondTool = {.isRunning=0};

Threadmill* getFirstThreadmill()
{
    return &firstThreadmill;
}

Threadmill* getSecondThreadmill()
{
    return &secondThreadmill;
}

Threadmill* getThirdThreadmill()
{
    return &thirdThreadmill;
}

Threadmill* getFourthThreadmill()
{
    return &fourthThreadmill;
}

Pusher* getFirstPusher()
{
    return &firstPusher;
}

Pusher* getSecondPusher()
{
    return &secondPusher;
}

LightBarrier* getFirstLightBarrier()
{
    return &firstLightBarrier;
}

LightBarrier* getSecondLightBarrier()
{
    return &secondLightBarrier;
}

LightBarrier* getThirdLightBarrier()
{
    return &thirdLightBarrier;
}

LightBarrier* getFourthLightBarrier()
{
    return &fourthLightBarrier;
}

LightBarrier* getFifthLightBarrier()
{
    return &fifthLightBarrier;
}

Tool* getFirstTool()
{
    return &firstTool;
}

Tool* getSecondTool()
{
    return &secondTool;
}
