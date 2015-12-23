#include "program.h"

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
}
