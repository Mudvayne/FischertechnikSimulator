#include "program.h"

void timeout(short stage)
{
	failure(stage * 10);
    setSiteState(PANIC_SWITCH, stage * 10);
}

void unexpectedItem(short lightBarrier)
{
    failure(lightBarrier);
    setSiteState(PANIC_SWITCH, lightBarrier);
}

void failure(short errorCode)
{
    printf("\n\n");
    switch(errorCode)
    {
        case 100: printf("panic switch activated!"); break;
        case 10: printf("Stage 1 expected an Item which never arrived!"); break;
        case 30: printf("Stage 3 expected an Item which never arrived!"); break;
        case 40: printf("Stage 4 expected an Item which never arrived!"); break;
        case 60: printf("Stage 6 expected an Item which never arrived!"); break;
        case 1:  printf("Unexpected Item in Light Barrier 1!"); break;
        case 2:  printf("Unexpected Item in Light Barrier 2!"); break;
        case 3:  printf("Unexpected Item in Light Barrier 3!"); break;
        case 4:  printf("Unexpected Item in Light Barrier 4!"); break;
        case 5:  printf("Unexpected Item in Light Barrier 5!"); break;
        default: printf("Unknown error code!"); break;
    }
}

void computeFirstTreadmill()
{
    //new items in stage?
    if(getFirstLightBarrier()->isBlocked == 1)
    {
        if(stageOne.firstLightBarrierBefore == 0)
        {
            stageOne.firstLightBarrierBefore = 1;
            // new item in LB 1
            totalSystem.itemsInSystem++;
            stageOne.itemCount++;
            short i = 0;
            for( i ; i < 3; i++)
            {
                if(stageOne.itemPositions[i] == -1)
                {
                    stageOne.itemPositions[i] = -2; //position will be set at second LB
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

    //has a item passed LB2?
    if(getSecondLightBarrier()->isBlocked == 0)
    {
        if(stageOne.secondLightBarrierBefore == 1)
        {
            // item left LB2
            stageOne.secondLightBarrierBefore = 0;
            stageOne.hasItemPassedSecondLB = 1;
        }
    }
    else
    {
        if(stageOne.secondLightBarrierBefore == 0)
        {
            //new item
            stageOne.secondLightBarrierBefore = 1;

            //update position to correct value
            short i = 0;
            for( i ; i < 3 ; i++)
            {
                if(stageOne.itemPositions[i] == -2)
                {
                    stageOne.itemPositions[i] = TM1_RUNTIME * LB2_POS;
                    break;
                }
            }
        }
    }

    //should it run?
    stageOne.isRunning = 0;
    if(stageOne.itemCount > 0)
    {
        stageOne.isRunning = 1;
        if(getSecondLightBarrier()->isBlocked && stageTwo.isReady == 0)
        {
            stageOne.isRunning = 0;
        }
    }

    //item left stage?
    short i = 0;
    for( i ; i < 3; i++)
    {
        if((stageOne.itemPositions[i] > TM1_RUNTIME) && stageOne.hasItemPassedSecondLB)
        {
            stageTwo.isOccupied = 1;
            stageOne.itemPositions[i] = -1;
            stageOne.itemCount --;
            stageOne.hasItemPassedSecondLB = 0;
            break;
        }
    }

    // item lost
    if(stageOne.isRunning)
    {
        stageOne.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageOne.timeout >= TIMEOUT)
    {
        timeout(1);
    }
    if(getSecondLightBarrier()->isBlocked || stageOne.itemCount == 0)
    {
        stageOne.timeout = 0;
    }

    // unexpected item
    if((stageOne.itemCount == 0 || stageOne.itemCount == 1 && stageOne.hasItemPassedSecondLB) && getSecondLightBarrier()->isBlocked)
    {
        unexpectedItem(2);
    }
}

void computeFirstPlate()
{
    //is stage ready?
    if(stageTwo.isOccupied || getFirstPusher()->isBackTriggerActivated != 1)
    {
        stageTwo.isReady = 0;
    }
    else
    {
        stageTwo.isReady = 1;
    }

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
        if(totalSystem.itemsInSystem - (stageOne.itemCount + stageThree.itemCount + stageFour.itemCount + stageFive.isOccupied + stageSix.itemCount) > 0) //do not "generate" items
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

        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageThree.itemPositions[i] == -1)
            {
                stageThree.itemPositions[i] = -2;
                break;
            }
        }
    }

    stageThree.isReady = 1;

    if(stageThree.itemCount > 0)
    {
        // RDY?
        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageThree.itemPositions[i] == -2)
            {
                stageThree.isReady = 0;
                break;
            }
        }

        // TM and TOOL
        if(stageThree.isToolTime)
        {
            if(stageThree.tMRuntimeLeftForCentering > 0)
            {
                stageThree.isTMRunning = 1;
            }
            else
            {
                if(stageThree.toolRuntimeLeft > 0)
                {
                    //Tooling
                    stageThree.isToolRunning = 1;
                    stageThree.isTMRunning = 0;
                }
                else
                {
                    //Done Tooltime
                    stageThree.isToolTime = 0;
                    stageThree.isToolRunning = 0;
                    stageThree.isTMRunning = 1;
                    stageThree.toolRuntimeLeft = 0;
                }

                //update position if not already done
                if(stageThree.updatedPosInToolTime == 0)
                {
                    short i = 0;
                    for( i ; i < 3 ; i++)
                    {
                        if(stageThree.itemPositions[i] == -2)
                        {
                            stageThree.itemPositions[i] = TM2_RUNTIME * TOOL_LB_POS;
                            break;
                        }
                    }
                    stageThree.updatedPosInToolTime = 1;
                }
            }
        }
        else
        {
            if(getThirdLightBarrier()->isBlocked)
            {
                if(stageThree.lightBarrierBefore == 0)
                {
                    //new item in light barrier
                    stageThree.lightBarrierBefore = 1;
                    stageThree.isToolTime = 1;
                    stageThree.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                    stageThree.toolRuntimeLeft = MILL_TIME;
                    stageThree.updatedPosInToolTime = 0;
                }
            }
            else
            {
                if(stageThree.lightBarrierBefore = 1)
                {
                    //item leaves light barrier
                    stageThree.hasItemPassedLightBarrier = 1;
                }
                // light barrier not blocked
                stageThree.lightBarrierBefore = 0;
            }
            stageThree.isTMRunning = 1;
        }
    }
    else
    {
        stageThree.isTMRunning = 0;
        stageThree.isToolTime = 0;
        stageThree.toolRuntimeLeft = 0;
        stageThree.tMRuntimeLeftForCentering = 0;
    }

    //should it wait for next station or traverse item?
    short i = 0;
    for( i ; i < 3 ; i++)
    {
        //wait for next stage?
        if((stageThree.itemPositions[i] >= (TM2_RUNTIME * TOOL_STAGE_WAIT_POS)) && stageFour.isReady == 0)
        {
            stageThree.isTMRunning = 0;
            break;
        }

        //does item leave stage?
        if(stageThree.itemPositions[i] > TM2_RUNTIME)
        {
            stageThree.itemCount--;
            stageThree.itemCountBefore--;
            stageThree.itemPositions[i] = -1;
            stageFour.itemCount++;
            stageThree.hasItemPassedLightBarrier = 0;
        }
    }

    // item lost
    if(stageThree.isTMRunning)
    {
        stageThree.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageThree.timeout >= TIMEOUT)
    {
        timeout(3);
    }
    short j = 0;
    short isItemInFrontOfLB = 0;
    for( j ; j < 3 ; j++)
    {
        if(stageThree.itemPositions[j] == -2)
        {
            isItemInFrontOfLB = 1;
            break;
        }
    }
    if(getThirdLightBarrier()->isBlocked || isItemInFrontOfLB == 0)
    {
        stageThree.timeout = 0;
    }

    // unexpected item
    if(stageThree.itemCount == 0 && getThirdLightBarrier()->isBlocked)
    {
        unexpectedItem(3);
    }
}

