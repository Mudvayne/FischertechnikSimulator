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
#define TRAVERSE_TIME_AFTER_CENTERED_LB             50
#define TOOL_TIME                                   3000
#define TIMEOUT                                     10000

#else

//must be adjusted
#define TRAVERSE_TIME_ONE_TM                        5000
#define RUN_LONGER_THAN_THEORETICALLY_NEEDED        1000
#define EMPTY_SPACE_TO_BE_READY                     1200
#define TRAVERSE_TIME_AFTER_CENTERED_LB             700
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
} PusherState;

typedef struct
{
    short itemCount;
    short itemCountBefore;
    int itemPositions[3];
    short isReady;
    short lightBarrierBefore;
    short isToolTime;
    short tMRuntimeLeftForCentering;
    short toolRuntimeLeft;
    short isTMRunning;
    short isToolRunning;
} ToolStage;

typedef struct
{
    short isReady;
    short itemCount;
} StageSix;

TotalSystem totalSystem = {.itemsInSystem = 0, .timeDiffSinceLastCall = 0};
StageOne stageOne = {.isRunning = 0, .firstLightBarrierBefore = 0, .isReady = 1, .itemCount = 0, .itemPositions = {-1, -1, -1}};
PusherState stageTwo = {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
ToolStage stageThree = {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0, .itemCountBefore = 0,
                         .lightBarrierBefore = 0, .isToolTime = 0, .tMRuntimeLeftForCentering = 0, .toolRuntimeLeft = 0,
                         .isTMRunning = 0, .isToolRunning = 0};
ToolStage stageFour = {.isReady = 1, .itemPositions = {-1, -1, -1}, .itemCount = 0, .itemCountBefore = 0,
                         .lightBarrierBefore = 0, .isToolTime = 0, .tMRuntimeLeftForCentering = 0, .toolRuntimeLeft = 0,
                         .isTMRunning = 0, .isToolRunning = 0};
PusherState stageFive = {.pusherDir = 1, .isOccupied = 0, .isReady = 1};
StageSix stageSix = {.isReady = 1, .itemCount = 0};

//STAGE 3 (TREADMILL 2)

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
    /*printf("items: %d, %d, %d                   ", stageOne.itemPositions[0], stageOne.itemPositions[1], stageOne.itemPositions[2]);
    printf("\nitem count: %d                   ", stageOne.itemCount);
    printf("\nisRunning: %d                   ", stageOne.isRunning);
    printf("\nisReady: %d                   ", stageOne.isReady);*/
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
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageThree.itemCount + stageFour.itemCount + stageFive.isOccupied + stageSix.itemCount) > 0)
        {
            stageThree.itemCount++;
        }
    }
}

void computeSecondTreadmill()
{
    // check if new item
    if(stageThree.itemCount > stageThree.itemCountBefore)
    {
        // new item
        stageThree.itemCountBefore = stageThree.itemCount;

        int i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageThree.itemPositions[i] == -1)
            {
                stageThree.itemPositions[i] = 0;
                break;
            }
        }
    }

    if(stageThree.itemCount > 0)
    {
        if(stageThree.isToolTime)
        {
            if(stageThree.tMRuntimeLeftForCentering <= 0)
            {
                //centered
                stageThree.isTMRunning = 0;
                if(stageThree.toolRuntimeLeft > 0)
                {
                    stageThree.isToolRunning = 1;
                }
                else
                {
                    //done with tooltime
                    stageThree.isToolTime = 0;
                    stageThree.isToolRunning = 0;
                    stageThree.toolRuntimeLeft = 0;
                    stageThree.isTMRunning = 1;
                    stageThree.tMRuntimeLeftForCentering = 0;
                }
            }
        }
        else
        {
            int i = 0;
            for( i ; i < 3 ; i++)
            {
                // check if ready
                if((stageThree.itemPositions[i] >= 0 && stageThree.itemPositions <= EMPTY_SPACE_TO_BE_READY) || stageThree.itemCount >= 3)
                {
                    stageThree.isReady = 0;
                }
                else
                {
                    stageThree.isReady = 1;
                }

                // check if tool time
                if(getThirdLightBarrier()->isBlocked)
                {
                    if(stageThree.lightBarrierBefore == 0)
                    {
                        stageThree.lightBarrierBefore = 1;
                        stageThree.isToolTime = 1;
                        stageThree.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                        stageThree.toolRuntimeLeft = TOOL_TIME;
                        break;
                    }
                }
                else
                {
                    stageThree.lightBarrierBefore = 0;
                    stageThree.isTMRunning = 1;
                }

                // check if item leaves
                if(stageThree.itemPositions[i] > (TRAVERSE_TIME_ONE_TM + RUN_LONGER_THAN_THEORETICALLY_NEEDED))
                {
                    stageThree.itemCount--;
                    stageThree.itemCountBefore--;
                    stageThree.itemPositions[i] = -1;

                    // TODO: wait if next stage is not ready
                    stageFour.itemCount++;
                }
            }
        }
    }
    else
    {
        stageThree.isTMRunning = 0;
    }
}

