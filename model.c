#include "model.h"

Treadmill firstTreadmill = {.id=0, .isRunning=0};
Treadmill secondTreadmill = {.id=1, .isRunning=0};
Treadmill thirdTreadmill = {.id=2, .isRunning=0};
Treadmill fourthTreadmill = {.id=3, .isRunning=0};

Pusher firstPusher = {.id=0, .runningDirection=INACTIVE};
Pusher secondPusher = {.id=1, .runningDirection=INACTIVE};

LightBarrier firstLightBarrier = {.id=0, .isBlocked = 0};
LightBarrier secondLightBarrier = {.id=1, .isBlocked = 0};
LightBarrier thirdLightBarrier = {.id=2, .isBlocked = 0};
LightBarrier fourthLightBarrier = {.id=3, .isBlocked = 0};
LightBarrier fifthLightBarrier = {.id=4, .isBlocked = 0};

Tool firstTool = {.id=0, .isRunning=0};
Tool secondTool = {.id=1, .isRunning=0};

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
