#include "pusher.h"

void runForwardPusher(Pusher *pusher)
{
    pusher->runningDirection = FORWARDS;
}
void runBackwardsPusher(Pusher *pusher)
{
    pusher->runningDirection = BACKWARDS;
}
void stopPusher(Pusher *pusher)
{
    pusher->runningDirection = INACTIVE;
}
