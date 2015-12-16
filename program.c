#include "program.h"
#include "model.h"
#include "treadmill.h"
#include "lightBarrier.h"
#include "timeUtil.h"

#ifdef ON_TARGET

//must be adjusted
#define TRAVERSE_TIME_ONE_TM                        1500
#define RUN_LONGER_THAN_THEORETICALLY_NEEDED        500
#define EMPTY_SPACE_TO_BE_READY                     1200

#define TRAVERSE_TIME_AFTER_THIRD_AND_FOURTH_LB     4000
#define TRAVERSE_TIME_AFTER_CENTERED_LB             50
#define WAIT_BEFORE_TOOLING                         500
#define WAIT_AFTER_TOOLING                          500
#define TOOL_TIME                                   3000
#define TIMEOUT                                     10000

#else

//must be adjusted
#define TRAVERSE_TIME_ONE_TM                        7000
#define RUN_LONGER_THAN_THEORETICALLY_NEEDED        1000
#define EMPTY_SPACE_TO_BE_READY                     1200

#define TRAVERSE_TIME_AFTER_THIRD_AND_FOURTH_LB     4000
#define TRAVERSE_TIME_AFTER_CENTERED_LB             700
#define WAIT_BEFORE_TOOLING                         500
#define WAIT_AFTER_TOOLING                          500
#define TOOL_TIME                                   3000
#define TIMEOUT                                     10000

#endif

//******************************************
//****** VARIABLES FOR STATE MACHINE *******
//******************************************

typedef struct
{
    short itemsInSystem;
    long long timeDiffSinceLastCall;
} TotalSystem;

typedef struct
{
    short itemCount;
    int itemPositions[3];
    short firstLightBarrierBefore;
    short secondLightBarrierBefore;

    short isRunning;
    short isReady;
} StageOne;

typedef struct
{
    short pusherDir;
    short isOccupied;
    short isReady;
} StageTwo;

typedef struct
{
    short itemCount;
    int itemPositions[3];

    short isReady;
} StageThree;

TotalSystem totalSystem = {.itemsInSystem = 0, .timeDiffSinceLastCall = 0};
StageOne stageOne = {.isRunning = 0, .firstLightBarrierBefore = 0, .isReady = 1, .itemCount = 0, .itemPositions = {-1, -1, -1}};
StageTwo stageTwo = {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
StageThree stageThree = {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0};


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
    totalSystem.timeDiffSinceLastCall = calculateTimeDiffSinceLastCall();

    updatePusher(getFirstPusher());
    updatePusher(getSecondPusher());

    updateLightBarrier(getFirstLightBarrier());
    updateLightBarrier(getSecondLightBarrier());
    updateLightBarrier(getThirdLightBarrier());
    updateLightBarrier(getFourthLightBarrier());
    updateLightBarrier(getFifthLightBarrier());
}

void computeFirstTreadmill()
{
		#ifndef ON_TARGET
    // debug
    printf("items: %d, %d, %d                   ", stageOne.itemPositions[0], stageOne.itemPositions[1], stageOne.itemPositions[2]);
    printf("\nitem count: %d                   ", stageOne.itemCount);
    printf("\nisRunning: %d                   ", stageOne.isRunning);
    printf("\nisReady: %d                   ", stageOne.isReady);
		#endif

    //new items in stage?
    if(getFirstLightBarrier()->isBlocked == 1)
    {
        if(stageOne.firstLightBarrierBefore == 0)
        {
            stageOne.firstLightBarrierBefore = 1;
            // new item in LB 1
            totalSystem.itemsInSystem++;
            stageOne.itemCount++;
            int i = 0;
            for( i ; i < 3; i++)
            {
                if(stageOne.itemPositions[i] == -1)
                {
                    stageOne.itemPositions[i] = 0;
                    break;
                }
            }
        }
    }
    else if(stageOne.firstLightBarrierBefore == 1)
    {
        //item left first light barrier
        stageOne.firstLightBarrierBefore = 0;
    }

    // is next stage ready?
    short isNextStageReady = 1;
    if(stageTwo.isOccupied || getFirstPusher()->isBackTriggerActivated != 1)
    {
        isNextStageReady = 0;
    }

    //is this stage ready for more items? (not really necessary in stage one, just a prototype for the next stages)
    int i = 0;
    if(stageOne.itemCount > 0)
    {
        for( i ; i < 3 ; i++)
        {
            int pos = stageOne.itemPositions[i];
            if(pos >= 0 && pos <= 0 + EMPTY_SPACE_TO_BE_READY)
            {
                stageOne.isReady = 0;
                break;
            }
            else
            {
                stageOne.isReady = 1;
            }
        }
    }
    else
    {
        stageOne.isReady = 1;
    }

    //should it run?
    stageOne.isRunning = 0;
    if(stageOne.itemCount > 0)
    {
        stageOne.isRunning = 1;
        i = 0;
        for( i ; i < 3 ; i++)
        {
            if(getSecondLightBarrier()->isBlocked && !isNextStageReady)
            {
                stageOne.isRunning = 0;
                break;
            }
        }
    }

    //item left stage?
    i = 0;
    for( i ; i < 3; i++)
    {
        if(stageOne.itemPositions[i] > (TRAVERSE_TIME_ONE_TM + RUN_LONGER_THAN_THEORETICALLY_NEEDED))
        {
            stageTwo.isOccupied = 1;
            stageOne.itemPositions[i] = -1;
            stageOne.itemCount --;
            break;
        }
    }
}