void computeThirdTreadmill()
{
    // check if new item
    if(stageFour.itemCount > stageFour.itemCountBefore)
    {
        // new item
        stageFour.itemCountBefore = stageFour.itemCount;

        int i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageFour.itemPositions[i] == -1)
            {
                stageFour.itemPositions[i] = 0;
                break;
            }
        }
    }

    if(stageFour.itemCount > 0)
    {
        if(stageFour.isToolTime)
        {
            if(stageFour.tMRuntimeLeftForCentering <= 0)
            {
                //centered
                stageFour.isTMRunning = 0;
                if(stageFour.toolRuntimeLeft > 0)
                {
                    stageFour.isToolRunning = 1;
                }
                else
                {
                    //done with tooltime
                    stageFour.isToolTime = 0;
                    stageFour.isToolRunning = 0;
                    stageFour.toolRuntimeLeft = 0;
                    stageFour.isTMRunning = 1;
                    stageFour.tMRuntimeLeftForCentering = 0;
                }
            }
        }
        else
        {
            int i = 0;
            for( i ; i < 3 ; i++)
            {
                // check if ready
                if((stageFour.itemPositions[i] >= 0 && stageFour.itemPositions <= EMPTY_SPACE_TO_BE_READY) || stageFour.itemCount >= 3)
                {
                    stageFour.isReady = 0;
                }
                else
                {
                    stageFour.isReady = 1;
                }

                // check if tool time
                if(getFourthLightBarrier()->isBlocked)
                {
                    if(stageFour.lightBarrierBefore == 0)
                    {
                        stageFour.lightBarrierBefore = 1;
                        stageFour.isToolTime = 1;
                        stageFour.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                        stageFour.toolRuntimeLeft = TOOL_TIME;
                        break;
                    }
                }
                else
                {
                    stageFour.lightBarrierBefore = 0;
                    stageFour.isTMRunning = 1;
                }

                // check if item leaves
                if(stageFour.itemPositions[i] > (TRAVERSE_TIME_ONE_TM + RUN_LONGER_THAN_THEORETICALLY_NEEDED))
                {
                    stageFour.itemCount--;
                    stageFour.itemCountBefore--;
                    stageFour.itemPositions[i] = -1;

                    // TODO: give it to next stage
                    stageFive.isOccupied = 1;
                }
            }
        }
    }
    else
    {
        stageFour.isTMRunning = 0;
    }
    //printf("\n\nITEMS: %d, %d, %d          \nCOUNT: %d        \nTM_LEFT: %d, TOOL_LEFT: %d       ", stageFour.itemPositions[0], stageFour.itemPositions[1], stageFour.itemPositions[2], stageFour.itemCount, stageFour.tMRuntimeLeftForCentering, stageFour.toolRuntimeLeft);
}

