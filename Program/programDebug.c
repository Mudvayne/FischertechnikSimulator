#include "program.h"

void debugInit() {
	//DO NOTHING!
}

void debugComputeActions()
{
    //DO NOTHING!
}

void stopPusherWhenTriggered(Pusher *pusher) {
    if(pusher->isBackTriggerActivated && pusher->runningDirection == BACKWARDS) {
        stopPusher(pusher);
    }
    if(pusher->isFrontTriggerActivated && pusher->runningDirection == FORWARDS) {
        stopPusher(pusher);
    }
}

void debugHandleActors()
{
    //Taking care that Pusher are not getting damaged.
    stopPusherWhenTriggered(getFirstPusher());
    stopPusherWhenTriggered(getSecondPusher());
}