void computeThirdTreadmill()
{
    // check if new item
    if(stageFour.itemCount > stageFour.itemCountBefore)
    {
        // new item
        stageFour.itemCountBefore = stageFour.itemCount;

        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageFour.itemPositions[i] == -1)
            {
                stageFour.itemPositions[i] = -2;
                break;
            }
        }
    }

    stageFour.isReady = 1;

    if(stageFour.itemCount > 0)
    {
        // RDY?
        short i = 0;
        for( i ; i < 3 ; i++)
        {
            if(stageFour.itemPositions[i] == -2)
            {
                stageFour.isReady = 0;
                break;
            }
        }

        // TM and TOOL
        if(stageFour.isToolTime)
        {
            if(stageFour.tMRuntimeLeftForCentering > 0)
            {
                stageFour.isTMRunning = 1;
            }
            else
            {
                if(stageFour.toolRuntimeLeft > 0)
                {
                    //Tooling
                    stageFour.isToolRunning = 1;
                    stageFour.isTMRunning = 0;
                }
                else
                {
                    //Done Tooltime
                    stageFour.isToolTime = 0;
                    stageFour.isToolRunning = 0;
                    stageFour.isTMRunning = 1;
                    stageFour.toolRuntimeLeft = 0;
                }

                //update position if not already done
                if(stageFour.updatedPosInToolTime == 0)
                {
                    short i = 0;
                    for( i ; i < 3 ; i++)
                    {
                        if(stageFour.itemPositions[i] == -2)
                        {
                            stageFour.itemPositions[i] = TM3_RUNTIME * TOOL_LB_POS;
                            break;
                        }
                    }
                    stageFour.updatedPosInToolTime = 1;
                }
            }
        }
        else
        {
            if(getFourthLightBarrier()->isBlocked)
            {
                if(stageFour.lightBarrierBefore == 0)
                {
                    //new item in light barrier
                    stageFour.lightBarrierBefore = 1;
                    stageFour.isToolTime = 1;
                    stageFour.tMRuntimeLeftForCentering = TRAVERSE_TIME_AFTER_CENTERED_LB;
                    stageFour.toolRuntimeLeft = MILL_TIME;
                    stageFour.updatedPosInToolTime = 0;
                }
            }
            else
            {
                if(stageFour.lightBarrierBefore = 1)
                {
                    //item leaves light barrier
                    stageFour.hasItemPassedLightBarrier = 1;
                }
                // light barrier not blocked
                stageFour.lightBarrierBefore = 0;
            }
            stageFour.isTMRunning = 1;
        }
    }
    else
    {
        stageFour.isTMRunning = 0;
        stageFour.isToolTime = 0;
        stageFour.toolRuntimeLeft = 0;
        stageFour.tMRuntimeLeftForCentering = 0;
    }

    //should it wait for next station or traverse item?
    short i = 0;
    for( i ; i < 3 ; i++)
    {
        //wait for next stage?
        if((stageFour.itemPositions[i] >= (TM3_RUNTIME * TOOL_STAGE_WAIT_POS)) && stageFive.isReady == 0)
        {
            stageFour.isTMRunning = 0;
            break;
        }

        //does item leave stage?
        if(stageFour.itemPositions[i] > TM3_RUNTIME)
        {
            stageFour.itemCount--;
            stageFour.itemCountBefore--;
            stageFour.itemPositions[i] = -1;
            stageFive.isOccupied = 1;
        }
    }

    // item lost
    if(stageFour.isTMRunning)
    {
        stageFour.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageFour.timeout >= TIMEOUT)
    {
        timeout(4);
    }
    short j = 0;
    short isItemInFrontOfLB = 0;
    for( j ; j < 3 ; j++)
    {
        if(stageFour.itemPositions[j] == -2)
        {
            isItemInFrontOfLB = 1;
            break;
        }
    }
    if(getFourthLightBarrier()->isBlocked || isItemInFrontOfLB == 0)
    {
        stageFour.timeout = 0;
    }

    // unexpected item
    if(stageFour.itemCount == 0 && getFourthLightBarrier()->isBlocked)
    {
        unexpectedItem(4);
    }
}

