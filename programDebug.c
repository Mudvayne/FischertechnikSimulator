#include "program.h"

void debugComputeActions()
{
    //DO NOTHING!
}

void stopPusherWhenTriggered(Pusher *pusher) {
    if(pusher->isBackTriggerActivated || pusher->isFrontTriggerActivated) {
        stopPusher(pusher);
    }
}

void debugHandleActors()
{
    //Taking care that Pusher are not getting damaged.
    stopPusherWhenTriggered(getFirstPusher());
    stopPusherWhenTriggered(getSecondPusher());
}
