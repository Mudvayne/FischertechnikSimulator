#include "model.h"

Threadmill firstThreadmill = {.isRunning=0};
Threadmill secondThreadmill = {.isRunning=0};
Threadmill thirdThreadmill = {.isRunning=0};
Threadmill fourthThreadmill = {.isRunning=0};

Pusher firstPusher;
Pusher secondPusher;

LightBarrier firstLightBarrier;
LightBarrier secondLightBarrier;
LightBarrier thirdLightBarrier;
LightBarrier fourthLightBarrier;
LightBarrier fifthLightBarrier;

Tool firstTool;
Tool secondTool;

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
