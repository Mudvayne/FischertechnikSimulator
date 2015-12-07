#include "program.h"
#include "model.h"
#include "treadmill.h"
#include "lightBarrier.h"
#include "timeUtil.h"

static int TRAVERSE_TIME_AFTER_SECOND_LB = 1500;
static int TRAVERSE_TIME_AFTER_CENTERED_LB = 500;

int remainingTimeFirstTM = 0;
int remainingTimeSecondTM = 0;
int remainingTimeThirdTM = 0;
int remainingTimeFourthTM = 0;

int itemCountFirstLB = 0;

int isFirstTmStopped = 0;
int isSecondTmStopped = 0;
int isThirdTmStopped = 0;
int isFourthTmStopped = 0;

int remainingTimeDriller = 0;
int remainingTimeMiller = 0;

int firstPusherDir = 1;
int secondPusherDir = 1;

int isFirstPlateFree = 1;
int isSecondPlateFree = 1;

int firstLightBarrierBefore = 0;

int firstTreadMillCount = 0;

long long timeDiffSinceLastCall = 0;

void aggregateSensors() {
    /*
    LightBarrier *firstLightBarrier = getFirstLightBarrier();
    LightBarrier *secondLightBarrier = getSecondLightBarrier();
    LightBarrier *thridLightBarrier = getThirdLightBarrier();
    LightBarrier *fourthLightBarrier = getFourthLightBarrier();
    LightBarrier *fifthLightBarrier = getFifthLightBarrier();

    automatTmp = firstLightBarrier->isBlocked;
    automatTmp2 = secondLightBarrier->isBlocked;
    */
    timeDiffSinceLastCall = calculateTimeDiffSinceLastCall();
}

void computeFirstTreadmill()
{
    if(remainingTimeFirstTM > 0)
    {
        if(isFirstTmStopped == 0){
            remainingTimeFirstTM -= timeDiffSinceLastCall;
        }
    }
    else if(itemCountFirstLB > 0)
    {
        itemCountFirstLB --;
        isFirstPlateFree = 0;
    }

    if(getFirstLightBarrier()->isBlocked == 1)
    {
        if(firstLightBarrierBefore == 0)
        {
            remainingTimeFirstTM = 65535;
            itemCountFirstLB ++;
            firstLightBarrierBefore = 1;
        }

    }
    else if(firstLightBarrierBefore == 1)
    {
        firstLightBarrierBefore = 0;
    }

    if(getSecondLightBarrier()->isBlocked == 1)
    {
        remainingTimeFirstTM = TRAVERSE_TIME_AFTER_SECOND_LB;
        if(isFirstPlateFree == 0 || getFirstPusher()->isBackTriggerActivated != 1)
        {
            isFirstTmStopped = 1;
        }
        else
        {
            isFirstTmStopped = 0;
        }
    }
}

void computeFirstPlate()
{
    if(!isFirstPlateFree)
    {
        if(getFirstPusher()->isBackTriggerActivated)
        {
            //TODO: Is second treadmill ready?
            firstPusherDir = 2; //forwards
        }
    }
    else if(getFirstPusher()->isBackTriggerActivated)
    {
        firstPusherDir = 1; //incative
    }
    if(getFirstPusher()->isFrontTriggerActivated)
    {
        firstPusherDir = 0; //backwards
        isFirstPlateFree = 1;
    }
}

void computeActions() {
    computeFirstTreadmill();
    computeFirstPlate();
}

void handleAktors() {
    Treadmill *firstTreadmill = getFirstTreadmill();
    if(remainingTimeFirstTM > 0 && isFirstTmStopped == 0)
    {
        startTreadmill(firstTreadmill);
    }
    else if(remainingTimeFirstTM <= 0 || isFirstTmStopped == 1)
    {
        stopTreadmill(firstTreadmill);
    }

    Pusher *firstPusher = getFirstPusher();
    switch(firstPusherDir)
    {
        case 0: runBackwardsPusher(firstPusher); break;
        case 1: stopPusher(firstPusher); break;
        case 2: runForwardPusher(firstPusher); break;
    }
}

void executeProgram() {
    aggregateSensors();
    computeActions();
    handleAktors();
}