void computeSecondPlate()
{
    // is stage ready?
    if(stageFive.isOccupied || getSecondPusher()->isBackTriggerActivated != 1)
    {
        stageFive.isReady = 0;
    }
    else
    {
        stageFive.isReady = 1;
    }

    if(stageFive.isOccupied && stageSix.isFull == 0)
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

void computeFourthTredmill()
{
    stageSix.isRunning = 1;

    if(stageSix.itemCount > 0)
    {
        if(stageSix.lightBarrierBlockedTime >= STAGE_SIX_LB_BLOCK_FOR_FULL)
        {
            //stage is full
            if(stageSix.isChecking == 0)
            {
                stageSix.timeLeftForNextEmptyCheck = STAGE_SIX_CHECK_INTERVAL;
                stageSix.tMRuntimeLeftForChecking = STAGE_SIX_RUN_TM_FOR_CHECK_TIME;
            }
            stageSix.isFull = 1;
            stageSix.isChecking = 1;
        }
        else
        {
            stageSix.isFull = 0;
        }
    }
    else
    {
        stageSix.isRunning = 0;
    }

    if(stageSix.isChecking)
    {
        if(stageSix.timeLeftForNextEmptyCheck > 0)
        {
            stageSix.timeLeftForNextEmptyCheck -= totalSystem.timeDiffSinceLastCall;
            stageSix.isRunning = 0;
        }
        else
        {
            //we are in the middle of a check
            if(stageSix.tMRuntimeLeftForChecking < 0)
            {
                stageSix.isRunning = 0;
                stageSix.isChecking = 0;
                stageSix.tMRuntimeLeftForChecking = 0;
                stageSix.timeLeftForNextEmptyCheck = STAGE_SIX_CHECK_INTERVAL;
            }
            else
            {
                stageSix.tMRuntimeLeftForChecking -= totalSystem.timeDiffSinceLastCall;
            }
        }
    }

    //light barrier toggled and blocked time?
    if(getFifthLightBarrier()->isBlocked == 1)
    {
        stageSix.lightBarrierBlockedTime += totalSystem.timeDiffSinceLastCall;
        if(stageSix.lightBarrierBlockedTime >= STAGE_SIX_LB_BLOCK_FOR_FULL)
        {
            stageSix.lightBarrierBlockedTime = STAGE_SIX_LB_BLOCK_FOR_FULL;
        }
        if(stageSix.lightBarrierBefore == 0)
        {
            stageSix.lightBarrierBefore = 1;
        }
    }
    else if(stageSix.lightBarrierBefore == 1)
    {
        stageSix.isFull = 0;
        stageSix.lightBarrierBlockedTime = 0;
        stageSix.lightBarrierBefore = 0;
        //toggled to off
        stageSix.itemCount--;
        totalSystem.itemsInSystem--;
        stageSix.wasFullBefore = 0;
    }

    // item lost
    if(stageSix.isRunning)
    {
        stageSix.timeout += totalSystem.timeDiffSinceLastCall;
    }
    if(stageSix.timeout >= TIMEOUT)
    {
        timeout(6);
    }
    if(getFifthLightBarrier()->isBlocked || stageSix.itemCount == 0)
    {
        stageSix.timeout = 0;
    }

    // unexpected item
    if(stageSix.itemCount == 0 && getFifthLightBarrier()->isBlocked)
    {
        unexpectedItem(6);
    }
}

void runningHandleActors() {
    Treadmill *treadmills[4] = {getFirstTreadmill(), getSecondTreadmill(), getThirdTreadmill(), getFourthTreadmill()};
    Tool *tools[2] = {getFirstTool(), getSecondTool()};
    Pusher *pushers[2] = {getFirstPusher(), getSecondPusher()};

    //first stage
    if(stageOne.isRunning)
    {
        startTreadmill(treadmills[0]);
        short i = 0;
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
    switch(stageTwo.pusherDir)
    {
        case 0: runBackwardsPusher(pushers[0]); break;
        case 1: stopPusher(pushers[0]); break;
        case 2: runForwardPusher(pushers[0]); break;
    }

    //third stage
    if(stageThree.isTMRunning)
    {
        startTreadmill(treadmills[1]);
        short i = 0;
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
        short i = 0;
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

    //fifth stage
    switch(stageFive.pusherDir)
    {
        case 0: runBackwardsPusher(pushers[1]); break;
        case 1: stopPusher(pushers[1]); break;
        case 2: runForwardPusher(pushers[1]); break;
    }

    //sixth stage
    if(stageSix.isRunning)
    {
        startTreadmill(getFourthTreadmill());
    }
    else
    {
        stopTreadmill(getFourthTreadmill());
    }
}

void runningComputeActions() {
    computeFirstTreadmill();
    computeFirstPlate();
    computeSecondTreadmill();
    computeThirdTreadmill();
    computeSecondPlate();
    computeFourthTredmill();
}

void runningInit() {
    totalSystem.itemsInSystem = 0;
    totalSystem.timeDiffSinceLastCall = 0;

    stageOne.isRunning = 0;
    stageOne.firstLightBarrierBefore = 0;
    stageOne.itemCount = 0;
    stageOne.itemPositions[0] = -1;
    stageOne.itemPositions[1] = -1;
    stageOne.itemPositions[2] = -1;
    stageOne.timeout = 0;
    stageOne.hasItemPassedSecondLB = 0;

    stageTwo.pusherDir = 1;
    stageTwo.isOccupied = 0;
    stageTwo.isReady = 1;

    stageThree.isReady = 1;
    stageThree.itemPositions[0] = -1;
    stageThree.itemPositions[1] = -1;
    stageThree.itemPositions[2] = -1;
    stageThree.itemCount = 0;
    stageThree.itemCountBefore = 0;
    stageThree.lightBarrierBefore = 0;
    stageThree.isToolTime = 0;
    stageThree.tMRuntimeLeftForCentering = 0;
    stageThree.toolRuntimeLeft = 0;
    stageThree.isTMRunning = 0;
    stageThree.isToolRunning = 0;
    stageThree.timeout = 0;
    stageThree.hasItemPassedLightBarrier = 0;
    stageThree.updatedPosInToolTime = 0;

    stageFour.isReady = 1;
    stageFour.itemPositions[0] = -1;
    stageFour.itemPositions[1] = -1;
    stageFour.itemPositions[2] = -1;
    stageFour.itemCount = 0;
    stageFour.itemCountBefore = 0;
    stageFour.lightBarrierBefore = 0;
    stageFour.isToolTime = 0;
    stageFour.tMRuntimeLeftForCentering = 0;
    stageFour.toolRuntimeLeft = 0;
    stageFour.isTMRunning = 0;
    stageFour.isToolRunning = 0;
    stageFour.timeout = 0;
    stageFour.hasItemPassedLightBarrier = 0;
    stageFour.updatedPosInToolTime = 0;

    stageFive.pusherDir = 1;
    stageFive.isOccupied = 0;
    stageFive.isReady = 1;

    stageSix.itemCount = 0;
    stageSix.isRunning = 0;
    stageSix.lightBarrierBefore = 0;
    stageSix.lightBarrierBlockedTime = 0;
    stageSix.timeLeftForNextEmptyCheck = -1;
    stageSix.tMRuntimeLeftForChecking = 0;
    stageSix.isFull = 0;
    stageSix.wasFullBefore = 0;
    stageSix.timeout = 0;
    stageSix.isChecking = 0;
}
