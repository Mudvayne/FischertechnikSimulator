#include "lightBarrier.h"

void setBlocked(LightBarrier *lightBarrier)
{
    lightBarrier->isBlocked=1;
}
void setUnblocked(LightBarrier *lightBarrier)
{
    lightBarrier->isBlocked=0;
}
