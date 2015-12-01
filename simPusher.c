#include "pusher.h"

void runForwardPusher(Pusher *pusher)
{
    pusher->runningDirection = FORWARD;
}
void runBackwardsPusher(Pusher *pusher)
{
    pusher->runningDirection = BACKWARDS;
}
void stopPusher(Pusher *pusher)
{
    pusher->runningDirection = INACTIVE;
}

void triggerTrigger(Pusher *pusher)
{
    pusher->isTriggerActivated = 1;
}
void untriggerTrigger(Pusher *pusher)
{
    pusher->isTriggerActivated = 0;
}