void computeSecondPlate()
{
    if(stageFive.isOccupied && stageThree.isReady)
    {
        if(getSecondPusher()->isBackTriggerActivated)
        {
            stageFive.pusherDir = 2; //forwards
        }
    }
    else if(getSecondPusher()->isBackTriggerActivated)
    {
        stageFive.pusherDir = 1; //incative
    }

    if(getSecondPusher()->isFrontTriggerActivated)
    {
        stageFive.pusherDir = 0; //backwards
        stageFive.isOccupied = 0;
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageTwo.isOccupied + stageThree.itemCount + stageFour.itemCount + stageSix.itemCount) > 0)
        {
            stageSix.itemCount++;
        }
    }
}


void computeActions() {
    computeFirstTreadmill();
    computeFirstPlate();
    computeSecondTreadmill();
    computeThirdTreadmill();
    computeSecondPlate();
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

    //fifth stage
    Pusher *secondPusher = getSecondPusher();
    switch(stageFive.pusherDir)
    {
        case 0: runBackwardsPusher(secondPusher); break;
        case 1: stopPusher(secondPusher); break;
        case 2: runForwardPusher(secondPusher); break;
    }

    //third stage
    if(stageThree.isTMRunning)
    {
        startTreadmill(treadmills[1]);
        int i = 0;
        for(i; i < 3; i++)
        {
            if(stageThree.itemPositions[i] > -1)
            {
                stageThree.itemPositions[i] += totalSystem.timeDiffSinceLastCall;
            }
        }
        if(stageThree.isToolTime)
        {
            stageThree.tMRuntimeLeftForCentering -= totalSystem.timeDiffSinceLastCall;
        }
    }
    else
    {
        stopTreadmill(treadmills[1]);
    }

    if(stageThree.isToolRunning)
    {
        startTool(tools[0]);
        stageThree.toolRuntimeLeft -= totalSystem.timeDiffSinceLastCall;
    }
    else
    {
        stopTool(tools[0]);
    }

    //fourth stage
    if(stageFour.isTMRunning)
    {
        startTreadmill(treadmills[2]);
        int i = 0;
        for(i; i < 3; i++)
        {
            if(stageFour.itemPositions[i] > -1)
            {
                stageFour.itemPositions[i] += totalSystem.timeDiffSinceLastCall;
            }
        }
        if(stageFour.isToolTime)
        {
            stageFour.tMRuntimeLeftForCentering -= totalSystem.timeDiffSinceLastCall;
        }
    }
    else
    {
        stopTreadmill(treadmills[2]);
    }

    if(stageFour.isToolRunning)
    {
        startTool(tools[1]);
        stageFour.toolRuntimeLeft -= totalSystem.timeDiffSinceLastCall;
    }
    else
    {
        stopTool(tools[1]);
    }
}

void executeProgram() {
    aggregateSensors();
    computeActions();
    printf("ITEM COUNT SYSTEM: %d", totalSystem.itemsInSystem);
    printf("\nSTAGE 1: items = %d, pos = %d %d %d, running = %d, ready = %d \t", stageOne.itemCount, stageOne.itemPositions[0], stageOne.itemPositions[1], stageOne.itemPositions[2], stageOne.isRunning, stageOne.isReady);
    printf("\nSTAGE 2: occupied = %d, dir = %d,  ready = %d\t\t", stageTwo.isOccupied, stageTwo.pusherDir, stageTwo.isReady);
    printf("\nSTAGE 3: items = %d, pos = %d %d %d, TM = %d, toolLeft = %d, ready = %d    ", stageThree.itemCount, stageThree.itemPositions[0], stageThree.itemPositions[1], stageThree.itemPositions[2], stageThree.isTMRunning, stageThree.toolRuntimeLeft, stageThree.isReady);
    printf("\nSTAGE 4: items = %d, pos = %d %d %d, TM = %d, toolLeft = %d, ready = %d    ", stageFour.itemCount, stageFour.itemPositions[0], stageFour.itemPositions[1], stageFour.itemPositions[2], stageFour.isTMRunning, stageFour.toolRuntimeLeft, stageFour.isReady);
    printf("\nSTAGE 5: occupied = %d, dir = %d, ready = %d\t\t", stageFive.isOccupied, stageFive.pusherDir, stageFive.isReady);
    printf("\nSTAGE 6: items = %d", stageSix.itemCount);
    handleAktors();
}