void computeFirstPlate()
{
    if(stageTwo.isOccupied && stageThree.isReady)
    {
        if(getFirstPusher()->isBackTriggerActivated)
        {
            stageTwo.pusherDir = 2; //forwards
        }
    }
    else if(getFirstPusher()->isBackTriggerActivated)
    {
        stageTwo.pusherDir = 1; //incative
    }

    if(getFirstPusher()->isFrontTriggerActivated)
    {
        stageTwo.pusherDir = 0; //backwards
        stageTwo.isOccupied = 0;
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageThree.itemCount + itemCountThirdTM + itemCountFourthTM + isItemOnSecondPlate) > 0)
        {
            //stageThree.itemCount++; new
            //todo: set item pos in stage 3

            itemCountSecondTM ++;
        }
    }
}

void computeSecondTreadmill()
{
    //printf("ITEMS: %d, TIMETOSTOP: %d, TOOLTIME: %d, TOOLACTIVE: %d, ITEMSINSYS: %d                  ", itemCountSecondTM, remainingTimeSecondTM, remainingTimeFirstTool, isSecondToolActive, totalSystem.itemsInSystem);

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
            thirdLightBarrierBlockTime += totalSystem.timeDiffSinceLastCall;
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
                remainingTimeFirstTool = -1000;
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
                if(remainingTimeFirstTool <= 0)
                {
                    remainingTimeSecondTM = TRAVERSE_TIME_AFTER_THIRD_AND_FOURTH_LB;
                }
            }
        }
        if(remainingTimeSecondTM <= 0)
        {
            if(totalSystem.itemsInSystem - (stageOne.itemCount + itemCountSecondTM + itemCountThirdTM + itemCountFourthTM + isItemOnSecondPlate) > 0)
            {
                itemCountThirdTM++;
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
    if(stageOne.isRunning)
    {
        startTreadmill(treadmills[0]);
        int i = 0;
        for(i; i < 3; i++)
        {
            if(stageOne.itemPositions[i] > -1)
            {
                stageOne.itemPositions[i] += totalSystem.timeDiffSinceLastCall;
            }
        }
    }
    else
    {
        stopTreadmill(treadmills[0]);
    }

    //second stage
    Pusher *firstPusher = getFirstPusher();
    switch(stageTwo.pusherDir)
    {
        case 0: runBackwardsPusher(firstPusher); break;
        case 1: stopPusher(firstPusher); break;
        case 2: runForwardPusher(firstPusher); break;
    }

    //third stage
    if(remainingTimeSecondTM > 0)
    {
        startTreadmill(treadmills[1]);
        remainingTimeSecondTM -= totalSystem.timeDiffSinceLastCall;
    }
    else
    {
        stopTreadmill(treadmills[1]);
    }

    if(remainingTimeFirstTool > 0)
    {
        startTool(tools[0]);
        remainingTimeFirstTool -= totalSystem.timeDiffSinceLastCall;
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
