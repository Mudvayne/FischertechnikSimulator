#include "model.h"

Treadmill firstTreadmill = {.isRunning=0};
Treadmill secondTreadmill = {.isRunning=0};
Treadmill thirdTreadmill = {.isRunning=0};
Treadmill fourthTreadmill = {.isRunning=0};

Pusher firstPusher = {.runningDirection=INACTIVE};
Pusher secondPusher = {.runningDirection=INACTIVE};

LightBarrier firstLightBarrier = {.isBlocked = 0};
LightBarrier secondLightBarrier = {.isBlocked = 0};
LightBarrier thirdLightBarrier = {.isBlocked = 0};
LightBarrier fourthLightBarrier = {.isBlocked = 0};
LightBarrier fifthLightBarrier = {.isBlocked = 0};

Tool firstTool = {.isRunning=0};
Tool secondTool = {.isRunning=0};

Treadmill* getFirstTreadmill()
{
    return &firstTreadmill;
}

Treadmill* getSecondTreadmill()
{
    return &secondTreadmill;
}

Treadmill* getThirdTreadmill()
{
    return &thirdTreadmill;
}

Treadmill* getFourthTreadmill()
{
    return &fourthTreadmill;
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
