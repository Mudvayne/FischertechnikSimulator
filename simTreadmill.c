#include "treadmill.h"

void run(Treadmill *treadmill)
{
    treadmill->isRunning = 1;
}

void stop(Treadmill *treadmill)
{
    treadmill->isRunning = 0;
}
