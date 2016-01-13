#include "program.h"

void panicInit() {
    //DO NOTHING!
}

void panicComputeActions()
{
    //DO NOTHING!
}

void panicHandleActors()
{
    stopTreadmill(getFirstTreadmill());
    stopTreadmill(getSecondTreadmill());
    stopTreadmill(getThirdTreadmill());
    stopTreadmill(getFourthTreadmill());

    stopTool(getFirstTool());
    stopTool(getSecondTool());

    stopPusher(getFirstPusher());
    stopPusher(getSecondPusher());
}
