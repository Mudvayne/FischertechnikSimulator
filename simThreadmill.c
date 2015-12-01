#include "threadmill.h"

void run(Threadmill *threadmill)
{
    threadmill->isRunning = 1;
}

void stop(Threadmill *threadmill)
{
    threadmill->isRunning = 0;
}
