#include "program.h"
#include "model.h"
#include "treadmill.h"
#include "lightBarrier.h"
#include "timeUtil.h"

#define TRAVERSE_TIME_AFTER_SECOND_LB               1500
#define TRAVERSE_TIME_AFTER_THIRD_AND_FOURTH_LB     3000
#define TRAVERSE_TIME_AFTER_CENTERED_LB             500
#define WAIT_BEFORE_TOOLING                         500
#define WAIT_AFTER_TOOLING                          500
#define TOOL_TIME                                   3000
#define TIMEOUT                                     10000

//************************************
//****** VARIABLES FOR AUTOMAT *******
//************************************

//SYSTEM
int itemsInSystem = 0;
long long timeDiffSinceLastCall = 0;

//STAGE 1 (TREADMILL 1)
int remainingTimeFirstTM = 0;
int itemCountFirstTM = 0;
int isFirstTmStopped = 0;
int firstLightBarrierBefore = 0;

//STAGE 2 (PLATE 1)
int firstPusherDir = 1;
int isItemOnFirstPlate = 0;

//STAGE 3 (TREADMILL 2)
int remainingTimeSecondTM = 0;
int itemCountSecondTM = 0;
int itemCountSecondTMBefore = 0;
int isSecondTmStopped = 0;
int remainingTimeFirstTool = -1000;
int isFirstToolActive = 0;
int thirdLightBarrierBefore = 0;
int thirdLightBarrierBlockTime = 0;

//STAGE 4 (TREADMILL 3)
int itemCountThirdTM = 0;
int isThirdTmStopped = 0;
int remainingTimeSecondTool = 0;
int isSecondToolActive = 0;

//STAGE 5 (PLATE 2)
int secondPusherDir = 1;
int isItemOnSecondPlate = 0;

//STAGE 6 (TREADMILL 4)
int isFourthTmStopped = 0;
int itemCountFourthTM = 0;


void aggregateSensors() {
    timeDiffSinceLastCall = calculateTimeDiffSinceLastCall();
}

void computeFirstTreadmill()
{
    if(itemCountFirstTM > 0 && remainingTimeFirstTM <= 0)
    {
        itemCountFirstTM --;
        isItemOnFirstPlate = 1;
    }

    if(getFirstLightBarrier()->isBlocked == 1)
    {
        if(firstLightBarrierBefore == 0)
        {
            remainingTimeFirstTM = TIMEOUT;
            itemCountFirstTM ++;
            itemsInSystem ++;
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
        if(isItemOnFirstPlate == 1 || getFirstPusher()->isBackTriggerActivated != 1)
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
    if(isItemOnFirstPlate)
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
        isItemOnFirstPlate = 0;
        if(itemsInSystem - (itemCountFirstTM + itemCountSecondTM + itemCountThirdTM + itemCountFourthTM + isItemOnFirstPlate + isItemOnSecondPlate) > 0)
        {
            itemCountSecondTM++;
        }
    }
}

void computeSecondTreadmill()
{
    printf("ITEMS: %d, TIMETOSTOP: %d, TOOLTIME: %d, TOOLACTIVE: %d, ITEMSINSYS: %d                  ", itemCountSecondTM, remainingTimeSecondTM, remainingTimeFirstTool, isSecondToolActive, itemsInSystem);

    if(itemCountSecondTM > 0)
    {
        if(itemCountSecondTM > itemCountSecondTMBefore)
        {
            //new item right now!
            itemCountSecondTMBefore = itemCountSecondTM;
            remainingTimeSecondTM = TIMEOUT;
        }
        if(getThirdLightBarrier()->isBlocked)
        {
            thirdLightBarrierBlockTime += timeDiffSinceLastCall;
            if(thirdLightBarrierBefore == 0)
            {
                //switched on right now!
                thirdLightBarrierBefore = 1;
                remainingTimeSecondTM = TRAVERSE_TIME_AFTER_CENTERED_LB;
            }
        }
        else
        {
            thirdLightBarrierBlockTime = 0;
            if(thirdLightBarrierBefore == 1)
            {
                //switched off right now!
                thirdLightBarrierBefore = 0;
            }
        }
        if(thirdLightBarrierBlockTime >= TRAVERSE_TIME_AFTER_CENTERED_LB + WAIT_BEFORE_TOOLING)
        {
            if(remainingTimeFirstTool<-900)
            {
                //start tooling
                remainingTimeFirstTool = TOOL_TIME;
            }
            else
            {

            }
        }
    }
}

void computeActions() {
    computeFirstTreadmill();
    computeFirstPlate();
    computeSecondTreadmill();
}

void handleAktors() {
    Treadmill *treadmills[4] = {getFirstTreadmill(), getSecondTreadmill(), getThirdTreadmill(), getFourthTreadmill()};
    Tool *tools[2] = {getFirstTool(), getSecondTool()};

    //first stage
    if(remainingTimeFirstTM > 0 && isFirstTmStopped == 0)
    {
        startTreadmill(treadmills[0]);
        remainingTimeFirstTM -= timeDiffSinceLastCall;
    }
    else if(remainingTimeFirstTM <= 0 || isFirstTmStopped == 1)
    {
        stopTreadmill(treadmills[0]);
    }

    //second stage
    Pusher *firstPusher = getFirstPusher();
    switch(firstPusherDir)
    {
        case 0: runBackwardsPusher(firstPusher); break;
        case 1: stopPusher(firstPusher); break;
        case 2: runForwardPusher(firstPusher); break;
    }

    //third stage
    if(remainingTimeSecondTM > 0)
    {
        startTreadmill(treadmills[1]);
        remainingTimeSecondTM -= timeDiffSinceLastCall;
    }
    else
    {
        stopTreadmill(treadmills[1]);
    }

    if(remainingTimeFirstTool > 0)
    {
        startTool(tools[0]);
        remainingTimeFirstTool -= timeDiffSinceLastCall;
    }
    else
    {
        stopTool(tools[0]);
    }
}

void executeProgram() {
    aggregateSensors();
    computeActions();
    handleAktors();
}
