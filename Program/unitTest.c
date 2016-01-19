#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "model.h"
#include "program.h"

uint32_t timeDiff = 0;

void main() {
    getFourthLightBarrier()->isBlocked = 1;

    initStageFour();
    totalSystem.timeDiffSinceLastCall = 1;

    uint16_t timer = 0;
    for(; timer <= 1000; timer++) {
        utilStageFourBegin();
    }

    getFourthLightBarrier()->isBlocked = 0;
    for(;timer<=2000; timer++) {
        utilStageFourBegin();
    }


    totalSystem.timeDiffSinceLastCall = 4000;
    getFourthLightBarrier()->isBlocked = 0;
    utilStageFourBegin();

    totalSystem.timeDiffSinceLastCall = 1000;
    utilStageFourEnd();

    totalSystem.timeDiffSinceLastCall = 5000;
    utilStageFourEnd();

    totalSystem.timeDiffSinceLastCall = 10000;
    utilStageFourEnd();

    totalSystem.timeDiffSinceLastCall = 10000;
    utilStageFourEnd();
}

long long calculateTimeDiffSinceLastCall()
{
    return timeDiff;
}

long long calculateTimeDiffSinceLastCallSimulator()
{
    return timeDiff;
}
