#include "treadmill.h"

void startTreadmill(Treadmill *treadmill)
{
    treadmill->isRunning = 1;
}

void stopTreadmill(Treadmill *treadmill)
{
    treadmill->isRunning = 0;
}
