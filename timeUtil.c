#include "timeUtil.h"

#ifdef WITH_MAKE_FILE
#define NULL 0
#endif

long long lastCall = 0;
long long lastCallSimulator = 0;

long long calculateTimeDiffSinceLastCall()
{
    struct timeval now;
    gettimeofday(&now, NULL); // get current time

    if(lastCall==0)
    {
        lastCall = now.tv_sec*1000LL + now.tv_usec/1000; // caculate milliseconds
        return 0;
    }
    long long callNow = now.tv_sec*1000LL + now.tv_usec/1000;
    long long retVal = callNow - lastCall;
    lastCall = callNow;
    return retVal;
}

long long calculateTimeDiffSinceLastCallSimulator()
{
    struct timeval now;
    gettimeofday(&now, NULL); // get current time

    if(lastCall==0)
    {
        lastCallSimulator = now.tv_sec*1000LL + now.tv_usec/1000; // caculate milliseconds
        return 0;
    }
    long long callNow = now.tv_sec*1000LL + now.tv_usec/1000;
    long long retVal = callNow - lastCallSimulator;
    lastCallSimulator = callNow;
    return retVal;
}
